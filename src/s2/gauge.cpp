#include "s2/gauge.hpp"

#include "d3t/texture.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/archive_data.hpp"
#include "s2/hlib.hpp"
#include "s2/pak.hpp"
#include "s2/legacy_sprite.hpp"
#include "d3t/dx11_renderer.hpp"
#include "s2/ui.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace shenmue::s2 {
namespace {

HLib::Task *gaugeTask{};
std::array<std::int32_t, 6> gaugeTextures{};
static_assert(sizeof(gaugeTextures) == 0x18);
float playerPercent{};
float enemyPercent{};
std::int32_t gaugeLayout{};
std::uint32_t gaugeFlags{};

// Fight-widget setup owns these independently of the legacy EGAG task globals.
d3t::UIDefinition *fightDefinition{};
std::array<std::int32_t, 3> fightWidgetIndices{-1, -1, -1};
std::array<std::int32_t, 3> fightPanelIndices{-1, -1, -1};
std::array<std::int32_t, 5> fightRoundImageIndices{-1, -1, -1, -1, -1};
std::vector<d3t::UIRect> fightWidgetRectangles;
std::int32_t fightDisplayLayout{1};
std::uint32_t fightPercentModeFlags{};
float fightPrimaryPercent{};
float fightSecondaryPercent{};
std::int32_t fightSecondaryOverlay{};
std::int32_t fightPrimaryUniform{1};
float fightPrimaryAlpha{255.0f};
float fightTrainingProgress{};
std::vector<d3t::UIResolvedTextureObject> fightTextureRectangles;
std::int32_t fightTrainingBar{-1};

d3t::UIGaugeState *FightWidget(std::size_t slot) noexcept {
    const auto index = fightWidgetIndices[slot];
    if (!fightDefinition || index < 0 || static_cast<std::size_t>(index) >= fightDefinition->gauges.size())
        return nullptr;
    return &fightDefinition->gauges[static_cast<std::size_t>(index)].state;
}

void ReleaseGaugeTextures() {
    for (std::int32_t &texture : gaugeTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
}

bool LoadGaugeTextures() {
    const std::uint32_t size = static_cast<std::uint32_t>(ArchiveData_GetFileSize("sprite/GAUGE.SPR"));
    void *const allocation = LoadLegacySceneFile("sprite", "GAUGE.SPR", TASK_FIGHT_GAUGE);
    if (!allocation)
        return false;
    TEXNReader(allocation, size, gaugeTextures.data());
    HLib::FreeTaskData(allocation);
    return true;
}

void DrawSingleFightGaugeOrbRow() {
    LegacySpritePacket packet;
    packet.quad.flags = 0x400;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    const std::array<float, 2> &base = FIGHT_GAUGE_BASE_POSITIONS[gaugeLayout];
    float threshold = 8.0f;
    for (std::size_t orb = 0; orb < FIGHT_GAUGE_ORB_POSITIONS.size(); ++orb) {
        if (threshold >= playerPercent)
            break;
        const std::int32_t texture = orb < 8 ? 5 : orb < 11 ? 2 : 4;
        const float y = (static_cast<float>(d3t::g_originalScreenHeight) * 0.5f - (base[1] + base[1])) + FIGHT_GAUGE_ORB_POSITIONS[orb][1];
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(base[0] + FIGHT_GAUGE_ORB_POSITIONS[orb][0]),
            TruncateSpriteCoordinate(y), 16, 16, static_cast<ScreenMode>(0));
        if (gaugeTextures[texture] != -1)
            LegacySpriteNullsub(packet, gaugeTextures[texture], 6.0f);
        threshold += 8.333333f;
    }
}

void CtrlGauge_Callback(void *) {
    const bool debugEnabled = GetDebugForceUIEnabled(UIElement::FightGauge) != 0;
    std::uint8_t flags = static_cast<std::uint8_t>(gaugeFlags);
    if (debugEnabled && (gaugeFlags & 2U)) {
        ExecuteFightGaugeUI();
        SetFightGaugeDisplayLayout(0);
        EnableElement(UIElement::FightGauge);
        flags = static_cast<std::uint8_t>(gaugeFlags);
        if (gaugeFlags & ~1U)
            return;
    }
    if (!(flags & 2U))
        return;
    LegacySpritePacket packet;
    packet.quad.flags = 0x400;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    const std::array<float, 2> &base = FIGHT_GAUGE_BASE_POSITIONS[gaugeLayout];
    float y = static_cast<float>(d3t::g_originalScreenHeight) * 0.5f - (base[1] + base[1]);
    AdjustScreenAspectRatio(
        packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(base[0]), TruncateSpriteCoordinate(y), 128, 128, static_cast<ScreenMode>(0));
    LegacySpriteNullsub(packet, gaugeTextures[0], 3.0f);
    packet.quad.flags = 0x400;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    for (const std::array<float, 2> &orb : FIGHT_GAUGE_ORB_POSITIONS) {
        y = (static_cast<float>(d3t::g_originalScreenHeight) * 0.5f - (base[1] + base[1])) + orb[1];
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(base[0] + orb[0]), TruncateSpriteCoordinate(y), 16,
            16, static_cast<ScreenMode>(0));
        if (gaugeTextures[3] != -1)
            LegacySpriteNullsub(packet, gaugeTextures[3], 5.0f);
    }
    if (!(gaugeFlags & 1U)) {
        DrawSingleFightGaugeOrbRow();
        return;
    }
    for (std::size_t pass = 0; pass < FIGHT_GAUGE_DUAL_TEXTURES.size(); ++pass) {
        const float &percentage = pass == 0 ? playerPercent : enemyPercent;
        const std::int32_t texture = FIGHT_GAUGE_DUAL_TEXTURES[pass];
        float threshold = 8.0f;
        for (const std::array<float, 2> &orb : FIGHT_GAUGE_ORB_POSITIONS) {
            if (threshold >= percentage)
                break;
            y = (static_cast<float>(d3t::g_originalScreenHeight) * 0.5f - (base[1] + base[1])) + orb[1];
            AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(base[0] + orb[0]), TruncateSpriteCoordinate(y),
                16, 16, static_cast<ScreenMode>(0));
            if (gaugeTextures[texture] != -1)
                LegacySpriteNullsub(packet, gaugeTextures[texture], static_cast<float>(pass) + 6.0f);
            threshold += 8.333333f;
        }
    }
}

