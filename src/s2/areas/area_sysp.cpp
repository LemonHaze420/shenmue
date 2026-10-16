#include "s2/areas/area_sysp.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include <bit>
#include <cmath>

namespace shenmue::s2::areas {
std::int32_t syspState, syspFlags, syspPackageLoaded, syspElapsedFrames, syspRyoNearRen, syspNpcWasVisible;
std::int32_t syspCameraPhase, syspCameraPhaseStep, syspPreviousPerspective, syspFovPercent, syspRetryCount, syspEventMode, syspNextDialogueMode;
std::uint32_t syspEventYaw, syspDefaultYaw, syspAlternateYaw, syspNextEvent, syspWaitTicks;
std::int32_t syspSavedMotion, syspItemAlphaState;
float syspSavedMotionFrame, syspSecondaryPhaseScale, syspItemAlpha;
shen_name syspNpc;
const char *syspVoiceScript;
const char *syspAttachedItem;
const Vector3 *syspTargetNpcPosition;
ConversationBuffer *syspConversation;
Vector3 syspTargetPosition, syspRyoTargetPosition, syspPreviousCameraPosition, syspPreviousNextCameraPosition, syspSavedNpcPosition;
Vector3i syspSavedNpcRotation;
Vector4 syspPreviousRyoPosition;

void SyspInitNullsub() {
}

void SyspStartEvent(SyspEventType type, std::uintptr_t, const char *voice, const Vector3 *target, std::uint32_t yaw, shen_name npc, std::uint32_t fovPercent,
    const char *item) {
    if (!syspPackageLoaded)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SYSP"), 0);
    syspPackageLoaded = 1;
    syspWaitTicks = 0;
    syspNextEvent = 0;
    syspVoiceScript = voice;
    if (target)
        syspTargetPosition = *target;
    syspEventYaw = yaw;
    syspFovPercent = std::bit_cast<std::int32_t>(fovPercent);
    syspNpc = npc;
    syspAttachedItem = item;
    if (item && !*item)
        syspAttachedItem = nullptr;
    syspEventMode = SyspInitialize;
    syspRetryCount = 0;
    EV_debug_log("SYSP\\eSYSP_et_sys.c", 130, "TYPE = %d\n", static_cast<std::int32_t>(type));
    SyspInitNullsub();
    HLib::TaskCallback callback;
    HLib::Task *task;
    Vector3 npcPosition, ryoPosition;
    switch (type) {
    case SyspEventType::Simple:
    case SyspEventType::SimpleAlt:
        callback = SyspSimpleCallback;
        break;
    case SyspEventType::WithNpc:
        SetScenarioFollowCharacter({});
        if (!npc.name) {
            task = HLib::EnqueueTaskWithoutParameter(SyspNpcCallback, 3, 0x0b, static_cast<std::int32_t>(SYSP_TASK.name));
            SyspNpcCallback(nullptr);
            camCtrlSetState(CameraState::Init);
            if (!task)
                SetPlayerControlEnabled(1);
            return;
        }
        syspNpcWasVisible = static_cast<std::int32_t>(IsNpcLoadedOrVisible(syspNpc));
        EV_CheckWarmStartNpcRoute(syspNpc);
        EV_PTYCUtils(syspNpc, 3, 1);
        task = HLib::EnqueueTaskWithoutParameter(SyspNpcCharacterCallback, 3, 0x0b, static_cast<std::int32_t>(SYSP_TASK.name));
        SyspNpcCharacterCallback(nullptr);
        if (!task)
            SetPlayerControlEnabled(1);
        return;
    case SyspEventType::WithItem:
        EV_debug_log("SYSP\\eSYSP_et_sys.c", 226, "eSYSP_Item = %s\n", syspAttachedItem);
        if (syspAttachedItem) {
            const char *const path = Filepath_Gen(FilepathGenMode::PackDirectory);
            EV_ControlPackageResources(
                PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>(syspAttachedItem));
        }
        callback = SyspItemCallback;
        break;
    case SyspEventType::RyoOnly:
        SetScenarioFollowCharacter({});
        callback = SyspRyoCallback;
        break;
    case SyspEventType::Other:
        callback = SyspOtherCallback;
        break;
    case SyspEventType::WarpIfOutOfRange:
        EV_debug_log("SYSP\\eSYSP_et_sys.c", 136, "type 7 %p\n", static_cast<const void *>(syspTargetNpcPosition));
        if (!syspTargetNpcPosition) {
            callback = SyspSimpleCallback;
            break;
        }
        if (ctrlGetCharaByName(npc)) {
            EV_GetCharaPosition(npc, CHARACTER_TRANSLATION_ROOT, &npcPosition, EventTransformFlags::World);
            EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &ryoPosition, EventTransformFlags::World);
            EV_debug_log("SYSP\\eSYSP_et_sys.c", 143, "exist %f %f %f\n", npcPosition.x, npcPosition.y, npcPosition.z);
            npcPosition.x -= ryoPosition.x;
            npcPosition.y -= ryoPosition.y;
            npcPosition.z -= ryoPosition.z;
            if (npcPosition.y * npcPosition.y <= 1.0f && npcPosition.x * npcPosition.x + npcPosition.z * npcPosition.z <= 25.0f) {
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 155, "範囲内\n");
                syspTargetNpcPosition = nullptr;
                callback = SyspSimpleCallback;
                break;
            }
            syspNpcWasVisible = static_cast<std::int32_t>(IsNpcLoadedOrVisible(syspNpc));
            EV_PTYCUtils(syspNpc, 3, 1);
            EV_CheckWarmStartNpcRoute(npc);
            EV_npcFree(npc);
        } else
            EV_debug_log("SYSP\\eSYSP_et_sys.c", 159, "居ない\n");
        EV_ForceLoadNpc(npc);
        callback = SyspSimpleCallback;
        break;
    case SyspEventType::StickyRange:
        SetScenarioFollowCharacter({});
        if (npc.name) {
            syspNpcWasVisible = static_cast<std::int32_t>(IsNpcLoadedOrVisible(syspNpc));
            EV_CheckWarmStartNpcRoute(syspNpc);
            EV_PTYCUtils(syspNpc, 3, 1);
            if (ctrlGetCharaByName(npc)) {
                EV_GetCharaPosition(npc, CHARACTER_TRANSLATION_ROOT, &npcPosition, EventTransformFlags::World);
                EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &ryoPosition, EventTransformFlags::World);
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 196, "exist %f %f %f\n", npcPosition.x, npcPosition.y, npcPosition.z);
                npcPosition.x -= ryoPosition.x;
                npcPosition.y -= ryoPosition.y;
                npcPosition.z -= ryoPosition.z;
                if (npcPosition.y * npcPosition.y > 1.0f || npcPosition.x * npcPosition.x + npcPosition.z * npcPosition.z > 25.0f) {
                    syspNpcWasVisible = static_cast<std::int32_t>(IsNpcLoadedOrVisible(syspNpc));
                    EV_PTYCUtils(syspNpc, 3, 1);
                    EV_CheckWarmStartNpcRoute(npc);
                    EV_npcFree(npc);
                    EV_ForceLoadNpc(npc);
                } else {
                    EV_debug_log("SYSP\\eSYSP_et_sys.c", 208, "範囲内\n");
                    syspTargetNpcPosition = nullptr;
                }
            } else {
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 212, "居ない\n");
                EV_ForceLoadNpc(npc);
            }
            task = HLib::EnqueueTaskWithoutParameter(SyspNpcCharacterCallback, 3, 0x0b, static_cast<std::int32_t>(SYSP_TASK.name));
            SyspNpcCharacterCallback(nullptr);
        } else {
            task = HLib::EnqueueTaskWithoutParameter(SyspNpcCallback, 3, 0x0b, static_cast<std::int32_t>(SYSP_TASK.name));
            SyspNpcCallback(nullptr);
        }
        if (!task)
            SetPlayerControlEnabled(1);
        return;
    default:
        SetPlayerControlEnabled(1);
        return;
    }
    task = HLib::EnqueueTaskWithoutParameter(callback, 3, 0x0b, static_cast<std::int32_t>(SYSP_TASK.name));
    if (!task)
        SetPlayerControlEnabled(1);
}

