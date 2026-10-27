#include "s2/areas/area_chaw.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>
namespace shenmue::s2 {
AreaEntityPlacement chawPlacements[22];
DateTime chawTargetDate;
shen_name chawAuthor, chawMapProp, chawAmbushCharacters[2]{shen_name{}, CHAW_INVALID};
std::uint32_t chawPrize, chawSmoke;

void AreaCHAWInit() {
    chawPreviousHand = {};
    chawAuthor = {};
    chawMapProp = {};
    chawPrize = 0;
    chawLectureErrors = 0;
    chawSmoke = 0;
    chawSerifDelay = 0;
    chawTargetDate = {};
    std::memcpy(chawPlacements, AREA_ENTITY_PLACEMENTS, sizeof(chawPlacements));
}

void AreaCHAWMain(std::uintptr_t argument) {
    std::int32_t entry = static_cast<std::int32_t>(argument), alternate = 0;
    if (entry >= 22) {
        alternate = 1;
        entry -= 22;
    }
    SetPlayerControlEnabled(0);
    mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
    if (g_currentMap.name == CHAW_AREA.name) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
            reinterpret_cast<std::uintptr_t>("CHAWAN"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    HLib::Task *task;
    if (entry == 21) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("LECTURE"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        task = HLib::EnqueueTaskWithParameter(ChawPreLectureCallback, 4, 11, sizeof(ChawEventWork), CHAW_TASK);
        Queue_EVT_UTL_LoadNPCChara_3(CHAW_LECTURE_CHARACTERS, task);
    } else
        task = HLib::EnqueueTaskWithParameter(ChawStartCallback, 4, 11, sizeof(ChawEventWork), CHAW_TASK);
    ChawEventWork *work = static_cast<ChawEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->entry = entry;
    work->alternate = alternate;
    ChawInitialize(work);
}
const ScriptAreaDefinition areaCHAWScriptDefinition{CHAW_AREA, 0, AreaCHAWInit, AreaCHAWMain};

void ChawInitialize(ChawEventWork *work) {
    EV_EventSetUpHelpIcon_0(0x43, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("CHAWAN"), 0);
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    EV_FreeAuthorPak(1, "0038");
    nullsub();
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(CHAW_RYO, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(CHAW_RYO, 3, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(CHAW_RYO, 1);
    mtwkSetRequestedRootMode(scenFindMotionwork(CHAW_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
    if (EV_GetShenmueFlag(0x247))
        EV_stepSetFlags(1, 0x247);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, CHAW_BASE);
    AreaEntityPlacement &placement = chawPlacements[work->entry];
    if (g_currentMap.name == CHAW_AREA.name)
        placement.position = {};
    EV_AT_SetPos(CHAW_BASE, EventTransformFlags::AllAxes, placement.position);
    EV_AT_SetAngY(CHAW_BASE, EventTransformFlags::AllAxes, {0, placement.yaw, 0});
    AT_SetLocalBaseChara(CHAW_FACE, CHAW_BASE);
    AT_SetLocalBaseChara(CHAW_RYO, CHAW_BASE);
    AT_SetLocalBaseChara(CHAW_HAND, CHAW_BASE);
    EV_AT_SetPos(CHAW_RYO, EventTransformFlags::AllAxes, {-0.45f, 0, 1.37f});
    EV_AT_SetAngY(CHAW_RYO, EventTransformFlags::AllAxes, {});
    EV_AT_SetPos(CHAW_HAND, EventTransformFlags::AllAxes, {0.17f, 0.86f, 0.53f});
    EV_AT_SetAngY(CHAW_HAND, EventTransformFlags::AllAxes, {49152, 40960, 0});
    const Vector3 scale{0.1f, 0.1f, 0.1f};
    mtwkSetMotionworkScale3(CHAW_HAND, &scale);
    EV_DisableCharacterDisplay(CHAW_HAND);
    EV_AT_SetPos(CHAW_FACE, EventTransformFlags::AllAxes, {0, 0.75f, 0.2f});
    EV_AT_SetAngY(CHAW_FACE, EventTransformFlags::AllAxes, {});
    if (work->entry != 21)
        ReplaceScenarioImageModule(CHAW_FACE, CHAW_MODELS[work->entry].faceModel, 0);
    SetScenarioCharacterTransparency(CHAW_FACE, 0.58f);
    EV_DisableCharacterDisplay(CHAW_FACE);
    for (std::int32_t cup = 0; cup < 4; ++cup) {
        const shen_name name{CHAW_CUP_BASE.name + (static_cast<std::uint32_t>(cup) << 24)};
        work->cups[cup] = name;
        CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>(CHAW_MODELS[work->entry].cupModel), name);
        AT_SetLocalBaseChara(name, CHAW_BASE);
        EV_DisableCharacterDisplay(name);
        EV_ToggleCharacterFlag(name);
        EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        const std::int16_t flag = static_cast<std::int16_t>((work->entry >= 16 ? work->entry - 16 : work->entry) + 16);
        work->cupRows[cup] = work->entry == 21 ? 1 : static_cast<std::int32_t>((EV_GetPlayerFlag(PlayerFlagReadMode::Stat, flag) >> (2 * cup)) & 3);
        ChawPlaceCup(work, cup);
    }
    work->helpFlags = 0;
    work->pendingButtons = 0;
    work->lectureWord = 0;
    work->handMovement = 0;
    work->selectedCup = 2;
    work->row = work->cupRows[2];
    work->carriedCup = 2;
    work->lecturePhase = 0;
    work->started = 0;
    work->state = 0;
    work->advanceTime = 0;
    work->lectureLoaded = 0;
    work->cupMoved = 0;
    work->error = 0;
    work->reminders = 0;
    work->ticks = 0;
    work->wrongSign = 0;
    work->progression = 0;
    work->enabled = 1;
    work->cameraOffset = 100 * placement.field18;
    const std::int32_t cell = 4 * work->row + 2;
    work->handTarget = ChawGetHandTarget(cell);
    work->handYaw = ChawGetHandYaw(cell);
    chawSerifDelay = 0;
    chawPrize = 0;
    EV_SetNewSceneIndex();
    work->savedAudioScene = GetSelectedSoundSceneIndex();
    const char *bgm = GetCurrentBgm();
    for (std::int32_t i = 0; i < 15 || bgm[i]; ++i)
        work->savedBgm[i] = bgm[i];
    work->savedBgm[15] = 0;
    const char *sbt = shenGetCurrentSbt();
    for (std::int32_t i = 0; i < 15 || sbt[i]; ++i)
        work->savedSbt[i] = sbt[i];
    work->savedSbt[15] = 0;
    EV_LoadCutsceneFromCurrentScene(CHAW_AUDIO_BANKS[g_actNumber]);
}

void ChawRestoreLighting(ChawEventWork *work) {
    const shen_name light = CHAW_MODELS[work->entry].lighting;
    if (light.name) {
        EV_lightLoad(light);
        std::int32_t index = 0;
        while (HasMapModelReader(index))
            ++index;
        AssignLightProgram(static_cast<std::uint32_t>(index - 1));
    } else {
        DateTime date;
        GetCurrentDateTime(date);
        if (date.hour >= 19 || g_currentMap.name == SHEN_NAME(AKS0)) {
            EV_lightLoad(SHEN_NAME(0000));
            PushGlobalLightingProgram();
        }
    }
    EV_UpdateWeatherForMinute(1, 1, 1, 0);
    EV_UpdateWeatherForMinute(2, 1, 1, 0);
}

void ChawLoadAuthorCups(ChawEventWork *work) {
    EV_AT_SetPos(CHAW_BASE, EventTransformFlags::AllAxes, chawPlacements[work->entry].position);
    EV_DisableCharacterDisplay(CHAW_FACE);
    for (std::int32_t cup = 0; cup < 4; ++cup) {
        const shen_name original{chawPlacements[work->entry].character.name + (static_cast<std::uint32_t>(cup) << 24)};
        if (DoesScenarioCharacterExist(original)) {
            Vector3 position;
            EV_GetCharaPosition(work->cups[cup], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            EV_AT_SetPos(original, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_DisableCharacterDisplay(work->cups[cup]);
            EV_EnableCharacterDisplay(original);
        }
    }
}

void ChawCreateAuthorCups(const char *model) {
    for (shen_name cup : CHAW_AUTHOR_CUPS)
        CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>(model), cup);
    for (shen_name cup : CHAW_AUTHOR_CUPS)
        EV_EnableCharacterDisplay(cup);
    for (shen_name cup : CHAW_AUTHOR_CUPS) {
        EV_SetCharaControllerFlags(cup, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(cup, 3, NpcCharacterControllerState::Disabled);
    }
}

void ChawSetupAuthorCharacter(shen_name name) {
    const Vector4 reflector{1, 0.3f, 0.3f, 0.3f};
    EV_EnableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(name, false);
    SetScenarioReflectorBoardValues(name, &reflector);
    EV_SetRefBoardParamsFor(name, 1);
    hmdlSetFlagsFor(FindCharacterByScenarioName(name), 0, 8, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(name), 1, 8, 16);
    AT_SetLocalBaseChara(name, CHAW_INVALID);
}
void ChawFadeOutCallback(void *) {
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
void ChawFadeAudioOutCallback(void *) {
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    HLib::DestroyCurrentTask();
}
void ChawQueueFadeAudioOut(std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(ChawFadeAudioOutCallback, 6, 11, static_cast<std::int32_t>(CHAW_EVENT_TASK.name));
    if (delay)
        HLib::SetupTimedTask(task, delay);
}
void ChawForwardTimeCallback(void *data) {
    DateTime date;
    GetCurrentDateTime(date);
    const std::int32_t minutes = *static_cast<std::int32_t *>(data), hours = minutes / 60;
    date.hour += hours;
    date.minute += minutes - 60 * hours;
    SetCurrentDateTimeTo(date);
    HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2
