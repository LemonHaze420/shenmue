#include "s2/areas/area_qrdk.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2::areas {
void QrdkInitNullsub() {
}

void QrdkMain(std::uintptr_t) {
    std::uint32_t count = 0;
    while (QRDK_ENEMIES[count].character.name != QRDK_INVALID.name)
        ++count;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QrdkInitializeCallback, 4, 0x0b, 84U + count * 4U, QRDK_TASK);
    if (!task)
        return;
    QrdkWork *const work = static_cast<QrdkWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    std::memset(static_cast<void *>(work), 0, static_cast<std::size_t>(84U + count * 4U));
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->enemyCount = count;
    QrdkHideCharacter(QRDK_RYO);
    SetMotionSpeed(scenFindMotionwork(QRDK_RYO), 1, MotionRegion::LowerBody);
    SetPlayerControlEnabled(0);
    HLib::SetupTaskDelay(-1);
}

void QrdkHideCharacter(shen_name character) {
    const bool hasMotion = scenFindMotionwork(character) != nullptr;
    if (hasMotion) {
        const Vector4 reflector{1, 1, 1, 1};
        SetScenarioReflectorBoardValues(character, &reflector);
        EV_SetRefBoardParamsFor(character, 1);
    }
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Disabled);
    if (hasMotion)
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}

void QrdkInitializeCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 3U) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags &= ~3U;
    } else {
        FinishLoading();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QrdkWaitPackageCallback);
    }
}

void QrdkWaitPackageCallback(void *) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("QRDK"))) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QRDK"), 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QRDK"), 0);
        HLib::SetCurrentTaskTo(QrdkLoadCharactersCallback);
    }
}

void QrdkLoadCharactersCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (!work->loadIndex) {
        for (std::size_t index = 0; QRDK_CHARACTERS[index].name != QRDK_INVALID.name; ++index)
            EV_CheckWarmStartNpcRoute(QRDK_CHARACTERS[index]);
        EV_ResetFreeTalkFlags();
        work->loadIndex = 1;
    } else {
        const shen_name character = QRDK_CHARACTERS[work->loadIndex - 1U];
        if (character.name == QRDK_INVALID.name) {
            --work->loadIndex;
            SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags |= 3U;
            HLib::SetCurrentTaskTo(QrdkBeginGameCallback);
        } else if (EV_ForceLoadNpc(character)) {
            QrdkHideCharacter(character);
            ++work->loadIndex;
        }
    }
}

void QrdkBeginGameCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    work->flags = (work->flags & ~2U) | 1U;
    EV_AT_SetPos(QRDK_REN, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{10000, 10000, 10000});
    HLib::SetCurrentTaskTo(QrdkStartRouteCallback);
}

const QrdkRoutePoint *QrdkGetRoutePoint(std::int32_t counter) {
    if (counter < 0) {
        std::int32_t start = 0, length = 1;
        while (!QRDK_ROUTE.data()[start].marker)
            ++start;
        while (!QRDK_ROUTE.data()[start + length].marker)
            ++length;
        do {
            counter += length + 1;
        } while (counter < 0);
    }
    std::int32_t index = 0;
    while (counter) {
        ++index;
        --counter;
        if (QRDK_ROUTE.data()[index].marker == 2) {
            index = 0;
            while (!QRDK_ROUTE.data()[index].marker)
                ++index;
        }
    }
    return &QRDK_ROUTE.data()[index];
}

void QrdkPlaceOnRoute(QrdkWork *work) {
    if (!work)
        return;
    Vector3 position;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const QrdkRoutePoint *const point = QrdkGetRoutePoint(work->routeCounter);
    position.x = point->x;
    position.z = point->z;
    const QrdkRoutePoint *const next = QrdkGetRoutePoint(static_cast<std::int32_t>(static_cast<std::uint32_t>(work->routeCounter) + 1U));
    float x = next->x - point->x, z = next->z - point->z;
    if (std::fabs(x) < 0.00000011920929f)
        x = 0;
    if (std::fabs(z) < 0.00000011920929f)
        z = 0;
    const std::uint16_t yaw = static_cast<std::uint16_t>(InverseFixedAtan2(x, z));
    CopyMotionTransition(scenFindMotionwork(QRDK_RYO), &position, yaw, 5, 0);
    CopyMotionTransition(scenFindMotionwork(QRDK_RYO), &position, yaw, 8, 0);
    CopyMotionTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 12, 0);
}

