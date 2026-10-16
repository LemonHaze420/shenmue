#include "s2/areas/area_kebi.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/math.hpp"

namespace shenmue::s2::areas {
// @verified - p
void AreaKEBIInit() {
}
// @verified - p
void AreaKEBIMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KEBICheckCallback, 5, 0x0b, sizeof(KebiTaskData), KEBI_TASK);
    KebiTaskData *const work = static_cast<KebiTaskData *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->frame = 0;
    work->motionBank = ReadMotiFromDirectory(g_actNumber, KEBI_MOTION_MAP, "0075E.MOT");
    EV_FreeAuthorPak(1, "0075");
}
// @verified - p
void KEBICheckCallback(void *) {
    HLib::SetCurrentTaskTo(KEBIMainCallback);
}
// @verified - p
void KEBIMainCallback(void *data) {
    KebiTaskData &work = *static_cast<KebiTaskData *>(data);
    switch (work.frame) {
    case 0: {
        SetPlayerControlEnabled(0);
        EV_NPC_NewLoadSw(0);
        EV_CheckWarmStartNpcRoute(KEBI_GUARD);
        EV_ResetFreeTalkFlags();
        Vector3 position;
        EV_GetCharaPosition(KEBI_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.x = 245.3f;
        position.z = 303.89999f;
        EV_AT_SetPos(KEBI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(KEBI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
        mtwkRequestMotion(scenFindMotionwork(KEBI_PLAYER), 0xf002, MotionRegion::LowerBody);
        ++work.frame;
        break;
    }
    case 1:
        if (EV_ForceLoadNpc(KEBI_GUARD)) {
            const Vector4 reflector{1, .30000001f, .30000001f, .30000001f};
            SetReflectorBoardValues(FindCharacterByScenarioName(KEBI_PLAYER), reflector);
            EV_SetRefBoardParamsFor(KEBI_PLAYER, 1);
            SetReflectorBoardValues(FindCharacterByScenarioName(KEBI_GUARD), reflector);
            EV_SetRefBoardParamsFor(KEBI_GUARD, 1);
            EV_SetRefBoardParamsFor(KEBI_PLAYER, 1);
            EV_SetRefBoardParamsFor(KEBI_GUARD, 1);
            EV_SetCharacterAlphaEnabled(KEBI_GUARD, false);
            Vector3 playerPosition;
            EV_GetCharaPosition(KEBI_PLAYER, 5, &playerPosition, EventTransformFlags::Local);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KEBI_GUARD), &playerPosition, 0);
            EV_GetCharaPosition(KEBI_PLAYER, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
            Vector3 guardPosition;
            EV_GetCharaPosition(KEBI_GUARD, CHARACTER_TRANSLATION_ROOT, &guardPosition, EventTransformFlags::World);
            const std::int32_t yaw = InverseFixedAtan2(playerPosition.x - guardPosition.x, playerPosition.z - guardPosition.z);
            EV_GetCharaPosition(KEBI_GUARD, CHARACTER_TRANSLATION_ROOT, &guardPosition, EventTransformFlags::World);
            CopyMotionTransition(scenFindMotionwork(KEBI_GUARD), &guardPosition, static_cast<std::uint16_t>(yaw), 0, 0);
            ++work.frame;
            EventSetCamera camera{0, 3150, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, {}, 0, 0};
            EVT_AT_SetCam_Callback(&camera);
            camCtrlDisableFilmBars();
            EVT_AT_SetVoice(GetRandomNumberInRange(2.0f) == 0 ? "0075B001" : "0075B002", 1, 0); // @text 0075B001 EN: "Hey, wait!" | JA: "ああ、ちょっと！" ; 0075B002 EN: "Wait up fella!" | JA: "ちょっと、君ィ！"
        }
        break;
    case 2:
        if (!mtwkSetExtraFlag(scenFindMotionwork(KEBI_GUARD), 58, 1)) {
            EVT_AT_SetMot(KEBI_GUARD, KEBI_STOP_MOTION, 0, 1, 0x80000000U, 0);
            Vector3 position;
            EV_GetCharaPosition(KEBI_GUARD, 5, &position, EventTransformFlags::Local);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KEBI_PLAYER), &position, 0);
            EVT_AT_SetCam(3160, KEBI_GUARD, KEBI_GUARD, 0, 0, 20);
            EVT_AT_SetCam(3170, KEBI_GUARD, KEBI_GUARD, 0, 0, 241);
            const float choice = GetRandomNumberInRange(3.0f);
            EVT_AT_SetVoice(choice == 0 ? "0075B003" : choice == 1 ? "0075B004" : "0075B005", 1, 50); // @text 0075B003 EN: "This place is for authorized\npersonnel only." | JA: "関係者以外は、\n立ち入り禁止だから。" ; 0075B004 EN: "You can't come in here." | JA: "一般の人は、入れないんだ。" ; 0075B005 EN: "If you don't have a permit,\nyou can't come in." | JA: "許可がないのに\n入っちゃダメだろ。"
            EVT_AT_SetVoice(GetRandomNumberInRange(2.0f) == 0 ? "0075B006" : "0075B007", 1, 140); // @text 0075B006 EN: "Go on, leave." | JA: "さっさと出て。" ; 0075B007 EN: "Stay away." | JA: "ほら、離れて。"
            ++work.frame;
        }
        break;
    case 23:
        EV_DisableCharacterDisplay(KEBI_PLAYER);
        break;
    case 80: {
        EventSetMotion motion{0, KEBI_GUARD, KEBI_DISMISS_MOTION, 0, 1.6f, 0x80000000U, 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(KEBI_DISMISS_MOTION);
        break;
    }
    case 230: {
        Vector3 position;
        EV_GetCharaPosition(KEBI_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.z = 306.0f;
        CopyMotionTransition(scenFindMotionwork(KEBI_PLAYER), &position, 0x8000, 0, 0);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KEBI_PLAYER), nullptr, 0);
        break;
    }
    case 244: {
        EventSetMotion motion{0, KEBI_GUARD, KEBI_IDLE_MOTION, 0, 1, 0x80000000U, 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(KEBI_IDLE_MOTION);
        EV_EnableCharacterDisplay(KEBI_PLAYER);
        break;
    }
    case 270:
        HLib::SetCurrentTaskTo(KEBIExitCallback);
        break;
    default:
        break;
    }
    if (work.frame >= 3)
        ++work.frame;
}
// @verified - p
void KEBIExitCallback(void *data) {
    KebiTaskData &work = *static_cast<KebiTaskData *>(data);
    EV_SetRefBoardParamsFor(KEBI_PLAYER, 0);
    EV_SetRefBoardParamsFor(KEBI_GUARD, 0);
    EV_npcFree(KEBI_GUARD);
    EV_npcLockUnlock(KEBI_GUARD);
    EV_NPC_NewLoadSw(1);
    DeleteMotion(work.motionBank);
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    SetPlayerControlEnabled(1);
    UpdateMotionWorkWithPosition(scenFindMotionwork(KEBI_PLAYER), nullptr, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(KEBI_GUARD), nullptr, 0);
    CleanupMapControlCharacters();
    EV_HLKillOtherHierarchyTask(work.self);
}
const ScriptAreaDefinition areaKEBIScriptDefinition{.areaId = AREA_KEBI, .field04 = 0, .init = AreaKEBIInit, .main = AreaKEBIMain};
} // namespace shenmue::s2::areas
