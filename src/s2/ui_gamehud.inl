

std::int32_t NativeButtonKey(std::int32_t action, std::int32_t type) {
    if (type < 0)
        type = GetCurrentUIInputType();
    std::uint32_t key = 0;
    if (action < 50) {
        if (action < 0)
            return -1;
        if (type <= 1)
            key = GetMenuKeyMapping(action);
        else if (type == 3 && action < 22)
            key = GetFightKeyMapping(action);
        else if (type == 5 && action >= 18 && action <= 21)
            key = GetQteKeyMapping(action - 18);
        else if ((type == 4 || type == 5) && action <= 3)
            key = GetQteKeyMapping(action);
        else
            key = GetGameKeyMapping(action);
    } else
        switch (action) {
        case 50:
        case 51:
            key = GetMenuKeyMapping(action - 42);
            break;
        case 52:
        case 53:
            key = GetMenuKeyMapping(action - 48);
            break;
        case 54:
        case 55:
            key = GetFightKeyMapping(action - 38);
            break;
        case 56:
            key = GetGameKeyMapping(8);
            break;
        case 64:
            key = GetMenuKeyMapping(13);
            break;
        case 65:
        case 66:
            if (type <= 1)
                key = GetMenuKeyMapping(action - 65);
            else if (type == 3)
                key = GetFightKeyMapping(action - 65);
            else if (type == 4 || type == 5)
                key = GetQteKeyMapping(action - 65);
            else
                key = GetGameKeyMapping(action - 65);
            break;
        default:
            break;
        }
    return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(key));
}
std::int32_t NativeButtonFrame(std::int32_t action, bool keyboard, std::int32_t type, bool doubleWidth, std::int32_t &key) {
    if (keyboard) {
        switch (action) {
        case 57:
        case 67:
            return 30;
        case 58:
            return 31;
        case 59:
            return 32;
        case 60:
        case 63:
            return 33;
        case 61:
            return 34;
        case 62:
            return 35;
        case 64:
            key = NativeButtonKey(action, type);
            return 55;
        case 68:
            return 37;
        default:
            break;
        }
        key = NativeButtonKey(action, type);
        switch (key) {
        case -1:
            return 40;
        case 256:
            return 41;
        case 257:
            return 43;
        case 258:
            return 42;
        case 37:
            return 36;
        case 38:
            return 37;
        case 39:
            return 38;
        case 40:
            return 39;
        case 13:
            return doubleWidth ? 29 : 26;
        case 8:
        case 9:
        case 16:
        case 17:
        case 20:
        case 32:
            return doubleWidth ? 27 : 26;
        default:
            return 26;
        }
    }
    switch (action) {
    case 10:
    case 14:
    case 68:
        return 7;
    case 11:
    case 15:
        return 5;
    case 12:
    case 16:
        return 8;
    case 13:
    case 17:
        return 6;
    case 57:
    case 60:
    case 67:
        return 4;
    case 58:
    case 61:
        return 10;
    case 59:
    case 62:
        return 9;
    case 63:
        return 15;
    case 0:
        return GetIsFreeInputType() ? 19 : 18;
    case 1:
        return GetIsFreeInputType() ? 18 : 19;
    case 18:
        return GetIsFreeInputType(4) ? 19 : 18;
    case 19:
        return GetIsFreeInputType(4) ? 18 : 19;
    case 22:
        return GetIsFreeInputType(3) ? 19 : 18;
    case 23:
        return GetIsFreeInputType(3) ? 18 : 19;
    case 2:
    case 20:
    case 24:
        return 20;
    case 3:
    case 21:
    case 25:
        return 21;
    case 4:
    case 52:
        return 2;
    case 5:
    case 53:
        return 3;
    case 8:
    case 50:
    case 56:
        return 0;
    case 9:
    case 51:
        return 1;
    case 54:
        return 11;
    case 55:
        return 13;
    case 64:
        return GetIsFreeInputType(0) ? 19 : 18;
    case 66:
        return 19;
    default:
        return 18;
    }
}
void DrawNativeButtonIcon(
    d3t::UIDefinition &definition, const d3t::UITextureObjectDefinition &object, const d3t::UIResolvedTextureObject &placement, float opacity) {
    if (!drawingUI || object.hidden)
        return;
    // The view's own Image (GameHud's HUDButtons), else the platform ButtonIcons; frames are always the platform sheet's.
    const auto &icons = uiDefinitions[63];
    const auto *image = definition.findImage(object.imageName);
    if (!image)
        image = icons.findImage("ButtonIcons");
    const auto sheet =
        std::find_if(icons.spriteSheets.begin(), icons.spriteSheets.end(), [](const auto &candidate) { return candidate.imageName == "ButtonIcons"; });
    if (!image || !image->textureHandle || sheet == icons.spriteSheets.end())
        return;
    const auto action = FindUIInputAction(object.action.c_str(), 0);
    const bool keyboard = (object.controlMethod < 0 ? GetIsKeyboardActive() : static_cast<std::uint64_t>(object.controlMethod)) == 1;
    std::int32_t key = -1;
    const auto frame = NativeButtonFrame(action, keyboard, object.controlState, object.allowDoubleWidth == 1, key);
    if (frame < 0 || frame >= static_cast<std::int32_t>(sheet->frames.size()))
        return;
    const auto &crop = sheet->frames[static_cast<std::size_t>(frame)];
    const d3t::UISharedSpriteImage shared{image->textureHandle, {crop.x, crop.y, crop.width, crop.height}};
    d3t::DrawUITextureObject(definition, object, placement, opacity, &shared);
    // Text draws in the centred three-quarter box, with the object's Alignment except for the direction-key clusters.
    const auto &outer = placement.bounds;
    const d3t::UIRect inner{outer.x + (outer.width - outer.width * 0.75f) * 0.5f, outer.y + (outer.height - outer.height * 0.75f) * 0.5f, outer.width * 0.75f,
        outer.height * 0.75f};
    const auto drawKey = [&](std::int32_t code, d3t::UIRect bounds, const std::string &alignment) {
        std::array<char, 64> name{};
        if (!FormatUIInputKeyName(code, name))
            return;
        d3t::UILabelDefinition label;
        label.alignment = alignment;
        label.colour = object.fontColour;
        label.fontSize = object.iconFontSize;
        DrawMovesLabel(label, {bounds, placement.clip}, name.data(), opacity, &definition);
    };
    const std::string centre = "Centre";
    if (keyboard && frame >= 26 && frame <= 28)
        drawKey(key, inner, object.textAlignment);
    else if (keyboard && (frame == 29 || frame == 55)) {
        auto bounds = inner;
        bounds.height *= 0.5f;
        bounds.y += bounds.height * 0.5f;
        drawKey(13, bounds, object.textAlignment);
    } else if (keyboard && frame >= 30 && frame <= 35) {
        const auto &bounds = inner;
        const float size = bounds.height * 0.6f;
        if (frame == 33 || frame == 34) {
            drawKey(static_cast<std::int32_t>(GetMenuKeyMapping(14)),
                {bounds.x + bounds.width * 0.5f - size * 0.5f - bounds.width * (frame == 34 ? 0.15f : 0.08f), bounds.y - size * 0.2f, size, size}, centre);
            drawKey(static_cast<std::int32_t>(GetMenuKeyMapping(15)),
                {bounds.x + bounds.width * 0.5f - size * 0.5f, bounds.y + bounds.height * 0.5f, size, size}, centre);
        }
        if (frame == 33 || frame == 35) {
            const float y = bounds.y + bounds.height * 0.5f - (frame == 35 ? size * 0.5f : 0);
            drawKey(static_cast<std::int32_t>(GetMenuKeyMapping(16)), {bounds.x + bounds.width * 0.2f - size * 0.5f, y, size, size}, centre);
            drawKey(static_cast<std::int32_t>(GetMenuKeyMapping(17)), {bounds.x + bounds.width * 0.8f - size * 0.5f, y, size, size}, centre);
        }
    } else if (!keyboard && frame >= 4 && frame <= 10) {
        const bool right = action == 10 || action == 11 || action == 12 || action == 13 || action == 57 || action == 58 || action == 59;
        const bool left =
            action == 14 || action == 15 || action == 16 || action == 17 || action == 60 || action == 61 || action == 62 || action == 67 || action == 68;
        if (right || left) {
            d3t::UILabelDefinition label;
            label.alignment = object.textAlignment;
            label.colour = (object.fontColour & 0xff000000U) | 0xffffffU;
            label.fontSize = object.iconFontSize;
            DrawMovesLabel(label, {inner, placement.clip}, right ? "R" : "L", opacity, &definition);
        }
    }
}
void DrawNativeMenuObject(
    d3t::UIDefinition &definition, const d3t::UITextureObjectDefinition &object, const d3t::UIResolvedTextureObject &placement, float opacity) {
    if (object.kind == d3t::UITextureObjectKind::ButtonIcon)
        DrawNativeButtonIcon(definition, object, placement, opacity);
    else if (object.kind == d3t::UITextureObjectKind::Sprite && object.useShared) {
        d3t::UISharedSpriteImage image{};
        if (object.frameIndex >= 0 && ResolveSharedItemImage(static_cast<std::uint16_t>(object.frameIndex), image))
            d3t::DrawUITextureObject(definition, object, placement, opacity, &image);
    } else
        d3t::DrawUITextureObject(definition, object, placement, opacity);
}
NativeHudRuntime nativeHud;
std::array<std::uint32_t, 5> nativeHudRequestedHints{}, nativeHudShownHints{}, nativeHudOverrideHints{};
bool nativeHudOverride{};
std::int32_t nativeHudItemPromptIcon{};
std::uint32_t LoadNativeGameHud() {
    nativeHud.bound = false;
    return ReadUIDefinitionFile(UIElement::GameHud, "/ui/gamehud/GameHud.ui", 1);
}
std::int64_t BindNativeGameHud() {
    constexpr std::array<const char *, 17> panels{"GameHUD::MainHudParent", "GameHUD::ButtonBParent", "GameHUD::ButtonBPanel", "GameHUD::ButtonB",
        "GameHUD::ButtonBIcon", "GameHUD::ButtonAPanel", "GameHUD::ButtonA", "GameHUD::ButtonAIcon", "GameHUD::ButtonYPanel", "GameHUD::ButtonY",
        "GameHUD::ButtonYIcon", "GameHUD::ButtonXPanel", "GameHUD::ButtonX", "GameHUD::ButtonXIcon", "GameHUD::CameraPanel", "GameHUD::CameraButtonPanel",
        "GameHUD::CameraIconPanel"};
    constexpr std::array<const char *, 6> sprites{
        "GameHUD_ConfirmSprite", "GameHUD_CancelSprite", "GameHUD_EventSprite", "GameHUD_SystemSprite", "GameHUD_CameraSprite", "GameHUD_CameraStateSprite"};
    constexpr std::array<const char *, 5> buttons{"GameHUD_ConfirmIcon", "GameHUD_CancelIcon", "GameHUD_EventIcon", "GameHUD_SystemIcon", "GameHUD_CameraIcon"};
    auto &definition = uiDefinitions[64];
    BindAuthoredMenuLayout(definition, nativeHud.layout);
    for (std::size_t i = 0; i < panels.size(); ++i) {
        nativeHud.panels[i] = MenuPanelIndex(definition, panels[i]);
        if (nativeHud.panels[i] >= 0)
            nativeHud.panelOffsetY[i] = definition.panels[static_cast<std::size_t>(nativeHud.panels[i])].y.offset;
    }
    nativeHud.sprites.fill(-1);
    nativeHud.buttons.fill(-1);
    nativeHud.cameraBase = -1;
    for (std::size_t i = 0; i < definition.textureObjects.size(); ++i) {
        const auto &object = definition.textureObjects[i];

        for (std::size_t j = 0; j < sprites.size(); ++j)
            if (object.id == sprites[j] && nativeHud.sprites[j] < 0)
                nativeHud.sprites[j] = static_cast<std::int32_t>(i);
        for (std::size_t j = 0; j < buttons.size(); ++j)
            if (object.id == buttons[j] && nativeHud.buttons[j] < 0)
                nativeHud.buttons[j] = static_cast<std::int32_t>(i);
        if (object.id == "HUDCameraBase" && nativeHud.cameraBase < 0)
            nativeHud.cameraBase = static_cast<std::int32_t>(i);
    }
    if (nativeHud.sprites[5] >= 0)
        definition.textureObjects[static_cast<std::size_t>(nativeHud.sprites[5])].hidden = true;
    nativeHud.blink = UIAnimAllocSlot();
    if (nativeHud.blink >= 0) {
        g_uiAnimSlots[static_cast<std::size_t>(nativeHud.blink)].loopMode = 2;
        UIAnim_Start(nativeHud.blink, 1, 0.1f, 0.2f, 0.01f);
    }
    for (auto &transition : nativeHud.transitions)
        transition = UIAnimAllocSlot();
    nativeHud.bound = true;
    return 1;
}
std::int64_t NativeGameHudInput() {
    if (!nativeHud.bound || !uiPointer.leftClicks)
        return 1;
    constexpr std::array<std::size_t, 5> panels{6, 3, 12, 9, 14};
    const float x = uiPointer.position.x * 1920 / GetUIViewportWidth();
    for (std::size_t i = 0; i < panels.size(); ++i) {
        const auto panel = nativeHud.panels[panels[i]];
        if (panel < 0)
            continue;
        const auto &bounds = nativeHud.layout.panels[static_cast<std::size_t>(panel)];
        if (x < bounds.x || x >= bounds.x + bounds.width || uiPointer.position.y < bounds.y || uiPointer.position.y >= bounds.y + bounds.height)
            continue;
        const auto &hints = nativeHudOverride ? nativeHudOverrideHints : nativeHudShownHints;
        if (i < 4 && (hints[i == 0 ? 0 : 3] & 0x20000U))
            continue;
        if (i == 4 && g_uiElements[8].isVisible)
            continue;
        const std::uint16_t mask = i == 0 ? (GetIsFreeInputType() ? 0x200 : 0x400) :
            i == 1                        ? (GetIsFreeInputType() ? 0x400 : 0x200) :
            i == 2                        ? 4 :
            i == 3                        ? 2 :
                                            8;
        InjectInputButtons(mask);
    }
    return 1;
}
std::int64_t RenderNativeGameHud() {
    if (!nativeHud.bound || !drawingUI)
        return 0;
    if (!nativeHudOverride) {
        const std::uint32_t flags = GetIconSelectFlags();
        if (static_cast<std::uint16_t>(nativeHudShownHints[1]) == 0xffffU && (flags & 0x200U))
            nativeHudShownHints[1] = 44;
        if (static_cast<std::uint16_t>(nativeHudShownHints[0]) == 0xffffU && (flags & 0x400U)) {
            nativeHudShownHints[1] &= 0xffffU;
            nativeHudShownHints[0] = (nativeHudShownHints[0] & 0xffff0000U) | 45U;
        }
        if (GetQteControlState())
            nativeHudShownHints[4] = 28;
        else
            nativeHudShownHints[4] |= 0xffffU;
    }
    auto hints = nativeHudOverride ? nativeHudOverrideHints : nativeHudShownHints;
    if (GetIsFreeInputType())
        std::swap(hints[0], hints[1]);
    const bool keyboard = GetIsKeyboardActive() == 1;
    auto &definition = uiDefinitions[64];
    constexpr std::array<std::size_t, 6> shifted{0, 14, 7, 4, 13, 10};
    for (const std::size_t i : shifted)
        if (nativeHud.panels[i] >= 0) {
            d3t::UIPanelDefinition &panel = definition.panels[static_cast<std::size_t>(nativeHud.panels[i])];
            panel.y.offset = nativeHud.panelOffsetY[i] + (keyboard ? (i == 0 ? -1 : 1) * 0.0092592593f * 1080 : 0);
        }
    bool blinking = false;
    for (std::size_t i = 0; i < 5; ++i) {
        const auto index = nativeHud.sprites[i];
        if (index < 0)
            continue;
        auto &object = definition.textureObjects[static_cast<std::size_t>(index)];
        std::uint32_t alpha = 255;
        if (hints[i] & 0x10000U) {
            blinking = true;
            if (nativeHud.blink >= 0)
                alpha = static_cast<std::uint32_t>(GetUITransitionValue(nativeHud.blink) * 255);
        }
        if (hints[i] & 0x20000U)
            alpha = 100;
        object.frameIndex = static_cast<std::int16_t>(hints[i]);
        object.useShared = true;
        if (i == 2 && object.frameIndex == 51 && nativeHudItemPromptIcon == 1)
            object.frameIndex = 3847;
        if (i == 4) {
            object.frameIndex = 2305;
            alpha = static_cast<std::uint16_t>(hints[i]) == 0xffffU ? 0 : 255;
        }
        object.colour = (object.colour & 0xffffffU) | (alpha << 24);
        if (i < 4 && nativeHud.buttons[i] >= 0) {
            auto &button = definition.textureObjects[static_cast<std::size_t>(nativeHud.buttons[i])];
            button.controlState = GetCurrentUIInputType();
            button.colour = (button.colour & 0xffffffU) | ((object.frameIndex == -1 ? 168U : 255U) << 24);
        }
    }
    if (!blinking && nativeHud.blink >= 0)
        RestartUITransition(nativeHud.blink);
    const bool camera = static_cast<std::uint16_t>(hints[4]) != 0xffffU;
    if (nativeHud.buttons[4] >= 0) {
        auto &button = definition.textureObjects[static_cast<std::size_t>(nativeHud.buttons[4])];
        button.colour = (button.colour & 0xffffffU) | ((camera && !keyboard ? 255U : 0U) << 24);
        button.fontColour = (button.fontColour & 0xffffffU) | ((camera ? 255U : 0U) << 24);
    }
    if (nativeHud.cameraBase >= 0) {
        auto &base = definition.textureObjects[static_cast<std::size_t>(nativeHud.cameraBase)];
        base.colour = (base.colour & 0xffffffU) | ((camera && keyboard ? 255U : 0U) << 24);
    }
    auto &layout = nativeHud.layout;
    d3t::ResolveUIDefinitionLayout(
        definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels, {}, GetUILayoutPaddingScale());
    const bool hideCamera = IsCameraIconHidden() == 0;
    const auto hidden = [&](std::int32_t panel) {
        for (; panel >= 0; panel = layout.parents[static_cast<std::size_t>(panel)])
            if (panel == nativeHud.panels[14] || panel == nativeHud.panels[15] || panel == nativeHud.panels[16])
                return hideCamera;
        return false;
    };
    for (std::size_t i = 0; i < definition.textureObjects.size(); ++i)
        if (!hidden(layout.objectPanels[i]))
            DrawNativeMenuObject(definition, definition.textureObjects[i], layout.objects[i], 1);
    for (std::size_t i = 0; i < definition.labels.size(); ++i)
        if (!hidden(layout.labelPanels[i])) {
            const auto &label = definition.labels[i];
            DrawMovesLabel(label, layout.labels[i], label.token.empty() ? std::string_view(label.text) : GetLocalizedSceneText(label.token), 1, &definition);
        }
    return 0;
}