void SyspSimpleCallback(void *) {
    switch (syspEventMode) {
    case SyspFinished:
        if (syspNpc.name) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(syspNpc), nullptr, 0);
            if (!syspNpcWasVisible)
                EV_npcLockUnlock(syspNpc);
            EV_PTYCUtils(syspNpc, 3, 0);
        }
        camCtrlSetState(CameraState::Init);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), nullptr, 0);
        SyspEndCallback(nullptr);
        return;
    case SyspInitialize: {
        Vector3 position;
        Vector3i rotation;
        if (syspNpc.name) {
            const std::int32_t result = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(EV_npcStreamOrLoadHuman(syspNpc)));
            if (result != 1) {
                std::int32_t errorLine;
                switch (result) {
                case -6:
                case -4:
                case -3: {
                    const std::int32_t previous = syspRetryCount;
                    syspRetryCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspRetryCount) + 1U);
                    if (previous <= 2) {
                        PrepareNpcAllFromArray();
                        return;
                    }
                    EV_debug_log("SYSP\\eSYSP_et_sys.c", 384, "LOAD ERROR %c%c%c%c %d %d\n", syspNpc.characters[0], syspNpc.characters[1],
                        syspNpc.characters[2], syspNpc.characters[3], result, syspRetryCount);
                    errorLine = 385;
                    break;
                }
                case -5:
                case -2:
                case -1:
                    EV_debug_log("SYSP\\eSYSP_et_sys.c", 370, "LOAD ERROR %c%c%c%c %d\n", syspNpc.characters[0], syspNpc.characters[1], syspNpc.characters[2],
                        syspNpc.characters[3], result);
                    errorLine = 371;
                    break;
                default:
                    return;
                }
                EV_debug_log("SYSP\\eSYSP_et_sys.c", errorLine, "");
                syspNpc = {};
                syspEventMode = SyspFinished;
                if (syspNpcWasVisible)
                    EV_npcLockUnlock({});
                return;
            }
            if (syspTargetNpcPosition)
                EV_AT_SetPos(syspNpc, SYSP_RELATIVE_XYZ_RECURSIVE, *syspTargetNpcPosition);
            EV_GetCharaPosition(syspNpc, CHARACTER_TRANSLATION_ROOT, &syspSavedNpcPosition, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(syspNpc, &syspSavedNpcRotation, EventTransformFlags::World);
            GetMotionBonePosition(scenFindMotionwork(SYSP_RYO_), 6, &position);
            const std::int16_t difference =
                static_cast<std::int16_t>(InverseFixedAtan2(position.x - syspSavedNpcPosition.x, position.z - syspSavedNpcPosition.z) - syspSavedNpcRotation.y);
            const std::int32_t turn =
                difference < 0 ? (difference + 0x2000 < 0 ? difference + 0x2000 : 0) : (difference - 0x2000 > 0 ? difference - 0x2000 : 0);
            if (turn)
                CopyMotionTransition(scenFindMotionwork(syspNpc), nullptr, static_cast<std::uint16_t>(syspSavedNpcRotation.y + turn), 13, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(syspNpc), &position, 1);
        }
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(SYSP_RYO_, &rotation, EventTransformFlags::World);
        const std::int16_t difference =
            static_cast<std::int16_t>(InverseFixedAtan2(syspTargetPosition.x - position.x, syspTargetPosition.z - position.z) - rotation.y);
        const std::int32_t turn = difference < 0 ? (difference + 0x2000 < 0 ? difference + 0x2000 : 0) : (difference - 0x2000 > 0 ? difference - 0x2000 : 0);
        if (turn)
            CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, static_cast<std::uint16_t>(rotation.y + turn), 13, 0);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), &syspTargetPosition, 1);
        SyspSwitchToEventCamera();
        syspEventMode = SyspBlendIn;
        return;
    }
    case SyspBlendIn:
        SyspUpdateCameraBlendAndRyoFade(0);
        if (!mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1))
            syspEventMode = SyspRyoSetup;
        return;
    case SyspRyoSetup:
        if (SyspUpdateCameraBlendAndRyoFade(0))
            syspEventMode = SyspDialogueStart;
        return;
    case SyspDialogueStart:
        if (*syspVoiceScript) {
            syspConversation = EV_PlayDialogue(syspVoiceScript);
            syspVoiceScript += 9;
            syspEventMode = SyspDialogueActive;
        } else
            syspEventMode = SyspFinished;
        SyspUpdateCameraBlendAndRyoFade(0);
        return;
    case SyspDialogueActive:
        if (!cvsHasValidConverseData(syspConversation))
            syspEventMode = SyspDialogueStart;
        SyspUpdateCameraBlendAndRyoFade(0);
        return;
    default:
        return;
    }
}

