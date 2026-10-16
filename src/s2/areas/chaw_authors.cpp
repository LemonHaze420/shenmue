#include "s2/areas/area_chaw.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
namespace {
void LoadPack(const char *name, FilepathGenMode directory) {
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(directory)), reinterpret_cast<std::uintptr_t>(name));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
void RestoreCharacter(shen_name name) {
    EV_EnableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(name, true);
}
void FreeCharacters(const shen_name *list) {
    for (; list->name != CHAW_INVALID.name; ++list)
        if (DoesScenarioCharacterExist(*list)) {
            EV_npcLockUnlock(*list);
            EV_npcFree(*list);
        }
}
void ReleasePack(const char *name) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
}
} // namespace

void ChawSuccessAuthorCallback(void *data) {
    HLib::Task *root = HLib::GetCurrentTask();
    EV_DisableCharacterDisplay(CHAW_HAND);
    EV_ToggleCharacterFlag(CHAW_HAND);
    EV_ResetFreeTalkFlags();
    LoadPack("TOSHOKEN", FilepathGenMode::SceneDirectory);
    HLib::SetCurrentTaskTo(ChawPlaySuccessAuthorCallback);
    Queue_EVT_UTL_LoadNPCChara_3(CHAW_TOSHOKEN_CHARACTERS, root);
    ChawRestoreLighting(static_cast<ChawEventWork *>(data));
}

void ChawPlaySuccessAuthorCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    if (DoesScenarioCharacterExist(SHEN_NAME(cw20)))
        ChawCreateAuthorCups("WSC0AC5G");
    else {
        ChawCreateAuthorCups("CWJN2G2G");
        Vector3 zero{};
        for (shen_name cup : work->cups)
            EV_scenApplyPosOrRotToPartOf(cup, 154, &zero, nullptr, EventCharacterPartTransformMode::Set);
    }
    for (shen_name prop : CHAW_TOSHOKEN_PROPS)
        CreateEventCharacter(CharacterCreationHighModel, nullptr, prop);
    for (shen_name prop : CHAW_TOSHOKEN_PROPS)
        EV_DisableCharacterDisplay(prop);
    for (shen_name prop : CHAW_TOSHOKEN_PROPS)
        EV_ToggleCharacterFlag(prop);
    ChawSetupAuthorCharacter(CHAW_RYO);
    ChawSetupAuthorCharacter(SHEN_NAME(CHO_));
    ChawLoadAuthorCups(work);
    SetDoorSubjectEnabled(SHEN_NAME(R129), 0);
    EV_LoadAudio("0038E000", 2);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(SCES)));
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(ChawFinishSuccessAuthorCallback);
}
void ChawFinishSuccessAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetDoorSubjectEnabled(SHEN_NAME(R129), 1);
    for (shen_name cup : CHAW_AUTHOR_CUPS)
        SetCharaKill(cup, 1);
    for (shen_name prop : CHAW_TOSHOKEN_PROPS)
        SetCharaKill(prop, 1);
    RestoreCharacter(CHAW_RYO);
    RestoreCharacter(SHEN_NAME(CHO_));
    ReleasePack("TOSHOKEN");
    camCtrlSetState(CameraState::Init);
    mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
    doorLoadAuthorSilently(shen_name{});
    EV_AT_SetPos(CHAW_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {213.85f, 104.5f, 209.2f});
    EV_AT_SetAngY(CHAW_RYO, EventTransformFlags::AllAxes, {0, 64322, 0});
    camCtrlSetFovTransitionTo(true);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(ChawFinishCallback);
}

void ChawGotoYamiutiCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    HLib::Task *root = HLib::GetCurrentTask();
    EV_DisableCharacterDisplay(CHAW_HAND);
    EV_ToggleCharacterFlag(CHAW_HAND);
    ChawLoadAuthorCups(work);
    bool replace = false;
    switch (work->entry) {
    case 0:
        chawAuthor = SHEN_NAME(ATH6);
        chawAmbushCharacters[0] = SHEN_NAME(KIC_);
        chawMapProp = SHEN_NAME(a474);
        chawPrize = 37;
        EV_stepSetFlags(0, 0xe6);
        break;
    case 3: {
        chawAuthor = SHEN_NAME(ATH7);
        chawAmbushCharacters[0] = SHEN_NAME(CLY_);
        chawMapProp = SHEN_NAME(aa83);
        DateTime date;
        GetCurrentDateTime(date);
        replace = date.hour < 19;
        chawPrize = replace ? 40 : 38;
        EV_stepSetFlags(replace ? 0 : 1, 0x302);
        break;
    }
    case 9:
        chawAuthor = SHEN_NAME(ATH0);
        chawAmbushCharacters[0] = SHEN_NAME(TKM_);
        chawMapProp = SHEN_NAME(a474);
        chawPrize = 37;
        EV_stepSetFlags(0, 0x2b3);
        break;
    case 11:
        chawAuthor = SHEN_NAME(ATH8);
        chawAmbushCharacters[0] = SHEN_NAME(HUR_);
        chawMapProp = SHEN_NAME(a474);
        chawPrize = 37;
        EV_stepSetFlags(0, 0xe6);
        break;
    default:
        break;
    }
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    LoadPack("0039", FilepathGenMode::PackDirectory);
    if (replace)
        ReplaceScenarioImageModule(chawMapProp, "TOSM29JI", 0);
    HLib::SetCurrentTaskTo(ChawPlayYamiutiCallback);
    Queue_EVT_UTL_LoadNPCChara_3(chawAmbushCharacters, root);
}
void ChawPlayYamiutiCallback(void *data) {
    ChawSetupAuthorCharacter(CHAW_RYO);
    EV_SetCharaControllerFlags(chawMapProp, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(chawMapProp, 3, NpcCharacterControllerState::Disabled);
    EV_FreeAuthorPak(1, "0039");
    EV_LoadCutsceneFromCurrentScene("0039A000");
    ChawRestoreLighting(static_cast<ChawEventWork *>(data));
    EV_authSceneCtrl(AuthSceneCtrl::ApplyYangleOffset);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(chawAuthor.name));
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(ChawFinishYamiutiCallback);
}
void ChawFinishYamiutiCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreCharacter(CHAW_RYO);
        FreeCharacters(chawAmbushCharacters);
        camCtrlSetState(CameraState::Init);
        mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
        HLib::SetCurrentTaskTo(ChawFinishCallback);
        EV_FreeAuthorPak(0, "0039");
        ReleasePack("0039");
    }
}
void ChawGotoDokuhebiCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    ChawLoadAuthorCups(work);
    ChawCleanup(work, false);
    for (std::int32_t cup = 0; cup < 4; ++cup) {
        const shen_name original{chawPlacements[work->entry].character.name + (static_cast<std::uint32_t>(cup) << 24)};
        if (DoesScenarioCharacterExist(original))
            EV_DisableCharacterDisplay(original);
    }
    ChawRestoreLighting(work);
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    DateTime date;
    GetCurrentDateTime(date);
    npcAdvanceTime(date.hour, date.minute);
    LoadPack("AHQB", FilepathGenMode::PackDirectory);
    ExecuteCurrAreaMainFunc("AHQB.EV2", 0);
}
void ChawGotoSeiyaCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    ChawLoadAuthorCups(work);
    ChawCleanup(work, false);
    ChawRestoreLighting(work);
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    DateTime date;
    GetCurrentDateTime(date);
    npcAdvanceTime(date.hour, date.minute);
    shenLoadEventFrom("WRSQ.EV2", EventFunctionType::Event2);
    ExecuteCurrAreaMainFunc("WRSQ.EV2", 0);
}
void ChawGotoKunCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    EV_DisableCharacterDisplay(CHAW_HAND);
    EV_ToggleCharacterFlag(CHAW_HAND);
    ChawLoadAuthorCups(work);
    ChawRestoreLighting(work);
    EV_LoadCutsceneFromCurrentScene("AKS1");
    EV_LoadAudio("AKS1_000", 0);
    EV_spmBeginSampleExecute();
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    LoadPack("KUN_HELP", FilepathGenMode::SceneDirectory);
    HLib::SetCurrentTaskTo(ChawPlayKunCallback);
    Queue_EVT_UTL_LoadNPCChara_3(CHAW_KUN_CHARACTERS, HLib::GetCurrentTask());
    EV_stepSetFlags(0, 0x43d);
}
void ChawPlayKunCallback(void *data) {
    ChawSetupAuthorCharacter(CHAW_RYO);
    EV_FreeAuthorPak(1, "0194");
    ChawRestoreLighting(static_cast<ChawEventWork *>(data));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(AUTH)));
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(ChawFinishKunCallback);
}
void ChawFinishKunCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreCharacter(CHAW_RYO);
        FreeCharacters(CHAW_KUN_CHARACTERS);
        camCtrlSetState(CameraState::Init);
        mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
        HLib::SetCurrentTaskTo(ChawFinishCallback);
        EV_FreeAuthorPak(0, "0194");
        ReleasePack("KUN_HELP");
    }
}
void ChawGotoHyakusiCallback(void *data) {
    ChawLoadAuthorCups(static_cast<ChawEventWork *>(data));
    EV_DisableCharacterDisplay(CHAW_HAND);
    EV_ToggleCharacterFlag(CHAW_HAND);
    EV_stepSetFlags(0, 0x2c1);
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    LoadPack("HYAKUSI", FilepathGenMode::SceneDirectory);
    HLib::SetCurrentTaskTo(ChawPlayHyakusiCallback);
    Queue_EVT_UTL_LoadNPCChara_3(CHAW_HYAKUSI_CHARACTERS, HLib::GetCurrentTask());
}
void ChawPlayHyakusiCallback(void *data) {
    ChawSetupAuthorCharacter(CHAW_RYO);
    EV_FreeAuthorPak(1, "0228");
    ChawRestoreLighting(static_cast<ChawEventWork *>(data));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(HYAK)));
    HLib::SetCurrentTaskTo(ChawFinishHyakusiCallback);
}
void ChawFinishHyakusiCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreCharacter(CHAW_RYO);
        FreeCharacters(CHAW_HYAKUSI_CHARACTERS);
        camCtrlSetState(CameraState::Init);
        mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
        EV_AT_SetPos(CHAW_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {263.3f, 100.09f, 195.6f});
        EV_AT_SetAngY(CHAW_RYO, EventTransformFlags::AllAxes, {});
        HLib::SetCurrentTaskTo(ChawFinishCallback);
        EV_FreeAuthorPak(0, "0228");
        ReleasePack("HYAKUSI");
    }
}
} // namespace shenmue::s2