void ga_CtrlGauge(void *) {
    HLib::ExecuteCallbackNow(CtrlGauge_Callback, nullptr);
}

void SetFilledOrbCount(d3t::UIGaugeState &widget, float percent) noexcept {
    const float count = (percent / 100.0f) * static_cast<float>(widget.count);
    const std::int32_t whole = TruncateSpriteCoordinate(count);
    widget.filledCount = whole;
    if (count - static_cast<float>(whole) > 0.0f && whole < widget.count)
        widget.filledCount = whole + 1;
}

} // namespace

std::int64_t CtrlGauge_Init() noexcept {
    gaugeTask = nullptr;
    gaugeTextures.fill(-1);
    playerPercent = 0.0f;
    enemyPercent = 0.0f;
    gaugeLayout = 0;
    gaugeFlags = 1;
    return 0;
}

std::int64_t LoadGauge() {
    if (gaugeTask || !LoadGaugeTextures())
        return 0;
    gaugeTask = HLib::EnqueueTaskWithoutParameter(ga_CtrlGauge, 8, 0, static_cast<std::int32_t>(TASK_FIGHT_GAUGE.name));
    if (!gaugeTask) {
        ReleaseGaugeTextures();
        return 0;
    }
    gaugeFlags |= 2U;
    return 1;
}

std::int64_t ReleaseGauge() {
    if (!gaugeTask)
        return 0;
    HLib::DestroyTaskInternal(gaugeTask);
    ReleaseGaugeTextures();
    gaugeTask = nullptr;
    playerPercent = 0.0f;
    enemyPercent = 0.0f;
    gaugeLayout = 0;
    gaugeTextures.fill(-1);
    gaugeFlags = 1;
    return 1;
}

bool IsFightHealthUIActive() noexcept {
    return gaugeTask && (gaugeFlags & 2U) != 0;
}

