#include "s1/task/cycle_man.hpp"
#include "s1/task/kdan.hpp"
#include "s1/motion.hpp"
#include "s1/collision.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/skinning_cache.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <cctype>
#include "s1/matrix_stack.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/conv.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/wexe.hpp"
#include "s1/mt5_types.hpp"
#include "s1/mt5_render.hpp"
#include "s1/face.hpp"
#include "s1/render_array.hpp"
#include "s1/player_control.hpp"
#include "s1/timing.hpp"
#include "s1/boot.hpp"
#include "s1/subtitles.hpp"
#include "s1/file_loader.hpp"

#include <cfloat>
#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {
using namespace CycleManager;

namespace CycleManager {


std::int32_t GetBikeSoundCommand(std::uint32_t index) {
    if (index >= kDobuitaBikeSounds.size())
        return -1;
    const std::uint32_t area = CycleManager::CurrentAreaId();
    const std::array<std::int32_t, 8> *commands = nullptr;
    if (area == SHEN_NAME(D000))
        commands = &kDobuitaBikeSounds;
    else if (area == SHEN_NAME(MFSY))
        commands = &kAmihamaBikeSounds;
    else if (area == SHEN_NAME(JD00))
        commands = &kSakuragaokaBikeSounds;
    else if (area == SHEN_NAME(JU00))
        commands = &kJu00BikeSounds;
    else
        return -1;
    const std::int32_t command = (*commands)[index];
    return command < 0 ? -1 : ExecuteAICACommand_Wrapper(static_cast<std::uint32_t>(command));
}

std::int32_t GetBikeSoundsForArea(CycleManager::NpcRecord &npc) {
    if (CycleManager::CurrentAreaId() != npc.cycle->areaID.name)
        return -1;
    if (!npc.cycle->moveObjRuntime)
        return CycleManager::GetBikeSoundCommand(3);
    const std::uint32_t token = npc.cycle->moveObjRuntime->moveObjectId;
    if (token == SHEN_NAME(K013))
        return CycleManager::GetBikeSoundCommand(6);
    if (token == SHEN_NAME(BIKE))
        return CycleManager::GetBikeSoundCommand(0);
    const std::uint32_t tokenFamily = token & 0x00ffffffU;
    if (tokenFamily == (SHEN_NAME(CAB0) & 0x00ffffffU))
        return CycleManager::GetBikeSoundCommand(1);
    if (tokenFamily == (SHEN_NAME(FK00) & 0x00ffffffU))
        return CycleManager::GetBikeSoundCommand(4);
    if (tokenFamily == (SHEN_NAME(HAL0) & 0x00ffffffU))
        return CycleManager::GetBikeSoundCommand(2);
    return -1;
}

void SynchronizeBikeSoundBank(CycleManager::NpcRecord &npc) {
    if (npc.cycle->motnSteeringActive) {
        if (npc.cycle->primarySoundBankId == -1)
            npc.cycle->primarySoundBankId = CycleManager::GetBikeSoundsForArea(npc);
    } else if (npc.cycle->primarySoundBankId >= 0) {
        ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
        npc.cycle->primarySoundBankId = -1;
    }
    npc.cycle->secondarySoundBankId = -1;
}




void PerformBikeSoundUpdate(CycleManager::NpcRecord &npc, float observedSpeed) {
    if (npc.cycle->primarySoundBankId < 0)
        return;
    float minimumDistance = 0.0f;
    float fadeDistance = 0.0f;
    std::uint32_t releaseVolume = 0;
    if (CycleManager::CurrentAreaId() == npc.cycle->areaID.name) {
        if (npc.cycle->moveObjRuntime) {
            const std::uint32_t token = npc.cycle->moveObjRuntime->moveObjectId;
            if (token == SHEN_NAME(K013) || (token & 0x00ffffffU) == (SHEN_NAME(CAB0) & 0x00ffffffU)) {
                minimumDistance = 3.0f;
                fadeDistance = npc.cycle->dist + 1.0f;
                releaseVolume = 127;
            } else if (token == SHEN_NAME(BIKE)) {
                minimumDistance = 2.0f;
                fadeDistance = npc.cycle->dist;
                releaseVolume = observedSpeed > 0.001f ? 127U : 0U;
            } else if ((token & 0x00ffffffU) == (SHEN_NAME(FK00) & 0x00ffffffU)) {
                minimumDistance = 2.0f;
                fadeDistance = npc.cycle->dist;
                releaseVolume = 127;
            } else if ((token & 0x00ffffffU) == (SHEN_NAME(HAL0) & 0x00ffffffU)) {
                minimumDistance = 4.0f;
                fadeDistance = npc.cycle->dist + 2.0f;
                releaseVolume = 127;
            }
        } else if (npc.cycle->moveConfig->cycleClassCode == 6u) {
            minimumDistance = 10.0f;
            fadeDistance = 100.0f;
            releaseVolume = npc.cycle->moveObjSpeedParam != 0.0f ?
                static_cast<std::uint32_t>(static_cast<std::int32_t>(npc.cycle->baseMoveSpeed / npc.cycle->moveObjSpeedParam * 64.0f)) :
                0U;
        } else {
            minimumDistance = 8.0f;
            fadeDistance = npc.cycle->dist + 3.0f;
            releaseVolume = 64;
        }
    }
    Execute3DSoundCommand(
        0xffffffffU, static_cast<std::uint32_t>(npc.cycle->primarySoundBankId), &npc.cycle->pos, minimumDistance, fadeDistance, releaseVolume);
}
} // namespace CycleManager

namespace CycleManager {






bool InitBikeMoveObjectRuntime(CycleManager::NpcRecord &npc, uint32_t opcode, uint32_t vehicle, uint32_t count, uint32_t offset, float speed) {
    if (count < 2u) {
        uint32_t index = g_TotalActiveCycles;
        if (index >= 10u)
            index = 0;
        g_CycleTable[2u * index] = npc.cycle->moveConfig->obj_id.name;
        g_TotalActiveCycles = index + 1;
        return false;
    }
    npc.cycle->savedOpcodeBeforeMoveObj = static_cast<CycleOpcode>(opcode);
    npc.cycle->moveObjActivationPending = false;
    const bool held = npc.cycle->moveObjRuntime != nullptr;
    CycleMoveObjectRuntime *const runtime = CycleManager::AttachVehicleRuntime(npc, vehicle, opcode == CycleOpcode_DriveForkliftPath2D ? 5u : 0u);
    if (!runtime)
        return false;
    npc.cycle->pathPoints = reinterpret_cast<Vector3 *>(CycleManager::FilePointer(offset));
    npc.cycle->pathPointCount = static_cast<uint16_t>(count);
    npc.cycle->pathPointIndex = 0;
    npc.cycle->moveObjSpeedParam = CycleManager::ResolveBikeSpeedParam(runtime, speed);
    const BikeMoveObjectToken *const token = npc.bikeToken;
    if (!token || !token->anims)
        return false;
    if (!npc.cycle->foundExistingMoveObjRuntime) {
        if (!held) {
            npc.cycle->animMoveSpeedScale = 1.0f;
            npc.cycle->baseMoveSpeed = 0.0f;
            npc.cycle->pathPointIndex = 1;
            runtime->steeringAngle = 0;
            runtime->bikeBankAngle = 0;
            runtime->leanHistory[0] = 0;
            runtime->leanHistory[1] = 0;
            runtime->wheelSpin = 0;
            runtime->yawVelocity = 0;
            runtime->slopeAngleX = 0;
            runtime->animLeanAngle = 0;
            const Vector2 *const points = reinterpret_cast<const Vector2 *>(npc.cycle->pathPoints);
            const Vector2 &first = points[0];
            const float segmentX = points[1].x - first.x;
            const float segmentZ = points[1].y - first.y;
            if (std::fabs(segmentX) >= 0.001f || std::fabs(segmentZ) >= 0.001f)
                runtime->yaw = static_cast<int32_t>(atan2_fixed16(segmentX, segmentZ)) + 0x8000;
            else
                runtime->yaw = npc.cycle->desiredPathYaw;
            runtime->tokenLocalOffset = runtime->moveObjectKind == 5u ? Vector3{0.0f, 0.0f, -0.5f} : token->pos;
            npc.cycle->pos.x = first.x;
            npc.cycle->pos.z = first.y;
            runtime->pathStartX = first.x;
            runtime->pathStartZ = first.y;
            runtime->pos.x = npc.cycle->pos.x;
            runtime->pos.z = npc.cycle->pos.z;
            runtime->overrideTransform = nullptr;
            runtime->shadowState.lightIndex[0] = -1;
            runtime->shadowState.lightIndex[1] = -1;
        }
        CycleManager::InitBikeMoveObjectPathContext(npc);
        return true;
    }

    npc.cycle->currentOpcode = CycleOpcode_BikeDismountInteraction;
    npc.cycle->requestAnimId = static_cast<uint16_t>(runtime->renderEuler.y);
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(runtime->pos.x, 0.0f, runtime->pos.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<uint16_t>(runtime->yaw));
    Vector3 mount{token->mount_target_x, 0.0f, token->mount_target_z};
    Matrix4x4::TransformPoint(mount, mount);
    Matrix4x4::Pop();
    Vector3 direction{mount.x - npc.cycle->pos.x, 0.0f, mount.z - npc.cycle->pos.z};
    float distance = 0.0f;
    if (direction.x > 0.00000011920929f || direction.x < -0.00000011920929f || direction.z > 0.00000011920929f || direction.z < -0.00000011920929f) {
        distance = Vector3::NormalizeInPlace(direction);
        npc.cycle->desiredPathYaw = static_cast<int16_t>(atan2_fixed16(direction.x, direction.z) + 0x8000);
    }
    const int16_t mountYaw = static_cast<int16_t>(static_cast<uint16_t>(runtime->yaw) + token->mount_yaw_delta);
    if (distance >= 0.001f) {
        npc.cycle->moveObjActivationPending = true;
        npc.cycle->moveTargetPos = mount;
        npc.cycle->moveTargetYaw = mountYaw;
        npcActivateIfNearInArea(npc, 16.0f, nullptr, 0, npc.cycle->motnSteeringActive, npc.cycle->moveConfig->motionParamIds[0]);
    } else {
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
        npc.cycle->desiredPathYaw = mountYaw;
        npc.cycle->activeAnimId = static_cast<uint16_t>(token->anims->anim[0]);
        MotionInformationWork *const motion = npc.cycle->mot;
        if (motion && npc.cycle->obj) {
            CycleManager::EnableRootMotion(npc.cycle->obj, true);
            motion->motionFlags &= ~2u;
            motion->pendingAnimId = npc.cycle->activeAnimId;
        }
    }
    return true;
}
} // namespace CycleManager






void InitBikePathRuntime(CycleManager::NpcRecord &npc, const char *record) {
    CycleManager::MobjCycle *const ctx = npc.cycle;
    const int32_t *const word = reinterpret_cast<const int32_t *>(record);
    ctx->currentOpcode = static_cast<CycleOpcode>(*reinterpret_cast<const uint16_t *>(record));
    ctx->activeAnimId = *reinterpret_cast<const uint16_t *>(record + 4);
    ctx->pathPointCount = *reinterpret_cast<const uint16_t *>(record + 8);
    ctx->pathPoints = reinterpret_cast<Vector3 *>(CycleManager::FilePointer(static_cast<uint32_t>(word[3])));
    const Vector2 *const points = reinterpret_cast<const Vector2 *>(ctx->pathPoints);
    ctx->animMoveSpeedScale = 1.0f;
    ctx->baseMoveSpeed = 0.0f;
    ctx->bikeSteeringState = BikeSteering_Normal;
    ctx->moveTargetYaw = 0;
    ctx->moveObjSpeedParam = std::bit_cast<float>(word[4]) * 0.0092592593f;
    const float dz = ctx->pos.z - points[0].y;
    const float dx = ctx->pos.x - points[0].x;
    if (ctx->snapToPathStart || dz * dz + dx * dx > 100.0f) {
        ctx->pathSegmentProgress = 1.0f;
        ctx->pathPointIndex = 1;
        ctx->pos = {points[0].x, 100.0f, points[0].y};
        ctx->pos.y = MapProbe_UpdateIntersectionStateAtPoint(ctx->pos);
    } else {
        ctx->pathPointIndex = 0;
        ctx->pathSegmentProgress = 0.0f;
    }
    const Vector2 &target = points[ctx->pathPointIndex];
    const int16_t yaw = CycleManager::Atan2Fixed16(ctx->pos.x - target.x, ctx->pos.z - target.y);
    ctx->desiredPathYaw = yaw;
    ctx->cycleObjectId = ctx->moveConfig->obj_id.name;
    ctx->cycleObjectIdCopy = ctx->moveConfig->obj_id.name;
    npc.selfCycleIndex = static_cast<int32_t>(&npc - g_cycleManagerRuntime.npcRecords.data());
    ctx->bikeClearanceTimer = 0;
    ctx->ryoBlockingCooldownFrames = 0;
    ctx->bikeClearanceBlocked = false;
    ctx->lastMoveObjPos = ctx->pos;
    ctx->lastMoveObjYaw = yaw;
    ctx->steeringWaitFrames = 0;
    ctx->reversePathFrames = 0;
    ctx->map_probe_cooldown = 0;
    if (ctx->motnSteeringActive && ctx->moveConfig->cycleClassCode == 6u) {
        const uint32_t area = CycleManager::CurrentAreaId();
        if (area == SHEN_NAME(D000) || area == SHEN_NAME(MFSY))
            Execute3DSoundCommand(area == SHEN_NAME(D000) ? 0x2605a9u : 0x5c05a9u, 4u, &ctx->pos, 6.0f, 60.0f, 0xffffffffu);
    }
    CycleManager::SynchronizeBikeSoundBank(npc);
    npc.driveWheelSteering = 0;
}
namespace CycleManager {



bool InitVehiclePath2D(CycleManager::NpcRecord &npc, uint32_t recordOffset, uint32_t opcode) {
    uint16_t anim = 0;
    uint32_t vehicle = 0;
    uint32_t count = 0;
    uint32_t offset = 0;
    float speed = 1.0f;
    const std::vector<uint8_t> &bytes = g_cycleManagerRuntime.bytes;
    if (!ReadValue(bytes, recordOffset + 4u, anim) || !ReadValue(bytes, recordOffset + 8u, vehicle) || !ReadValue(bytes, recordOffset + 12u, count) ||
        !ReadValue(bytes, recordOffset + 16u, offset) || !ReadValue(bytes, recordOffset + 20u, speed))
        return false;
    npc.vehiclePathAnimId = anim;
    npc.vehicleToken = vehicle;
    return CycleManager::InitBikeMoveObjectRuntime(npc, opcode, vehicle, count, offset, speed);
}
} // namespace CycleManager

