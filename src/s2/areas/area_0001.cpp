#include "s2/areas/area_0001.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/system_modules.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {
bool g_titleIntroPressedStart{};
namespace {
std::array<std::int16_t, 4> previous0001Input{1, 1, 1, 1};
HLib::Task *lighting0001Task{};
} // namespace

// @verified - p: dbc90 PressedStartHandler publishes the actual UI-selected flag.
void NotifyTitleIntroPressStart() {
    g_titleIntroPressedStart = true;
}

// @verified - p
void Area0001Init() {
    previous0001Input.fill(1);
    lighting0001Task = nullptr;
}

// @verified - p
void Area0001Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0001, 4, 0x0b, sizeof(Title0001TaskParameter), AREA_0001_TASK);
    Title0001TaskParameter *const parent = static_cast<Title0001TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->mainState = 1;
    Setup0001();
}

// @verified - p
void Update0001(void *taskData) {
    Title0001TaskParameter *const parent = static_cast<Title0001TaskParameter *>(taskData);
    if (parent->mainState == 1)
        Queue0001Author(parent);
}

// @verified - p
void Setup0001() {
    g_titleIntroPressedStart = false;
    DispatchPostFxFilterRender(1);
    SetSnapshotDataOption(1);
    SetPostFxFilterMode(1);
    SetPlayerControlEnabled(0);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    SetLegacyVertexColourType(0);
    SetWeatherUpdateEnabledTo(0);
    ReinitFogTasks(0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    SetRenderClockAndCompassTo(1);
    EV_ToggleCharacterFlag(AREA_0001_PLAYER);
    EV_EnsureCharacterDraw(AREA_0001_HIDDEN_MODEL);
    EV_DisableCharacterDisplay(AREA_0001_HIDDEN_MODEL);
    Queue0001Lighting();
    UpdateGlobalLighting(Vector4{0.28f, 0.28f, 0.28f, 0.28f});
}

// @verified - p
std::int64_t Queue0001Author(Title0001TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0001Author, 4, 0x0b, sizeof(Title0001AuthorTaskParameter), AREA_0001_TASK);
    Title0001AuthorTaskParameter *const child = static_cast<Title0001AuthorTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    child->active = 1;
    child->elapsedFrames = 0.0f;
    for (std::int32_t user = 0; user < 4; ++user)
        previous0001Input[user] = EVT_GetControllerInput(user, 12);
    camCtrlSaveRuntime();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_LoadAudio("0001A020", 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(0));
    parent->mainState = 2;
    shenSetForcedFilmbars(1);
    return 2;
}

// @verified - p
void Update0001Author(void *taskData) {
    Title0001AuthorTaskParameter *const child = static_cast<Title0001AuthorTaskParameter *>(taskData);
    child->currentFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (child->currentFrame) {
    case 0:
        SelectPostFxBank(1, 1);
        UploadGlowParams(AREA_0001_AUTHOR_GLOW);
        camCtrlResetClipDistances();
        Prepare0001Characters();
        figpAssignModeFor(AREA_0001_FIGURE_FIRST, 1);
        figpSetFlagsBlendForBlur(AREA_0001_FIGURE_FIRST, 10, -1);
        figpAssignModeFor(AREA_0001_FIGURE_SECOND, 1);
        figpSetFlagsBlendForBlur(AREA_0001_FIGURE_SECOND, 10, -1);
        FreezePressStartPresentation();
        break;
    case 105:
        figpAssignModeFor(AREA_0001_FIGURE_FIRST, 0);
        figpAssignModeFor(AREA_0001_FIGURE_SECOND, 0);
        break;
    case 130:
        Queue0001Alpha(AREA_0001_FIGURE_FIRST, child->currentFrame, 1.0f, 0.0f);
        Queue0001Alpha(AREA_0001_FIGURE_SECOND, child->currentFrame, 1.0f, 0.0f);
        AnimatePressStartWidget(7, 1.0f, 0.0f, 1.0f);
        AnimatePressStartWidget(8, 1.0f, 0.0f, 1.0f);
        AnimatePressStartAngle(1.0f, 5.8468499f, 6.2831898f);
        break;
    case 160:
        AnimatePressStartWidget(6, 1.0f, 0.0f, 1.0f);
        AnimatePressStartWidget(7, 1.0f, 1.0f, 0.0f);
        AnimatePressStartWidget(8, 0.5f, 1.0f, 0.0f);
        break;
    case 190:
        SelectPostFxBank(1, 1);
        UploadGlowParams(AREA_0001_PROMPT_GLOW);
        camCtrlSetClipDistances(0.5f, 2000.0f);
        AnimatePressStartWidget(5, 1.0f, 0.0f, 1.0f);
        break;
    case 220:
        AnimatePressStartWidget(3, 1.0f, 0.0f, 1.0f);
        AnimatePressStartWidget(13, 1.0f, 0.0f, 1.0f);
        AnimatePressStartWidget(5, 1.0f, 1.0f, 0.0f);
        break;
    default:
        break;
    }
    bool requested = (EVT_GetControllerInput(0, 1) & 0x0ede) != 0;
    for (std::int32_t user = 0; user < 4 && !requested; ++user) {
        if (previous0001Input[user] == 0 && EVT_GetControllerInput(user, 12) != 0)
            requested = true;
        if (!requested && previous0001Input[user] != 0 && EVT_GetControllerInput(user, 13) != 0)
            requested = true;
    }
    if (requested) {
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        Clear0001Characters();
        child->parent->state = 1;
        ProcessAicaCommand(0x1200a0, 0);
        for (const shen_name character : AREA_0001_CLEANUP_CHARACTERS) {
            if (character.name == 0xffffffffU)
                break;
            EV_EnsureCharacterDraw(character);
            EV_DisableCharacterDisplay(character);
        }
        RestorePressStartPresentation();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(1));
        shenSetForcedFilmbars(1);
        HLib::SetCurrentTaskTo(Update0001Prompt);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Clear0001Characters();
        child->parent->state = 1;
        ProcessAicaCommand(0x1200a0, 0);
        for (const shen_name character : AREA_0001_CLEANUP_CHARACTERS) {
            if (character.name == 0xffffffffU)
                break;
            EV_EnsureCharacterDraw(character);
            EV_DisableCharacterDisplay(character);
        }
        RestorePressStartPresentation();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(1));
        shenSetForcedFilmbars(1);
        HLib::SetCurrentTaskTo(Update0001Prompt);
    }
    for (std::int32_t user = 0; user < 4; ++user)
        previous0001Input[user] = EVT_GetControllerInput(user, 12);
    EnableElement(UIElement::PressStart);
}

