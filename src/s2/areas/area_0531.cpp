#include "s2/areas/area_0531.hpp"
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

#include "s2/areas/area_0507.hpp"
#include "s2/date_time.hpp"
#include "s2/particle_effects.hpp"
#include "s2/weather.hpp"
#include "common/math.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t fire0531, angle0531;
std::int32_t countdown0531, angleStep0531;
constexpr std::uint16_t palette0531[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
} // namespace
// @verified - p
void Setup0531Fire() {
    const Vector3 position{154.74000549316406f, 100.125f, 392.17999267578125f};
    DateTime date;
    GetCurrentDateTime(date);
    date.hour = 22;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(6);
    AssignLightProgram(7);
    AssignLightProgram(8);
    EV_lightLoad(AREA_0531_LIGHT);
    AssignLightProgram(3);
    AssignLightProgram(5);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0531_CONVERSATION);
    fire0531 = static_cast<std::uint32_t>(CreateFireEffect(position, 0.5f, 0.5f, 0, 0));
    SetFirePalette(fire0531, palette0531);
    SetFireFalloff(fire0531, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(fire0531, -0.2980392277240753f);
    SetParticleEffectSize(4, fire0531, 0.5490196347236633f, 0.8235294222831726f);
    SetParticleEffectPosition(4, fire0531, &position, &position);
    ResumeParticleEffect(4, fire0531);
    HLib::EnqueueTaskWithoutParameter(Wave0507Resources, 3, 2, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
}

// @verified - p
void Initialize0531() {
    fire0531 = angle0531 = 0;
    countdown0531 = angleStep0531 = 0;
}

// @verified - p
void Start0531(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0531, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0531_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0531Fire();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0531_CHARACTERS, parent->self);
}

// @verified - p
void Update0531(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0531Author(parent);
    } else if (parent->state == 3) {
        Cleanup0531(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, INPUT_PRESSED)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 0;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 0)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0531_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0531(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0531_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    SuspendParticleEffect(4, fire0531);
    FreeParticleEffect(4, fire0531);
    EV_FreeAuthorPak(0, AREA_0531_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0531Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0531AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_LoadAudio("0531A000", 0);
    Prepare0531Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0531Author(AuthorViewer519ELoopTaskParameter *child) {
    const float flicker = sin_fixed16(static_cast<std::int16_t>(angle0531)) * 0.05f + 0.5f;
    const float intensity = sin_fixed16(static_cast<std::int16_t>(angle0531)) * 0.05f + 0.8999999761581421f;
    countdown0531 = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(countdown0531) - 1U);
    if (countdown0531 <= 0) {
        countdown0531 = shenGetRandomInt32(60) + 30;
        angleStep0531 = shenGetRandomInt32(2048) + 2048;
    }
    angle0531 += static_cast<std::uint32_t>(angleStep0531);
    SetLightSceneIntensity(1, intensity);
    SetLightSceneIntensity(3, flicker);
    SetLightSceneIntensity(4, flicker);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 410)
        SetNewWindPhaseAndSpeed(4096, 0.2f);
    else if (frame == 559)
        SyncWindWithWDATWeatherState();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0531Characters();
        child->parent->state = 0;
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0531AuthorTask(void *taskData) {
    Update0531Author(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0531Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0531_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0531_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_a988, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_a990, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_a992, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_a994, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa51, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa52, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa53, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa54, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa55, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa56, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa57, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa58, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa59, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa60, false);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa61, false);
    for (const shen_name *character = AREA_0531_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0531Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0531_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0531_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_a988, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_a990, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_a992, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_a994, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa51, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa52, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa53, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa54, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa55, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa56, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa57, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa58, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa59, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa60, true);
    EV_SetCharacterAlphaEnabled(AREA_0531_aa61, true);
    for (const shen_name *character = AREA_0531_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0531ScriptDefinition{.areaId = AREA_0531, .field04 = 0, .init = Initialize0531, .main = Start0531};

} // namespace shenmue::s2::areas
