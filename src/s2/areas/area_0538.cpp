#include "s2/areas/area_0538.hpp"
#include "s2/author.hpp"
#include "s2/input.hpp"
#include "s2/sound_program.hpp"
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

#include "s2/areas/area_0514.hpp"
#include "s2/areas/area_0516.hpp"
#include "s2/particle_effects.hpp"
#include "common/math.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t fire0538, angle0538;
std::int32_t countdown0538, step0538;
constexpr std::uint16_t palette0538[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
} // namespace

// @verified - p
void Initialize0538() {
    countdown0538 = step0538 = 0;
    angle0538 = 0;
}

// @verified - p
void Start0538(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0538, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0538_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Setup0538Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0538_CHARACTERS, parent->self);
}

// @verified - p
void Update0538(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0538AuthorTask, 4, 0x0b, sizeof(Area0538AuthorParameter), AUTHOR_VIEWER_TASK);
        Area0538AuthorParameter *const child = static_cast<Area0538AuthorParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->currentFrame = 0;
        EV_spmBeginSampleExecute();
        Prepare0514Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0538(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, INPUT_PRESSED)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(parent->selection - 1));
            if (parent->selection < 0)
                parent->selection = 0;
            break;
        case 128:
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(parent->selection + 1));
            if (parent->selection > 0)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0538A000.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0538(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0538_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    SetFirePosition(fire0538, 1, nullptr);
    SuspendParticleEffect(4, fire0538);
    FreeParticleEffect(4, fire0538);
    EV_FreeAuthorPak(0, AREA_0538_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Update0538Author(Area0538AuthorParameter *child) {
    Update0538FireLighting();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0514Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0538AuthorTask(void *taskData) {
    Update0538Author(static_cast<Area0538AuthorParameter *>(taskData));
}

// @verified - p
void Setup0538Scene() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0538_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_lightLoad(AREA_0538_LIGHT);
    PushGlobalLightingProgram();
    Create0538Fire();
    EV_SetObjectFlagByIndex(0, 0);
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(3, 0);
    EV_SetObjectFlagByIndex(6, 0);
    EV_SetObjectFlagByIndex(14, 0);
    HLib::EnqueueTaskWithoutParameter(Update0516Waves, 3, 2, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
}

// @verified - p
void Create0538Fire() {
    const Vector3 zero{};
    fire0538 = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.2f, 0.5f, 0, 4));
    SetFirePalette(fire0538, palette0538);
    SetFireFalloff(fire0538, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(fire0538, -0.2980392277240753f);
    SetParticleEffectSize(4, fire0538, 0.2f, 0.5f);
    ResumeParticleEffect(4, fire0538);
    AttachFireEffect(fire0538, AREA_0538_a287, 152, 0.1f);
    SetFirePosition(fire0538, 1, &zero);
    AttachFireInstance(fire0538, 1, AREA_0538_a288, 152, 0x3dcccccd);
}

// @verified - p
void Update0538FireLighting() {
    const float intensity = sin_fixed16(static_cast<std::int16_t>(angle0538)) * 0.05f + 0.699999988079071f;
    countdown0538 = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(countdown0538) - 1U);
    if (countdown0538 <= 0) {
        countdown0538 = shenGetRandomInt32(60) + 30;
        step0538 = shenGetRandomInt32(2048) + 2048;
    }
    angle0538 += static_cast<std::uint32_t>(step0538);
    Vector3 offset{-0.01f, 0.35f, -0.013f}, position;
    Vector3i rotation;
    EV_GetCharaPosition(AREA_0538_a287, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0538_a287, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    SetLightRuntimePosition(1, offset);
    SetLightSceneIntensity(1, intensity);
    offset = {-0.01f, 0.35f, -0.013f};
    EV_GetCharaPosition(AREA_0538_a288, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0538_a288, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    SetLightRuntimePosition(2, offset);
    SetLightSceneIntensity(2, intensity);
    PrioritizeLight(1);
    PrioritizeLight(2);
}

const ScriptAreaDefinition area0538ScriptDefinition{.areaId = AREA_0538, .field04 = 0, .init = Initialize0538, .main = Start0538};

} // namespace shenmue::s2::areas