void SyspNpcCallback(void *) {
    switch (syspEventMode) {
    case SyspFinished:
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), nullptr, 0);
        SyspEndCallback(nullptr);
        return;
    case SyspInitialize:
    case SyspBlendIn:
        if (*syspVoiceScript) {
            syspConversation = EV_PlayDialogue(syspVoiceScript);
            syspVoiceScript += 9;
            syspEventMode = SyspRyoSetup;
        } else
            syspEventMode = SyspDialogueStart;
        syspElapsedFrames = 0;
        syspRyoNearRen = 0;
        return;
    case SyspRyoSetup:
        if (!cvsHasValidConverseData(syspConversation))
            syspEventMode = SyspBlendIn;
        return;
    case SyspDialogueStart:
        CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, static_cast<std::uint16_t>(syspEventYaw), 5, 2);
        syspEventMode = SyspDialogueActive;
        return;
    case SyspDialogueActive:
        if (mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1))
            return;
        if (ctrlGetCharaByName(SYSP_REN_)) {
            Vector3 ryo, ren;
            EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
            EV_GetCharaPosition(SYSP_REN_, CHARACTER_TRANSLATION_ROOT, &ren, EventTransformFlags::World);
            const float distance = ((ryo.x - ren.x) * (ryo.x - ren.x) + (ryo.y - ren.y) * (ryo.y - ren.y)) + (ryo.z - ren.z) * (ryo.z - ren.z);
            EV_debug_log("SYSP\\eSYSP_et_sys.c", 522, "DDD %f\n", distance);
            if (distance < 2.5600002f) {
                syspRyoNearRen = 1;
                syspNpcWasVisible = static_cast<std::int32_t>(IsNpcLoadedOrVisible(syspNpc));
                EV_CheckWarmStartNpcRoute(syspNpc);
                EV_PTYCUtils(syspNpc, 3, 1);
                CopyMotionTransition(scenFindMotionwork(SYSP_REN_), nullptr, static_cast<std::uint16_t>(syspEventYaw), 5, 2);
            }
        }
        syspEventMode = SyspWaitOrBranch;
        [[fallthrough]];
    case SyspWaitOrBranch: {
        syspElapsedFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspElapsedFrames) + 1U);
        if (ryoIsCurrentScenarioFlagSatisfied())
            return;
        Vector3 position;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        syspPreviousRyoPosition.x = position.x;
        syspPreviousRyoPosition.y = position.y;
        syspPreviousRyoPosition.z = position.z;
        syspEventMode = SyspCheckBattle;
        [[fallthrough]];
    }
    case SyspCheckBattle: {
        EV_debug_log("SYSP\\eSYSP_et_sys.c", 547, "T %d\n", syspElapsedFrames);
        syspElapsedFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspElapsedFrames) + 1U);
        if (syspRyoNearRen) {
            if (syspNpc.name && !syspNpcWasVisible)
                EV_ReleaseNpcLockPreservingMotion(syspNpc);
            EV_PTYCUtils(syspNpc, 3, 0);
        }
        Vector3 position;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        Matrix4x4_Push();
        matrix_stack::workTransformMatrix = matrix_stack::transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const float cosine = sin_fixed16(0x4000), sine = sin_fixed16(0);
        Matrix4x4::RotateAroundZ(sine, cosine);
        const std::uint16_t yaw = static_cast<std::uint16_t>(syspEventYaw);
        const std::int16_t folded = static_cast<std::int16_t>(syspEventYaw <= 0x8000U ? syspEventYaw : 0U - syspEventYaw);
        Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)));
        Matrix4x4::RotateAroundX(sine, cosine);
        const Matrix4x4 transform = matrix_stack::workTransformMatrix;
        Matrix4x4::Pop(1);
        const float dy = syspPreviousRyoPosition.y - transform.rows[3].y, dz = syspPreviousRyoPosition.z - transform.rows[3].z;
        const float dx = syspPreviousRyoPosition.x - transform.rows[3].x;
        position.x = (transform.rows[0].y * dy + transform.rows[0].x * dx) + transform.rows[0].z * dz;
        position.y = (transform.rows[1].y * dy + transform.rows[1].x * dx) + transform.rows[1].z * dz;
        position.z = (transform.rows[2].y * dy + transform.rows[2].x * dx) + transform.rows[2].z * dz;
        if (position.z > 1.0f || (syspElapsedFrames > 180 && position.z > 0.15000001f)) {
            CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, yaw, 13, 0);
            syspEventMode = SyspPostRotate;
        }
        return;
    }
    case SyspPostRotate:
        if (!mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1)) {
            syspEventMode = SyspFinished;
            UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), nullptr, 0);
            SyspEndCallback(nullptr);
        }
        return;
    default:
        return;
    }
}

