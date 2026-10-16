#include "s2/areas/area_wbbk.hpp"
#include "s2/areas/wbbk_dialogue.hpp"
#include "s2/areas/wbbk_shadow.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
namespace shenmue::s2::areas {
std::uint8_t bookAvailableStacks[14];
void BookIdleInput() {
    std::int32_t selected = -1;
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(BOOK_RYO, &rotation, EventTransformFlags::Local);
    if (position.x <= 344.28f || position.x >= 350.60001f || position.z <= 230.28999f || position.z >= 233.36f) {
        for (std::int32_t index = 0; index < 14; ++index)
            bookSearchTargets[index].flags |= 2;
    } else {
        SetPlayerGameFlags(PlayerGameFlagMode::Bit29, reinterpret_cast<void *>(std::uintptr_t{1}));
        for (std::int32_t index = 0; index < 14; ++index) {
            if (bookAvailableStacks[index])
                bookSearchTargets[index].flags &= ~2U;
            else
                bookSearchTargets[index].flags |= 2;
        }
    }
    if (static_cast<std::int16_t>(EVT_GetControllerInput(0, 0)))
        bookCarryIdleFrames = 0;
    DateTime time;
    GetCurrentDateTime(time);
    if (srchHasActiveLockonTarget() && time.hour >= 12)
        srchRequestLockonExit();
    if (ryoNotBusy()) {
        if (IsBookTimeExpired()) {
            mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), 0xf002, MotionRegion::LowerBody);
            mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_RYO), 1);
            SetMotionFrameTime(scenFindMotionwork(BOOK_RYO), 20, MotionRegion::LowerBody);
        }
        if (bookCarryIdleFrames++ > 900) {
            QueueBookDialogue(8, 0);
            bookCarryIdleFrames = 0;
        }
    }
    shenUpdateFlagEvaluation(0);
    EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (position.x < 331.73001f && position.z > 236.91f && ryoNotBusy()) {
        SetPlayerControlEnabled(0);
        ChangeBookEventState(6);
    }
    EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(BOOK_RYO, &rotation, EventTransformFlags::Local);
    float nearest = 99;
    for (std::int32_t index = 0; bookStackPlacements[index].position.y > 0; ++index) {
        const BookStackPlacement &stack = bookStackPlacements[index];
        const float distance = GetEuclideanDistance(&position, &stack.position);
        if (distance > 1.8f)
            continue;
        std::uint16_t approach =
            static_cast<std::uint16_t>(stack.yaw + 32768 - InverseFixedAtan2(position.x - stack.position.x, position.z - stack.position.z));
        if (approach >= 32768)
            approach = static_cast<std::uint16_t>(-approach);
        if (approach >= 0x38e3)
            continue;
        std::uint16_t facing = static_cast<std::uint16_t>(rotation.y - InverseFixedAtan2(stack.position.x - position.x, stack.position.z - position.z));
        if (facing >= 32768)
            facing = static_cast<std::uint16_t>(-facing);
        if (facing < 0x2e38 && ryoNotBusy() && bookAvailableStacks[index] && nearest > distance) {
            nearest = distance;
            selected = index;
        }
    }
    const shen_name focus = srchGetTargetName(0);
    if ((focus.name & 0xffffffU) == (SHEN_NAME(EV0A) & 0xffffffU))
        selected = static_cast<std::int32_t>((focus.name - SHEN_NAME(EV0A)) >> 24);
    if (static_cast<std::uint32_t>(selected) <= 13U && bookAvailableStacks[selected]) {
        GetBattleDataConfig(0, SHEN_NAME(BTBA));
        if (!GetBattleDataConfig(2, SHEN_NAME(BTBA)))
            return;
        SetPlayerControlEnabled(0);
        bookModelState.currentStack = selected;
        bookAvailableStacks[selected] = 0;
        bookCarryPhase = 7;
        bookSearchTargets[selected].flags |= 2;
        if (srchGetTargetName(0).name) {
            srchReset();
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(CameraState::Init);
            bookPlayerAlpha = 0;
            HLib::EnqueueTaskWithoutParameter(BookPlayerAlphaCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
            EV_SetCharacterAlphaEnabled(BOOK_RYO, true);
            ChangeBookEventState(4);
        } else
            ChangeBookEventState(3);
        BindBookStackShadow(bookModelState.stacks[bookModelState.currentStack + 1]);
    }
}
} // namespace shenmue::s2::areas
