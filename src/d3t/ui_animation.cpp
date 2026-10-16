#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif



void UI_ReleaseAnimSlot(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiAnimSlots.size()))
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(index)];
    slot.allocated = 0;
    slot.interpolation = 0;
    slot.status = 0;
    slot.start[0] = 0.0f;
    slot.current[0] = 0.0f;
    slot.end[0] = 0.0f;
    slot.duration = 0.0f;
    slot.delay = 0.0f;
    slot.elapsed = 0.0f;
    slot.loopMode = 0;
    slot.reserved40 = -1;
}

std::int32_t UIAnimAllocSlot() {
    for (std::size_t index = 0; index < g_uiAnimSlots.size(); ++index) {
        UIAnimSlot &slot = g_uiAnimSlots[index];
        if (slot.allocated != 0)
            continue;
        slot.interpolation = 0;
        slot.status = 0;
        slot.start[0] = 0.0f;
        slot.current[0] = 0.0f;
        slot.end[0] = 0.0f;
        slot.duration = 0.0f;
        slot.delay = 0.0f;
        slot.elapsed = 0.0f;
        slot.loopMode = 0;
        slot.reserved40 = -1;
        slot.allocated = 1;
        return static_cast<std::int32_t>(index);
    }
    return -1;
}

// ActionSelector_SetSelectedChoice: integer lane 1 -> 0 after a 0.15 s delay.
void UIAnimConfigureSelectedChoice(std::int32_t slotIndex) {
    if (slotIndex < 0 || static_cast<std::size_t>(slotIndex) >= g_uiAnimSlots.size())
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 3;
    slot.start[0] = std::bit_cast<float>(1);
    slot.current[0] = std::bit_cast<float>(1);
    slot.end[0] = 0.0f;
    slot.delay = 0.15f;
    slot.elapsed = 0.0f;
    slot.duration = 0.001f;
    slot.reserved40 = -1;
}


void UIWidget_ClearBinding(std::int32_t index) {
    UIWidgetBinding &binding = g_uiWidgetBindings[static_cast<std::size_t>(index)];
#if SHENMUE_IS_S2
    if (binding.allocated) {
#else
    if (binding.allocated && binding.animSlot >= 0 && binding.animSlot < static_cast<std::int32_t>(g_uiAnimSlots.size())) {
#endif
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(binding.animSlot)];
        slot.allocated = 0;
        slot.interpolation = 0;
        slot.status = 0;
        slot.start[0] = 0.0f;
#if SHENMUE_IS_S2
        slot.current[0] = 0.0f;
#else
        slot.current = {};
#endif
        slot.end[0] = 0.0f;
        slot.duration = 0.0f;
        slot.delay = 0.0f;
        slot.elapsed = 0.0f;
        slot.loopMode = 0;
        slot.reserved40 = -1;
    }
    binding.allocated = 0;
    binding.animating = 0;
    binding.objectHandle = -1;
    binding.hidden = {};
    binding.shown = {};
    binding.property = 0;
    binding.objectKind = 0;
#if !SHENMUE_IS_S2
    g_uiWidgetBindingProperties[static_cast<std::size_t>(index)] = nullptr;
#endif
}


void UIAnim_Start(std::int32_t slotIndex, float from, float to, float duration, float delay) {
#if SHENMUE_IS_S2
    if (!(duration > 0.0f))
#else
    if (slotIndex == -1 || duration <= 0.0f)
#endif
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
    slot.start[0] = from;
    slot.current[0] = from;
    slot.end[0] = to;
    slot.duration = duration;
    slot.delay = delay;
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 2;
    slot.elapsed = 0.0f;
    slot.reserved40 = -1;
}

#if SHENMUE_IS_S2
// @verified - p
void UIAnim_StartPair(std::int32_t slotIndex, std::array<float, 2> from, std::array<float, 2> to, float duration, float delay) {
    if (!(duration > 0.0f))
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
    slot.duration = duration;
    slot.delay = delay;
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 1;
    slot.start = from;
    slot.current = from;
    slot.end = to;
    slot.elapsed = 0.0f;
    slot.reserved40 = -1;
}
#endif