// @verified - p
void Update0001Prompt(void *taskData) {
    Title0001AuthorTaskParameter *const child = static_cast<Title0001AuthorTaskParameter *>(taskData);
    camCtrlSetClipDistances(0.5f, 2000.0f);
    EnableElement(UIElement::PressStart);
    EV_DisableCharacterDisplay(AREA_0001_PRESS_MODEL);
    EV_DisableCharacterDisplay(AREA_0001_LAST_MODEL);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        ProcessAicaCommand(0x1200a0, 0);
        Cleanup0001Characters();
        HLib::DestroyCurrentTask();
        shenQueueChangeModuleAndScene(SystemModule::Title, g_actNumber, g_currentMap, 0);
    } else if (g_titleIntroPressedStart) {
        SelectInputDeviceForButtons(3072);
        ProcessAicaCommand(0x1200a0, 0);
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        Cleanup0001Characters();
        HLib::DestroyCurrentTask();
        shenQueueChangeModuleAndScene(SystemModule::Title, g_actNumber, g_currentMap, 49);
    } else {
        child->elapsedFrames += g_stabilizeFps ? g_stabilizedFrameTime * 30.0f : 1.0f;
    }
}

// @verified - p
void Prepare0001Characters() {
    for (const shen_name character : AREA_0001_AUTHOR_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        EV_ToggleCharacterFlag(character);
        EV_EnableCharacterDisplay(character);
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(character, false);
        SetScenarioCharacterRadius(character, 1000.0f);
        SetScenarioCharacterTransparency(character, 0.0f);
        SetPluralControlFor(FindCharacterByScenarioName(character), 0);
    }
    for (const shen_name character : AREA_0001_FOREGROUND_CHARACTERS) {
        EV_ToggleCharacterFlag(character);
        EV_EnableCharacterDisplay(character);
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(character, false);
        SetScenarioCharacterRadius(character, 1000.0f);
    }
    SetScenarioCharacterTransparency(AREA_0001_FOREGROUND_CHARACTERS[1], 1.0f);
    SetScenarioCharacterTransparency(AREA_0001_FOREGROUND_CHARACTERS[0], 1.0f);
}

// @verified - p
void Clear0001Characters() {
    for (const shen_name character : AREA_0001_CLEAR_CHARACTERS) {
        EV_EnsureCharacterDraw(character);
        EV_DisableCharacterDisplay(character);
    }
}

// @verified - p
void Cleanup0001Characters() {
    SelectPostFxBank(1, 0);
    SetSnapshotDataOption(0);
    std::int32_t index = 0;
    for (const shen_name character : AREA_0001_CLEANUP_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        EV_debug_log("0001\\e0001_et_0001.c", 718, "ID : %d [%c%c%c%c]\n", index, character.name & 0xffU, character.name >> 8U & 0xffU,
            character.name >> 16U & 0xffU, character.name >> 24U);
        EV_EnsureCharacterDraw(character);
        EV_DisableCharacterDisplay(character);
        ++index;
    }
    camCtrlRestoreRuntime();
}

