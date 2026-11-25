#include "s2/easy_option_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/ui_state_dc.hpp"

namespace shenmue {
namespace s2 {

EasyOptionState g_easyOptionState;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetNewEasyOptionState(std::int32_t mode) {
    g_easyOptionState.active = 1;
    g_easyOptionState.mode = static_cast<std::uint8_t>(mode);
    icsToggleIconState(7, 1);
}
#endif

// @verified - d
bool IsEasyOptionActive() {
    if (g_easyOptionState.active)
        return true;
    return false;
}

} // namespace s2
} // namespace shenmue

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/easy_option_data_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/window.hpp"
#include "s2/texture_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/sound_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/pause.hpp"
#include "s2/story_state.hpp"
#include "s2/hand_dc.hpp"
#include "s2/math.hpp"
#include "s2/face_controller_dc.hpp"
#include <cstring>
extern "C" void njPolygonCullingMode(int mode);

namespace shenmue {
namespace s2 {
static std::int8_t easyOptionDraft[4];
struct EasyOptionPadControls {
    PadControlEntry *previous;
    PadControlEntry *current;
};
static EasyOptionPadControls easyOptionPadControls;
static std::int16_t easyOptionLabelTextures[18], easyOptionDescriptionTextures[2], easyOptionPulse, easyOptionPulseStep;
static std::uint16_t easyOptionSavedHelpVisible;
static std::uint8_t easyOptionSavedHelpPresent;
static std::uint32_t easyOptionPreviousNpcState;
static ItemEffectPosition easyOptionPosition;
static LegacyIconSprite easyOptionTextSprite, easyOptionButtonSprite;
static EasyOptionCaption easyOptionValueCaption, easyOptionDescriptionCaption;
void ReleaseNamedTexture_Wrapper(std::int32_t index);

// @verified - d
static void ReleaseEasyOptionDescriptions() {
    std::int16_t *textures = easyOptionDescriptionTextures;
    for (std::int32_t index = 0; index < 2; ++index) {
        if (textures[index] != -1)
            ReleaseNamedTexture_Wrapper(textures[index]);
        textures[index] = -1;
    }
}

// @verified - d
static void ReleaseEasyOptionLabels() {
    std::int16_t *textures = easyOptionLabelTextures;
    for (std::int32_t index = 0; index < 18; ++index) {
        if (textures[index] != -1)
            ReleaseNamedTexture_Wrapper(textures[index]);
        textures[index] = -1;
    }
}

// @verified - d
static void ReleaseEasyOptionTextures() {
    ReleaseEasyOptionDescriptions();
    ReleaseEasyOptionLabels();
}

// @verified - d
static void LoadEasyOptionLabels() {
    for (std::int32_t index = 0; index < 18; ++index) {
        const EasyOptionLabel *label = &easyOptionLabels[index];
        TextureChunk *const ptx = SetupPtxTexture(label->width * 64, SHEN_NAME4('E', 'Z', '0', '0'), (index + 0x30) << 24 | SHEN_NAME4('E', 'Z', '1', 0));
        if (ptx) {
            RasterizeWindowTextLine4bpp(reinterpret_cast<std::byte *>(ptx + 1), label->width * 64, label->text, 1, 1);
            easyOptionLabelTextures[index] = static_cast<std::int16_t>(LoadAndFindTEXN_Wrapper(ptx));
        }
    }
}

static void RefreshEasyOptionDescriptions();

static void ReloadEasyOptionTextures() {
    ReleaseEasyOptionTextures();
    LoadEasyOptionLabels();
    RefreshEasyOptionDescriptions();
}

static void RefreshEasyOptionDescriptions() {
    const std::int8_t row = g_easyOptionState.selectedRow;
    ReleaseEasyOptionDescriptions();
    if (row) {
        const char *const *text = easyOptionDescriptions + 4 + (row - 1) * 2;
        for (std::int32_t index = 0; index <= 1; ++index) {
            const std::uint32_t name = (index + 0x30U) << 24U | SHEN_NAME4('E', 'Z', '1', 0);
            TextureChunk *const ptx = SetupPtxTexture(512, SHEN_NAME4('E', 'Z', 'E', 'X'), name);
            if (ptx) {
                RasterizeWindowTextLine4bpp(reinterpret_cast<std::byte *>(ptx + 1), 512, text[index], 1, 1);
                easyOptionDescriptionTextures[index] = static_cast<std::int16_t>(LoadAndFindTEXN_Wrapper(ptx));
            }
        }
    } else {
        const std::int8_t *indices = easyOptionDialogueDescriptionIndices[easyOptionDraft[0]];
        for (std::int32_t index = 0; index <= 1; ++index) {
            const std::int8_t description = indices[index];
            const char *text = easyOptionDescriptions[description == -1 ? 2 : description];
            const std::uint32_t name = (index + 0x30U) << 24U | SHEN_NAME4('E', 'Z', '1', 0);
            TextureChunk *const ptx = SetupPtxTexture(512, SHEN_NAME4('E', 'Z', 'E', 'X'), name);
            if (ptx) {
                RasterizeWindowTextLine4bpp(reinterpret_cast<std::byte *>(ptx + 1), 512, text, 1, 1);
                easyOptionDescriptionTextures[index] = static_cast<std::int16_t>(LoadAndFindTEXN_Wrapper(ptx));
            }
        }
    }
}

// @verified - d
static void RefreshEasyOptionDialogueCaption() {
    easyOptionValueCaption.first = static_cast<std::int8_t>(easyOptionDialogueLabels[easyOptionDraft[0]]);
    easyOptionDescriptionCaption.first = 1;
    easyOptionDescriptionCaption.second = 1;
    if (!g_easyOptionState.selectedRow && easyOptionDraft[0] != 1)
        easyOptionDescriptionCaption.second = -1;
}

// @verified - d
static void ReadEasyOptionConfiguration() {
    easyOptionDraft[0] = static_cast<std::uint8_t>(GetS2DialogueMode());
    easyOptionDraft[1] = static_cast<std::uint8_t>(GetSystemAudioOutputMode());
    easyOptionDraft[2] = static_cast<std::uint8_t>(GetS2ControlMode());
    easyOptionDraft[3] = GetNormalTriggerMapping() == 0;
}

// @verified - d
static void ApplyEasyOptionConfiguration() {
    ApplyS2DialogueMode(static_cast<std::uint8_t>(easyOptionDraft[0]));
    SetSystemAudioOutputMode(easyOptionDraft[1]);
    ApplyS2ControlMode(easyOptionDraft[2]);
    SetNormalTriggerMapping(easyOptionDraft[3] == 0);
}

// @verified - d
static std::int32_t IsEasyOptionCaptionSelected(std::int32_t condition, std::int32_t value) {
    if (condition == 0)
        return 1;
    else if (condition == 1)
        return 0;
    else if (condition == 2) {
        if (g_easyOptionState.selectedRow != value)
            return 0;
        return 1;
    } else {
        if (easyOptionDraft[condition - 3] != value)
            return 0;
        return 1;
    }
}

// @verified - d
static void DrawEasyOptionFrame(const Vector2 &position, const EasyOptionCaption &caption) {
    std::uint32_t colour, border;
    if (IsEasyOptionCaptionSelected(caption.condition, caption.value)) {
        const std::int32_t pulse = easyOptionPulse << 16;
        colour = pulse | 0xbf000000U;
        border = 0xff650000U;
    } else {
        colour = 0xbf000020U;
        border = 0xff000065U;
    }
    shenSubmitQuadsUI(position.x, position.y, colour, border, easyOptionBoxSizes[caption.size][0], easyOptionBoxSizes[caption.size][1], 3.0f);
}

static void DrawEasyOptionTextSprite(const LegacyIconSprite &sprite) {
    if (static_cast<std::int16_t>(sprite.icon) >= 0)
        PrepareDrawTextureWithMode(sprite);
}

// @verified - d
static void DrawEasyOptionCaption(const EasyOptionCaption &caption, const Vector2 &origin) {
    LegacyIconSprite *sprite = &easyOptionTextSprite;
    sprite->x = static_cast<float>(caption.x) + origin.x;
    sprite->y = static_cast<float>(caption.y) + origin.y;
    if (caption.first >= 0 && caption.second >= 0)
        sprite->y = static_cast<float>(caption.y - 20) + origin.y;
    for (std::int32_t index = 0; index < 2; ++index) {
        const std::int8_t label = (&caption.first)[index];
        if (label >= 0) {
            sprite->width = easyOptionLabels[label].width * 64;
            sprite->icon = static_cast<std::uint16_t>(easyOptionLabelTextures[label]);
            DrawEasyOptionTextSprite(*sprite);
            sprite->y += 40.0f;
        }
    }
}

// @verified - d
static void DrawEasyOptionBox(const EasyOptionCaption &caption, const Vector2 &origin) {
    Vector2 position;
    position.x = static_cast<float>(caption.x) + origin.x;
    position.y = static_cast<float>(caption.y) + origin.y;
    DrawEasyOptionFrame(position, caption);
    DrawEasyOptionCaption(caption, origin);
}

static void DrawEasyOptionDescriptionText(const EasyOptionCaption &caption, const Vector2 &origin);

// @verified - d
static void DrawEasyOptionDescriptions(const EasyOptionCaption &caption, const Vector2 &origin) {
    Vector2 position;
    position.x = static_cast<float>(caption.x) + origin.x;
    position.y = static_cast<float>(caption.y) + origin.y;
    DrawEasyOptionFrame(position, caption);
    DrawEasyOptionDescriptionText(caption, origin);
}

static void DrawEasyOptionDescriptionText(const EasyOptionCaption &caption, const Vector2 &origin) {
    LegacyIconSprite *sprite = &easyOptionTextSprite;
    sprite->x = static_cast<float>(caption.x) + origin.x;
    sprite->y = static_cast<float>(caption.y - 20) + origin.y;
    sprite->width = 512;
    for (std::int32_t index = 0; index < 2; ++index) {
        sprite->icon = static_cast<std::uint16_t>(easyOptionDescriptionTextures[index]);
        if ((&caption.first)[index] >= 0) {
            DrawEasyOptionTextSprite(*sprite);
            sprite->y += 40.0f;
        }
    }
}

// @verified - d
static void DrawEasyOptionArrow(std::int16_t x, std::int16_t y, std::int32_t left) {
    NJS_POLYGON_VTX vertices[4];
    std::int32_t count;
    NJS_POLYGON_VTX *vertex = vertices;
    for (count = 4; count != 0; --count) {
        vertex->z = 0.009708738f;
        vertex->col = ((*reinterpret_cast<volatile std::uint16_t *>(&easyOptionPulse) + 51) << 16) | 0xbf000000U;
        ++vertex;
    }
    NJS_POLYGON_VTX *edge = vertices, *centre = vertices;
    if (left)
        ++edge;
    else
        ++centre;
    vertices[0].x = vertices[2].x = static_cast<float>(x - 16);
    vertices[1].x = vertices[3].x = static_cast<float>(x + 16);
    centre[0].y = static_cast<float>(y);
    centre[2].y = static_cast<float>(y);
    edge[0].y = static_cast<float>(y - 16);
    edge[2].y = static_cast<float>(y + 16);
    dc::sdk::SetMT7PolygonFogMode(2);
    njDrawPolygon(vertices, 4, 1);
    dc::sdk::SetMT7PolygonFogMode(0);
}

// @verified - d
static void DrawEasyOptionMenu() {
    LegacyIconSprite &button = easyOptionButtonSprite;
    easyOptionPulse += easyOptionPulseStep;
    if (easyOptionPulse <= 32)
        easyOptionPulseStep = 2;
    else if (easyOptionPulse > 79)
        easyOptionPulseStep = -2;
    DrawEasyOptionDescriptions(easyOptionDescriptionCaption, reinterpret_cast<const Vector2 &>(easyOptionPosition));
    DrawEasyOptionBox(easyOptionValueCaption, reinterpret_cast<const Vector2 &>(easyOptionPosition));
    DrawEasyOptionArrow(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(easyOptionValueCaption.x) + easyOptionPosition.x - 148.0f)),
        static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(easyOptionValueCaption.y) + easyOptionPosition.y)), 1);
    DrawEasyOptionArrow(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(easyOptionValueCaption.x) + easyOptionPosition.x + 148.0f)),
        static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(easyOptionValueCaption.y) + easyOptionPosition.y)), 0);
    for (std::int32_t index = 0; index < 10; ++index)
        DrawEasyOptionBox(easyOptionCaptions[index], reinterpret_cast<const Vector2 &>(easyOptionPosition));
    DrawMainIconOverlay(0x76000000U, 0.0f);
    button.icon = 0x304;
    button.y = easyOptionPosition.y + 192.0f - 20.0f;
    button.x = easyOptionPosition.x + 200.0f - 48.0f;
    PrepareDrawWithMode(button);
    button.x += 200.0f;
    PrepareDrawWithMode(button);
    button.icon = 0x305;
    button.y += 40.0f;
    button.x = easyOptionPosition.x + 200.0f - 48.0f;
    PrepareDrawWithMode(button);
    button.x += 200.0f;
    PrepareDrawWithMode(button);
}