std::int64_t GameHudWatchTickNullsub() {
    return 0;
}
std::int64_t GameHudWatchDestroy() {
    return 1;
}
// View images 1-3 (compass ring, minute and hour hands) turn by the time HUD's angles, the TIME label shows AM or PM for the current
// hour, and the view draws; the CompassPanel window alpha fades the ring.
std::int64_t RenderGameHudWatch() {
    auto &definition = uiDefinitions[static_cast<std::size_t>(UIElement::GameHudWatch)];
    const std::array<float, 3> angles{gameHudCompassRotation, gameHudMinuteHandRotation, gameHudHourHandRotation};
    for (std::size_t image = 1; image < 4 && image < definition.images.size(); ++image)
        for (auto &object : definition.textureObjects)
            if (object.imageName == definition.images[image].name)
                object.rotation = angles[image - 1];
    DateTime now{};
    GetCurrentDateTime(now);
    const auto suffix=GetLocalizedSceneText(now.hour<12?"UILOADAM00":"UILOADPM00"); // @text UILOADAM00 EN: "am" | JA: "午前" ; UILOADPM00 EN: "pm" | JA: "午後"
    std::snprintf(gameHudWatchTimeText.data(), gameHudWatchTimeText.size(), "%.*s", static_cast<int>(suffix.size()), suffix.data());
    if (!drawingUI)
        return 0;
    auto &layout = gameHudWatchLayout;
    d3t::ResolveUIDefinitionLayout(
        definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels, {}, GetUILayoutPaddingScale());
    for (std::size_t i = 0; i < definition.textureObjects.size(); ++i) {
        const auto panel = layout.objectPanels[i];
        d3t::DrawUITextureObject(
            definition, definition.textureObjects[i], layout.objects[i], panel < 0 ? 1.0f : definition.panels[static_cast<std::size_t>(panel)].imageOpacity);
    }
    for (std::size_t i = 0; i < definition.labels.size(); ++i) {
        const auto &label = definition.labels[i];
        DrawMovesLabel(label, layout.labels[i],
            static_cast<std::int32_t>(i) == gameHudWatchTimeLabel ? std::string_view(gameHudWatchTimeText.data()) : std::string_view(label.text), 1,
            &definition);
    }
    return 0;
}
void RegisterNativeGameHud() {
    auto &hud = g_uiElements[64];
    hud.name = "Game HUD";
    hud.parseDefinitionFromFile = LoadNativeGameHud;
    hud.isSelected = BindNativeGameHud;
    hud.isPressed = NativeGameHudInput;
    hud.render = RenderNativeGameHud;
    hud.fieldAc = 1;
    nativeHud.transitions.fill(-1);
    nativeHud.blink = -1;
    nativeHud.bound = false;
    nativeHudShownHints.fill(0xffffU);
    nativeHudOverrideHints.fill(0xffffU);
    nativeHudRequestedHints.fill(0xffffU);
    ClearMenuInputHints();
    nativeHudOverride = false;
}
