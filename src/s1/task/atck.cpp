#include "s1/task/atck.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/player_stats.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/ui_definition.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

AtckNativeHudState ATCK{};
HLib::Task *g_pATCK = nullptr;
std::uint8_t g_ATCK_flags = 0;
std::uint8_t g_ATCK_enable_state = 0;
std::uint8_t g_BlinkPhase = 0;
std::uint8_t g_racedial_state = 0;
std::uint8_t g_RaceDialMode = 0;
const AtckRaceDialConfig *g_RaceDial_UI = nullptr;
const float *g_pGaugeValueSrc = nullptr;
std::array<std::int32_t, 3> g_RaceTimeSlots{};
std::array<std::int32_t, 5> g_LAPTIME_sprites{};
AtckResetRecord g_atckResetRecord{};

namespace {

constexpr std::int32_t kRaceHudElement = 0x42;
constexpr std::uint8_t kFreeQuestHelpToken = 0;
constexpr std::uint8_t kForkliftRaceHelpToken = 0x43;
constexpr std::int32_t kTaskToken = static_cast<std::int32_t>(SHEN_NAME(ATCK));
constexpr std::int32_t kSpriteToken = static_cast<std::int32_t>(SHEN_NAME(LAPT));
constexpr std::int32_t kRaceHudLapNode = 10;
constexpr std::int32_t kRaceHudLapCountNode = 11;
constexpr std::int32_t kRaceHudRankNode = 45;
constexpr std::int32_t kRaceHudRacerCountNode = 46;
// ConstructOpaqueColourRGB(230, 32, 52).
constexpr std::uint32_t kNativeRaceBestColour = 0xff3420e6U;

void LoadLapTimeSurfaces() {
    constexpr const char *kDirectory = "sprite";
    constexpr const char *kFilename = "LAPTIME.SPR";
    const std::uint64_t fileSize64 = shenOpenFile(kDirectory, kFilename);
    if (!fileSize64 || fileSize64 > static_cast<std::uint64_t>((std::numeric_limits<std::int32_t>::max)()))
        return;

    void *const file = ReadFile(kDirectory, kFilename, kSpriteToken);
    if (!file)
        return;

    const std::size_t fileSize = static_cast<std::size_t>(fileSize64);
    auto *const bytes = static_cast<std::byte *>(file);
    std::size_t offset = 0;
    std::size_t surfaceIndex = 0;
    while (offset + 8 <= fileSize && surfaceIndex < g_LAPTIME_sprites.size()) {
        std::uint32_t token = 0;
        std::uint32_t chunkSize = 0;
        std::memcpy(&token, bytes + offset, sizeof(token));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (!token || chunkSize < 8 || chunkSize > fileSize - offset)
            break;
        g_LAPTIME_sprites[surfaceIndex++] = LoadTEXN(bytes + offset);
        offset += chunkSize;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
}

std::int32_t EncodeDialTicks(std::int32_t dial) {
    const std::int32_t below10000 = dial % 10000;
    const std::int32_t below1000 = below10000 % 1000;
    const std::int32_t below100 = below1000 % 100;
    const std::int32_t ones = below100 % 10;
    return ones + 300 * (below1000 / 100) + 3000 * (below10000 / 1000) + 18000 * (dial / 10000) + 30 * (below100 / 10) + 2 * ones;
}

// A bound Race_UI sprite node's frame (+0xA8); unbound (null) nodes are skipped.
void SetNativeNodeMaterial(std::int32_t index, std::int32_t material) {
    if (void *const node = ATCK.nodes[static_cast<std::size_t>(index)])
        static_cast<UIButtonIconWidget *>(node)->sprite = material;
}


void SetInfoWindowAlpha(std::size_t index, float alpha) {
    const std::int32_t window = ATCK.alphaNodeIndices[index];
    if (static_cast<std::uint32_t>(window) > 0x3ffU)
        return;
    g_uiWindows[static_cast<std::size_t>(window)].alpha = alpha;
}
} // namespace



void ATCK_AssignNewDialPreset(std::int32_t preset) {
    AtckNativeHudState &hud = ATCK;
    if (hud.dialPreset == preset)
        return;

    d3t::UIImageWidget *const dial =
        static_cast<std::uint32_t>(hud.gameHudDial) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(hud.gameHudDial)] : nullptr;
    if (preset == 0) {
        for (std::size_t index = 0; index < 7; ++index)
            SetInfoWindowAlpha(index, 0.0f);
    } else if (preset == 1) {
        hud.previousDialValue = 0.0f;
        hud.dialTargetValue = 160.0f;
        for (std::size_t index = 0; index < 6; ++index)
            SetInfoWindowAlpha(index, 0.0f);
        SetInfoWindowAlpha(6, 1.0f);
        if (dial)
            dial->image = hud.dialMaterialA;
    } else if (preset == 2) {
        hud.previousDialValue = 0.0f;
        hud.dialTargetValue = 40.0f;
        for (std::size_t index = 0; index < 6; ++index)
            SetInfoWindowAlpha(index, 1.0f);
        SetInfoWindowAlpha(6, 0.0f);
        if (dial)
            dial->image = hud.field1e4[0];
    }
    hud.dialPreset = preset;
}