std::int64_t SetFightGaugeVisible(std::int32_t visible) noexcept {
    const std::int64_t previous = (gaugeFlags >> 1U) & 1U;
    if (visible)
        gaugeFlags |= 2U;
    else
        gaugeFlags &= ~2U;
    return previous;
}

// @verified - p
std::int64_t SetFightGaugeLayout(std::int32_t layout) noexcept {
    const std::int32_t previous = gaugeLayout;
    if (layout < 6)
        gaugeLayout = layout;
    return static_cast<std::uint32_t>(previous);
}

// @verified - p
double UpdateDualFightGauge(float player, float enemy) noexcept {
    gaugeFlags |= 1U;
    playerPercent = player;
    enemyPercent = enemy;
    if (GetDebugForceUIEnabled(UIElement::FightGauge))
        SetPlayerEnemyPercents(player, enemy);
    return 0.0;
}

d3t::UIGaugeState *UpdateSingleFightGauge(float percent) noexcept {
    gaugeFlags &= ~1U;
    enemyPercent = 0.0f;
    playerPercent = percent;
    return GetDebugForceUIEnabled(UIElement::FightGauge) ? SetFightGaugePercentSingle(percent) : nullptr;
}

void BindFightGaugeDefinition(d3t::UIDefinition &definition) {
    fightDefinition = &definition;
    fightWidgetIndices[0] = definition.gauges.empty() ? -1 : 0;
    fightWidgetIndices[1] = definition.gauges.size() < 2 ? -1 : 1;
    const auto previousLayout = fightDisplayLayout;
    fightDisplayLayout = -1;
    SetFightGaugeDisplayLayout(previousLayout);
    // SetupFightGauge appends an additional kind35 entity after applying the
    // layout. It shares the primary panel, but not background/empty-orb draws.
    const auto *primary = FightWidget(0);
    if (primary && definition.gauges.size() < 5) {
        d3t::UIGaugeDefinition overlay;
        overlay.panelId = definition.gauges[static_cast<std::size_t>(fightWidgetIndices[0])].panelId;
        auto &state = overlay.state;
        state.allocated = 1;
        state.panelIndex = primary->panelIndex;
        state.hidden = primary->hidden;
        state.images = primary->images;
        state.count = primary->count;
        state.filledCount = primary->filledCount;
        state.positions = primary->positions;
        state.width = primary->width;
        state.height = primary->height;
        state.orbColours = primary->orbColours;
        state.fieldE8 = primary->fieldE8;
        state.transparentBackground = 1;
        state.suppressEmptyOrbs = 1;
        fightWidgetIndices[2] = static_cast<std::int32_t>(definition.gauges.size());
        definition.gauges.push_back(std::move(overlay));
    } else {
        fightWidgetIndices[2] = -1;
    }
    fightWidgetRectangles = d3t::GetUIGaugeRectangles(definition);
    fightPanelIndices.fill(-1);
    fightRoundImageIndices.fill(-1);
    for (std::size_t slot = 0; slot < FIGHT_GAUGE_PANELS.size(); ++slot) {
        for (std::size_t index = 0; index < definition.panels.size(); ++index) {
            if (definition.panels[index].id == FIGHT_GAUGE_PANELS[slot]) {
                fightPanelIndices[slot] = static_cast<std::int32_t>(index);
                break;
            }
        }
    }
    for (std::size_t slot = 0; slot < FIGHT_GAUGE_ROUND_IMAGES.size(); ++slot) {
        for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
            if (definition.textureObjects[index].id == FIGHT_GAUGE_ROUND_IMAGES[slot]) {
                fightRoundImageIndices[slot] = static_cast<std::int32_t>(index);
                definition.textureObjects[index].hidden = true;
                break;
            }
        }
    }
    for (const std::int32_t index : fightPanelIndices)
        if (index >= 0)
            definition.panels[index].imageOpacity = 0.0f;
    fightTextureRectangles = d3t::GetUITextureObjectRectangles(definition);
    fightTrainingBar = -1;
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index)
        if (definition.textureObjects[index].layoutId == "HUDGauge::RedBar_Spacepush")
            fightTrainingBar = static_cast<std::int32_t>(index);
}

