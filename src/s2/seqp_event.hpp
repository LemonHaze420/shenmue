#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {

struct CompiledSequenceWork {
    std::uint16_t mode, nextMode, reserved04, previousMode;
    shen_name token;
    std::int32_t continueLoop, continueAfterWait, reference, childSlot, resultSlot, waitChildIndex;
    std::uint32_t reserved24;
    HLib::Task *task;
    CompiledSequenceWork *parent;
    CompiledSequenceWork *children[10];
    std::uint32_t reserved88, flags8c;
    std::byte parameters[0xa0];
    void *event;
    std::byte reserved138[0x54];
    std::int32_t choice;
    shen_name waitCharacter;
    std::int32_t delay, callbackFlag, timer, reserved1a0;
    std::byte extension[0xcc];
};
static_assert(sizeof(CompiledSequenceWork) == 0x270);
static_assert(offsetof(CompiledSequenceWork, parent) == 0x30 && offsetof(CompiledSequenceWork, event) == 0x130);
static_assert(offsetof(CompiledSequenceWork, delay) == 0x194 && offsetof(CompiledSequenceWork, extension) == 0x1a4);
inline constexpr std::size_t COMPILED_LCD_FRAMES_OFFSET = 0x630, COMPILED_LCD_BITMAP_OFFSET = 0x638;
void CompiledLcdCountdownCallback(void *data);
CompiledSequenceWork *QueueCompiledSequence(HLib::TaskCallback callback, shen_name token, void *event);
void RetireCompiledSequenceLinks(CompiledSequenceWork *work);
std::int64_t UpdateCompiledSequenceWait(CompiledSequenceWork *work);
void ResetAuthorCharacterForSequence(shen_name character);
}