void ATCK_RaceHUDWriteNumberToStrip(std::int32_t baseSlot, std::int32_t value) {
    const std::int32_t digit0 = value / 10;
    const std::int32_t digit1 = digit0 / 10;
    const std::int32_t digit2 = digit1 / 10;
    const std::int32_t digit3 = digit2 / 10;
    SetNativeNodeMaterial(baseSlot + 5, value % 10 + 6);
    SetNativeNodeMaterial(baseSlot + 4, digit0 % 10 + 6);
    SetNativeNodeMaterial(baseSlot + 3, digit1 % 10 + 6);
    SetNativeNodeMaterial(baseSlot + 2, digit2 % 10 + 6);
    SetNativeNodeMaterial(baseSlot + 1, digit3 % 10 + 6);
}

std::int32_t ATCK_EncodeRaceDialCompositeValue(std::int32_t dialA, std::int32_t dialB, std::int32_t dialC) {
    const std::int32_t weightedSum = EncodeDialTicks(dialA) + EncodeDialTicks(dialB) + EncodeDialTicks(dialC);
    const std::int32_t below18000 = weightedSum % 18000;
    const std::int32_t below3000 = below18000 % 3000;
    const std::int32_t below300 = below3000 % 300;
    return 100 * (below3000 / 300) + 1000 * (below18000 / 3000) + 10000 * (weightedSum / 18000) + 10 * (below300 / 30) + (below300 % 30) / 3;
}

void ATCK_UpdateRaceLapTimeSlots() {
    AtckNativeHudState &hud = ATCK;
    if (hud.bestLapIndex != -1) {
        const std::int32_t first = 8 * hud.bestLapIndex + 11;
        for (std::int32_t index = first; index < first + 8; ++index) {
            static_cast<UIButtonIconWidget *>(hud.nodes[static_cast<std::size_t>(index)])->colour = 0xffffffffU;
        }
    }

    std::int32_t bestLapIndex = 0;
    hud.bestLapIndex = 0;
    if (hud.currentLap >= 2) {
        const std::array<std::int32_t, 3> values{hud.lapValue0, hud.lapValue1, hud.lapValue2};
        for (std::int32_t index = 1; index < hud.currentLap; ++index) {
            if (values[static_cast<std::size_t>(index)] < values[static_cast<std::size_t>(index - 1)])
                bestLapIndex = index;
            hud.bestLapIndex = bestLapIndex;
        }
    }

    const std::int32_t first = 8 * bestLapIndex + 11;
    for (std::int32_t index = first; index < first + 8; ++index) {
        static_cast<UIButtonIconWidget *>(hud.nodes[static_cast<std::size_t>(index)])->colour = kNativeRaceBestColour;
    }
}

