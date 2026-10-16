#include "s2/areas/area_0545.hpp"
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

namespace shenmue::s2::areas {
namespace {
alignas(16) Matrix4x4 placement0545;
}

// @verified - p
void Initialize0545() {
    placement0545 = {};
}

// @verified - p
void Start0545(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0545, 4, 0x0b, sizeof(Area0545Parameter), AUTHOR_VIEWER_TASK);
    Area0545Parameter *const parent = static_cast<Area0545Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->input = reinterpret_cast<Area0545Input *>(argument);
    parent->author = parent->input->author;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0545");
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0545_CHARACTERS, task);
}

// @verified - p
void Update0545(void *taskData) {
    Area0545Parameter *const parent = static_cast<Area0545Parameter *>(taskData);
    if (parent->state == 1)
        Queue0545Author(parent);
    else if (parent->state == 3)
        Cleanup0545(parent);
}

// @verified - p
std::int64_t Queue0545Author(Area0545Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0545AuthorTask, 4, 0x0b, sizeof(Area0545AuthorParameter), AUTHOR_VIEWER_TASK);
    Area0545AuthorParameter *const child = static_cast<Area0545AuthorParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0545_AUTHORS[parent->author].name));
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(parent->input->position.x, parent->input->position.y, parent->input->position.z);
    const std::uint16_t yaw = static_cast<std::uint16_t>(parent->input->yaw);
    const std::uint16_t mirrored = yaw <= 0x8000U ? yaw : static_cast<std::uint16_t>(0U - yaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - mirrored));
    const float sine = sin_fixed16(static_cast<std::int16_t>(parent->input->yaw));
    ApplyShearToPrePassMatrix(sine, cosine);
    placement0545 = workTransformMatrix;
    Author_SetPlacementMatrix(&placement0545);
    Matrix4x4_Push();
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0545Author(Area0545AuthorParameter *child) {
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0)
        SetupAreaRyoSinAuthorCharacters();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 3;
        EnableCharDisplayAndSetMotionworkData(AREA_0545_RYO);
        EnableCharDisplayAndSetMotionworkData(AREA_0545_SIN);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0545AuthorTask(void *taskData) {
    Update0545Author(static_cast<Area0545AuthorParameter *>(taskData));
}

// @verified - p
void Cleanup0545(Area0545Parameter *parent) {
    for (const shen_name *character = AREA_0545_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, "0545");
    parent->input->author = -1;
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0545"), 0);
}

const ScriptAreaDefinition area0545ScriptDefinition{.areaId = AREA_0545, .field04 = 0, .init = Initialize0545, .main = Start0545};

} // namespace shenmue::s2::areas
