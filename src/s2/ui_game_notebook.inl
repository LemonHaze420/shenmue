


std::int32_t gameNotebookLoadState{};
std::int32_t gameNotebookClosing{};
std::int32_t gameNotebookPendingOpen{};
std::int32_t gameNotebookTransition{-1};
std::int32_t gameNotebookWindow{-1};


void StartGameNotebookTransition(std::int32_t direction) {
    if (gameNotebookTransition == -1)
        return;
    UIAnim_Start(gameNotebookTransition, direction ? 0.0f : 1.0f, direction ? 1.0f : 0.0f, 0.3f, 0.0f);
}

void BindGameNotebook() {
    const auto &definition = uiDefinitions[static_cast<std::size_t>(UIElement::GameNotebook)];
    gameNotebookWindow = definition.panels.empty() ? -1 : 0;
    gameNotebookTransition = gameNotebookWindow == -1 ? -1 : UIAnimAllocSlot();
    if (gameNotebookPendingOpen && static_cast<std::uint32_t>(gameNotebookLoadState - 2) <= 1U) {
        gameNotebookClosing = 0;
        OpenMemoFromNotebookMenu();
        StartGameNotebookTransition(1);
    }
    gameNotebookPendingOpen = 0;
}

// The load step: read the view, then follow it to parsed (2) and ready (3).
std::uint32_t LoadGameNotebook() {
    const auto &element = g_uiElements[static_cast<std::size_t>(UIElement::GameNotebook)];
    if (gameNotebookLoadState == 0) {
        gameNotebookLoadState = static_cast<std::int32_t>(ReadUIDefinitionFile(UIElement::GameNotebook, "/ui/gamesubmenus/gamenotebook/GameNotebook.ui", 0));
    } else if (gameNotebookLoadState == 1) {
        gameNotebookLoadState = element.config.init >= 2 ? 2 : 1;
        if (gameNotebookLoadState == 3)
            BindGameNotebook();
    } else if (gameNotebookLoadState == 2) {
        gameNotebookLoadState = element.config.init == 3 ? 3 : 2;
    }
    return static_cast<std::uint32_t>(gameNotebookLoadState);
}

std::int64_t GameNotebookCallbackNullsub() {
    return 0;
}

std::int64_t DrawGameNotebook() {
    if (gameNotebookLoadState != 3)
        LoadGameNotebook();
    if (static_cast<std::uint32_t>(gameNotebookLoadState - 2) <= 1U && drawingUI)
        d3t::DrawUIDefinition(uiDefinitions[static_cast<std::size_t>(UIElement::GameNotebook)], 1.0f);
    return 0;
}

void OpenGameNotebook() {
    if (static_cast<std::uint32_t>(gameNotebookLoadState - 2) > 1U) {
        gameNotebookPendingOpen = 1;
        return;
    }
    gameNotebookClosing = 0;
    OpenMemoFromNotebookMenu();
    StartGameNotebookTransition(1);
}

// 1 lets the element hide: the memo was still open and has been asked to close.
std::int64_t CloseGameNotebook() {
    if (static_cast<std::uint32_t>(gameNotebookLoadState - 2) <= 1U && !gameNotebookClosing)
        StartGameNotebookTransition(0);
    gameNotebookClosing = 1;
    gameNotebookPendingOpen = 0;
    return CloseMemoFromNotebookMenu() == 0;
}

std::int64_t GameNotebookLoadState() {
    return gameNotebookLoadState;
}

void RegisterGameNotebook() {
    auto &notebook = g_uiElements[static_cast<std::size_t>(UIElement::GameNotebook)];
    notebook.name = "GAME_NOTEBOOK";
    notebook.parseDefinitionFromFile = LoadGameNotebook;
    notebook.unknown28 = GameNotebookCallbackNullsub;
    notebook.postInit = DrawGameNotebook;
    notebook.initialize = OpenGameNotebook;
    notebook.unknown60 = CloseGameNotebook;
    notebook.unknown78 = GameNotebookLoadState;
}