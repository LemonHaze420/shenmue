#include "s2/areas/area_exqt.hpp"
#include "s2/areas/area_dgct.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/input.hpp"
#include "s2/motion_work.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {
bool ReadExciteButtons(ExciteInput &input) {
    bool pressed = false;
    if (GetIsFreeInputType(-1)) {
        if (GetCurrentControlInput(7) == 1024) {
            input.pressed |= 0x200;
            pressed = true;
        }
        if (GetCurrentControlInput(7) == 512) {
            input.pressed |= 0x100;
            pressed = true;
        }
    } else {
        if (GetCurrentControlInput(7) == 1024) {
            input.pressed |= 0x100;
            pressed = true;
        }
        if (GetCurrentControlInput(7) == 512) {
            input.pressed |= 0x200;
            pressed = true;
        }
    }
    if (GetCurrentControlInput(7) == 4) {
        input.pressed |= 0x400;
        pressed = true;
    }
    if (GetCurrentControlInput(7) == 2) {
        input.pressed |= 0x800;
        pressed = true;
    }
    if (GetCurrentControlInput(7) == 32768) {
        input.pressed |= 4;
        pressed = true;
    }
    if (GetCurrentControlInput(7) == 16384) {
        input.pressed |= 64;
        pressed = true;
    }
    if (GetCurrentControlInput(7) == 4096) {
        input.pressed |= 1;
        pressed = true;
    }
    if (GetCurrentControlInput(7) == 8192) {
        input.pressed |= 16;
        pressed = true;
    }
    return pressed;
}
bool AdvanceExcitePrompt(ExciteInput &input) {
    if (input.elapsed >= input.window) {
        input.pressed = 0xa000;
        return true;
    }
    const std::uint32_t phase = input.elapsed % 5;
    Vector3 position{0, -100.46f, phase < 2 ? 0 : .015f};
    if (phase == 2)
        Loop3DSoundEffect(0xab);
    EV_scenApplyPosOrRotToPartOf(EXQT_MACHINE, static_cast<std::uint16_t>(input.part), &position, nullptr, EventCharacterPartTransformMode::Absolute);
    ++input.elapsed;
    return false;
}
void HideExcitePrompt(const ExciteInput &input) {
    Vector3 position{0, -100.46f, 0};
    EV_scenApplyPosOrRotToPartOf(EXQT_MACHINE, static_cast<std::uint16_t>(input.part), &position, nullptr, EventCharacterPartTransformMode::Absolute);
}
} // namespace
void SetExciteRequiredInput(ExciteInput &input, std::uint32_t mask) {
    input.required = mask;
    input.pressed = 0;
    input.elapsed = 0;
    switch (mask) {
    case 256:
        input.part = 183;
        return;
    case 512:
        input.part = 184;
        return;
    case 1024:
        input.part = 185;
        return;
    case 2048:
        input.part = 186;
        return;
    case 1:
    case 4:
    case 16:
    case 64:
        input.part = 187;
        break;
    default:
        return;
    }
    Vector3 position{0, -100.46f, 0};
    Vector3i rotation{0, 0, mask == 4 ? 49152 : mask == 64 ? 16384 : mask == 16 ? 32768 : 0};
    EV_scenApplyPosOrRotToPartOf(EXQT_MACHINE, 187, &position, &rotation, EventCharacterPartTransformMode::Absolute);
}
bool PollExciteInput(ExciteInput &input) {
    if (ReadExciteButtons(input))
        return true;
    if (g_forcePlayerGodmode && input.elapsed >= input.window)
        input.elapsed = input.window >> 1;
    return AdvanceExcitePrompt(input);
}
bool ConsumeExciteInput(ExciteInput &input) {
    HideExcitePrompt(input);
    const bool success = (input.pressed & input.required) != 0;
    Loop3DSoundEffect(success ? 0x900ab : 0xa00ab);
    return success;
}
bool PollGameCenterInput(ExciteInput &input) {
    const bool pressed = ReadExciteButtons(input);
    if (g_forcePlayerGodmode || pressed)
        return true;
    return AdvanceExcitePrompt(input);
}
bool ConsumeGameCenterInput(ExciteInput &input) {
    HideExcitePrompt(input);
    const bool success = (input.pressed & input.required) != 0 || g_forcePlayerGodmode;
    Loop3DSoundEffect(success ? 0x900ab : 0xa00ab);
    return success;
}
void SetExciteTiming(ExciteState &state, ExciteInput &input) {
    switch (state.tier) {
    case 0:
        state.patternScale = 3;
        state.timeScale = 8;
        break;
    case 1:
        state.patternScale = 4;
        state.timeScale = 8;
        break;
    case 2:
        state.patternScale = 6;
        state.timeScale = 8;
        break;
    case 3:
        state.patternScale = 8;
        state.timeScale = 6;
        break;
    case 4:
        state.patternScale = 8;
        state.timeScale = 5;
        break;
    case 5:
        state.patternScale = 8;
        state.timeScale = 4;
        break;
    case 6:
        state.patternScale = 8;
        state.timeScale = 3;
        break;
    case 7:
        state.patternScale = 8;
        state.timeScale = 2;
        break;
    default:
        state.patternScale = 8;
        state.timeScale = 1;
        break;
    }
    std::int32_t window, threshold;
    switch (state.tier) {
    case 0:
        window = 40 - input.attempts / 4;
        threshold = 30;
        break;
    case 1:
        window = 35 - input.attempts / 4;
        threshold = 30;
        break;
    case 2:
        window = 30 - input.attempts / 4;
        threshold = 30;
        break;
    case 3:
        window = 25 - input.attempts / 9;
        threshold = 50;
        break;
    case 4:
        window = 23 - input.attempts / 10;
        threshold = 50;
        break;
    case 5:
        window = 21 - input.attempts / 10;
        threshold = 50;
        break;
    case 6:
        window = 19 - input.attempts / 10;
        threshold = 50;
        break;
    default:
        window = 17 - input.attempts / 10;
        threshold = 50;
        break;
    }
    if (window <= 0)
        window = 1;
    input.window = window;
    state.game.waitTarget = window;
    if (input.attempts >= threshold) {
        input.attempts = -1;
        ++state.tier;
    }
}
void BuildExciteInput(ExciteState &state) {
    if (state.tier == 6 && state.patternStage == 9) {
        Loop3DSoundEffect(0x186);
        QueueExciteStageDisplay(state);
        state.patternStage = 10;
        state.patternScale = 1;
    } else if (state.tier == 3 && state.patternStage == 2) {
        Loop3DSoundEffect(0x186);
        QueueExciteStageDisplay(state);
        state.patternStage = 9;
        state.patternScale = 2;
    } else if (state.tier == 2 && state.patternStage == 1) {
        Loop3DSoundEffect(0x186);
        QueueExciteStageDisplay(state);
        state.patternStage = 2;
        state.patternScale = 5;
    }
    ExciteInput &input = state.controls.excite.input;
    SetExciteTiming(state, input);
    const std::uint32_t buttons[]{256, 512, 1024, 2048, 4, 64, 16, 1};
    float index = IncrementSharedPrngFloat() * state.patternScale;
    if (state.tier == 1)
        index += 4;
    SetExciteRequiredInput(input, buttons[static_cast<std::int32_t>(index)]);
    const float scale = state.timeScale, random = IncrementSharedPrngFloat();
    ++input.attempts;
    state.immediateAdvance = (scale - 1.0f) == random * scale;
}
void BuildGameCenterInput(ExciteState &state) {
    ExciteInput &input = state.controls.arcade.input;
    SetExciteTiming(state, input);
    const std::uint32_t buttons[]{256, 512, 1024, 2048, 4, 64, 16, 1};
    float index = IncrementSharedPrngFloat() * state.patternScale;
    if (state.tier == 1)
        index += 4;
    SetExciteRequiredInput(input, buttons[static_cast<std::int32_t>(index)]);
    const float scale = state.timeScale, random = IncrementSharedPrngFloat();
    ++input.attempts;
    state.immediateAdvance = (scale - 1.0f) == random * scale;
}
} // namespace shenmue::s2::areas