void ReleaseFightGaugeDefinition() noexcept {
    fightWidgetIndices[0] = -1;
    fightWidgetIndices[1] = -1;
    fightWidgetIndices[2] = -1;
    fightPanelIndices.fill(-1);
    fightRoundImageIndices.fill(-1);
    fightDefinition = nullptr;
    fightWidgetRectangles.clear();
    fightTextureRectangles.clear();
    fightTrainingBar = -1;
}

// @verified - p
void UpdateFightRoundMarkers(std::int32_t playerRounds, std::int32_t enemyRounds) {
    if (!fightDefinition)
        return;
    for (std::size_t slot = 0; slot < 2; ++slot) {
        const std::int32_t index = fightPanelIndices[slot];
        if (index >= 0)
            fightDefinition->panels[index].imageOpacity = 1.0f;
    }
    for (std::size_t slot = 0; slot < fightRoundImageIndices.size(); ++slot) {
        const std::int32_t index = fightRoundImageIndices[slot];
        if (index >= 0)
            fightDefinition->textureObjects[index].hidden = true;
    }
    const std::array<bool, 4> visible{playerRounds >= 1, playerRounds >= 2, enemyRounds >= 1, enemyRounds >= 2};
    for (std::size_t slot = 0; slot < visible.size(); ++slot) {
        const std::int32_t index = fightRoundImageIndices[slot + 1];
        if (index >= 0 && visible[slot])
            fightDefinition->textureObjects[index].hidden = false;
    }
}

void UpdateFightGaugeDefinition() {
    if (fightDefinition && (fightPercentModeFlags & 4U) && fightPanelIndices[2] >= 0)
        fightDefinition->panels[fightPanelIndices[2]].imageOpacity = 1.0f;
    auto *primary = FightWidget(0);
    auto *secondary = FightWidget(1);
    auto *overlay = FightWidget(2);
    if (primary && secondary && overlay) {
        if (fightPercentModeFlags & 1U) {
            primary->hidden = 0;
            secondary->hidden = 1;
            overlay->hidden = 1;
        } else if (fightPercentModeFlags & 2U) {
            primary->hidden = 0;
            secondary->hidden = fightSecondaryOverlay ? 1 : 0;
            overlay->hidden = fightSecondaryOverlay ? 0 : 1;
        }
    }
    if (fightDefinition) {
        for (std::size_t index = 0; index < fightDefinition->textureObjects.size(); ++index) {
            auto placement = fightTextureRectangles[index];
            const auto &object = fightDefinition->textureObjects[index];
            float opacity = 1.0f;
            if (static_cast<std::int32_t>(index) == fightTrainingBar) {
                placement.bounds.width *= fightTrainingProgress;
                if (fightPanelIndices[2] >= 0)
                    opacity = fightDefinition->panels[fightPanelIndices[2]].imageOpacity;
            }
            d3t::DrawUITextureObject(*fightDefinition, object, placement, opacity);
        }
        for (std::size_t index = 0; index < fightDefinition->gauges.size(); ++index)
            d3t::DrawUIGauge(*fightDefinition, fightDefinition->gauges[index].state, fightWidgetRectangles[index], 1.0f);
    }
    fightPercentModeFlags = 0;
    if (fightDefinition) {
        for (std::size_t slot = 0; slot < 2; ++slot) {
            const std::int32_t index = fightPanelIndices[slot];
            if (index >= 0)
                fightDefinition->panels[index].imageOpacity = 0.0f;
        }
        for (const std::int32_t index : fightRoundImageIndices)
            if (index >= 0)
                fightDefinition->textureObjects[index].hidden = true;
    }
}

bool IsFightGaugeInputActive() noexcept {
    return (fightPercentModeFlags & 1U) == 0 && fightDisplayLayout == 1;
}

// @verified - p
bool IsFightGaugeOverlayInputActive() noexcept {
    return fightDisplayLayout != 1;
}

std::int32_t GetFightGaugeDisplayLayout() noexcept {
    return fightDisplayLayout;
}