void UIWidget_ApplyAnimations() {
#if SHENMUE_IS_S2
    for (UIWidgetBinding &binding : g_uiWidgetBindings) {
        if (binding.allocated != 1 || !binding.animating)
            continue;
        d3t::UIWindow *window = nullptr;
        UIScrollList *scroll = nullptr;
        UIOptionsList *options = nullptr;
        UIColourImageWidget *image = nullptr;
        switch (binding.objectKind) {
        case 0:
            window = UI_GetWindow(binding.objectHandle);
            if (!window)
                continue;
            break;
        case 1:
        case 2:
            if (static_cast<std::uint32_t>(binding.objectHandle) < g_uiScrollLists.size()) {
                scroll = &g_uiScrollLists[static_cast<std::size_t>(binding.objectHandle)];
                if (binding.objectKind == 1)
                    window = UI_GetWindow(scroll->window);
            }
            break;
        case 3:
            if (static_cast<std::uint32_t>(binding.objectHandle) < g_uiOptionsLists.size())
                options = &g_uiOptionsLists[static_cast<std::size_t>(binding.objectHandle)];
            break;
        case 4:
            if (static_cast<std::uint32_t>(binding.objectHandle) < g_uiColourImageWidgets.size())
                image = &g_uiColourImageWidgets[static_cast<std::size_t>(binding.objectHandle)];
            break;
        }
        const UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(binding.animSlot)];
        switch (binding.property) {
        case 0:
            if (window)
                window->alpha = slot.current[0];
            else if (image)
                image->alpha = slot.current[0];
            break;
        case 1:
            if (binding.objectKind == 0) {
                window->layout.offsetX = slot.current[0];
                window->layout.offsetY = slot.current[1];
            }
            break;
        case 2:
            if (binding.objectKind == 0)
                window->layout.offsetY = slot.current[0];
            else if (scroll && binding.objectKind == 1)
                scroll->field3A4 = std::bit_cast<std::int32_t>(slot.current[0]);
            else if (scroll && binding.objectKind == 2)
                scroll->field3CC = std::bit_cast<std::int32_t>(slot.current[0]);
            else if (options)
                options->field74[1] = std::bit_cast<std::int32_t>(slot.current[0]);
            break;
        case 3:
            if (binding.objectKind == 0)
                window->layout.offsetX = slot.current[0];
            break;
        }
        if (!slot.status)
            binding.animating = 0;
    }
#else
    for (std::size_t index = 0; index < g_uiWidgetBindings.size(); ++index) {
        UIWidgetBinding &binding = g_uiWidgetBindings[index];
        if (binding.allocated != 1 || !binding.animating || binding.objectKind != 0 || !g_uiWidgetBindingProperties[index])
            continue;
        const UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(binding.animSlot)];
        if (binding.property == 0)
            *g_uiWidgetBindingProperties[index] = slot.current[0];
        if (!slot.status)
            binding.animating = 0;
    }
#endif
}

#if SHENMUE_IS_S2
// @verified - p
std::int32_t UIWidget_Bind(std::int32_t objectHandle, std::int32_t objectKind, std::int32_t property, std::array<float, 2> hidden, std::array<float, 2> shown) {
    if (objectHandle == -1)
        return -1;
    for (std::size_t index = 0; index < g_uiWidgetBindings.size(); ++index) {
        UIWidgetBinding &binding = g_uiWidgetBindings[index];
        if (binding.allocated)
            continue;
        binding.hidden = hidden;
        binding.shown = shown;
        binding.objectHandle = objectHandle;
        binding.property = property;
        binding.objectKind = objectKind;
        binding.allocated = 1;
        binding.animating = 0;
        binding.animSlot = UIAnimAllocSlot();
        return static_cast<std::int32_t>(index);
    }
    return -1;
}
#else


std::int32_t UIWidget_Bind(std::int32_t objectHandle, float *property, std::array<float, 2> hidden, std::array<float, 2> shown) {
    if (objectHandle == -1)
        return -1;
    for (std::size_t index = 0; index < g_uiWidgetBindings.size(); ++index) {
        UIWidgetBinding &binding = g_uiWidgetBindings[index];
        if (binding.allocated)
            continue;
        UIWidget_ClearBinding(static_cast<std::int32_t>(index));
        binding.allocated = 1;
        binding.shown = shown;
        binding.hidden = hidden;
        binding.objectHandle = objectHandle;
        binding.animSlot = UIAnimAllocSlot();
        g_uiWidgetBindingProperties[index] = property;
        return static_cast<std::int32_t>(index);
    }
    return -1;
}
#endif