namespace CycleManager {

BikeMoveObjectToken MakeBikeToken(const BikeAnimTokenSet *anims, Vector3 localOffset, int16_t steeringClamp, int16_t wheelPitch,
    Vector3 leftAnkleLocal, Vector3 rightAnkleLocal, Vector3 wheelPivotLocal, float mountTargetX, float mountTargetZ, int16_t mountYawDelta, int16_t runtimeC0,
    float steeringHalfWidth, float steeringHalfDepth, uint8_t speedCurveMode) {
    BikeMoveObjectToken token{};
    token.pos = localOffset;
    token.steering_clamp = steeringClamp;
    token.wheel_pitch = wheelPitch;
    token.left_ankle_local = leftAnkleLocal;
    token.right_ankle_local = rightAnkleLocal;
    token.wheel_pivot_local = wheelPivotLocal;
    token.mount_target_x = mountTargetX;
    token.mount_target_z = mountTargetZ;
    token.mount_yaw_delta = mountYawDelta;
    token.runtime_c0_value = runtimeC0;
    token.anims = anims;
    token.steering_half_width = steeringHalfWidth;
    token.steering_half_depth = steeringHalfDepth;
    token.speed_curve_mode = speedCurveMode;
    return token;
}



CycleMoveObjectRuntime *FindVehicleRuntime(uint32_t ownerToken, uint32_t vehicleToken) {
    for (CycleMoveObjectRuntime *runtime = g_parkedMoveObjects; runtime; runtime = runtime->next) {
        if (runtime->ownerObjectId == ownerToken && runtime->moveObjectId == vehicleToken)
            return runtime;
    }
    return nullptr;
}



uint32_t GetForkliftLoadToken(CycleManager::NpcRecord &npc) {
    for (const ForkliftLoadToken &entry : kForkliftLoadTokens) {
        if (entry.driver.name == npc.cycle->moveConfig->obj_id.name) {
            npc.cycle->map_probe_cooldown = entry.probePhase;
            return entry.load.name;
        }
    }
    npc.cycle->map_probe_cooldown = 0;
    return 0;
}

uint32_t VehicleKindForToken(uint32_t vehicleToken) {
    if (vehicleToken == SHEN_NAME(FK00) || vehicleToken == SHEN_NAME(FK01))
        return 5;
    if (vehicleToken == SHEN_NAME(HDCA) || vehicleToken == SHEN_NAME(TOMC))
        return 4;
    return vehicleToken == SHEN_NAME(BIKE) ? 0u : 1u;
}


float GetBikeGroundOffset(const CycleMoveObjectRuntime &runtime) {
    if (runtime.moveObjectKind == 5u)
        return 0.0f;
    if (runtime.moveObjectId == SHEN_NAME(HAL1)) {
        const bool firstFlat = runtime.pos.x > 18.0f && runtime.pos.x < 21.0f && runtime.pos.z > 30.0f && runtime.pos.z < 33.0f;
        const bool secondFlat = runtime.pos.x > 25.0f && runtime.pos.x < 28.0f && runtime.pos.z > 27.0f && runtime.pos.z < 29.0f;
        return firstFlat || secondFlat ? 0.0f : 0.071999997f;
    }
    return runtime.moveObjectId == SHEN_NAME(CABU) ? 0.071999997f : 0.0f;
}



void ResolveVisibleBikeModels(CycleMoveObjectRuntime &runtime) {
    if (!runtime.model) {
        Character *const character = CycleManager::FindCharacter(runtime.moveObjectId);
        if (character) {
            runtime.model = reinterpret_cast<HMDL_t *>(character->model);
            runtime.shadowOrAuxModel = character->shadowModel;
            auto *const root = reinterpret_cast<Mt5RuntimeNode *>(runtime.model);
            std::byte *const modelBase = root ? MT5_ModelBaseForNode(root) : nullptr;
            if (root && modelBase && FindRuntimeNodeByKind(root, modelBase, 196u)) {
                const uint32_t low24 = runtime.moveObjectId & 0x00ffffffu;
                runtime.modelAttachOffset.x = runtime.moveObjectId == SHEN_NAME(K013)                                                                 ? -0.05f :
                    (runtime.moveObjectId == SHEN_NAME(BIKE) || low24 == (SHEN_NAME(CAB0) & 0x00ffffffu) || low24 == (SHEN_NAME(HAL0) & 0x00ffffffu)) ? -0.0f :
                                                                                                                                                        -100.0f;
                runtime.modelAttachOffset.y = runtime.moveObjectId == SHEN_NAME(K013) ? 0.94239998f :
                    runtime.moveObjectId == SHEN_NAME(BIKE)                           ? 0.8937f :
                    low24 == (SHEN_NAME(CAB0) & 0x00ffffffu)                          ? 1.0207f :
                    low24 == (SHEN_NAME(HAL0) & 0x00ffffffu)                          ? 0.93809998f :
                                                                                        -100.0f;
                runtime.modelAttachOffset.z = runtime.moveObjectId == SHEN_NAME(K013) ? -0.8624f :
                    runtime.moveObjectId == SHEN_NAME(BIKE)                           ? -0.81370002f :
                    low24 == (SHEN_NAME(CAB0) & 0x00ffffffu)                          ? -0.94069999f :
                    low24 == (SHEN_NAME(HAL0) & 0x00ffffffu)                          ? -0.8581f :
                                                                                        -100.0f;
            } else {
                runtime.modelAttachOffset = {-100.0f, -100.0f, -100.0f};
            }
        }
    }

    if (runtime.moveObjectKind == 5u && runtime.token) {
        runtime.auxModel = nullptr;
        if (Character *const auxiliary = CycleManager::FindCharacter(runtime.token))
            runtime.auxModel = auxiliary->model;
    }
}



void PublishVisibleParkedMoveObjects() {
    if (!g_parkedMoveObjects)
        return;
    CycleMoveObjectRuntime *visible = g_visibleMoveObjects;
    for (CycleMoveObjectRuntime *runtime = g_parkedMoveObjects; runtime; runtime = runtime->next) {
        if (runtime->areaId != CycleManager::CurrentAreaId())
            continue;
        if (runtime->visibilityState == 2u) {
            runtime->pos.y = CycleManager::GetBikeGroundOffset(*runtime);
            runtime->visibilityState = 1u;
        }
        if (!shenIsLocalSphereInsideViewCullVolume(runtime->pos, 2.0f) || !shenIsLocalSphereVisibleInScreenDepthTable(runtime->pos, 2.0f))
            continue;
        CycleManager::ResolveVisibleBikeModels(*runtime);
        if (runtime->model) {
            runtime->visibleNext = visible;
            visible = runtime;
            runtime->renderOpacity = 1.0f;
        }
    }
    g_visibleMoveObjects = visible;
}



void AppendParkedVehicleSteeringVolume(const CycleMoveObjectRuntime &runtime) {
    float backOffset = runtime.tokenLocalOffset.z;
    Vector3 center = runtime.pos;
    float halfWidth = 1.0f;
    float halfDepth = 0.5f;
    if (runtime.moveObjectKind == 4u) {
        halfWidth = 2.4000001f;
        halfDepth = 1.4f;
    } else if (runtime.moveObjectKind == 5u) {
        halfWidth = 1.5f;
        halfDepth = 0.80000001f;
    } else if (runtime.moveObjectId == SHEN_NAME(HAL0)) {
        halfWidth = 1.3f;
        halfDepth = 0.47500002f;
        backOffset -= 0.19999999f;
        center.x = runtime.pos.x - cos_fixed16(static_cast<uint16_t>(runtime.yaw)) * 0.32499999f;
    } else if (runtime.moveObjectId == SHEN_NAME(HAL1)) {
        halfDepth = 0.44999999f;
        center.x = runtime.pos.x - cos_fixed16(static_cast<uint16_t>(runtime.yaw)) * 0.25f;
        center.z = runtime.pos.z - sin_fixed16(static_cast<int16_t>(runtime.yaw)) * -0.25f;
    } else if (runtime.moveObjectId == SHEN_NAME(BIKE)) {
        halfWidth = 0.89999998f;
        halfDepth = 0.31999999f;
    }
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (g_dynamicTurnCollisionVolumeCount >= cm.dynamicTurnCollisionVolumes.size())
        return;
    CycleManager::IntersectionEntry &entry = cm.dynamicTurnCollisionVolumes[g_dynamicTurnCollisionVolumeCount++];
    entry.operand = SCN_U32FromFloat(center.x);
    entry.offset = SCN_U32FromFloat(center.y);
    entry.dwordC = SCN_U32FromFloat(center.z);
    entry.dword10 = static_cast<uint32_t>(runtime.yaw);
    entry.dword14 = SCN_U32FromFloat(halfWidth);
    entry.dword18 = SCN_U32FromFloat(halfDepth);
    entry.dword1C = SCN_U32FromFloat(backOffset);
}



void AppendParkedVehicleSteeringVolumes() {
    for (CycleMoveObjectRuntime *runtime = g_parkedMoveObjects; runtime; runtime = runtime->next) {
        if (runtime->areaId == CycleManager::CurrentAreaId() && shenIsLocalSphereInsideViewCullVolume(runtime->pos, 2.0f) &&
            shenIsLocalSphereVisibleInScreenDepthTable(runtime->pos, 2.0f) && runtime->model)
            CycleManager::AppendParkedVehicleSteeringVolume(*runtime);
    }
}
} // namespace CycleManager
  // namespace

namespace CycleManager {

const BikeMoveObjectToken *GetBikeMoveObjectToken(uint32_t ownerToken, uint32_t vehicleToken) {
    if (vehicleToken == SHEN_NAME(K013))
        return &kBikeTokenK013;

    if (vehicleToken == SHEN_NAME(BIKE)) {
        if (ownerToken == SHEN_NAME(YKHI))
            return &kBikeTokenYKHI;
        if (ownerToken == SHEN_NAME(SOBA))
            return &kBikeTokenSOBA;
    } else if (vehicleToken == SHEN_NAME(HAL0)) {
        return &kBikeTokenHAL0;
    }

    const uint32_t low24 = vehicleToken & 0x00FFFFFFu;
    if (low24 == (SHEN_NAME(CAB0) & 0x00FFFFFFu)) {
        if (ownerToken != SHEN_NAME(SNKC) && (ownerToken == SHEN_NAME(YMST) || ownerToken == SHEN_NAME(KWMT))) {
            return &kBikeTokenHALAlt;
        }
        return &kBikeTokenHAL;
    }

    if (low24 == (SHEN_NAME(HAL0) & 0x00FFFFFFu)) {
        if (ownerToken == SHEN_NAME(TSUC) || ownerToken == SHEN_NAME(KJIY))
            return &kBikeTokenHAL0Alt;
        if (ownerToken == SHEN_NAME(BIKA) || ownerToken == SHEN_NAME(BIKB))
            return &kBikeTokenHAL0;
        return &kBikeTokenHAL;
    }
    return &kBikeTokenHAL;
}



const BikeSpeedProfile &GetBikeSpeedProfile(const CycleMoveObjectRuntime *runtime) {
    if (!runtime)
        return kBikeSpeedDefault;
    const uint32_t vehicleToken = runtime->moveObjectId;
    if (vehicleToken == SHEN_NAME(BIKE))
        return kBikeSpeedBike;
    if (vehicleToken == SHEN_NAME(K013))
        return kBikeSpeedHAL;

    const uint32_t low24 = vehicleToken & 0x00FFFFFFu;
    if (low24 == (SHEN_NAME(HAL0) & 0x00FFFFFFu) || low24 == (SHEN_NAME(CAB0) & 0x00FFFFFFu)) {
        return kBikeSpeedHAL;
    }
    return kBikeSpeedFallback;
}

float GetBikeSteeringReferenceSpeed(const CycleMoveObjectRuntime *runtime) {
    return CycleManager::GetBikeSpeedProfile(runtime).steeringReferenceSpeed;
}

float ResolveBikeSpeedParam(const CycleMoveObjectRuntime *runtime, float rawSpeed) {
    if (!std::isfinite(rawSpeed))
        return kBikeSpeedFallback.presetA;
    if (rawSpeed >= 0.0f)
        return rawSpeed * 0.0092592593f;

    const BikeSpeedProfile &profile = CycleManager::GetBikeSpeedProfile(runtime);
    if (rawSpeed == -10.0f)
        return profile.presetA;
    if (rawSpeed == -20.0f)
        return profile.presetB;
    if (rawSpeed == -21.0f)
        return profile.presetC;
    if (rawSpeed == -22.0f)
        return profile.presetD;
    if (rawSpeed == -30.0f)
        return profile.steeringReferenceSpeed;
    return 0.18518519f;
}
} // namespace CycleManager

namespace CycleManager {

MotionNodeRuntime *FindBikeConstraintRow(void *rows, std::uint8_t nodeId) {
    auto *row = static_cast<MotionNodeRuntime *>(rows);
    while (row && row->nodeId != kMotionNodeEnd) {
        if (row->nodeId == nodeId)
            return row;
        ++row;
    }
    return nullptr;
}



bool ResolveBikeConstraintRows(MotionInformationWork &motion, std::uint8_t nodeId, MotionNodeRuntime *&source, MotionNodeRuntime *&active) {
    source = nullptr;
    active = FindBikeConstraintRow(motion.activeNodes, nodeId);
    if (motion.interpolationMode == 0) {
        source = active;
        active = nullptr;
        return source != nullptr;
    }

    source = FindBikeConstraintRow(motion.interpolationMode == kTransitionRowsBlendMode ? motion.transitionNodes : motion.blendTargetNodes, nodeId);
    return source && active;
}

void WriteBikeConstraintTarget(MotionInformationWork &motion, std::uint8_t nodeId, const Vector3 &target) {
    MotionNodeRuntime *source = nullptr;
    MotionNodeRuntime *active = nullptr;
    if (!ResolveBikeConstraintRows(motion, nodeId, source, active)) {
        return;
    }

    source->effectorPosition.x = target.x;
    source->effectorPosition.y = target.y;
    source->effectorPosition.z = target.z;
    if (active) {
        active->effectorPosition.x = target.x;
        active->effectorPosition.y = target.y;
        active->effectorPosition.z = target.z;
    }
}



void MOBJ_BikeMotionConstraintCallback(MotionInformationWork *motion, void *user) {
    auto *const npc = static_cast<CycleManager::NpcRecord *>(user);
    if (!motion || !npc || !npc->cycle->obj || !npc->cycle->moveObjRuntime)
        return;

    const BikeMoveObjectToken *token = npc->bikeToken;
    if (!token) {
        token = CycleManager::GetBikeMoveObjectToken(npc->cycle->moveConfig->obj_id.name, npc->cycle->moveObjRuntime->moveObjectId);
    }
    if (!token)
        return;

    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(motion->rootPreTranslate.x, npc->cycle->moveObjRuntime->pos.y, motion->rootPreTranslate.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(npc->cycle->moveObjRuntime->renderEuler.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(npc->cycle->moveObjRuntime->renderEuler.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(npc->cycle->moveObjRuntime->renderEuler.x));
    Matrix4x4::preTranslate(token->wheel_pivot_local.x, token->wheel_pivot_local.y, token->wheel_pivot_local.z);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(token->wheel_pitch));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(npc->cycle->moveObjRuntime->steeringAngle));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-token->wheel_pitch));
    Matrix4x4::preTranslate(-token->wheel_pivot_local.x, -token->wheel_pivot_local.y, -token->wheel_pivot_local.z);
    Vector3 leftTarget{};
    Vector3 rightTarget{};
    Matrix4x4::TransformPoint(token->left_ankle_local, leftTarget);
    Matrix4x4::TransformPoint(token->right_ankle_local, rightTarget);
    Matrix4x4::Pop();

    const bool sobaBike = npc->cycle->moveConfig->obj_id.name == SHEN_NAME(SOBA) && npc->cycle->moveObjRuntime->moveObjectId == SHEN_NAME(BIKE);
    if (!sobaBike) {
        WriteBikeConstraintTarget(*motion, kMotionLeftAnkleNode, leftTarget);
    }
    WriteBikeConstraintTarget(*motion, kMotionRightAnkleNode, rightTarget);

    if (!sobaBike)
        return;

    MotionNodeRuntime *source = nullptr;
    MotionNodeRuntime *active = nullptr;
    if (!ResolveBikeConstraintRows(*motion, kMotionRightFootEndNode, source, active)) {
        return;
    }
    constexpr Vector3s footEuler{static_cast<std::int16_t>(61823u), static_cast<std::int16_t>(892), static_cast<std::int16_t>(-8537)};
    source->bindEuler = footEuler;
    if (active)
        active->bindEuler = footEuler;
}

void SetBikeMotionConstraintOwner(CycleManager::NpcRecord &npc) {
    if (!npc.cycle->moveObjRuntime || npc.cycle->moveObjRuntime->moveObjectKind == 5u) {
        return;
    }
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!motion)
        return;
    motion->mobjCallback = reinterpret_cast<void *>(MOBJ_BikeMotionConstraintCallback);
    motion->mobjCallbackUser = &npc;
}
} // namespace CycleManager
  // namespace

namespace CycleManager {

CycleMoveObjectRuntime *AttachVehicleRuntime(CycleManager::NpcRecord &npc, uint32_t vehicleToken, uint32_t moveObjectKind) {
    if (!vehicleToken)
        return nullptr;





    npc.cycle->foundExistingMoveObjRuntime = false;
    CycleMoveObjectRuntime *runtime = npc.cycle->moveObjRuntime;
    if (!runtime) {
        runtime = CycleManager::FindVehicleRuntime(npc.cycle->moveConfig->obj_id.name, vehicleToken);
        npc.cycle->foundExistingMoveObjRuntime = runtime != nullptr;
        if (runtime) {
            npc.releasedMoveObjRuntime = runtime;
            npc.bikeToken = CycleManager::GetBikeMoveObjectToken(npc.cycle->moveConfig->obj_id.name, vehicleToken);
            return runtime;
        }
        runtime = g_freeMoveObjects;
        if (!runtime)
            return nullptr;
        g_freeMoveObjects = runtime->next;
        runtime->next = nullptr;
        runtime->areaId = npc.cycle->areaID.name;
        runtime->ownerObjectId = npc.cycle->moveConfig->obj_id.name;
        runtime->moveObjectId = vehicleToken;
        runtime->model = nullptr;
        runtime->moveObjectKind = moveObjectKind;
        runtime->token = CycleManager::GetForkliftLoadToken(npc);
        runtime->auxModel = nullptr;
    }
    npc.cycle->moveObjRuntime = runtime;
    npc.releasedMoveObjRuntime = nullptr;
    npc.bikeToken = CycleManager::GetBikeMoveObjectToken(npc.cycle->moveConfig->obj_id.name, vehicleToken);
    SetBikeMotionConstraintOwner(npc);
    return runtime;
}

void SyncVehicleRuntimeFromNpc(CycleManager::NpcRecord &npc) {
    CycleMoveObjectRuntime *runtime = npc.cycle->moveObjRuntime;
    if (!runtime)
        return;


    runtime->yaw = npc.cycle->desiredPathYaw;
    runtime->pos = npc.cycle->pos;
    SetBikeMotionConstraintOwner(npc);
}

void ReleaseVehicleRuntime(CycleManager::NpcRecord &npc) {
    CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime;
    if (!runtime)
        return;



    runtime->next = g_parkedMoveObjects;
    g_parkedMoveObjects = runtime;
    runtime->activeFlag = 1;
    if (npc.cycle->obj)
        fixoClearAttachment(npc.cycle->obj);
    npc.releasedMoveObjRuntime = runtime;
    npc.cycle->moveObjRuntime = nullptr;
    npc.cycle->dist = 3.0f;
    npc.cycle->range = 1.5f;
    CycleManager::RecomputeSteeringExtents(npc);
}

bool BeginBikeMoveObjectRelease(CycleManager::NpcRecord &npc) {
    CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime;
    if (!runtime)
        return false;

    const BikeMoveObjectToken *const token = CycleManager::GetBikeMoveObjectToken(npc.cycle->moveConfig->obj_id.name, runtime->moveObjectId);
    if (!token || !token->anims)
        return false;

    npc.bikeToken = token;
    npc.releasedMoveObjRuntime = runtime;
    runtime->animLeanAngle = 0;
    runtime->renderEuler.y = npc.cycle->desiredPathYaw;
    runtime->renderEuler.z = 0;
    runtime->visibilityState = 2;
    if (runtime->moveObjectId == SHEN_NAME(BIKE))
        runtime->wheelSpin = npc.cycle->moveConfig->obj_id.name == SHEN_NAME(SOBA) ? 0x8000 : 0;
    npc.cycle->requestAnimId = static_cast<uint16_t>(runtime->renderEuler.y);

    // HAL1 remains attached during this transition in the original.  Every
    // other vehicle is published and detached immediately, while the saved
    // released pointer remains valid until cleanup.
    if (runtime->moveObjectId != SHEN_NAME(HAL1))
        CycleManager::ReleaseVehicleRuntime(npc);
    if (MotionInformationWork *const motion = npc.cycle->mot) {
        motion->mobjCallback = nullptr;
        motion->mobjCallbackUser = nullptr;
    }

    npc.cycle->currentOpcode = CycleOpcode_BikeMoveInteraction;
    npc.cycle->activeAnimId = static_cast<uint16_t>(token->anims->anim[4]);
    npc.cycle->moveObjActivationPending = false;
    npc.cycle->facingConstraintFlag = true;
    if (MotionInformationWork *const motion = npc.cycle->mot; motion && npc.cycle->obj) {
        CycleManager::EnableRootMotion(npc.cycle->obj, true);
        motion->playbackRate = 1.0f;
        motion->motionFlags &= ~2u;
        motion->pendingAnimId = npc.cycle->activeAnimId;
    }
    npc.cycle->flags &= ~0x80u;
    return true;
}
} // namespace CycleManager

