

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
std::array<d3t::UIWindow *, 17> nativeHudWindows{};
std::array<UIButtonIconWidget *, 5> nativeHudButtonWidgets{};
d3t::UILabelWidget *nativeHudCameraLabel{};
UIColourImageWidget *nativeHudCameraBase{};
std::array<std::int32_t, 5> nativeHudTransitions{};
std::int32_t nativeHudBlink{}, nativeHudKeyboardMode{};
std::array<UIButtonIconWidget *, 6> nativeHudSpriteWidgets{};
std::array<
std::uint32_t, 5> nativeHudRequestedHints{}, nativeHudShownHints{}, nativeHudOverrideHints{};
bool nativeHudOverride{};
std::int32_t nativeHudItemPromptIcon{};
std::uint32_t LoadNativeGameHud() {

    return ReadUIDefinitionFile(UIElement::GameHud, "/ui/gamehud/GameHud.ui", 1);
}
// @verified - p
std::int64_t BindNativeGameHud() {
    nativeHudKeyboardMode = 0;
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::GameHud)].config.uiHandleIndex;
    for (std::size_t index = 0; index < nativeHudWindows.size(); ++index)
        nativeHudWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_GAME_HUD_WINDOWS[index]));
    for (const std::size_t index : {6U, 3U, 12U, 9U, 14U})
        if (nativeHudWindows[index])
            nativeHudWindows[index]->mouseCallbacks.clicked = NativeGameHudClicked;
    nativeHudCameraLabel = UI_GetLabel(UI_FindViewLabel(slot, UI_GAME_HUD_CAMERA_LABEL));
    for (std::size_t index = 0; index < nativeHudSpriteWidgets.size(); ++index)
        nativeHudSpriteWidgets[index] = UI_FindViewIconWidget(slot, UI_GAME_HUD_SPRITES[index], 20);
    nativeHudCameraBase = UI_GetColourImageWidget(UI_FindViewColourImage(slot, UI_GAME_HUD_CAMERA_BASE));

    for (std::size_t index = 0; index < nativeHudButtonWidgets.size(); ++index)
        nativeHudButtonWidgets[index] = UI_FindViewIconWidget(slot, UI_GAME_HUD_BUTTONS[index], 21);

    if (nativeHudSpriteWidgets[5])
        nativeHudSpriteWidgets[5]->fieldAC = 1;
    nativeHudBlink = UIAnimAllocSlot();
    SetUITransitionLoopMode(nativeHudBlink, 2);
    UIAnim_Start(nativeHudBlink, 1.0f, 0.1f, 0.2f, 0.01f);

    for (std::int32_t &transition : nativeHudTransitions)
        transition = UIAnimAllocSlot();

    return 1;
}
// @verified - p
std::int64_t NativeGameHudInput() {

        return 1;
}
// @verified - p
void NativeGameHudClicked(float *, void *window) {
    const std::array<std::uint32_t, 5> &hints = nativeHudOverride ? nativeHudOverrideHints : nativeHudShownHints;
    if (window == nativeHudWindows[6] && !(hints[0] & 0x20000U))
        InjectInputButtons(GetIsFreeInputType() ? 512 : 1024);
    if (window == nativeHudWindows[3] && !(hints[3] & 0x20000U))
        InjectInputButtons(GetIsFreeInputType() ? 1024 : 512);
    if (window == nativeHudWindows[12] && !(hints[3] & 0x20000U))
        InjectInputButtons(4);
    if (window == nativeHudWindows[9] && !(hints[3] & 0x20000U))
        InjectInputButtons(2);
    if (window == nativeHudWindows[14] && !g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].isVisible)
        InjectInputButtons(8);
}

// @verified - p
void RefreshNativeHudHints(std::uint32_t *hints) {
    if (static_cast<std::uint16_t>(hints[1]) == 0xffffU && (GetIconSelectFlags() & 0x200U))
        hints[1] = 44U;
    if (static_cast<std::uint16_t>(hints[0]) == 0xffffU && (GetIconSelectFlags() & 0x400U)) {
        hints[1] &= 0xffffU;
        hints[0] = (hints[0] & 0xffff0000U) | 45U;
    }
    if (GetQteControlState())
        hints[4] = 28U;
    else
        hints[4] |= 0xffffU;
}
// @verified - p
std::int64_t UpdateGameHudCameraVisibility() {
    const float alpha = IsCameraIconHidden() ? 1.0f : 0.0f;
    for (std::size_t index = 14; index < 17; ++index)
        if (nativeHudWindows[index])
            nativeHudWindows[index]->alpha = alpha;
    return 0;
}

// @verified - p
std::int64_t NativeGameHudNullsub() {
    return 0;
}

// @verified - p
std::int64_t NativeGameHudInputZero() {
    return 0;
}

