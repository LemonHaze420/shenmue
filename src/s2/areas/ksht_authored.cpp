#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
KshtAuthorController kshtAuthorController;
namespace {
struct KshtAuthorFamily {
    void (*producer)(KshtAuthorController *);
    void (*setup)();
    void (*exit)(KshtAuthorController *);
};
const KshtAuthorFamily families[]{{.producer = QueueKshtCloudAuthor, .setup = SetupKshtCloudAuthor, .exit = ExitKshtCloudAuthor},
    {.producer = QueueKshtDrawingAuthor, .setup = SetupKshtDrawingAuthor, .exit = ExitKshtDrawingAuthor},
    {.producer = QueueKshtConversationAuthor, .setup = SetupKshtConversationAuthor, .exit = ExitKshtConversationAuthor},
    {.producer = QueueKshtRabbitAuthor, .setup = SetupKshtRabbitAuthor, .exit = ExitKshtRabbitAuthor},
    {.producer = QueueKshtDepartureAuthor, .setup = SetupKshtDepartureAuthor, .exit = ExitKshtDepartureAuthor},
    {.producer = QueueKshtMorningAuthor, .setup = SetupKshtMorningAuthor, .exit = ExitKshtMorningAuthor},
    {.producer = QueueKshtRabbitDropAuthor, .setup = SetupKshtRabbitDropAuthor, .exit = ExitKshtRabbitDropAuthor}};
constexpr std::uint8_t familyIndices[]{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6};
} // namespace
void QueueKshtAuthor(std::int32_t *selection) {
    kshtAuthorController.task = HLib::EnqueueTaskWithoutParameter(KshtAuthorCallback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(shau)));
    kshtAuthorController.state = 1;
    kshtAuthorController.savedFarDistance = camCtrlGetFarDistance();
    kshtAuthorController.selection = *selection;
    kshtAuthorController.result = selection;
    LoadKshtCharacters(*selection == 7 ? kshtGuestList : kshtCompanionList, kshtAuthorController.task);
    families[familyIndices[kshtAuthorController.selection]].setup();
}
void KshtAuthorCallback(void *) {
    const KshtAuthorFamily &family = families[familyIndices[kshtAuthorController.selection]];
    if (kshtAuthorController.state > 0 && kshtAuthorController.state <= 2)
        family.producer(&kshtAuthorController);
    else if (kshtAuthorController.state == 4)
        family.exit(&kshtAuthorController);
    else if (kshtAuthorController.state == 5) {
        *kshtAuthorController.result = -1;
        HLib::DestroyTask(kshtAuthorController.task);
        SetScenarioCharacterTransparency(KSHT_RYO, 1);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit26, reinterpret_cast<void *>(std::uintptr_t{1}));
        EV_LoadCutsceneFromCurrentScene("KSH1");
        EV_LoadAudio("KSH1_000", 0);
        EV_spmBeginSampleExecute();
    }
}
void LoadKshtCharacters(const shen_name *characters, HLib::Task *parent) {
    AssignGenericNpcLoadContext(characters, parent);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (parent)
        HLib::SetupTimedTask(parent, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(load)));
}
void FreeKshtCharacters(const shen_name *characters) {
    for (const shen_name *name = characters; name->name != KSHT_INVALID.name; ++name)
        if (DoesScenarioCharacterExist(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
}
void PrepareKshtActor(shen_name actor) {
    EV_EnableCharacterDisplay(actor);
    EV_SetCharaControllerFlags(actor, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 6, 2);
    EV_SetCharacterAlphaEnabled(actor, false);
    EV_EnsureCharacterDraw(actor);
}
void RestoreKshtActor(shen_name actor) {
    EV_EnableCharacterDisplay(actor);
    EV_SetCharaControllerFlags(actor, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(actor, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(actor, 3, NpcCharacterControllerState::Enabled);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 6, 0);
    EV_SetCharacterAlphaEnabled(actor, true);
}
void PrepareKshtProp(shen_name actor) {
    EV_EnableCharacterDisplay(actor);
    EV_SetCharaControllerFlags(actor, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, 3, NpcCharacterControllerState::Disabled);
    EV_EnsureCharacterDraw(actor);
}
void RestoreKshtProp(shen_name actor) {
    EV_SetCharaControllerFlags(actor, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(actor, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(actor, 3, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(actor, true);
    EV_ToggleCharacterFlag(actor);
    EV_DisableCharacterDisplay(actor);
}
void ResetKshtMotion(shen_name actor) {
    mtwkSetExtraFlag(scenFindMotionwork(actor), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(actor), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(actor), 13, 0);
}
void ResetKshtCamera() {
    using enum CameraState;
    camCtrlSetFovTransitionTo(true);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetState(Init);
}
} // namespace shenmue::s2::areas
