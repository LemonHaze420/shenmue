#include "s2/areas/area_omak.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/area_jump.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/omak_collection_ui.hpp"
#include "s2/player.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/story_state.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <cstring>

namespace shenmue::s2::areas {
OmakeEntry omakeGameList[13], omakeMinigameList[13], omakeFightList[13];
OmakeSelection omakeSelection;
OmakeCategory omakeCategories[4]{{.icon = 17, .reserved04 = 0, .entries = omakeGameList}, {.icon = 16, .reserved04 = 0, .entries = omakeMinigameList},
    {.icon = 2, .reserved04 = 0, .entries = omakeFightList}, {}};
std::int32_t omakeSelectedCategory = -1, omakeSelectedEntry = -1, omakeBackRequested, omakeUnlockOverride;
std::int32_t omakeDrawEnabled, omakeVideoCount;
float omakeSpriteAnimation;

void InitializeOmake() {
    std::memset(omakeGameList, 0, sizeof(omakeGameList));
    std::memset(omakeMinigameList, 0, sizeof(omakeMinigameList));
    std::memset(omakeFightList, 0, sizeof(omakeFightList));
    omakeSpriteAnimation = 0;
    omakeDrawEnabled = 0;
    omakeVideoCount = 0;
}
void RunOmake(std::uintptr_t) {
    shenUpdateFlagEvaluation(1);
    BootOmake();
    SetD3TRenderSettings(1);
    ShowElementByIndex(UIElement::MainMenu);
    SelectMainMenuPage(4);
}
void InitializeOmakeList(OmakeEntry *destination, const OmakeEntry *source) {
    const std::uint32_t flags = static_cast<std::uint32_t>(GetCurrentGameFlags());
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < 13; ++index) {
        if (source[index].name && (flags & source[index].requiredFlags) == source[index].requiredFlags)
            destination[count++] = source[index];
    }
    for (std::int32_t index = 0; index < 13; ++index) {
        if (source[index].name && (flags & source[index].requiredFlags) != source[index].requiredFlags)
            destination[count++] = source[index];
    }
    for (std::int32_t index = 0; index < 13; ++index)
        if (!source[index].name)
            destination[count++] = source[index];
}
void OmakeBootNullsub() {
}
void GiveDefaultPlayerFlags() {
    g_storyStep = 100000;
    DateTime date{.year = 83, .month = 2, .day = 23, .week = 1, .hour = 10, .minute = 0, .second = 0};
    SetCurrentDateTimeTo(date);
    SetCompletedGameCount(static_cast<std::uint32_t>(omakeVideoCount) + 1U);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1001, 1);
    for (std::int16_t flag = 1011; flag <= 1017; ++flag)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, flag, 1);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 123, 0);
    SetPlayerStat(PlayerStatsId::Cash, 500);
    SetPlayerStat(PlayerStatsId::SegaCoins, 0);
}
void BootOmake() {
    SetOmakeCollectionSuppressed(0);
    EV_debug_log("OMAK\\eOMAK_et_omak.c", 461, "BootUp oma'K' Menu\n");
    HLib::Task *main = HLib::EnqueueTaskWithParameter(OmakeMainCallback, 5, 0x0b, sizeof(OmakeWork), OMAKE_EVENT);
    OmakeWork *work = static_cast<OmakeWork *>(HLib::GetTaskParameter(main));
    HLib::Task *draw = HLib::EnqueueTaskWithParameter(OmakeSpriteDrawCallback, 15, 0x0b, sizeof(OmakeWork *), OMAKE_EVENT);
    *static_cast<OmakeWork **>(HLib::GetTaskParameter(draw)) = work;
    work->currentTask = HLib::CurrentTask;
    work->entry = g_entryPoint;
    work->mainTask = main;
    work->drawTask = draw;
    SetPlayerControlEnabled(0);
    EV_DisableCharacterDisplay(OMAKE_PLAYER);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    EV_SetObjectFlagByIndex(0, 0);
    SetWeatherUpdateEnabledTo(0);
    SetIsSkySetup(0);
    shenSetTimeSimulationTo(0);
    SetRenderClockAndCompassTo(1);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit22, reinterpret_cast<void *>(1));
    ReinitializeLights(1);
    omakeDrawEnabled = 1;
    omakeSpriteAnimation = 0;
    OmakeBootNullsub();
    SelectPostFxBank(1, 0);
    SetPostFxFilterMode(1);
    DispatchPostFxFilterRender(0);
    work->delay = -1;
    work->state = 0;
    work->debugSprite = -1;
    for (std::int32_t index = 0; index < 512; ++index)
        work->sprites[index].name = shen_name{};
    for (std::int32_t index = 0; index < 128; ++index)
        work->textTextures[index].name = shen_name{};
    work->selection = &omakeSelection;
    if (work->entry)
        omakeVideoCount = static_cast<std::int32_t>(GetCompletedGameCount()) - 1;
    else {
        work->selection->category = 0;
        for (std::int32_t index = 0; index < 3; ++index) {
            work->selection->selected[index] = 0;
            work->selection->scroll[index] = 0;
        }
        for (std::int32_t index = 0; index < 16; ++index) {
            vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(index | 0x40)}, 0, 1);
            vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(index | 0x80)}, 0, 1);
        }
        omakeVideoCount = static_cast<std::int32_t>(GetCompletedGameCount());
    }
    if (static_cast<std::int32_t>(static_cast<std::uint32_t>(omakeVideoCount) + 1) <= omakeVideoCount)
        --omakeVideoCount;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1018, omakeVideoCount != 0);
    GiveDefaultPlayerFlags();
    work->animationFrames = 5;
    work->animationRemaining = 0;
    work->animationDirection = 0;
    work->refreshDescription = 1;
    InitializeOmakeList(omakeGameList, EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1018) ? OMAKE_VIDEO_ENTRIES : OMAKE_ARCADE_ENTRIES);
    InitializeOmakeList(omakeMinigameList, OMAKE_MINIGAME_ENTRIES);
    InitializeOmakeList(omakeFightList, OMAKE_FIGHT_ENTRIES);
    HLib::SetupTaskDelay(0);
}
void QueueOmakeSceneChange(OmakeWork *, const OmakeCommand *command) {
    GiveDefaultPlayerFlags();
    if (command->area.name != 0x20202020)
        EV_scenQueueLoad(4, command->area, command->entry);
}
void QueueOmakeVideo(OmakeWork *, const OmakeCommand *command) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1018, 0);
    ChangeSystemModule(SystemModule::Movie, command->entry);
}
void OmakeShowMainMenu(OmakeWork *, const OmakeCommand *) {
    ShowElementByIndex(UIElement::MainMenu);
    SelectMainMenuPage(5);
}
void OmakeModuleCallCallback(void *data) {
    OmakeModuleCallWork *work = static_cast<OmakeModuleCallWork *>(data);
    const std::int32_t frames = work->frames--;
    if (frames > 0)
        return;
    switch (work->command->entry) {
    case 1:
        SetArcadeEmulatorState(EmulatorState::SetEntryID, -1);
        SetArcadeEmulatorState(EmulatorState::StartOutRun, 1);
        break;
    case 2:
        SetArcadeEmulatorState(EmulatorState::SetEntryID, -1);
        SetArcadeEmulatorState(EmulatorState::StartAfterBurner, 1);
        break;
    case 3:
        SetArcadeEmulatorState(EmulatorState::SetEntryID, -1);
        SetArcadeEmulatorState(EmulatorState::StartHangOn, 1);
        break;
    case 4:
        SetArcadeEmulatorState(EmulatorState::SetEntryID, -1);
        SetArcadeEmulatorState(EmulatorState::StartHarrier, 1);
        break;
    case 5:
        g_actNumber = 1;
        g_entryPoint = 0;
        g_loadedActNumber = 1;
        SetNextSceneNumber(1);
        hotLoadModule = SystemModule::Cold;
        ChangeDisc();
        ChangeSystemModule(SystemModule::Cold, 0);
        break;
    default:
        break;
    }
    HLib::SetupTaskDelay(0);
}
void QueueOmakeModuleCall(OmakeWork *owner, const OmakeCommand *command) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(OmakeModuleCallCallback, 5, 0x0b, sizeof(OmakeModuleCallWork), OMAKE_EVENT);
    OmakeModuleCallWork *work = static_cast<OmakeModuleCallWork *>(HLib::GetTaskParameter(task));
    work->owner = owner;
    work->frames = 10;
    work->command = command;
    HLib::SetupTaskDelay(0);
}
void SelectOmakeEntry(std::int32_t category, std::uint32_t index) {
    const OmakeEntry *const tables[]{OMAKE_ARCADE_ENTRIES, OMAKE_MINIGAME_ENTRIES, OMAKE_FIGHT_ENTRIES};
    const OmakeEntry *table = tables[category];
    const OmakeEntry *entry = table + index;
    if (!entry->callback || ((static_cast<std::uint32_t>(GetCurrentGameFlags()) & entry->requiredFlags) != entry->requiredFlags && !omakeUnlockOverride)) {
        ExecuteAicaCommand(0x800ab, 0, 0);
        return;
    }
    omakeSelectedCategory = category;
    omakeSelectedEntry = static_cast<std::int32_t>(index);
}
bool IsOmakeSelectionPending() {
    return omakeSelectedCategory != -1 && omakeSelectedEntry != -1;
}
void RestoreOmakeArcadeSelection(OmakeWork *work) {
    if (work->selection->category != 0)
        return;
    const char *name = nullptr;
    switch (static_cast<EmulatorGame>(emuGetResult())) {
    case EmulatorGame::OutRun:
        name = "UIMCOLTY03"; // @text EN: "OutRun" | JA: "アウトラン"
        break;
    case EmulatorGame::AfterBurner:
        name = "UIMCOLTY04"; // @text EN: "After Burner II" | JA: "アフターバーナーII"
        break;
    case EmulatorGame::HangOn:
        name = "UIMCOLTY01"; // @text EN: "Hang-On" | JA: "ハングオン"
        break;
    case EmulatorGame::SpaceHarrier:
        name = "UIMCOLTY02"; // @text EN: "Space Harrier" | JA: "スペースハリアー"
        break;
    default:
        break;
    }
    std::int64_t selected = 0;
    for (std::int64_t index = 0; index < 13 && name && omakeGameList[index].name; ++index) {
        if (std::strcmp(name, omakeGameList[index].name) == 0) {
            selected = index;
            break;
        }
    }
    work->selection->selected[work->selection->category] = selected;
}
void RequestOmakeBack() {
    omakeBackRequested = 1;
}
const ScriptAreaDefinition omakScriptDefinition{.areaId = OMAKE_AREA, .field04 = 0, .init = InitializeOmake, .main = RunOmake};
} // namespace shenmue::s2::areas
