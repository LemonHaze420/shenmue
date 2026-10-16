#include "s2/areas/area_kami.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <array>
#include <bit>

namespace shenmue::s2::areas {
namespace {
struct KamiRuntime {
    HLib::Task *parent;
    HLib::Task *self;
    KamiAuthorRequest request;
};
static_assert(sizeof(KamiRuntime) == 48);
static_assert(offsetof(KamiRuntime, request) == 16);
KamiRuntime kami;
const shen_name characters[2]{KAMI_BARBER, KAMI_INVALID};
std::int32_t barberHand[57]{0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0,
    0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638};
void KAMIDebugNullsub() {
}
void DisablePluralControl() {
    SetPluralControlFor(FindCharacterByScenarioName(KAMI_PLAYER), 0);
    SetPluralControlFor(FindCharacterByScenarioName(KAMI_BARBER), 0);
}
void DisableControllers(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}
void EnablePlayerControllers() {
    EV_SetCharaControllerFlags(KAMI_PLAYER, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(KAMI_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(KAMI_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    mtwkSetExtraFlag(scenFindMotionwork(KAMI_PLAYER), 6, 0);
    EV_SetCharacterAlphaEnabled(KAMI_PLAYER, true);
}
void RetireCharacters() {
    for (const shen_name *character = characters; character->name != KAMI_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
}
void ResetFaceModels() {
    SetHighModelXYZ(FindCharacterByScenarioName(KAMI_BARBER), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(KAMI_BARBER), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(KAMI_PLAYER), FacialNodeOriginal);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(KAMI_BARBER), FacialNodeOriginal);
}
void RevealScene() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void DispatchAuthor() {
    ExecuteCurrAreaMainFunc_Wrapper(3, reinterpret_cast<std::uintptr_t>(&kami.request));
}
} // namespace
// @verified - p
void AreaKAMIInit() {
    kami = {};
}
// @verified - p
void AreaKAMIMain(std::uintptr_t parameter) {
    SetPlayerControlEnabled(0);
    SetRenderClockAndCompassTo(1);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    kami.self = HLib::EnqueueTaskWithoutParameter(KAMIInitializeCallback, 5, 0x0b, static_cast<std::int32_t>(KAMI_TASK.name));
    kami.request.state = 0;
    kami.parent = HLib::GetCurrentTask();
    kami.request.command = -1;
    kami.request.nextState = *reinterpret_cast<const std::int32_t *>(parameter) != 0 ? 4 : 6;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KAMI"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("KAMI"), 0);
    HLib::SetupTimedTask(kami.self, 0);
    AssignGenericNpcLoadContext(characters, kami.self);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (kami.self)
        HLib::SetupTimedTask(kami.self, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(KAMI_LOAD_TASK.name));
}
// @verified - p
void KAMIInitializeCallback(void *) {
    if (kami.request.state == 0) {
        KAMIDebugNullsub();
        SetRenderClockAndCompassTo(1);
        SetPlayerControlEnabled(0);
        DisablePluralControl();
        EV_FreeAuthorPak(1, "0027");
        EV_SetNewSceneIndex();
        EV_LoadCutsceneFromCurrentScene("0027A000");
        EV_LoadAudio("0027A000", 0);
        EV_spmBeginSampleExecute();
        kami.request.state = kami.request.nextState;
    }
    HLib::SetCurrentTaskTo(KAMIStateCallback);
}
// @verified - p
void KAMIStateCallback(void *) {
    switch (kami.request.state) {
    case 1:
        if (EV_ForceLoadNpc(KAMI_BARBER))
            kami.request.state = kami.request.nextState;
        return;
    case 2:
        if (--kami.request.timer <= 0)
            kami.request.state = kami.request.nextState;
        return;
    case 3:
        if (--kami.request.timer <= 0)
            kami.request.state = kami.request.nextState;
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            kami.request.state = static_cast<std::int16_t>(EV_GetCurrentQteStatus()) == -24576 ? kami.request.nextState : kami.request.failureState;
        return;
    case 4:
        SetPlayerControlEnabled(0);
        kami.request.command = 0;
        DispatchAuthor();
        kami.request.state = 17;
        kami.request.nextState = 5;
        return;
    case 5:
        KAMIBeginRazorRepeat();
        return;
    case 6:
        SetPlayerControlEnabled(0);
        kami.request.command = 1;
        DispatchAuthor();
        kami.request.state = 17;
        kami.request.nextState = 7;
        return;
    case 7:
        KAMIBeginRazorIntroduction();
        return;
    case 10: {
        DisablePluralControl();
        SetPlayerControlEnabled(0);
        DisableControllers(KAMI_PLAYER);
        EV_AT_SetPos(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{217.55f, 104.52f, 175.03999f});
        EV_AT_SetAngY(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0x4000, 0});
        mtwkSetExtraFlag(scenFindMotionwork(KAMI_PLAYER), 17, 0);
        mtwkRequestMotion(scenFindMotionwork(KAMI_PLAYER), KAMI_PLAYER_SEATED_MOTION, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(KAMI_PLAYER), 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(KAMI_PLAYER), 13, 0);
        const Vector4 reflector{1, .30000001f, .30000001f, .30000001f};
        SetReflectorBoardValues(FindCharacterByScenarioName(KAMI_PLAYER), reflector);
        EV_SetRefBoardParamsFor(KAMI_PLAYER, 1);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(KAMI_PLAYER), FacialNodeReplacement);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(KAMI_PLAYER), 4, 30, 1);
        DisableControllers(KAMI_BARBER);
        SetReflectorBoardValues(FindCharacterByScenarioName(KAMI_BARBER), reflector);
        EV_SetRefBoardParamsFor(KAMI_BARBER, 1);
        SetHighModelXYZ(FindCharacterByScenarioName(KAMI_BARBER), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(KAMI_BARBER), 0, 1, 0);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(KAMI_BARBER), 0, reinterpret_cast<std::byte *>(barberHand), 0);
        EV_AT_SetPos(KAMI_BARBER, EventTransformFlags::World | EventTransformFlags::AllAxes,
            Vector3{std::bit_cast<float>(0x435a6b85U), std::bit_cast<float>(0x42d10000U), 175.06999f});
        EV_AT_SetAngY(KAMI_BARBER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 18021, 0});
        mtwkSetExtraFlag(scenFindMotionwork(KAMI_BARBER), 17, 0);
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(KAMI_BARBER), FacialNodeReplacement);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(KAMI_BARBER), 1, 1, 1);
        SetFixoDataForMotionWork(FindCharacterByScenarioName(KAMI_RAZOR), FindCharacterByScenarioName(KAMI_BARBER), 14,
            Vector3{std::bit_cast<float>(0x3de76c8bU), std::bit_cast<float>(0x3cf5c28fU), -.033f}, std::array<std::int32_t, 3>{18204, 31493, 5825});
        mtwkRequestMotion(scenFindMotionwork(KAMI_BARBER), KAMI_RAZOR_APPROACH_MOTION, MotionRegion::LowerBody);
        SetMotionLayerLoopStartTime(scenFindMotionwork(KAMI_BARBER), 80, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(KAMI_BARBER), 2, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(KAMI_BARBER), 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(KAMI_BARBER), 13, 0);
        kami.request.timer = (GetMotionFrameCount(KAMI_RAZOR_APPROACH_MOTION) - 70) / 2;
        EVS_LoopCamera(200, {}, {});
        kami.request.state = 3;
        kami.request.nextState = 11;
        kami.request.qteContinuation = 15;
        kami.request.failureState = 16;
        SetShowQteFlagForButtonType(256, static_cast<std::int16_t>(static_cast<std::uint16_t>(kami.request.timer) + 80U), 5);
        return;
    }
    case 11:
        mtwkRequestMotion(scenFindMotionwork(KAMI_BARBER), KAMI_RAZOR_HOLD_MOTION, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(KAMI_BARBER), 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(KAMI_BARBER), 13, 0);
        SetMotionSpeed(scenFindMotionwork(KAMI_BARBER), 1, MotionRegion::LowerBody);
        kami.request.timer = 200;
        kami.request.state = 3;
        kami.request.nextState = 15;
        EVS_LoopCamera(210, {}, {});
        return;
    case 15:
    case 16: {
        const std::uint8_t failed = kami.request.state == 16;
        camCtrlSetFlags();
        ResetCharaFixo(FindCharacterByScenarioName(KAMI_RAZOR));
        kami.request.command = failed ? 4 : 5;
        SetPlayerControlEnabled(0);
        DispatchAuthor();
        kami.request.state = 17;
        kami.request.nextState = failed ? 20 : 18;
        EV_stepSetFlags(failed, EV_GetShenmueFlag(0x1f) != 0 ? 0x1f : 0x37);
        break;
    }
    case 17:
        if (kami.request.command == -1)
            kami.request.state = kami.request.nextState;
        break;
    case 18:
        KAMIRestoreSuccessfulGameplay();
        break;
    case 19:
        SetPlayerControlEnabled(1);
        FinishLoading();
        SetRenderClockAndCompassTo(0);
        SetPluralControlFor(FindCharacterByScenarioName(KAMI_PLAYER), 1);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KAMI"), 0);
        HLib::DestroyTask(kami.self);
        break;
    case 20:
        KAMIRestoreFailedGameplay();
        break;
    case 21:
        KAMIFinish();
        break;
    default:
        return;
    }
}
// @verified - p
void KAMIBeginRazorRepeat() {
    DisablePluralControl();
    EV_LoadAudio("0027A000", 1);
    EV_spmBeginSampleExecute();
    SetPlayerControlEnabled(0);
    kami.request.savedSkip = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetSkipAuthorEnabled));
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    kami.request.command = 2;
    DispatchAuthor();
    kami.request.state = 17;
    kami.request.nextState = 10;
}
// @verified - p
void KAMIBeginRazorIntroduction() {
    DisablePluralControl();
    EV_LoadAudio("0027A000", 1);
    EV_spmBeginSampleExecute();
    SetPlayerControlEnabled(0);
    kami.request.savedSkip = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetSkipAuthorEnabled));
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    kami.request.command = 3;
    DispatchAuthor();
    kami.request.state = 17;
    kami.request.nextState = 10;
}
// @verified - p
void KAMIRestoreSuccessfulGameplay() {
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    ResetFaceModels();
    EnablePlayerControllers();
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 141)) {
        EV_AT_SetPos(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{217.1999969482422f, 104.5f, 175.75f});
        EV_AT_SetAngY(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 40960, 0});
    } else {
        EV_AT_SetPos(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{224.5f, 104.5f, 174.89999f});
        EV_AT_SetAngY(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 49152, 0});
    }
    mtwkRequestMotion(scenFindMotionwork(KAMI_PLAYER), 0xf002, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(KAMI_PLAYER), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(KAMI_PLAYER), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(KAMI_PLAYER), 13, 0);
    LoadCutsceneAndSoundProgramForArea();
    EV_FreeAuthorPak(0, "0027");
    RetireCharacters();
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 141))
        doorLoadAuthorWithAudio({});
    kami.request.state = 19;
    RevealScene();
    HLib::SetupTaskDelay(30);
    EV_NPC_NewLoadSw(1);
    camCtrlSetState(CameraState::Init);
    camCtrlSetFovTransitionTo(true);
}
// @verified - p
void KAMIRestoreFailedGameplay() {
    EV_authSceneCtrl(kami.request.savedSkip != 0 ? AuthSceneCtrl::AllowSkipAuthor : AuthSceneCtrl::DisallowSkipAuthor);
    ResetFaceModels();
    mtwkRequestMotion(scenFindMotionwork(KAMI_PLAYER), 0xf002, MotionRegion::LowerBody);
    EV_AT_SetPos(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes,
        Vector3{std::bit_cast<float>(0x4364b333U), std::bit_cast<float>(0x42d10000U), 177.8f});
    EV_AT_SetAngY(KAMI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 10693, 0});
    mtwkSetExtraFlag(scenFindMotionwork(KAMI_PLAYER), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(KAMI_PLAYER), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(KAMI_PLAYER), 13, 0);
    EnablePlayerControllers();
    EV_FreeAuthorPak(0, "0027");
    RetireCharacters();
    LoadCutsceneAndSoundProgramForArea();
    EnablePlayerControllers();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KAMI"), 0);
    doorLoadAuthorWithAudio({});
    EV_NPC_NewLoadSw(1);
    KAMIFinish();
}
// @verified - p
void KAMIFinish() {
    RevealScene();
    ST_LoadTitleFade();
    SetPlayerControlEnabled(1);
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    SetPluralControlFor(FindCharacterByScenarioName(KAMI_PLAYER), 1);
    camCtrlSetState(CameraState::Init);
    camCtrlSetFovTransitionTo(true);
    HLib::DestroyTask(kami.self);
}
const ScriptAreaDefinition areaKAMIScriptDefinition{.areaId = AREA_KAMI, .field04 = 0, .init = AreaKAMIInit, .main = AreaKAMIMain};
} // namespace shenmue::s2::areas