static void UpdateEasyOptionSelectionInput() {
    std::uint16_t repeated = ctrlGetPadInputBuffer0(easyOptionPadControls.current)->repeatedPresses;
    const std::uint16_t pressed = ctrlGetPadInputBuffer0(easyOptionPadControls.current)->pressedPriority;
    const std::int8_t previousRow = g_easyOptionState.selectedRow;
    const std::uint8_t previousValue = g_easyOptionState.previousValue;
    g_easyOptionState.previousValue = easyOptionDraft[g_easyOptionState.selectedRow];
    std::uint8_t mask = 3;
    if (g_easyOptionState.selectedRow) {
        mask = 1;
        repeated &= g_easyOptionState.previousValue ? 0x7fffU : 0xbfffU;
    }
    if (pressed & 0x600U) {
        g_easyOptionState.active = 4;
        ApplyEasyOptionConfiguration();
        shenExecuteAICACommand(0x900ab, 0, 0);
    } else {
        if (repeated & 0x3000U) {
            if (repeated & 0x1000U) {
                if (--g_easyOptionState.selectedRow < 1)
                    g_easyOptionState.selectedRow = 0;
            } else if ((repeated & 0x2000U) && ++g_easyOptionState.selectedRow > 3)
                g_easyOptionState.selectedRow = 3;
            g_easyOptionState.previousValue = easyOptionDraft[g_easyOptionState.selectedRow] & (g_easyOptionState.selectedRow ? 1U : 3U);
        } else if (repeated & 0xc000U) {
            if (repeated & 0x4000U)
                --g_easyOptionState.previousValue;
            if (repeated & 0x8000U)
                ++g_easyOptionState.previousValue;
            g_easyOptionState.previousValue &= mask;
        }
        easyOptionDraft[g_easyOptionState.selectedRow] = g_easyOptionState.previousValue;
    }
    if (((previousRow ^ g_easyOptionState.selectedRow) | (previousValue ^ g_easyOptionState.previousValue)) != 0) {
        RefreshEasyOptionDialogueCaption();
        RefreshEasyOptionDescriptions();
        shenExecuteAICACommand(0x200ab, 0, 0);
    }
}

