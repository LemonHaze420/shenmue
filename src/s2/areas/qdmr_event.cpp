#include "s2/areas/area_qdmr.hpp"
#include "s2/areas/area_loop.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/navi.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cmath>

namespace shenmue::s2 {
shen_name FindNearestQdmrDoor(const std::uint32_t *roomFlags) {
    shen_name result{};
    if (!roomFlags)
        return result;
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, EventTransformFlags::World);
    float nearest = -1.0f;
    for (std::size_t index = 0; QDMR_DOOR_PLACEMENTS[index].door.name; ++index) {
        if (!(roomFlags[index] & 1U))
            return result;
        const QdmrDoorPlacement &door = QDMR_DOOR_PLACEMENTS[index];
        float x = position.x - door.position.x, z = position.z - door.position.y;
        if (std::fabs(x) < 1.1920929e-7f)
            x = 0;
        if (std::fabs(z) < 1.1920929e-7f)
            z = 0;
        float xx = x * x, zz = z * z;
        if (std::fabs(xx) < 1.1920929e-7f)
            xx = 0;
        if (std::fabs(zz) < 1.1920929e-7f)
            zz = 0;
        float distance = zz + xx;
        if (std::fabs(distance) < 1.1920929e-7f)
            distance = 0;
        if (distance >= 4.0f)
            continue;
        Vector3 direction{0, 0, 4.0f - distance};
        if (std::fabs(direction.z) < 1.1920929e-7f)
            direction.z = 0;
        const Vector3i yaw{0, rotation.y - door.yaw, 0};
        ApplyScenarioTranslationAndRotation(nullptr, &yaw, &direction);
        if (direction.z >= nearest) {
            nearest = direction.z;
            if (direction.z > -0.1f)
                result = door.door;
        }
    }
    return result;
}

std::int32_t GetQdmrDoorVariant(shen_name door, const std::uint32_t *roomFlags) {
    for (std::size_t index = 0; QDMR_ROUTES[index].door.name; ++index) {
        if (QDMR_ROUTES[index].door.name != door.name)
            continue;
        const std::int32_t variant = QDMR_ROUTES[index].variant;
        if ((roomFlags[index] & 2U) && variant >= 1 && variant <= 3)
            return variant + 3;
        return variant;
    }
    return 0;
}

void MarkQdmrRoomVisited(shen_name door, std::uint32_t *roomFlags) {
    if (!door.name || !roomFlags)
        return;
    for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index) {
        if (QDMR_ROOMS[index].door.name != door.name)
            continue;
        roomFlags[index] &= ~1U;
        if (!(roomFlags[index] & 4U)) {
            Vector3 position;
            for (std::size_t placement = 0; QDMR_DOOR_PLACEMENTS[placement].door.name; ++placement) {
                if (QDMR_DOOR_PLACEMENTS[placement].door.name != door.name)
                    continue;
                position = {QDMR_DOOR_PLACEMENTS[placement].position.x, GetQronTemporaryFloorHeight(), QDMR_DOOR_PLACEMENTS[placement].position.y};
                break;
            }
            roomFlags[index] |= 4U;
            PlaceNaviMarker(position, 4);
        }
        if (!(roomFlags[index] & 2U))
            roomFlags[index] |= 2U;
    }
}