namespace CycleManager {



void SolveCubicSegmentCoeffs(SplineAxis4Cache &axis, const std::array<float, 4> &arcLength) {
    axis.secondDeriv[3] = 0.0f;
    axis.secondDeriv[0] = 0.0f;
    const float h0 = arcLength[1] - arcLength[0];
    const float h1 = arcLength[2] - arcLength[1];
    const float h2 = arcLength[3] - arcLength[2];
    const double inverseH1 = 1.0 / h1;
    const float diagonal0 = static_cast<float>((h1 + h0) * 0.3333333333333333);
    const float rhs0 = static_cast<float>(static_cast<double>(axis.control[0] / h0) - (1.0 / h0 + inverseH1) * axis.control[1] +
        static_cast<double>(axis.control[2] / h1) - static_cast<double>(h0 * axis.secondDeriv[0]) * 0.1666666666666667);
    float control3 = axis.control[3];
    if (std::fabs(control3) < 0.00000011920929f)
        control3 = 0.0f;
    float control2 = axis.control[2];
    if (std::fabs(control2) < 0.00000011920929f)
        control2 = 0.0f;
    const float inner1 = rhs0 / diagonal0;
    const float offDiagonal = static_cast<float>(h1 * 0.1666666666666667);
    const float diagonal1 = static_cast<float>((h2 + h1) * 0.3333333333333333);
    const float rhs1 = static_cast<float>(static_cast<double>(axis.control[1] / h1) - (1.0 / h2 + inverseH1) * control2 + static_cast<double>(control3 / h2) -
        static_cast<double>(h2 * axis.secondDeriv[3]) * 0.1666666666666667);
    const float inner2 = (rhs1 - offDiagonal * inner1) / (diagonal1 - offDiagonal * (offDiagonal / diagonal0));
    axis.secondDeriv[2] = inner2;
    axis.secondDeriv[1] = inner1 - inner2 * (offDiagonal / diagonal0);
}



void BuildNaturalCubicSpline3DCache() {
    const auto chord = [](std::size_t from) {
        float dx = g_spline3d.x.control[from + 1] - g_spline3d.x.control[from];
        if (std::fabs(dx) < 0.00000011920929f)
            dx = 0.0f;
        float dy = g_spline3d.y.control[from + 1] - g_spline3d.y.control[from];
        if (std::fabs(dy) < 0.00000011920929f)
            dy = 0.0f;
        float dz = g_spline3d.z.control[from + 1] - g_spline3d.z.control[from];
        if (std::fabs(dz) < 0.00000011920929f)
            dz = 0.0f;
        return std::sqrt((dy * dy + dx * dx) + dz * dz);
    };
    g_spline3d.cumulativeArcLen[0] = 0.0f;
    const float first = chord(0);
    g_spline3d.cumulativeArcLen[1] = first + 0.0f;
    const float second = (first + 0.0f) + chord(1);
    g_spline3d.cumulativeArcLen[2] = second;
    g_spline3d.cumulativeArcLen[3] = second + chord(2);
    SolveCubicSegmentCoeffs(g_spline3d.x, g_spline3d.cumulativeArcLen);
    SolveCubicSegmentCoeffs(g_spline3d.y, g_spline3d.cumulativeArcLen);
    SolveCubicSegmentCoeffs(g_spline3d.z, g_spline3d.cumulativeArcLen);
}
} // namespace CycleManager
  // namespace


void UpdateAnimationCurvePoints(const std::array<Vector3, 4> &points) {
    for (std::size_t index = 0; index < points.size(); ++index) {
        g_spline3d.x.control[index] = points[index].x;
        g_spline3d.y.control[index] = points[index].y;
        g_spline3d.z.control[index] = points[index].z;
    }
    BuildNaturalCubicSpline3DCache();
}


void CalculateCubicPolynomial(Vector3 &result, float distance, float extra) {
#if SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX
    if (g_bEnableFrameTimeSmoothing) {
        const float scale = GetNpcMovementFrameScale();
        distance *= scale;
        extra *= scale;
    }
#endif
    const std::array<float, 4> &arc = g_spline3d.cumulativeArcLen;
    const float along = (arc[1] + distance) + extra;
    const std::size_t index = along > arc[2] ? 2u : 1u;
    const float from = along - arc[index];
    const float to = along - arc[index + 1];
    const float span = arc[index + 1] - arc[index];
    const float span6 = static_cast<float>(span * 6.0);
    const auto value = [&](const SplineAxis4Cache &axis) {
        const float second0 = axis.secondDeriv[index];
        const float second1 = axis.secondDeriv[index + 1];
        return static_cast<float>(static_cast<double>((((second1 * from) * from) * from - ((second0 * to) * to) * to) / span6) +
            (static_cast<double>(axis.control[index + 1] / span) - static_cast<double>(second1 * span) * 0.1666666666666667) * from -
            (static_cast<double>(axis.control[index] / span) - static_cast<double>(second0 * span) * 0.1666666666666667) * to);
    };
    result = {value(g_spline3d.x), value(g_spline3d.y), value(g_spline3d.z)};
}
namespace CycleManager {






bool TickVehicleActiveState(CycleManager::NpcRecord &npc) {
    CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime;
    if (!runtime)
        return false;
    const bool live = npc.cycle->motnSteeringActive;
    if (live)
        runtime->renderOpacity = npc.cycle->obj ? npc.cycle->obj->renderOpacity : 0.0f;
    npc.cycle->bikeSteeringState = BikeSteering_Crawl;
    const float observedSpeed = CycleManager::SnapshotMoveObjTransform(npc, live);
    const float targetX = npc.vehicleCommandPos.x;
    const float targetZ = npc.vehicleCommandPos.z;
    const float speed = npc.cycle->baseMoveSpeed;
    npc.cycle->moveTargetPos = {npc.cycle->pos.x - targetX, 0.0f, npc.cycle->pos.z - targetZ};
    float distance = 0.0f;
    if (npc.cycle->moveTargetPos.x > 0.00000011920929f || npc.cycle->moveTargetPos.x < -0.00000011920929f || npc.cycle->moveTargetPos.z > 0.00000011920929f ||
        npc.cycle->moveTargetPos.z < -0.00000011920929f) {
        distance = Vector3::NormalizeInPlace(npc.cycle->moveTargetPos);
    } else {
        npc.cycle->moveTargetPos = {};
    }

    if (distance < 1.0f || speed * 10.0f > distance) {
        if (distance <= speed) {
            npc.cycle->pos.x = targetX;
            npc.cycle->pos.z = targetZ;
            npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
            npc.cycle->desiredPathYaw = npc.cycle->moveTargetYaw;
            runtime->pos = npc.cycle->pos;
            runtime->yaw = npc.cycle->desiredPathYaw;
            runtime->animLeanAngle = 0;
            runtime->renderEuler = {0, npc.cycle->desiredPathYaw, 0};
            runtime->pathYaw = npc.cycle->desiredPathYaw;
            if (npc.cycle->primarySoundBankId >= 0) {
                ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
                npc.cycle->primarySoundBankId = -1;
            }
            npc.cycle->moveObjActivationPending = false;
            return false;
        }
        if (npc.cycle->reversePathFrames == 0 || npc.cycle->pathSegmentProgress > 0.0f) {
            npc.cycle->pos.x -= speed * npc.cycle->moveTargetPos.x;
            npc.cycle->pos.z -= speed * npc.cycle->moveTargetPos.z;
            if (npc.cycle->moveTargetPos.x > 0.00000011920929f || npc.cycle->moveTargetPos.x < -0.00000011920929f ||
                npc.cycle->moveTargetPos.z > 0.00000011920929f || npc.cycle->moveTargetPos.z < -0.00000011920929f) {
                npc.cycle->desiredPathYaw = CycleManager::Atan2Fixed16(npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z);
            }
        }
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
        runtime->pos = npc.cycle->pos;
        runtime->yaw = npc.cycle->desiredPathYaw;
        runtime->animLeanAngle = 0;
        runtime->renderEuler = {0, npc.cycle->desiredPathYaw, 0};
        return true;
    }

    if (live) {
        const std::uint16_t yaw = static_cast<std::uint16_t>(npc.cycle->desiredPathYaw);
        const std::uint16_t parkedYaw = static_cast<std::uint16_t>(npc.cycle->moveTargetYaw);
        const Vector3 ahead{npc.cycle->pos.x + sin_fixed16(static_cast<std::int16_t>(yaw)), npc.cycle->pos.y,
            npc.cycle->pos.z + sin_fixed16(static_cast<std::int16_t>(0x4000 - CycleManager::FoldUnsignedYaw(yaw)))};
        UpdateAnimationCurvePoints({ahead, npc.cycle->pos, Vector3{targetX, npc.cycle->pos.y, targetZ},
            Vector3{targetX - sin_fixed16(static_cast<std::int16_t>(parkedYaw)), npc.cycle->pos.y,
                targetZ - sin_fixed16(static_cast<std::int16_t>(0x4000 - CycleManager::FoldUnsignedYaw(parkedYaw)))}});
        Vector3 next{};
        CalculateCubicPolynomial(next, speed, 0.0f);
        const std::int16_t heading = CycleManager::Atan2Fixed16(ahead.x - npc.cycle->pos.x, ahead.z - npc.cycle->pos.z);
        runtime->steeringAngle += std::clamp<std::int32_t>(
            static_cast<std::int16_t>(heading - npc.cycle->desiredPathYaw - static_cast<std::int16_t>(runtime->steeringAngle)), -182, 182);
        runtime->yawVelocity += static_cast<std::int32_t>((observedSpeed / 2.198f) * 65536.0f);
        runtime->steeringAngle = std::clamp(runtime->steeringAngle, -5461, 5461);
        DoBikeAngles(npc, *runtime, speed);
        runtime->animLeanAngle = 0;
        const std::int16_t previousYaw = npc.cycle->desiredPathYaw;
        const std::int32_t turn = std::clamp<std::int32_t>(
            static_cast<std::int16_t>(CycleManager::Atan2Fixed16(npc.cycle->pos.x - next.x, npc.cycle->pos.z - next.z) - previousYaw), -364, 364);
        npc.cycle->desiredPathYaw = static_cast<std::int16_t>(turn + previousYaw);
        npc.cycle->pos = next;
        runtime->yaw = npc.cycle->desiredPathYaw;
        runtime->renderEuler = {runtime->slopeAngleX, npc.cycle->desiredPathYaw, runtime->animLeanAngle};
        runtime->animOutput = 0;
    } else {
        npc.cycle->pos.x = targetX;
        npc.cycle->pos.z = targetZ;
        npc.cycle->desiredPathYaw = npc.cycle->moveTargetYaw;
    }
    runtime->yaw = npc.cycle->desiredPathYaw;
    npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
    runtime->pos = npc.cycle->pos;
    if (runtime->moveObjectKind != 5u)
        SetBikeMotionConstraintOwner(npc);
    return true;
}
} // namespace CycleManager
 // namespace



void BIKE_Init(CycleMoveObjectRuntime &runtime) {
    runtime.steeringAngle = 0;
    runtime.leanHistory[0] = 0;
    runtime.leanHistory[1] = 0;
    runtime.pathCurveDelta = 0;
    runtime.pathYaw = runtime.yaw;
    if (runtime.moveObjectId == SHEN_NAME(BIKE)) {
        runtime.animOutput = 0x4000;
        runtime.renderEuler = {-910, runtime.yaw, 0};
        runtime.tokenLocalOffset.y = static_cast<float>(static_cast<double>(sin_fixed16(910)) * 0.83);
        return;
    }
    if ((runtime.moveObjectId & 0x00ffffffu) == (SHEN_NAME(HAL0) & 0x00ffffffu))
        RotateForBIKE(runtime, 0, runtime.yaw, 0x1555);
    else
        runtime.renderEuler = {0, runtime.yaw, 0};
    runtime.animOutput = 0x4000;
}
namespace CycleManager {




bool BeginVehicleActiveState(CycleManager::NpcRecord &npc, bool live) {
    CycleMoveObjectRuntime *attached = npc.cycle->moveObjRuntime;
    if (attached && attached->moveObjectId != npc.vehicleToken)
        attached = nullptr;

    if (attached) {

        if (npc.cycle->obj) {
            attached->renderEuler = {0, npc.cycle->desiredPathYaw, 0};
            ApplyEulerAnglesToNode(npc.cycle->obj->entity.node, attached->renderEuler);
        }
        if (!npc.vehicleCommandEnabled) {
            if (npc.cycle->primarySoundBankId >= 0) {
                ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
                npc.cycle->primarySoundBankId = -1;
            }
            attached->next = g_freeMoveObjects;

            npc.cycle->moveObjRuntime = nullptr;
            g_freeMoveObjects = attached;
            npc.cycle->moveObjActivationPending = false;
            npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
            return false;
        }

        npc.cycle->moveObjSpeedParam = 0.0f;
        attached->areaId = npc.cycle->areaID.name;
        if (live) {
            npc.cycle->moveTargetPos = npc.vehicleCommandPos;
            npc.cycle->moveTargetYaw = npc.vehicleCommandYaw;
            npc.cycle->moveObjActivationPending = true;
            npc.cycle->currentOpcode = CycleOpcode_SetVehicleActiveState;
            return true;
        }

        attached->pos = {npc.vehicleCommandPos.x, 100.0f, npc.vehicleCommandPos.z};
        attached->pos.y = MapProbe_UpdateIntersectionStateAtPoint(attached->pos);
        attached->yaw = npc.vehicleCommandYaw;
        attached->pathYaw = npc.vehicleCommandYaw;
        attached->visibilityState = 2;
        BIKE_Init(*attached);

        if (npc.cycle->primarySoundBankId >= 0) {
            ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
            npc.cycle->primarySoundBankId = -1;
        }
        attached->next = g_parkedMoveObjects;
        attached->activeFlag = 1;
        npc.cycle->moveObjRuntime = nullptr;
        g_parkedMoveObjects = attached;
        npc.cycle->moveObjActivationPending = false;
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return false;
    }

    // A vehicle parked for this owner is withdrawn to the free list or placed again; otherwise the head of the free list is parked as
    // a new one, keeping the fields this does not set from its last use.
    CycleMoveObjectRuntime *runtime = CycleManager::FindVehicleRuntime(npc.cycle->moveConfig->obj_id.name, npc.vehicleToken);
    if (!npc.vehicleCommandEnabled) {
        if (runtime) {
            CycleManager::UnlinkParkedMoveObject(runtime);
            runtime->next = g_freeMoveObjects;
            g_freeMoveObjects = runtime;
        }
        npc.cycle->moveObjActivationPending = false;
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return false;
    }
    if (runtime) {
        runtime->activeFlag = 1;
    } else {
        runtime = g_freeMoveObjects;
        if (!runtime)
            return false;
        g_freeMoveObjects = runtime->next;
        runtime->next = g_parkedMoveObjects;
        runtime->activeFlag = 1;
        g_parkedMoveObjects = runtime;
        runtime->ownerObjectId = npc.cycle->moveConfig->obj_id.name;
        runtime->moveObjectId = npc.vehicleToken;
        runtime->model = nullptr;
        runtime->token = CycleManager::GetForkliftLoadToken(npc);
        runtime->auxModel = nullptr;
        npc.cycle->moveObjSpeedParam = 0.0f;
        npc.cycle->baseMoveSpeed = 0.0f;
        npc.cycle->pathPointCount = 0;
        npc.cycle->pathPointIndex = 0;
        runtime->steeringAngle = 0;
        runtime->bikeBankAngle = 0;
        runtime->leanHistory[0] = 0;
        runtime->leanHistory[1] = 0;
        runtime->pathCurveDelta = 0;
        if (runtime->moveObjectId == SHEN_NAME(BIKE))
            runtime->wheelSpin = npc.cycle->moveConfig->obj_id.name == SHEN_NAME(SOBA) ? 0x8000 : 0;
        runtime->yawVelocity = 0;
        runtime->slopeAngleX = 0;
        runtime->animLeanAngle = 0;
        runtime->pathCurveBase = 0;
        runtime->unk11E = 0;
        runtime->overrideTransform = nullptr;
        runtime->moveObjectKind = CycleManager::VehicleKindForToken(runtime->moveObjectId);
        if (runtime->moveObjectKind == 4u) {
            runtime->tokenLocalOffset = {};
        } else if (runtime->moveObjectKind == 5u) {
            runtime->tokenLocalOffset = {0.0f, 0.0f, -0.5f};
        } else if (const BikeMoveObjectToken *const token = CycleManager::GetBikeMoveObjectToken(npc.cycle->moveConfig->obj_id.name, runtime->moveObjectId)) {
            runtime->tokenLocalOffset = token->pos;
        }
    }

    runtime->areaId = npc.cycle->areaID.name;
    runtime->pos = {npc.vehicleCommandPos.x, 100.0f, npc.vehicleCommandPos.z};
    runtime->pos.y = MapProbe_UpdateIntersectionStateAtPoint(runtime->pos);
    runtime->yaw = npc.vehicleCommandYaw;
    runtime->pathYaw = npc.vehicleCommandYaw;
    runtime->renderEuler = {runtime->slopeAngleX, runtime->yaw, runtime->animLeanAngle};
    runtime->pathStartX = runtime->pos.x;
    runtime->pathStartZ = runtime->pos.z;
    runtime->visibilityState = 2;
    BIKE_Init(*runtime);
    npc.cycle->moveObjActivationPending = false;
    npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
    return false;
}
} // namespace CycleManager