static void EasyOptionTaskCallback(void *) {
    if (g_loadingFlag & 2U) {
        if (g_easyOptionState.showsOptions)
            DrawEasyOptionMenu();
        return;
    }
    switch (g_easyOptionState.active) {
    case 0:
        break;
    case 1:
        if (easyOptionPadControls.current && !IsPadControlEntryBound(easyOptionPadControls.current))
            easyOptionPadControls.previous = BindPadControlEntry(easyOptionPadControls.current);
        g_easyOptionState.active = 2;
        g_easyOptionState.showsOptions = 1;
        easyOptionPosition.x = -640.0f;
        easyOptionPosition.y = 65.0f;
        ReadEasyOptionConfiguration();
        g_easyOptionState.previousValue = easyOptionDraft[g_easyOptionState.selectedRow];
        easyOptionPreviousNpcState = GetNpcNewLoadState();
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        RefreshEasyOptionDialogueCaption();
        ReloadEasyOptionTextures();
        if (!g_easyOptionState.mode) {
            easyOptionSavedHelpVisible = static_cast<std::uint16_t>(GetUiFlag(0));
            easyOptionSavedHelpPresent = static_cast<std::uint8_t>(GetUiFlag(1));
            CheckHelpIcon(0x21, 0, CheckHelpIconOp_Set);
            shenSetClockAndCompassRenderTo(1);
        }
        break;
    case 2:
        if (AdvanceIconPosition(&easyOptionPosition, 125.0f))
            g_easyOptionState.active = 3;
        break;
    case 3:
        UpdateEasyOptionSelectionInput();
        RequestHelpIconSelection(0x600);
        break;
    case 4:
        if (RetreatIconPosition(&easyOptionPosition, -640.0f)) {
            g_easyOptionState.active = 0;
            if (easyOptionPadControls.previous && IsPadControlEntryBound(easyOptionPadControls.current))
                BindPadControlEntry(easyOptionPadControls.previous);
            g_easyOptionState.showsOptions = 0;
            ReleaseEasyOptionTextures();
            if (g_easyOptionState.mode)
                SetSleepActionIcon(0);
            else {
                CheckHelpIcon(easyOptionSavedHelpVisible, 0, CheckHelpIconOp_Set);
                shenSetClockAndCompassRenderTo(0);
            }
            icsToggleIconState(7, 0);
            EV_NPC_NewLoadSw(easyOptionPreviousNpcState);
        }
        break;
    }
    if (g_easyOptionState.showsOptions)
        DrawEasyOptionMenu();
}