// @verified - p
std::int64_t RenderNativeGameHud() {
    if (!nativeHudOverride)
        RefreshNativeHudHints(nativeHudShownHints.data());
    std::array<std::uint32_t, 5> hints = nativeHudOverride ? nativeHudOverrideHints : nativeHudShownHints;
    if (GetIsFreeInputType()) {
        const std::uint32_t first = hints[0];
        hints[0] = hints[1];
        hints[1] = first;
    }
    const std::int32_t keyboard = GetIsKeyboardActive();
    if (keyboard == 1) {
        if (nativeHudKeyboardMode == 0) {
            nativeHudKeyboardMode = 1;
            if (nativeHudWindows[0])
                nativeHudWindows[0]->layout.offsetY -= UI_HUD_KEYBOARD_Y_SHIFT;
            for (const std::size_t index : {14U, 7U, 4U, 13U, 10U})
                if (nativeHudWindows[index])
                    nativeHudWindows[index]->layout.offsetY += UI_HUD_KEYBOARD_Y_SHIFT;
            if (nativeHudCameraBase)
                nativeHudCameraBase->colour |= 0xff000000U;
            if (nativeHudButtonWidgets[4])
                nativeHudButtonWidgets[4]->colour &= 0xffffffU;
        }
    } else if (nativeHudKeyboardMode == 1) {
        nativeHudKeyboardMode = 0;
        if (nativeHudWindows[0])
            nativeHudWindows[0]->layout.offsetY += UI_HUD_KEYBOARD_Y_SHIFT;
        for (const std::size_t index : {14U, 7U, 4U, 13U, 10U})
            if (nativeHudWindows[index])
                nativeHudWindows[index]->layout.offsetY -= UI_HUD_KEYBOARD_Y_SHIFT;
        if (nativeHudCameraBase)
            nativeHudCameraBase->colour &= 0xffffffU;
        if (nativeHudButtonWidgets[4])
            nativeHudButtonWidgets[4]->colour |= 0xff000000U;
    }
    for (std::size_t index = 0; index < 5; ++index) {
        const std::uint32_t alpha =
            hints[index] & 0x10000U ? static_cast<std::uint8_t>(static_cast<std::int32_t>(GetUITransitionValue(nativeHudBlink) * 255.0f)) : 255U;
        nativeHudSpriteWidgets[index]->colour = (nativeHudSpriteWidgets[index]->colour & 0xffffffU) | (alpha << 24U);
    }
    for (std::size_t index = 0; index < 5; ++index) {
        if (hints[index] & 0x10000U)
            break;
        if (index == 4)
            RestartUITransition(nativeHudBlink);
    }
    for (std::size_t index = 0; index < 5; ++index) {
        UIButtonIconWidget *const sprite = nativeHudSpriteWidgets[index];
        if (!sprite)
            continue;
        if (index == 4) {
            const std::uint32_t alpha = static_cast<std::uint16_t>(hints[index]) == 0xffffU ? 0U : 0xff000000U;
            sprite->colour = (sprite->colour & 0xffffffU) | alpha;
            nativeHudButtonWidgets[4]->colour = (nativeHudButtonWidgets[4]->colour & 0xffffffU) | alpha;
            nativeHudButtonWidgets[4]->fontColour = (nativeHudButtonWidgets[4]->fontColour & 0xffffffU) | alpha;

            if (nativeHudCameraBase)
                nativeHudCameraBase->colour = (nativeHudCameraBase->colour & 0xffffffU) | alpha;
            if (alpha) {

                if (keyboard == 1)
                    nativeHudButtonWidgets[4]->colour &= 0xffffffU;
                else if (nativeHudCameraBase)
                    nativeHudCameraBase->colour &= 0xffffffU;
            }
            sprite->sprite = 2305;
            sprite->inventoryIcon = 1;
        } else {
            sprite->sprite = static_cast<std::int16_t>(hints[index]);
            sprite->inventoryIcon = 1;
            if (hints[index] & 0x20000U)
                sprite->colour = (sprite->colour & 0xffffffU) | 0x64000000U;
        }
    }
    if (nativeHudSpriteWidgets[2] && nativeHudSpriteWidgets[2]->sprite == 51 && nativeHudItemPromptIcon == 1)
        nativeHudSpriteWidgets[2]->sprite = 3847;
    for (std::size_t index = 0; index < 4; ++index) {
        nativeHudButtonWidgets[index]->controlState = GetCurrentUIInputType();
        const std::uint32_t alpha = nativeHudSpriteWidgets[index]->sprite == -1 ? 168U : 255U;
        nativeHudButtonWidgets[index]->colour = (nativeHudButtonWidgets[index]->colour & 0xffffffU) | (alpha << 24U);
    }
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::GameHud)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);

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
    hud.unknown28 = NativeGameHudInputZero;
    hud.reset = NativeGameHudNullsub;
    hud.postInit = NativeGameHudNullsub;
    hud.unknown88 = UpdateGameHudCameraVisibility;
    hud.render = RenderNativeGameHud;
    hud.fieldAc = 1;
    nativeHudTransitions.fill(-1);
    nativeHudBlink = -1;

    nativeHudShownHints.fill(0xffffU);
    nativeHudOverrideHints.fill(0xffffU);
    nativeHudRequestedHints.fill(0xffffU);
    ClearMenuInputHints();
    nativeHudOverride = false;
}
