#include "s2/areas/area_0017.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <array>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name characters0[]{SHEN_NAME(JOP_), SHEN_NAME(QLN_), AREA_AUTHOR_INVALID};
constexpr shen_name characters1[]{SHEN_NAME(DEY_), SHEN_NAME(QRN_), AREA_AUTHOR_INVALID};
constexpr shen_name characters2[]{SHEN_NAME(SON_), SHEN_NAME(TKR_), AREA_AUTHOR_INVALID};
constexpr const shen_name *characters[]{characters0, characters1, characters2};
constexpr shen_name cleanupCharacters[]{characters0[0], characters1[0], characters1[1], characters2[0], characters0[1], characters2[1], AREA_AUTHOR_INVALID};
constexpr std::array<shen_name, 10> props{SHEN_NAME(a016) | 0x80300U, SHEN_NAME(a878) | 0x100U, SHEN_NAME(a796), SHEN_NAME(a075) | 0x300U,
    SHEN_NAME(a076) | 0x300U, SHEN_NAME(a438) | 0x200U, SHEN_NAME(a640), SHEN_NAME(a202) | 0x40400U, SHEN_NAME(a444) | 0x200U, SHEN_NAME(a246) | 0x400U};
ConversationBuffer *voice{};
PakAfsPartition *archive{};
void MainCallback(void *);
void Loop0(void *);
void Loop1(void *);
void Loop2(void *);
void WaitFade1(void *);
void WaitFade2(void *);
// @verified - p
void Init() {
    voice = nullptr;
    archive = nullptr;
}
// @verified - p
void ConfigureArchive(std::int32_t selection) {
    std::uintptr_t index;
    if (selection == 1) {
        for (std::int32_t i = 0; i < 4; ++i)
            EV_SetObjectFlagByIndex(i, 0);
        index = 2;
    } else if (selection == 2) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::DestroyAfs, reinterpret_cast<std::uintptr_t>(archive), 2);
        index = 0;
    } else
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::LoadAfs, reinterpret_cast<std::uintptr_t>(archive), index);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
void PrepareCharacter(shen_name name, bool motion) {
    EV_EnableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    if (motion)
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
    EV_SetCharacterAlphaEnabled(name, false);
}
// @verified - p
void SetupCharacters(std::int16_t selection) {
    PrepareCharacter(AREA_AUTHOR_RYO, true);
    for (shen_name name : props)
        PrepareCharacter(name, false);
    for (const shen_name *name = characters[selection]; name->name != AREA_AUTHOR_INVALID.name; ++name)
        PrepareCharacter(*name, true);
}
void RestoreCharacter(shen_name name, bool motion) {
    EV_EnableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    if (motion)
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
    EV_SetCharacterAlphaEnabled(name, true);
}
// @verified - p
void RestoreCharacters(AreaImmediateAuthorTaskParameter *parent) {
    EnableCharDisplayAndSetMotionworkData(AREA_AUTHOR_RYO);
    for (shen_name name : props)
        RestoreCharacter(name, false);
    for (std::int32_t index = 0; index < parent->selection; ++index)
        for (const shen_name *name = characters[index]; name->name != AREA_AUTHOR_INVALID.name; ++name)
            RestoreCharacter(*name, true);
}
// @verified - p
void Cleanup(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *name = cleanupCharacters; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_stepSetFlags(0, 17);
    EV_FreeAuthorPak(0, "0017");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    scenSetTaskFlags(8);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(ARBT), 0);
}
// @verified - p
void QueueLoop(AreaImmediateAuthorTaskParameter *parent) {
    ConfigureArchive(parent->selection);
    constexpr HLib::TaskCallback callbacks[]{Loop0, Loop1, Loop2};
    HLib::Task *task = HLib::EnqueueTaskWithParameter(callbacks[parent->selection], 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    *static_cast<AreaImmediateAuthorTaskParameter **>(HLib::GetTaskParameter(task)) = parent;
    if (parent->selection) {
        EV_SetWeatherInstruction(12, 1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    } else {
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(SHEN_NAME(l001));
        PushGlobalLightingProgram();
    }
    SetupCharacters(parent->selection);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    EV_spmBeginSampleExecute();
    parent->state = 2;
}
void BeginExitFade(bool command) {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    hsFadeVoice(voice, 30);
    SetMotionEnvironmentEnabled(0);
    if (command)
        ExecuteAicaCommand(0x1100a0, 4, 0);
    spmFadeOut();
    HLib::SetCurrentTaskTo(WaitFade2);
}
// @verified - p
void Loop0(void *data) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (GetAuthorFlags())
        BeginExitFade(true);
    else {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        auto *parent = *static_cast<AreaImmediateAuthorTaskParameter **>(data);
        HLib::SetupTimedTask(parent->self, 30);
        parent->state = 4;
        parent->selection = 1;
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void Loop1(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1000)
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (GetAuthorFlags())
        BeginExitFade(true);
    else {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(WaitFade1);
    }
}
// @verified - p
void Loop2(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        BeginExitFade(false);
}
// @verified - p
void WaitFade1(void *data) {
    if (!AreSoundBankFadesComplete())
        return;
    auto *parent = *static_cast<AreaImmediateAuthorTaskParameter **>(data);
    parent->state = 4;
    parent->selection = 2;
    HLib::DestroyCurrentTask();
}
// @verified - p
void WaitFade2(void *data) {
    if (!AreSoundBankFadesComplete())
        return;
    auto *parent = *static_cast<AreaImmediateAuthorTaskParameter **>(data);
    parent->state = 3;
    HLib::SetupTimedTask(parent->self, 30);
    RestoreCharacters(parent);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    HLib::DestroyCurrentTask();
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaImmediateAuthorTaskParameter *>(data);
    switch (parent->state) {
    case 1:
        QueueLoop(parent);
        break;
    case 3:
        Cleanup(parent);
        break;
    case 4:
        SetAreaNpcLowQualityHands(false);
        Queue_EVT_UTL_LoadNPCChara_3(characters[parent->selection], parent->self);
        parent->state = 5;
        break;
    case 5:
        parent->state = 1;
        break;
    default:
        break;
    }
}
// @verified - p
void Main(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    parent->selection = static_cast<std::int16_t>(static_cast<std::uint32_t>(g_entryPoint) > 2 ? 0 : g_entryPoint);
    archive = reinterpret_cast<PakAfsPartition *>(EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Mount,
        reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>("0017_MAP.afs")));
    voice = shenPlayAudioWithFlags("joy", 1);
    SetAreaNpcLowQualityHands(false);
    Queue_EVT_UTL_LoadNPCChara_3(characters0, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0017");
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
}
} // namespace
const ScriptAreaDefinition area0017ScriptDefinition{.areaId = SHEN_NAME(0017), .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2::areas