void SetQdmrPlayerHandState(QdmrEventWork *, bool scripted) {
    Character *const player = ctrlGetCharaByName(QDMR_PLAYER);
    if (!player)
        return;
    if (scripted) {
        GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(QDMR_PLAYER));
        SetLowQualityHands(ctrlGetCharaByName(QDMR_PLAYER));
        SetHighModelXYZ(ctrlGetCharaByName(QDMR_PLAYER), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(QDMR_PLAYER), 1, 1, 0);
        std::byte *const pose = reinterpret_cast<std::byte *>(QDMR_HAND_POSE.data());
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(QDMR_PLAYER), 1, pose, 1);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(QDMR_PLAYER), 0, pose, 1);
    } else {
        HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
        hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
    }
}
std::int64_t QdmrInitialAuthorCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    if (phase == 0) {
        EV_SetModelDataDefaultVertexShader();
        LGHT_Parser_Internal(QDMR_DEFAULT_LIGHT.name);
        SetQronFloor(18);
        const Vector3 floorOffset{0, GetQronTemporaryFloorHeight() - 100.0f, 0};
        SetLightRootPosition(floorOffset);
        ExecuteCleanupFuncForAreaWithCachedScene();
        LGHT_Parser_Internal(QDMR_INIT_LIGHT.name);
        const Vector3 lightOffset{QDMR_INIT_LIGHT_OFFSET.x, QDMR_INIT_LIGHT_OFFSET.y, QDMR_INIT_LIGHT_OFFSET.z};
        SetLightRootPosition(lightOffset);
        PushGlobalLightingProgram();
    } else if (phase == 1 && parent) {
        HLib::ResolveDeferredCallback(parent);
        HLib::SetupTimedTask(parent, 1);
    }
    return 1;
}
std::int64_t QdmrStoryAuthorCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    if (phase == 0) {
        EV_ResetFreeTalkFlags();
        EV_NPC_NewLoadSw(1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2097, 1);
    } else if (phase == 1) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2097, 0);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        if (parent) {
            HLib::ResolveDeferredCallback(parent);
            HLib::SetupTimedTask(parent, 1);
        }
    }
    return 1;
}
std::int64_t QdmrYuanAuthorCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    if (phase == 0) {
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QES3END"), 0);
        EV_LoadCutsceneFromCurrentScene("0364A000");
        EV_LoadAudio("0364A000", 0);
        EV_spmBeginSampleExecute();
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(parent));
        if (work)
            work->flags &= ~0x20U;
    } else if (phase == 1 && parent) {
        HLib::ResolveDeferredCallback(parent);
        HLib::SetupTimedTask(parent, 1);
    }
    return 1;
}
std::int64_t QdmrBingoWinAuthorCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    if (phase == 0) {
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 5);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(parent));
        if (work)
            work->flags &= ~1U;
    } else if (phase == 1) {
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(parent));
        if (work)
            work->flags |= 0x21U;
        if (parent) {
            HLib::ResolveDeferredCallback(parent);
            HLib::SetupTimedTask(parent, 1);
        }
    }
    return 1;
}
std::int64_t QdmrBingoOpeningAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent) {
    if (phase == 0) {
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 4);
        EV_spmBeginSampleExecute();
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(parent));
        if (work) {
            work->flags &= ~0x20U;
            SetDoorSubjectEnabled(work->door, 0);
        }
    } else if (phase == 1) {
        ExecuteCleanupFuncForAreaWithCachedScene();
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(parent));
        if (work)
            SetDoorSubjectEnabled(work->door, 1);
        EV_DisableCharacterDisplay(QDMR_AA99);
        EV_DisableCharacterDisplay(QDMR_A020);
        if (EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor)) {
            if (work)
                work->flags |= 1U;
            SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        spmFadeOut();
        EV_LoadCutsceneFromCurrentScene("0266A000");
        EV_LoadAudio("0266A000", 3);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                .character = QDMR_PLAYER,
                                .flags = 0,
                                .motion = 13,
                                .duration = -1,
                                .elapsed = 0,
                                .startFrame = 0,
                                .endFrame = -1,
                                .speed = 1,
                                .frameTime = 0,
                                .reserved2c = 0,
                                .completion = nullptr},
            0);
        Vector3 playerPosition;
        EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
        SetAreaLoopCharacterControl(QDMR_PLAYER, 0);
        Vector3i rotation{0, 0, 0};
        for (std::size_t index = 0; QDMR_BINGO_NPCS[index].name; ++index) {
            const shen_name character = QDMR_BINGO_NPCS[index];
            QueueAreaLoopMotion({.parent = HLib::GetCurrentTask(),
                                    .character = character,
                                    .flags = 0,
                                    .motion = 3632,
                                    .duration = -1,
                                    .elapsed = 0,
                                    .startFrame = 0,
                                    .endFrame = -1,
                                    .speed = 1,
                                    .frameTime = 0,
                                    .reserved2c = 0,
                                    .completion = nullptr},
                0);
            Vector3 position;
            EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            float x = playerPosition.x - position.x, z = playerPosition.z - position.z;
            if (std::fabs(x) < 1.1920929e-7f)
                x = 0;
            if (std::fabs(z) < 1.1920929e-7f)
                z = 0;
            rotation.y = std::fabs(x) >= 1.1920929e-7f || std::fabs(z) >= 1.1920929e-7f ? InverseFixedAtan2(x, z) : 0;
            EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        }
        if (parent) {
            HLib::ResolveDeferredCallback(parent);
            HLib::SetupTimedTask(parent, 1);
        }
    } else if (phase == 2 && EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 32 == static_cast<std::int32_t>(frame)) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(parent));
        if (work)
            work->flags |= 1U;
    }
    return 1;
}
void QueueQdmrTransitionAttachment(const Vector3 *position, const Vector3i *rotation) {
    if (!position || !rotation)
        return;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopAttachmentCallback, 4, 0x0b, sizeof(AreaLoopAttachmentWork), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopAttachmentWork *const work = static_cast<AreaLoopAttachmentWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->elapsed = 0;
    work->character = QDMR_PLAYER;
    work->attachmentFlags = 14;
    work->target = QDMR_TRANSITION_TARGET;
    work->position = *position;
    work->rotation = *rotation;
    work->flags |= 0x80000000U;
    work->completion = nullptr;
    work->duration = 540;
    AreaLoopAttachmentCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= ~0x80000000U;
    else
        HLib::DestroyTask(task);
}
void QueueQdmrTransitionLookAt() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopLookAtCallback, 4, 0x0b, sizeof(AreaLoopLookAtWork), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopLookAtWork *const work = static_cast<AreaLoopLookAtWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->character = QDMR_PLAYER;
    work->elapsed = 0;
    work->completion = nullptr;
    work->target = QDMR_TRANSITION_TARGET;
    work->bone = -1;
    work->duration = 539;
    work->flags = 0x80000000U;
    AreaLoopLookAtCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= ~0x80000000U;
    else
        HLib::DestroyTask(task);
}
bool FindClearRotationPathForAngY(std::int32_t *yaw) {
    if (!yaw)
        return false;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(QDMR_PLAYER, &rotation, EventTransformFlags::Local);
    float angle = static_cast<float>(rotation.y) / 182.04445f + 22.5f;
    if (std::fabs(angle) < 1.1920929e-7f)
        angle = 0;
    else {
        while (angle >= 360.0f) {
            angle -= 360.0f;
            if (std::fabs(angle) < 1.1920929e-7f) {
                angle = 0;
                break;
            }
        }
        while (angle < 0) {
            angle += 360.0f;
            if (std::fabs(angle) < 1.1920929e-7f) {
                angle = 0;
                break;
            }
        }
    }
    const std::int32_t firstSector = static_cast<std::int32_t>(angle) / 45;
    std::int32_t intersections[8];
    rotation.x = 0;
    rotation.z = 0;
    for (std::int32_t index = 0; index < 8; ++index) {
        Vector3 origin, intersection;
        EV_GetCharaPosition(QDMR_PLAYER, CHARACTER_TRANSLATION_ROOT, &origin, EventTransformFlags::Local);
        Vector3 destination{0, 0.30000001f, -1.8f};
        rotation.y = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<float>(firstSector + index) * 45.0f * 8192.0f) / 45.0f));
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &destination);
        intersections[index] = (FindNearestRayIntersection(&intersection, &destination, &origin) & 1) == 0;
    }
    std::int32_t start = 0, current = 1;
    for (;;) {
        const std::int32_t near = intersections[(current + 6) % 8], previous = intersections[(current + 5) % 8];
        if (near && intersections[start % 8] && intersections[current % 8] && current >= 17)
            break;
        if ((previous || intersections[(current + 1) % 8]) && near && intersections[start % 8] && intersections[current % 8] && current >= 9)
            break;
        if (previous && near && intersections[start % 8] && intersections[current % 8] && intersections[(current + 1) % 8])
            break;
        ++start;
        if (++current >= 25)
            return false;
    }
    *yaw = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<float>(start + firstSector) * 45.0f * 8192.0f) / 45.0f));
    return true;
}
void QdmrLoopSoundCallback(void *parameter) {
    if (parameter)
        Loop3DSoundEffect(*static_cast<const std::uint32_t *>(parameter));
    HLib::DestroyCurrentTask();
}
void QueueQdmrLoopSound(std::uint32_t command, std::int32_t delay) {
    if (delay <= 0) {
        Loop3DSoundEffect(command);
        return;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QdmrLoopSoundCallback, 4, 0x0b, sizeof(std::uint32_t), AREA_EVENT_TASK);
    if (!task)
        return;
    std::uint32_t *const work = static_cast<std::uint32_t *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    HLib::SetupTimedTask(task, delay);
    *work = command;
}
void QdmrFadeOutSoundCallback(void *) {
    spmFadeOut();
    HLib::DestroyCurrentTask();
}
void QdmrDoneCallback(void *parameter) {
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(parameter);
    if (work) {
        for (std::size_t index = 0; QDMR_ROOMS[index].door.name; ++index)
            ReleaseDoor(QDMR_ROOMS[index].door.name, QDMR_PLAYER);
        if (work->timerFlags < 0) {
            work->timerRequests |= 0x80000000U;
            return;
        }
        if (!(work->flags & 0x20U)) {
            spmFadeOut();
            work->flags |= 0x20U;
        }
        if (filmCheckFadeTrackColour())
            return;
        if (!(work->flags & 1U)) {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            return;
        }
        if (work->flags & 2U) {
            if (ctrlGetCharaByName(QDMR_PLAYER)) {
                HasCharacterMotionWork(ctrlGetCharaByName(QDMR_PLAYER));
                hndlSetFlags(ctrlGetCharaByName(QDMR_PLAYER));
            }
            work->flags &= ~2U;
        }
        if (work->timerText) {
            work->timerText->release(work->timerText);
            work->timerText = nullptr;
        }
        EV_NPC_NewLoadSw(1);
    }
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_EventSetUpHelpIcon_0(0, 0);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QDMR"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDMR"), 0);
    EV_LoadCutsceneFromCurrentScene("QFIX");
    EV_LoadAudio("QFIX", 0);
    EV_spmBeginSampleExecute();
    EV_SetCanDoAreaJumpTo(1);
    FinishLoading();
    SetPlayerControlEnabled(1);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
}
void QdmrDoorMotionCallback(void *parameter) {
    QdmrDoorMotionWork *const work = static_cast<QdmrDoorMotionWork *>(parameter);
    if (work && --work->frames <= 0) {
        doorLoopRigidMotion(work->door, static_cast<std::uint16_t>(work->motion), work->startFrame, work->speed);
        HLib::DestroyCurrentTask();
    }
}
bool GetQdmrDoorPlacement(shen_name door, Vector3 *position, std::int32_t *yaw) {
    for (std::size_t index = 0; QDMR_DOOR_PLACEMENTS[index].door.name; ++index) {
        const QdmrDoorPlacement &placement = QDMR_DOOR_PLACEMENTS[index];
        if (placement.door.name != door.name)
            continue;
        if (position)
            *position = {placement.position.x, GetQronTemporaryFloorHeight(), placement.position.y};
        if (yaw)
            *yaw = placement.yaw;
        return true;
    }
    return false;
}
} // namespace shenmue::s2