void ATCK_UpdateRaceDial() {
    if (!g_updateRenderFrameMode)
        return;

    AtckNativeHudState &hud = ATCK;
    if ((g_ATCK_flags & 1U) != 0U && g_RaceDial_UI) {
        const AtckRaceDialConfig &config = *g_RaceDial_UI;
        const std::int32_t currentLap = config.currentLap;
        std::array<std::int32_t, 3> displayed{};

        if (g_racedial_state) {
            std::int32_t usedTicks = 0;
            for (std::int32_t index = 0; index < 3; ++index) {
                const std::int32_t value = g_RaceTimeSlots[static_cast<std::size_t>(index)];
                if (value == -1 || index >= currentLap - 1) {
                    displayed[static_cast<std::size_t>(index)] = 0;
                } else {
                    displayed[static_cast<std::size_t>(index)] = value;
                    usedTicks += EncodeDialTicks(value);
                }
            }

            if (currentLap >= 1 && currentLap <= 3) {
                const std::int32_t remainingTicks = 108000 - usedTicks;
                const std::int32_t below18000 = remainingTicks % 18000;
                const std::int32_t below3000 = below18000 % 3000;
                const std::int32_t below300 = below3000 % 300;
                displayed[static_cast<std::size_t>(currentLap - 1)] =
                    (below300 % 30) / 3 + 100 * (below3000 / 300) + 1000 * (below18000 / 3000) + 10000 * (remainingTicks / 18000) + 10 * (below300 / 30);
            }
        } else {
            for (std::size_t index = 0; index < displayed.size(); ++index) {
                std::int32_t value = g_RaceTimeSlots[index];
                if (value < 0) {
                    const std::int32_t third = config.lapFrames[index] / 3;
                    value = 10 * (third + 400 * (third / 10 / 60));
                }
                displayed[index] = value;
            }
        }

        std::int32_t composite = ATCK_EncodeRaceDialCompositeValue(displayed[0], displayed[1], displayed[2]);
        if (g_RaceTimeSlots[2] == -1) {
            const std::int32_t incompleteLapIndex = currentLap - 1;
            if (incompleteLapIndex >= 0 && incompleteLapIndex < 3 && g_RaceTimeSlots[static_cast<std::size_t>(incompleteLapIndex)] == -1)
                displayed[static_cast<std::size_t>(incompleteLapIndex)] += kBlinkGlyphLut[g_BlinkPhase];
            composite = kBlinkGlyphLut[static_cast<std::size_t>(g_BlinkPhase) + 1U] + 10 * (composite / 10);
        }

        ATCK_AssignNewDialPreset(2);
        SetNativeNodeMaterial(kRaceHudLapNode, currentLap + 6);
        SetNativeNodeMaterial(kRaceHudLapCountNode, config.lapCount + 6);
        hud.currentLap = currentLap;
        ATCK_RaceHUDWriteNumberToStrip(12, displayed[0]);
        ATCK_RaceHUDWriteNumberToStrip(20, displayed[1]);
        ATCK_RaceHUDWriteNumberToStrip(28, displayed[2]);
        hud.lapValue0 = displayed[0];
        hud.lapValue1 = displayed[1];
        hud.lapValue2 = displayed[2];
        ATCK_RaceHUDWriteNumberToStrip(1, composite);
        SetNativeNodeMaterial(kRaceHudRankNode, config.rank + 6);
        SetNativeNodeMaterial(kRaceHudRacerCountNode, config.racerCount + 6);
    }


    if ((g_ATCK_flags & 2U) != 0U && g_pGaugeValueSrc) {
        TryInvokeRegisteredCallback(kRaceHudElement);
        hud.currentDialValue = *g_pGaugeValueSrc;
        g_uiElements[static_cast<std::size_t>(kRaceHudElement)].isActive = 1;
    }
}

void ATCK_Callback(HLib::TaskData *) {
    ATCK_UpdateRaceDial();
    if (g_ATCK_enable_state)
        g_BlinkPhase = static_cast<std::uint8_t>((g_BlinkPhase + 1U) & 15U);
}

void shenStartForkliftRaceMode() {
    d3t::ActivateElementByIdx(kRaceHudElement);
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_defaultHelpTokenId = kForkliftRaceHelpToken;
    g_currentHelpTokenId = kForkliftRaceHelpToken;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
    if (g_LAPTIME_sprites[0] == -1)
        LoadLapTimeSurfaces();
    if (!g_pATCK) {
        g_pATCK = HLib_EnqueueTaskWithoutParameter(ATCK_Callback, 8, 0, kTaskToken);
        g_ATCK_flags = static_cast<std::uint8_t>(g_ATCK_flags | 3U);
    }
}