void SyspNpcCharacterCallback(void *) {
    EV_debug_log("SYSP\\eSYSP_et_sys.c", 605, "eSYSP_mode : %d\n", syspEventMode);
    switch (syspEventMode) {
    case SyspFinished:
        SyspEndCallback(nullptr);
        return;
    case SyspInitialize: {
        syspElapsedFrames = 0;
        const std::int32_t result = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(EV_npcStreamOrLoadHuman(syspNpc)));
        if (result != 1) {
            switch (result) {
            case -6:
            case -4:
            case -3: {
                const std::int32_t previous = syspRetryCount;
                syspRetryCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspRetryCount) + 1U);
                if (previous <= 2) {
                    PrepareNpcAllFromArray();
                    return;
                }
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 632, "LOAD ERROR %c%c%c%c %d %d\n", syspNpc.characters[0], syspNpc.characters[1], syspNpc.characters[2],
                    syspNpc.characters[3], result, syspRetryCount);
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 633, "");
                syspNpc = {};
                syspEventMode = SyspWaitOrBranch;
                break;
            }
            case -5:
            case -2:
            case -1:
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 619, "LOAD ERROR %c%c%c%c %d\n", syspNpc.characters[0], syspNpc.characters[1], syspNpc.characters[2],
                    syspNpc.characters[3], result);
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 620, "");
                syspNpc = {};
                syspEventMode = SyspWaitOrBranch;
                break;
            default:
                break;
            }
            return;
        }
        if (syspTargetNpcPosition)
            EV_AT_SetPos(syspNpc, SYSP_RELATIVE_XYZ_RECURSIVE, *syspTargetNpcPosition);
        EV_debug_log("SYSP\\eSYSP_et_sys.c", 650, "type 2 chara %p\n", static_cast<const void *>(syspTargetNpcPosition));
        EV_GetCharaPosition(syspNpc, CHARACTER_TRANSLATION_ROOT, &syspSavedNpcPosition, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(syspNpc, &syspSavedNpcRotation, EventTransformFlags::World);
        syspSavedMotion = GetMotionAnimationId(scenFindMotionwork(syspNpc), MotionRegion::LowerBody);
        syspSavedMotionFrame = GetMotionFrame(scenFindMotionwork(syspNpc), MotionRegion::LowerBody);
        Vector3 position;
        GetMotionBonePosition(scenFindMotionwork(SYSP_RYO_), 6, &position);
        const std::int16_t difference =
            static_cast<std::int16_t>(InverseFixedAtan2(position.x - syspSavedNpcPosition.x, position.z - syspSavedNpcPosition.z) - syspSavedNpcRotation.y);
        const std::int32_t turn = difference < 0 ? (difference + 0x2000 < 0 ? difference + 0x2000 : 0) : (difference - 0x2000 > 0 ? difference - 0x2000 : 0);
        if (turn)
            CopyMotionTransition(scenFindMotionwork(syspNpc), nullptr, static_cast<std::uint16_t>(syspSavedNpcRotation.y + turn), 13, 0);
        UpdateMotionWorkWithPosition(scenFindMotionwork(syspNpc), &position, 1);
        syspEventMode = SyspBlendIn;
        [[fallthrough]];
    }
    case SyspBlendIn: {
        if (mtwkSetExtraFlag(scenFindMotionwork(syspNpc), 58, 1))
            return;
        Vector3 target, position;
        Vector3i rotation;
        GetMotionBonePosition(scenFindMotionwork(syspNpc), 6, &target);
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(SYSP_RYO_, &rotation, EventTransformFlags::World);
        const std::int16_t difference = static_cast<std::int16_t>(InverseFixedAtan2(target.x - position.x, target.z - position.z) - rotation.y);
        const std::int32_t turn = difference < 0 ? (difference + 0x2000 < 0 ? difference + 0x2000 : 0) : (difference - 0x2000 > 0 ? difference - 0x2000 : 0);
        if (turn)
            CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, static_cast<std::uint16_t>(rotation.y + turn), 13, 0);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), &target, 1);
        syspEventMode = SyspDialogueStart;
        return;
    }
    case SyspDialogueStart:
        if (*syspVoiceScript) {
            syspConversation = EV_PlayDialogue(syspVoiceScript);
            syspVoiceScript += 9;
            syspEventMode = SyspDialogueActive;
        } else
            syspEventMode = SyspWaitOrBranch;
        return;
    case SyspDialogueActive: {
        if (!cvsHasValidConverseData(syspConversation)) {
            syspEventMode = SyspDialogueStart;
            return;
        }
        const shen_name speaker = cvsFindVoiceDataFor(syspConversation);
        if (speaker.name) {
            const shen_name other = speaker.name == SYSP_RYO_.name ? syspNpc : SYSP_RYO_;
            camCtrlSetState(CameraState::Event);
            Vector3 position, target;
            GetMotionBonePosition(scenFindMotionwork(other), SYSP_RIGHT_UPPER_LEG, &position);
            GetMotionBonePosition(scenFindMotionwork(speaker), 6, &target);
            Vector3 direction{.x = position.x - target.x, .y = position.y - target.y, .z = position.z - target.z};
            const float dz = direction.z;
            Vector3::NormalizeInPlace(direction);
            if (dz > 3.0f) {
                position.x = direction.x * 3.0f + target.x;
                position.y = direction.y * 3.0f + target.y;
                position.z = direction.z * 3.0f + target.z;
            }
            camCtrlSetNextPosition(position);
            modelSetCharaAlpha(ctrlGetCharaByName(other), 0.0f);
            camCtrlSetTargetPosition(target);
            modelSetCharaAlpha(ctrlGetCharaByName(speaker), 1.0f);
        }
        return;
    }
    case SyspWaitOrBranch:
        CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, static_cast<std::uint16_t>(syspEventYaw), 5, 2);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), nullptr, 0);
        camCtrlSetState(CameraState::Init);
        syspEventMode = SyspCheckBattle;
        return;
    case SyspCheckBattle:
        if (mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1))
            return;
        if (syspNpc.name) {
            if (syspNpc.name == SYSP_REN_.name) {
                UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_REN_), nullptr, 0);
                Vector3 ryo, ren;
                EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
                EV_GetCharaPosition(SYSP_REN_, CHARACTER_TRANSLATION_ROOT, &ren, EventTransformFlags::World);
                const float distance = ((ryo.x - ren.x) * (ryo.x - ren.x) + (ryo.y - ren.y) * (ryo.y - ren.y)) + (ryo.z - ren.z) * (ryo.z - ren.z);
                EV_debug_log("SYSP\\eSYSP_et_sys.c", 769, "DDD %f\n", distance);
                if (distance < 2.5600002f)
                    CopyMotionTransition(scenFindMotionwork(SYSP_REN_), nullptr, static_cast<std::uint16_t>(syspEventYaw), 5, 2);
            } else {
                UpdateMotionWorkWithPosition(scenFindMotionwork(syspNpc), nullptr, 0);
                CopyMotionTransition(scenFindMotionwork(syspNpc), &syspSavedNpcPosition, static_cast<std::uint16_t>(syspSavedNpcRotation.y), 0, 0);
                SetMotionTimeRelated(scenFindMotionwork(syspNpc), 1, 5, static_cast<std::uint16_t>(syspSavedMotion));
                SetMotionRegionEvalTime(scenFindMotionwork(syspNpc), syspSavedMotionFrame, MotionRegion::LowerBody);
            }
        }
        syspEventMode = SyspPostRotate;
        [[fallthrough]];
    case SyspPostRotate: {
        syspElapsedFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspElapsedFrames) + 1U);
        if (ryoIsCurrentScenarioFlagSatisfied())
            return;
        Vector3 position;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        syspPreviousRyoPosition.x = position.x;
        syspPreviousRyoPosition.y = position.y;
        syspPreviousRyoPosition.z = position.z;
        syspEventMode = SyspTrackRyoMove;
        [[fallthrough]];
    }
    case SyspTrackRyoMove: {
        EV_debug_log("SYSP\\eSYSP_et_sys.c", 784, "T %d\n", syspElapsedFrames);
        syspElapsedFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspElapsedFrames) + 1U);
        if (syspNpc.name && !syspNpcWasVisible)
            EV_ReleaseNpcLockPreservingMotion(syspNpc);
        EV_PTYCUtils(syspNpc, 3, 0);
        Vector3 position;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        Matrix4x4_Push();
        matrix_stack::workTransformMatrix = matrix_stack::transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const float cosine = sin_fixed16(0x4000), sine = sin_fixed16(0);
        Matrix4x4::RotateAroundZ(sine, cosine);
        const std::uint16_t yaw = static_cast<std::uint16_t>(syspEventYaw);
        const std::int16_t folded = static_cast<std::int16_t>(syspEventYaw <= 0x8000U ? syspEventYaw : 0U - syspEventYaw);
        Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)));
        Matrix4x4::RotateAroundX(sine, cosine);
        const Matrix4x4 transform = matrix_stack::workTransformMatrix;
        Matrix4x4::Pop(1);
        const float dy = syspPreviousRyoPosition.y - transform.rows[3].y, dz = syspPreviousRyoPosition.z - transform.rows[3].z;
        const float dx = syspPreviousRyoPosition.x - transform.rows[3].x;
        position.x = (transform.rows[0].y * dy + transform.rows[0].x * dx) + transform.rows[0].z * dz;
        position.y = (transform.rows[1].y * dy + transform.rows[1].x * dx) + transform.rows[1].z * dz;
        position.z = (transform.rows[2].y * dy + transform.rows[2].x * dx) + transform.rows[2].z * dz;
        if (position.z > 1.0f || (syspElapsedFrames > 180 && position.z > 0.15000001f)) {
            CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, yaw, 13, 0);
            syspEventMode = SyspFinalRotate;
        }
        return;
    }
    case SyspFinalRotate:
        if (!mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1)) {
            syspEventMode = SyspFinished;
            SyspEndCallback(nullptr);
        }
        return;
    default:
        return;
    }
}