namespace CycleManager {



void UpdatePathSegmentCache(CycleMoveObjectRuntime &runtime, const Vector2 &from, const Vector2 &to) {
    const float dx = to.x - from.x;
    const float dz = to.y - from.y;
    const float length = std::sqrt(dz * dz + dx * dx);
    if (length == 0.0f)
        return;
    const int16_t yaw = CycleManager::Atan2Fixed16(dx, dz);
    const int16_t turn = static_cast<int16_t>(static_cast<uint16_t>(runtime.pathYaw) - static_cast<uint16_t>(yaw));
    const int32_t delta = static_cast<int32_t>(static_cast<float>(-turn) * 2.916667f * 1.078841f / length);
    runtime.pathCurveDelta = delta;
    runtime.pathCurveStep = static_cast<int16_t>(runtime.pathCurveBase - delta) / 16;
    runtime.pathYaw = yaw;
}





BikeWaypointAdvanceResult UpdateRotationBikeWaypointTarget(
    CycleManager::NpcRecord &npc, Vector3 &outDirection, Vector3 &outWaypoint, float minWaypointDistSq, int16_t turnAngleLimit, bool doForwardConeCheck) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    uint16_t pathPointIndex = static_cast<uint16_t>(npc.cycle->pathPointIndex);
    const uint16_t startIndex = pathPointIndex;
    const float speedDistSq = (npc.cycle->baseMoveSpeed * npc.cycle->baseMoveSpeed) * 100.0f;
    float turnRadiusSq = 0.0f;
    float forwardX = 0.0f;
    float forwardZ = 0.0f;
    if (doForwardConeCheck) {
        const int16_t halfOutside = static_cast<int16_t>(static_cast<int16_t>(0x8000 - turnAngleLimit) / 2);
        const float radius = (npc.cycle->baseMoveSpeed * 0.5f) / sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(static_cast<uint16_t>(halfOutside)));
        turnRadiusSq = radius * radius;
        forwardX = -sin_fixed16(npc.cycle->desiredPathYaw);
        forwardZ = -sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(static_cast<uint16_t>(npc.cycle->desiredPathYaw)));
    }

    BikeWaypointAdvanceResult status = BikeWaypoint_ReachedStart;
    const Vector2 *const points = reinterpret_cast<const Vector2 *>(npc.cycle->pathPoints);
    const Vector2 *point = nullptr;
    float dx = 0.0f;
    float dz = 0.0f;
    for (;;) {
        point = &points[pathPointIndex];
        dx = point->x - npc.cycle->pos.x;
        dz = point->y - npc.cycle->pos.z;
        const float distSq = dx * dx + dz * dz;
        bool keep = minWaypointDistSq <= distSq && speedDistSq <= distSq;
        if (doForwardConeCheck) {
            float dot = 1.0f;
            if (std::fabs(dx) >= epsilon || std::fabs(dz) >= epsilon) {
                const float length = std::sqrt(dx * dx + dz * dz);
                if (length > 0.0f) {
                    dx = dx * (1.0f / length);
                    dz = dz * (1.0f / length);
                }
                dot = dz * forwardZ + dx * forwardX;
            }
            keep = keep && (turnRadiusSq < distSq || dot > 0.0f);
        }
        if (keep) {
            status = BikeWaypoint_Continuing;
            break;
        }
        npc.cycle->pathSegmentProgress = npc.cycle->pathSegmentProgress + 1.0f;
        if (npc.cycle->reversePathFrames) {
            if (!pathPointIndex)
                break;
            npc.cycle->pathPointIndex = static_cast<int16_t>(--pathPointIndex);
            continue;
        }
        if (pathPointIndex + 1u >= npc.cycle->pathPointCount) {
            npc.cycle->bikeSteeringState = BikeSteering_ReachedEnd;
            status = BikeWaypoint_ReachedEnd;
            break;
        }
        if (!doForwardConeCheck && npc.cycle->moveObjRuntime && pathPointIndex + 5u >= npc.cycle->pathPointCount) {
            npc.cycle->pos.x = point->x;
            npc.cycle->pos.z = point->y;
        }
        npc.cycle->pathPointIndex = static_cast<int16_t>(++pathPointIndex);
    }

    if (CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime; runtime && runtime->moveObjectKind != 5u) {
        if (status == BikeWaypoint_ReachedEnd) {
            runtime->pathStartX = points[startIndex].x;
            runtime->pathStartZ = points[startIndex].y;
        }
        if (pathPointIndex != startIndex && status == BikeWaypoint_Continuing)
            UpdatePathSegmentCache(*runtime, points[startIndex], *point);
    }
    outWaypoint = {point->x, npc.cycle->pos.y, point->y};
    outDirection = {-dx, 0.0f, -dz};
    if (outDirection.x > epsilon || outDirection.x < -epsilon || outDirection.z > epsilon || outDirection.z < -epsilon) {
        Vector3::NormalizeInPlace(outDirection);
    } else {
        outDirection.x = 0.0f;
        outDirection.z = 0.0f;
    }
    return status;
}



void cmRecalcAngleForPos(CycleManager::NpcRecord &npc, const Vector3 &direction) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    if (npc.cycle->reversePathFrames && !(npc.cycle->pathSegmentProgress > 0.0f))
        return;
    npc.cycle->pos.x = npc.cycle->pos.x - npc.cycle->baseMoveSpeed * direction.x;
    npc.cycle->pos.z = npc.cycle->pos.z - npc.cycle->baseMoveSpeed * direction.z;
    if (direction.x > epsilon || direction.x < -epsilon || direction.z > epsilon || direction.z < -epsilon)
        npc.cycle->desiredPathYaw = CycleManager::Atan2Fixed16(direction.x, direction.z);
}




bool BIKEStuff(CycleManager::NpcRecord &npc, Vector3 &direction, const Vector3 &waypoint, Vector3 (&moved)[2], int32_t yawLimit) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
#if SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX
    const float frameScale = GetNpcMovementFrameScale();
    const float speed = npc.cycle->baseMoveSpeed * frameScale;
#else
    const float speed = npc.cycle->baseMoveSpeed;
#endif
    moved[0] = npc.cycle->pos;
    if (speed >= 0.001f) {
        const bool blocked = static_cast<uint8_t>(npc.cycle->bikeSteeringState - 1u) <= 2u;
        const int16_t previousYaw = npc.cycle->desiredPathYaw;
        direction.x = -(waypoint.x - npc.cycle->pos.x);
        direction.z = -(waypoint.z - npc.cycle->pos.z);
        if (direction.x > epsilon || direction.x < -epsilon || direction.z > epsilon || direction.z < -epsilon)
            npc.cycle->desiredPathYaw = CycleManager::Atan2Fixed16(direction.x, direction.z);
        int32_t turn = static_cast<int16_t>(npc.cycle->desiredPathYaw - previousYaw);
        const int32_t turnSize = std::abs(turn);
        if (turnSize > yawLimit)
            npc.cycle->bikeSteeringState = BikeSteering_SharpTurn;
        const int32_t rate = blocked ? 36 : npc.cycle->bikeSteeringState == BikeSteering_ProbeActive ? 16 : 91;
        const BikeSpeedProfile &profile = CycleManager::GetBikeSpeedProfile(npc.cycle->moveObjRuntime);
#if SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX
        const float steeringSpeed = npc.cycle->baseMoveSpeed * frameScale;
#else
        const float steeringSpeed = npc.cycle->baseMoveSpeed;
#endif
        int32_t step = static_cast<int16_t>(static_cast<int32_t>((steeringSpeed / profile.steeringReferenceSpeed) * static_cast<float>(rate)));
        if (!step)
            step = 1;
        int32_t previousTurn = npc.cycle->moveTargetYaw;
        if (previousTurn * turn < 0)
            previousTurn = 0;
        if (turnSize > std::abs(previousTurn + step))
            turn = turn > 0 ? previousTurn + step : previousTurn - step;
        npc.cycle->moveTargetYaw = static_cast<int16_t>(turn);
        npc.cycle->desiredPathYaw = static_cast<int16_t>(previousYaw + turn);
        npc.cycle->pos.x = npc.cycle->pos.x - sin_fixed16(npc.cycle->desiredPathYaw) * speed;
        npc.cycle->pos.z = npc.cycle->pos.z - sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(static_cast<uint16_t>(npc.cycle->desiredPathYaw))) * speed;
    }
    moved[1] = npc.cycle->pos;
    return true;
}

int32_t EvaluateBikeAnimCurve(const CycleManager::NpcRecord &npc, const BikeAnimCurve &curve) {
    const MotionInformationWork *const motion = npc.cycle->mot;
    const float frame = motion ? motion->frameTimer : 1000.0f;
    if (frame <= static_cast<float>(curve.startFrame))
        return curve.startValue;
    if (frame >= static_cast<float>(curve.endFrame))
        return curve.endValue;
    const float amount = (frame - static_cast<float>(curve.startFrame)) / static_cast<float>(curve.endFrame - curve.startFrame);
    return static_cast<int32_t>(static_cast<float>(curve.startValue) + amount * static_cast<float>(curve.endValue - curve.startValue));
}


void RotateForBIKE(CycleMoveObjectRuntime &runtime, int32_t slope, int32_t yaw, int32_t lean) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<uint16_t>(yaw));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<uint16_t>(lean));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<uint16_t>(slope));
    const Matrix4x4 pose = workTransformMatrix;
    Matrix4x4::Pop();
    runtime.renderEuler = CHAR_EulerAnglesFromMatrix(pose);
}

void UpdateBikeAnimCurveOutputs(CycleManager::NpcRecord &npc, CycleMoveObjectRuntime &runtime, bool composeRenderEuler) {
    int32_t slope = 0;
    int32_t lean = 0;
    runtime.animOutput = 0;
    if (npc.moveConfigOffset && npc.bikeToken && npc.bikeToken->anims) {
        const BikeAnimTokenSet &anims = *npc.bikeToken->anims;
        if (runtime.moveObjectId == SHEN_NAME(BIKE)) {
            const std::array<BikeAnimCurve, 8> *curves = nullptr;
            if (npc.cycle->moveConfig->obj_id.name == SHEN_NAME(YKHI)) {
                curves = &kYkhiBikeAnimCurves;
            } else if (npc.cycle->moveConfig->obj_id.name == SHEN_NAME(SOBA)) {
                curves = &kSobaBikeAnimCurves;
            }
            if (curves) {
                if (npc.cycle->activeAnimId == static_cast<uint16_t>(anims.anim[4])) {
                    lean = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[2]);
                } else if (npc.cycle->activeAnimId == static_cast<uint16_t>(anims.anim[1])) {
                    lean = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[1]);
                    slope = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[4]);
                    runtime.animOutput = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[6]);
                } else if (npc.cycle->activeAnimId == static_cast<uint16_t>(anims.anim[0])) {
                    lean = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[0]);
                    slope = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[5]);
                    runtime.animOutput = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[7]);
                } else if (npc.cycle->activeAnimId == static_cast<uint16_t>(anims.anim[3])) {
                    lean = CycleManager::EvaluateBikeAnimCurve(npc, (*curves)[3]);
                }
                runtime.tokenLocalOffset.y = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<int16_t>(slope))) * -0.83);
            }
        } else if ((runtime.moveObjectId & 0x00ffffffu) == (SHEN_NAME(HAL0) & 0x00ffffffu)) {
            if (npc.cycle->activeAnimId == static_cast<uint16_t>(anims.anim[0])) {
                lean = CycleManager::EvaluateBikeAnimCurve(npc, kHalBikeAnimCurves[0]);
            } else if (npc.cycle->activeAnimId == static_cast<uint16_t>(anims.anim[1])) {
                lean = CycleManager::EvaluateBikeAnimCurve(npc, kHalBikeAnimCurves[1]);
            }
        }
    }
    if (!composeRenderEuler) {
        runtime.slopeAngleX = slope;
        runtime.animLeanAngle = lean;
        return;
    }
    RotateForBIKE(runtime, slope, runtime.yaw, lean);
}



void DoBikeAngles(CycleManager::NpcRecord &npc, CycleMoveObjectRuntime &runtime, float speed) {
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!motion || runtime.moveObjectId != SHEN_NAME(BIKE))
        return;
    std::uint32_t waitAnim = 0;
    std::memcpy(&waitAnim, &runtime.waitAnim, sizeof(waitAnim));
    if (npc.cycle->activeAnimId == waitAnim) {
        motion->playbackRate = 1.0f;
        runtime.wheelSpin = 0;
    } else {
        motion->playbackRate = npc.cycle->moveObjSpeedParam == 0.0f ? 0.0f : speed / npc.cycle->moveObjSpeedParam;
        runtime.wheelSpin = static_cast<int32_t>(MOTI_GetPlayRateScaledRemain(*motion) / motion->frameEnd * 65536.0f);
    }
    if (npc.cycle->moveConfig->obj_id.name == SHEN_NAME(SOBA))
        runtime.wheelSpin = static_cast<int16_t>(static_cast<uint16_t>(runtime.wheelSpin) + 0x8000U);
}




void BIKE_UpdateCurveLean(CycleManager::NpcRecord &npc, CycleMoveObjectRuntime &runtime) {
    const BikeMoveObjectToken *const token = CycleManager::GetBikeMoveObjectToken(npc.cycle->moveConfig->obj_id.name, runtime.moveObjectId);
    float speedScale = 0.0f;
    switch (token->speed_curve_mode) {
    case 0:
        speedScale = npc.cycle->baseMoveSpeed / 0.1388888955116272f * 0.5f;
        break;
    case 1:
    case 3:
        speedScale = npc.cycle->baseMoveSpeed / 0.1805555522441864f * 0.6000000238418579f;
        break;
    case 2:
        speedScale = npc.cycle->baseMoveSpeed / 0.18518519401550293f;
        break;
    default:
        break;
    }
    const float scale = 1.0f < speedScale ? 1.0f : speedScale;
    const auto base = static_cast<uint16_t>(runtime.pathCurveBase);
    const int32_t distance = std::abs(static_cast<int32_t>(static_cast<int16_t>(base - static_cast<uint16_t>(runtime.pathCurveDelta))));
    const int32_t newBase =
        distance <= std::abs(runtime.pathCurveStep) ? runtime.pathCurveDelta : static_cast<int16_t>(base - static_cast<uint16_t>(runtime.pathCurveStep));
    runtime.pathCurveBase = static_cast<int16_t>(newBase);
    runtime.unk11E = static_cast<int16_t>(static_cast<uint32_t>(newBase) >> 16);
    runtime.leanHistory[1] = runtime.leanHistory[0];
    runtime.leanHistory[0] = runtime.animLeanAngle;
    const int32_t lean = static_cast<int32_t>(static_cast<float>(newBase) * scale);
    runtime.animLeanAngle = lean > 6371 ? 6371 : lean < -6371 ? -6371 : lean;
    runtime.animLeanAngle = (runtime.leanHistory[1] + runtime.animLeanAngle + runtime.leanHistory[0]) / 3;
}