void SetFightGaugeDisplayLayout(std::int32_t layout) noexcept {
    if (fightDisplayLayout == layout || (!layout && fightSecondaryOverlay == 1))
        return;
    auto *primary = FightWidget(0);
    auto *secondary = FightWidget(1);
    auto *overlay = FightWidget(2);
    if (layout == 0) {
        if (primary) {
            for (std::size_t index = 0; index < primary->orbColours.size(); ++index)
                primary->orbColours[index] = index < 8 ? 2 : index < 11 ? 3 : 1;
            primary->translucentLastOrb = 0;
        }
    } else if (layout == 1) {
        if (primary) {
            primary->orbColours.fill(0);
            primary->translucentLastOrb = 1;
        }
        if (secondary) {
            secondary->orbColours.fill(1);
            secondary->translucentLastOrb = 1;
        }
    } else if (layout == 2) {
        if (primary) {
            primary->orbColours.fill(3);
            primary->translucentLastOrb = 0;
        }
        if (overlay)
            overlay->orbColours.fill(2);
        if (secondary)
            secondary->translucentLastOrb = 0;
    }
    fightDisplayLayout = layout;
}

void SetPlayerEnemyPercents(float player, float enemy) noexcept {
    fightPrimaryPercent = player > 100.0f ? 100.0f : player;
    fightSecondaryPercent = enemy > 100.0f ? 100.0f : enemy;
    fightPercentModeFlags = (fightPercentModeFlags & ~3U) | 2U;
    if (auto *primary = FightWidget(0)) {
        SetFilledOrbCount(*primary, fightPrimaryPercent);
        if (fightDisplayLayout == 1) {
            primary->orbColours.fill(0);
            if (!fightPrimaryUniform) {
                // ExecuteFightGaugeUI writes at +0xa8 + filledCount*4. Count zero
                // intentionally reaches the preceding height field.
                const auto offset = 0xa8 + static_cast<std::ptrdiff_t>(primary->filledCount) * 4;
                if (offset >= 0 && offset <= static_cast<std::ptrdiff_t>(sizeof(*primary)) - 4) {
                    const std::int32_t colour = 3;
                    std::memcpy(reinterpret_cast<std::byte *>(primary) + offset, &colour, sizeof(colour));
                }
            }
            primary->lastOrbAlpha = fightPrimaryAlpha;
            if (auto *secondary = FightWidget(1))
                secondary->lastOrbAlpha = fightPrimaryAlpha;
        }
    }
    if (auto *secondary = FightWidget(fightSecondaryOverlay ? 2 : 1))
        SetFilledOrbCount(*secondary, fightSecondaryPercent);
}

d3t::UIGaugeState *SetFightGaugePercentSingle(float percent) noexcept {
    fightPrimaryPercent = percent < 0.0f ? 0.0f : percent > 100.0f ? 100.0f : percent;
    fightPercentModeFlags = (fightPercentModeFlags & ~3U) | 1U;
    d3t::UIGaugeState *const primary = FightWidget(0);
    if (primary)
        primary->filledCount = TruncateSpriteCoordinate(static_cast<float>(primary->count) * (fightPrimaryPercent / 100.0f));
    return primary;
}

void SetFightGaugePrimaryUniform(std::int32_t uniform) noexcept {
    fightPrimaryUniform = uniform;
}

void SetFightGaugePrimaryVisuals(std::int32_t uniform, float alpha) noexcept {
    fightPrimaryUniform = uniform;
    fightPrimaryAlpha = alpha;
}

void FillFightGaugeSecondaryOrbStates(std::int32_t colour) noexcept {
    if (auto *secondary = FightWidget(1))
        secondary->orbColours.fill(colour);
}
// @verified - p
void ActivateFightGaugePrimary() noexcept {
    if (fightDefinition && fightRoundImageIndices[0] >= 0)
        fightDefinition->textureObjects[fightRoundImageIndices[0]].hidden = false;
}
// @verified - p
void SetFightGaugeTrainingProgress(float progress) noexcept {
    fightPercentModeFlags |= 4U;
    fightTrainingProgress = progress;
}
// @verified - p
void ClearFightGaugeTrainingVisibility() noexcept {
    if (!fightDefinition)
        return;
    if (fightRoundImageIndices[0] >= 0)
        fightDefinition->textureObjects[fightRoundImageIndices[0]].hidden = true;
    if (fightPanelIndices[2] >= 0)
        fightDefinition->panels[fightPanelIndices[2]].imageOpacity = 0.0f;
}

} // namespace shenmue::s2