void UIWidget_Animate(std::int32_t index, std::int32_t direction, float duration, float delay) {
    if (index == -1)
        return;
    UIWidgetBinding &binding = g_uiWidgetBindings[static_cast<std::size_t>(index)];
#if SHENMUE_IS_S2
    std::array<float, 2> from = binding.hidden;
    std::array<float, 2> to = binding.shown;
    if (direction == 0) {
        from = binding.shown;
        to = binding.hidden;
    } else if (direction == 2) {
        from = {-binding.hidden[0], -binding.hidden[1]};
        to = {-binding.shown[0], -binding.shown[1]};
    }
    d3t::UIWindow *window = nullptr;
    UIScrollList *scroll = nullptr;
    UIOptionsList *options = nullptr;
    switch (binding.objectKind) {
    case 0:
        window = UI_GetWindow(binding.objectHandle);
        break;
    case 1:
    case 2:
        if (static_cast<std::uint32_t>(binding.objectHandle) < g_uiScrollLists.size()) {
            scroll = &g_uiScrollLists[static_cast<std::size_t>(binding.objectHandle)];
            window = UI_GetWindow(scroll->window);
        }
        break;
    case 3:
        if (static_cast<std::uint32_t>(binding.objectHandle) < g_uiOptionsLists.size()) {
            options = &g_uiOptionsLists[static_cast<std::size_t>(binding.objectHandle)];
            window = UI_GetWindow(options->window);
        }
        break;
    }
    switch (binding.property) {
    case 0:
        UIAnim_Start(binding.animSlot, from[0], to[0], duration, delay);
        if (binding.objectKind == 0 && window)
            window->alpha = from[0];
        break;
    case 1:
        UIAnim_StartPair(binding.animSlot, from, to, duration, delay);
        if (binding.objectKind == 0 && window) {
            window->layout.offsetY = from[1];
            window->layout.offsetX = from[0];
        }
        break;
    case 2:
        UIAnim_Start(binding.animSlot, from[0], to[0], duration, delay);
        if (binding.objectKind == 0 && window)
            window->layout.offsetY = from[0];
        else if (scroll && binding.objectKind == 1)
            scroll->field3A4 = std::bit_cast<std::int32_t>(from[0]);
        else if (scroll && binding.objectKind == 2)
            scroll->field3CC = std::bit_cast<std::int32_t>(from[0]);
        else if (options)
            options->field74[1] = std::bit_cast<std::int32_t>(from[0]);
        break;
    case 3:
        UIAnim_Start(binding.animSlot, from[0], to[0], duration, delay);
        if (binding.objectKind == 0 && window)
            window->layout.offsetX = from[0];
        break;
    }
#else
    float from = binding.hidden[0];
    float to = binding.shown[0];
    if (direction == 0) {
        from = binding.shown[0];
        to = binding.hidden[0];
    } else if (direction == 2) {
        from = -binding.hidden[0];
        to = -binding.shown[0];
    }
    UIAnim_Start(binding.animSlot, from, to, duration, delay);
    if (binding.property == 0 && g_uiWidgetBindingProperties[static_cast<std::size_t>(index)])
        *g_uiWidgetBindingProperties[static_cast<std::size_t>(index)] = from;
#endif
    binding.animating = 1;
}


void UIAnim_UpdateDeltaTime(float deltaTime) {
    for (UIAnimSlot &slot : g_uiAnimSlots) {
        if (!slot.allocated || slot.status <= 0)
            continue;
        const float elapsed = deltaTime + slot.elapsed;
        slot.elapsed = elapsed;
        if (slot.status != 1) {
            slot.status = 0;
            continue;
        }
        float progress = (elapsed - slot.delay) / slot.duration;
        progress = 0.0f > progress ? 0.0f : 1.0f < progress ? 1.0f : progress;
        if (slot.delay >= elapsed)
            progress = 0.0f;
        switch (slot.interpolation) {
        case 0:
        case 2:
            slot.current[0] = (1.0f - progress) * slot.start[0] + progress * slot.end[0];
            break;
        case 1:
#if SHENMUE_IS_S2
        {
            const float x = (1.0f - progress) * slot.start[0] + progress * slot.end[0];
            slot.current[1] = (1.0f - progress) * slot.start[1] + progress * slot.end[1];
            slot.current[0] = x;
        }
#else
            slot.current[1] = progress * slot.end[1] + (1.0f - progress) * slot.start[1];
            slot.current[0] = progress * slot.end[0] + (1.0f - progress) * slot.start[0];
#endif
        break;
        case 3: {
            const float start = static_cast<float>(std::bit_cast<std::int32_t>(slot.start[0]));
            const float end = static_cast<float>(std::bit_cast<std::int32_t>(slot.end[0]));
            slot.current[0] = std::bit_cast<float>(static_cast<std::int32_t>(start * (1.0f - progress) + end * progress));
            break;
        }
        default:
            break;
        }
#if SHENMUE_IS_S2
        if (!(elapsed >= slot.duration + slot.delay))
#else
        if (elapsed < slot.duration + slot.delay)
#endif
            continue;
        slot.status = 2;
        slot.delay = 0.0f;
        if (slot.loopMode == 1) {
            slot.status = 1;
            slot.elapsed = 0.0f;
            continue;
        }
        if (slot.loopMode == 2) {
            slot.elapsed = 0.0f;
        } else if (slot.loopMode == 3) {
            slot.elapsed = 0.0f;
            slot.loopMode = 0;
        } else {
            continue;
        }
        std::swap(slot.start, slot.end);
        slot.status = 1;
    }
    UIWidget_ApplyAnimations();
}

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