void SyspItemCallback(void *) {
    switch (syspEventMode) {
    case SyspFinished:
        SyspEndCallback(nullptr);
        return;
    case SyspInitialize: {
        syspItemAlphaState = SyspItemInactive;
        if (!PrepareNpcAllFromArray())
            break;
        mtwkRequestMotion(scenFindMotionwork(SYSP_RYO_), SYSP_ITEM_BEGIN_MOTION, MotionRegion::LowerBody);
        SyspSwitchToEventCamera();
        syspSecondaryPhaseScale = 1.0f;
        syspFovPercent = 88;
        syspTargetPosition = {.x = 0.105653f, .y = 1.546608f, .z = -0.33885401f};
        syspRyoTargetPosition = {.x = 0.443643f, .y = 2.009285f, .z = 0.081377f};
        syspCameraPhaseStep = static_cast<std::int32_t>(static_cast<float>(syspCameraPhaseStep) * 0.75f);
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(SYSP_RYO_, &rotation, EventTransformFlags::World);
        Matrix4x4_Push();
        matrix_stack::workTransformMatrix = matrix_stack::transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.x));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.z));
        Matrix4x4::TransformPoint(syspTargetPosition, syspTargetPosition);
        Matrix4x4::TransformPoint(syspRyoTargetPosition, syspRyoTargetPosition);
        Matrix4x4::Pop(1);
        ++syspEventMode;
        break;
    }
    case SyspBlendIn:
        SyspUpdateCameraBlendAndRyoFade(1);
        if (GetMotionFrame(scenFindMotionwork(SYSP_RYO_), MotionRegion::LowerBody) < 40.0f)
            break;
        SetLowQualityHands(ctrlGetCharaByName(SYSP_RYO_));
        SetHandRegionXYZWithFlags(ctrlGetCharaByName(SYSP_RYO_), 1, 0, 0, -1);
        SetHandRegionXYZWithFlags(ctrlGetCharaByName(SYSP_RYO_), 1, 1, 0, -1);
        ++syspEventMode;
        break;
    case SyspRyoSetup:
        if (SetMotionRegionFlag(scenFindMotionwork(SYSP_RYO_), 65, 1, MotionRegion::LowerBody)) {
            mtwkRequestMotion(scenFindMotionwork(SYSP_RYO_), SYSP_ITEM_LOOP_MOTION, MotionRegion::LowerBody);
            ++syspEventMode;
        }
        SyspUpdateCameraBlendAndRyoFade(1);
        break;
    case SyspDialogueStart:
        if (SyspUpdateCameraBlendAndRyoFade(1))
            ++syspEventMode;
        break;
    case SyspDialogueActive:
        SyspUpdateCameraBlendAndRyoFade(1);
        if (*syspVoiceScript) {
            syspConversation = EV_PlayDialogue(syspVoiceScript);
            syspVoiceScript += 9;
            ++syspEventMode;
        } else
            syspEventMode += 2;
        break;
    case SyspWaitOrBranch:
        SyspUpdateCameraBlendAndRyoFade(1);
        if (!cvsHasValidConverseData(syspConversation))
            --syspEventMode;
        break;
    case SyspCheckBattle:
        SyspUpdateCameraBlendAndRyoFade(1);
        if (GetBattleDataConfig(2, SYSP_CANCEL_BUTTON) || GetCurrentControlInput(1) == 1 || std::bit_cast<std::int32_t>(syspWaitTicks) >= 1800) {
            mtwkSetRequestedRootMode(scenFindMotionwork(SYSP_RYO_), 1);
            SetMotionFrameTime(scenFindMotionwork(SYSP_RYO_), 4, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYSP_RYO_), SYSP_ITEM_END_MOTION, MotionRegion::LowerBody);
            camCtrlSetState(CameraState::Init);
            ++syspEventMode;
        } else {
            GetBattleDataConfig(0, SYSP_CANCEL_BUTTON);
            ++syspWaitTicks;
        }
        break;
    case SyspPostRotate:
        if (GetMotionFrame(scenFindMotionwork(SYSP_RYO_), MotionRegion::LowerBody) < 6.0f)
            break;
        syspItemAlphaState = SyspItemFadeOut;
        if (GetMotionFrame(scenFindMotionwork(SYSP_RYO_), MotionRegion::LowerBody) < 20.0f)
            break;
        SetHandRegionXYZWithFlags(ctrlGetCharaByName(SYSP_RYO_), 0, 0, 0, -1);
        SetHandRegionXYZWithFlags(ctrlGetCharaByName(SYSP_RYO_), 0, 1, 0, -1);
        ResetScenarioCharacterAttachment(AREA_SYSP);
        ++syspEventMode;
        break;
    case SyspTrackRyoMove:
        if (!SetMotionRegionFlag(scenFindMotionwork(SYSP_RYO_), 65, 1, MotionRegion::LowerBody))
            break;
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(syspAttachedItem), 0);
        syspEventMode = SyspFinished;
        SyspEndCallback(nullptr);
        return;
    default:
        break;
    }
    if (!syspAttachedItem)
        return;
    const Vector4 reflector{.x = 1, .y = 1, .z = 1, .w = 1};
    if (syspItemAlphaState) {
        switch (syspItemAlphaState) {
        case SyspItemWaitEvent:
            if (syspEventMode >= SyspRyoSetup) {
                syspItemAlpha = std::fmin(1.0f, syspItemAlpha + 0.050000001f);
                modelSetCharaAlpha(ctrlGetCharaByName(AREA_SYSP), syspItemAlpha);
                ++syspItemAlphaState;
            }
            break;
        case SyspItemFadeIn:
            syspItemAlpha = std::fmin(1.0f, syspItemAlpha + 0.050000001f);
            modelSetCharaAlpha(ctrlGetCharaByName(AREA_SYSP), syspItemAlpha);
            break;
        case SyspItemFadeOut:
            syspItemAlpha = std::fmax(0.0f, syspItemAlpha - 0.1f);
            modelSetCharaAlpha(ctrlGetCharaByName(AREA_SYSP), syspItemAlpha);
            SetScenarioCharacterDrawControl(AREA_SYSP, 0, 1);
            break;
        default:
            break;
        }
    } else if (Character *const item = ctrlGetCharaByName(AREA_SYSP)) {
        const Vector3 position{.x = 0.19f, .y = 0.039999999f, .z = -0.044f};
        const Vector3i rotation{.x = 63897, .y = 44600, .z = 52246};
        AttachScenarioCharacter(AREA_SYSP, SYSP_RYO_, 14, &position, &rotation);
        SetFixoAlphaPropagation(item, 1);
        syspItemAlpha = 0.0f;
        modelSetCharaAlpha(ctrlGetCharaByName(AREA_SYSP), 0.0f);
        SetScenarioReflectorBoardValues(AREA_SYSP, &reflector);
        SetScenarioCharacterType(AREA_SYSP, CharacterTypeModelShadow);
        SetScenarioCharacterDrawControl(AREA_SYSP, 1, 1);
        ++syspItemAlphaState;
    }
}