void FindForwardSteeringObstacleInList(CycleManager::NpcRecord &npc, uint32_t objectId, float minAhead, float halfDepth, float maxAhead,
    CycleProgramList list, float &nearest, float &nearestRadius) {
    for (CycleManager::NpcRecord *other = CycleManager::NextProgramInList(list); other; other = CycleManager::NextProgramInList(list, other)) {
        if (!other->cycle->motnSteeringActive || other->cycle->moveConfig->obj_id.name == objectId || other->cycle->trackedMobj == npc.cycle ||
            other->cycle->moveObjRuntime || static_cast<int32_t>(other->cycle->moveConfig->cycleClassCode) >= 4 ||
            CycleManager::ApproachSteeringTargetXZ(*other, 0.5f))
            continue;
        Vector3 local{};
        Matrix4x4::TransformPoint(other->cycle->pos, local);
        const float radius = other->cycle->steeringHalfDepth > 0.0f ?
            std::sqrt(other->cycle->steeringHalfWidth * other->cycle->steeringHalfWidth + other->cycle->steeringHalfDepth * other->cycle->steeringHalfDepth) :
            other->cycle->steeringHalfWidth;
        if (radius + halfDepth > std::fabs(local.x) && -minAhead >= local.z && local.z >= -maxAhead && local.z > nearest) {
            nearest = local.z;
            nearestRadius = radius;
        }
    }
}




bool CheckForwardSteeringClearance(CycleManager::NpcRecord &npc, float reach, float &nearest) {
    const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    nearest = -100.0f;
    const float minAhead = CycleManager::GetMoveObjectShapeOffset(npc) + npc.cycle->steeringHalfWidth;
    const float halfDepth = npc.cycle->steeringHalfDepth;
    const float maxAhead = reach + minAhead;
    Vector3 ryo{};
    Matrix4x4::TransformPoint(cm.center, ryo);
    const float turningRadius = g_fGlobalTurningRadius;
    const float boxDepth = g_default_turning_box_half_depth_z;
    float nearestRadius = boxDepth > 0.0f ? std::sqrt(turningRadius * turningRadius + boxDepth * boxDepth) : turningRadius;
    if (npc.cycle->moveObjRuntime && npc.cycle->moveObjRuntime->moveObjectKind != 5u)
        nearestRadius = nearestRadius * 0.64999998f;
    if (nearestRadius + halfDepth >= std::fabs(ryo.x) && -minAhead >= ryo.z && ryo.z >= -maxAhead) {
        nearest = ryo.z;
        nearestRadius = turningRadius;
    }
    CycleManager::FindForwardSteeringObstacleInList(
        npc, npc.cycle->moveConfig->obj_id.name, minAhead, halfDepth, maxAhead, CycleProgramList_SimSet, nearest, nearestRadius);
    CycleManager::FindForwardSteeringObstacleInList(
        npc, npc.cycle->moveConfig->obj_id.name, minAhead, halfDepth, maxAhead, CycleProgramList_Running, nearest, nearestRadius);
    if (nearest < -maxAhead || nearest > 0.0f)
        return false;
    const float stopAt = -((nearestRadius + minAhead) + 1.5f);
    if (nearest >= stopAt) {
        npc.cycle->approachStartSpeed = 0.0f;
        return true;
    }
    const float scale = (stopAt - nearest) / (maxAhead - (minAhead + 1.5f));
    if (std::fabs(scale) >= std::numeric_limits<float>::epsilon()) {
        const float speed = scale * npc.cycle->moveObjSpeedParam;
        npc.cycle->approachStartSpeed = npc.cycle->baseMoveSpeed <= speed ? npc.cycle->baseMoveSpeed * scale : speed;
    } else {
        npc.cycle->approachStartSpeed = 0.0f;
    }
    if (npc.cycle->approachStartSpeed > npc.cycle->baseMoveSpeed)
        npc.cycle->approachStartSpeed = npc.cycle->baseMoveSpeed;
    return true;
}






void FindSteeringOverlapInList(
    CycleManager::NpcRecord &npc, float halfWidth, CycleProgramList list, float &nearest, CycleManager::NpcRecord *&overlap, int32_t &sameWay, bool probe) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    const int16_t yaw = npc.cycle->desiredPathYaw;
    const float forwardX = -sin_fixed16(yaw);
    const float forwardZ = -sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(static_cast<uint16_t>(yaw)));
    Vector3 box[4]{};
    ApplyRotationMatrix({.center = &npc.cycle->pos,
        .yaw = yaw,
        .half_width = 5.0f,
        .half_depth = npc.cycle->steeringHalfDepth,
        .yaw_offset = npc.cycle->steeringHalfWidth + 5.0f,
        .out_corners = box});
    Vector3 points[5]{};
    for (CycleManager::NpcRecord *other = CycleManager::NextProgramInList(list); other; other = CycleManager::NextProgramInList(list, other)) {
        if (!other->cycle->motnSteeringActive || other->cycle->moveConfig->obj_id.name == npc.cycle->moveConfig->obj_id.name ||
            other->cycle->trackedMobj == npc.cycle)
            continue;
        const CycleMoveObjectRuntime *const runtime = other->cycle->moveObjRuntime;
        if (!runtime && static_cast<int32_t>(other->cycle->moveConfig->cycleClassCode) < 4)
            continue;
        if (other->cycle->moveConfig->cycleClassCode == 7u && other->cycle->currentOpcode != CycleOpcode_DriveVehiclePath2D)
            continue;
        if (probe && (!runtime || runtime->moveObjectKind == 5u))
            continue;
        const int16_t otherYaw = other->cycle->desiredPathYaw;
        const int32_t turn = std::abs(static_cast<int32_t>(static_cast<int16_t>(yaw - otherYaw)));
        const bool vehicle = runtime && !(runtime->moveObjectKind == 5u && turn <= 0x4000);
        int32_t together = 1;
        if (turn > 0x4000) {
            if (!vehicle)
                continue;
            together = 0;
        }
        if (other->cycle->moveConfig->cycleClassCode != 6u && together && other->cycle->baseMoveSpeed > npc.cycle->baseMoveSpeed)
            continue;
        float dx = other->cycle->pos.x - npc.cycle->pos.x;
        float dz = other->cycle->pos.z - npc.cycle->pos.z;
        const float reach = (halfWidth + 6.5f) + other->cycle->steeringHalfWidth;
        const float distSq = dx * dx + dz * dz;
        if (std::fabs(dx) >= epsilon || std::fabs(dz) >= epsilon) {
            const float length = std::sqrt(dx * dx + dz * dz);
            if (length > 0.0f) {
                dx = dx * (1.0f / length);
                dz = dz * (1.0f / length);
            }
        }
        if (distSq >= reach * reach || dz * forwardZ + dx * forwardX <= 0.0f)
            continue;
        ApplyRotationMatrix({.center = &other->cycle->pos,
            .yaw = otherYaw,
            .half_width = other->cycle->steeringHalfWidth,
            .half_depth = other->cycle->steeringHalfDepth,
            .yaw_offset = CycleManager::GetMoveObjectShapeOffset(*other),
            .out_corners = points});
        points[4].x = other->cycle->pos.x;
        points[4].z = other->cycle->pos.z;
        if (vehicle) {
            for (const Vector3 &point : points) {
                if (!check_if_point_in_quadrilateral(box[0], box[1], box[2], box[3], point))
                    continue;
                Vector3 local{};
                Matrix4x4::TransformPoint(point, local);
                if (local.z > nearest) {
                    nearest = local.z;
                    overlap = other;
                    sameWay = together;
                }
            }
            continue;
        }
        const float distance = std::sqrt(distSq);
        const uint16_t cone = static_cast<uint16_t>(static_cast<int32_t>((((20.0f - (20.0f < distance ? 20.0f : distance)) + 50.0f) * 8192.0f) / 45.0f));
        for (const Vector3 &point : points) {
            float px = point.x - npc.cycle->pos.x;
            float pz = point.z - npc.cycle->pos.z;
            float dot = 1.0f;
            if (std::fabs(px) >= epsilon || std::fabs(pz) >= epsilon) {
                const float length = std::sqrt(px * px + pz * pz);
                if (length > 0.0f) {
                    px = px * (1.0f / length);
                    pz = pz * (1.0f / length);
                }
                dot = pz * forwardZ + px * forwardX;
            }
            if (dot <= sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(cone)))
                continue;
            Vector3 local{};
            Matrix4x4::TransformPoint(point, local);
            if (local.z > nearest) {
                nearest = local.z;
                overlap = other;
                sameWay = together;
            }
        }
    }
}





bool CheckSteeringOverlap(CycleManager::NpcRecord &npc, float &nearest, bool probe) {
    npc.cycle->cycleObjectIdCopy = npc.cycle->cycleObjectId;
    const float halfWidth = npc.cycle->steeringHalfWidth;
    nearest = -100.0f;
    CycleManager::NpcRecord *overlap = nullptr;
    int32_t sameWay = 1;
    CycleManager::FindSteeringOverlapInList(npc, halfWidth, CycleProgramList_SimSet, nearest, overlap, sameWay, probe);
    CycleManager::FindSteeringOverlapInList(npc, halfWidth, CycleProgramList_Running, nearest, overlap, sameWay, probe);
    if (!overlap) {
        if (!probe) {
            npc.cycle->cycleObjectId = npc.cycle->moveConfig->obj_id.name;
            npc.cycle->selfCyclePtr = npc.cycle;
            npc.selfCycleIndex = static_cast<int32_t>(&npc - g_cycleManagerRuntime.npcRecords.data());
        }
        return false;
    }
    bool blocked = true;
    if (npc.cycle->bikeSteeringState == BikeSteering_FrontBlocked) {
        blocked = false;
    } else {
        const float stopAt = -((halfWidth + overlap->cycle->steeringHalfWidth) + 1.5f);
        if (nearest >= stopAt) {
            npc.cycle->approachStartSpeed = 0.0f;
        } else {
            const float scale = (stopAt - nearest) / 5.0f;
            if (std::fabs(scale) >= std::numeric_limits<float>::epsilon())
                npc.cycle->approachStartSpeed = (sameWay ? (1.0f - scale) * overlap->cycle->baseMoveSpeed : 0.0f) + scale * npc.cycle->moveObjSpeedParam;
            else
                npc.cycle->approachStartSpeed = 0.0f;
        }
        if (npc.cycle->approachStartSpeed > npc.cycle->baseMoveSpeed)
            npc.cycle->approachStartSpeed = npc.cycle->baseMoveSpeed;
        npc.cycle->approachStartSpeed = npc.cycle->approachStartSpeed * 0.89999998f;
        if (sameWay && static_cast<int32_t>(overlap->cycle->reversePathFrames) > 0 && overlap->cycle->cycleObjectId != npc.cycle->moveConfig->obj_id.name)
            npc.cycle->steeringWaitFrames = 15;
    }
    if (!probe) {
        npc.cycle->cycleObjectId = overlap->cycle->moveConfig->obj_id.name;
        npc.cycle->selfCyclePtr = overlap->cycle;
        npc.selfCycleIndex = static_cast<int32_t>(overlap - g_cycleManagerRuntime.npcRecords.data());
    }
    return blocked;
}




void UpdateBikeClearanceBlockTimer(CycleManager::NpcRecord &npc, CycleMoveObjectRuntime &runtime, float speed, int16_t maxClearanceTime) {
    if (runtime.moveObjectKind == 5u && npc.steeringCollisionResult) {
        npc.cycle->bikeClearanceTimer = 0;
        runtime.clearanceCooldown = 60;
        npc.cycle->bikeClearanceBlocked = true;
        return;
    }
    const uint32_t objectId = npc.cycle->moveConfig->obj_id.name;
    if (npc.cycle->cycleObjectId != objectId) {
        if (npc.cycle->cycleObjectId == npc.cycle->cycleObjectIdCopy) {
            const CycleManager::NpcRecord &other = g_cycleManagerRuntime.npcRecords[static_cast<size_t>(npc.selfCycleIndex)];
            if (objectId == other.cycle->cycleObjectId && other.cycle->moveObjRuntime) {
                if (static_cast<int16_t>(++npc.cycle->bikeClearanceTimer) > maxClearanceTime) {
                    npc.cycle->bikeClearanceTimer = 0;
                    if (static_cast<int16_t>(other.cycle->bikeClearanceTimer) > 0) {
                        runtime.clearanceCooldown = 60;
                        npc.cycle->bikeClearanceBlocked = true;
                    }
                }
                return;
            }
        }
    } else if (npc.cycle->bikeSteeringState == BikeSteering_FrontBlocked) {
        npc.cycle->bikeClearanceTimer = 0;
        return;
    }
    if (speed > 0.001f) {
        npc.cycle->bikeClearanceTimer = 0;
        return;
    }
    if (static_cast<float>(static_cast<int16_t>(++npc.cycle->bikeClearanceTimer)) > 450.0f) {
        npc.cycle->bikeClearanceTimer = 0;
        runtime.clearanceCooldown = 90;
        npc.cycle->bikeClearanceBlocked = true;
    }
}