void QrdkStartRouteCallback(void *data) {
    if (data) {
        QrdkPlaceOnRoute(static_cast<QrdkWork *>(data));
        HLib::SetCurrentTaskTo(QrdkRunCallback);
    } else
        HLib::SetCurrentTaskTo(QrdkExitCallback);
}

std::uint32_t QrdkGetDoorPoint(shen_name door, Vector3 *position, std::uint32_t *yaw) {
    std::uint32_t result = QRDK_DOORS[0].door.name;
    for (std::size_t index = 0; result != QRDK_INVALID.name; result = QRDK_DOORS[++index].door.name) {
        const QrdkDoorPoint &point = QRDK_DOORS[index];
        if (result != door.name)
            continue;
        if (position) {
            Vector3 ryo;
            EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
            *position = {point.x, ryo.y, point.z};
            result = std::bit_cast<std::uint32_t>(point.z);
        }
        if (yaw)
            result = *yaw = point.yaw;
        return result;
    }
    return result;
}

shen_name QrdkFindClosestDoor(float *distance) {
    Vector3 ryo;
    Vector3i rotation;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(QRDK_RYO, &rotation, EventTransformFlags::World);
    shen_name result = QRDK_INVALID;
    float best = 10000;
    for (std::size_t index = 0; QRDK_DOORS[index].door.name != QRDK_INVALID.name; ++index) {
        Vector3 position;
        std::uint32_t yaw;
        QrdkGetDoorPoint(QRDK_DOORS[index].door, &position, &yaw);
        if (ComputeAngle(ryo, best, static_cast<std::int16_t>(rotation.y), 0x2000, position) &&
            ComputeAngle(position, 8, static_cast<std::int16_t>(yaw + 0x8000U), 10012, ryo)) {
            const float candidate = GetPlanarDistance(&ryo, &position);
            if (best > candidate) {
                result = QRDK_DOORS[index].door;
                best = candidate;
            }
        }
    }
    if (distance)
        *distance = best;
    return result;
}

void QrdkRunCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        CopyMotionTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 13, 0);
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (GetCurrentControlInput(INPUT_REPEAT) & 0x400U)
        work->action |= 1U;
    if (work->action & 1U) {
        float distance;
        const shen_name door = QrdkFindClosestDoor(&distance);
        if (door.name == QRDK_INVALID.name)
            return;
        if (distance < 5) {
            work->action &= ~1U;
            bool defeated = false, hasEnemies = false;
            for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
                if (QRDK_ENEMIES[index].door.name == door.name) {
                    if (work->defeated[index])
                        defeated = true;
                    hasEnemies = true;
                }
            }
            if (defeated && hasEnemies)
                QrdkQueueSerif("ここはまずいな", 1, 50, nullptr);
            else {
                work->door = door;
                EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &work->savedPosition, EventTransformFlags::World);
                GetScenarioCharacterEulerAngles(QRDK_RYO, &work->savedRotation, EventTransformFlags::World);
                if (mtwkSetExtraFlag(scenFindMotionwork(QRDK_RYO), 58, 1))
                    CopyMotionTransition(scenFindMotionwork(QRDK_RYO), nullptr, static_cast<std::uint16_t>(work->savedRotation.y), 13, 0);
                HLib::SetCurrentTaskTo(QrdkBeginDoorCallback);
                return;
            }
        }
    }
    if (!mtwkSetExtraFlag(scenFindMotionwork(QRDK_RYO), 58, 1)) {
        work->routeCounter = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->routeCounter) + 1U);
        QrdkPlaceOnRoute(work);
    }
    if (!(work->action & 1U))
        CheckHelpIcon(4, 0, CheckHelpIconOp::FrameOverlay);
}