void SyspRyoCallback(void *) {
    switch (syspEventMode) {
    case SyspFinished:
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), nullptr, 0);
        SyspEndCallback(nullptr);
        return;
    case SyspInitialize: {
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(SYSP_RYO_, &rotation, EventTransformFlags::World);
        const std::int16_t difference =
            static_cast<std::int16_t>(InverseFixedAtan2(syspTargetPosition.x - position.x, syspTargetPosition.z - position.z) - rotation.y);
        const std::int32_t turn = difference < 0 ? (difference + 0x2000 < 0 ? difference + 0x2000 : 0) : (difference - 0x2000 > 0 ? difference - 0x2000 : 0);
        if (turn)
            CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, static_cast<std::uint16_t>(rotation.y + turn), 13, 0);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), &syspTargetPosition, 1);
        SyspSwitchToEventCamera();
        syspEventMode = SyspBlendIn;
        return;
    }
    case SyspBlendIn:
        SyspUpdateCameraBlendAndRyoFade(0);
        if (!mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1))
            syspEventMode = SyspRyoSetup;
        return;
    case SyspRyoSetup:
        if (SyspUpdateCameraBlendAndRyoFade(0)) {
            syspEventMode = SyspDialogueStart;
            syspNextDialogueMode = SyspDialogueActive;
            if (syspNpc.name)
                syspNpcWasVisible = static_cast<std::int32_t>(IsNpcLoadedOrVisible(syspNpc));
        }
        return;
    case SyspDialogueStart:
        if (*syspVoiceScript) {
            syspConversation = EV_PlayDialogue(syspVoiceScript);
            syspVoiceScript += 9;
            syspEventMode = syspNextDialogueMode;
        } else
            syspEventMode = SyspWaitOrBranch;
        SyspUpdateCameraBlendAndRyoFade(0);
        return;
    case SyspDialogueActive:
        SyspUpdateCameraBlendAndRyoFade(0);
        if (!cvsHasValidConverseData(syspConversation))
            syspEventMode = SyspDialogueStart;
        return;
    case SyspWaitOrBranch:
        if (syspNextDialogueMode == SyspDialogueCameraSwap) {
            if (!syspNpcWasVisible)
                EV_npcLockUnlock(syspNpc);
            EV_PTYCUtils(syspNpc, 3, 0);
        }
        camCtrlSetState(CameraState::Init);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), nullptr, 0);
        CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, static_cast<std::uint16_t>(syspEventYaw), 5, 2);
        syspEventMode = SyspCheckBattle;
        return;
    case SyspCheckBattle:
        if (mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1))
            return;
        syspEventMode = SyspPostRotate;
        [[fallthrough]];
    case SyspPostRotate: {
        if (ryoIsCurrentScenarioFlagSatisfied())
            return;
        Vector3 position;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        syspPreviousRyoPosition.x = position.x;
        syspPreviousRyoPosition.y = position.y;
        syspPreviousRyoPosition.z = position.z;
        syspEventMode = SyspTrackRyoMove;
        [[fallthrough]];
    }
    case SyspTrackRyoMove: {
        Vector3 position;
        EV_GetCharaPosition(SYSP_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        Matrix4x4_Push();
        matrix_stack::workTransformMatrix = matrix_stack::transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const float cosine = sin_fixed16(0x4000), sine = sin_fixed16(0);
        Matrix4x4::RotateAroundZ(sine, cosine);
        const std::uint16_t yaw = static_cast<std::uint16_t>(syspEventYaw);
        const std::int16_t folded = static_cast<std::int16_t>(syspEventYaw <= 0x8000U ? syspEventYaw : 0U - syspEventYaw);
        Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)));
        Matrix4x4::RotateAroundX(sine, cosine);
        const Matrix4x4 transform = matrix_stack::workTransformMatrix;
        Matrix4x4::Pop(1);
        const float dy = syspPreviousRyoPosition.y - transform.rows[3].y, dz = syspPreviousRyoPosition.z - transform.rows[3].z;
        const float dx = syspPreviousRyoPosition.x - transform.rows[3].x;
        position.x = (transform.rows[0].y * dy + transform.rows[0].x * dx) + transform.rows[0].z * dz;
        position.y = (transform.rows[1].y * dy + transform.rows[1].x * dx) + transform.rows[1].z * dz;
        position.z = (transform.rows[2].y * dy + transform.rows[2].x * dx) + transform.rows[2].z * dz;
        if (position.z > 1.0f) {
            CopyMotionTransition(scenFindMotionwork(SYSP_RYO_), nullptr, yaw, 13, 0);
            syspEventMode = SyspFinalRotate;
        }
        return;
    }
    case SyspFinalRotate:
        if (!mtwkSetExtraFlag(scenFindMotionwork(SYSP_RYO_), 58, 1)) {
            syspEventMode = SyspFinished;
            UpdateMotionWorkWithPosition(scenFindMotionwork(SYSP_RYO_), nullptr, 0);
            SyspEndCallback(nullptr);
        }
        return;
    case SyspDialogueCameraSwap:
        if (cvsHasValidConverseData(syspConversation)) {
            const shen_name speaker = cvsFindVoiceDataFor(syspConversation);
            if (speaker.name) {
                const shen_name other = speaker.name == SYSP_RYO_.name ? syspNpc : SYSP_RYO_;
                camCtrlSetState(CameraState::Event);
                Vector3 position;
                GetMotionBonePosition(scenFindMotionwork(other), SYSP_RIGHT_UPPER_LEG, &position);
                camCtrlSetNextPosition(position);
                modelSetCharaAlpha(ctrlGetCharaByName(other), 0.0f);
                GetMotionBonePosition(scenFindMotionwork(speaker), 6, &position);
                camCtrlSetTargetPosition(position);
                modelSetCharaAlpha(ctrlGetCharaByName(speaker), 1.0f);
            }
        } else
            syspEventMode = SyspDialogueStart;
        return;
    default:
        return;
    }
}