void FreeAtck() {
    g_RaceDial_UI = nullptr;
    g_pGaugeValueSrc = nullptr;
    g_BlinkPhase = 0;
    g_ATCK_enable_state = 0;
    if (g_pATCK) {
        HLib_DestroyTask(g_pATCK);
        g_pATCK = nullptr;
    }
    for (std::int32_t &sprite : g_LAPTIME_sprites) {
        if (sprite != -1) {
            PVR_FreeAllocatedSurfaceByIndex(sprite);
            sprite = -1;
        }
    }
    g_RaceTimeSlots.fill(-1);
    g_racedial_state = 0;
}

void Script_atckUtils(ScenarioContext *, void *rawParams) {
    const AtckUtilityParameters *const params = static_cast<const AtckUtilityParameters *>(rawParams);
    switch (params->mode) {
    case AtckUtilityMode::Start:
        shenStartForkliftRaceMode();
        break;
    case AtckUtilityMode::SetRaceDialSource:
        g_RaceDial_UI = params->parameter1 ? reinterpret_cast<const AtckRaceDialConfig *>(g_pCurrentTaskData + params->parameter1) : nullptr;
        break;
    case AtckUtilityMode::Destroy:
        FreeAtck();
        break;
    case AtckUtilityMode::SetUpdateFlag: {
        const std::uint32_t bit = 1U << (static_cast<std::uint8_t>(params->parameter1) & 31U);
        g_ATCK_flags = static_cast<std::uint8_t>((params->parameter2 & 0xffU) != 0U ? g_ATCK_flags | bit : g_ATCK_flags & ~bit);
        if (params->parameter2 == 0U) {
            d3t::ResetElementByIdx(kRaceHudElement);
            g_previousHelpTokenId = g_currentHelpTokenId;
            g_currentHelpIconState = 0;
            g_defaultHelpTokenId = kFreeQuestHelpToken;
            g_currentHelpTokenId = kFreeQuestHelpToken;
            g_helpButtonId = 0;
        }
        break;
    }
    case AtckUtilityMode::SetGaugeSource:
        g_pGaugeValueSrc = params->parameter1 ? reinterpret_cast<const float *>(g_pCurrentTaskData + params->parameter1) : nullptr;
        break;
    case AtckUtilityMode::SetRaceDialMode:
        g_RaceDialMode = static_cast<std::uint8_t>(params->parameter1);
        break;
    case AtckUtilityMode::SetLapTime:
        if (params->parameter3 >= 0 && params->parameter3 < 3) {
            const std::int32_t third = params->parameter1 / 3;
            const std::int32_t adjustment = static_cast<std::int32_t>(std::bit_cast<float>(params->parameter2) * -100.0f);
            g_RaceTimeSlots[static_cast<std::size_t>(params->parameter3)] = 3 * (params->parameter1 % 3) + 10 * (third + 400 * (third / 10 / 60)) - adjustment;
            ATCK_UpdateRaceLapTimeSlots();
        }
        break;
    case AtckUtilityMode::SetBlinkEnabled:
        if (params->parameter1) {
            g_ATCK_enable_state = 1;
        } else {
            g_ATCK_enable_state = 0;
            g_BlinkPhase = 0;
        }
        break;
    case AtckUtilityMode::RecordBestTime:
        if (params->parameter1) {
            const std::uint32_t composite =
                static_cast<std::uint32_t>(ATCK_EncodeRaceDialCompositeValue(g_RaceTimeSlots[0], g_RaceTimeSlots[1], g_RaceTimeSlots[2]));
            std::uint32_t &best = g_playerStats.pages.bestCompositeTime;
            if (!best || best > composite)
                best = composite;
        } else {
            const std::int32_t index = std::bit_cast<std::int32_t>(params->parameter2);
            if (index >= 0 && index < 3) {
                const std::int32_t time = g_RaceTimeSlots[static_cast<std::size_t>(index)];
                std::uint32_t &best = g_playerStats.pages.bestSegmentTime;
                if (time >= 0 && (!best || best > static_cast<std::uint32_t>(time)))
                    best = static_cast<std::uint32_t>(time);
            }
        }
        break;
    case AtckUtilityMode::CompleteRaceDial:
        if (g_RaceDial_UI)
            g_racedial_state = 1;
        break;
    }
}

} // namespace shenmue::s1
