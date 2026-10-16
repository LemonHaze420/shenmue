#include "s2/areas/area_0260.hpp"
#include "s2/sound_program.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include <array>
#include <bit>
namespace shenmue::s2::areas {
namespace {
HLib::Task *elevator0260PanelTask{};
std::array<shen_name, 9> elevator0260Characters{};
void E0260MainCallback(void *);
void E0260LoopCallback(void *);
void E0260PanelCallback(void *);
// @verified - p
void E0260Init() {
    elevator0260PanelTask = nullptr;
    elevator0260Characters.fill(shen_name{0});
}
void CreateE0260PanelCharacters() {
    Vector3 position{249.67999267578125f, 83.2300033569336f, 123.54f};
    const Vector3i rotation{0, 0x4000, 0};
    Vector3 hidden{0.0f, 1000.0f, 0.0f}, zero{};
    for (std::int32_t index = 0; index < 9; ++index) {
        elevator0260Characters[index] = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("ELVTTBBG"), shen_name{0});
        const shen_name character = elevator0260Characters[index];
        EV_EnableCharacterDisplay(character);
        position.y += 0.090000004f;
        if (index == 4) {
            position.y = 83.230003f;
            position.z += -0.090000004f;
        }
        EV_AT_SetPos(character, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
        EV_AT_SetAngY(character, AREA_AUTHOR_ROOT_POSITION_FLAGS, rotation, nullptr);
        for (const std::int32_t *part = AREA_0260_DIGIT_PARTS; *part != -1; ++part) {
            EV_scenApplyPosOrRotToPartOf(character, static_cast<std::uint16_t>(*part), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
            EV_scenApplyPosOrRotToPartOf(character, 0xa2, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
        }
    }
    for (const std::int32_t *part = AREA_0260_DIGIT_PARTS; *part != -1; ++part)
        EV_scenApplyPosOrRotToPartOf(AREA_0260_A800, static_cast<std::uint16_t>(*part), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    for (const std::int32_t *part = AREA_0260_PANEL_PARTS; *part != -1; ++part)
        EV_scenApplyPosOrRotToPartOf(AREA_0260_A798, static_cast<std::uint16_t>(*part), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    const shen_name last = elevator0260Characters[8];
    EV_scenApplyPosOrRotToPartOf(last, 0xa2, &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(last, 0xe7, &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(last, 0xe8, &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(last, 0xe9, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(last, 0xc4, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(last, 0xf2, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(AREA_0260_A800, 0xcf, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(AREA_0260_A800, 0xe7, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(AREA_0260_A800, 0xf7, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
}
// @verified - p
void E0260Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0260MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0260A.MOT"));
    parent->self = task;
    parent->motion = motion;
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0260");
    EV_SetWeatherInstruction(12, 0);
    CreateE0260PanelCharacters();
}
// @verified - p
void QueueE0260Panel(std::int32_t fromFloor, std::int32_t toFloor) {
    Vector3 hidden{0.0f, 1000.0f, 0.0f}, zero{};
    for (const std::int32_t *part = AREA_0260_PANEL_PARTS; *part != -1; ++part)
        EV_scenApplyPosOrRotToPartOf(AREA_0260_A798, static_cast<std::uint16_t>(*part), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    const std::int32_t absoluteFloor = fromFloor < 0 ? -fromFloor : fromFloor;
    EV_scenApplyPosOrRotToPartOf(
        AREA_0260_A798, static_cast<std::uint16_t>(absoluteFloor % 10 + 152), &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(
        AREA_0260_A798, static_cast<std::uint16_t>(AREA_0260_TENS_PARTS[fromFloor / 10]), &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    elevator0260PanelTask = HLib::EnqueueTaskWithParameter(E0260PanelCallback, 4, 0x0b, 20, AREA_AUTHOR_TASK);
    E0260PanelWork *const panel = static_cast<E0260PanelWork *>(HLib::GetTaskParameter(elevator0260PanelTask));
    panel->fromFloor = fromFloor;
    panel->toFloor = toFloor;
    panel->currentFloor = fromFloor + 1;
    panel->tens = fromFloor / 10;
    panel->delay = 35;
}
// @verified - p
void E0260PanelCallback(void *parameter) {
    E0260PanelWork *const panel = static_cast<E0260PanelWork *>(parameter);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0)
        HLib::DestroyCurrentTask();
    if (panel->delay-- != 1)
        return;
    Vector3 hidden{0.0f, 1000.0f, 0.0f}, zero{};
    for (const std::int32_t *part = AREA_0260_PANEL_PARTS; *part != -1; ++part)
        EV_scenApplyPosOrRotToPartOf(AREA_0260_A798, static_cast<std::uint16_t>(*part), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    std::uint16_t units;
    if (panel->currentFloor >= 0) {
        const std::int32_t tens = panel->currentFloor / 10;
        EV_scenApplyPosOrRotToPartOf(
            AREA_0260_A798, static_cast<std::uint16_t>(AREA_0260_TENS_PARTS[tens]), &zero, nullptr, EventCharacterPartTransformMode::Absolute);
        units = tens != 0                 ? static_cast<std::uint16_t>(panel->currentFloor % 10 + 152) :
            panel->currentFloor % 10 == 0 ? 153 :
                                            static_cast<std::uint16_t>(panel->currentFloor % 10 + 152);
    } else {
        EV_scenApplyPosOrRotToPartOf(AREA_0260_A798, 0xcf, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
        const std::int32_t absoluteFloor = -panel->currentFloor;
        units = absoluteFloor % 10 == 0 ? 153 : static_cast<std::uint16_t>(absoluteFloor % 10 + 152);
    }
    EV_scenApplyPosOrRotToPartOf(AREA_0260_A798, units, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
    panel->delay = 35;
    if (panel->currentFloor == panel->toFloor) {
        HLib::DestroyCurrentTask();
        return;
    }
    ++panel->currentFloor;
}
void SetupE0260Characters(std::int16_t selection) {
    EV_EnsureCharacterDraw(AREA_AUTHOR_RYO);
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    for (const shen_name *character = AREA_0260_CHARACTER_TABLES[selection]; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_EnsureCharacterDraw(*character);
        EV_EnableCharacterDisplay(*character);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
    if (selection == 4) {
        EV_EnsureCharacterDraw(AREA_0260_JOP);
        EV_EnableCharacterDisplay(AREA_0260_JOP);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0260_JOP), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0260_JOP, false);
        EV_EnableCharacterDisplay(AREA_0260_A800);
        EV_SetCharacterAlphaEnabled(AREA_0260_A800, false);
        EV_EnableCharacterDisplay(AREA_0260_A798);
        EV_SetCharacterAlphaEnabled(AREA_0260_A798, false);
    } else {
        if (selection < 5 || selection > 6) {
            EV_EnableCharacterDisplay(AREA_0260_A516);
            EV_SetCharacterAlphaEnabled(AREA_0260_A516, false);
            EV_EnableCharacterDisplay(AREA_0260_A512);
            EV_SetCharacterAlphaEnabled(AREA_0260_A512, false);
            EV_EnableCharacterDisplay(AREA_0260_A734);
            EV_SetCharacterAlphaEnabled(AREA_0260_A734, false);
        }
        LoadBlurModelForCharacter(AREA_0260_SF4, "SF4_B.MT7", 1.0f);
    }
}
void RestoreE0260Characters(std::int16_t selection) {
    EV_ToggleCharacterFlag(AREA_AUTHOR_RYO);
    EV_DisableCharacterDisplay(AREA_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    for (const shen_name *character = AREA_0260_CHARACTER_TABLES[selection]; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_ToggleCharacterFlag(*character);
        EV_DisableCharacterDisplay(*character);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
    if (selection == 4) {
        EV_EnableCharacterDisplay(AREA_0260_A800);
        EV_SetCharacterAlphaEnabled(AREA_0260_A800, true);
        EV_EnableCharacterDisplay(AREA_0260_A798);
        EV_SetCharacterAlphaEnabled(AREA_0260_A798, true);
    } else if (selection < 5 || selection > 6) {
        EV_EnableCharacterDisplay(AREA_0260_A516);
        EV_SetCharacterAlphaEnabled(AREA_0260_A516, true);
        EV_EnableCharacterDisplay(AREA_0260_A512);
        EV_SetCharacterAlphaEnabled(AREA_0260_A512, true);
        EV_EnableCharacterDisplay(AREA_0260_A734);
        EV_SetCharacterAlphaEnabled(AREA_0260_A734, true);
    }
}
// @verified - p
void QueueE0260Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0260LoopCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const loop = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    loop->parent = parent;
    loop->currentFrame = 0;
    EV_authSceneCtrl(parent->selection >= 5 && parent->selection <= 6 ? AuthSceneCtrl::HideFilmBars : AuthSceneCtrl::ShowFilmBars);
    EV_LoadCutsceneFromCurrentScene(parent->selection == 4 ? "0260A090" :
            parent->selection == 5                         ? "0260A030" :
            parent->selection == 6                         ? "0260A050" :
                                                             "0260A000");
    PushGlobalLightingProgram();
    if (parent->selection == 4) {
        EV_lightLoad(AREA_0260_LIGHT);
        PushGlobalLightingProgram();
        EV_lightLoad(AREA_0260_LIGHT_SPECIAL);
    } else if (parent->selection >= 5 && parent->selection <= 6) {
        EV_lightLoad(AREA_0260_LIGHT);
        PushGlobalLightingProgram();
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetupE0260Characters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void E0260LoopCallback(void *parameter) {
    AreaAuthorFrameTaskParameter *const loop = static_cast<AreaAuthorFrameTaskParameter *>(parameter);
    loop->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (loop->parent->selection == 4) {
        if (loop->currentFrame == 60) {
            Vector3 zero{};
            EV_scenApplyPosOrRotToPartOf(AREA_0260_A798, 0x98, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
            EV_scenApplyPosOrRotToPartOf(AREA_0260_A798, 0x99, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
        } else if (loop->currentFrame == 90)
            QueueE0260Panel(1, 3);
        else if (loop->currentFrame == 310)
            QueueE0260Panel(2, 12);
        else if (loop->currentFrame == 1575)
            QueueE0260Panel(32, 36);
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    RestoreE0260Characters(loop->parent->selection);
    loop->parent->state = 0;
    for (const shen_name *character = AREA_0260_CHARACTER_TABLES[loop->parent->selection]; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
// @verified - p
void E0260MainCallback(void *parameter) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(parameter);
    if (parent->state != 0) {
        if (parent->state == 1)
            QueueE0260Loop(parent);
        else if (parent->state == 3) {
            EV_FreeAuthorPak(0, "0260");
            DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
            EV_CleanupMapControlTask();
            HLib::DestroyTask(parent->self);
            EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        }
        return;
    }
    const std::int16_t input = EVT_GetControllerInput(0, 1);
    if (input == 1024) {
        parent->state = 1;
        Queue_EVT_UTL_LoadNPCChara_3(AREA_0260_CHARACTER_TABLES[parent->selection], parent->self);
    } else if (input == 64) {
        if (--parent->selection < 0)
            parent->selection = 6;
    } else if (input == 128) {
        if (++parent->selection > 6)
            parent->selection = 0;
    }
    if (parent->selection >= 0 && parent->selection <= 6) {
        EV_set_debug_text_pos(2, 4);
        EV_debug_log_to_screen("%s\n", AREA_0260_LABELS[parent->selection]);
    }
    EV_set_debug_text_pos(2, 5);
    EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
}
} // namespace
const ScriptAreaDefinition area0260ScriptDefinition{AREA_0260_AREA, 0, E0260Init, E0260Main};
} // namespace shenmue::s2::areas