void SyspOtherCallback(void *) {
    if (syspEventMode == SyspFinished)
        SyspEndCallback(nullptr);
    else if (syspEventMode) {
        if (syspEventMode == SyspBlendIn)
            syspEventMode = cvsHasValidConverseData(syspConversation);
    } else if (*syspVoiceScript) {
        syspConversation = EV_PlayDialogue(syspVoiceScript);
        syspVoiceScript += 9;
        syspEventMode = SyspBlendIn;
    } else
        syspEventMode = SyspFinished;
}

std::int16_t SyspSwitchToEventCamera() {
    camCtrlSetState(CameraState::Event);
    camCtrlGetTargetPosition(&syspPreviousCameraPosition);
    camCtrlGetNextCamPos(&syspPreviousNextCameraPosition);
    const std::int16_t perspective = static_cast<std::int16_t>(camCtrlGetPerspective());
    syspSecondaryPhaseScale = 2.0f;
    syspPreviousPerspective = perspective;
    syspCameraPhase = 16384;
    syspCameraPhaseStep = 364;
    return perspective;
}

bool SyspUpdateCameraBlendAndRyoFade(std::int32_t skipRyoFade) {
    const std::int32_t phase = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspCameraPhase) + static_cast<std::uint32_t>(syspCameraPhaseStep));
    syspCameraPhase = phase < 49152 ? phase : 49152;
    const float blend = 1.0f - (sin_fixed16(static_cast<std::int16_t>(syspCameraPhase)) + 1.0f) * 0.5f;
    Vector3 position{.x = (1.0f - blend) * syspPreviousCameraPosition.x + syspTargetPosition.x * blend,
        .y = (1.0f - blend) * syspPreviousCameraPosition.y + syspTargetPosition.y * blend,
        .z = (1.0f - blend) * syspPreviousCameraPosition.z + syspTargetPosition.z * blend};
    camCtrlSetTargetPosition(position);
    const std::int32_t scale = syspFovPercent > 899 ? std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspFovPercent) - 1000U) : syspFovPercent;
    const std::int32_t inverseScale = std::bit_cast<std::int32_t>(100U - static_cast<std::uint32_t>(scale));
    const float perspective =
        ((static_cast<float>(inverseScale) * 7281.0f / 100.0f) + 3640.0f) * blend + static_cast<float>(syspPreviousPerspective) * (1.0f - blend);
    camCtrlSetPerspective(static_cast<std::uint16_t>(TruncateToInt32(perspective)));
    if (syspFovPercent < 900) {
        if (!skipRyoFade)
            GetMotionBonePosition(scenFindMotionwork(SYSP_RYO_), 6, &syspRyoTargetPosition);
        const float relativePhase = static_cast<float>(std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(syspCameraPhase) - 16384U));
        const float secondaryPhase = relativePhase * syspSecondaryPhaseScale + 16384.0f;
        const std::uint16_t angle = secondaryPhase < 49152.0f ? static_cast<std::uint16_t>(TruncateToInt32(secondaryPhase)) : 49152U;
        const float radius = 1.0f - (sin_fixed16(static_cast<std::int16_t>(angle)) + 1.0f) * 0.5f;
        position.x = (1.0f - radius) * syspPreviousNextCameraPosition.x + syspRyoTargetPosition.x * radius;
        position.y = (1.0f - radius) * syspPreviousNextCameraPosition.y + syspRyoTargetPosition.y * radius;
        position.z = (1.0f - radius) * syspPreviousNextCameraPosition.z + syspRyoTargetPosition.z * radius;
        camCtrlSetNextPosition(position);
        if (!skipRyoFade) {
            const float alphaPhase = relativePhase * 2.2f + 16384.0f;
            const std::uint16_t alphaAngle = alphaPhase < 49152.0f ? static_cast<std::uint16_t>(TruncateToInt32(alphaPhase)) : 49152U;
            const float alpha = 1.0f - (1.0f - (sin_fixed16(static_cast<std::int16_t>(alphaAngle)) + 1.0f) * 0.5f);
            modelSetCharaAlpha(ctrlGetCharaByName(SYSP_RYO_), alpha);
        }
    }
    return syspCameraPhase >= 32768;
}

void SyspEndCallback(void *) {
    FinishLoading();
    if (syspFlags & 2) {
        EV_EventSetUpHelpIcon_0(SYSP_HELP_MONEY_NEEDED, 0);
        SetupCurrentHelpIcon();
        syspFlags &= ~2;
        HLib::SetCurrentTaskTo(SyspEndCallback);
    } else {
        EV_debug_log("SYSP\\eSYSP_et_sys.c", 1175, "end of SYSP : eSYSP_Next %d\n", syspNextEvent);
        if (syspFlags & 1)
            SetScenarioFollowCharacter({});
        HLib::DestroyCurrentTask();
        if (syspNextEvent)
            SyspMain(syspNextEvent);
        else {
            scenFreeEvent("SYSP");
            if (!syspState)
                SetPlayerControlEnabled(1);
            EV_NPC_NewLoadSw(1);
        }
    }
}
} // namespace shenmue::s2::areas
