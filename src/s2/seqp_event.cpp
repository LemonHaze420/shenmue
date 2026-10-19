#include "s2/seqp_event.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include "s2/save.hpp"
#include <bit>
#include <cstring>
namespace shenmue::s2 {
namespace {
template <class T> T SequenceParameter(const CompiledSequenceWork *work, std::size_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(work) + offset, sizeof(value));
    return value;
}
void AdvanceSequence(CompiledSequenceWork *work, std::uint16_t next) {
    work->previousMode = work->mode;
    work->mode = next;
    work->continueLoop = work->continueAfterWait;
}
}

CompiledSequenceWork *QueueCompiledSequence(HLib::TaskCallback callback, shen_name token, void *event) {
    auto *task = HLib::EnqueueTaskWithParameter(callback, 4, 0x0b, sizeof(CompiledSequenceWork), SHEN_NAME(EVNT));
    auto *work = static_cast<CompiledSequenceWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->event = event;
    work->mode = 0;
    work->nextMode = 0;
    work->waitCharacter = shen_name{};
    work->delay = 0;
    work->reference = 0;
    work->token = token;
    work->reserved1a0 = 0;
    work->parent = nullptr;
    work->flags8c = 0;
    for (std::size_t i = 0; i < 10; ++i) {
        work->children[i] = nullptr;
        std::uint32_t zero = 0;
        std::memcpy(work->parameters + 4 * i, &zero, sizeof(zero));
    }
    return work;
}

void RetireCompiledSequenceLinks(CompiledSequenceWork *work) {
    if (work->parent) {
        work->parent->children[work->childSlot] = nullptr;
        work->parent = nullptr;
    }
    for (auto &child : work->children)
        if (child) {
            child->parent = nullptr;
            child = nullptr;
        }
}
// @verified - p
void CompiledLcdCountdownCallback(void *data) {
    CompiledSequenceWork *const work = static_cast<CompiledSequenceWork *>(data);
    std::byte *const event = static_cast<std::byte *>(work->event);
    work->callbackFlag = 0;
    do {
        work->continueLoop = 0;
        while (work->mode == 0) {
            const std::uint32_t frames = 1;
            const void *const bitmap = nullptr;
            // DLCD uses these borrowed offsets even when KEW0 supplies its smaller event allocation.
            std::memcpy(event + COMPILED_LCD_FRAMES_OFFSET, &frames, sizeof(frames));
            std::memcpy(event + COMPILED_LCD_BITMAP_OFFSET, &bitmap, sizeof(bitmap));
            work->previousMode = work->mode;
            work->continueAfterWait = 1;
            work->mode = 100;
            work->continueLoop = 1;
        }
        work->continueLoop = 0;
        if (work->mode == 100) {
            std::uint32_t frames;
            std::memcpy(&frames, event + COMPILED_LCD_FRAMES_OFFSET, sizeof(frames));
            if (!frames)
                return;
            --frames;
            std::memcpy(event + COMPILED_LCD_FRAMES_OFFSET, &frames, sizeof(frames));
            if (!frames) {
                const void *const bitmap = nullptr;
                std::memcpy(event + COMPILED_LCD_BITMAP_OFFSET, &bitmap, sizeof(bitmap));
                LcdmInit(0);
            }
        } else
            UpdateCompiledSequenceWait(work);
    } while (work->continueLoop);
}

std::int64_t UpdateCompiledSequenceWait(CompiledSequenceWork *work) {
    switch (work->mode) {
    case 1:
        RetireCompiledSequenceLinks(work);
        HLib::DestroyCurrentTask();
        return 0;
    case 2: {
        const auto old = work->delay;
        work->delay = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(old) - 1);
        if (old != 1)
            return 0;
        break;
    }
    case 3:
        if (camCtrlCheckIsEventCam())
            return 0;
        break;
    case 4:
        if (work->delay) {
            --work->delay;
            return 0;
        }
        if (cvsHasValidConverseData(0))
            return 0;
        ctrlStopSubtitles();
        break;
    case 5:
        if (!SetMotionRegionFlag(scenFindMotionwork(work->waitCharacter), 65, 1, MotionRegion::LowerBody))
            return 0;
        break;
    case 6:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return 0;
        {
            auto choice = shenGetCtrlSpeakIconFlag();
            work->choice = choice == 0xffff ? 0 : static_cast<std::int32_t>(choice);
        }
        break;
    case 7:
        if (!scenMotionworkUtil(SequenceParameter<shen_name>(work, 0xe0), SequenceParameter<std::uint32_t>(work, 0xf8), SequenceParameter<float>(work, 0x100),
                SequenceParameter<std::int32_t>(work, 0x108), SequenceParameter<const void *>(work, 0xf0)))
            return 0;
        break;
    case 8:
        if (!SequenceParameter<std::uint32_t>(work, 0x90 + 4 * work->resultSlot))
            return 0;
        break;
    case 9:
        if (!SequenceParameter<std::uint32_t>(work, 0x90 + 4 * work->resultSlot))
            return 0;
        AdvanceSequence(work, SequenceParameter<std::uint16_t>(work, 0xb8 + 4 * work->resultSlot));
        return 1;
    case 10:
        if (work->children[work->waitChildIndex])
            return 0;
        break;
    case 11:
        if (work->delay) {
            --work->delay;
            return 0;
        }
        if (filmCheckFadeTrackColour())
            return 0;
        break;
    case 12:
        if (IsFightActive())
            return 0;
        break;
    case 13:
        if (work->delay) {
            --work->delay;
            return 0;
        }
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            return 0;
        break;
    default:
        return 0;
    }
    AdvanceSequence(work, work->nextMode);
    return 1;
}

void ResetAuthorCharacterForSequence(shen_name character) {
    const Vector4 values{1, .3f, .3f, .3f};
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
    SetScenarioReflectorBoardValues(character, &values);
    EV_SetRefBoardParamsFor(character, 1);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 0, 8, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 1, 8, 16);
}
}