void QrdkQueueSerif(const char *text, std::int32_t startTicks, std::int32_t endTicks, QrdkSerifCallback callback) {
    HLib::Task *const parent = HLib::GetCurrentTask();
    if (!text) {
        if (callback)
            callback(0, reinterpret_cast<std::uintptr_t>(parent));
        return;
    }
    const std::size_t length = std::strlen(text);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QrdkSerifCallbackLoop, 4, 0x0b, startTicks > 0 ? 40 + length : 39, QRDK_TASK);
    if (!task)
        return;
    QrdkSerifWork *const work = static_cast<QrdkSerifWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        if (callback)
            callback(0, reinterpret_cast<std::uintptr_t>(parent));
        return;
    }
    work->parent = parent;
    work->conversation = nullptr;
    work->startTicks = startTicks;
    work->endTicks = endTicks;
    work->callback = callback;
    work->voice = 0;
    if (startTicks > 0)
        std::memcpy(reinterpret_cast<std::byte *>(work) + 36, text, length + 1);
    else {
        dbgPrintEventMessage(text);
        if (callback)
            callback(1, 0);
    }
}

void QrdkSerifCallbackLoop(void *data) {
    QrdkSerifWork *const work = static_cast<QrdkSerifWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (GetCurrentControlInput(INPUT_REPEAT) & 0x200U) {
        if (work->startTicks <= 1) {
            if (work->endTicks > 1)
                work->endTicks = 1;
        } else
            work->startTicks = 1;
    }
    work->startTicks = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->startTicks) - 1U);
    if (!work->startTicks) {
        const char *const text = reinterpret_cast<const char *>(work) + 36;
        if (work->voice) {
            work->conversation = EV_PlayDialogue(text);
            if (!work->conversation) {
                HLib::DestroyCurrentTask();
                if (work->callback)
                    work->callback(0, reinterpret_cast<std::uintptr_t>(work->parent));
            } else if (work->callback)
                work->callback(1, 0);
        } else {
            dbgPrintEventMessage(text);
            if (work->callback)
                work->callback(1, 0);
        }
    }
    work->endTicks = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->endTicks) - 1U);
    if (!work->endTicks) {
        if (work->startTicks <= 0) {
            if (work->voice) {
                if (work->conversation && cvsHasValidConverseData(work->conversation))
                    scenCvsCtrlFree(work->conversation);
                work->conversation = nullptr;
            } else
                ctrlStopSubtitles();
        }
        HLib::DestroyCurrentTask();
        if (work->callback)
            work->callback(3, reinterpret_cast<std::uintptr_t>(work->parent));
    }

    if (work->callback)
        work->callback(2, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(work->endTicks)));
}

void QrdkResumeOnSerifFinished(std::int32_t event, std::uintptr_t argument) {
    if (event == 3)
        HLib::ResolveDeferredCallback(reinterpret_cast<HLib::Task *>(argument));
}

void QrdkBeginDoorCallback(void *data) {
    if (data)
        HLib::SetCurrentTaskTo(QrdkGetDoorCallback);
    else {
        CopyMotionTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 13, 0);
        HLib::SetCurrentTaskTo(QrdkExitCallback);
    }
}

void QrdkGetDoorCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        CopyMotionTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 13, 0);
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (!ClaimDoor(work->door, shen_name{0})) {
        EV_AT_SetPos(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
        EV_AT_SetAngY(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedRotation);
        HLib::SetCurrentTaskTo(QrdkRunCallback);
        return;
    }
    work->flags |= 4U;
    Vector3 ryo, doorPosition;
    Vector3i rotation;
    std::uint32_t doorYaw;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    QrdkGetDoorPoint(work->door, &doorPosition, &doorYaw);
    GetScenarioCharacterEulerAngles(QRDK_RYO, &rotation, EventTransformFlags::World);
    const bool side = ComputeAngle(ryo, 10, static_cast<std::int16_t>(rotation.y + 0x4000), 0x4000, doorPosition);
    CopyMotionTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 1, 0);
    Vector3 position{-0.8999999761581421f, 0, 2};
    float start, end;
    std::uint16_t motion;
    switch (shenGetRandomInt32(4)) {
    case 0:
        motion = QRDK_MOTION_KT6_TALK_D;
        start = 19;
        end = 70;
        position.z = 1;
        break;
    case 1:
        motion = QRDK_MOTION_DOOR_KICK_PUSH;
        start = 314;
        end = 360;
        position.z = 1.5f;
        break;
    case 2:
        motion = QRDK_MOTION_BREAK_DOOR;
        start = 26;
        end = 74;
        break;
    default:
        motion = QRDK_MOTION_ANC_TALK_C;
        start = 36;
        end = 87;
        break;
    }
    rotation = {0, static_cast<std::int32_t>(doorYaw + (side ? 0xc000U : 0x4000U)), 0};
    if (side)
        position.x = 0.8999999761581421f;
    ApplyScenarioTranslationAndRotation(&doorPosition, &rotation, &position);
    EV_AT_SetPos(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    if (side)
        SetMotionRegionFlag(scenFindMotionwork(QRDK_RYO), 69, 0, MotionRegion::LowerBody);
    work->action |= 2U;
    doorPlayRigidMotion(work->door, 0xf0ca, 0, 1);
    mtwkRequestMotion(scenFindMotionwork(QRDK_RYO), motion, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(QRDK_RYO), 0);
    SetMotionLayerLoopStartTime(scenFindMotionwork(QRDK_RYO), start, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(QRDK_RYO), start, MotionRegion::LowerBody);
    SetMotionLayerEndFrame(scenFindMotionwork(QRDK_RYO), end, MotionRegion::LowerBody);
    if (work->door.name == QRDK_R102.name || work->door.name == QRDK_R104.name || work->door.name == QRDK_R117.name)
        HLib::SetCurrentTaskTo(QrdkBattleMessageCallback);
    else
        HLib::SetCurrentTaskTo(work->door.name == QRDK_R122.name ? QrdkFoundCallback : QrdkWrongRoomCallback);
    if (QrdkPassedRoutePoint(work->routeCounter))
        work->routeCounter = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->routeCounter) + 1U);
}

bool QrdkPassedRoutePoint(std::int32_t counter) {
    Vector3 ryo;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    const QrdkRoutePoint *const previous = QrdkGetRoutePoint(static_cast<std::int32_t>(static_cast<std::uint32_t>(counter) - 1U));
    const QrdkRoutePoint *const point = QrdkGetRoutePoint(counter);
    float px = previous->x - point->x, pz = previous->z - point->z;
    float x = ryo.x - point->x, z = ryo.z - point->z;
    if (std::fabs(px) < 0.00000011920929f)
        px = 0;
    if (std::fabs(pz) < 0.00000011920929f)
        pz = 0;
    if (std::fabs(x) < 0.00000011920929f)
        x = 0;
    if (std::fabs(z) < 0.00000011920929f)
        z = 0;
    const float y = ryo.y * 0.0f;
    const float backward = (x * px + y) + z * pz;
    const QrdkRoutePoint *const next = QrdkGetRoutePoint(static_cast<std::int32_t>(static_cast<std::uint32_t>(counter) + 1U));
    float nx = next->x - point->x, nz = next->z - point->z;
    if (std::fabs(nx) < 0.00000011920929f)
        nx = 0;
    if (std::fabs(nz) < 0.00000011920929f)
        nz = 0;
    return (x * nx + y) + z * nz > backward;
}

void QrdkWrongRoomCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if ((work->action & 2U) && !SetMotionRegionFlag(scenFindMotionwork(QRDK_RYO), 65, 1, MotionRegion::LowerBody))
        return;
    work->action &= ~2U;
    const char *text;
    switch (shenGetRandomInt32(5)) {
    case 0:
        text = "違う。";
        break;
    case 1:
        text = "いない。";
        break;
    case 2:
        text = "くそっ。";
        break;
    case 3:
        text = "違ったか。";
        break;
    default:
        text = "ここじゃない。";
        break;
    }
    QrdkQueueSerif(text, 0, 50, nullptr);
    HLib::SetCurrentTaskTo(QrdkReturnToRouteCallback);
}

void QrdkReturnToRouteCallback(void *data) {
    using enum CameraState;
    if (!data) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    QrdkPlaceOnRoute(static_cast<QrdkWork *>(data));
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    HLib::SetCurrentTaskTo(QrdkRunCallback);
}