bool TickBikeMoveObjectRuntime(CycleManager::NpcRecord &npc, bool live) {
    CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime;
    if (!runtime)
        return false;
    bool ticking = false;
    if (live && npc.cycle->bikeSteeringState == BikeSteering_PathOnly && CycleManager::CheckClearanceAgainstRyoAndCycles(npc, true)) {
        npc.cycle->bikeClearanceBlocked = true;
        live = false;
        npc.cycle->bikeClearanceTimer = 0;
        runtime->clearanceCooldown = 15;
    }
    float speed = CycleManager::SnapshotMoveObjTransform(npc, live);
    if (npc.cycle->primarySoundBankId >= 0)
        PerformBikeSoundUpdate(npc, speed);
    Vector3 waypoint{};
    if (UpdateRotationBikeWaypointTarget(npc, npc.cycle->moveTargetPos, waypoint, 9.0f, 273, live) == BikeWaypoint_ReachedEnd) {
        if (runtime->moveObjectKind == 5u && static_cast<int32_t>(npc.cycle->reversePathFrames) > 0)
            npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000);
        if (live)
            runtime->renderOpacity = npc.cycle->obj ? npc.cycle->obj->renderOpacity : 0.0f;
        npc.cycle->bikeClearanceBlocked = false;
        npc.cycle->bikeClearanceTimer = 0;
        runtime->clearanceCooldown = 0;
    } else {
        if (static_cast<int32_t>(npc.cycle->steeringWaitFrames) > 0)
            --npc.cycle->steeringWaitFrames;
        const uint16_t tickAnimId = npc.cycle->activeAnimId;
        if (!live) {
            if (npc.cycle->bikeClearanceBlocked && static_cast<int8_t>(runtime->clearanceCooldown) > 0 && !--runtime->clearanceCooldown)
                npc.cycle->bikeClearanceBlocked = false;
            cmRecalcAngleForPos(npc, npc.cycle->moveTargetPos);
            BIKE_UpdateCurveLean(npc, *runtime);
            npc.cycle->bikeSteeringState = BikeSteering_PathOnly;
            npc.cycle->steeringWaitFrames = 0;
            runtime->steeringAngle = 0;
        } else {
            runtime->renderOpacity = npc.cycle->obj ? npc.cycle->obj->renderOpacity : 0.0f;
            npc.cycle->activeAnimId = static_cast<uint16_t>(runtime->activeAnim);
            const int16_t previousYaw = npc.cycle->desiredPathYaw;
            npc.cycle->bikeSteeringState = BikeSteering_ProbeActive;
            if (MotionInformationWork *const motion = npc.cycle->mot) {
                motion->mobjCallback = reinterpret_cast<void *>(MOBJ_BikeMotionConstraintCallback);
                motion->mobjCallbackUser = &npc;
            }
            if (static_cast<int32_t>(npc.cycle->steeringWaitFrames) > 0) {
                npc.cycle->bikeSteeringState = BikeSteering_FrontBlocked;
                npc.cycle->activeAnimId = static_cast<uint16_t>(runtime->waitAnim);
            } else {
                Matrix4x4::SetActiveTransformToPrePass();
                Matrix4x4::RotateAroundY_Wrapper(static_cast<uint16_t>(-npc.cycle->desiredPathYaw));
                Matrix4x4::preTranslate(-npc.cycle->pos.x, -npc.cycle->pos.y, -npc.cycle->pos.z);
                Vector3 moved[2]{};
                if (BIKEStuff(npc, npc.cycle->moveTargetPos, waypoint, moved, 273)) {
                    float nearest = 0.0f;
                    if (CycleManager::CheckForwardSteeringClearance(npc, 6.5f, nearest)) {
                        npc.cycle->bikeSteeringState = BikeSteering_FrontBlocked;
                        if (speed <= 0.001f) {
                            npc.cycle->steeringWaitFrames = 45;
                            npc.cycle->activeAnimId = static_cast<uint16_t>(runtime->waitAnim);
                        }
                    }
                    if (CycleManager::CheckSteeringOverlap(npc, nearest, false)) {
                        if (speed <= 0.001f) {
                            npc.cycle->activeAnimId = static_cast<uint16_t>(runtime->waitAnim);
                            npc.cycle->bikeSteeringState = BikeSteering_SideBlockedStop;
                        } else {
                            npc.cycle->bikeSteeringState = BikeSteering_SideAvoidMoving;
                        }
                    } else if (npc.cycle->bikeSteeringState == BikeSteering_ProbeActive) {
                        npc.cycle->bikeSteeringState = BikeSteering_Normal;
                        if (speed <= 0.001f)
                            npc.cycle->activeAnimId = static_cast<uint16_t>(runtime->waitAnim);
                    }
                } else {
                    npc.cycle->bikeSteeringState = BikeSteering_FrontBlocked;
                    if (speed <= 0.001f) {
                        npc.cycle->steeringWaitFrames = 45;
                        npc.cycle->activeAnimId = static_cast<uint16_t>(runtime->waitAnim);
                    }
                }
                Matrix4x4::Pop(1);
                UpdateBikeClearanceBlockTimer(npc, *runtime, speed, 300);
            }

            const int32_t turn = static_cast<int16_t>(npc.cycle->desiredPathYaw - previousYaw);
            int32_t step = runtime->steeringStep;
            if (step * turn < 0)
                step = 0;
            int16_t applied = 0;
            if (!turn) {
                const int32_t steering = runtime->steeringAngle;
                step = 0;
                if (steering) {
                    const int32_t decay = steering < 0 ? 364 : -364;
                    applied = steering * (steering + decay) < 0 ? 0 : static_cast<int16_t>(decay);
                }
            } else if (std::abs(turn) > std::abs(step) + 364) {
                step = turn > 0 ? step + 364 : step - 364;
                applied = static_cast<int16_t>(step);
            } else {
                step = turn;
                applied = static_cast<int16_t>(turn);
            }
            runtime->steeringAngle += applied;
            runtime->steeringStep = step;

            std::uint32_t waitAnim = 0;
            std::memcpy(&waitAnim, &runtime->waitAnim, sizeof(waitAnim));
            if (npc.cycle->activeAnimId == waitAnim && tickAnimId == npc.cycle->activeAnimId)
                CycleManager::UpdateBikeAnimCurveOutputs(npc, *runtime, false);
            else
                BIKE_UpdateCurveLean(npc, *runtime);

            const uint16_t yaw = static_cast<uint16_t>(npc.cycle->desiredPathYaw);
            const float forwardX = -sin_fixed16(static_cast<int16_t>(yaw));
            const float forwardZ = -sin_fixed16(static_cast<int16_t>(0x4000 - CycleManager::FoldUnsignedYaw(yaw)));
            const float along = CycleManager::GetMoveObjectShapeOffset(npc);
            const float halfX = npc.cycle->steeringHalfWidth * forwardX;
            const float alongX = along * forwardX;
            const float halfZ = npc.cycle->steeringHalfWidth * forwardZ;
            const float alongZ = along * forwardZ;
            const Vector3 front{(halfX + npc.cycle->pos.x) + alongX, npc.cycle->pos.y, (halfZ + npc.cycle->pos.z) + alongZ};
            const float frontY = MapProbe_UpdateIntersectionStateAtPoint(front);
            const Vector3 back{(npc.cycle->pos.x - halfX) + alongX, npc.cycle->pos.y, (npc.cycle->pos.z - halfZ) + alongZ};
            const float distance = std::sqrt((front.z - back.z) * (front.z - back.z) + (front.x - back.x) * (front.x - back.x));
            const float backY = MapProbe_UpdateIntersectionStateAtPoint(back);
            int16_t slopeStep = static_cast<int16_t>(atan2_fixed16(frontY - backY, distance) - static_cast<uint16_t>(runtime->slopeAngleX));
            if (slopeStep <= -183)
                slopeStep = -182;
            runtime->slopeAngleX += slopeStep;

            const int32_t steeringClamp = CycleManager::GetBikeMoveObjectToken(npc.cycle->moveConfig->obj_id.name, runtime->moveObjectId)->steering_clamp;
            if (runtime->steeringAngle > steeringClamp)
                runtime->steeringAngle = steeringClamp;
            else if (runtime->steeringAngle < -steeringClamp)
                runtime->steeringAngle = -steeringClamp;
            if (speed <= 0.001f)
                speed = 0.0f;
            runtime->yawVelocity += static_cast<int32_t>((speed / 2.198f) * 65536.0f);
            DoBikeAngles(npc, *runtime, speed);
            runtime->yaw = npc.cycle->desiredPathYaw;
            RotateForBIKE(*runtime, runtime->slopeAngleX, runtime->yaw, runtime->animLeanAngle);
            runtime->unkC4 = 0;
            runtime->yawRuntime = static_cast<std::uint32_t>(static_cast<int32_t>(npc.cycle->desiredPathYaw));
        }
        ticking = true;
    }
    runtime->yaw = npc.cycle->desiredPathYaw;
    if (live || (npc.cycle->areaID.name != SHEN_NAME(MFSY) && ((npc.cycle->moveConfig->obj_id.name - SHEN_NAME(BIKA)) & 0xFEFFFFFFu) != 0))
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
    runtime->pos = npc.cycle->pos;
    return ticking;
}







bool DoBikeStuff(CycleManager::NpcRecord &npc, bool live) {
    CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime;
    if (!runtime)
        return false;
    bool ticking = true;
    if (live && npc.cycle->bikeSteeringState == BikeSteering_PathOnly && CycleManager::CheckClearanceAgainstRyoAndCycles(npc, true)) {
        npc.cycle->bikeClearanceBlocked = true;
        live = false;
        npc.cycle->bikeClearanceTimer = 0;
        runtime->clearanceCooldown = 15;
    }
    const float speed = CycleManager::SnapshotMoveObjTransform(npc, live);
    if (npc.cycle->primarySoundBankId >= 0)
        PerformBikeSoundUpdate(npc, speed);
    CycleManager::UpdateBikeReverseAvoidance(npc, speed, live);
    Vector3 waypoint{};
    const BikeWaypointAdvanceResult status = UpdateRotationBikeWaypointTarget(npc, npc.cycle->moveTargetPos, waypoint, 1.0f, 546, live);
    const int32_t reversing = static_cast<int32_t>(npc.cycle->reversePathFrames);
    if (status != BikeWaypoint_Continuing) {
        if (runtime->moveObjectKind == 5u && reversing > 0)
            npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000);
        if (live)
            runtime->renderOpacity = npc.cycle->obj ? npc.cycle->obj->renderOpacity : 0.0f;
        if (status == BikeWaypoint_ReachedEnd) {
            npc.cycle->bikeClearanceBlocked = false;
            ticking = false;
            npc.cycle->bikeClearanceTimer = 0;
            runtime->clearanceCooldown = 0;
        }
    } else if (!live) {
        if (npc.cycle->bikeClearanceBlocked && static_cast<int8_t>(runtime->clearanceCooldown) > 0 && !--runtime->clearanceCooldown)
            npc.cycle->bikeClearanceBlocked = false;
        cmRecalcAngleForPos(npc, npc.cycle->moveTargetPos);
        npc.cycle->bikeSteeringState = BikeSteering_PathOnly;
        if (reversing > 0)
            npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000);
    } else {
        runtime->renderOpacity = npc.cycle->obj ? npc.cycle->obj->renderOpacity : 0.0f;
        npc.cycle->bikeSteeringState = BikeSteering_ProbeActive;
        const int16_t previousYaw = npc.cycle->desiredPathYaw;
        Vector3 moved[2]{};
        BIKEStuff(npc, npc.cycle->moveTargetPos, waypoint, moved, 546);
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<uint16_t>(-npc.cycle->desiredPathYaw));
        Matrix4x4::preTranslate(-npc.cycle->pos.x, -npc.cycle->pos.y, -npc.cycle->pos.z);
        float nearest = 0.0f;
        const bool frontBlocked = CycleManager::CheckForwardSteeringClearance(npc, 6.5f, nearest);
        bool overlapped = false;
        if (reversing) {
            if (frontBlocked || CycleManager::CheckSteeringOverlap(npc, nearest, true)) {
                npc.cycle->pos = npc.cycle->lastMoveObjPos;
                npc.cycle->desiredPathYaw = static_cast<int16_t>(static_cast<uint16_t>(npc.cycle->lastMoveObjYaw) + 0x8000);
                npc.cycle->baseMoveSpeed = 0.0f;
            }
        } else {
            if (frontBlocked) {
                npc.cycle->bikeSteeringState = BikeSteering_FrontBlocked;
                if (speed <= 0.001f) {
                    npc.cycle->pos = npc.cycle->lastMoveObjPos;
                    npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->lastMoveObjYaw);
                    npc.cycle->baseMoveSpeed = 0.0f;
                }
            }
            overlapped = CycleManager::CheckSteeringOverlap(npc, nearest, false);
            if (overlapped) {
                if (speed <= 0.001f) {
                    npc.cycle->pos = npc.cycle->lastMoveObjPos;
                    npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->lastMoveObjYaw);
                    npc.cycle->baseMoveSpeed = 0.0f;
                    npc.cycle->bikeSteeringState = BikeSteering_SideBlockedStop;
                } else {
                    npc.cycle->bikeSteeringState = BikeSteering_SideAvoidMoving;
                }
            }
        }
        if (!overlapped && npc.cycle->bikeSteeringState == BikeSteering_ProbeActive)
            npc.cycle->bikeSteeringState = BikeSteering_Normal;
        Matrix4x4::Pop(1);
        UpdateBikeClearanceBlockTimer(npc, *runtime, speed, 150);

        const int16_t travelYaw = moved[0].x == npc.cycle->pos.x && moved[0].z == npc.cycle->pos.z ?
            previousYaw :
            CycleManager::Atan2Fixed16(moved[0].x - npc.cycle->pos.x, moved[0].z - npc.cycle->pos.z);
        const int32_t steer = std::clamp<int32_t>(
            static_cast<int16_t>(static_cast<uint16_t>(travelYaw - previousYaw) - static_cast<uint16_t>(runtime->steeringAngle)), -182, 182);
        runtime->steeringAngle = std::clamp(runtime->steeringAngle + steer, -5461, 5461);
        int32_t roll = static_cast<int32_t>((speed / 2.198f) * 65536.0f);
        if (reversing > 0)
            roll = -roll;
        runtime->yawVelocity += roll;
        if (reversing > 0)
            npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000);
        runtime->renderEuler.y = npc.cycle->desiredPathYaw;
    }
    runtime->yaw = npc.cycle->desiredPathYaw;
    if (live && --npc.cycle->map_probe_cooldown <= 0) {
        npc.cycle->map_probe_cooldown = 15;
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
    }
    runtime->pos = npc.cycle->pos;
    return ticking;
}









bool DoBikeStuff_0(CycleManager::NpcRecord &npc, bool live) {
    npc.cycle->flags &= ~2u;
    if (npc.cycle->moveConfig->cycleClassCode == 6u)
        npc.cycle->flags &= ~0x60u;
    if (npc.cycle->pathPointIndex == 0 || npc.cycle->moveConfig->cycleClassCode == 6u)
        npc.cycle->flags &= ~0x60u;
    if (live && npc.cycle->bikeSteeringState == BikeSteering_PathOnly)
        CycleManager::AdjustDrivePathForClearance(npc);
    const float speed = CycleManager::SnapshotMoveObjTransform(npc, live);
    if (npc.cycle->primarySoundBankId >= 0)
        PerformBikeSoundUpdate(npc, speed);
    CycleManager::UpdateBikeReverseAvoidance(npc, speed, live);
    Vector3 waypoint{};
    const BikeWaypointAdvanceResult status = UpdateRotationBikeWaypointTarget(npc, npc.cycle->moveTargetPos, waypoint, 0.25f, 364, live);
    bool ticking = true;
    if (status == BikeWaypoint_ReachedEnd) {
        if (npc.cycle->primarySoundBankId >= 0) {
            ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
            npc.cycle->primarySoundBankId = -1;
        }
        if (npc.cycle->secondarySoundBankId >= 0) {
            ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->secondarySoundBankId));
            npc.cycle->secondarySoundBankId = -1;
        }
        ticking = false;
    } else if (status == BikeWaypoint_Continuing && !live) {
        cmRecalcAngleForPos(npc, npc.cycle->moveTargetPos);
        npc.cycle->bikeSteeringState = BikeSteering_PathOnly;
    } else if (status == BikeWaypoint_Continuing) {
        npc.cycle->bikeSteeringState = BikeSteering_ProbeActive;
        const int16_t previousYaw = npc.cycle->desiredPathYaw;
        if (npc.cycle->baseMoveSpeed > 0.001f) {
            const Vector3 previousPos = npc.cycle->pos;
            const uint16_t yaw = static_cast<uint16_t>(previousYaw);
            const Vector3 tangent{sin_fixed16(yaw), 0.0f, sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(yaw))};
            std::array<Vector3, 4> control{Vector3{previousPos.x + tangent.x, previousPos.y, previousPos.z + tangent.z}, previousPos,
                Vector3{waypoint.x, previousPos.y, waypoint.z}, Vector3{waypoint.x - tangent.x, previousPos.y, waypoint.z - tangent.z}};
            const int32_t adjacentIndex = npc.cycle->pathPointIndex + (npc.cycle->reversePathFrames ? -1 : 1);
            if (adjacentIndex >= 0 && adjacentIndex < static_cast<int32_t>(npc.cycle->pathPointCount)) {
                const Vector2 &adjacent = reinterpret_cast<const Vector2 *>(npc.cycle->pathPoints)[adjacentIndex];
                control[3].x = adjacent.x;
                control[3].z = adjacent.y;
            }
            UpdateAnimationCurvePoints(control);
            CalculateCubicPolynomial(npc.cycle->pos, npc.cycle->baseMoveSpeed, 0.0f);
            const float dx = previousPos.x - npc.cycle->pos.x;
            const float dz = previousPos.z - npc.cycle->pos.z;
            constexpr float epsilon = std::numeric_limits<float>::epsilon();
            if (std::fabs(dx) >= epsilon || std::fabs(dz) >= epsilon) {
                const int16_t turn = static_cast<int16_t>(CycleManager::Atan2Fixed16(dx, dz) - previousYaw);
                if (std::abs(static_cast<int32_t>(turn)) > 364)
                    npc.cycle->bikeSteeringState = BikeSteering_SharpTurn;
                npc.cycle->desiredPathYaw = static_cast<int16_t>(previousYaw + std::clamp<int32_t>(turn, -364, 364));
            }
            if (!npc.cycle->reversePathFrames && CycleManager::CheckClearanceAgainstRyoAndCycles(npc, false)) {
                npc.cycle->pos = previousPos;
                npc.cycle->desiredPathYaw = previousYaw;
            }
        }
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<uint16_t>(-npc.cycle->desiredPathYaw));
        Matrix4x4::preTranslate(-npc.cycle->pos.x, -npc.cycle->pos.y, -npc.cycle->pos.z);
        float nearest = 0.0f;
        if (npc.cycle->reversePathFrames) {
            if (CycleManager::CheckForwardSteeringClearance(npc, 7.0f, nearest) || CycleManager::CheckSteeringOverlap(npc, nearest, true)) {
                npc.cycle->pos = npc.cycle->lastMoveObjPos;
                npc.cycle->desiredPathYaw = static_cast<int16_t>(static_cast<uint16_t>(npc.cycle->lastMoveObjYaw) + 0x8000);
                npc.cycle->baseMoveSpeed = 0.0f;
            }
        } else {
            if (npc.cycle->moveConfig->cycleClassCode == 7u && CycleManager::CheckForwardSteeringClearance(npc, 7.0f, nearest)) {
                npc.cycle->bikeSteeringState = BikeSteering_FrontBlocked;
                if (speed <= 0.001f) {
                    npc.cycle->pos = npc.cycle->lastMoveObjPos;
                    npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->lastMoveObjYaw);
                    npc.cycle->baseMoveSpeed = 0.0f;
                }
            }
            if (CycleManager::CheckSteeringOverlap(npc, nearest, false)) {
                if (speed > 0.001f) {
                    npc.cycle->bikeSteeringState = BikeSteering_SideAvoidMoving;
                } else {
                    npc.cycle->pos = npc.cycle->lastMoveObjPos;
                    npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->lastMoveObjYaw);
                    npc.cycle->baseMoveSpeed = 0.0f;
                    npc.cycle->bikeSteeringState = BikeSteering_SideBlockedStop;
                }
            }
        }
        Matrix4x4::Pop(1);
        if (npc.cycle->bikeSteeringState == BikeSteering_ProbeActive)
            npc.cycle->bikeSteeringState = BikeSteering_Normal;
        npc.driveWheelSteering = std::clamp(8 * static_cast<int32_t>(static_cast<int16_t>(npc.cycle->desiredPathYaw - previousYaw)), -5461, 5461);
        if (npc.cycle->obj) {
            npc.cycle->obj->shadowMode = 2;
            npc.cycle->foundExistingMoveObjRuntime = false;
            npc.cycle->obj->shadowScale = {
                npc.cycle->steeringHalfDepth + npc.cycle->steeringHalfDepth, 0.0f, npc.cycle->steeringHalfWidth + npc.cycle->steeringHalfWidth};
        }
    }
    if (npc.cycle->activeAnimId == 1u || static_cast<int32_t>(npc.cycle->reversePathFrames) > 0)
        npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000);
    if (live && --npc.cycle->map_probe_cooldown <= 0) {
        npc.cycle->map_probe_cooldown = 15;
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
    }
    return ticking;
}

