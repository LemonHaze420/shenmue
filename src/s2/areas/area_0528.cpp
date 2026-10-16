#include "s2/areas/area_0528.hpp"
#include "s2/input.hpp"
#include "s2/sound_program.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
alignas(16) Matrix4x4 placement0528;
}

// @verified - p
void Initialize0528() {
    placement0528 = {};
}

// @verified - p
void Start0528(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0528, 4, 0x0b, sizeof(Area0528Parameter), AUTHOR_VIEWER_TASK);
    Area0528Parameter *const parent = static_cast<Area0528Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->input = reinterpret_cast<Area0528Input *>(argument);
    parent->author = parent->input->author;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0528");
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0528_CHARACTERS, task);
}

// @verified - p
void Update0528(void *taskData) {
    Area0528Parameter *const parent = static_cast<Area0528Parameter *>(taskData);
    if (parent->state == 1)
        Queue0528Author(parent);
    else if (parent->state == 3)
        Cleanup0528(parent);
    else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            parent->author = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(parent->author) - 1U);
            if (parent->author < 0)
                parent->author = 1;
            break;
        case 128:
            parent->author = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(parent->author) + 1U);
            if (parent->author > 1)
                parent->author = 0;
            break;
        default:
            break;
        }
        if (parent->author == 0 || parent->author == 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(parent->author == 0 ? "0528E010.c\n" : "0528E020.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->author);
    }
}

// @verified - p
std::int64_t Queue0528Author(Area0528Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0528AuthorTask, 4, 0x0b, sizeof(Area0528AuthorParameter), AUTHOR_VIEWER_TASK);
    Area0528AuthorParameter *const child = static_cast<Area0528AuthorParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0528_AUTHORS[parent->author].name));
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(parent->input->position.x, parent->input->position.y, parent->input->position.z);
    const std::uint16_t yaw = static_cast<std::uint16_t>(parent->input->yaw);
    const std::uint16_t mirrored = yaw <= 0x8000U ? yaw : static_cast<std::uint16_t>(0U - yaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - mirrored));
    const float sine = sin_fixed16(static_cast<std::int16_t>(parent->input->yaw));
    ApplyShearToPrePassMatrix(sine, cosine);
    placement0528 = workTransformMatrix;
    Author_SetPlacementMatrix(&placement0528);
    Matrix4x4_Push();
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0528Author(Area0528AuthorParameter *child) {
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0)
        SetupAreaRyoSinAuthorCharacters();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 3;
        EnableCharDisplayAndSetMotionworkData(AREA_0528_RYO);
        EnableCharDisplayAndSetMotionworkData(AREA_0528_SIN);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0528AuthorTask(void *taskData) {
    Update0528Author(static_cast<Area0528AuthorParameter *>(taskData));
}

// @verified - p
void Cleanup0528(Area0528Parameter *parent) {
    for (const shen_name *character = AREA_0528_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, "0528");
    parent->input->author = -1;
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0528"), 0);
}

const ScriptAreaDefinition area0528ScriptDefinition{.areaId = AREA_0528, .field04 = 0, .init = Initialize0528, .main = Start0528};

} // namespace shenmue::s2::areas
