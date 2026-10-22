#include "s2/areas/area_0527.hpp"
#include "s2/input.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/author.hpp"
#include "s2/game_runtime.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/maze.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include "s2/npc.hpp"
#include <bit>
#include <limits>

namespace shenmue::s2::areas {
namespace {
std::int32_t debug0527;
Matrix4x4 placement0527;
MotionWork *Motion0527(shen_name character) {
    return FindMotionWork(FindCharacterByScenarioName(character));
}
void Set0527Placement(const Area0527Request &request) {
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(request.position.x, request.position.y, request.position.z);
    const std::int16_t angle = static_cast<std::int16_t>(request.yaw <= 32768U ? request.yaw : 0U - request.yaw);
    ApplyShearToPrePassMatrix(sin_fixed16(static_cast<std::int16_t>(request.yaw)), sin_fixed16(static_cast<std::int16_t>(16384 - angle)));
    placement0527 = workTransformMatrix;
    Author_SetPlacementMatrix(&placement0527);
    Matrix4x4::Pop(1);
}
bool FadeAfter0527(std::int32_t author) {
    switch (author) {
    case 2:
    case 6:
    case 9:
    case 11:
    case 23:
    case 24:
    case 32:
        return false;
    default:
        return true;
    }
}
void Show0527Choice(std::int32_t author) {
    EVT_AT_KsIcon_Enqueue(-1, static_cast<std::int16_t>(author == 9 ? 69 : 398), static_cast<std::int16_t>(author == 9 ? 68 : 399), -1, 10800, 65535, 2, 0);
}
void Restore0527CharacterMotion(shen_name character, std::uint16_t animation, const Area0527Request &request) {
    MotionWork *const motion = Motion0527(character);
    Vector3i rotation{};
    GetScenarioCharacterEulerAngles(character, &rotation, EventTransformFlags::World | EventTransformFlags::MotionYaw);
    Vector3 sampledPosition{};
    Vector3i startRotation{};
    SampleNativeMotionRootAndRotation(
        GetMotionAnimationId(motion, MotionRegion::LowerBody), GetMotionRigBoneId(motion), 0.0f, &sampledPosition, &startRotation);
    const float frame = GetMotionFrame(motion, MotionRegion::LowerBody);
    Vector3i endRotation{};
    SampleNativeMotionRootAndRotation(GetMotionAnimationId(motion, MotionRegion::LowerBody), GetMotionRigBoneId(motion), frame, &sampledPosition, &endRotation);
    Vector3 position{};
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    mtwkRequestMotion(motion, animation, MotionRegion::LowerBody);
    const float height = position.y;
    position.y = 100.0f;
    position.y = shenFindIntersectionWithScene(position, frame);
    if (position.y == -std::numeric_limits<float>::max())
        position.y = height;
    rotation = Vector3i{0, static_cast<std::int32_t>(static_cast<std::uint32_t>(endRotation.y) + request.yaw - static_cast<std::uint32_t>(startRotation.y)), 0};
    EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
    mtwkSetExtraFlag(motion, 17, 0);
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
}
} // namespace
void Initialize0527() {
    debug0527 = 0;
    placement0527 = Matrix4x4{};
}
void Start0527(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0527, 4, 11, sizeof(Area0527Parameter), AREA_0527_EVENT);
    Area0527Parameter *const parent = static_cast<Area0527Parameter *>(HLib::GetTaskParameter(task));
    debug0527 = 0;
    parent->self = task;
    parent->state = 1;
    parent->request = reinterpret_cast<Area0527Request *>(argument);
    parent->author = parent->request->author;
    parent->choiceCompleted = 0;
}
void Update0527(void *data) {
    Area0527Parameter *const parent = static_cast<Area0527Parameter *>(data);
    if (parent->state == 1)
        Queue0527Author(parent);
    else if (parent->state == 3)
        Cleanup0527(parent);
}
void Queue0527Author(Area0527Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0527Author, 4, 11, sizeof(Area0527AuthorParameter), AREA_0527_EVENT);
    Area0527AuthorParameter *const child = static_cast<Area0527AuthorParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->frame = 0;
    MotionWork *const motion = Motion0527(AREA_0527_SIN);
    CopyMotionTransition(motion, nullptr, 0, 1, 0);
    mtwkSetExtraFlag(motion, 17, 0);
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(motion, 13, 0);
    switch (parent->author) {
    case 2:
    case 9:
    case 11:
    case 12:
    case 14:
    case 15:
    case 23:
    case 24:
    case 25:
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        break;
    default:
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        break;
    }
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0527_AUTHORS[parent->author].name));
    if (parent->author == 2)
        child->choiceFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 16);
    else if (parent->author == 6)
        child->choiceFrame = 950;
    else if (parent->author == 9 || parent->author == 23)
        child->choiceFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 31);
    if (parent->author == 26) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::Task *const fadeTask = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 11, sizeof(EventFilmControl), AREA_0527_EVENT);
        RegisterEventTask(fadeTask);
        EventFilmControl *const film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(fadeTask));
        *film = EventFilmControl{1, 15, 255, 0, 0, 0, 0, 0, 0, 0, 10.0f};
    } else
        SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
    Set0527Placement(*parent->request);
    parent->state = 2;
}
void Prepare0527Character(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    MotionWork *const motion = Motion0527(character);
    mtwkSetExtraFlag(motion, 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
    mtwkSetExtraFlag(motion, 17, 0);
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(motion, 13, 0);
}
void Prepare0527Characters() {
    Prepare0527Character(AREA_0527_RYO);
    Prepare0527Character(AREA_0527_SIN);
    for (const shen_name prop : AREA_0527_PROPS) {
        if (prop.name == 0xffffffffU)
            break;
        EV_EnableCharacterDisplay(prop);
        EV_SetCharaControllerFlags(prop, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(prop, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(prop, false);
    }
}
void Restore0527Characters() {
    EnableCharDisplayAndSetMotionworkData(AREA_0527_RYO);
    EnableCharDisplayAndSetMotionworkData(AREA_0527_SIN);
    for (const shen_name prop : AREA_0527_PROPS) {
        if (prop.name == 0xffffffffU)
            break;
        EV_EnableCharacterDisplay(prop);
        EV_SetCharaControllerFlags(prop, 1, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(prop, 2, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(prop, 3, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(prop, true);
    }
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(std::uintptr_t{1}));
}
void Update0527Author(void *data) {
    Area0527AuthorParameter *const child = static_cast<Area0527AuthorParameter *>(data);
    Area0527Parameter *const parent = child->parent;
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->frame == 0)
        Prepare0527Characters();
    if (parent->author == 1) {
        const bool enabled = SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(~std::uintptr_t{})) != 0;
        if (static_cast<std::uint32_t>(static_cast<std::int32_t>(child->frame) - 850) <= 228U) {
            if (enabled)
                SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        } else if (!enabled) {
            SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(std::uintptr_t{1}));
            EV_DisableCharacterDisplay(AREA_0527_CHOICE_PROP);
        }
    }
    if (child->choiceFrame == child->frame && (parent->author == 2 || parent->author == 9 || parent->author == 23)) {
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        Show0527Choice(parent->author);
        HLib::SetCurrentTaskTo(Wait0527Choice);
    }
    if (child->choiceFrame == child->frame && parent->author == 6)
        SetMazeHourAnimationFrame(0);
    if (EVT_GetControllerInput(2, INPUT_PRESSED) & 0x80U)
        debug0527 = debug0527 == 0;
    if (EVT_GetControllerInput(2, INPUT_HELD) & PAD_LEFT)
        parent->request->position.x -= 0.1f;
    if (EVT_GetControllerInput(2, INPUT_HELD) & PAD_RIGHT)
        parent->request->position.x += 0.1f;
    if (EVT_GetControllerInput(2, INPUT_HELD) & PAD_UP)
        parent->request->position.z -= 0.1f;
    if (EVT_GetControllerInput(2, INPUT_HELD) & PAD_DOWN)
        parent->request->position.z += 0.1f;
    if (EVT_GetControllerInput(2, INPUT_HELD) & 0x400U)
        parent->request->yaw -= 182;
    if (EVT_GetControllerInput(2, INPUT_HELD) & 0x200U)
        parent->request->yaw += 182;
    if (EVT_GetControllerInput(2, INPUT_HELD) & 0x4U)
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
    if (EVT_GetControllerInput(2, INPUT_HELD) & 0x2U)
        EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
    if (debug0527) {
        EV_set_debug_text_pos(30, 21);
        EV_debug_log_to_screen("AA:%6.3f", parent->request->yaw / 182.04445f);
        EV_set_debug_text_pos(30, 22);
        EV_debug_log_to_screen("AX:%6.3f", parent->request->position.x);
        EV_set_debug_text_pos(30, 23);
        EV_debug_log_to_screen("AZ:%6.3f", parent->request->position.z);
    }
    Set0527Placement(*parent->request);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Restore0527Motions(child);
        return;
    }
    if (authGetFinished() && !parent->choiceCompleted && (parent->author == 2 || parent->author == 9 || parent->author == 23)) {
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        Show0527Choice(parent->author);
        HLib::SetCurrentTaskTo(Finish0527Choice);
    }
}
void Wait0527Choice(void *data) {
    Area0527AuthorParameter *const child = static_cast<Area0527AuthorParameter *>(data);
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
        HLib::SetCurrentTaskTo(Update0527Author);
        child->parent->choiceCompleted = 1;
    }
}
void Finish0527Choice(void *data) {
    Area0527AuthorParameter *const child = static_cast<Area0527AuthorParameter *>(data);
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        Author_RequestAbort();
        child->parent->choiceCompleted = 1;
        Restore0527Motions(child);
    }
}
void Restore0527Motions(Area0527AuthorParameter *child) {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(std::uintptr_t{1}));
    Restore0527CharacterMotion(AREA_0527_RYO, 0xf003, *child->parent->request);
    Restore0527CharacterMotion(AREA_0527_SIN, 0x9503, *child->parent->request);
    Restore0527Characters();
    child->parent->state = 3;
    HLib::DestroyCurrentTask();
}
void Cleanup0527(Area0527Parameter *parent) {
    parent->request->author = -1;
    if (FadeAfter0527(parent->author))
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
}
const ScriptAreaDefinition area0527ScriptDefinition{AREA_0527, 0, Initialize0527, Start0527};
} // namespace shenmue::s2::areas