// @verified - p
Title0001LightingTaskParameter *Queue0001Lighting() {
    lighting0001Task = HLib::EnqueueTaskWithParameter(Update0001Lighting, 4, 0x0b, sizeof(Title0001LightingTaskParameter), AREA_0001_TASK);
    Title0001LightingTaskParameter *const work = static_cast<Title0001LightingTaskParameter *>(HLib::GetTaskParameter(lighting0001Task));
    work->firstState = work->secondState = 0;
    work->firstDelta = work->secondDelta = 0.0f;
    work->firstRemaining = work->secondRemaining = 13;
    work->firstRange = 35.189999f;
    work->firstUpper = 40.0f;
    work->firstLower = 26.0f;
    work->secondRange = 29.33f;
    work->secondUpper = 35.0f;
    work->secondLower = 30.0f;
    return work;
}

// @verified - p
void Update0001Lighting(void *taskData) {
    Title0001LightingTaskParameter *const work = static_cast<Title0001LightingTaskParameter *>(taskData);
    if (work->firstState != 0) {
        const std::int16_t remaining = work->firstRemaining--;
        if (remaining != 0) {
            work->firstRange += work->firstDelta;
            if (work->firstLower <= work->firstRange && work->firstRange <= work->firstUpper) {
                SetLightRange(10, work->firstRange);
            } else {
                work->firstRemaining = 0;
                work->firstState = 0;
            }
        } else {
            work->firstRemaining = 13;
            work->firstState = 0;
        }
    } else {
        work->firstDelta = GetRandomNumberInRange(5.0f) / (work->firstRemaining ? static_cast<float>(work->firstRemaining) : 0.0000099999997f);
        if (work->firstRange <= work->firstUpper) {
            if (work->firstLower <= work->firstRange)
                work->firstDelta *= shenGetRandomInt32(5) <= 2 ? -1.0f : 1.0f;
            else
                work->firstDelta *= work->firstDelta <= 0.0f ? -1.0f : 1.0f;
        } else
            work->firstDelta *= work->firstDelta <= 0.0f ? 1.0f : -1.0f;
        work->firstState = 1;
    }
    if (work->secondState != 0) {
        const std::int16_t remaining = work->secondRemaining--;
        if (remaining != 0) {
            work->secondRange += work->secondDelta;
            if (work->secondLower <= work->secondRange && work->secondRange <= work->secondUpper) {
                SetLightRange(11, work->secondRange);
            } else {
                work->secondRemaining = 0;
                work->secondState = 0;
            }
        } else {
            work->secondRemaining = 13;
            work->secondState = 0;
        }
    } else {
        work->secondDelta = GetRandomNumberInRange(5.0f) / (work->secondRemaining ? static_cast<float>(work->secondRemaining) : 0.0000099999997f);
        if (work->secondRange <= work->secondUpper) {
            if (work->secondLower <= work->secondRange)
                work->secondDelta *= shenGetRandomInt32(5) <= 2 ? -1.0f : 1.0f;
            else
                work->secondDelta *= work->secondDelta <= 0.0f ? -1.0f : 1.0f;
        } else
            work->secondDelta *= work->secondDelta <= 0.0f ? 1.0f : -1.0f;
        work->secondState = 1;
    }
}

// @verified - p: the second native argument is unused; the payload is exactly 20 bytes.
Title0001AlphaTaskParameter *Queue0001Alpha(shen_name character, std::int64_t, float from, float to) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0001Alpha, 4, 0x0b, sizeof(Title0001AlphaTaskParameter), AREA_0001_TASK);
    Title0001AlphaTaskParameter *const work = static_cast<Title0001AlphaTaskParameter *>(HLib::GetTaskParameter(task));
    work->current = from;
    work->character = character;
    work->increasing = 0;
    work->remaining = 30;
    work->target = to;
    work->step = from / 30.0f;
    return work;
}

// @verified - p: task destruction is deferred; the unloaded branch intentionally continues.
void Update0001Alpha(void *taskData) {
    Title0001AlphaTaskParameter *const work = static_cast<Title0001AlphaTaskParameter *>(taskData);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::DestroyCurrentTask();
    if (work->remaining <= 0) {
        SetScenarioCharacterTransparency(work->character, work->increasing ? work->target : 0.0f);
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            HLib::DestroyCurrentTask();
    } else {
        work->current = work->increasing ? work->current + work->step : work->current - work->step;
        SetScenarioCharacterTransparency(work->character, work->current);
        --work->remaining;
    }
}

// @verified - p: explicit native nullsub_0 boundary.
void Finish0001Noop(void *) {
}

const ScriptAreaDefinition area0001ScriptDefinition{.areaId = AREA_0001, .field04 = 0, .init = Area0001Init, .main = Area0001Main};
} // namespace shenmue::s2::areas