static void InitEasyOptionTaskCallback(void *) {
    easyOptionPadControls.previous = 0;
    easyOptionPadControls.current = 0;
    easyOptionPadControls.current = AllocatePadControlEntry();
    easyOptionPulse = 32;
    easyOptionPulseStep = 2;
    g_easyOptionState.selectedRow = 0;
    g_easyOptionState.mode = 0;
    g_easyOptionState.active = 0;
    g_easyOptionState.showsOptions = 0;
    for (std::int32_t index = 0; index < 18; ++index)
        easyOptionLabelTextures[index] = -1;
    easyOptionDescriptionTextures[0] = -1;
    easyOptionDescriptionTextures[1] = -1;
    easyOptionPosition.x = -50.0f;
    easyOptionPosition.y = 100.0f;
    easyOptionPosition.velocity = 1.0f;
    easyOptionPosition.reservedScale = 0.0f;
    easyOptionPosition.selectedDirection = 4;
    easyOptionPosition.depth = 3.0f;
    easyOptionTextSprite.width = 256;
    easyOptionTextSprite.height = 32;
    easyOptionTextSprite.x = easyOptionPosition.x;
    easyOptionTextSprite.scaleX = 1.0f;
    easyOptionTextSprite.scaleY = 1.0f;
    easyOptionTextSprite.rotation = 0;
    easyOptionTextSprite.depth = 5.0f;
    easyOptionTextSprite.flags = 0;
    easyOptionTextSprite.alignment = 5;
    InitializeLegacyIconSprite(easyOptionButtonSprite);
    easyOptionButtonSprite.depth = 5.0f;
    HLib::SetCurrentTaskTo(EasyOptionTaskCallback);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetCachedFacialModelRoot(std::int8_t index, MT7Node *root) {
    facialModelCache[index].model->root = root;
    facialModelCache[index].root = root;
}
#endif

// @verified - d
static void StepDebugTaskCallback(void *) {
    UpdateStepDebugCounters();
}

// @verified - d
void EnqueueESOP() {
    for (std::int32_t index = 0; index < facialModelCacheCount; ++index) {
        std::memcpy(facialModelCache[index].name, facialModelCacheEmptyName, sizeof(facialModelCacheEmptyName));
        facialModelCache[index].references = 0;
        facialModelCache[index].root = 0;
        facialModelCache[index].model = 0;
    }
    InitializeHandWork();
    HLib::EnqueueTaskWithoutParameter(InitEasyOptionTaskCallback, 14, 0, SHEN_NAME(ESOP));
    HLib::EnqueueTaskWithoutParameter(StepDebugTaskCallback, 12, 0, SHEN_NAME(UPDB));
    reinterpret_cast<void (*)()>(StepDebugTaskCallback)();
}

void ClaimCachedFacialModelSlot(const char *name, std::uint8_t index) {
    std::strcpy(facialModelCache[index].name, name);
    facialModelCache[index].references = 1;
}

} // namespace s2
} // namespace shenmue

#endif
