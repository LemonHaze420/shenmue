#include "s2/areas/area_0509.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/date_time.hpp"
#include "s2/particle_effects.hpp"
#include "s2/model_component.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t fire0509;
constexpr std::uint16_t palette0509[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
} // namespace
// @verified - p
void Setup0509Fire() {
    const Vector3 position{154.74000549316406f, 100.125f, 392.17999267578125f};
    DateTime date;
    GetCurrentDateTime(date);
    date.minute = 0;
    SetPlayerControlEnabled(0);
    fire0509 = static_cast<std::uint32_t>(CreateFireEffect(position, 0.5f, 0.5f, 0, 0));
    SetFirePalette(fire0509, palette0509);
    SetFireFalloff(fire0509, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(fire0509, -0.2980392277240753f);
    SetParticleEffectSize(4, fire0509, 0.5490196347236633f, 0.8235294222831726f);
    SetParticleEffectPosition(4, fire0509, &position, &position);
    ResumeParticleEffect(4, fire0509);
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0509_LIGHT);
    AssignLightProgram(0);
    date.hour = 22;
    SetCurrentDateTimeTo(date);
    EV_FreeAuthorPak(1, AREA_0509_CONVERSATION);
}

// @verified - p
void Initialize0509() {
    fire0509 = 0;
}

// @verified - p
void Start0509(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0509, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0509_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0509Fire();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0509_CHARACTERS, parent->self);
}

// @verified - p
void Update0509(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0509Author(parent);
    } else if (parent->state == 3) {
        Cleanup0509(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, INPUT_PRESSED)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 4;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 4)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 4) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0509_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0509(AuthorViewerPairTaskParameter *parent) {
    clmdUtil(FindCharacterByScenarioName(AREA_0509_SIN_), 0);
    SuspendParticleEffect(4, fire0509);
    FreeParticleEffect(4, fire0509);
    for (const shen_name *character = AREA_0509_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0509_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0509Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0509AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_LoadAudio("0509A000", 0);
    if (parent->selection == 1)
        clmdUtil(FindCharacterByScenarioName(AREA_0509_SIN_), 8);
    else if (parent->selection == 0 || parent->selection == 3)
        clmdUtil(FindCharacterByScenarioName(AREA_0509_SIN_), 5);
    EV_authSceneCtrl((parent->selection & 0xfffffffcU) != 0 || parent->selection == 2 ? AuthSceneCtrl::ShowFilmBars : AuthSceneCtrl::HideFilmBars);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0509Characters();
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0509Author(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0509Characters();
        child->parent->state = 0;
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0509AuthorTask(void *taskData) {
    Update0509Author(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0509Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0509_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0509_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0509_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0509_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0509_a540, false);
    EV_SetCharacterAlphaEnabled(AREA_0509_a436, false);
    EV_SetCharacterAlphaEnabled(AREA_0509_a528, false);
    EV_SetCharacterAlphaEnabled(AREA_0509_a526, false);
    const Vector3 zero{};
    EV_AT_SetPos(AREA_0509_a540, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_AT_SetPos(AREA_0509_a436, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_AT_SetPos(AREA_0509_a528, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_AT_SetPos(AREA_0509_a526, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
}

// @verified - p
void Restore0509Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0509_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0509_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0509_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0509_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0509_a540, true);
    EV_SetCharacterAlphaEnabled(AREA_0509_a436, true);
    EV_SetCharacterAlphaEnabled(AREA_0509_a528, true);
    EV_SetCharacterAlphaEnabled(AREA_0509_a526, true);
}

const ScriptAreaDefinition area0509ScriptDefinition{.areaId = AREA_0509, .field04 = 0, .init = Initialize0509, .main = Start0509};

} // namespace shenmue::s2::areas
