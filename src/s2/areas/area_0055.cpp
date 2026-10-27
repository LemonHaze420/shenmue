#include "s2/areas/area_0055.hpp"
#include "s2/map_assets.hpp"
#include "s2/sound_program.hpp"
#include "s2/areas/harbor_author_event.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
namespace shenmue::s2::areas {
namespace {
std::array<std::byte, 0x100> distance0055{}, kernel0055First{}, kernel0055Second{};
void E0055MainCallback(void *);
void E0055LoopCallback(void *);
// @verified - p
void E0055Init() {
    distance0055.fill(std::byte{});
    kernel0055First.fill(std::byte{});
    kernel0055Second.fill(std::byte{});
}
// @verified - p
void SetupE0055Environment() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0055");
    if (g_entryPoint == 0 || g_entryPoint == 1 || g_entryPoint == 2 || g_entryPoint == 6) {
        DateTime time{};
        GetCurrentDateTime(time);
        time.hour = 22;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0055_LIGHT);
        PushGlobalLightingProgram();
        EV_SetObjectFlagByIndex(10, 0);
        EV_SetObjectFlagByIndex(11, 1);
        EV_SetObjectFlagByIndex(12, 0);
    } else if (g_entryPoint == 3 || g_entryPoint == 4 || g_entryPoint == 5 || g_entryPoint == 7) {
        DateTime time{};
        GetCurrentDateTime(time);
        time.hour = 8;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(10, 1);
        EV_SetObjectFlagByIndex(11, 0);
    }
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    shenSetClockAndCompassRenderTo(1);
    if (g_entryPoint < 6)
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    InitializeWaterTextureAnimation(5);
    GenerateFFT(2.0f);
    FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(1.7170000076293945f, distance0055)));
    void *const second = FillFftKernel(0.0729999989271164f, GenerateDistanceFalloffKernel8x8(0.6470000147819519f, kernel0055Second));
    void *const first = FillFftKernel(0.061000000685453415f, GenerateDistanceFalloffKernel8x8(0.37599998712539673f, kernel0055First));
    ApplyFft(first, second);
    FinalizeFft(0.001f, 0.001f);
    SetWaveLighting(0.1f, {-0.16320001f, 0.98180002f, -0.096900001f});
    HLib::EnqueueTaskWithoutParameter(HarborAuthorWaveCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
    HLib::EnqueueTaskWithoutParameter(HarborAuthorShipCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
}
// @verified - p
void E0055Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0055MainCallback, 4, 0x0b, 24, AREA_AUTHOR_TASK);
    E0055TaskParameter *const parent = static_cast<E0055TaskParameter *>(HLib::GetTaskParameter(task));
    parent->author.self = task;
    parent->author.motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0055A.MOT"));
    parent->author.selection = g_entryPoint <= 7 ? static_cast<std::int16_t>(g_entryPoint) : 0;
    parent->author.state = 1;
    SetupE0055Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0055_CHARACTERS, parent->author.self);
}
// @verified - p
void SetupE0055Evening() {
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 22;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(3, 1);
    EV_SetObjectFlagByIndex(12, 1);
    SetMapModelVertexShaderType(3, 1);
    SetMapModelVertexShaderType(12, 1);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
}
// @verified - p
void SetupE0055Morning() {
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(3, 1);
    EV_SetObjectFlagByIndex(10, 1);
    EV_SetObjectFlagByIndex(11, 0);
    EV_SetObjectFlagByIndex(12, 1);
    SetMapModelVertexShaderType(3, 1);
    SetMapModelVertexShaderType(12, 1);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
}
void SetupE0055Characters(std::int16_t selection) {
    EV_EnsureCharacterDraw(AREA_AUTHOR_RYO);
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_EnsureCharacterDraw(AREA_0055_WON);
    EV_EnableCharacterDisplay(AREA_0055_WON);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0055_WON), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0055_WON, false);
    const shen_name evening[]{AREA_0055_A126, AREA_0055_A142};
    const shen_name morning[]{AREA_0055_A142, AREA_0055_A506, AREA_0055_AA50, AREA_0055_A504};
    const shen_name ordinary[]{AREA_0055_A016, AREA_0055_A298};
    const shen_name *const characters = selection == 6 ? evening : selection == 7 ? morning : ordinary;
    const std::int32_t count = selection == 7 ? 4 : 2;
    for (std::int32_t index = 0; index < count; ++index) {
        EV_EnsureCharacterDraw(characters[index]);
        EV_EnableCharacterDisplay(characters[index]);
        EV_SetCharacterAlphaEnabled(characters[index], false);
    }
}
void RestoreE0055Characters(std::int16_t selection) {
    EV_ToggleCharacterFlag(AREA_AUTHOR_RYO);
    EV_DisableCharacterDisplay(AREA_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_ToggleCharacterFlag(AREA_0055_WON);
    EV_DisableCharacterDisplay(AREA_0055_WON);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0055_WON), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0055_WON, true);
    const shen_name evening[]{AREA_0055_A126, AREA_0055_A142};
    const shen_name morning[]{AREA_0055_A142, AREA_0055_A506, AREA_0055_AA50, AREA_0055_A504};
    const shen_name ordinary[]{AREA_0055_A016, AREA_0055_A298};
    const shen_name *const characters = selection == 6 ? evening : selection == 7 ? morning : ordinary;
    const std::int32_t count = selection == 7 ? 4 : 2;
    for (std::int32_t index = 0; index < count; ++index) {
        EV_ToggleCharacterFlag(characters[index]);
        EV_DisableCharacterDisplay(characters[index]);
        EV_SetCharacterAlphaEnabled(characters[index], true);
    }
}
// @verified - p
void QueueE0055Loop(E0055TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0055LoopCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const loop = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    loop->parent = &parent->author;
    loop->currentFrame = 0;
    if (parent->author.selection == 6) {
        EV_LoadCutsceneFromCurrentScene("0055A000");
        SetupE0055Evening();
    } else if (parent->author.selection == 7) {
        EV_LoadCutsceneFromCurrentScene("0055A005");
        parent->voice = shenPlayAudioWithFlags("wonjunk", 1);
        SetupE0055Morning();
    } else
        EV_LoadCutsceneFromCurrentScene("0055A010");
    EV_spmBeginSampleExecute();
    SetupE0055Characters(parent->author.selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author.selection));
    parent->author.state = 2;
}
// @verified - p
void E0055LoopCallback(void *parameter) {
    AreaAuthorFrameTaskParameter *const loop = static_cast<AreaAuthorFrameTaskParameter *>(parameter);
    E0055TaskParameter *const parent = reinterpret_cast<E0055TaskParameter *>(loop->parent);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0055Characters(parent->author.selection);
    if (parent->author.selection == 6) {
        HLib::SetupTimedTask(parent->author.self, 30);
        parent->author.state = 4;
        ++parent->author.selection;
    } else {
        if (parent->author.selection == 7) {
            hsFadeVoice(parent->voice, 30);
            HLib::SetupTimedTask(parent->author.self, 130);
            AddInventoryItem(0x18, 1, 1);
        } else
            HLib::SetupTimedTask(parent->author.self, 30);
        parent->author.state = 3;
    }
    HLib::DestroyCurrentTask();
}
// @verified - p
void CleanupE0055(E0055TaskParameter *parent) {
    if (FindCharacterByScenarioName(AREA_0055_WON)) {
        EV_npcLockUnlock(AREA_0055_WON);
        EV_npcFree(AREA_0055_WON);
    }
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_FreeAuthorPak(0, "0055");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->author.motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->author.self);
    if (g_entryPoint == 6)
        EV_scenQueueLoad(g_actNumber, AREA_0055_AB00, 51);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void E0055MainCallback(void *parameter) {
    E0055TaskParameter *const parent = static_cast<E0055TaskParameter *>(parameter);
    switch (parent->author.state) {
    case 1:
        QueueE0055Loop(parent);
        break;
    case 3:
        CleanupE0055(parent);
        break;
    case 4:
        shenUpdateCurrentDateTime();
        Queue_ST_LoadTitleFadeIn(AREA_0055_NDAY);
        HLib::SetupTimedTask(parent->author.self, 60);
        parent->author.state = 5;
        break;
    case 5:
        ST_LoadTitleFade();
        HLib::SetupTimedTask(parent->author.self, 60);
        parent->author.state = 1;
        break;
    default:
        break;
    }
}
} // namespace
const ScriptAreaDefinition area0055ScriptDefinition{AREA_0055_AREA, 0, E0055Init, E0055Main};
} // namespace shenmue::s2::areas