void AdjustBikeReversePathIndex(CycleManager::NpcRecord &npc, int32_t direction) {
    if (!npc.cycle->pathPointCount) {
        npc.cycle->pathPointIndex = 0;
        return;
    }
    int32_t index = static_cast<int32_t>(npc.cycle->pathPointIndex) + direction;
    const int32_t endpoint = direction < 0 ? 0 : static_cast<int32_t>(npc.cycle->pathPointCount) - 1;
    const uint16_t yaw = static_cast<uint16_t>(npc.cycle->desiredPathYaw);
    const float facingX = -sin_fixed16(yaw);
    const float facingZ = -sin_fixed16(0x4000 - CycleManager::FoldUnsignedYaw(yaw));
    while (index >= 0 && index < static_cast<int32_t>(npc.cycle->pathPointCount)) {
        const Vector2 &point = reinterpret_cast<const Vector2 *>(npc.cycle->pathPoints)[index];
        float dx = point.x - npc.cycle->pos.x;
        float dz = point.y - npc.cycle->pos.z;
        const float length = std::sqrt(dx * dx + dz * dz);
        if (length > 0.0f) {
            dx /= length;
            dz /= length;
        }
        npc.cycle->pathPointIndex = static_cast<int16_t>(index);
        if (dx * facingX + dz * facingZ > 0.0f) {
            return;
        }
        index += direction;
    }
    npc.cycle->pathPointIndex = static_cast<int16_t>(endpoint);
}




bool CheckRyoFacingCycleBlockingBox(CycleManager::NpcRecord &npc) {
    if (g_flmwForwardInputPressed) {
        const Vector3 &ryoPosition = g_cycleManagerRuntime.center;
        float toRyoX = ryoPosition.x - npc.cycle->pos.x;
        float toRyoZ = ryoPosition.z - npc.cycle->pos.z;
        if (100.0f > toRyoZ * toRyoZ + toRyoX * toRyoX) {
            const std::int16_t pathYaw = npc.cycle->desiredPathYaw;
            const float pathForwardX = -sin_fixed16(static_cast<int16_t>(static_cast<std::uint16_t>(pathYaw)));
            const float pathForwardZ = -cos_fixed16(static_cast<std::uint16_t>(pathYaw));
            bool facingRyo = true;
            if (!(FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(toRyoX)))) ||
                !(FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(toRyoZ))))) {
                const float length = std::sqrt((toRyoX * toRyoX + 0.0f) + toRyoZ * toRyoZ);
                if (length > 0.0f) {
                    toRyoX *= 1.0f / length;
                    toRyoZ *= 1.0f / length;
                }
                facingRyo = !(0.0f > toRyoZ * pathForwardZ + toRyoX * pathForwardX);
            }
            if (facingRyo) {
                const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
                auto *const ryo = static_cast<Character *>(control->ryo);
                std::int16_t ryoYaw = 0;
                if (ryo) {
                    for (const CharacterNode *node = &ryo->entity.node; node; node = node->child)
                        ryoYaw = static_cast<std::int16_t>(ryoYaw + node->rotation.y);
                }
                const auto yawDifference = static_cast<std::int16_t>(pathYaw - ryoYaw);
                if (std::abs(static_cast<std::int32_t>(yawDifference)) < 0x4000) {
                    if (static_cast<std::int32_t>(npc.cycle->reversePathFrames) > 1)
                        npc.cycle->reversePathFrames = npc.cycle->steeringBlockedFrames;
                } else {
                    Vector3 points[5]{};
                    ApplyRotationMatrix({.center = &npc.cycle->pos,
                        .yaw = pathYaw,
                        .half_width = npc.cycle->steeringHalfWidth,
                        .half_depth = npc.cycle->steeringHalfDepth,
                        .yaw_offset = CycleManager::GetMoveObjectShapeOffset(npc),
                        .out_corners = points});
                    points[4] = {npc.cycle->pos.x, 0.0f, npc.cycle->pos.z};
                    const float coneCosine = sin_fixed16(static_cast<int16_t>(0x2000));
                    const float ryoForwardX = -sin_fixed16(static_cast<int16_t>(static_cast<std::uint16_t>(ryoYaw)));
                    const float ryoForwardZ = -cos_fixed16(static_cast<std::uint16_t>(ryoYaw));
                    float pointY = 0.0f;
                    for (const Vector3 &point : points) {
                        float pointX = point.x - ryoPosition.x;
                        float pointZ = point.z - ryoPosition.z;
                        float alignment = 1.0f;
                        if (!(FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(pointX)))) ||
                            !(FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(pointZ))))) {
                            const float length = std::sqrt((pointY * pointY + pointX * pointX) + pointZ * pointZ);
                            if (length > 0.0f) {
                                pointX *= 1.0f / length;
                                pointY *= 1.0f / length;
                                pointZ *= 1.0f / length;
                            }
                            alignment = pointZ * ryoForwardZ + pointX * ryoForwardX;
                        }
                        if (alignment > coneCosine) {
                            npc.cycle->ryoBlockingCooldownFrames = 30;
                            return true;
                        }
                    }
                }
            }
        }
    }
    if (npc.cycle->reversePathFrames == 0u)
        npc.cycle->ryoBlockingCooldownFrames = 0;
    return false;
}

void UpdateBikeReverseAvoidance(CycleManager::NpcRecord &npc, float observedSpeed, bool doForwardConeCheck) {
    const bool reverseCapable = npc.cycle->moveObjRuntime != nullptr || npc.cycle->moveConfig->cycleClassCode == 7u;
    bool forceWait = false;
    if (npc.steeringCollisionResult != 0u) {
        forceWait = (npc.steeringCollisionResult & 4u) == 0u || g_default_turning_box_half_depth_z <= 0.0f;
    }
    const bool ryoBlocking = reverseCapable && doForwardConeCheck && CheckRyoFacingCycleBlockingBox(npc);
    if (!npc.cycle->moveObjRuntime && npc.cycle->activeAnimId == 1u) {
        npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000u);
        npc.cycle->reversePathFrames = 0;
        npc.cycle->ryoBlockingCooldownFrames = 0;
        npc.cycle->steeringWaitFrames = 0;
    } else if (doForwardConeCheck && npc.cycle->reversePathFrames == 0u && npc.cycle->pathSegmentProgress > 0.0f) {
        const bool stoppedByFront =
            npc.cycle->bikeSteeringState == BikeSteering_FrontBlocked && std::fabs(observedSpeed) < std::numeric_limits<float>::epsilon();
        if (npc.cycle->steeringWaitFrames == 15u || stoppedByFront || (reverseCapable && npc.steeringCollisionResult != 0u) || ryoBlocking) {
            if (forceWait || ryoBlocking) {
                npc.cycle->steeringWaitFrames = 15u;
            } else {
                ++npc.cycle->steeringWaitFrames;
            }
            if (npc.cycle->steeringWaitFrames >= 15u) {
                npc.cycle->steeringWaitFrames = 0;
                npc.cycle->reversePathFrames = 1;
                npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000u);
                const uint32_t area = CycleManager::CurrentAreaId();
                if (area == SHEN_NAME(D000) || (!reverseCapable && area == SHEN_NAME(MFSY))) {
                    npc.cycle->steeringBlockedFrames = 15u;
                } else {
                    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
                    npc.cycle->steeringBlockedFrames = ((g_EnvPrngSeed >> 16u) & 0x1fu) + 30u;
                }
                CycleManager::AdjustBikeReversePathIndex(npc, -1);
            }
        } else {
            npc.cycle->steeringWaitFrames = 0;
        }
    } else {
        npc.cycle->steeringWaitFrames = 0;
    }

    if (npc.cycle->reversePathFrames == 0u)
        return;
    float reverseSpeed = 0.0f;
    if (npc.cycle->ryoBlockingCooldownFrames > 0) {
        --npc.cycle->ryoBlockingCooldownFrames;
        reverseSpeed = (npc.cycle->moveObjSpeedParam - npc.cycle->baseMoveSpeed) * 0.0099999998f + npc.cycle->baseMoveSpeed;
        if (npc.cycle->reversePathFrames > 1u)
            npc.cycle->reversePathFrames = 2u;
    } else if (npc.cycle->moveConfig->cycleClassCode == 7u) {
        float frame = static_cast<float>(npc.cycle->reversePathFrames);
        const float duration = static_cast<float>(npc.cycle->steeringBlockedFrames);
        if (duration * 0.5f < frame)
            frame = duration - frame;
        if (duration > 0.0f) {
            reverseSpeed = 2.0f * (frame / duration) * npc.cycle->moveObjSpeedParam;
        }
    } else if (doForwardConeCheck && npc.cycle->reversePathFrames > 1u) {
        Matrix4x4::SetActiveTransformToPrePass();
        if (observedSpeed <= 0.001f)
            observedSpeed = 0.0092592593f;
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-npc.cycle->desiredPathYaw));
        Matrix4x4::preTranslate(-npc.cycle->pos.x, -npc.cycle->pos.y, -npc.cycle->pos.z);
        float nearest = 0.0f;
        float clearanceSpeed = 0.0f;
        bool blocked = false;
        if (CycleManager::CheckForwardSteeringClearance(npc, 4.5f, nearest)) {
            clearanceSpeed = ((nearest + 6.5f) / 5.5f) * npc.cycle->moveObjSpeedParam;
            blocked = true;
        } else if (CycleManager::CheckSteeringOverlap(npc, nearest, false) && npc.cycle->moveConfig->obj_id.name != npc.cycle->selfCyclePtr->cycleObjectId) {
            clearanceSpeed = ((nearest + 4.5f) / 3.5f) * npc.cycle->moveObjSpeedParam;
            blocked = true;
        }
        if (blocked) {
            if (clearanceSpeed > observedSpeed)
                clearanceSpeed = (clearanceSpeed - observedSpeed) * 0.5f + observedSpeed;
            npc.cycle->reversePathFrames = 2;
        }
        reverseSpeed = std::max(0.0f, clearanceSpeed);
        Matrix4x4::Pop(1);
    }
    const uint32_t frame = npc.cycle->reversePathFrames;
    npc.cycle->baseMoveSpeed = std::max(0.0f, reverseSpeed);
    if (frame != 1u) {
        npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000u);
    }
    npc.cycle->reversePathFrames = frame + 1u;
    if (!doForwardConeCheck || npc.cycle->reversePathFrames >= npc.cycle->steeringBlockedFrames) {
        npc.cycle->desiredPathYaw = static_cast<int16_t>(npc.cycle->desiredPathYaw + 0x8000u);
        npc.cycle->reversePathFrames = 0;
        npc.cycle->steeringWaitFrames = 0;
        npc.cycle->ryoBlockingCooldownFrames = 0;
        CycleManager::AdjustBikeReversePathIndex(npc, 1);
    }
}

void AdjustDrivePathForClearance(CycleManager::NpcRecord &npc) {
    if (!CycleManager::CheckClearanceAgainstRyoAndCycles(npc, true))
        return;
    bool moved = false;
    while (npc.cycle->pathPointIndex > 0) {
        --npc.cycle->pathPointIndex;
        const std::size_t index = static_cast<std::size_t>(npc.cycle->pathPointIndex);
        const Vector2 *const points = reinterpret_cast<const Vector2 *>(npc.cycle->pathPoints);
        const Vector2 &point = points[index];
        npc.cycle->pos.x = point.x;
        npc.cycle->pos.z = point.y;
        const std::size_t nextIndex = std::min<std::size_t>(index + 1u, npc.cycle->pathPointCount - 1u);
        const Vector2 &next = points[nextIndex];
        npc.cycle->desiredPathYaw = CycleManager::Atan2Fixed16(npc.cycle->pos.x - next.x, npc.cycle->pos.z - next.y);
        npc.cycle->baseMoveSpeed = 0.0f;
        moved = true;
        if (!CycleManager::CheckClearanceAgainstRyoAndCycles(npc, true))
            break;
    }
    if (moved) {
        npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
    }
}


bool IsEntityWithinRange(Character *chara, const BikeSideVolumeQuery &query) {
    const Vector3 position = chara ? GetPositionForEntity(chara->entity.node) : Vector3{};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(-query.extents.z, 0.0f, 0.0f);
    Matrix4x4::preTranslate(-query.localOffset->x, -query.localOffset->y, -query.localOffset->z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<uint16_t>(-query.yaw));
    Matrix4x4::preTranslate(-query.origin->x, -query.origin->y, -query.origin->z);
    const Matrix4x4 &local = workTransformMatrix;
    const float localX = local.rows[1].x * position.y + local.rows[0].x * position.x + local.rows[2].x * position.z + local.rows[3].x;
    const float localZ = local.rows[0].z * position.x + local.rows[1].z * position.y + local.rows[2].z * position.z + local.rows[3].z;
    Matrix4x4::Pop();
    const float halfWidth = query.extents.x + 0.5f;
    if (!(halfWidth > localX && localX > -halfWidth))
        return false;
    const float halfDepth = query.extents.y + 0.5f;
    return halfDepth > localZ && localZ > -halfDepth;
}


bool cmCheckEntityInRangeOf(const CycleManager::NpcRecord &npc, const BikeSideVolumeQuery &query) {
    if (!CycleManager::IsReady())
        return false;
    if (Character *const ryo = CycleManager::MapRyo(); ryo && IsEntityWithinRange(ryo, query))
        return true;
    for (const CycleProgramList list : {CycleProgramList_SimSet, CycleProgramList_Running}) {
        for (const CycleManager::NpcRecord &other : g_cycleManagerRuntime.npcRecords) {
            if (other.programList != list || !other.cycle->obj || &other == &npc)
                continue;
            if (IsEntityWithinRange(other.cycle->obj, query))
                return true;
        }
    }
    return false;
}
} // namespace CycleManager
  // namespace

