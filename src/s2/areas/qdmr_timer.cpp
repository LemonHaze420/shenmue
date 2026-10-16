#include "s2/areas/area_qdmr.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {
void QdmrTimerCallback(void *parameter) {
    HLib::Task **const parent = static_cast<HLib::Task **>(parameter);
    if (!parent || !*parent) {
        HLib::DestroyCurrentTask();
        return;
    }
    QdmrEventWork *const work = static_cast<QdmrEventWork *>(HLib::GetTaskParameter(*parent));
    if (!work || !work->timerText) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->timer > 0 && !GetShouldRenderIcons(RenderIconSlot::ItemSelect))
        --work->timer;
    std::uint32_t requests = work->timerRequests;
    while (requests & 0xc0000000U) {
        if (requests & 0x40000000U) {
            if (work->timerFlags & 2) {
                work->timerFlags ^= 1;
                work->timerFadeFrames = 16 - work->timerFadeFrames;
            } else {
                work->timerFadeFrames = 16;
                work->timerFlags |= 2;
            }
            requests &= ~0x40000000U;
        }
        if (requests & 0x80000000U) {
            if (work->timerFlags & 0x40000000)
                requests &= ~0x80000000U;
            else {
                work->timerFlags |= 0x40000000;
                if (work->timerFlags & 1) {
                    if (!(work->timerFlags & 2))
                        requests |= 0x40000000U;
                } else if (work->timerFlags & 2)
                    requests |= 0x40000000U;
            }
        }
    }
    work->timerRequests = requests;
    std::uint32_t flags = static_cast<std::uint32_t>(work->timerFlags);
    bool retire = false;
    while (flags & 0xc0000003U) {
        if (flags & 2U) {
            if (--work->timerFadeFrames > 0)
                flags |= 1U;
            else {
                if (work->timerFlags & 1) {
                    flags &= ~1U;
                    work->timerFlags &= ~1;
                    DestroyElementByIndex(UIElement::GameHudTimer);
                } else {
                    flags |= 1U;
                    work->timerFlags |= 1;
                    ShowElementByIndex(UIElement::GameHudTimer);
                }
                work->timerFlags &= ~2;
            }
            flags &= ~2U;
        }
        if (flags & 3U) {
            const std::int32_t alpha = (flags & 2U) ? ((flags & 1U) ? 255 * work->timerFadeFrames / 16 : 255 - 255 * work->timerFadeFrames / 16) : 255;
            std::uint32_t rgb = 0xffffffU;
            if (work->timer <= work->timerWarningThreshold + 16) {
                if (work->timer >= work->timerWarningThreshold - 16) {
                    const std::uint32_t component = static_cast<std::uint32_t>(255 * (work->timer - work->timerWarningThreshold + 16) / 32);
                    rgb = component | ((component | 0xff00U) << 8U);
                } else
                    rgb = 0xff0000U;
            }
            LegacyHudText *const text = work->timerText;
            text->colour = rgb | (static_cast<std::uint32_t>(alpha) << 24U);
            const float scaled = static_cast<float>(work->timer) * 3.3333333f;
            const std::int32_t centiseconds = scaled >= 0 ? static_cast<std::int32_t>(scaled) : 0;
            const std::int32_t tenths = centiseconds / 10 % 10;
            const char separator = tenths == 1 || tenths == 4 ? ' ' : ':';
            text->text[0] = static_cast<char>(centiseconds / 6000 % 10 + '0');
            text->text[1] = separator;
            text->text[2] = static_cast<char>(centiseconds / 1000 % 6 + '0');
            text->text[3] = static_cast<char>(centiseconds / 100 % 10 + '0');
            text->text[4] = separator;
            text->text[5] = static_cast<char>(tenths + '0');
            text->text[6] = static_cast<char>(centiseconds % 10 + '0');
            text->text[7] = '\0';
            SetGameHudTimerCaption(text->text.data(), text->colour);
            text->depth = 8.0f;
            text->y = 64.0f;
            text->draw(text);
            if (static_cast<std::uint32_t>(centiseconds - 1) <= 998U) {
                const std::int32_t period = centiseconds / 333 ? (centiseconds / 333 == 1 ? 15 : 30) : 10;
                if (work->timer == period * (work->timer / period))
                    Loop3DSoundEffect(0xabU, scaled);
            }
            flags &= ~1U;
        }
        if (flags & 0x40000000U) {
            if (!(flags & 3U)) {
                work->timerFlags = static_cast<std::int32_t>((static_cast<std::uint32_t>(work->timerFlags) & 0x3fffffffU) | 0x80000000U);
                retire = true;
            }
            flags &= ~0x40000000U;
        }
        if (flags & 0x80000000U) {
            retire = true;
            flags &= ~0x80000000U;
        }
    }
    if (work->flags & 8U)
        SetPlayerGameFlags(PlayerGameFlagMode::SearchBlocked, reinterpret_cast<void *>(1));
    if (retire)
        HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2