void QrdkFoundCallback(void *data) {
    using enum CameraState;
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if ((work->action & 2U) && !SetMotionRegionFlag(scenFindMotionwork(QRDK_RYO), 65, 1, MotionRegion::LowerBody))
        return;
    work->action &= ~2U;
    QrdkQueueSerif("発見　イベント終了", 1, 100, QrdkResumeOnSerifFinished);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    mtwkRequestMotion(scenFindMotionwork(QRDK_RYO), 0xf003, MotionRegion::LowerBody);
    HLib::SetCurrentTaskTo(QrdkExitCallback);
    HLib::SetupTaskDelay(-1);
}

void QrdkBattleMessageCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if ((work->action & 2U) && !SetMotionRegionFlag(scenFindMotionwork(QRDK_RYO), 65, 1, MotionRegion::LowerBody))
        return;
    work->action &= ~2U;
    ReleaseDoor(std::bit_cast<std::int32_t>(work->door.name), shen_name{0});
    work->flags &= ~4U;
    const char *text = nullptr;
    if (work->door.name == QRDK_R102.name)
        text = "ネズミが飛び込んできたか！";
    else if (work->door.name == QRDK_R104.name)
        text = "なんだ、てめえは！";
    else if (work->door.name == QRDK_R117.name)
        text = "てめえは、あの時の！";
    if (text)
        QrdkQueueSerif(text, 1, 100, QrdkResumeOnSerifFinished);
    mtwkRequestMotion(scenFindMotionwork(QRDK_RYO), 0xf003, MotionRegion::LowerBody);
    HLib::SetCurrentTaskTo(QrdkPlaceEnemiesCallback);
}

void QrdkPlaceEnemiesCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 3U) {
        work->flags &= ~3U;
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        return;
    }
    work->flags |= 3U;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        const QrdkEnemy &enemy = QRDK_ENEMIES[index];
        if (work->defeated[index] || enemy.door.name != work->door.name)
            continue;
        Vector3 door;
        std::uint32_t yaw;
        QrdkGetDoorPoint(work->door, &door, &yaw);
        Vector3 position{-sin_fixed16(static_cast<std::int16_t>(yaw)) * enemy.z, door.y + 0.30000001f, -cos_fixed16(static_cast<std::uint16_t>(yaw)) * enemy.z};
        if (std::fabs(position.x) < 0.00000011920929f)
            position.x = 0;
        if (std::fabs(position.z) < 0.00000011920929f)
            position.z = 0;
        position.x -= cos_fixed16(static_cast<std::uint16_t>(yaw)) * enemy.x;
        position.z += sin_fixed16(static_cast<std::int16_t>(yaw)) * enemy.x;
        if (std::fabs(position.x) < 0.00000011920929f)
            position.x = 0;
        if (std::fabs(position.z) < 0.00000011920929f)
            position.z = 0;
        position.x += door.x;
        position.z += door.z;
        if (std::fabs(position.x) < 0.00000011920929f)
            position.x = 0;
        if (std::fabs(position.z) < 0.00000011920929f)
            position.z = 0;
        const Vector3i rotation{0, static_cast<std::int32_t>(yaw + enemy.yaw), 0};
        EV_AT_SetPos(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        EV_SetCharaControllerFlags(enemy.character, 0, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(enemy.character, 1, NpcCharacterControllerState::Enabled);
    }
    HLib::SetCurrentTaskTo(QrdkStartBattleCallback);
}

void QrdkStartBattleCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    Vector3 position;
    QrdkGetDoorPoint(work->door, &position, nullptr);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{0}, reinterpret_cast<std::uintptr_t>("QRDK"), 1);
    shenInitFightData(FindCharacterByScenarioName(QRDK_RYO), nullptr, 0, 0, 0, 0);
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        const QrdkEnemy &enemy = QRDK_ENEMIES[index];
        if (work->defeated[index] || enemy.door.name != work->door.name)
            continue;
        shenInitFightData(FindCharacterByScenarioName(enemy.character), nullptr, 1, 1, 40, static_cast<std::uint8_t>(enemy.style));
        vfUpdateFlagsFor(FindCharacterByScenarioName(enemy.character), 0, 0x400000, 128);
        vfUpdateFlagsFor(FindCharacterByScenarioName(enemy.character), 0, 0x200000, 2048);
        vfUpdateFlagsFor(FindCharacterByScenarioName(enemy.character), 0, 0x8000000, 1);
    }
    vfCopyPosFromEntity(0, nullptr, position.x, position.z, 7);
    vfmQueueFightStart(2, 2, 1, 1, -1);
    HLib::SetCurrentTaskTo(QrdkBattleCallback);
}

void QrdkBattleCallback(void *data) {
    if (g_fightActive || filmCheckFadeTrackColour())
        return;
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (work->flags & 3U) {
        work->flags &= ~3U;
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        return;
    }
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        const QrdkEnemy &enemy = QRDK_ENEMIES[index];
        if (enemy.door.name != work->door.name)
            continue;
        EV_AT_SetPos(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{10000, 10000, 10000});
        EV_AT_SetAngY(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
        work->defeated[index] = 1;
    }
    FinishLoading();
    if (GetBtlStatus(FindCharacterByScenarioName(QRDK_RYO)) == 3) {
        work->flags |= 3U;
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        QrdkPlaceOnRoute(work);
        HLib::SetCurrentTaskTo(QrdkRunCallback);
    } else
        HLib::SetCurrentTaskTo(QrdkRetryCallback);
}

void QrdkRetryCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        EV_AT_SetPos(QRDK_ENEMIES[index].character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{10000, 10000, 10000});
        EV_AT_SetAngY(QRDK_ENEMIES[index].character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
        work->defeated[index] = 0;
    }
    QrdkQueueSerif("りとらい", 0, 50, nullptr);
    Vector3 position;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = QRDK_ROUTE[0].x;
    position.z = QRDK_ROUTE[0].z;
    EV_AT_SetPos(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    work->flags |= 3U;
    work->routeCounter = 1;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    QrdkPlaceOnRoute(work);
    HLib::SetCurrentTaskTo(QrdkRunCallback);
}

void QrdkExitCallback(void *data) {
    if (filmCheckFadeTrackColour())
        return;
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    HLib::Task *const parent = work ? work->parent : nullptr;
    const std::uint32_t flags = work ? work->flags : 0;
    const shen_name door = work ? work->door : shen_name{0};
    if (flags & 3U) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        if (work)
            work->flags &= ~3U;
        return;
    }
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    const bool ryoHasMotion = scenFindMotionwork(QRDK_RYO) != nullptr;
    EV_SetCharacterAlphaEnabled(QRDK_RYO, true);
    if (ryoHasMotion)
        mtwkSetExtraFlag(scenFindMotionwork(QRDK_RYO), 6, 0);
    EV_SetCharaControllerFlags(QRDK_RYO, 0, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(QRDK_RYO, 1, NpcCharacterControllerState::Enabled);
    if (ryoHasMotion)
        EV_SetRefBoardParamsFor(QRDK_RYO, 0);
    if (flags & 4U)
        ReleaseDoor(std::bit_cast<std::int32_t>(door.name), QRDK_RYO);
    for (std::size_t index = 0; QRDK_CHARACTERS[index].name != QRDK_INVALID.name; ++index) {
        const shen_name character = QRDK_CHARACTERS[index];
        const bool hasMotion = scenFindMotionwork(character) != nullptr;
        EV_SetCharacterAlphaEnabled(character, true);
        if (hasMotion)
            mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
        EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Enabled);
        if (hasMotion)
            EV_SetRefBoardParamsFor(character, 0);
        EV_npcLockUnlock(character);
    }
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QRDK"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QRDK"), 0);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    if (parent)
        HLib::ResolveDeferredCallback(parent);
    SetPlayerControlEnabled(1);
}

const ScriptAreaDefinition qrdkScriptDefinition{.areaId = AREA_QRDK, .field04 = 0, .init = QrdkInitNullsub, .main = QrdkMain};
} // namespace shenmue::s2::areas