namespace CycleManager {






void InitBikeMoveObjectPathContext(CycleManager::NpcRecord &npc) {
    CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime ? npc.cycle->moveObjRuntime : npc.releasedMoveObjRuntime;
    const BikeMoveObjectToken *const token = npc.bikeToken;
    if (!runtime || !token || !token->anims || npc.cycle->pathPointCount < 2u)
        return;
    npc.cycle->moveObjRuntime = runtime;
    npc.releasedMoveObjRuntime = nullptr;
    const uint32_t kind = runtime->moveObjectKind;
    const Vector2 *const points = reinterpret_cast<const Vector2 *>(npc.cycle->pathPoints);
    const Vector2 &firstPoint = points[0];
    const float dz = runtime->pos.z - firstPoint.y;
    const float dx = runtime->pos.x - firstPoint.x;
    const float distanceSquared = dz * dz + dx * dx;
    const Vector2 pathStart{runtime->pathStartX, runtime->pathStartZ};
    if (npc.cycle->snapToPathStart || distanceSquared > 2500.0f) {
        runtime->pos.x = firstPoint.x;
        runtime->pos.z = firstPoint.y;
        npc.cycle->pathPointIndex = 1;
        npc.cycle->pathSegmentProgress = 1.0f;
        UpdatePathSegmentCache(*runtime, firstPoint, points[1]);
    } else if (distanceSquared < 1.0f) {
        npc.cycle->pathPointIndex = 1;
        npc.cycle->pathSegmentProgress = 1.0f;
        UpdatePathSegmentCache(*runtime, pathStart, points[1]);
    } else {
        npc.cycle->pathPointIndex = 0;
        npc.cycle->pathSegmentProgress = 0.0f;
        UpdatePathSegmentCache(*runtime, pathStart, firstPoint);
    }
    npc.cycle->pos = runtime->pos;
    npc.cycle->activeAnimId = kind == 5u ? npc.vehiclePathAnimId : static_cast<uint16_t>(token->anims->anim[2]);
    npc.cycle->pathMoveSpeed = 1.0f;
    if (npc.cycle->baseMoveSpeed >= 0.74074078f)
        npc.cycle->baseMoveSpeed = 0.0f;
    npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
    const Vector2 &waypoint = points[npc.cycle->pathPointIndex];
    if (kind == 5u) {
        npc.cycle->desiredPathYaw = CycleManager::Atan2Fixed16(npc.cycle->pos.x - waypoint.x, npc.cycle->pos.z - waypoint.y);
        runtime->slopeAngleX = 0;
        runtime->animLeanAngle = 0;
        runtime->renderEuler.x = 0;
        runtime->renderEuler.z = 0;
    } else {
        npc.cycle->desiredPathYaw = static_cast<int16_t>(runtime->yaw);
    }
    npc.cycle->dist = 10.0f;
    npc.cycle->range = 3.0f;
    npc.cycle->bikeSteeringState = BikeSteering_Normal;
    npc.cycle->approachStartSpeed = 0.0f;
    npc.cycle->cycleObjectId = npc.cycle->moveConfig->obj_id.name;
    npc.cycle->cycleObjectIdCopy = npc.cycle->moveConfig->obj_id.name;
    npc.cycle->lastMoveObjPos = npc.cycle->pos;
    npc.cycle->lastMoveObjYaw = npc.cycle->desiredPathYaw;
    npc.selfCycleIndex = static_cast<int32_t>(&npc - g_cycleManagerRuntime.npcRecords.data());
    npc.cycle->bikeClearanceTimer = 0;
    npc.cycle->ryoBlockingCooldownFrames = 0;
    npc.cycle->bikeClearanceBlocked = false;
    npc.cycle->moveTargetYaw = 0;
    npc.cycle->steeringWaitFrames = 0;
    npc.cycle->reversePathFrames = 0;
    runtime->activeAnim = static_cast<int16_t>(npc.cycle->activeAnimId);
    runtime->unkAA = 0;
    runtime->clearanceCooldown = 0;
    runtime->waitAnim = token->anims->anim[4];
    runtime->unkAE[0] = 0;
    runtime->unkAE[1] = 0;
    runtime->runtimeC0 = static_cast<float>(token->runtime_c0_value);
    CycleManager::SynchronizeBikeSoundBank(npc);
    runtime->activeFlag = 0;
    npc.cycle->currentOpcode = npc.cycle->savedOpcodeBeforeMoveObj;
    if (npc.cycle->foundExistingMoveObjRuntime)
        CycleManager::UnlinkParkedMoveObject(runtime);
    if (kind == 5u) {
        npc.cycle->steeringHalfWidth = 1.47f;
        npc.cycle->steeringHalfDepth = 0.61000001f;
    } else {
        npc.cycle->steeringHalfWidth = token->steering_half_width;
        npc.cycle->steeringHalfDepth = token->steering_half_depth;
        SetBikeMotionConstraintOwner(npc);
    }
    runtime->animOutput = 0;
    npc.cycle->moveTargetPos = {waypoint.x, npc.cycle->pos.y, waypoint.y};
    npc.cycle->currentWaypointPos = npc.cycle->moveTargetPos;
}

void TickBikeMoveObjectTransition(CycleManager::NpcRecord &npc) {
    CycleMoveObjectRuntime *const runtime = npc.cycle->moveObjRuntime ? npc.cycle->moveObjRuntime : npc.releasedMoveObjRuntime;
    const BikeMoveObjectToken *const token = npc.bikeToken;
    if (!runtime || !token || !token->anims) {
        npc.cycle->currentOpcode = npc.cycle->savedOpcodeBeforeMoveObj;
        npc.cycle->flags &= ~0x80u;
        return;
    }

    if (npc.cycle->moveObjActivationPending) {
        if (!CycleManager::TickPositionAndHeadingMove(npc)) {
            npc.cycle->moveObjActivationPending = false;
            npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint({npc.cycle->pos.x, 100.0f, npc.cycle->pos.z});
            npc.cycle->activeAnimId = static_cast<uint16_t>(token->anims->anim[0]);
            MotionInformationWork *const motion = npc.cycle->mot;
            if (motion && npc.cycle->obj) {
                CycleManager::EnableRootMotion(npc.cycle->obj, true);
                motion->motionFlags &= ~2u;
                motion->pendingAnimId = npc.cycle->activeAnimId;
            }
        }
        npc.cycle->flags &= ~0x80u;
        return;
    }

    CycleManager::UpdateBikeAnimCurveOutputs(npc, *runtime, true);
    MotionInformationWork *const motion = npc.cycle->mot;
    const bool animationFinished = !npc.cycle->motnSteeringActive || (motion && (motion->motionFlags & 2u) != 0);
    if (!animationFinished) {
        npc.cycle->flags &= ~0x80u;
        return;
    }

    const uint16_t approachAnim = static_cast<uint16_t>(token->anims->anim[0]);
    if (npc.cycle->activeAnimId == approachAnim) {
        npc.cycle->activeAnimId = static_cast<uint16_t>(token->anims->anim[3]);
        npc.cycle->desiredPathYaw = static_cast<int16_t>(runtime->yaw);
        npc.yaw = npc.cycle->desiredPathYaw;
        if (motion && npc.cycle->obj) {
            motion->motionFlags &= ~2u;
            motion->pendingAnimId = npc.cycle->activeAnimId;
            CycleManager::SetNodeChainYaw(*npc.cycle->obj, npc.cycle->desiredPathYaw);
        }
        npc.cycle->flags &= ~0x80u;
        return;
    }

    if (runtime->moveObjectId == SHEN_NAME(HAL1)) {
        const BikeSideVolumeQuery query{.origin = &runtime->pos,
            .localOffset = &runtime->tokenLocalOffset,
            .yaw = static_cast<int16_t>(runtime->yaw),
            .extents = {1.2f, 0.49000001f, 0.0f}};
        if (cmCheckEntityInRangeOf(npc, query) && npc.cycle->motnSteeringActive) {
            npc.cycle->flags &= ~0x80u;
            return;
        }
    }

    CycleManager::InitBikeMoveObjectPathContext(npc);
    npc.cycle->flags &= ~0x80u;
}
} // namespace CycleManager

namespace CycleManager {



bool cmCheckEntityInRangeOf_Wrapper(const CycleManager::NpcRecord &npc, const CycleMoveObjectRuntime &runtime, const BikeMoveObjectToken &token) {
    const bool hal1 = runtime.moveObjectId == SHEN_NAME(HAL1);
    BikeSideVolumeQuery query{.origin = &runtime.pos,
        .localOffset = &runtime.tokenLocalOffset,
        .yaw = static_cast<int16_t>(runtime.yaw),
        .extents = hal1 ? Vector3{0.80000001f, 1.0f, -0.60000002f} : Vector3{0.64999998f, 0.5f, -0.30000001f}};
    if (token.mount_target_x > 0.0f)
        query.extents.z = -query.extents.z;
    return cmCheckEntityInRangeOf(npc, query);
}

void AddBikeReleaseCollisionVolume(const CycleMoveObjectRuntime &runtime, const BikeMoveObjectToken &token) {
    const bool hal1 = runtime.moveObjectId == SHEN_NAME(HAL1);
    const float halfExtent = hal1 ? 0.80000001f : 0.64999998f;
    float lateralOffset = hal1 ? -0.60000002f : -0.30000001f;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(runtime.pos.x, runtime.pos.y, runtime.pos.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<uint16_t>(runtime.yaw));
    Matrix4x4::preTranslate(runtime.tokenLocalOffset.x, runtime.tokenLocalOffset.y, runtime.tokenLocalOffset.z);
    if (token.mount_target_x >= 0.0f)
        lateralOffset = -lateralOffset;
    Matrix4x4::preTranslate(lateralOffset, 0.0f, 0.0f);
    const Vector3 center{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop();
    if (!CycleManager::IsReady())
        return;
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (g_dynamicTurnCollisionVolumeCount >= cm.dynamicTurnCollisionVolumes.size())
        return;

    CycleManager::IntersectionEntry &entry = cm.dynamicTurnCollisionVolumes[g_dynamicTurnCollisionVolumeCount++];
    entry.operand = SCN_U32FromFloat(center.x);
    entry.offset = SCN_U32FromFloat(center.y);
    entry.dwordC = SCN_U32FromFloat(center.z);
    entry.dword10 = static_cast<uint32_t>(runtime.yaw);
    entry.dword14 = SCN_U32FromFloat(halfExtent);
    entry.dword18 = SCN_U32FromFloat(halfExtent);
    entry.dword1C = SCN_U32FromFloat(0.0f);
}
} // namespace CycleManager
  // namespace

namespace CycleManager {



void TickBikeMoveObjectRelease(CycleManager::NpcRecord &npc) {
    CycleMoveObjectRuntime *const runtime = npc.releasedMoveObjRuntime ? npc.releasedMoveObjRuntime : npc.cycle->moveObjRuntime;
    const BikeMoveObjectToken *const token = npc.bikeToken;
    if (!runtime || !token || !token->anims) {
        npc.cycle->moveObjRuntime = nullptr;
        npc.releasedMoveObjRuntime = nullptr;
        npc.bikeToken = nullptr;
        npc.cycle->currentOpcode = CycleOpcode_RemoveCharacter;
        npc.cycle->flags &= ~0x80u;
        return;
    }

    MotionInformationWork *const releaseMotion = npc.cycle->mot;
    if (releaseMotion)
        releaseMotion->playbackRate = 1.0f;
    CycleManager::UpdateBikeAnimCurveOutputs(npc, *runtime, true);

    const MotionInformationWork *const motion = npc.cycle->mot;
    const bool animationFinished = !npc.cycle->motnSteeringActive || (motion && (motion->motionFlags & 2u) != 0);
    const uint16_t waitAnim = static_cast<uint16_t>(token->anims->anim[4]);
    const uint16_t leaveAnim = static_cast<uint16_t>(token->anims->anim[1]);
    if (animationFinished) {
        if (npc.cycle->activeAnimId == waitAnim) {
            if (!cmCheckEntityInRangeOf_Wrapper(npc, *runtime, *token)) {
                npc.cycle->activeAnimId = leaveAnim;
                if (releaseMotion) {
                    releaseMotion->motionFlags &= ~2u;
                    releaseMotion->pendingAnimId = leaveAnim;
                }
                if (npc.cycle->moveObjRuntime)
                    CycleManager::ReleaseVehicleRuntime(npc);
            }
        } else {


            if (npc.cycle->moveObjRuntime)
                CycleManager::ReleaseVehicleRuntime(npc);
            npc.cycle->currentOpcode = CycleOpcode_RemoveCharacter;
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
            if (releaseMotion) {
                npc.cycle->pos = {releaseMotion->rootPreTranslate.x, 0.0f, releaseMotion->rootPreTranslate.z};
                npc.cycle->pos.y = MapProbe_UpdateIntersectionStateAtPoint(npc.cycle->pos);
                npc.cycle->desiredPathYaw = releaseMotion->extractedYaw;
                releaseMotion->motionFlags &= ~2u;
                releaseMotion->pendingAnimId = npc.cycle->activeAnimId;
            }
            BIKE_Init(*runtime);
            if (npc.cycle->primarySoundBankId >= 0) {
                ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(npc.cycle->primarySoundBankId));
                npc.cycle->primarySoundBankId = -1;
            }
            if (npc.cycle->obj) {
                shenApplyTranslationToAll(npc.cycle->obj->entity.node, npc.cycle->pos);
                CycleManager::SetNodeChainYaw(*npc.cycle->obj, npc.cycle->desiredPathYaw);
            }
            runtime->animOutput = 0x4000;
            npc.cycle->moveObjActivationPending = false;
            npc.cycle->facingConstraintFlag = false;
            npc.cycle->moveObjRuntime = nullptr;
            npc.releasedMoveObjRuntime = nullptr;
            npc.bikeToken = nullptr;
        }
    }

    if (npc.cycle->activeAnimId == leaveAnim)
        CycleManager::AddBikeReleaseCollisionVolume(*runtime, *token);
    npc.cycle->flags &= ~0x80u;
}
} // namespace CycleManager

namespace CycleManager {

int32_t DegreesToFixed16(float degrees) {
    return static_cast<int16_t>(static_cast<uint16_t>(static_cast<int32_t>((degrees * 8192.0f) / 45.0f)));
}

void SetRawObjectNodeRotationY(Character &local, uint32_t nodeId, int32_t rotationY) {
    if (g_cycleManagerObjectNodeRotation) {
        g_cycleManagerObjectNodeRotation(&local, nodeId, rotationY);
        return;
    }

    auto *const root = static_cast<Mt5RuntimeNode *>(local.model);
    std::byte *const modelBase = root ? MT5_ModelBaseForNode(root) : nullptr;
    if (!root || !modelBase)
        return;
    if (Mt5RuntimeNode *const node = FindRuntimeNodeByKind(root, modelBase, static_cast<uint16_t>(nodeId))) {
        node->rotation.y = rotationY;
    }
}





void UpdateVehicleWheelNodes(CycleManager::NpcRecord &npc) {
    Mt5RuntimeNode *const model = npc.cycle->obj ? static_cast<Mt5RuntimeNode *>(npc.cycle->obj->model) : nullptr;
    if (!model)
        return;
    float distance = 0.0f;
    if (npc.cycle->currentOpcode == CycleOpcode_DriveVehiclePath2D) {
        Vector3 delta{
            npc.cycle->lastMoveObjPos.x - npc.cycle->pos.x, npc.cycle->lastMoveObjPos.y - npc.cycle->pos.y, npc.cycle->lastMoveObjPos.z - npc.cycle->pos.z};
        if (delta.x > 0.00000011920929f || delta.x < -0.00000011920929f || delta.z > 0.00000011920929f || delta.z < -0.00000011920929f)
            distance = Vector3::NormalizeInPlace(delta);
    } else {
        const float dx = npc.cycle->lastMoveObjPos.x - npc.cycle->pos.x;
        const float dz = npc.cycle->lastMoveObjPos.z - npc.cycle->pos.z;
        npcUpdateBaseSpeed(npc, static_cast<uint16_t>(npc.cycle->pathPointIndex));
        npc.cycle->lastMoveObjPos = npc.cycle->pos;
        npc.cycle->lastMoveObjYaw = npc.cycle->desiredPathYaw;
        distance = std::sqrt(dz * dz + dx * dx);
    }
    if (npc.cycle->reversePathFrames > 0)
        distance = -distance;
    const std::int16_t roll = static_cast<std::int16_t>(static_cast<std::int32_t>(distance / 0.30000001f * 10430.387f));
    if (npc.cycle->moveConfig->cycleClassCode == 6u) {
        if (Mt5RuntimeNode *const axle = shenGetNodeByChildIdx(model, 182))
            axle->rotation.x -= roll;
        if (Mt5RuntimeNode *const wheel = shenGetNodeByChildIdx(model, 162)) {
            wheel->rotation.x -= roll;
            wheel->rotation.y = npc.motnCycleYawDelta;
        }
        if (Mt5RuntimeNode *const wheel = shenGetNodeByChildIdx(model, 172)) {
            wheel->rotation.x += roll;
            wheel->rotation.y = npc.motnCycleYawDelta;
        }
        return;
    }
    if (Mt5RuntimeNode *const axle = shenGetNodeByChildIdx(model, 165))
        axle->rotation.x -= roll;
    for (const std::int16_t nodeKind : {std::int16_t{163}, std::int16_t{164}}) {
        if (Mt5RuntimeNode *const wheel = shenGetNodeByChildIdx(model, nodeKind)) {
            wheel->rotation.x -= roll;
            wheel->rotation.y = npc.driveWheelSteering;
        }
    }
}

void UpdateClass6DoorNodes(CycleManager::NpcRecord &npc) {
    // UpdatePos returns before both the counters and node writes while the
    // owning cycle is controlled by Script (RunningPrograms).
    if (npc.programList == CycleProgramList_Running)
        return;

    int32_t step = 0;
    static_assert(sizeof(step) == sizeof(npc.cycle->pathMoveSpeed));
    std::memcpy(&step, &npc.cycle->pathMoveSpeed, sizeof(step));

    int32_t firstCounter = static_cast<int32_t>(npc.cycle->selectedMotionToken);
    int32_t secondCounter = static_cast<int32_t>(npc.field148);
    firstCounter = CycleManager::UpdateClass6DoorCounter(firstCounter, step, (npc.cycle->flags & 1u) != 0);
    secondCounter = CycleManager::UpdateClass6DoorCounter(secondCounter, step, (npc.cycle->flags & 2u) != 0);
    npc.cycle->selectedMotionToken = static_cast<uint32_t>(firstCounter);
    npc.field148 = static_cast<uint32_t>(secondCounter);

    if (!npc.cycle->obj)
        return;

    const float firstBlend = CycleManager::Class6DoorBlend(firstCounter);
    CycleManager::SetRawObjectNodeRotationY(*npc.cycle->obj, 0x99u, CycleManager::DegreesToFixed16(firstBlend * 85.0f));
    CycleManager::SetRawObjectNodeRotationY(*npc.cycle->obj, 0x9Au, CycleManager::DegreesToFixed16(firstBlend * -160.0f));

    const float secondBlend = CycleManager::Class6DoorBlend(secondCounter);
    const int32_t second80 = CycleManager::DegreesToFixed16(secondBlend * 80.0f);
    const int32_t secondNeg160 = CycleManager::DegreesToFixed16(secondBlend * -160.0f);
    CycleManager::SetRawObjectNodeRotationY(*npc.cycle->obj, 0x9Bu, static_cast<int16_t>(-second80));
    CycleManager::SetRawObjectNodeRotationY(*npc.cycle->obj, 0x9Cu, static_cast<int16_t>(-secondNeg160));
    CycleManager::SetRawObjectNodeRotationY(*npc.cycle->obj, 0x9Du, second80);
    CycleManager::SetRawObjectNodeRotationY(*npc.cycle->obj, 0x9Eu, secondNeg160);
}
} // namespace CycleManager

} // namespace shenmue::s1
