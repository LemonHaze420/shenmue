#include "s1/ui_internal.hpp"
#ifdef _DEBUG
#include "shenmue_internal.h"
#include "d3t/debug_font.hpp"
#endif

namespace shenmue::s1 {

extern void (*g_pfnDrawNuklear)();

void ReadSubtitleGlyphsAndSubs(std::int32_t language);

extern std::int32_t g_bIsNewGame;

extern std::int32_t g_busy_reset;

std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);

extern std::int32_t g_actNumber;

extern std::int32_t g_debugForceShowAllItems;

extern std::array<std::int16_t, 5> g_searchPromptIconSlots;

std::int32_t g_isPlayerGameStateActive = 7;

std::uint8_t g_previousHelpTokenId = 0;

std::uint8_t g_defaultHelpTokenId = 0;

std::uint8_t g_currentHelpTokenId = 0;

std::uint8_t g_helpButtonId = 0;

std::uint16_t g_currentHelpIconState = 0;

std::array<UIBaseRuntime, 80> g_uiElements{};

UIBaseRuntime *const g_cinemaBars = &g_uiElements[73];

std::int32_t g_currentSplashScreenIndex = 0;

IconPresentationOwner g_iconPresentation;

std::uint16_t g_tmpPadcHeld = 0;

std::uint32_t g_sfxInputFlags = 0;

std::uint16_t g_tmpRightStickHi = 0;

std::uint16_t g_tmpRightStickLo = 0;

std::int32_t g_conversationPromptActivationLock = 0;

std::int8_t g_cassetteCollectionUiState = 0;

std::int32_t g_optionsPendingAction = 0;

std::int64_t LoadSplash();

void UI_HELPInitCallback(HLib::TaskData *);

void UI_ICONMainCallback(HLib::TaskData *);

void UI_ICISMainCallback(HLib::TaskData *);

void UI_ICSSMainCallback(HLib::TaskData *);

void UI_ICLIMainCallback(HLib::TaskData *);

void UI_ICMCMainCallback(HLib::TaskData *);

void UI_ICSpMainCallback(HLib::TaskData *);

void UI_ICQuMainCallback(HLib::TaskData *);

void UI_ICNQMainCallback(HLib::TaskData *);

void UI_ICSVMainCallback(HLib::TaskData *);

void iconUpdateInputBuffer();

void DestroySharedItemsMenuIfTrueResponse();

bool shenPrepareCassetteUI();

void ResetUIStates();

std::int64_t GameMenu_IsPressed();

std::int64_t GameMenu_Tick();

void GameMenu_Render();

std::int64_t GameMenu_Destroy();

std::int64_t GameMenu_StartDraw();

std::int64_t CassettesMenu_IsSelected();

std::int64_t CassettesMenu_Reset();

std::int64_t CassettesMenu_IsPressed();

std::int64_t CassettesMenu_Tick();

void CassettesMenu_Render();

std::int64_t CassettesMenu_Destroy();

std::int64_t CassettesMenu_GetPromptIconSet();

std::int64_t GachaMenu_IsSelected();

std::int64_t GachaMenu_Reset();

std::int64_t GachaMenu_IsPressed();

std::int64_t GachaMenu_Tick();

void GachaMenu_Render();

std::int64_t CollectionMenu_IsSelected();

std::int64_t CollectionMenu_Reset();

std::int64_t CollectionMenu_IsPressed();

std::int64_t CollectionMenu_Tick();

void CollectionMenu_Render();

std::int64_t CollectionMenu_Destroy();

std::int64_t CollectionMenu_ElementDraw();

std::int64_t CollectionMenu_GetPromptIconSet();

std::int64_t ScrollsMenu_IsSelected();

std::int64_t ScrollsMenu_Reset();

std::int64_t ScrollsMenu_IsPressed();

std::int64_t ScrollsMenu_Tick();

void ScrollsMenu_Render();

void KeepOrRevertSettings(UIRetry70ManBattlePromptResult result);

bool Load3DCassetteModelForInventory(std::int16_t id, bool cassette);

void DestroySharedItemsMenu();

void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags);

std::int64_t BatteryHud_IsSelected();

std::int64_t BatteryHud_IsPressed();

std::int64_t BatteryHud_Tick();

std::int64_t BatteryHud_GetPromptIconSet();

std::int64_t OptionsMenu_IsSelected();

std::int64_t OptionsMenu_GetContentPanel();

std::int64_t OptionsMenu_Reset();

std::int64_t OptionsMenu_Tick();

void OptionsMenu_Render();

std::int64_t OptionsMenu_IsPressed();

std::int64_t OptionsMenu_Destroy();

std::int64_t OptionsMenu_GetPromptIconSet();

std::int64_t OptionsControls_IsSelected();

std::int64_t OptionsControls_Reset();

std::int64_t OptionsControls_IsPressed();

std::int64_t OptionsControls_Tick();

void OptionsControls_Render();

std::int64_t OptionsAudio_IsSelected();

std::int64_t OptionsAudio_Reset();

std::int64_t OptionsAudio_IsPressed();

std::int64_t OptionsAudio_Tick();

void OptionsAudio_Render();

std::int64_t OptionsGraphics_IsSelected();

std::int64_t OptionsGraphics_Reset();

std::int64_t OptionsGraphics_IsPressed();

std::int64_t OptionsGraphics_Tick();

void OptionsGraphics_Render();

extern std::int32_t g_searchCandidateId;

extern std::int32_t g_currentSearchState;

extern std::array<std::uint8_t, 128> g_shenmueFlags;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

std::array<std::vector<std::byte>, 7> g_subtitleData;

std::array<std::vector<std::byte>, 7> g_subtitleGlyphData;

std::array<std::vector<std::byte>, 80> g_uiDefinitions;

std::array<UIDefinition, 80> g_parsedUIDefinitions;

std::array<std::atomic<bool>, 80> g_uiDefinitionReady{};






std::array<HelpNewRun, 64> g_helpNewRuns{};

std::int32_t g_helpNewRunCount = 0;

std::int32_t g_helpNewLineCount = 0;

std::array<UIButtonIconWidget, 32> g_helpNewIcons{};

std::array<std::int32_t, 32> g_helpNewIconActions{};

std::int32_t g_helpNewIconCount = 0;

std::array<HelpNewInfoBlock, 4> g_helpNewInfoBlocks{};

std::int32_t g_helpNewInfoBlockCount = 0;

std::array<d3t::UIWindow *, 2> g_helpNewWindows{};

UIStyleItem *g_helpNewBackground = nullptr;

std::int32_t g_helpNewTransitionAnimSlot = -1;

std::int32_t g_helpNewEmulatorContext = 0;

std::int32_t g_helpNewEmulatorPrompt = 0;

UILayoutRect g_helpNewTitleRect{};

UILayoutRect g_helpNewBodyRect{};

std::array<char, 64> g_helpNewTitleText{};

std::array<char, 128> g_helpNewBodyText{};

std::int32_t g_helpNewNextIcon = 0;

std::uint8_t g_helpNewRelayout = 0;

std::uint8_t g_helpFlag = 0;

std::int32_t g_helpNewKeyboardActive = -1;

std::array<std::int16_t, 4> g_helpNewPromptIconSet{-1, 0, -1, -1};

nk_panel g_helpNewPanel{};




std::array<char, 1024> g_helpFormattedText{};

std::uint32_t g_helpFormattedLength = 0;

std::uint32_t g_helpColumnSkip = 0;

std::uint32_t g_helpLineCounter = 0;

std::array<HelpIconRecord, 10> g_helpIconRecords{};

std::int16_t g_helpIconRecordCount = 0;

std::array<std::int16_t, 20> g_helpLineWidths{};

std::uint8_t g_helpColumnOffset = 0;

std::array<std::array<char, 128>, 32> g_helpLegacyLines{};

std::int32_t g_helpLegacyLineCount = 0;

std::array<std::int32_t, 12> g_helpLegacyIconIds{};

std::array<std::uint64_t, 12> g_helpLegacyIconLines{};

std::array<float, 12> g_helpLegacyIconColumns{};

std::int32_t g_helpLegacyIconCount = 0;




std::int32_t g_helpTextRunLength = 0;

std::int32_t g_helpTextWidth = 0;

std::int32_t g_helpTextMaxWidth = 0;

std::array<char, 2048> g_currentHelpText{};

std::array<std::int16_t, 20> g_helpIconSurfaceIds{};

std::uint8_t g_moveListScreenState = 0;

std::int32_t g_debugHelpPaging = 0;




std::int32_t g_fightGaugeDisplayMode = 1;

std::array<std::int32_t, 2> g_fightGaugeWidgetIndices{0, 1};

std::array<std::int32_t, 2> g_fightGaugeScissorIndices{};

std::uint32_t g_fightGaugeFrameFlags = 0;

float g_vfGaugeHealthPercent = 0.0f;

std::array<char, 512> g_gameHudWatchTime{};

std::array<ActionSelectorChoiceUISlot, 4> g_actionSelectorChoiceSlots{};



ActionSelectorChoiceUISlot *g_actionSelectorSelectedChoiceSlot = nullptr;

std::array<d3t::UIWindow *, 4> g_actionSelectorChoicePanels{};

std::array<d3t::UIWindow *, 3> g_actionSelectorMainPanels{};

UIButtonIconWidget *g_actionSelectorCenterIcon = nullptr;

d3t::UIImageWidget *g_actionSelectorPhoenixImage = nullptr;

std::array<float, 2> g_batteryLifeFractions{1.0f, 1.0f};

static_assert(sizeof(MessageBoxState) == 8);





HLib::Task *g_sicnTask = nullptr;

std::uint8_t g_sicnTaskActive = 0;

std::uint16_t g_uiState = 0;

std::int16_t g_sicnTransientStateTimeoutFrames = 0;

UISicnPausePolicy g_currentSicn = UISicnPausePolicy::ClearOnPause;

std::array<SicnPromptSlot, 12> g_sicnPromptSlots{};

std::uint16_t g_hudPromptFlagsThisFrame = 0;

bool g_sicnPopulatedHudPromptsThisFrame = false;

bool g_sicnClearPendingUntilUnpause = false;

std::int32_t Shenmue_nPrevPauseState = 0;

std::int32_t g_sfxFlashOnFrames = 0;

std::int32_t g_sfxTimedOutMarker = 0;

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

UISfxOverlayState g_sfxOverlayState = UISfxOverlayState::Idle;


std::uint8_t g_helpOverlayPageTokenId = 0;

UIHelpWidgetState g_helpOverlayState = UIHelpWidgetState::Idle;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

std::uint8_t g_sfxOverlayFlags = 0;

std::uint16_t g_currentSfxMask = 0;

std::int16_t g_sfxTimeoutFrames = 0;

std::int16_t g_sfxFrameCounter = 0;

std::array<std::int16_t, 2> g_sfxOverlayResult{};

SfxOverlayBoostFrames g_qteOverlay{};

std::int16_t g_sfxOverlayTimeoutPendingBonusFrames = 0;

std::int16_t g_qteOverlayTimeoutMaxFrames = 0;

std::int32_t g_sfxOverlayTimeoutBoostEnabled = 0;

std::int32_t g_sfxFlashOffFrames = 0;

std::int32_t g_sfxOverlayTimeoutBoostCount = 0;

std::uint8_t g_sfxFlashCounter = 0;

std::uint8_t g_sfxStateDirty = 0;

HLib::Task *g_sfxOverlayTask = nullptr;

HelpPresentationOwner g_helpPresentation;

ActionSelectorOwner g_actionSelectorPresentation;

CashOverlayOwner g_cashOverlayPresentation;

UIActionSelectorElement g_actionSelectorElement;

MessageBoxState g_messageBoxState{};



std::int32_t g_yesNoPromptTimeoutSlot = -1;

std::int32_t g_messageBoxSelection = 0;

std::int32_t g_yesNoPromptTransitionSlot = -1;

UIRetry70ManBattlePromptCallback g_messageBoxCallback = nullptr;

const char *g_messageBoxToken = nullptr;

std::uint8_t g_messageBoxUseString = 0;

float g_messageBoxTimeout = 0.0f;


UIRetry70ManBattlePromptCallback g_retry70ManBattleFlowCallback = nullptr;

SeventyManBattlePresentationOwner g_seventyManBattlePresentation;

MoveLearnedPresentationOwner g_moveLearnedPresentation;

UIGameMessagePresentation g_gameMessagePresentation;



std::array<d3t::UIWindow *, 5> g_itemsGridWindows{};

d3t::UILabelWidget *g_itemsGridPlaceholderLabel = nullptr;

std::array<d3t::UILayoutItem *, 2> g_itemsGridLayoutItems{};

d3t::UIImageWidget *g_itemsGridPlaceholderImage = nullptr;

UIGridTemplate g_itemsGridTemplate{};

UIGrid g_itemsGrid{};

std::array<UIGridEntry, 256> g_itemsGridElements{};

std::int32_t g_itemsGridElementCount = 0;




std::array<d3t::UIWindow *, 6> g_cassettesWindows{};

std::array<d3t::UILayoutItem *, 2> g_cassettesLayoutItems{};

UIButtonIconWidget *g_cassettesSpeakers = nullptr;

UIGridTemplate g_cassettesGridTemplate{};

UIGrid g_cassettesGrid{};

std::array<UIGridEntry, 32> g_cassettesGridElements{};

std::int32_t g_cassettesGridElementCount = 0;



std::array<d3t::UIWindow *, 5> g_gachaWindows{};

d3t::UILabelWidget *g_gachaPlaceholderLabel = nullptr;

std::array<d3t::UILayoutItem *, 2> g_gachaLayoutItems{};

d3t::UIImageWidget *g_gachaPlaceholderImage = nullptr;

UIGridTemplate g_gachaGridTemplate{};

UIGrid g_gachaGrid{};

std::array<UIGridEntry, 512> g_gachaGridElements{};

std::int32_t g_gachaGridElementCount = 0;




std::array<d3t::UIWindow *, 5> g_scrollsWindows{};

std::array<d3t::UILayoutItem *, 2> g_scrollsLayoutItems{};

d3t::UIImageWidget *g_scrollsPlaceholderImage = nullptr;

UIGridTemplate g_scrollsGridTemplate{};

UIGrid g_scrollsGrid{};

std::array<UIGridEntry, 9> g_scrollsGridElements{};

std::int32_t g_scrollsGridElementCount = 0;

UIGridEntry g_scrollsEmptyElement{};




UIGridTemplate g_movesGridTemplate{};

std::array<d3t::UIWindow *, 25> g_movesWindows{};

std::array<d3t::UILabelWidget *, 2> g_movesLabels{};

std::array<UIColourImageWidget *, 5> g_movesColourImages{};

std::array<d3t::UILayoutItem *, 28> g_movesLayoutItems{};

std::array<UIButtonIconWidget *, 7> g_movesButtonIcons{};

std::array<UIButtonWidget *, 8> g_movesButtons{};

UIGrid g_movesGrid{};

std::array<std::array<char, 256>, 2> g_movesLabelTexts{};




std::int32_t g_uiGridInputEnabled = 0;

UIFocusList *g_uiFocusedList = nullptr;

UIGrid *g_uiCurrentGrid = nullptr;


nk_panel g_uiGridPanel{};

nk_panel g_uiGridScrollBarPanel{};






std::uint8_t g_icmcCassetteIsPlayerActive = 0;

std::uint8_t g_icmcCassetteSelectedIndex = 0;

std::uint8_t g_cassetteBaseIndex = 0;

std::int8_t g_cassetteIndex = -1;

std::uint8_t g_cassettePlaying = 0;

std::uint16_t g_cassetteOwnedCount = 0;

std::uint16_t g_cassetteSavedHelpToken = 0;

std::int16_t g_cassetteSavedHelpButton = 0;

CVWKRuntimeSlot *g_currentCassetteDialogue = nullptr;

std::uint8_t g_cassetteVisibleCount = 1;

PadCaptureEntry *g_icmcInput = nullptr;

PadCaptureEntry *g_cassetteUiCachedInput = nullptr;

std::array<std::int16_t, 9> g_cassetteSurfaces{-1, -1, -1, -1, -1, -1, -1, -1, -1};

std::int32_t g_cassettePlayRequested = 0;

CassetteSoundState g_cassetteSound{};

std::uint8_t g_cassettePlayFlag = 0;




std::int32_t g_cassetteSpeakerAnimSlot = -1;

std::uint8_t g_cassetteMusicNoteVisible = 0;

std::array<float, 4> g_cassetteMusicNoteRect{};

std::int32_t g_cassetteMusicNoteAnimSlot = -1;

std::int32_t g_cassetteBatteryHudOwned = 0;

nk_panel g_cassetteMusicNotePanel{};

std::array<std::int32_t, 5> g_cassettePromptIconSet{};



std::int32_t g_userResponse = 0;

std::uint64_t g_uiRejectSoundFrame = 0;

SharedItemsPreviewRequest g_sharedItemRequest{};

std::int32_t g_sharedItemsRequested = 0;

std::int32_t g_batteryHudIndex = 0;


std::int32_t g_batteryHudAnimSlot = -1;

UIButtonIconWidget *g_batteryHudSprite = nullptr;

std::array<std::int32_t, 5> g_batteryPromptIconSet{};



std::int32_t g_gameMenuSubmenu = 0;

std::int32_t g_gameMenuPendingSubmenu = 0;

std::int32_t g_gameMenuCloseState = 0;

std::array<char, 256> g_gameMenuMoneyText{};

std::array<char, 256> g_gameMenuTokenText{};

std::array<char, 256> g_gameMenuDateText{};




constexpr std::array<const char *, 12> kGameMenuWindowIds{"GameMenu::BackgroundTab_00", "GameMenu::BackgroundTab_01", "GameMenu::BackgroundTab_02",
    "GameMenu::BackgroundTab_03", "GameMenu::BackgroundTab_04", "GameMenu::SelectedTab", "GameMenu::LeftIconPanel", "GameMenu::RightIconPanel",
    "GameMenu::PlayerInfoParent", "GameMenu::PlayerInformationBar_Date_Value", "GameMenu::PlayerInformationBar_Tokens_Value",
    "GameMenu::PlayerInformationBar_Money_Value"};

constexpr std::array<const char *, 8> kGameMenuLabelIds{"Label_PlayerInfoBar_Money", "Label_PlayerInfoBar_Tokens", "Label_PlayerInfoBar_Calendar",
    "GameMenu::Label_Items", "GameMenu::Label_Collection", "GameMenu::Label_Moves", "GameMenu::Label_Save", "GameMenu::Label_Options"};

constexpr std::array<const char *, 5> kGameMenuIconIds{
    "GameMenu::Icon_Items", "GameMenu::Icon_Collection", "GameMenu::Icon_Moves", "GameMenu::Icon_Save", "GameMenu::Icon_Options"};

std::array<d3t::UIWindow *, 12> g_gameMenuWindows{};

std::array<d3t::UILabelWidget *, 8> g_gameMenuLabels{};

std::array<UIColourImageWidget *, 5> g_gameMenuIcons{};

OwnedSpriteSet<32> g_commonIconSprites;

bool g_conversationPromptRuntimeReady = true;

bool g_conversationPromptSceneEnabled = true;

bool Options_SettingsChanged();

void AssignMultipleChoice();


constexpr std::array<std::int16_t, 40> kHelpTextRunWidths{64, 64, 64, 64, 128, 128, 128, 128, 128, 256, 256, 256, 256, 256, 256, 256, 256, 256, 512, 512, 512,
    512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 1024, 1024, 0, 0};

constexpr std::array<const char *, 68> kHelpStringIds{"UIHELPFREQ", "UIHELPSEAQ", "UIHELPINVE", "UIHELPNEOD", "UIHELPEXCI", "UIHELPBILL",
    "UIHELPCAPS", // @text UIHELPFREQ EN: "%[Free Quest]%% @^XNotebook@^ZZoom%@^Y Item@^N Look%%@^W Run@ ^A Action% @^C Walk@^B Cancel%@ ^B Stop%" | JA: "%【フリークエスト】%% @^Xメモ帳@^Zズーム%@^Yアイテム@^N見る%%@^W走る@ ^Aアクション% @^C歩く@^Bキャンセル%@ ^B止まる%" ; UIHELPSEAQ EN: "%[Search(Quest)]%%Use these commands to examine%various objects.%% @^A Pick Up@ ^L Zoom out%@ ^A Open@ ^R Zoom in%@ ^B Cancel@ ^N @^C Move to the next object" | JA: "%【探索】%%様々な物を調べることができます。%%% @^Aつかむ@ ^L縮小%@ ^A開ける@ ^R拡大%@ ^Bキャンセル@ ^N @^C次の対象に移る" ; UIHELPINVE EN: "%[Investigate]%%Use these commands to examine the object.%%@^A Pick up@ ^L Zoom out%@^B Put down@ ^R Zoom in%@^B Cancel@ ^N Look%@@ ^C Rotate%" | JA: "%【探索】%%様々な物を調べることができます。%%@^A取る@ ^L縮小%@^B戻す@ ^R拡大%@^Bキャンセル@ ^N見る%@@ ^C回転%" ; UIHELPNEOD EN: "[NEO Darts]%%Compete by throwing darts to earn a high%score. Focus on the motion of the arm,%and press^Aat just the right moment to%throw the dart. 100¥ per game.%%@^AThrow the dart%%Darts is a sport started by English%soldiers during the War of the Roses.%" | JA: "【ＮＥＯダーツ】%%矢を投げて、合計得点を競います。%腕の動きを見て、タイミング良く%^Aボタンを押してください。%１ゲーム１００円です。%%@^A矢を投げる%%ダーツは、バラ戦争時、英国兵士によって%生み出された伝統ある室内競技です。%" ; UIHELPEXCI EN: "[Excite QTE 2]%%Match the input displayed within the allotted time. You only have 3%chances, think you can do it? One%game costs 100¥. Give it a try now!%%Use ^D ^A ^B ^X ^Y%%Excite QTE is back with more excitement!%See if you can beat the high score!%" | JA: "【エキサイトＱＴＥ２】%%制限時間内に画面に表示されるボタンや方向をすばやく%入力するんだ。ミスは３回まで。ついてこれるかな？%１ゲーム１００円、即チャレンジだ！%%使用するボタン／スティック： ^C ^A ^B ^X ^Y%%エキサイトＱＴＥがさらにパワーアップ。%ハイスコアをぬりかえろ！%" ; UIHELPBILL EN: "%[Billiards]%%From the overhead view, assess your%position and consider how to aim.%Press^A to switch to close-up and%use the ^C to fine-tune your aim.%Press^B to return to the previous view.%Press^A to shoot the cue ball.%Sink the 9 ball in the pocket to win.%" | JA: "%【ビリヤード】%%上から見ている状態で、ある程度狙いを定めます。%^Aを押すと覗きこむ状態になるので、%^Cで微調整してください。%^Bで前の状態に戻ります。%再度^Aを押すと白玉を突きます。%９番の玉をポケット（穴）に落とせば勝ちです。%" ; UIHELPCAPS EN: "%[Capsule Toy Machine]%%There are a variety of toys available.%The fun lies in seeing what you can get!%100¥ per capsule%%@^A Pick up@ ^L Zoom out %@@^R Zoom in %@@^N Look %@@^C Rotate%" | JA: "%【ガチャガチャ】%%いろいろなアイテムがあります。%何が出るかは、お楽しみです。%１回１００円です。%%@^A取る@ ^L縮小%@@^R拡大%@@^N見る%@@^C回転%"
    "UIHELPJUKE", "UIHELPTELE", "UIHELPPAYP", "UIHELPRYOR", "UIHELPSAVE", "UIHELPSETT", "UIHELPINTE", "UIHELPMOVE", "UIHELPPRAC", "UIHELPTRAI", "UIHELPSYST", // @text UIHELPJUKE EN: "%[Jukebox]%%Enjoy your favorite BGM song. Choose%a tune you like from the 10 selections.%100¥ per tune.%% @^C Select a tune @^A Enter%" | JA: "%【ジュークボックス】%%パネルに書かれた全１０曲の中から好きな曲を選べます。%あなた好みのＢＧＭをお楽しみください。%１曲１００円です。%% @^C曲の選択@^N見る%%@@^A曲の決定%" ; UIHELPTELE EN: "%[Telephone]%%Use these commands to make a phone call.%Position Ryo's finger on the number to%dial, and then press^A.%% @^C Select a number @^A Dial %@@^B Cancel %@@^X Notebook%" | JA: "%【電話】%%ダイヤルしたい番号に指先を合わせ、%^Aを押してください。%電話をかけることができます。%% @^C左右で番号選択@^Aダイヤルを回す%@@^Bキャンセル%@@^Xメモ帳%" ; UIHELPPAYP EN: "[Pay Phone]%%Use these commands to make a phone call.%Position Ryo's finger on the number to%dial, and then press^A.%Each call costs 10¥.%% @^C Select a number @^A Dial %@@^B Cancel %@@^X Notebook%" | JA: "【公衆電話】%%ダイヤルしたい番号に指先を合わせ、%^Aを押してください。%電話をかけることができます。%１回の通話ごとに１０円かかります。%% @^C左右で番号選択@^Aダイヤルを回す%@@^Bキャンセル%@@^Xメモ帳%" ; UIHELPRYOR EN: "%[Ryo's Room]%%Save: Save your game data to a file.%Setting: Check/Modify the game settings.%Sleep: Replenish Ryo's power.%Training: Review/Modify Ryo's training.%%Sleep & Training only appear after 20:00.%" | JA: "%【涼の部屋】%%記録：ここまでのゲーム進行を記録%設定：システムの確認、変更%寝る：体力回復%修行：修行方法の確認、変更%%「寝る」、「修行」は２０：００以降に利用可能です。%" ; UIHELPSAVE EN: "%%[Save]%%Save your game data to a file.%A total of 3 files can be created.%%@^CFile selection@ ^AEnter%@@^BCancel%" | JA: "%%【記録】%%ここまでのゲーム進行を記録できます。%記録しておけるゲームデータは１０個までです。%%@^Cデータ選択@ ^A決定%@@^Bキャンセル%" ; UIHELPSETT EN: "%%[Setting]%%Check or modify the game settings.%%@^C Select@^A Enter %@@^B Cancel%" | JA: "%%【設定】%%システムの確認、変更ができます。%%@^C選択@^A決定%@@^Bキャンセル%" ; UIHELPINTE EN: "[Intensive Training]%%The bar graph indicates Ryo's skill level.%Red shows a move set for priority training.%Green shows the move set to ^R.%Input commands when Ryo is facing right.%%@^CSelect@ ^RZoom%@^AEnter@ ^NRotate%@^XMove explanation%@^LToggle display@ ^BReturn%" | JA: "【重点的な修行】%%棒グラフは、技の「熟練度」を表しています。%まだ修得していない技は、技名が暗くなっています。%重点的に修行したい技を選択してください。%登録されている技はコマンドに^Rが表示されています。%コマンドは、涼が右向きのときのものです。%%@^C選択@@^A決定@ ^N回転%@ ^B戻る%" ; UIHELPMOVE EN: "[Move Instruction]%%Follow the instructions, and input the%commands for the move. Moves which Ryo%masters can be used during Free Battles.%%@^C Command @^K Leg Move %@@^O Throw Move %@@^P Hand Move %@@^G Block/Evade%" | JA: "【技伝授】%%教えを頼りに、%技コマンドを入力してください。%覚えた技は、フリーバトルで使えます。\n%%@^Cコマンド@^K足技%@@^O投技%@@^P手技%@@^G防御/避ける%" ; UIHELPPRAC EN: "%[Practice]%%After exiting “Help” or in a few minutes,%an AS asking “continue” or “quit” appears.%%@^L Run@ ^K Leg Move%@^C Walk@ ^O Throw Move%@^C Command@ ^P Hand Move% @@^G Block/Evade%@^R See All Moves%" | JA: "%【練習中】%%ヘルプを抜けるか、一定時間過ぎると%練習を続けるかどうか聞いてきます。%%@^L走る@ ^K足技%@^C歩く@ ^O投技%@^Cコマンド@ ^P手技% @@^G防御/避ける%@^R全技巻物を見る%" ; UIHELPTRAI EN: "[Training]%%???%" | JA: "【修行】%%？？？%" ; UIHELPSYST EN: "%%[System Menu]%%Examine items in Ryo's possession, modify%the game settings or execute +Resume+.%%@^C Move cursor @^A ^Y Enter %@@^B Return%" | JA: "%%【システムウィンドウ】%%アイテムなどを見ることができます。%また、記録や、設定の変更ができます。%%@^Cカーソル移動@^A ^Y決定%@@^B戻る%"
    "UIHELPITEM", "UIHELPNOTE", "UIHELPFREB", "UIMISS0000", "UIHELPMOTO", "UIHELPFORK", "UIHELPWRIS", "UIHELPVEN1", "UIHELPFLAS", "UIHELPQTET", "UIHELPSLOT", // @text UIHELPITEM EN: "%[Item]%%Use these commands to examine objects.%You can also zoom in and zoom out.%%In some situations you can use^Aor^Y.%%@^L Zoom out@^B Return%@^R Zoom in%@^C Rotate%%" | JA: "%【アイテム】%%回転させて、見ることができます。%拡大、縮小することもできます。%%^A^Y：状況に応じて、使うことができます。%%@^L縮小@^B戻る%@^R拡大%@^C回転%%" ; UIHELPNOTE EN: "%[Notebook]%%Information important to the game%progression appears in this notebook.%% @^H Change Page @^B Return%@^8First Page%@^2Last Page%" | JA: "%【メモ帳】%%物語の進行に必要な事柄が、%メモされていきます。%% @^Hページめくり@^B戻る%@^8最初のページ%@^2最新のページ%" ; UIHELPFREB EN: "%%[Free Battle]%%@^L Run@ ^K Leg Move%@^C Walk@ ^O Throw Move%@^C Command@ ^P Hand Move%@@^GBlock/Evade%@^R Set move%" | JA: "%%【フリーバトル】%%@^L走る@ ^K足技%@^C歩く@ ^O投技%@^Cコマンド@ ^P手技%@@^G防御/避ける%@^R登録技%" ; UIHELPMOTO EN: "%[Motorcycle]%% Arrive at the harbor before the%R.TIME (remaining time)%reaches 0'00+.%%@^R Accelerator %@^L Brake %@^C Steering%" | JA: "%【バイク】%%ＴＩＭＥ（残りタイム）が%０’００”になるまでに、%港にたどりついてください。%%@^Rアクセル%@^Lブレーキ%@^Cハンドル%" ; UIHELPFORK EN: "[Forklift]%%@^RMove forward@ ^BChange%@^LMove back@ ^   camera view%@^CSteering wheel%%@^XMap@ ^ARaise/lower%@^YItem@ ^   forklift%%Place the crates within the white%dotted lines.%" | JA: "【フォークリフト】%%@^R前進@ ^B視点切り替え%@^L後退@%@^Cハンドル%%@^Xマップ@ ^Aフォークの上下%@^Yアイテム@ %%荷物は倉庫の白線内に置きます。%" ; UIHELPWRIS EN: "%%%[Wristwatch]%%This wristwatch keeps the exact time.%%@^A Light @^B Return%" | JA: "%%%【時計】%%肌身離さず身につけている、お気に入りの腕時計です。%%@^Aライト@^B戻る%" ; UIHELPVEN1 EN: "%%[Vending Machine]%%Select the drink of your choice.%100¥%%@^C Select @^A Buy %@@^B Cancel%" | JA: "%%【自動販売機】%%好きな飲み物を選んでください。%一缶１００円です。%%@^C飲み物の選択@^A買う%@@^Bキャンセル%" ; UIHELPFLAS EN: "%%%[Flashlight]%%Use the flashlight to light up dark areas.%To use, open the system menu and select%Press^A again to turn the flashlight off.%" | JA: "%%%【懐中電灯】%%暗闇を明るく照らし出す懐中電灯です。%システムウィンドウを開き、%もう一度使う（^A）と消灯します。%" ; UIHELPQTET EN: "[QTE Title]%%Match the input displayed within%the allotted time.You only have 3%chances. One game costs 100¥%%Use: ^A ^B ^X%%Punch down gloves as they move forward.%Think you can take on the champion?!%" | JA: "【ＱＴＥ　ＴＩＴＬＥ】%%表示されるアイコンに合わせて、%ボタンをすばやく押すんだ。%ミスは３回まで。１ゲーム１００円。%%使用するボタン：^A ^B ^X%%起き上がってくる的（まと）をパンチで倒せ。%さあ、君もチャンピオンに挑戦だ！%" ; UIHELPSLOT EN: "[Slot Machine]%%100¥ buys 20 tokens.%Remaining coins can be kept at the counter.%@BET:@Bet%@COIN:@Bet tokens%@START:@Start%@PAY OUT:@Collect winnings%%@^CSelect@ ^AEnter%@^XTotal Tokens@ ^BReturn%" | JA: "【スロットマシン】%%１００円で、コイン２０枚です。%あまったコインは、店を出る際に預かります。%@ＢＥＴ：@賭ける%@ＣＯＩＮ：@賭ける%@ＳＴＡＲＴ：@スタート%@ＰＡＹ　ＯＵＴ：@払い出し%%@^C選択@ ^A決定%@^Xコイン合計@ ^B戻る%"
    "UIHELPOBSE", "UIHELPSNEA", "UIHELPMTAM", "UIHELPMTHM", "UIHELPMTLM", "UIHELPMTTM", "UIHELPQTE0", "UIHELPAS00", "UIHELPARCA", "UIHELPCONV", "UIHELPLING", // @text UIHELPOBSE EN: "%%[Observe]%%Use these commands to search and examine.%%@^A Pick up@ ^L Zoom out%@^A Put down@ ^R Zoom in%@^B Cancel@ ^C Look%" | JA: "%%【調べる】%%見回し、調べる事ができます。%%@^Aつかむ@ ^L縮小%@^A戻す@ ^R拡大%@^Bキャンセル@ ^C見る%" ; UIHELPSNEA EN: "[Sneak into the Old Warehouse Area]%%%Find Warehouse No.8%without being caught by the security guards.%%You can hide by pressing^A%near objects and corners.%%You can check the number of the warehouse%by pressing^A.%" | JA: "【旧倉庫街潜入】%%%警備員に見つからずに、第８倉庫を探して下さい。%%物陰や、曲がり角で^Aを押せば、隠れる事ができます。%%倉庫の番号は、^Aで確認することができます。%" ; UIHELPMTAM EN: "[Modify Training / All Moves]%%Modify Ryo's method of training.%%@^C Select @^A Enter %@@^B Cancel%%All moves: Practice all moves equally.%Benefits of this practice will be%distributed equally over all the%available moves.%" | JA: "【修行方法の変更・全技】%%修行方法を変える事ができます。%%@^C選択@^A決定%@@^Bキャンセル%%全技：全ての技を均等に練習します。%練習によって得られる効果も、%均等に振り分けられます。%%" ; UIHELPMTHM EN: "[Modify Training / Hand Moves]%%Modify Ryo's method of training.%%@^C Select @^A Enter %@@^B Cancel%% Hand moves: Practice will be focused on%hand moves. Hand moves will become more%effective compared to leg moves and%throw moves.%" | JA: "【修行方法の変更・手技】%%修行方法を変える事ができます。%%@^C選択@^A決定%@@^Bキャンセル%%手技：手技を重点的に練習します。%足技、投技に比べ、%手技は高い効果を得られます。%%" ; UIHELPMTLM EN: "[Modify Training / Leg Moves]%%Modify Ryo's method of training.%%@^C Select @^A Enter %@@^B Cancel%%Leg moves: Practice will be focused on%leg moves. Leg moves will become more%effective compared to hand moves and%throw moves.%" | JA: "【修行方法の変更・足技】%%修行方法を変える事ができます。%%@^C選択@^A決定%@@^Bキャンセル%%足技：足技を重点的に練習します。%手技、投技に比べ、%足技は高い効果を得られます。%%" ; UIHELPMTTM EN: "[Modify Training / Throw Moves]%%Modify Ryo's method of training%%@^C Select @^A Enter %@@^B Cancel%%Throw moves: Practice will be focused on%throw moves. Throw moves will become more%effective compared to hand moves and%leg moves.%" | JA: "【修行方法の変更・投技】%%修行方法を変える事ができます。%%@^C選択@^A決定%@@^Bキャンセル%%投技：投技を重点的に練習します。%手技、足技に比べ、%投技は高い効果を得られます。%%" ; UIHELPQTE0 EN: "%[QTE]%%Match the input displayed%within the allotted time.%%Use: ^C ^A ^B ^X ^Y%%QTE stands for%Quick Timer Event.%" | JA: "【ＱＴＥ】%%中央にアイコンが表示されます。%アイコンに合わせ、表示中にボタンや左スティックを%すばやく入力してください。%%@使用するボタン／スティック：^C^A^B^X^Y@%%ＱＴＥとは、クイック・タイマー・イベントの略です。%" ; UIHELPAS00 EN: "%%%[AS: Action Selector]%%Use the ^C to select an action.%%@^C Enter selection%" | JA: "%%%【ＡＳ：アクション・セレクター】%%選択したい項目に対応する^Cキーを入力してください。%%@^C選択決定%" ; UIHELPARCA EN: "%%[Arcade]%%To play an arcade game, position%Ryo in front of the game and press^A.%Each game is 100¥ per play.%%@^A Play the game.%" | JA: "%%【ゲームセンター】%%ゲーム機の前で^Aを押すと、%そのゲームで遊ぶことができます。%それぞれ、１ゲーム１００円です。%%@^Aゲームをする%" ; UIHELPCONV EN: "%%[Conversation]%%If Ryo is able to converse with%someone, an icon will appear in%the lower right corner.%%^A Speak  ^B Skip%" | JA: "%%【会話】%%会話できる場所では、%右下にアイコンが表示されます。%%%^A会話^Bスキップ%" ; UIHELPLING EN: "%[Ling Sha Hua]%%@^CCamera@ ^LZoom Out%@^NLight@ ^RZoom In%%@^ASubtitles ON@ ^BCancel%%Pure and naive.%Sha Hua is a mysterious young woman with%a deep and compassionate character.%" | JA: "%【玲莎花】%%@^Cカメラ@ ^L縮小%@^Nライト@ ^R拡大%%@^A字幕ＯＮ@ ^Bキャンセル%%純粋で素朴、%そして深い優しさを持った%不思議な少女。%"
    "UIHELPRYOH", "UIHELPHONG", "UIHELPMARK", "UIHELPCCNV", "UIHELPSNE2", "UIHELPHAZU", "UIHELPLOCK", "UIHELPDART", "UIHELPCHAI", "UIHELPKEIZ",
    "UIHELPNOZO", // @text UIHELPRYOH EN: "%[Ryo Hazuki]%%@^CCamera@ ^LZoom Out%@^NLight@ ^RZoom In%%@^ASubtitles ON@ ^BCancel%%A young man who has succeeded as master%to his father Iwao, the originator of%the Hazuki martial art style.%" | JA: "%【芭月涼】%%@^Cカメラ@ ^L縮小%@^Nライト@ ^R拡大%%@^A字幕ＯＮ@ ^Bキャンセル%%父、巌の創出した芭月流柔術。%その技と心を受け継いだ少年。%%" ; UIHELPHONG EN: "%[Hong Xiu Ying]%%@^CCamera@ ^LZoom Out%@^NLight@ ^RZoom In%%@^ASubtitles ON@ ^BCancel%%This woman who harbors a strong love of%traditional Chinese culture, has eyes%that appear to conceal a tale of sorrow.%" | JA: "%【紅秀瑛】%%@^Cカメラ@ ^L縮小%@^Nライト@ ^R拡大%%@^A字幕ＯＮ@ ^Bキャンセル%%中国の伝統文化を愛する女性。%瞳に宿した憂いは何を物語るのか。%%" ; UIHELPMARK EN: "%[Mark Kimberly]%%@^CCamera@ ^LZoom Out%@^NLight@ ^RZoom In%%@^ASubtitles ON@ ^BCancel%%This warm hearted and good natured%man works at the harbor as a%forklift operator.%" | JA: "%【マーク・キンバリー】%%@^Cカメラ@ ^L縮小%@^Nライト@ ^R拡大%%@^A字幕ＯＮ@ ^Bキャンセル%%フォークリフトの運転手。%港で働く、気のいい男。%%" ; UIHELPCCNV EN: "%[Conversation]%%If Ryo is able to converse with%someone, the^A icon will appear in%the lower right corner..%% ^A Speak%" | JA: "%【会話】%%会話できそうな場所で、^Aボタンを押して%相手に話しかけてみましょう。%%%^A会話%" ; UIHELPSNE2 EN: "[Sneak into the Old Warehouse Area]%%%Find Warehouse No.8%without being caught by the security guards.%You can hide by pressing^A%near objects and corners.%You can check the number of the warehouse%by pressing^A%to write it on your map.%You can see the map by pressing^X.%" | JA: "【旧倉庫街潜入】%%%警備員に見つからずに、第８倉庫を探して下さい。%%物陰や、曲がり角で^Aを押せば、%隠れる事ができます。%%倉庫の番号は、^Aで確認することができます。%^Xで地図を見ることができます。%" ; UIHELPHAZU EN: "%[Hazuki House]%%Position Ryo in front of the bed and%press^A to save the game or display%the game settings.%(Sleep/Training only appear after 20:00)%%Ryo returns here at night%to sleep.%" | JA: "%【芭月家・涼の部屋】%%ベッドの前で^Aを押すことで、%記録、設定を呼び出すことができます。%※「寝る」、「修行」は２０：００以降にならないと%表示されません。%%涼は夜になると、眠るためにここへ帰ってきます。%%" ; UIHELPLOCK EN: "%%When near certain objects, holding%^Z will cause the viewpoint%to lock onto that object. If ^A can be%pressed, an action such as picking up,%opening, etc., will be executed. Use%these commands to investigate many%things anywhere.%" | JA: "%%物に対し、^Zでズームすると、%視点が合う物があります。%その時^Aを押すと、つかむ、開ける、などができます。%いろいろな物を調べてみてください。%%%" ; UIHELPDART EN: "[Darts Seven]%%100¥ per game. Press^A to throw the dart.%Exceed 120 points to go to the 2nd stage.%Exceed 240 points in the 2nd stage%to go on to the 3rd stage.%% Bull's-eye red center        : 50 points% Bull's-eye green perimeter  : 25 points% Outer red/green line       : Double% Inner red/green line      : Triple%" | JA: "【ダーツセブン】%%１ゲーム１００円です。^Aで矢を投げます。%１２０点を越えると２ｎｄステージに、%２ｎｄステージで２４０点を越えると、%３ｒｄステージに進めます。%%中央の赤丸：５０点%中央の周囲：２５点%外側赤緑ライン：２倍%内側赤緑ライン：３倍%" ; UIHELPCHAI EN: "%[Chai]%%@^CCamera@ ^LZoom Out%@^NLight@ ^RZoom In%%@^ASubtitles ON@ ^BCancel%%Lurking in dark shadows, Chai spies%on all with beastly eyes.%No one is aware of his background.%" | JA: "%【チャイ】%%@^Cカメラ@ ^L縮小%@^Nライト@ ^R拡大%%@^A字幕ＯＮ@ ^Bキャンセル%%闇に潜む、獣のような目。%その生い立ちを知る者はない。%%" ; UIHELPKEIZ EN: "%[Keizo Oishi]%%@^CCamera@ ^LZoom Out%@^NLight@ ^RZoom In%%@^ASubtitles ON@ ^BCancel%%An antique shop owner with an extensive%knowledge backed by experience, his words%reveal a wealth of information.%" | JA: "%【大石敬三】%%@^Cカメラ@ ^L縮小%@^Nライト@ ^R拡大%%@^A字幕ＯＮ@ ^Bキャンセル%%深い知識を持つ、骨董屋の主人。%経験に裏付けられた言葉は、%何を語るのか。%" ; UIHELPNOZO EN: "%[Nozomi Harasaki]%%@^CCamera@ ^LZoom Out%@^NLight@ ^RZoom In%%@^ASubtitles ON@ ^BCancel%%A cheerful and liberated character,%Nozomi maintains an iron strong will.%At who do her shining eyes gaze?%" | JA: "%【原崎望】%%@^Cカメラ@ ^L縮小%@^Nライト@ ^R拡大%%@^A字幕ＯＮ@ ^Bキャンセル%%あかるく、自由な性格の中に、%強い意思を持つ。%澄んだ瞳は、何を見つめるのか。%"
    "UIHELPMART", "UIHELPSPAC", "UIHELPHANG", "UIHELPFORT", "UIHELPSPAR", "UIHELPSHO1", "UIHELPSHO2", "UIHELPPRIZ", "UIHELPVEN2", "UIHELPALLM", "UIHELPCOLL", // @text UIHELPMART EN: "[Martial Arts Moves Scroll]%%The bar graph indicates Ryo's skill level.%Red indicates a mastered move.%Green indicates a move not yet learnt.%Input commands when Ryo is facing right.%%@^CSelect@ ^NRotate%@^ASet move to^R@ %@^XMove explanation%@ @ ^BReturn%" | JA: "【全技巻物】%%棒グラフは、技の「熟練度」を表しています。%技名が明るく表示されているものは、修得した技です。%技名が暗いものは、まだ習得していない技です。%コマンドは、涼が右向きのときのものです。%%@^C選択@ ^N回転%@^A技の登録%@ %@@ ^B戻る%" ; UIHELPSPAC EN: "%[Space Harrier]%%Evade enemy fire and%attack by shooting them down.%%^C Move %^A ^B ^X ^Y Shoot%%Press^Yon the title screen to quit.%Remaining credits will be returned in cash.%" | JA: "%【スペースハリアー】%%敵弾を避けて、%ショットで敵を倒してください。%%^C移動%^A ^B ^X ^Yショット%%タイトル画面で^Yを押せば、%やめる事ができます。%残ったクレジットは返金されます。%" ; UIHELPHANG EN: "%[Hang-On]%%Drive the motorcycle and pass the%checkpoints within the allotted time.%%@^C Steering @^R Accelerator %@@^L Brake%%Press^Yon the title screen to quit.%Remaining credits will be returned in cash.%" | JA: "%【ハングオン】%%バイクを操作して、制限時間内に%チェックポイントを通過してください。%%@^Cハンドル@^Rアクセル%@@^Lブレーキ%%タイトル画面で^Bを押せば、%やめる事ができます。%残ったクレジットは返金されます。%" ; UIHELPFORT EN: "%[Fortune-telling]%%Have Ryo's fortune told via a crystal ball.%Each fortune costs 300¥.%% Future : Foretells Ryo's near future.% Clue   : Reveals a game hint.% Gamble : Reveals Ryo's lucky numbers.%" | JA: "%【占い】%%水晶玉を使った占いです。%１回３００円です。%%幸運の鍵：ゲーム進行のカギを占います。%運命の糸：涼の未来を占います。%勝利の扉：ラッキーナンバーを占います。%" ; UIHELPSPAR EN: "%[Sparring]%%Have Ryo experiment newly mastered moves.%%@^L Run@ ^K Leg Move%@^C Walk@ ^O Throw Move%@^C Command@ ^P Hand Move% @@^G Block/Evade%@^R Set Move%" | JA: "%【組手】%%修得した技を、試すことができます。%%@^L走る@ ^K足技%@^C歩く@ ^O投技%@^Cコマンド@ ^P手技% @@^G防御/避ける%@^R登録技%" ; UIHELPSHO1 EN: "%%Here Ryo can shop for various items.%Zoom in to lock-on to an item and press^A.%Select “Buy” or “Don't Buy” from the AS.%%Buy Shenmue goods or bring in a winning%can from a vending machine to earn a chance%at the store raffle drawing.%" | JA: "%%ここでは買い物ができます。%商品をロックオンして、^Aを押した後、%「買う／買わない」のどちらかを選んでください。%%自動販売機で手に入れた当たり缶を、%レジで使うか、シェンムー商品を購入すると、%くじが引けます。%" ; UIHELPSHO2 EN: "%%%Here Ryo can shop for various items.%Zoom in to lock-on to an item, and then%press^A to display the AS selection.%Select whether to “Buy” or “Don't%Buy” the item.%" | JA: "%%%ここでは買い物ができます。%商品をロックオンして、^Aを押した後、%「買う／買わない」のどちらかを選んでください。%%" ; UIHELPPRIZ EN: "%%%[Prize Selection]%%Select the prize that you want.%%@^C Select @^A Enter%" | JA: "%%%【景品選択】%%欲しい景品を選んでください。%%@^C選択@^A決定%" ; UIHELPVEN2 EN: "%%%[Vending Machine]%%Select the drink you recommend.%Each drink costs 100¥.%%@^C Select @^A Buy%" | JA: "%%%【自動販売機】%%おすすめの飲み物を選んであげてください。%一缶１００円です。%%@^C飲み物の選択@^A買う%" ; UIHELPALLM EN: "[All Moves Scroll]%%Commands are valid when Ryo's facing right%and his opponent is to the right Reverse%commands when necessary. The green box%indicates the move pre-set to ^R%%@^CSelect@ ^RZoom%@^AMove explanation@ ^NRotate%%@^LToggle display@ ^BReturn%" | JA: "【全技巻物】%%ここに記されているのは、涼が右向きで、%相手が右にいる場合のコマンドです。%涼の向きが逆の場合は、コマンドも逆になります。%コマンド欄にショートカット表示のある技が、%Ｌ２ボタンに登録中の技です。%%@^C選択@ ^N涼の回転%%@ ^B戻る%" ; UIHELPCOLL EN: "[Collection]%%Check or examine the collection items%that Ryo has amassed throughout the game.%When the number of items exceeds 85,%use ^L/^Rto toggle windows.%%@^C Move the cursor@ ^A ^Y Look@%%@@^BReturn%" | JA: "【コレクション】%%手に入れたコレクションアイテムを%見ることができます。%%@^Cカーソル移動@ ^A ^Y見る@%%@@^B戻る%"
    "UIHELPCAS1", "UIHELPBUSS", "UIHELPCAS2", "UIHELPEMUL", "UIHELPON02",
    "UIHELPFORA"}; // @text UIHELPCAS1 EN: "%[Cassette]%%Examine the cassette%tapes Ryo has obtained.%% @^C Move the cursor @^A ^Y Look %%@@^B Return%" | JA: "%【カセットテープ】%%手に入れたカセットテープを見ることができます。%% @^Cカーソル移動@^A ^Y見る%%@@^B戻る%" ; UIHELPBUSS EN: "%%[Bus Stop]%%When the bus arrives at the bus stop,%press^A to have Ryo get on the bus.%The bus fare is 160¥.%%@^A Get on the bus.%" | JA: "%%【バス停】%%バスが停車している時に、%^Aを押すと乗ることができます。%運賃は１６０円です。%%@^Aバスに乗る%" ; UIHELPCAS2 EN: "%%[Cassette]%%Have Ryo play a cassette tape.%%@^CMove the cursor@^A ^Y Playback%%@@^BReturn%" | JA: "%【カセットテープ】%%手に入れたカセットテープの%音楽を聴くことができます。%%@^Cカーソル移動@^A ^Y聴く%%@@^B戻る%" ; UIHELPEMUL EN: "%%%%%Press^B to quit game.%" | JA: "%%%%%ゲームを終了するには^Bを押してください。%" ; UIHELPON02 EN: "%%%%%Please connect a wireless controller to continue.%" | JA: "%%%%%続けるにはワイヤレスコントローラーを接続してください：%" ; UIHELPFORA EN: "%%[Forklift Race]%%@^RMove forward@ ^BChange camera view%@^LMove back%@^CSteering wheel%%@^XMap@ ^ARaise/lower forklift%%" | JA: "%%【フォークリフトレース】%%@^R前進@ ^B視点切り替え%@^L後退%@^Cハンドル%%@^Xマップ@ ^Aフォークの上下%%"




constexpr std::array<std::int32_t, 3> kCollectionChildren{kUiSubMenuGacha, kUiSubMenuCassettes, kUiSubMenuScrolls};

std::array<d3t::UIWindow *, 3> g_collectionWindows{};

std::array<d3t::UILabelWidget *, 4> g_collectionLabels{};

std::array<UIColourImageWidget *, 7> g_collectionColourImages{};

std::array<UIButtonWidget *, 3> g_collectionButtons{};

UIFocusList g_collectionTabList{.focused = 0,
    .lastFocused = 0,
    .selected = 0,
    .previousSelected = 0,
    .hasFocus = 0,
    .previousHasFocus = 0,
    .reserved12 = {},
    .first = 0,
    .last = 0,
    .reserved1c = 0,
    .windows = nullptr,
    .labels = nullptr,
    .initialized = 0,
    .reserved31 = {},
    .shownIndex = 0};

std::array<std::int32_t, 5> g_collectionPromptIconSet{};







constexpr std::array<std::int32_t, 4> kOptionsChildren{kUiSubMenuOptionsControls, kUiSubMenuOptionsAudio, kUiSubMenuOptionsGraphics, -1};

std::array<d3t::UIWindow *, 41> g_optionsWindows{};

std::array<d3t::UILayoutItem *, 47> g_optionsLayoutItems{};

std::array<d3t::UILabelWidget *, 13> g_optionsLabels{};

std::array<UIColourImageWidget *, 48> g_optionsColourImages{};

std::array<UIButtonWidget *, 19> g_optionsButtons{};

UIButtonIconWidget *g_optionsKeyBindingKey = nullptr;

UIGridTemplate g_optionsRowTemplate{};

UIFocusList g_optionsTabList{.focused = 0,
    .lastFocused = 0,
    .selected = 0,
    .previousSelected = 0,
    .hasFocus = 0,
    .previousHasFocus = 0,
    .reserved12 = {},
    .first = 0,
    .last = 0,
    .reserved1c = 0,
    .windows = nullptr,
    .labels = nullptr,
    .initialized = 0,
    .reserved31 = {},
    .shownIndex = 0};

std::uint32_t g_optionsTabButtonColour = 0;

std::uint32_t g_optionsTabButtonSelectedColour = 0;

std::uint32_t g_optionsTabBackgroundColour = 0;

std::uint32_t g_optionsTabBackgroundSelectedColour = 0;

std::int32_t g_optionsTabBackgrounds = 0;

std::uint8_t g_optionsClosePending = 0;

std::int32_t g_optionsShownLanguage = -1;

std::array<char, 256> g_optionsSliderText{};

std::array<std::int32_t, 5> g_optionsPromptIconSet{};

UIGridDrawCell g_optionsControlsRowDraw = nullptr;

UIGridDrawCell g_optionsAudioRowDraw = nullptr;

UIGridDrawCell g_optionsGraphicsRowDraw = nullptr;



d3t::UIWindow *g_optionsControlsPanel = nullptr;

UIGrid g_optionsControlsGrid{};

std::array<std::uint64_t, 42> g_optionsControlsRecordedRows{};

d3t::UIWindow *g_optionsAudioPanel = nullptr;

UIGrid g_optionsAudioGrid{};

std::array<std::uint64_t, 7> g_optionsAudioRecordedRows{};

d3t::UIWindow *g_optionsGraphicsPanel = nullptr;

UIGrid g_optionsGraphicsGrid{};

std::array<std::uint64_t, 11> g_optionsGraphicsRecordedRows{};

void DestroyOwnedTask(HLib::Task *&task, std::int32_t expectedToken) {
    HLib::Task *const owned = task;
    task = nullptr;
    if (owned && owned->taskToken == expectedToken)
        HLib_DestroyTask(owned);
}

void InstallOwnedTaskMain(HLib::TaskCallback callback) {
    if (!CurrentTask)
        return;
    CurrentTask->mainTaskCallbackFnPtr = callback;
    CurrentTask->initCallbackFnPtr = nullptr;
    CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
}

bool CapturePad(PadCaptureEntry *input, PadCaptureEntry *&previous) {
    if (!input || (input->flags & 2U) != 0)
        return false;
    previous = shenUpdateInputsInto(input);
    return true;
}

void RestorePad(PadCaptureEntry *&previous) {
    if (previous)
        (void)shenUpdateInputsInto(previous);
    previous = nullptr;
}

void ReleasePadCapture(PadCaptureEntry *&input, PadCaptureEntry *&previous) {
    if (input)
        ReleasePADCEntry(input, previous);
    input = nullptr;
    previous = nullptr;
}

bool IsOwnedUiTask(const HLib::Task *task, std::int16_t queueIndex, std::int32_t token, HLib::TaskCallback initialCallback, HLib::TaskCallback mainCallback) {
    return task && task->taskIndex == queueIndex && !task->task_data_ptr && task->taskToken == token &&
        (task->mainTaskCallbackFnPtr == initialCallback || (mainCallback && task->mainTaskCallbackFnPtr == mainCallback));
}

std::uint32_t LoadPresentationDefinition(std::int32_t index, const char *path);

void ActivateOwnedElement(std::int32_t index, const char *path) {
    LoadPresentationDefinition(index, path);
    d3t::ActivateElementByIdx(index);
}

bool ResetOwnedElement(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiElements.size())) {
        return false;
    }
    const bool overlay = index == 56 || index == 57 || index == 65;
    if (g_currUIState != -1 && index != g_currUIState && !overlay) {
        return false;
    }

    UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(index)];
    if (element.isVisible && element.destroy && element.destroy() != 1 && g_busy_reset != 1) {
        return false;
    }
    element.isVisible = 0;
    element.isActive = 0;
    return true;
}

void HideOwnedElement(std::int32_t index) {
    UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(index)];
    element.isVisible = 0;
    element.isActive = 0;
}

Character *MapRyo() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return static_cast<Character *>(reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->ryo);
}

static_assert(sizeof(SubtitleLoadRequest) == 8);

static_assert(sizeof(GlyphLoadRequest) == 16);

static_assert(sizeof(SubtitleTableHeader) == 16);

static_assert(sizeof(SpecialSubtitleEntry) == 12);

static_assert(sizeof(SubtitleFontGlyph) == 0x30);

std::array<SubtitleLoadRequest, 8192> g_subtitleLoadRequests{};

std::array<GlyphLoadRequest, 8192> g_glyphLoadRequests{};

std::atomic<std::uint16_t> g_subtitleLoadRequestIndex{0};

std::atomic<std::uint16_t> g_glyphLoadRequestIndex{0};

std::array<std::int32_t, 7> g_glyphLoadStates{};

std::array<std::array<SubtitleFontLayer, 2>, 7> g_subtitleFonts{};


std::array<std::array<std::array<float, 6>, 4>, 7> g_uiFontPixelSizes{};

std::array<std::array<SubtitleFontAtlasRequest, 2>, 7> g_subtitleFontRequests{};

std::array<SubtitleFontLayer, 7> g_memoFonts{};

std::array<SubtitleFontAtlasRequest, 7> g_memoFontRequests{};

SubtitleTableHeader *g_stringTable = nullptr;

const std::byte *g_stringTableData = nullptr;

const std::byte *g_stringTableEnd = nullptr;

std::vector<SpecialSubtitleEntry> g_specialSubtitleEntries;

std::array<std::array<char, 512>, 3> g_subtitleLines{};

CVWKRuntimeSlot *g_currentSubtitleSlot = nullptr;


std::int32_t g_cvwkReadSubtitleOnPlay = 0;



SubtitlePresentationColours g_subtitlePresentationColours = kDefaultSubtitleColours;

std::array<SubtitlePresentationColours, 4> g_subtitleClassColours{};

static_assert(sizeof(SplashScreenRuntime) == 0x40);

constexpr std::array<SplashImageDefinition, 6> kSplashImages{{
    {"/ui/Splash/Textures/jp_cesa.png?usage=0", nullptr, .2f, 5.0f, .4f, 3.0f},
    {"/ui/Splash/Textures/jp_photo.png?usage=0", nullptr, .2f, 3.0f, .4f, 0.0f},
    {"/ui/Splash/Textures/segalogo_01.png?usage=0", "/misc/SegaLogo.wav", .1f, 5.0f, .4f, 5.0f},
    {"/ui/Splash/Textures/d3tlogo.png?usage=0", nullptr, .2f, 5.0f, .4f, 2.5f},
    {"/ui/Splash/Textures/legal.png?usage=0", nullptr, .2f, 3.0f, .4f, 0.0f},
    {"/ui/Splash/Textures/legal_02.png?usage=0", nullptr, .2f, 3.0f, .4f, 0.0f},
}};

std::array<SplashScreenRuntime, 6> g_splashScreens{};

std::int32_t g_splashState = 0;


std::int32_t g_forceSkipSplashScreens = 0;

std::uint64_t g_splashStateStart = 0;

bool g_splashPanel = false;

bool g_skipSplash = false;

float g_splashOpacity = 0.0f;

std::int32_t g_pressStartIdleFrames = 600;


bool g_pressStartIntroPending = true;

bool g_pressStartIntroActive = false;



constexpr std::array<const char *, 5> kPressStartWindows{
    "Press_Start::Background_Panel", "Press_Start::Emblem_Panel", "Press_Start::Logo_Panel", "PressStart::SegaCopyright", "PressStart::PressStartBox"};

std::array<std::int32_t, 5> g_pressStartFadeSlots{-1, -1, -1, -1, -1};

float g_pressStartIntroTime = 0.0f;

std::int32_t g_pressStartBlinkSlot = -1;



constexpr std::array<const char *, 3> kPressStartLogoImages{"GameLogo", "GameLogo_KR", "GameLogo_SC"};

std::array<std::int32_t, 3> g_pressStartLogoImages{};

std::array<d3t::UIWindow *, 5> g_pressStartWindows{};

UIColourImageWidget *g_pressStartEmblemImage = nullptr;

constexpr std::array<const char *, 2> kPressStartIconIds{"PressStart::Icon_Console", "PressStart::Icon_PC"};

std::array<UIButtonIconWidget *, 2> g_pressStartIcons{};

d3t::UILabelWidget *g_pressStartLabel = nullptr;

d3t::UIImageWidget *g_pressStartLogoImage = nullptr;

std::array<UILegacyFile, 80> g_uiLegacyFiles{};

// The Main Menu's ButtonWheel (MainMenuButtonWheel): its record index and the record, whose selection is the item id (0 New Game, 1
// Continue, 2 70 Man Battle, 3 Options), and the record as the wheel's IsSelected found it (its Angles are the progress preset's).
std::int32_t g_mainMenuWheelIndex = -1;

UIButtonWheel *g_activeMainMenuWheelLayout = nullptr;

UIButtonWheel g_mainMenuWheelSnapshot{};


std::int32_t g_mainMenuWheelRefresh = 0;

// The selection whose description the Information Bar last showed.
std::int32_t g_mainMenuWheelPreviousSelection = 0;

constexpr std::array<const char *, 28> kMainMenuWheelImages{"LT_NewGame_En", "LT_Continue_En", "LT_70ManBattle_En", "LT_Options_En", "LT_NewGame_Jp",
    "LT_Continue_Jp", "LT_70ManBattle_Jp", "LT_Options_Jp", "LT_NewGame_Fr", "LT_Continue_Fr", "LT_70ManBattle_Fr", "LT_Options_Fr", "LT_NewGame_De",
    "LT_Continue_De", "LT_70ManBattle_De", "LT_Options_De", "LT_NewGame_Kr", "LT_Continue_Kr", "LT_70ManBattle_Kr", "LT_Options_Kr", "LT_NewGame_Ct",
    "LT_Continue_Ct", "LT_70ManBattle_Ct", "LT_Options_Ct", "LT_NewGame_Cs", "LT_Continue_Cs", "LT_70ManBattle_Cs", "LT_Options_Cs"};

std::array<std::int32_t, 28> g_mainMenuWheelImages{};



std::int32_t g_mainMenuStatIndex = 0;

constexpr std::array<std::int32_t, 4> kMainMenuIds{kMainMenuWheelElement, kUiLoadSave, kUiSubMenuOptions, kUiLoadSave};

std::uint8_t g_mainMenuNewGamePicked = 0;

std::uint8_t g_mainMenuWheelArmed = 0;


std::int32_t g_mainMenuWheelAnimSlot = -1;


std::uint64_t g_uiAcceptSoundFrame = 0;

bool UpdateStatByIdx(std::int32_t current, std::int32_t next);

std::vector<char> g_frontendAutomationOriginalSettingsFile;

bool g_frontendAutomationSettingsFileCaptured = false;

bool g_frontendAutomationSettingsFileExisted = false;

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1::d3t::ui_runtime_detail {

bool MessageBox(const char *token, bool useString, std::int32_t clearAfterDone, UIRetry70ManBattlePromptCallback callback, float timeBeforeInactive);

} // namespace shenmue::s1::d3t::ui_runtime_detail

namespace shenmue::s1::ui_runtime_detail {

constexpr std::uint16_t kFrontendAutomationInputInterval = 4;

constexpr std::uint16_t kFrontendOptionsAutomationTimeout = 600;

FrontendAutomationStage g_frontendAutomationStage = FrontendAutomationStage::PressStart;

std::uint16_t g_frontendAutomationFrames = 0;

// The --options automation's step (an edit of kFrontendOptionsEdits, then its return), the edited option's grid row, its value before the
// edit and the value the step started from.
std::int32_t g_frontendOptionsStep = 0;

std::int32_t g_frontendOptionsRow = -1;

float g_frontendOptionsOriginal = 0.0f;

float g_frontendOptionsStart = 0.0f;

bool QueueFrontendAutomationInput(std::uint16_t input) {
    if (g_frontendAutomationStage == FrontendAutomationStage::Complete || g_frontendAutomationStage == FrontendAutomationStage::Failed) {
        return false;
    }
    if (g_frontendAutomationFrames != 0xffffU) {
        ++g_frontendAutomationFrames;
    }
    if (g_frontendLaunchTarget == FrontendLaunchTarget::Options && g_frontendAutomationFrames >= kFrontendOptionsAutomationTimeout) {
        if (g_frontendAutomationSettingsFileCaptured && !RestoreFrontendOptionsSettingsFile()) {
        }
        SetFrontendAutomationStage(FrontendAutomationStage::Failed);
        return false;
    }
    if ((g_frontendAutomationFrames % kFrontendAutomationInputInterval) != 0) {
        return false;
    }
    g_pendingInputFlags |= input;
    return true;
}





constexpr std::array<const char *, 4> kMainMenuPageWindowIds{
    "MainMenu_WheelContainer", "LoadScreen::Load_Screen_Panel", "Options_MainPanel", "Snapshots_BackgroundPanel"};

constexpr std::array<const char *, 28> kMainMenuTitleImages{"LT_MainMenu_En", "LT_LoadContinue_En", "LT_Options_En", "LT_70MB_En", "LT_MainMenu_Jp",
    "LT_LoadContinue_Jp", "LT_Options_Jp", "LT_70MB_Jp", "LT_MainMenu_Fr", "LT_LoadContinue_Fr", "LT_Options_Fr", "LT_70MB_Fr", "LT_MainMenu_De",
    "LT_LoadContinue_De", "LT_Options_De", "LT_70MB_De", "LT_MainMenu_Kr", "LT_LoadContinue_Kr", "LT_Options_Kr", "LT_70MB_Kr", "LT_MainMenu_Ct",
    "LT_LoadContinue_Ct", "LT_Options_Ct", "LT_70MB_Ct", "LT_MainMenu_Cs", "LT_LoadContinue_Cs", "LT_Options_Cs", "LT_70MB_Cs"};

constexpr std::array<const char *, 3> kMainMenuFrontImageIds{"MainMenu::ContentBackground", "Texture_Alpha_Front", "Texture_Alpha_Front2"};

d3t::UIWindow *g_mainMenuContentPanel = nullptr;

std::array<d3t::UIWindow *, 4> g_mainMenuPageWindows{};

d3t::UIImageWidget *g_mainMenuTitleImage = nullptr;

d3t::UIWindow *g_switchAccountButtonIcon = nullptr;

d3t::UIWindow *g_switchAccountButtonLabel = nullptr;

std::array<std::int32_t, 28> g_mainMenuTitleImageIndices{};

std::array<UIColourImageWidget *, 3> g_mainMenuFrontImages{};

std::array<std::uint8_t, 3> g_mainMenuFrontImageAlphas{};

d3t::UILabelWidget *g_mainMenuUserIdLabel = nullptr;

std::array<char, 1> g_mainMenuUserIdText{};

void MainMenu_UpdateTitle();

UIColourImageWidget *UI_ColourImageRecord(std::int32_t index);

d3t::UILabelWidget *UI_LabelRecord(std::int32_t index);

void FrontendAutomation_OptionsTick();

extern std::array<std::int32_t, 5> g_hudPromptRequestedSlots;

std::int32_t UI_FindViewIconWidget(std::int32_t kind, std::uint32_t slotIndex, const char *id);

UIButtonIconWidget *UI_ButtonIconRecord(std::int32_t index);

UIColourImageWidget *UI_ColourImageRecord(std::int32_t index);

d3t::UILabelWidget *UI_LabelRecord(std::int32_t index);






constexpr std::array<const char *, 5> kSubtitleWindowIds{"SubFullPanel", "SubHudPanel", "Sub_TopLine_Panel", "Sub_MidLine_Panel", "Sub_BotLine_Panel"};

constexpr std::array<const char *, 2> kSubtitleImageIds{"Sub_Background_Full_Image", "Sub_Background_Hud_Image"};

std::array<d3t::UIWindow *, 5> g_subtitleWindows{};

std::int32_t g_subtitleTextAnimSlot = -1;

std::int32_t g_subtitleFullPanelBinding = -1;

std::int32_t g_subtitleHudPanelBinding = -1;

std::array<d3t::UIImageWidget *, 2> g_subtitleBackgroundImages{};

d3t::UILabelWidget *g_subtitlePhoneLabel = nullptr;

std::array<std::int32_t, 3> g_subtitleForegroundLabels{-1, -1, -1};

std::array<std::int32_t, 3> g_subtitleOutlineLabels{-1, -1, -1};

std::array<std::int32_t, 3> g_subtitleBackgroundLabels{-1, -1, -1};

std::array<char, 512> g_subtitleText{};

float g_subtitleVariableScale = 0.0f;

bool g_subtitlesShown = false;

[[maybe_unused]] std::int32_t GetIsSame(std::int32_t lhs, std::int32_t rhs) {
    return lhs == 0 ? 0 : 1 + static_cast<std::int32_t>(lhs == rhs);
}

[[maybe_unused]] std::int32_t SelectListEndpointOrMiddleSpriteSlot(std::int32_t index, std::int32_t lastIndex) {
    return index == 0 || index == lastIndex ? 3 : 1;
}

void GameMessage_SetContent(UIGameMessageContentType type, std::int32_t value, const char *text);


std::int64_t UI_NullsubReturn1() {
    return 1;
}

std::array<std::int32_t, 12> g_uiSoundFrames{};

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1::d3t::ui_runtime_detail {

void ShowDescriptionForItem(float *rect, void *data);

} // namespace shenmue::s1::d3t::ui_runtime_detail

namespace shenmue::s1::ui_runtime_detail {

void cbHandleAllItemsWithInteraction(float *rect, void *data);

static_assert(sizeof(nk_style_item) == 0x28);

static_assert(offsetof(nk_context, input.mouse.buttons) == 0xfc && offsetof(nk_context, input.mouse.pos) == 0x12c);

static_assert(offsetof(nk_context, input.mouse.prev) == 0x134 && offsetof(nk_context, input.mouse.scroll_delta) == 0x144);

static_assert(offsetof(nk_context, style.text.color) == 0x1a4 && offsetof(nk_context, style.button.border_color) == 0x228);

static_assert(offsetof(nk_context, style.button.rounding) == 0x244 && offsetof(nk_context, style.window.fixed_background) == 0x2228);



constexpr std::array<std::int32_t, 7> kMovesCommandActions{24, 22, 55, 54, 31, 23, 25};

std::array<d3t::UIWindow *, 7> g_informationBarWindows{};

std::array<d3t::UILabelWidget *, 2> g_informationBarSideLabels{};

std::array<UIIconLabelWidget *, 3> g_informationBarTextLabels{};

std::array<d3t::UIImageWidget *, 3> g_informationBarIcons{};

std::array<std::int32_t, 3> g_informationBarBackgroundImages{};

// The text colour d3t_CopyStringToUILabel gives a message without its own colour.
std::uint32_t g_informationBarTextColour = 0xffffffffU;

// The YesNo prompt's widgets: the left (Yes) and right (No) selector containers and the question container, the left and right text images,
// the selected and unselected sprites of each side, and the view images (the selector off and on, then Yes and No in each language).
constexpr std::array<const char *, 3> kYesNoPromptWindowIds{
    "YesNoSelector::SelectorContainer_Left", "YesNoSelector::SelectorContainer_Right", "YesNoSelector::QuestionContainer"};

constexpr std::array<const char *, 2> kYesNoPromptTextImageIds{"YesNoSelector::LeftText", "YesNoSelector::RightText"};

constexpr std::array<const char *, 4> kYesNoPromptSpriteIds{
    "YesNoSelector::SelectedLeft", "YesNoSelector::UnselectedLeft", "YesNoSelector::SelectedRight", "YesNoSelector::UnselectedRight"};

constexpr std::array<const char *, 16> kYesNoPromptImageIds{"YesNo_Off", "YesNo_On", "Yes_EN", "Yes_JP", "Yes_FR", "Yes_DE", "Yes_KR", "Yes_CT", "Yes_CS",
    "No_EN", "No_JP", "No_FR", "No_DE", "No_KR", "No_CT", "No_CS"};

std::array<d3t::UIWindow *, 3> g_yesNoPromptWindows{};

std::array<d3t::UIImageWidget *, 2> g_yesNoPromptTextImages{};

std::array<UIButtonIconWidget *, 4> g_yesNoPromptSprites{};

std::array<std::int32_t, 16> g_yesNoPromptImages{};


std::array<char, 512> g_messageBoxTimedText{};


std::int32_t g_actionSelectorInputCode = 13;

std::uint16_t g_actionSelectorPreviousPressed = 0;

std::array<d3t::UIWindow *, 17> g_gameHudWindows{};


d3t::UILabelWidget *g_gameHudCameraLabel = nullptr;

UIColourImageWidget *g_gameHudCameraBase = nullptr;

std::array<UIButtonIconWidget *, 6> g_gameHudPromptSprites{};

std::array<UIButtonIconWidget *, 5> g_gameHudButtonIcons{};


std::int32_t g_hudPromptKeyboardLayoutApplied = 0;




std::array<std::int32_t, 5> g_hudPromptRequestedSlots{-1, -1, -1, -1, -1};


std::array<std::int32_t, 4> g_hudPromptEffectiveSlots{-1, -1, -1, -1};

std::array<std::int32_t, 5> g_hudPromptOverrideSlots{-1, -1, -1, -1, -1};

bool g_hudPromptOverrideActive = false;


std::int32_t g_hudPromptHighlightAnimSlot = -1;

std::array<std::int32_t, 5> g_hudPromptIconAnimSlots{-1, -1, -1, -1, -1};

UIButtonIconWidget *UI_ButtonIconRecord(std::int32_t index);

UIColourImageWidget *UI_ColourImageRecord(std::int32_t index);

d3t::UILabelWidget *UI_LabelRecord(std::int32_t index);

d3t::UILabelWidget *UI_LabelRecord(std::int32_t index);



std::array<std::int32_t, 6> g_70mbPanelWindows{};

std::array<std::int32_t, 6> g_70mbPanelBindings{};

std::array<std::int32_t, 8> g_70mbLabels{};


std::array<d3t::UILabelWidget *, 3> g_moveLearnedLabels{};

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

static_assert(sizeof(CreditsImage) == 40);

std::array<CreditsTable, 3> g_creditsTables{};

float g_creditsScrollSpeed = 0.0f;

float g_creditsScrollProgress = 0.0f;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {





CreditsImage *g_creditsSections = nullptr;

std::uint32_t g_creditsSectionCount = 0;

std::uint32_t g_creditsSectionCapacity = 0;

CreditsImage g_creditsSignature{};

d3t::UIWindow *g_creditsScrollSpace = nullptr;

float g_creditsFadeAlpha = 0.0f;

std::uint8_t g_creditsFadeOutRequested = 0;

std::uint8_t g_creditsScrollInitialized = 0;

nk_panel g_creditsPanel{};

constexpr std::array<const char *, 3> kCreditsTypes{"OriginalSM1", "d3t", "Sega"};

std::array<UIButtonIconWidget *, 4> g_qteMinigameKeyIcons{};

std::array<UIButtonIconWidget *, 4> g_qteMinigamePadIcons{};




std::array<UIButtonIconWidget *, 3> g_gameQteIcons{};

std::array<d3t::UIWindow *, 19> g_gameQteWindows{};

std::array<float, 3> g_gameQtePanelOffsets{};

std::int32_t g_gameQteAnimSlot = -1;

std::int32_t g_gameQteHidden = 1;

std::uint16_t g_gameQteMask = 0xffff;


std::int32_t UI_FindSpriteSheetByImageName(const char *name) {
    for (std::size_t sheet = 0; sheet < g_uiSpriteSheets.size(); ++sheet) {
        const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
        if (candidate.imageName && candidate.used == 1 && !std::strncmp(name, candidate.imageName, 0x40))
            return static_cast<std::int32_t>(sheet);
    }
    return -1;
}


constexpr std::array<const char *, 9> kLoadScreenWindowIds{"LoadScreen::BackgroundPanel", "LoadScreen::BlackBackground", "LoadScreen::Panel_Header",
    "LoadScreen::LocationLabelParent", "LoadScreen::TimeLabel", "LoadScreen::DateLabel", "LoadScreen::LocationNameLabel", "LoadScreen::LocationSubNameLabel",
    "LoadScreen::Panel_LoadingText"};

bool DrawPresentationOwners(std::uint32_t) {
    if (g_helpOverlayState == UIHelpWidgetState::Idle && ((g_uiRefreshedElements[kUiHelpNew >> 5] >> (kUiHelpNew & 31)) & 1U) == 0)
        HideOwnedElement(kUiHelpNew);
    return false;
}

bool DrawDebugTextQueue(std::uint32_t renderTarget) {
#ifdef _DEBUG
    float line = 1.0f;
    const bool commitQueued = QueueDebugText(1.0f, line, SHENMUE_COMMIT_HASH);
#endif
    const std::byte *const data = GetDebugTextQueueData();
    const std::size_t size = GetDebugTextQueueSize();
    const std::uint32_t recordCount = GetDebugTextQueueCount();
    if (!data || size < sizeof(DebugTextRecordHeader) || recordCount == 0 || g_uiLoadedLanguage < 0 ||
        g_uiLoadedLanguage >= static_cast<std::int32_t>(g_subtitleFonts.size())) {
        return false;
    }

    const SubtitleFontLayer &font = g_subtitleFonts[static_cast<std::size_t>(g_uiLoadedLanguage)][0];
    const std::uint32_t texture = font.texture.load(std::memory_order_acquire);
    if (!texture)
        return false;

    std::array<NuklearQuad, 512> quads{};
    std::size_t quadCount = 0;
    bool drew = false;
    const auto flush = [&]() {
        if (quadCount == 0)
            return;
        drew |= DrawNuklearQuads(g_RenderDeviceContext, renderTarget, texture, std::span<const NuklearQuad>{quads.data(), quadCount});
        quadCount = 0;
    };

    std::size_t cursor = 0;
    for (std::uint32_t record = 0; record < recordCount && cursor <= size - sizeof(DebugTextRecordHeader); ++record) {
        DebugTextRecordHeader header{};
        std::memcpy(&header, data + cursor, sizeof(header));
        const std::size_t textOffset = cursor + sizeof(header);
        if (header.textLength > size - textOffset)
            break;

        float x = header.x * header.advanceX;
        const float startX = x;
        float y = header.y * header.lineHeight;
        const char *const text = reinterpret_cast<const char *>(data + textOffset);
#ifdef _DEBUG
        if (commitQueued && record + 1 == recordCount)
            drew |= DrawLegacyDebugText({text, header.textLength}, x, y, header.characterWidth, header.characterHeight, header.advanceX, header.lineHeight, header.colour);
        else
#endif
        for (std::uint32_t index = 0; index < header.textLength; ++index) {
            const std::uint8_t character = static_cast<std::uint8_t>(text[index]);
            if (character == '\n') {
                x = startX;
                y += header.lineHeight;
                continue;
            }
            if (const SubtitleFontGlyph *const glyph = FindSubtitleGlyph(font, character);
                glyph && character != ' ' && glyph->width > 0.0f && glyph->height > 0.0f) {
                quads[quadCount++] = {x, y, header.characterWidth, header.characterHeight, glyph->u0, glyph->v0, glyph->u1, glyph->v1, header.colour};
                if (quadCount == quads.size())
                    flush();
            }
            x += header.advanceX;
        }
        flush();

        constexpr std::size_t kRecordPadding = 3;
        const std::size_t recordSize = sizeof(DebugTextRecordHeader) + header.textLength + kRecordPadding;
        if (recordSize > size - cursor)
            break;
        cursor += recordSize;
    }
    return drew;
}



bool g_screenFadeWindowBegun = false;

std::uint32_t NotebookPages_ParseDefinition();

std::int64_t ScreenFade() {
    g_screenFadeWindowBegun = true;
    return 0;
}


std::int32_t g_uiHelpNewOnly = 0;

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

std::array<std::uint32_t, 3> g_uiRefreshedElements{};

std::array<std::uint32_t, 3> g_uiPreviousRefreshedElements{};

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {




void UI_UpdateElementEnableStatus() {
    for (UIMouseCallbackQueue &queue : g_uiMouseCallbackQueues)
        queue.count = 0;
    for (UIBaseRuntime &element : g_uiElements) {
        const bool loading = element.setup && element.config.init != 3;
        std::int32_t enabled = 0;
        if ((!g_uiHelpNewOnly || element.index == kUiHelpNew) && !loading && !element.isHandledExternally && !g_uiDebugSuspendElements) {
            if (element.isActive)
                enabled = 1;
            else if (g_uiDebugElementDispatch == 1 || element.field_ac)
                enabled = element.isVisible || (element.unknown80 && element.unknown80());
        }
        element.config.isEnabled = enabled;
        std::int32_t status = 0;
        if (!loading && !(element.unknown80 && element.unknown80()) && !g_uiDebugSuspendElements && !element.isHandledExternally)
            status = element.isActive || ((g_uiDebugElementDispatch == 1 || element.field_ac) && element.isVisible);
        element.config.status = status;
    }
}


void RefreshAll(std::uint32_t first, std::uint32_t last) {
    for (std::uint32_t index = first; index < last; ++index) {
        UIBaseRuntime &element = g_uiElements[index];
        if (!element.config.isEnabled)
            continue;
        std::int64_t (*callback)() = nullptr;
        if (g_uiDebugElementDispatch == 1)
            callback = element.tick ? element.tick : element.postInit;
        else
            callback = element.postInit ? element.postInit : element.tick;
        if (!callback)
            continue;
        g_uiRefreshedElements[index >> 5] |= 1U << (index & 31);
        callback();
    }
}





void UI_DispatchElementInput() {
    if (g_uiDebugElementDispatch != 1)
        return;
    for (std::uint32_t index = 79; index; --index) {
        UIBaseRuntime &element = g_uiElements[index];
        const std::uint32_t bit = 1U << (index & 31);
        if (!element.config.status) {
            if (element.isVisible)
                g_uiRegisteredCallbackRequests[index >> 5] |= bit;
            continue;
        }
        const auto stateAllows = [index] {
            return g_currUIState == -1 || static_cast<std::int32_t>(index) == g_currUIState || (((index - 56) & ~9U) == 0 && index != 64);
        };
        if (stateAllows()) {
            for (UIMouseCallbackQueue &queue : g_uiMouseCallbackQueues) {
                for (std::int32_t entry = queue.count - 1; entry >= 0; --entry) {
                    UIMouseCallbackEntry &queued = queue.entries[static_cast<std::size_t>(entry)];
                    if (queued.element != element.index)
                        continue;
                    if (queued.callback)
                        queued.callback(queued.rect.data(), queued.data);
                    queue.count = 0;
                    break;
                }
            }
        }
        if (element.isPressed && stateAllows()) {
            g_uiRegisteredCallbackRequests[index >> 5] |= bit;
            if (!element.isPressed())
                return;
        }
    }
}


std::int32_t g_uiSafeAreaHeight = -1;

std::int32_t g_uiSafeAreaWidth = -1;






void UI_BeginFrame() {
    const std::int32_t mode = !g_isRenderSuppressed && (!g_currentSettings.display.aspectRatio || g_seqActive) ? 1 : 0;
    const UIViewport viewport = AdjustScreenForAspectRatio(mode);
    g_currentUIViewportRect = viewport.rect;
    g_currentUIViewportDepth = viewport.depthRange;
    for (std::size_t word = 0; word < g_uiRefreshedElements.size(); ++word) {
        g_uiPreviousRefreshedElements[word] = g_uiRefreshedElements[word];
        g_uiRefreshedElements[word] = 0;
    }
    const std::int32_t safeHeight = static_cast<std::int32_t>(UI_SafeAreaScale() * 1080.0f);
    const std::int32_t aspectWidth = UI_GetAspectWidth();
    const std::int32_t safeWidth = static_cast<std::int32_t>(static_cast<float>(aspectWidth) - (1080.0f - static_cast<float>(g_uiSafeAreaHeight)));
    if (safeHeight != g_uiSafeAreaHeight || safeWidth != g_uiSafeAreaWidth) {
        g_uiSafeAreaWidth = safeWidth;
        g_uiSafeAreaHeight = safeHeight;
        g_informationBarLabels.message.noOutline = 1;
        for (d3t::UIWindow &window : g_uiWindows)
            window.rectCached = 0;
    }
}



std::int32_t g_uiDebugRevealAspectBars = 0;

void DrawNuklear() {
    if (!g_NuklearInitialized) {
        SetupNuklear();
    }
    FontLoader();
    UI_BeginFrame();
    UpdateInGameStatus();
    g_uiPreviousInputElements = g_uiRegisteredCallbackRequests;
    g_uiRegisteredCallbackRequests.fill(0);
#if defined(_WIN32)
    if (!g_RenderDeviceContext) {
        g_RenderDeviceContext = d3t_DXGetPipelineContext(1);
    }
#endif


    for (std::size_t index = 0; index < g_uiElements.size(); ++index) {
        UIBaseRuntime &element = g_uiElements[index];
        if (g_uiDefinitionReady[index].exchange(false, std::memory_order_acq_rel)) {
            element.config.init = 2;
            const std::string text(reinterpret_cast<const char *>(g_uiDefinitions[index].data()), g_uiDefinitions[index].size());
            LoadUIFromMemoryIntoViewSlot(text.c_str(), element.config.uiHandleIndex);
            g_parsedUIDefinitions[index].viewSlot = element.config.uiHandleIndex;
            UI_BindViewImages(g_parsedUIDefinitions[index]);
            UI_ResolveLabelScaleVars(g_parsedUIDefinitions[index]);
        }
        if (element.config.init == 2 && static_cast<std::uint32_t>(element.config.uiHandleIndex) < 0x4cU &&
            TryFinalizeUIViewTextures(element.config.uiHandleIndex) == 1) {
            element.config.init = 3;
            if (element.isSelected)
                element.isSelected();
            if (element.getContentPanel)
                element.getContentPanel();
        }
    }
    g_uiElements[kUiNotebookPages].isActive = 1;

    nk_clear(&GNuklearWindow);
    UIAnim_UpdateDeltaTime(g_bEnableFrameTimeSmoothing ? fFrameTimeMs : 0.033333335f);
    UI_UpdateElementEnableStatus();
    UI_UpdateSearchIcons();
    const std::uint32_t uiRenderTarget = g_nuklearTexture;
    const bool canRender = BeginNuklearFrame(g_RenderDeviceContext, uiRenderTarget);
    bool drewUI = false;




    const auto frameViewportMode = [] { return !g_isRenderSuppressed && (!g_currentSettings.display.aspectRatio || g_seqActive) ? 1 : 0; };
    const UIViewport firstRange = AdjustScreenForAspectRatio(frameViewportMode());
    g_currentUIViewportRect = firstRange.rect;
    g_currentUIViewportDepth = firstRange.depthRange;

    if (g_uiElements[0x3c].config.isEnabled && g_frontendLaunchTarget != FrontendLaunchTarget::None &&
        g_frontendAutomationStage == FrontendAutomationStage::PressStart)
        QueueFrontendAutomationInput(0x400U);
    if (g_uiElements[4].config.isEnabled)
        FrontendAutomation_MainMenuTick();
    if (g_uiElements[kMainMenuWheelElement].config.isEnabled)
        FrontendAutomation_MainMenuWheelTick();
    RefreshAll(0, kUiHelpNew);
    Nuklear_BindRenderTarget();
    NuklearUpdateForAspectRatio(g_bIs43AspectRatio);
    if (g_RenderDeviceContext && g_NuklearInitialized) {
        const UIViewport column = AdjustScreenForAspectRatio(g_bIs43AspectRatio);
        if (g_bIs43AspectRatio)
            DrawAspectBars(1.0f, column, g_uiDebugRevealAspectBars);
    }
    const UIViewport secondRange = AdjustScreenForAspectRatio(0);
    g_currentUIViewportRect = secondRange.rect;
    g_currentUIViewportDepth = secondRange.depthRange;
    RefreshAll(kUiHelpNew, 80);
    NuklearUpdateForAspectRatio(g_bIs43AspectRatio);
    if (std::exchange(g_screenFadeWindowBegun, false) && canRender && g_rendererWhiteTexture) {
        const float scale = static_cast<float>(g_uiScreenHeight) / 1080.0f;
        const NuklearQuad fade{
            -10.0f * scale, -10.0f * scale, static_cast<float>(g_uiScreenWidth) + 20.0f * scale, 1100.0f * scale, 0.0f, 0.0f, 1.0f, 1.0f, g_currentFilmColour};
        drewUI |= DrawNuklearQuads(g_RenderDeviceContext, uiRenderTarget, g_rendererWhiteTexture, std::span{&fade, 1});
    }
    if (canRender && g_splashPanel && g_currentSplashScreenIndex >= 0 && g_currentSplashScreenIndex < static_cast<std::int32_t>(g_splashScreens.size())) {
        const SplashScreenRuntime &screen = g_splashScreens[static_cast<std::size_t>(g_currentSplashScreenIndex)];
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        if (screen.resolvedTextureHandle && GetTexture2DDimensions(screen.resolvedTextureHandle, &width, &height)) {
            const float x = (static_cast<float>(g_uiScreenWidth) - static_cast<float>(width)) * 0.5f;
            const float y = (static_cast<float>(g_uiScreenHeight) - static_cast<float>(height)) * 0.5f;
            drewUI |= DrawNuklearImage(g_RenderDeviceContext, uiRenderTarget, screen.resolvedTextureHandle, x, y, static_cast<float>(width),
                static_cast<float>(height), g_splashOpacity);
        }
    }
    if (canRender) {
        drewUI |= DrawPresentationOwners(uiRenderTarget);
        drewUI |= DrawLoadScreenPresentation(uiRenderTarget);
        drewUI |= DrawDebugTextQueue(uiRenderTarget);
    }

    if (canRender) {
        CompositeNuklearFrame(g_RenderDeviceContext, uiRenderTarget);
    }
    const UIViewport restored = AdjustScreenForAspectRatio(frameViewportMode());
    g_currentUIViewportRect = restored.rect;
    g_currentUIViewportDepth = restored.depthRange;
    UI_DispatchElementInput();
    if (g_movesScreen.destroyRyoPending) {
        DestroyForMovesScreen();
        g_movesScreen.destroyRyoPending = 0;
    }
    for (UIBaseRuntime &element : g_uiElements)
        element.isActive = 0;
}


std::array<std::array<char, 11>, 3232> g_notebookTextIds{};



std::int32_t g_memoPadDebugMode = 0;



std::int32_t g_debugNotebookOpenRequested = 0;

std::int32_t g_debugNotebookOpenPage = 0;

std::uint8_t g_debugNotebookRedraw = 0;

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

std::array<std::uint32_t, 4> g_notebookTextureLeft{};

std::array<std::uint32_t, 4> g_notebookTextureRight{};

std::array<std::uint8_t, 4> g_notebookLeftPageFlags{};

std::array<std::uint8_t, 4> g_notebookRightPageFlags{};

std::array<std::array<char, 1280>, 4> g_memoText{};

std::array<const char *, 4> g_memoTextPages{};

std::array<std::byte *, 4> g_notebookMemoTextureData{};


std::array<std::int32_t, 80> g_uiDefinitionParseState{};



void HandleUICallbacks() {
    for (std::size_t index = 0; index < g_uiElements.size(); ++index) {
        UIBaseRuntime &element = g_uiElements[index];
        if (element.setup) {
            if (element.config.init == 3)
                continue;
        } else if (element.loadState && element.loadState() == 3) {
            continue;
        }
        if (g_uiDefinitionParseState[index] != 4 && (!element.parseDefinitionFromFile || element.parseDefinitionFromFile() == 3))
            g_uiDefinitionParseState[index] = 3;
    }
}

void UI_ActivateElement(UIBaseRuntime &element) {
    if (!element.isVisible) {
        std::int64_t (*callback)() = nullptr;
        if (element.render) {
            element.render();
            callback = element.tick;
        } else if (element.initialize) {
            element.initialize();
            callback = element.postInit;
        }
        if (callback) {
            g_waitingForNextTick = 0;
            callback();
            g_waitingForNextTick = 1;
        }
        element.isVisible = 1;
    }
    element.isHandledExternally = 0;
}

std::uint32_t UI_SetPVRSurfaceTexture(std::int32_t surface, std::uint32_t texture) {
    return ExchangePVRSurfaceTexture(surface, texture);
}


void InitGUIStates() {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    cash.value = 0;
    cash.targetBalance = 0;
    cash.balanceAnimating = false;
    cash.selectedValue = 0;
    cash.state = UICashOverlayState::Idle;
    cash.timeoutFrames = 600;
    cash.autoCloseFrames = 0.0f;
    cash.useCash = true;
    cash.maximumValue = 0;
    cash.selectorActive = false;
    cash.elapsedFrames = 0;
}




void UI_GUITasksEnqueue() {
    EWZO_SetUiHandler(HandleEwzoUiEvent);
    if (g_commonIconSprites.count == 0)
        return;

    HLib::Task *const outerTask = CurrentTask;
    IconPresentationOwner &icon = g_iconPresentation;
    if (!IsOwnedUiTask(icon.rootTask, 14, SHEN_NAME(ICON), UI_ICONCallback, UI_ICONMainCallback)) {
        icon.rootTask = HLib_EnqueueTaskWithoutParameter(UI_ICONCallback, 14, 0, SHEN_NAME(ICON));
    }
    g_conversationPromptRuntimeReady = true;
    SCEN_SetConversationPromptEnabled(true);
    g_conversationPromptSceneEnabled = true;
    if (icon.rootTask) {
        CurrentTask = icon.rootTask;
        if (!IsOwnedUiTask(icon.menuTask, 15, SHEN_NAME(ICIS), UI_ICISCallback, UI_ICISMainCallback)) {
            icon.menuTask = HLib_EnqueueTaskWithoutParameter(UI_ICISCallback, 15, 0, SHEN_NAME(ICIS));
        }
        CurrentTask = outerTask;
    }
    g_playerStats.batteryDecrementMask = 0;
    InitGUIStates();
    if (icon.rootTask) {
        CurrentTask = icon.rootTask;
        if (!IsOwnedUiTask(icon.moneyTask, 8, 0x6f4d4349, UI_ICMoCallback)) {
            icon.moneyTask = HLib_EnqueueTaskWithoutParameter(UI_ICMoCallback, 8, 0, 0x6f4d4349);
        }
        if (!IsOwnedUiTask(icon.actionSelectorTask, 8, 0x70534349, UI_ICSpCallback, UI_ICSpMainCallback)) {
            icon.actionSelectorTask = HLib_EnqueueTaskWithoutParameter(UI_ICSpCallback, 8, 0, 0x70534349);
        }
        if (!IsOwnedUiTask(g_sfxOverlayTask, 8, 0x75514349, UI_ICQuCallback, UI_ICQuMainCallback)) {
            g_sfxOverlayTask = HLib_EnqueueTaskWithoutParameter(UI_ICQuCallback, 8, 0, 0x75514349);
        }
        if (!IsOwnedUiTask(icon.notificationTask, 8, SHEN_NAME(ICNQ), UI_ICNQCallback, UI_ICNQMainCallback)) {
            icon.notificationTask = HLib_EnqueueTaskWithoutParameter(UI_ICNQCallback, 8, 0, SHEN_NAME(ICNQ));
        }
        if (!IsOwnedUiTask(icon.cassetteTask, 8, SHEN_NAME(ICMC), UI_ICMCCallback, UI_ICMCMainCallback)) {
            icon.cassetteTask = HLib_EnqueueTaskWithoutParameter(UI_ICMCCallback, 8, 0, SHEN_NAME(ICMC));
        }
        CurrentTask = outerTask;
    }

    if (!IsOwnedUiTask(g_sleepSaveTrainingMenu.task, 8, SHEN_NAME(ICSV), UI_ICSVCallback, UI_ICSVMainCallback)) {
        g_sleepSaveTrainingMenu.task = HLib_EnqueueTaskWithoutParameter(UI_ICSVCallback, 8, 0, SHEN_NAME(ICSV));
    }
    EnsureHelpTask();
    TMEXRuntimeState &tmex = TMEX_GetRuntimeState();
    tmex.watchModelOwner = nullptr;
    WatchModelFilePtr = nullptr;
    g_TMEX_lifecycleState = TMEXLifecycleState::Inactive;
    g_tmpCameraLookAtV3_2 = {};
    g_TMEX_watchModelRotation = {};
    g_TMEX_auxHandsPackedTime = 0;
    g_tmexTask = nullptr;
    g_TMEX_displayFlag = 0;
    g_TMEX_State = 0;
    g_TMEX_forceShowWatchNode162 = 0;
    EnqueueMETK();
    g_pATCK = nullptr;
    g_LAPTIME_sprites.fill(-1);
    g_RaceTimeSlots.fill(-1);
    g_RaceDial_UI = nullptr;
    g_pGaugeValueSrc = nullptr;
    g_ATCK_flags = 0;
    g_BlinkPhase = 0;
    g_ATCK_enable_state = 0;
    g_racedial_state = 0;
    g_atckResetRecord = {
        .rect = {0.0f, 0.0f, 1.0f, 1.0f}, .timer = 2.0f, .field14 = 0, .field18 = 0, .field1c = 277, .field1e = 0x10, .field20 = 0x10, .field22 = 0};
    for (std::uint8_t &flags : g_playerStats.ownedItemMenuFlags0To66)
        flags &= static_cast<std::uint8_t>(~2U);
    if (g_debugForceShowAllItems != 1 && !GetInventoryItemCount(3))
        AddInventoryItemCount(3, 1);
}

void UI_GUITasksShutdown() {
    HandleEwzoUiEvent(EwzoUiEvent::Close, EWZO_GetUiBinding());
    EWZO_SetUiHandler(nullptr);
    UI_HELPClear();
    UI_ICONClear();
    CDTT_Clear();
    UI_ICSpCancel();
    ResetOwnedElement(kUiGameMenu);
    ResetOwnedElement(kUiSharedItems);
    ResetOwnedElement(kUiSubMenuCassettes);
    ResetOwnedElement(kUiGameMenuBackground);
    ResetOwnedElement(kUiGameMessage);
    ResetOwnedElement(kUiActionSelector);
    ResetOwnedElement(kUiGameHud);
    ResetOwnedElement(kUiGameHudBattery);
    ResetOwnedElement(kUiGameHudWatch);
    ResetOwnedElement(kUiHelpNew);
    ResetOwnedElement(kUiCredits);
    HideOwnedElement(kUi70ManBattle);
    UI_CloseRetry70ManBattlePrompt();

    DestroyOwnedTask(g_sicnTask, SHEN_NAME(SICN));
    g_sicnTask = nullptr;
    g_sicnTaskActive = 0;
    g_uiState = 0;
    g_sicnTransientStateTimeoutFrames = 0;
    g_currentSicn = UISicnPausePolicy::ClearOnPause;
    g_sicnPromptSlots = {};
    g_hudPromptFlagsThisFrame = 0;
    g_sicnPopulatedHudPromptsThisFrame = false;
    g_sicnClearPendingUntilUnpause = false;
    Shenmue_nPrevPauseState = 0;

    DestroyOwnedTask(g_helpPresentation.task, SHEN_NAME(HELP));
    g_helpPresentation = {};

    ReleasePadCapture(g_sleepSaveTrainingMenu.input, g_sleepSaveTrainingMenu.previousInput);
    DestroyOwnedTask(g_sleepSaveTrainingMenu.task, SHEN_NAME(ICSV));
    g_sleepSaveTrainingMenu = {};
    ReleaseIconInput();
    DestroyOwnedTask(g_iconPresentation.itemLookTask, SHEN_NAME(ICLI));
    DestroyOwnedTask(g_iconPresentation.systemStateTask, SHEN_NAME(ICSS));
    DestroyOwnedTask(g_iconPresentation.menuTask, SHEN_NAME(ICIS));
    DestroyOwnedTask(g_iconPresentation.moneyTask, 0x6f4d4349);
    DestroyOwnedTask(g_iconPresentation.actionSelectorTask, 0x70534349);
    DestroyOwnedTask(g_sfxOverlayTask, 0x75514349);
    DestroyOwnedTask(g_iconPresentation.notificationTask, SHEN_NAME(ICNQ));
    DestroyOwnedTask(g_iconPresentation.cassetteTask, SHEN_NAME(ICMC));
    DestroyOwnedTask(g_iconPresentation.rootTask, SHEN_NAME(ICON));
    g_iconPresentation.actionSelectorTask = nullptr;
    g_iconPresentation.notificationTask = nullptr;
    g_iconPresentation.cassetteTask = nullptr;
    g_sfxOverlayTask = nullptr;

    g_moveLearnedPresentation = {};
    g_actionSelectorPresentation = {};
    g_cashOverlayPresentation = {};
    if (g_iconPresentation.itemInfoSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(g_iconPresentation.itemInfoSurface);
    g_iconPresentation = {};
}



std::array<std::array<char, 256>, 8> g_70mbLabelText{};

std::int32_t g_70mbShownIconStatus = 0;

std::int32_t g_70mbStartPromptIconVisible = 0;

std::uint8_t g_lcdmHelpIconSpritesLoaded = 0;

std::int16_t g_helpIconWidth = 0;

std::int16_t g_helpIconHeight = 0;


std::array<std::int32_t, 16> g_helpRetrySurfaces{};

// convCanOpen: the free-roam gate the ICON system menu and the notebook share.
bool convCanOpen() {
    return SCEN_IsConversationPromptEnabled() && ShouldRun() && (Shenmue_nPauseState & 2) == 0 && g_isPlayerGameStateActive == 7 &&
        g_conversationPromptRuntimeReady && !SCEN_HasActiveScript() && !VFMA_IsFighting() && g_searchTemporaryFocusedCharacter == 0 &&
        g_conversationPromptSceneEnabled;
}

void UI_SetConversationPromptSceneEnabled(bool enabled) {
    g_conversationPromptSceneEnabled = enabled;
}




std::int32_t COMICONSPRLoad(PackSlot *pack) {
    const std::uint32_t size = PACK_FindBinFileSize(pack, "COMICON.SPR");
    void *const file = LoadFileIntoHeapMem(pack, "COMICON.SPR", 0x204e4942U);
    g_texnTextureCacheBank[0] = 2;
    OwnedSpriteSet<32> &sprites = g_commonIconSprites;
    sprites.loadAttempted = true;
    sprites.count = 0;
    if (!file)
        return 0;
    std::byte *const bytes = static_cast<std::byte *>(file);
    for (std::size_t offset = 0; offset < size;) {
        std::uint32_t token = 0;
        std::uint32_t chunkSize = 0;
        if (offset + 16 > size || sprites.count == sprites.assets.size())
            break;
        std::memcpy(&token, bytes + offset, sizeof(token));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (!token || !chunkSize)
            break;
        OwnedSpriteAsset &asset = sprites.assets[sprites.count++];
        std::memcpy(&asset.textureIdLow, bytes + offset + 8, sizeof(asset.textureIdLow));
        std::memcpy(&asset.textureIdHigh, bytes + offset + 12, sizeof(asset.textureIdHigh));
        asset.surface = LoadTEXN(bytes + offset);
        offset += chunkSize;
    }
    return static_cast<std::int32_t>(sprites.count);
}

void UI_PresentationOwnersInit() {
    EWZO_SetUiHandler(HandleEwzoUiEvent);
    LoadSpriteSet(g_commonIconSprites, "COMICON.SPR", SHEN_NAME(ICON));
    UI_GUITasksEnqueue();
}

void UI_PresentationOwnersShutdown() {
    UI_GUITasksShutdown();
    ReleaseSpriteSet(g_commonIconSprites);
}

UIPresentationSnapshot UI_GetPresentationSnapshot() {
    return {
        g_currentHelpTokenId,
        g_helpButtonId,
        static_cast<std::uint8_t>(g_helpPresentation.onScreenCount > 0),
        static_cast<std::uint8_t>(g_helpPresentation.closing),
        g_helpPresentation.alpha,
        g_iconPresentation.state,
        g_iconPresentation.kind,
        g_iconPresentation.expectedInput,
        g_iconPresentation.alpha,
        g_iconPresentation.ownerMask,
        g_uiState,
        g_sicnTransientStateTimeoutFrames,
        g_currentSicn,
        g_cdttMode,
        g_cdttAlpha,
        {
            g_sfxOverlayState,
            g_sfxTimeoutFrames,
            g_sfxFrameCounter,
            g_sfxOverlayResult[0],
            g_sfxOverlayTimeoutPendingBonusFrames,
            g_qteOverlayTimeoutMaxFrames,
            g_qteOverlay.stepFrames,
            g_qteOverlay.accumulatedFrames,
            g_sfxOverlayTimeoutBoostCount,
            static_cast<std::uint8_t>(g_sfxOverlayTimeoutBoostEnabled),
            static_cast<std::uint8_t>(g_qteSessionActive),
        },
        g_helpOverlayState,
        g_iconPresentation.menuState,
        g_iconPresentation.menuSelection,
        static_cast<std::uint8_t>((g_iconPresentation.iconFlags & 0x20U) != 0),
        static_cast<std::uint8_t>(g_loadScreenPresentation.windowAlpha[8] > 0.0f),
    };
}



std::int32_t g_qteSessionActive = 0;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

std::int32_t DecodeUITextCharacter(const char *text, std::uint32_t *codepoint, std::int32_t size);

std::int32_t FillStringData(const char *text, std::int32_t size);

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {



void UI_ResetForSequenceStart() {
    const auto hideElement = [](std::int32_t index) {
        if (g_currUIState != -1 && g_currUIState != index)
            return false;
        UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(index)];
        if (element.isVisible && element.destroy && element.destroy() != 1 && g_busy_reset != 1) {
            return false;
        }
        element.isVisible = 0;
        return true;
    };

    if (g_uiElements[kUiGameMenu].isVisible && hideElement(kUiGameMenu))
        ResetUIStates();
    static_cast<void>(hideElement(kUiSubMenuCassettes));
    static_cast<void>(hideElement(kUiSubMenuCollection));
    static_cast<void>(hideElement(kUiGameMenuBackground));
}

void UI_HandleDobuitaEventStartTransition() {
    DestroyTMEX();
    UI_ResetForSequenceStart();
}

void UI_ResetForCPCT() {
    if (g_isPlayerGameStateActive != 4)
        return;
    IconPresentationOwner &icon = g_iconPresentation;
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    icon.iconFlags &= static_cast<std::uint16_t>(~0x0400U);
    selector.state = UIActionSelectorState::Idle;
    icon.overlayMode = 0;
    selector.openRequested = false;
    selector.resetRequested = false;
    selector.panelX = -176.0f;
    if (icon.input && (icon.input->flags & 2U) != 0 && icon.previousInput) {
        shenUpdateInputsInto(icon.previousInput);
        icon.previousInput = nullptr;
    }
    g_previousHelpTokenId = g_currentHelpTokenId;
    std::uint8_t helpToken = selector.savedHelpToken;
    g_isPlayerGameStateActive = 7;
    if (static_cast<std::int8_t>(helpToken) >= 68)
        helpToken = 0;
    g_currentHelpIconState = 0;
    g_defaultHelpTokenId = helpToken;
    g_currentHelpTokenId = helpToken;
    g_helpButtonId = selector.savedHelpButton;
    if ((selector.savedPauseState & 0x20) != 0) {
        Shenmue_nPauseState = selector.savedPauseState;
        selector.savedPauseState = 0;
    }
}

void UI_CloseConversationUIAndReturnToFreeRoam() {
    if (g_isPlayerGameStateActive != 0)
        return;

    UI_ResetForSequenceStart();
    static_cast<void>(ResetOwnedElement(kUiGameHudBattery));
    shenEndNotebook();
    DestroyTMEX();
    shenRefreshUI();
    if (g_iconPresentation.sharedItemMenuState)
        UI_ICLIResetSharedItemMenu();
    UI_StopPlayingCassettes();

    IconPresentationOwner &icon = g_iconPresentation;
    icon.systemGameState = UIIconSystemGameState::SystemMenuIdle;
    icon.cassetteItemState = 0;
    g_bIsConversationCamInactive = 1;
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 0;
    ApplyPendingGameState(0);

    icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
    icon.itemInfoPopupPosition[0] = -672.0f;
    icon.cameraTransition.mode = 0;
    icon.overlayTransitionStage = 5;
    icon.menuState = UIIconSystemMenuState::Closed;
    if (icon.itemInfoSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(icon.itemInfoSurface);
    icon.itemInfoSurface = -1;

    g_previousHelpTokenId = g_currentHelpTokenId;
    const std::uint8_t helpToken = icon.savedHelpToken < 68 ? icon.savedHelpToken : 0;
    g_defaultHelpTokenId = helpToken;
    g_currentHelpTokenId = helpToken;
    g_helpButtonId = icon.savedHelpButton;
    g_currentHelpIconState = 0;
    if (Character *const ryo = MapRyo()) {
        ryo->motionFlags = static_cast<std::uint8_t>(icon.savedRyoMotionVisible ? ryo->motionFlags | 4U : ryo->motionFlags & ~4U);
        ryo->flags.value = icon.savedRyoFlag10 ? ryo->flags.value | 0x10U : ryo->flags.value & ~0x10U;
        ryo->renderOpacity = 1.0f;
    }
    UI_SICNClearState(static_cast<std::uint16_t>(((~g_uiState) & 0x200U) | 0x0dffU));
}

void UI_ResetConversationDetailsState() {
    switch (g_isPlayerGameStateActive) {
    case 0:
        UI_CloseConversationUIAndReturnToFreeRoam();
        break;
    case 4:
        UI_ResetForCPCT();
        break;
    case 5:
        UI_ICMoWaitCloseAnimation();
        break;
    case 6: {
        IconPresentationOwner &icon = g_iconPresentation;
        icon.itemInfoPopupPosition[0] = -672.0f;
        icon.itemInfoPopupPosition[1] = 55.0f;
        icon.itemInfoPopupPhase = 0;
        if (icon.itemInfoSurface != -1)
            PVR_FreeAllocatedSurfaceByIndex(icon.itemInfoSurface);
        icon.itemInfoPopupVisible = 0;
        icon.itemInfoSurface = -1;
        g_isPlayerGameStateActive = 7;
        break;
    }
    default:
        break;
    }

    IconPresentationOwner &icon = g_iconPresentation;
    icon.overlayTransitionStage = 0;
    icon.overlayMode = 0;
    icon.iconFlags = 0;
    UI_StopPlayingCassettes();
    if (icon.input && (icon.input->flags & 2U) != 0 && icon.previousInput) {
        shenUpdateInputsInto(icon.previousInput);
        icon.previousInput = nullptr;
    }
    g_isPlayerGameStateActive = 7;
}



void ResetUIStates() {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.iconFlags &= static_cast<std::uint16_t>(~0x20U);
    icon.pendingState = 0;
    const std::int32_t author = icon.cameraTransition.savedAuthorState;
    if (author == 0 || author == 9) {
        g_temporaryGameState = icon.cameraTransition.savedTemporaryGameState;
        g_bIsConversationCamInactive = 0;
        ApplyPendingGameState(g_previousAuthorState);
        SetNewAuthorGameState(author);
        icon.cameraTransition.mode = 0;
        icon.menuState = UIIconSystemMenuState::Closed;
        icon.menuCloseRequested = false;
        icon.overlayTransitionStage = 5;
    } else {
        RestoreIconSystemMenuCameraTransition(icon.cameraTransition);
        icon.menuState = UIIconSystemMenuState::ClosingTransition;
    }
    if (icon.itemInfoSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(icon.itemInfoSurface);
    icon.itemInfoSurface = -1;
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_currentHelpIconState = 0;
    g_defaultHelpTokenId = icon.savedHelpToken < 68 ? icon.savedHelpToken : 0;
    g_currentHelpTokenId = g_defaultHelpTokenId;
    g_helpButtonId = icon.savedHelpButton;
}

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

constexpr std::array<ItemRemarkEntry, 11> kItemRemarks{{{17, 17, 0, 0}, {19, 49, 1, 1}, {21, 18, 3, 5}, {23, 48, 6, 6}, {25, 47, 1, 1}, {27, 16, 0, 0},
    {29, 46, 1, 1}, {31, 75, 2, 2}, {35, -1, 4, 4}, {33, 74, 2, 2}, {36, -1, 7, 3}}};


constexpr std::array<std::array<const char *, 3>, 8> kItemRemarkLines{{{"E1060A001", "E1060A002", "E1060A003"}, {"E1060A004", "E1060A005", "E1060A006"},
    {"E1060A007", "E1060A008", "E1060A009"}, {"E1060A013", "E1060A014", "E1060A014"}, {"E1060A017", "E1060A018", "E1060A019"},
    {"E1060A016", "E1060A010", "E1060A012"}, {"E1060A010", "E1060A011", "E1060A012"}, {"E1060A013", "E1060A014", "E1060A013"}}};


constexpr std::array<std::int16_t, 3> kMoveSetMoveCounts{19, 18, 14};


constexpr std::array<std::int32_t, 5> kGameMenuSubmenus{kUiSubMenuItems, kUiSubMenuCollection, kUiSubMenuMoves, kUiLoadSave, kUiSubMenuOptions};

std::int32_t UI_FindViewButton(std::uint32_t slotIndex, const char *id);

std::int32_t TextFormat(char *buffer, std::int32_t size, const char *tag, const char *replacement);



std::array<const char *, 9> g_languageChoiceLabels{};

std::array<std::int32_t, 9> g_languageChoiceValues{};


constexpr std::array<const char *, 3> kOptionsOffOnChoices{
    "UIOTGLOFF0", "UIOTGLON00", nullptr}; // @text UIOTGLOFF0 EN: "Off" | JA: "オフ" ; UIOTGLON00 EN: "On" | JA: "オン"

constexpr std::array<std::int32_t, 3> kOptionsChoiceValues{0, 1, 2};

constexpr std::array<const char *, 3> kOptionsAudioLanguageChoices{
    "UIOTGLENG0", "UIOTGLJPN0", nullptr}; // @text UIOTGLENG0 EN: "English" | JA: "英語" ; UIOTGLJPN0 EN: "Japanese" | JA: "日本語"

constexpr std::array<const char *, 9> kOptionsLanguages{"UIOTGLENG0", "UIOTGLJPN0", "UIOTGLFRN0", "UIOTGLGER0", "UIOTGLKOR0",
    "UIOTGLCHTR", // @text UIOTGLENG0 EN: "English" | JA: "英語" ; UIOTGLJPN0 EN: "Japanese" | JA: "日本語" ; UIOTGLFRN0 EN: "French" | JA: "フランス語" ; UIOTGLGER0 EN: "German" | JA: "ドイツ語" ; UIOTGLKOR0 EN: "Korean" | JA: "韓国語" ; UIOTGLCHTR EN: "T. Chinese" | JA: "中国語（繁体字）"
    "UIOTGLCHSU", "UIOTGLDEF0", nullptr}; // @text UIOTGLCHSU EN: "S. Chinese" | JA: "中国語（簡体字）" ; UIOTGLDEF0 EN: "Default" | JA: "デフォルト"

constexpr std::array<std::int32_t, 9> kOptionsLanguageValues{0, 1, 2, 3, 4, 5, 6, -1, 0};

constexpr std::array<const char *, 7> kOptionsLanguageNames{"UIOTGLENG0", "UIOTGLJAP0", "UIOTGLFRN0", "UIOTGLGER0", "UIOTGLKOR0", "UIOTGLCHTR",
    "UIOTGLCHSU"}; // @text UIOTGLENG0 EN: "English" | JA: "英語" ; UIOTGLFRN0 EN: "French" | JA: "フランス語" ; UIOTGLGER0 EN: "German" | JA: "ドイツ語" ; UIOTGLKOR0 EN: "Korean" | JA: "韓国語" ; UIOTGLCHTR EN: "T. Chinese" | JA: "中国語（繁体字）" ; UIOTGLCHSU EN: "S. Chinese" | JA: "中国語（簡体字）"

constexpr std::array<const char *, 5> kOptionsSubtitleModes{"UIMODECINE", "UIMODESHEN", "UIMODEGAME", "UIMODETEXT",
    nullptr}; // @text UIMODECINE EN: "Cinema Mode" | JA: "シネマモード" ; UIMODESHEN EN: "Shenmue Mode" | JA: "シェンムーモード" ; UIMODEGAME EN: "Game Mode" | JA: "ゲームモード" ; UIMODETEXT EN: "Text Mode" | JA: "テキストモード"

constexpr std::array<const char *, 4> kOptionsSubtitleModeTexts{"UIMODECINEDS", "UIMODESHENDS", "UIMODEGAMEDS",
    "UIMODETEXTDS"}; // @text UIMODECINEDS EN: "Voice ON, Text OFF. Dialog skip disabled." | JA: "声だけでゲームを進行します。\n会話はスキップできません。" ; UIMODESHENDS EN: "Voice ON, Text ON. Subtitles appear only if dialog is skipped." | JA: "声と字幕でゲームを進行します。\nスキップ時、字幕が表示されます。" ; UIMODEGAMEDS EN: "Voice ON, Text ON" | JA: "声と字幕でゲームを進行します。" ; UIMODETEXTDS EN: "Voice OFF, Text ON" | JA: "字幕だけでゲームを進行します。"

constexpr std::array<std::int32_t, 4> kOptionsSubtitleModeValues{0, 1, 2, 3};

constexpr std::array<std::int32_t, 10> kOptionsAudioDeviceValues{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

constexpr std::array<std::int32_t, 2> kOptionsPercentRange{0, 100};

constexpr std::array<const char *, 3> kOptionsInvertYChoices{
    "UIGNRLNO00", "UIACTSYES0", nullptr}; // @text UIGNRLNO00 EN: "No" | JA: "オフ" ; UIACTSYES0 EN: "Yes" | JA: "オン"

constexpr std::array<const char *, 3> kOptionsWeatherChoices{
    "UIOPWEASHE", "UIOPWEAYOK", nullptr}; // @text UIOPWEASHE EN: "Shenmue" | JA: "シェンムーオリジナル" ; UIOPWEAYOK EN: "Yokohama" | JA: "気象台"

constexpr std::array<const char *, 4> kOptionsWindowModeChoices{"UIOTGLWIND", "UIOTGLBORD", "UIOTGLFULL",
    nullptr}; // @text UIOTGLWIND EN: "Windowed" | JA: "ウィンドウ" ; UIOTGLBORD EN: "Borderless" | JA: "ボーダーレス" ; UIOTGLFULL EN: "Fullscreen" | JA: "フルスクリーン"

constexpr std::array<std::int32_t, 2> kOptionsResolutionScaleRange{40, 100};

constexpr std::array<std::int32_t, 2> kOptionsSuperSamplingRange{100, 200};

constexpr std::array<const char *, 4> kOptionsBloomChoices{"UIOTGLOFF0", "UIOTGLLOW0", "UIOTGLHIGH",
    nullptr}; // @text UIOTGLOFF0 EN: "Off" | JA: "オフ" ; UIOTGLLOW0 EN: "Low" | JA: "低" ; UIOTGLHIGH EN: "High" | JA: "高"

constexpr std::array<std::int32_t, 2> kOptionsBrightnessRange{-100, 100};

constexpr std::array<const char *, 3> kOptionsAspectChoices{
    "UIASPECT43", "UIASPECT169", nullptr}; // @text UIASPECT43 EN: "4:3" | JA: "4:3" ; UIASPECT169 EN: "16:9" | JA: "16:9"

constexpr std::array<std::int32_t, 2> kOptionsUiScaleRange{80, 100};



std::array<std::array<char, 18>, 10> g_optionsAudioDeviceTexts{};

std::array<const char *, 11> g_optionsAudioDeviceChoices{};



std::array<UIOption, 7> g_optionsAudioTable{{
    {.type = UIOptionType::List,
        .name = "UIOPTADEV",
        .text = Options_AudioDeviceText, // @text EN: "Audio Output Device" | JA: "音声出力デバイス"
        .choice = [](std::int32_t index) { return g_optionsAudioDeviceChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsAudioDeviceValues[static_cast<std::size_t>(index)]; },
        .getInt = [] { return g_applicationSettings.savedAudioDevice; },
        .setInt = [](std::int32_t value) { g_applicationSettings.savedAudioDevice = value; }},
    {.type = UIOptionType::Toggle,
        .name = "UIOPTADLAN",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTALDSC");
            }, // @text UIOPTADLAN EN: "Audio Language" | JA: "音声言語" ; UIOPTALDSC EN: "Toggles the language used for audio." | JA: "音声言語の切り換えを行います。"
        .choice = [](std::int32_t index) { return kOptionsAudioLanguageChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return g_applicationSettings.savedAudioLanguage; },
        .setInt = [](std::int32_t value) { g_applicationSettings.savedAudioLanguage = value; },
        .available = [] { return GetIsJPVersion() ? 1 : 0; }},
    {.type = UIOptionType::List,
        .name = "UIOTTXTLNG",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTTLDSC");
            }, // @text UIOTTXTLNG EN: "Text Language" | JA: "字幕言語" ; UIOPTTLDSC EN: "Select the language used for subtitles & UI." | JA: "字幕とUIの言語を選択します。"
        .choice = [](std::int32_t index) { return kOptionsLanguages[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsLanguageValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return g_applicationSettings.savedSubtitleLanguage; },
        .setInt = [](std::int32_t value) { g_applicationSettings.savedSubtitleLanguage = value; },
        .available = [] { return GetIsJPVersion() ? 1 : 0; }},
    {.type = UIOptionType::List,
        .name = "UIOPTSBSEN", // @text EN: "Dialogue & Text" | JA: "声＆字幕"
        .text =
            [](std::int32_t choice) {
                return InitAndRetrieveStringFromID(static_cast<std::uint32_t>(choice) >= 4U ?
                        "UIOPTDTDSC" : // @text EN: "Select which mode dialogue & subtitles\nare presented in." | JA: "音声と字幕の表示方法を選択します。"
                        kOptionsSubtitleModeTexts[static_cast<std::size_t>(choice)]);
            },
        .choice = [](std::int32_t index) { return kOptionsSubtitleModes[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsSubtitleModeValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return g_applicationSettings.savedDialogueMode; },
        .setInt = [](std::int32_t value) { g_applicationSettings.savedDialogueMode = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIOPTSSVOL",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTSFXVDSC");
            }, // @text UIOPTSSVOL EN: "SFX Volume" | JA: "SE音量" ; UIOPTSFXVDSC EN: "Adjusts the volume of sound effects." | JA: "効果音の音量を調節します。"
        .choiceValue = [](std::int32_t index) { return kOptionsPercentRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.savedSfxVolume; },
        .setFloat = [](float value) { g_applicationSettings.savedSfxVolume = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIOPTSMVOL",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTMUSDSC");
            }, // @text UIOPTSMVOL EN: "Music Volume" | JA: "BGM音量" ; UIOPTMUSDSC EN: "Adjusts the volume of music." | JA: "音楽の音量を調節します。"
        .choiceValue = [](std::int32_t index) { return kOptionsPercentRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.savedMusicVolume; },
        .setFloat = [](float value) { g_applicationSettings.savedMusicVolume = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIOPTSTVOL",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTSPEDSC");
            }, // @text UIOPTSTVOL EN: "Speech Volume" | JA: "ボイス音量" ; UIOPTSPEDSC EN: "Adjusts the volume of speech." | JA: "声の音量を調節します。"
        .choiceValue = [](std::int32_t index) { return kOptionsPercentRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.savedDialogueVolume; },
        .setFloat = [](float value) { g_applicationSettings.savedDialogueVolume = value; }},
}};

// A key binding row of the Controls table: its action token (also its description) and the input action it binds.
constexpr UIOption Options_KeyBinding(const char *name, const char *(*text)(std::int32_t), std::int32_t action) {
    return {.type = UIOptionType::KeyBinding, .name = name, .text = text, .current = action, .localizeChoices = 1};
}

// A heading row of the Controls table.
constexpr UIOption Options_Heading(const char *name) {
    return {.type = UIOptionType::Heading, .name = name, .localizeChoices = 1};
}



std::array<UIOption, 42> g_optionsControlsTable{{
    {.type = UIOptionType::Toggle,
        .name = "UISWAPTRIG",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UISWAPTRDS");
            }, // @text UISWAPTRIG EN: "Swap Look & Run" | JA: "走りボタン変更" ; UISWAPTRDS EN: "Swap the inputs used for look & run." | JA: "「走る」と「ズーム」の機能を入れ替えます。\nオンにすると、L2ボタンで走ります。"
        .choice = [](std::int32_t index) { return kOptionsOffOnChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return static_cast<std::int32_t>(g_applicationSettings.savedInput.swapTriggers); },
        .setInt = [](std::int32_t value) { g_applicationSettings.savedInput.swapTriggers = value != 0; }},
    {.type = UIOptionType::Toggle,
        .name = "UIOPTSINVY",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTLYADSC");
            }, // @text UIOPTSINVY EN: "Invert Look Y Axis" | JA: "上下反転" ; UIOPTLYADSC EN: "Inverts the Y axis when looking around." | JA: "見回す操作を行う際に、\n入力に対して上下を反転させます。"
        .choice = [](std::int32_t index) { return kOptionsInvertYChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return static_cast<std::int32_t>(g_applicationSettings.savedInput.invertY); },
        .setInt = [](std::int32_t value) { g_applicationSettings.savedInput.invertY = value != 0; }},
    {.type = UIOptionType::Toggle,
        .name = "UIOPTAJUMP",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTAJDSC");
            }, // @text UIOPTAJUMP EN: "Area Jump" | JA: "エリアジャンプ" ; UIOPTAJDSC EN: "Enable area jumping when exiting\nthe Hazuki residence." | JA: "芭月家玄関より、状況に応じ、\n行き先に直接ジャンプできます。"
        .choice = [](std::int32_t index) { return kOptionsOffOnChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return static_cast<std::int32_t>(g_applicationSettings.savedInput.areaJump & 0xffU); },
        .setInt =
            [](std::int32_t value) {
                std::uint16_t &areaJump = g_applicationSettings.savedInput.areaJump;
                areaJump = static_cast<std::uint16_t>((areaJump & 0xff00U) | (value != 0 ? 1U : 0U));
            }},
    {.type = UIOptionType::Toggle,
        .name = "UIOPTWEATH",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPWEADES");
            }, // @text UIOPTWEATH EN: "Weather Mode" | JA: "マジックウェザー" ; UIOPWEADES EN: "Set weather mode to use Shenmue data\nor actual 1986 Yokohama data." | JA: "シェンムーオリジナル、もしくは、1986年発表の\n実際の横浜地方気象台データを設定します。"
        .choice = [](std::int32_t index) { return kOptionsWeatherChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return g_applicationSettings.savedInput.weatherMode; },
        .setInt = [](std::int32_t value) { g_applicationSettings.savedInput.weatherMode = value; },
        .available = [] { return HPLG_HasBookkeepingProgress() ? 1 : 0; }},
    Options_Heading("UIOPTTKBGE"), // @text EN: "Key Bindings - General" | JA: "キーアサイン（一般）"
    Options_KeyBinding(
        "UIBUT2CONF", [](std::int32_t) { return Options_ReadString("UIBUT2CONF"); }, 0), // @text EN: "Talk / Action" | JA: "話す/アクション"
    Options_KeyBinding(
        "UIBUT2CANC", [](std::int32_t) { return Options_ReadString("UIBUT2CANC"); }, 1), // @text EN: "Cancel" | JA: "キャンセル"
    Options_KeyBinding(
        "UIBUT2EVEN", [](std::int32_t) { return Options_ReadString("UIBUT2EVEN"); }, 2), // @text EN: "Notebook / Action" | JA: "メモ帳/アクション"
    Options_KeyBinding(
        "UIBUT2SYST", [](std::int32_t) { return Options_ReadString("UIBUT2SYST"); }, 3), // @text EN: "Inventory / Menu" | JA: "アイテム/メニュー"
    Options_KeyBinding(
        "UIBUTTHELP", [](std::int32_t) { return Options_ReadString("UIBUTTHELP"); }, 7), // @text EN: "Pause/Help" | JA: "ポーズ/ヘルプ"
    Options_Heading("UIOPTTKBMC"), // @text EN: "Key Bindings - Movement / Camera" | JA: "キーアサイン（移動＆カメラ）"
    Options_KeyBinding(
        "UIBUTTLOOK", [](std::int32_t) { return Options_ReadString("UIBUTTLOOK"); }, 8), // @text EN: "Look Zoom" | JA: "ズーム"
    Options_KeyBinding(
        "UIBUTTRUN0", [](std::int32_t) { return Options_ReadString("UIBUTTRUN0"); }, 9), // @text EN: "Run" | JA: "走る"
    Options_KeyBinding(
        "UIMOVEUP00", [](std::int32_t) { return Options_ReadString("UIMOVEUP00"); }, 14), // @text EN: "Walk Forward" | JA: "前に進む"
    Options_KeyBinding(
        "UIMOVEDOWN", [](std::int32_t) { return Options_ReadString("UIMOVEDOWN"); }, 15), // @text EN: "Turnaround" | JA: "振り向く"
    Options_KeyBinding(
        "UIMOVELEFT", [](std::int32_t) { return Options_ReadString("UIMOVELEFT"); }, 16), // @text EN: "Turn Left" | JA: "左に曲がる"
    Options_KeyBinding(
        "UIMOVERIGH", [](std::int32_t) { return Options_ReadString("UIMOVERIGH"); }, 17), // @text EN: "Turn Right" | JA: "右に曲がる"
    Options_KeyBinding(
        "UILOOKUP00", [](std::int32_t) { return Options_ReadString("UILOOKUP00"); }, 10), // @text EN: "Look Up" | JA: "上を見る"
    Options_KeyBinding(
        "UILOOKDOWN", [](std::int32_t) { return Options_ReadString("UILOOKDOWN"); }, 11), // @text EN: "Look Down" | JA: "下を見る"
    Options_KeyBinding(
        "UILOOKLEFT", [](std::int32_t) { return Options_ReadString("UILOOKLEFT"); }, 12), // @text EN: "Look Left" | JA: "左を見る"
    Options_KeyBinding(
        "UILOOKRIGH", [](std::int32_t) { return Options_ReadString("UILOOKRIGH"); }, 13), // @text EN: "Look Right" | JA: "右を見る"
    Options_Heading("UIOPTTKBQT"),                                                        // @text EN: "Key Bindings - QTE" | JA: "キーアサイン（QTE）"
    Options_KeyBinding(
        "UIQTE00001", [](std::int32_t) { return Options_ReadString("UIQTE00001"); }, 18), // @text EN: "Quick Time Action 1" | JA: "クイックタイムアクション１"
    Options_KeyBinding(
        "UIQTE00002", [](std::int32_t) { return Options_ReadString("UIQTE00002"); }, 19), // @text EN: "Quick Time Action 2" | JA: "クイックタイムアクション２"
    Options_KeyBinding(
        "UIQTE00003", [](std::int32_t) { return Options_ReadString("UIQTE00003"); }, 20), // @text EN: "Quick Time Action 3" | JA: "クイックタイムアクション３"
    Options_KeyBinding(
        "UIQTE00004", [](std::int32_t) { return Options_ReadString("UIQTE00004"); }, 21), // @text EN: "Quick Time Action 4" | JA: "クイックタイムアクション４"
    Options_Heading("UIOPTTKBFT"), // @text EN: "Key Bindings - Fight" | JA: "キーアサイン（バトル）"
    Options_KeyBinding(
        "UIKICK0000", [](std::int32_t) { return Options_ReadString("UIKICK0000"); }, 22), // @text EN: "Kick" | JA: "足技"
    Options_KeyBinding(
        "UITHROW000", [](std::int32_t) { return Options_ReadString("UITHROW000"); }, 23), // @text EN: "Throw" | JA: "投技"
    Options_KeyBinding(
        "UIPUNCH000", [](std::int32_t) { return Options_ReadString("UIPUNCH000"); }, 24), // @text EN: "Punch" | JA: "手技"
    Options_KeyBinding(
        "UIBLOCK000", [](std::int32_t) { return Options_ReadString("UIBLOCK000"); }, 25), // @text EN: "Block/Evade" | JA: "防御/避ける"
    Options_KeyBinding(
        "UIFIGHTRUN", [](std::int32_t) { return Options_ReadString("UIFIGHTRUN"); }, 31), // @text EN: "Run" | JA: "走る"
    Options_KeyBinding(
        "UIFGHTASMO", [](std::int32_t) { return Options_ReadString("UIFGHTASMO"); }, 30), // @text EN: "Assigned Move" | JA: "登録技"
    Options_Heading("UIVEHICLEO"), // @text EN: "Key Bindings - Vehicle" | JA: "キーアサイン（フォークリフト）"
    Options_KeyBinding(
        "UIVCLRAISEL", [](std::int32_t) { return Options_ReadString("UIVCLRAISEL"); }, 32), // @text EN: "Raise Lift" | JA: "フォークの上下"
    Options_KeyBinding(
        "UIVCLCAMER", [](std::int32_t) { return Options_ReadString("UIVCLCAMER"); }, 33), // @text EN: "Camera" | JA: "視点切り替え"
    Options_KeyBinding(
        "UIVCLMAPOO", [](std::int32_t) { return Options_ReadString("UIVCLMAPOO"); }, 34), // @text EN: "Map" | JA: "マップ"
    Options_KeyBinding(
        "UIVCLMENUO", [](std::int32_t) { return Options_ReadString("UIVCLMENUO"); }, 35), // @text EN: "Menu" | JA: "メニュー"
    Options_KeyBinding(
        "UIVCLACCEL", [](std::int32_t) { return Options_ReadString("UIVCLACCEL"); }, 41), // @text EN: "Accelerate" | JA: "前進"
    Options_KeyBinding(
        "UIVCLREVER", [](std::int32_t) { return Options_ReadString("UIVCLREVER"); }, 40), // @text EN: "Reverse" | JA: "後退"
    Options_KeyBinding(
        "UIVCLLEFTO", [](std::int32_t) { return Options_ReadString("UIVCLLEFTO"); }, 48), // @text EN: "Left" | JA: "左"
    Options_KeyBinding(
        "UIVCLRIGHT", [](std::int32_t) { return Options_ReadString("UIVCLRIGHT"); }, 49), // @text EN: "Right" | JA: "右"
}};



std::array<UIOption, 11> g_optionsGraphicsTable{{
    {.type = UIOptionType::List,
        .name = "UIOPTSFULL",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTDMDSC");
            }, // @text UIOPTSFULL EN: "Display Mode" | JA: "フルスクリーンモード" ; UIOPTDMDSC EN: "Select the display mode." | JA: "表示モードを選択します。"
        .choice = [](std::int32_t index) { return kOptionsWindowModeChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return g_applicationSettings.display.windowMode; },
        .setInt = [](std::int32_t value) { g_applicationSettings.display.windowMode = value; }},
    {.type = UIOptionType::List,
        .name = "UIOPTSMONI",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTMONSDC");
            }, // @text UIOPTSMONI EN: "Monitor" | JA: "モニター" ; UIOPTMONSDC EN: "Select which monitor the game should appear on." | JA: "ゲームを表示するモニターを選択します。"
        .choice = [](std::int32_t index) -> const char * {
            return static_cast<std::uint32_t>(index) < g_monitorCount ? g_monitors[static_cast<std::size_t>(index)].adapterName : nullptr;
        },
        .choiceValue = [](std::int32_t index) { return index; },
        .getInt = [] { return g_applicationSettings.display.displayOutput; },
        .setInt = [](std::int32_t value) { g_applicationSettings.display.displayOutput = value; }},
    {.type = UIOptionType::List,
        .name = "UIOPTSRESO",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTRESDSC");
            }, // @text UIOPTSRESO EN: "Resolution" | JA: "解像度" ; UIOPTRESDSC EN: "Select the games resolution." | JA: "ゲームの解像度を選択します。"
        .choice = [](std::int32_t index) -> const char * {
            const std::uint32_t output = static_cast<std::uint32_t>(g_currentSettings.display.displayOutput);
            if (output >= g_monitorCount)
                return nullptr;
            const MonitorInfo &monitor = g_monitors[output];
            if (static_cast<std::uint32_t>(index) >= monitor.displayModeCount || !monitor.displayModes)
                return nullptr;
            return monitor.displayModes[static_cast<std::size_t>(index)].description;
        },
        .choiceValue = [](std::int32_t index) { return index; },
        .getInt = [] { return g_applicationSettings.display.antiAliasingMode; },
        .setInt = [](std::int32_t value) { g_applicationSettings.display.antiAliasingMode = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIOPTRESSCAL",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTRESSCDS");
            }, // @text UIOPTRESSCAL EN: "Resolution Scale" | JA: "解像度スケール" ; UIOPTRESSCDS EN: "Scale resolution down to emulate original\nresolution and/or improve performance." | JA: "解像度を低くします。オリジナルの解像度でプレイしたい、\nまたはパフォーマンスを向上させたい場合に最適です。"
        .choiceValue = [](std::int32_t index) { return kOptionsResolutionScaleRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.display.resolutionScale; },
        .setFloat = [](float value) { g_applicationSettings.display.resolutionScale = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIOPTSUPSA",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTSUPSADS");
            }, // @text UIOPTSUPSA EN: "Super Sampling" | JA: "スーパーサンプリング" ; UIOPTSUPSADS EN: "Enable super sampling. Higher values\nrequire more memory and may impact performance." | JA: "スーパーサンプリングを有効にします。高く設定すると、より多くの\nメモリが必要となり、パフォーマンスに影響が出る場合があります。"
        .choiceValue = [](std::int32_t index) { return kOptionsSuperSamplingRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.display.superSampling; },
        .setFloat = [](float value) { g_applicationSettings.display.superSampling = value; }},
    {.type = UIOptionType::Toggle,
        .name = "UIOPFXAALBL",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPFXAADSC");
            }, // @text UIOPFXAALBL EN: "FXAA" | JA: "FXAA" ; UIOPFXAADSC EN: "Toggle FXAA (Fast Approximate Anti-Aliasing)." | JA: "FXAAを切り替えます。"
        .choice = [](std::int32_t index) { return kOptionsOffOnChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return static_cast<std::int32_t>(g_applicationSettings.display.fxaa); },
        .setInt = [](std::int32_t value) { g_applicationSettings.display.fxaa = value != 0; }},
    {.type = UIOptionType::List,
        .name = "UIOPBLMTOGLB",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPBLMTOGDC");
            }, // @text UIOPBLMTOGLB EN: "Bloom" | JA: "ブルーム" ; UIOPBLMTOGDC EN: "Toggle bloom post FX." | JA: "ブルームを「オン」にすることで、光源から光が\n溢れ出るようなエフェクトが有効になります。"
        .choice = [](std::int32_t index) { return kOptionsBloomChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return g_applicationSettings.display.bloomMode; },
        .setInt = [](std::int32_t value) { g_applicationSettings.display.bloomMode = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIGAMMGAMM",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPBRGHTDSC");
            }, // @text UIGAMMGAMM EN: "Gamma" | JA: "ガンマ" ; UIOPBRGHTDSC EN: "Adjust gameplay brightness level." | JA: "鮮明度を調節します。"
        .choiceValue = [](std::int32_t index) { return kOptionsBrightnessRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.display.gamma; },
        .setFloat = [](float value) { g_applicationSettings.display.gamma = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIOPTSCONT",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTSCONDSC");
            }, // @text UIOPTSCONT EN: "Contrast" | JA: "コントラスト" ; UIOPTSCONDSC EN: "Adjust contrast level." | JA: "コントラストを調節します。"
        .choiceValue = [](std::int32_t index) { return kOptionsBrightnessRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.display.contrast; },
        .setFloat = [](float value) { g_applicationSettings.display.contrast = value; }},
    {.type = UIOptionType::Toggle,
        .name = "UIOPTSSD43",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPTASPDSC");
            }, // @text UIOPTSSD43 EN: "Aspect Ratio" | JA: "アスペクト比" ; UIOPTASPDSC EN: "Select the aspect ratio used during gameplay." | JA: "ゲームプレイ時の縦横比を選択します。"
        .choice = [](std::int32_t index) { return kOptionsAspectChoices[static_cast<std::size_t>(index)]; },
        .choiceValue = [](std::int32_t index) { return kOptionsChoiceValues[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getInt = [] { return g_applicationSettings.display.aspectRatio; },
        .setInt = [](std::int32_t value) { g_applicationSettings.display.aspectRatio = value; }},
    {.type = UIOptionType::Slider,
        .name = "UIOPTSCALE",
        .text =
            [](std::int32_t) {
                return Options_ReadString("UIOPUISCADSC");
            }, // @text UIOPTSCALE EN: "UI Display Area" | JA: "UIサイズ" ; UIOPUISCADSC EN: "Adjust the screen area that\nthe UI is contained within." | JA: "UIが含まれる\nスクリーンエリアを調節します。"
        .choiceValue = [](std::int32_t index) { return kOptionsUiScaleRange[static_cast<std::size_t>(index)]; },
        .localizeChoices = 1,
        .getFloat = [] { return g_applicationSettings.display.uiDisplayArea; },
        .setFloat = [](float value) { g_applicationSettings.display.uiDisplayArea = value; }},
}};

static_assert(kOptionsSettingsSize == 284);

std::array<std::byte, kOptionsSettingsSize> g_tmpUserKeyMapping{};

UIOption *g_optionsDescribedOption = nullptr;

UIOption *g_optionsInputAction = nullptr;

float g_optionsEditValue = 0.0f;

std::int32_t g_optionsEditCurrent = 0;

std::int32_t g_optionsKeyMapAction = 0;

std::int32_t g_optionsLastKeyIndex = -1;

std::array<char, 512> g_optionsKeyBindingText{};

std::uint32_t g_optionsSuccessSoundFrame = 0;

const std::array<FrontendOptionsEdit, 3> kFrontendOptionsEdits{{
    {.tab = 0, .grid = &g_optionsControlsGrid, .table = g_optionsControlsTable.data(), .count = 42, .entry = 0, .name = "controls-swap"},
    {.tab = 1, .grid = &g_optionsAudioGrid, .table = g_optionsAudioTable.data(), .count = 7, .entry = 4, .name = "audio-sfx"},
    {.tab = 2, .grid = &g_optionsGraphicsGrid, .table = g_optionsGraphicsTable.data(), .count = 11, .entry = 3, .name = "graphics-resolution-scale"},
}};

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

SleepSaveTrainingMenu g_sleepSaveTrainingMenu;

InformationBarLabels g_informationBarLabels{.message = {.iconAction = -1, .outlinePanel = -1, .iconIndex = -1}};

MovesScreenElement g_movesScreen{.buttonAnimSlot = -1, .highlightAnimSlot = -1};

extern std::array<std::int16_t, 5> g_searchPromptIconSlots;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

std::array<std::int32_t, 5> g_movesPromptIconSet{};


std::int32_t g_movesRemapAssignSoundFrame = 0;

std::int32_t g_trainingMovePickSoundFrame = 0;


constexpr char kUnknownMoveText[] = " ? ? ? ";


std::array<std::array<char, 512>, 3> g_informationBarLocalizedTexts{};



constexpr std::array<std::uint32_t, 4> kMovesLevelColours{0xc8699673U, 0xc85096afU, 0xc85082d2U, 0xc84b64d7U};

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

void ReadSubtitleGlyphsAndSubs(std::int32_t language);

GameTrainingElement g_gameTrainingElement;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



d3t::UILabelWidget *g_gameTrainingResultLabel = nullptr;

std::array<d3t::UIImageWidget *, 5> g_gameTrainingLabelWidgets{};

std::array<d3t::UIWindow *, 11> g_gameTrainingWindows{};

std::array<std::int32_t, 84> g_gameTrainingLabelImageIndices{};


void UI_ScriptNoOp() {
}

std::int64_t UI_ScriptNoOpReset() {
    return 0;
}

std::int64_t UI_SCNFNoOpHandler() {
    return 0;
}

std::int32_t g_gameTrainingLegacyUIViewSlot = -1;

std::int32_t g_gameTrainingLegacyUILoadState = 0;

std::uint32_t g_gameTrainingLegacyUIFileHandle = 0;

const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> *g_gameTrainingLegacyView = nullptr;

std::array<d3t::UILabelWidget *, 8> g_gameTrainingLegacyLabels{};

std::array<d3t::UILabelWidget *, 2> g_gameTrainingLegacyDescriptions{};

std::array<UIColourImageWidget *, 8> g_gameTrainingLegacyArrows{};

std::array<UIColourImageWidget *, 4> g_gameTrainingLegacyGlows{};

std::int32_t g_gameTrainingLegacySelection = -1;

const char *g_uiReloadFileMemory = nullptr;

// The GAMEHUD_SKIP element's skip-prompt view: its load state (0 not opened, 1 file loading, 2 textures pending, 3 ready), file handle
// and view slot.
std::int32_t g_gameHudSkipUILoadState = 0;

std::uint32_t g_gameHudSkipUIFileHandle = 0;

std::int32_t g_gameHudSkipUIViewSlot = -1;

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

std::array<SaveSlotInfo, 10> g_saveSlotInfo{};

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {


constexpr std::array<const char *, 12> kLoadMonthIds{"UILOADMN00", "UILOADMN01", "UILOADMN02", "UILOADMN03", "UILOADMN04", "UILOADMN05",
    "UILOADMN06", // @text UILOADMN00 EN: "Jan" | JA: "１月" ; UILOADMN01 EN: "Feb" | JA: "２月" ; UILOADMN02 EN: "Mar" | JA: "３月" ; UILOADMN03 EN: "Apr" | JA: "４月" ; UILOADMN04 EN: "May" | JA: "５月" ; UILOADMN05 EN: "Jun" | JA: "６月" ; UILOADMN06 EN: "Jul" | JA: "７月"
    "UILOADMN07", "UILOADMN08", "UILOADMN09", "UILOADMN10",
    "UILOADMN11"}; // @text UILOADMN07 EN: "Aug" | JA: "８月" ; UILOADMN08 EN: "Sep" | JA: "９月" ; UILOADMN09 EN: "Oct" | JA: "１０月" ; UILOADMN10 EN: "Nov" | JA: "１１月" ; UILOADMN11 EN: "Dec" | JA: "１２月"

constexpr std::array<const char *, 8> kLoadDayIds{"UILOADDAY0", "UILOADDAY1", "UILOADDAY2", "UILOADDAY3", "UILOADDAY4", "UILOADDAY5",
    "UILOADDAY6", // @text UILOADDAY0 EN: "Sun." | JA: "日" ; UILOADDAY1 EN: "Mon." | JA: "月" ; UILOADDAY2 EN: "Tue." | JA: "火" ; UILOADDAY3 EN: "Wed." | JA: "水" ; UILOADDAY4 EN: "Thu." | JA: "木" ; UILOADDAY5 EN: "Fri." | JA: "金" ; UILOADDAY6 EN: "Sat." | JA: "土"
    "UILOADDAY0"}; // @text EN: "Sun." | JA: "日"


constexpr AreaLocationName kAreaLocationNames[]{
    {"9999", "UILDLOCT99"}, {"ARAR", "UILDLOCT13"}, {"BETD", "UILDLOCT80"}, {"D000", "UILDLOCT00"}, {"DAZA", "UILDLOCT11"},
    {"DBHB",
        "UILDLOCT19"}, // @text UILDLOCT99 EN: "9999" | JA: "9999" ; UILDLOCT13 EN: "Asia Travel Company" | JA: "アジア旅行社" ; UILDLOCT80 EN: "Hazuki Residence" | JA: "芭月家" ; UILDLOCT00 EN: "Dobuita" | JA: "ドブ板" ; UILDLOCT11 EN: "Asia Travel Company" | JA: "アジア旅行社" ; UILDLOCT19 EN: "Heartbeats Bar" | JA: "ハートビーツ"
    {"DBYO", "UILDLOCT23"}, {"DCBN", "UILDLOCT25"}, {"DCHA", "UILDLOCT18"}, {"DGCT", "UILDLOCT20"}, {"DHQB", "UILDLOCT55"},
    {"DJAZ",
        "UILDLOCT22"}, // @text UILDLOCT23 EN: "Bar Yokosuka" | JA: "BAR ヨコスカ" ; UILDLOCT25 EN: "Tomato Convenience Store" | JA: "トマトマート" ; UILDLOCT18 EN: "Ajiichi Chinese Restaurant" | JA: "中華 味壱" ; UILDLOCT20 EN: "You Arcade" | JA: "ゲームセンター" ; UILDLOCT55 EN: "Heartbeats Bar" | JA: "ハートビーツ" ; UILDLOCT22 EN: "MJQ" | JA: "MJQ"
    {"DKPA", "UILDLOCT34"}, {"DKTY", "UILDLOCT01"}, {"DMAJ", "UILDLOCT28"}, {"DNOZ", "UILDLOCT81"}, {"DPIZ", "UILDLOCT29"},
    {"DRHT",
        "UILDLOCT26"}, // @text UILDLOCT34 EN: "Nana's Karaoke Bar" | JA: "カラオケパブ ナナ" ; UILDLOCT01 EN: "Antique Shop" | JA: "骨董屋" ; UILDLOCT28 EN: "Daisangen" | JA: "大三元" ; UILDLOCT81 EN: "Sakuragaoka" | JA: "桜ヶ丘" ; UILDLOCT29 EN: "Bob's Pizzeria" | JA: "ボブズ ピザ" ; UILDLOCT26 EN: "Liu Barber & Hair Salon" | JA: "バーバー劉"
    {"DRME", "UILDLOCT27"}, {"DRSA", "UILDLOCT24"}, {"DSBA", "UILDLOCT21"}, {"DSKI", "UILDLOCT17"}, {"DSLI", "UILDLOCT33"},
    {"DSLT",
        "UILDLOCT56"}, // @text UILDLOCT27 EN: "Manpukuken Ramen" | JA: "満福軒" ; UILDLOCT24 EN: "Russiya China Shop" | JA: "陶器屋ロシヤ" ; UILDLOCT21 EN: "Yamaji Soba Noodles" | JA: "そば屋 やま路" ; UILDLOCT17 EN: "Global Travel Agency" | JA: "世界旅行社" ; UILDLOCT33 EN: "Linda" | JA: "リンダ" ; UILDLOCT56 EN: "Slot House" | JA: "スロットハウス"
    {"DSUS", "UILDLOCT30"}, {"DTAT", "UILDLOCT09"}, {"DTKY", "UILDLOCT15"}, {"DURN", "UILDLOCT32"}, {"DYKZ", "UILDLOCT31"},
    {"GMCT",
        "UILDLOCT71"}, // @text UILDLOCT30 EN: "Takara Sushi Restaurant" | JA: "宝ずし" ; UILDLOCT09 EN: "Tattoo Parlor" | JA: "タトゥショップ" ; UILDLOCT15 EN: "Maeda Barbershop" | JA: "理容マエダ" ; UILDLOCT32 EN: "Lapis Fortune Tellers Shop" | JA: "ラピス" ; UILDLOCT31 EN: "Nagai Industries" | JA: "永井興業" ; UILDLOCT71 EN: "You Arcade" | JA: "ゲームセンター"
    {"JABE", "UILDLOCT35"}, {"JD00", "UILDLOCT08"}, {"JHD0", "UILDLOCT03"}, {"JOMO", "UILDLOCT04"}, {"JU00", "UILDLOCT07"},
    {"KOTO",
        "UILDLOCT02"}, // @text UILDLOCT35 EN: "Abe Store" | JA: "阿部商店" ; UILDLOCT08 EN: "Sakuragaoka" | JA: "桜ヶ丘" ; UILDLOCT03 EN: "Hazuki Residence" | JA: "芭月家" ; UILDLOCT04 EN: "Hazuki Residence\n- Main House -" | JA: "芭月家母屋" ; UILDLOCT07 EN: "Yamanose" | JA: "山の瀬" ; UILDLOCT02 EN: "Antique Shop" | JA: "骨董屋"
    {"MA00", "UILDLOCT76"}, {"MFSY", "UILDLOCT12"}, {"MKS0", "UILDLOCT74"}, {"MKS1", "UILDLOCT60"}, {"MKS2", "UILDLOCT61"},
    {"MKS3",
        "UILDLOCT62"}, // @text UILDLOCT76 EN: "New Yokosuka Harbor" | JA: "新横須賀港" ; UILDLOCT12 EN: "New Yokosuka Harbor" | JA: "新横須賀港" ; UILDLOCT74 EN: "Warehouse No. 10" | JA: "第１０倉庫" ; UILDLOCT60 EN: "Warehouse No. 1" | JA: "第１倉庫" ; UILDLOCT61 EN: "Warehouse No. 2" | JA: "第２倉庫" ; UILDLOCT62 EN: "Warehouse No. 3" | JA: "第３倉庫"
    {"MKS4", "UILDLOCT63"}, {"MKS5", "UILDLOCT64"}, {"MKS6", "UILDLOCT65"}, {"MKS7", "UILDLOCT66"}, {"MKS8", "UILDLOCT67"},
    {"MKS9",
        "UILDLOCT68"}, // @text UILDLOCT63 EN: "Warehouse No. 4" | JA: "第４倉庫" ; UILDLOCT64 EN: "Warehouse No. 5" | JA: "第５倉庫" ; UILDLOCT65 EN: "Warehouse No. 6" | JA: "第６倉庫" ; UILDLOCT66 EN: "Warehouse No. 7" | JA: "第７倉庫" ; UILDLOCT67 EN: "Warehouse No. 8" | JA: "第８倉庫" ; UILDLOCT68 EN: "Warehouse No. 9" | JA: "第９倉庫"
    {"MKSG", "UILDLOCT16"}, {"MKYU", "UILDLOCT75"}, {"MN01", "UILDLOCT36"}, {"MN02", "UILDLOCT37"}, {"MN03", "UILDLOCT38"},
    {"MN04",
        "UILDLOCT39"}, // @text UILDLOCT16 EN: "Old Warehouse District" | JA: "旧倉庫街" ; UILDLOCT75 EN: "Harbor Lounge" | JA: "港湾休憩所" ; UILDLOCT36 EN: "Warehouse No. 1" | JA: "第１倉庫" ; UILDLOCT37 EN: "Warehouse No. 2" | JA: "第２倉庫" ; UILDLOCT38 EN: "Warehouse No. 3" | JA: "第３倉庫" ; UILDLOCT39 EN: "Warehouse No. 4" | JA: "第４倉庫"
    {"MN05", "UILDLOCT40"}, {"MN06", "UILDLOCT41"}, {"MN07", "UILDLOCT42"}, {"MN08", "UILDLOCT43"}, {"MN09", "UILDLOCT44"},
    {"MN10",
        "UILDLOCT45"}, // @text UILDLOCT40 EN: "Warehouse No. 5" | JA: "第５倉庫" ; UILDLOCT41 EN: "Warehouse No. 6" | JA: "第６倉庫" ; UILDLOCT42 EN: "Warehouse No. 7" | JA: "第７倉庫" ; UILDLOCT43 EN: "Warehouse No. 8" | JA: "第８倉庫" ; UILDLOCT44 EN: "Warehouse No. 9" | JA: "第９倉庫" ; UILDLOCT45 EN: "Warehouse No. 10" | JA: "第１０倉庫"
    {"MN11", "UILDLOCT46"}, {"MN12", "UILDLOCT47"}, {"MN13", "UILDLOCT48"}, {"MN14", "UILDLOCT49"}, {"MN15", "UILDLOCT50"},
    {"MN16",
        "UILDLOCT51"}, // @text UILDLOCT46 EN: "Warehouse No. 11" | JA: "第１１倉庫" ; UILDLOCT47 EN: "Warehouse No. 12" | JA: "第１２倉庫" ; UILDLOCT48 EN: "Warehouse No. 13" | JA: "第１３倉庫" ; UILDLOCT49 EN: "Warehouse No. 14" | JA: "第１４倉庫" ; UILDLOCT50 EN: "Warehouse No. 15" | JA: "第１５倉庫" ; UILDLOCT51 EN: "Warehouse No. 16" | JA: "第１６倉庫"
    {"MN17", "UILDLOCT52"}, {"MN18", "UILDLOCT72"}, {"MO01", "UILDLOCT59"}, {"MO99", "UILDLOCT73"}, {"MS08", "UILDLOCT14"},
    {"MS8A",
        "UILDLOCT57"}, // @text UILDLOCT52 EN: "Warehouse No. 17" | JA: "第１７倉庫" ; UILDLOCT72 EN: "Warehouse No. 18" | JA: "第１８倉庫" ; UILDLOCT59 EN: "Alpha Trading Office" | JA: "事務所" ; UILDLOCT73 EN: "Warehouse No. 8" | JA: "第８倉庫" ; UILDLOCT14 EN: "Warehouse No. 8" | JA: "第８倉庫" ; UILDLOCT57 EN: "Warehouse No. 8" | JA: "第８倉庫"
    {"MS8S", "UILDLOCT58"}, {"MSBS", "UILDLOCT82"}, {"MSYO", "UILDLOCT79"}, {"OMIN", "UILDLOCT05"}, {"OP00", "UILDLOCT53"},
    {"TATQ",
        "UILDLOCT10"}, // @text UILDLOCT58 EN: "Warehouse No. 8" | JA: "第８倉庫" ; UILDLOCT82 EN: "Hazuki Residence" | JA: "芭月家" ; UILDLOCT79 EN: "Harbor Cafeteria" | JA: "港湾食堂" ; UILDLOCT05 EN: "Hazuki Residence\n- Main House -" | JA: "芭月家母屋" ; UILDLOCT53 EN: "4 Days Later..." | JA: "4 Days Later..." ; UILDLOCT10 EN: "Tattoo Parlor" | JA: "タトゥショップ"
    {"TERY", "UILDLOCT78"}, {"TOKI", "UILDLOCT69"}, {"YDB1", "UILDLOCT06"},
    {"YDMA",
        "UILDLOCT83"} // @text UILDLOCT78 EN: "New Yokosuka Harbor" | JA: "新横須賀港" ; UILDLOCT69 EN: "Russiya China Shop" | JA: "陶器屋ロシヤ" ; UILDLOCT06 EN: "Hazuki Residence\n- Basement -" | JA: "芭月家地下室" ; UILDLOCT83 EN: "Hazuki Residence" | JA: "芭月家"
};

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {



std::int32_t g_iIsInGame = 0;

std::int32_t g_isInGame = 0;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

std::uint64_t g_loadSaveRejectFrame = 0;

std::uint64_t g_loadSaveSaveSoundFrame = 0;

std::uint64_t g_loadSaveBlockedSoundFrame = 0;





UIGridTemplate g_loadSaveGridTemplate{};

UIGrid g_loadSaveGrid{};

std::array<std::array<char, 256>, 7> g_loadSaveLabelTexts{};

std::array<d3t::UIWindow *, 9> g_loadSaveWindows{};

std::array<d3t::UILayoutItem *, 12> g_loadSaveLayoutItems{};

std::array<d3t::UILabelWidget *, 7> g_loadSaveLabels{};

std::array<UIButtonWidget *, 2> g_loadSaveButtons{};

std::array<UIColourImageWidget *, 9> g_loadSaveColourImages{};

std::array<d3t::UIViewImage *, 8> g_loadSaveViewImages{};

std::array<d3t::UIViewImage *, 6> g_loadSaveAreaImages{};

std::array<d3t::UIViewImage *, 25> g_loadSaveMapImages{};

d3t::UIImageWidget *g_loadSaveMapImage = nullptr;

d3t::UIImageWidget *g_loadSaveAreaImage = nullptr;

d3t::UIWindow *g_loadSaveGridHolder = nullptr;

bool g_loadSaveConfirmLocked = false;

void LoadSaveGrid_DrawSlot(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element,
    void *elements, std::int32_t index);

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

LoadScreenPresentation g_loadScreenPresentation;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {


constexpr std::array<const char *, 4> kLoadingUITokens{"UILDNYDAY", "UILDCHRISTMEVE", "UILDCHRISTMDAY",
    "UILDNYEVE"}; // @text UILDNYDAY EN: "New Year's Day" | JA: "元日" ; UILDNYEVE EN: "New Year's Eve" | JA: "大晦日"


constexpr std::array<const char *, 4> kLoadingDots{"     ", ".    ", ". .  ", ". . ."};


constexpr char kLoadScreenSubAreaMark[] = "\xa1\xf5";


constexpr std::array<MapLocationToken, 86> kMapLocationTokens{{
    {"D000", "UILDLOCT00", "Dobuita"}, {"DKTY", "UILDLOCT01", "Antique Shop"},
    {"KOTO", "UILDLOCT02",
        "Antique Shop"}, // @text UILDLOCT00 EN: "Dobuita" | JA: "ドブ板" ; UILDLOCT01 EN: "Antique Shop" | JA: "骨董屋" ; UILDLOCT02 EN: "Antique Shop" | JA: "骨董屋"
    {"JHD0", "UILDLOCT03", "Hazuki Residence"},
    {"JOMO", "UILDLOCT04",
        "Hazuki Residence - Main House"}, // @text UILDLOCT03 EN: "Hazuki Residence" | JA: "芭月家" ; UILDLOCT04 EN: "Hazuki Residence\n- Main House -" | JA: "芭月家母屋"
    {"OMIN", "UILDLOCT05", "Hazuki Residence - Main House"}, {"YDB1", "UILDLOCT06", "Hazuki Residence - Basement"},
    {"JU00", "UILDLOCT07",
        "Yamanose"}, // @text UILDLOCT05 EN: "Hazuki Residence\n- Main House -" | JA: "芭月家母屋" ; UILDLOCT06 EN: "Hazuki Residence\n- Basement -" | JA: "芭月家地下室" ; UILDLOCT07 EN: "Yamanose" | JA: "山の瀬"
    {"JD00", "UILDLOCT08", "Sakuragaoka"}, {"DTAT", "UILDLOCT09", "Tattoo Parlor"},
    {"TATQ", "UILDLOCT10",
        "Tattoo Parlor"}, // @text UILDLOCT08 EN: "Sakuragaoka" | JA: "桜ヶ丘" ; UILDLOCT09 EN: "Tattoo Parlor" | JA: "タトゥショップ" ; UILDLOCT10 EN: "Tattoo Parlor" | JA: "タトゥショップ"
    {"DAZA", "UILDLOCT11", "Asia Travel Company"}, {"MFSY", "UILDLOCT12", "New Yokosuka Harbor"},
    {"ARAR", "UILDLOCT13",
        "Asia Travel Company"}, // @text UILDLOCT11 EN: "Asia Travel Company" | JA: "アジア旅行社" ; UILDLOCT12 EN: "New Yokosuka Harbor" | JA: "新横須賀港" ; UILDLOCT13 EN: "Asia Travel Company" | JA: "アジア旅行社"
    {"MS08", "UILDLOCT14", "Warehouse No. 8"}, {"DTKY", "UILDLOCT15", "Maeda Barbershop"},
    {"MKSG", "UILDLOCT16",
        "Old Warehouse District"}, // @text UILDLOCT14 EN: "Warehouse No. 8" | JA: "第８倉庫" ; UILDLOCT15 EN: "Maeda Barbershop" | JA: "理容マエダ" ; UILDLOCT16 EN: "Old Warehouse District" | JA: "旧倉庫街"
    {"DSKI", "UILDLOCT17", "Global Travel Agency"}, {"DCHA", "UILDLOCT18", "Ajiichi Chinese Restaurant"},
    {"DBHB", "UILDLOCT19",
        "Heartbeats Bar"}, // @text UILDLOCT17 EN: "Global Travel Agency" | JA: "世界旅行社" ; UILDLOCT18 EN: "Ajiichi Chinese Restaurant" | JA: "中華 味壱" ; UILDLOCT19 EN: "Heartbeats Bar" | JA: "ハートビーツ"
    {"DGCT", "UILDLOCT20", "You Arcade"}, {"DSBA", "UILDLOCT21", "Yamaji Soba Noodles"},
    {"DJAZ", "UILDLOCT22",
        "MJQ"}, // @text UILDLOCT20 EN: "You Arcade" | JA: "ゲームセンター" ; UILDLOCT21 EN: "Yamaji Soba Noodles" | JA: "そば屋 やま路" ; UILDLOCT22 EN: "MJQ" | JA: "MJQ"
    {"DBYO", "UILDLOCT23", "Bar Yokosuka"}, {"DRSA", "UILDLOCT24", "Russiya China Shop"},
    {"DCBN", "UILDLOCT25",
        "Tomato Convenience Store"}, // @text UILDLOCT23 EN: "Bar Yokosuka" | JA: "BAR ヨコスカ" ; UILDLOCT24 EN: "Russiya China Shop" | JA: "陶器屋ロシヤ" ; UILDLOCT25 EN: "Tomato Convenience Store" | JA: "トマトマート"
    {"DRHT", "UILDLOCT26", "Liu Barber & Hair Salon"}, {"DRME", "UILDLOCT27", "Manpukuken Ramen"},
    {"DMAJ", "UILDLOCT28",
        "Daisangen"}, // @text UILDLOCT26 EN: "Liu Barber & Hair Salon" | JA: "バーバー劉" ; UILDLOCT27 EN: "Manpukuken Ramen" | JA: "満福軒" ; UILDLOCT28 EN: "Daisangen" | JA: "大三元"
    {"DPIZ", "UILDLOCT29", "Bob's Pizzeria"}, {"DSUS", "UILDLOCT30", "Takara Sushi Restaurant"},
    {"DYKZ", "UILDLOCT31",
        "Nagai Industries"}, // @text UILDLOCT29 EN: "Bob's Pizzeria" | JA: "ボブズ ピザ" ; UILDLOCT30 EN: "Takara Sushi Restaurant" | JA: "宝ずし" ; UILDLOCT31 EN: "Nagai Industries" | JA: "永井興業"
    {"DURN", "UILDLOCT32", "Lapis Fortune Tellers Shop"}, {"DSLI", "UILDLOCT33", "Linda"},
    {"DKPA", "UILDLOCT34",
        "Nana's Karaoke Bar"}, // @text UILDLOCT32 EN: "Lapis Fortune Tellers Shop" | JA: "ラピス" ; UILDLOCT33 EN: "Linda" | JA: "リンダ" ; UILDLOCT34 EN: "Nana's Karaoke Bar" | JA: "カラオケパブ ナナ"
    {"JABE", "UILDLOCT35", "Abe Store"}, {"MN01", "UILDLOCT36", "Warehouse No. 1"},
    {"MN02", "UILDLOCT37",
        "Warehouse No. 2"}, // @text UILDLOCT35 EN: "Abe Store" | JA: "阿部商店" ; UILDLOCT36 EN: "Warehouse No. 1" | JA: "第１倉庫" ; UILDLOCT37 EN: "Warehouse No. 2" | JA: "第２倉庫"
    {"MN03", "UILDLOCT38", "Warehouse No. 3"}, {"MN04", "UILDLOCT39", "Warehouse No. 4"},
    {"MN05", "UILDLOCT40",
        "Warehouse No. 5"}, // @text UILDLOCT38 EN: "Warehouse No. 3" | JA: "第３倉庫" ; UILDLOCT39 EN: "Warehouse No. 4" | JA: "第４倉庫" ; UILDLOCT40 EN: "Warehouse No. 5" | JA: "第５倉庫"
    {"MN06", "UILDLOCT41", "Warehouse No. 6"}, {"MN07", "UILDLOCT42", "Warehouse No. 7"},
    {"MN08", "UILDLOCT43",
        "Warehouse No. 8"}, // @text UILDLOCT41 EN: "Warehouse No. 6" | JA: "第６倉庫" ; UILDLOCT42 EN: "Warehouse No. 7" | JA: "第７倉庫" ; UILDLOCT43 EN: "Warehouse No. 8" | JA: "第８倉庫"
    {"MN09", "UILDLOCT44", "Warehouse No. 9"}, {"MN10", "UILDLOCT45", "Warehouse No. 10"},
    {"MN11", "UILDLOCT46",
        "Warehouse No. 11"}, // @text UILDLOCT44 EN: "Warehouse No. 9" | JA: "第９倉庫" ; UILDLOCT45 EN: "Warehouse No. 10" | JA: "第１０倉庫" ; UILDLOCT46 EN: "Warehouse No. 11" | JA: "第１１倉庫"
    {"MN12", "UILDLOCT47", "Warehouse No. 12"}, {"MN13", "UILDLOCT48", "Warehouse No. 13"},
    {"MN14", "UILDLOCT49",
        "Warehouse No. 14"}, // @text UILDLOCT47 EN: "Warehouse No. 12" | JA: "第１２倉庫" ; UILDLOCT48 EN: "Warehouse No. 13" | JA: "第１３倉庫" ; UILDLOCT49 EN: "Warehouse No. 14" | JA: "第１４倉庫"
    {"MN15", "UILDLOCT50", "Warehouse No. 15"}, {"MN16", "UILDLOCT51", "Warehouse No. 16"},
    {"MN17", "UILDLOCT52",
        "Warehouse No. 17"}, // @text UILDLOCT50 EN: "Warehouse No. 15" | JA: "第１５倉庫" ; UILDLOCT51 EN: "Warehouse No. 16" | JA: "第１６倉庫" ; UILDLOCT52 EN: "Warehouse No. 17" | JA: "第１７倉庫"
    {"OP00", "UILDLOCT53", "4 Days Later..."}, {"JOMO", "UILDLOCT54", "Home..."},
    {"DHQB", "UILDLOCT55",
        "Heartbeats Bar"}, // @text UILDLOCT53 EN: "4 Days Later..." | JA: "4 Days Later..." ; UILDLOCT54 EN: "Home..." | JA: "Home..." ; UILDLOCT55 EN: "Heartbeats Bar" | JA: "ハートビーツ"
    {"DSLT", "UILDLOCT56", "Slot House"}, {"MS8A", "UILDLOCT57", "Warehouse No. 8"},
    {"MS8S", "UILDLOCT58",
        "Warehouse No. 8"}, // @text UILDLOCT56 EN: "Slot House" | JA: "スロットハウス" ; UILDLOCT57 EN: "Warehouse No. 8" | JA: "第８倉庫" ; UILDLOCT58 EN: "Warehouse No. 8" | JA: "第８倉庫"
    {"MO01", "UILDLOCT59", "Alpha Trading Office"}, {"MKS1", "UILDLOCT60", "Warehouse No. 1"},
    {"MKS2", "UILDLOCT61",
        "Warehouse No. 2"}, // @text UILDLOCT59 EN: "Alpha Trading Office" | JA: "事務所" ; UILDLOCT60 EN: "Warehouse No. 1" | JA: "第１倉庫" ; UILDLOCT61 EN: "Warehouse No. 2" | JA: "第２倉庫"
    {"MKS3", "UILDLOCT62", "Warehouse No. 3"}, {"MKS4", "UILDLOCT63", "Warehouse No. 4"},
    {"MKS5", "UILDLOCT64",
        "Warehouse No. 5"}, // @text UILDLOCT62 EN: "Warehouse No. 3" | JA: "第３倉庫" ; UILDLOCT63 EN: "Warehouse No. 4" | JA: "第４倉庫" ; UILDLOCT64 EN: "Warehouse No. 5" | JA: "第５倉庫"
    {"MKS6", "UILDLOCT65", "Warehouse No. 6"}, {"MKS7", "UILDLOCT66", "Warehouse No. 7"},
    {"MKS8", "UILDLOCT67",
        "Warehouse No. 8"}, // @text UILDLOCT65 EN: "Warehouse No. 6" | JA: "第６倉庫" ; UILDLOCT66 EN: "Warehouse No. 7" | JA: "第７倉庫" ; UILDLOCT67 EN: "Warehouse No. 8" | JA: "第８倉庫"
    {"MKS9", "UILDLOCT68", "Warehouse No. 9"}, {"TOKI", "UILDLOCT69", "Russiya China Shop"},
    {"JD00", "UILDLOCT70",
        "Construction Site"}, // @text UILDLOCT68 EN: "Warehouse No. 9" | JA: "第９倉庫" ; UILDLOCT69 EN: "Russiya China Shop" | JA: "陶器屋ロシヤ" ; UILDLOCT70 EN: "Construction Site" | JA: "工事現場"
    {"GMCT", "UILDLOCT71", "You Arcade"}, {"MN18", "UILDLOCT72", "Warehouse No. 18"},
    {"MO99", "UILDLOCT73",
        "Warehouse No. 8"}, // @text UILDLOCT71 EN: "You Arcade" | JA: "ゲームセンター" ; UILDLOCT72 EN: "Warehouse No. 18" | JA: "第１８倉庫" ; UILDLOCT73 EN: "Warehouse No. 8" | JA: "第８倉庫"
    {"MKS0", "UILDLOCT74", "Warehouse No. 10"}, {"MKYU", "UILDLOCT75", "Harbor Lounge"},
    {"MA00", "UILDLOCT76",
        "New Yokosuka Harbor"}, // @text UILDLOCT74 EN: "Warehouse No. 10" | JA: "第１０倉庫" ; UILDLOCT75 EN: "Harbor Lounge" | JA: "港湾休憩所" ; UILDLOCT76 EN: "New Yokosuka Harbor" | JA: "新横須賀港"
    {"JOMO", "UILDLOCT77", "Ryo's Room"}, {"TERY", "UILDLOCT78", "New Yokosuka Harbor"},
    {"MSYO", "UILDLOCT79",
        "Harbor Cafeteria"}, // @text UILDLOCT77 EN: "Ryo's Room" | JA: "涼の部屋" ; UILDLOCT78 EN: "New Yokosuka Harbor" | JA: "新横須賀港" ; UILDLOCT79 EN: "Harbor Cafeteria" | JA: "港湾食堂"
    {"BETD", "UILDLOCT80", "Hazuki Residence"}, {"DNOZ", "UILDLOCT81", "Sakuragaoka"},
    {"MSBS", "UILDLOCT82",
        "Hazuki Residence"}, // @text UILDLOCT80 EN: "Hazuki Residence" | JA: "芭月家" ; UILDLOCT81 EN: "Sakuragaoka" | JA: "桜ヶ丘" ; UILDLOCT82 EN: "Hazuki Residence" | JA: "芭月家"
    {"YDMA", "UILDLOCT83", "Hazuki Residence"}, {"OP00", "UILDLOCT84", "Yokosuka"},
    {"9999", nullptr, "9999"}, // @text UILDLOCT83 EN: "Hazuki Residence" | JA: "芭月家" ; UILDLOCT84 EN: "Yokosuka" | JA: "横須賀"
}};

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

const char *UI_Elements_Init() {
    EWZO_SetUiHandler(HandleEwzoUiEvent);
    g_pfnDrawNuklear = DrawNuklear;
    g_getAspectRatioModeFunction = d3t_GetAspectRatioMode;
    for (std::size_t index = 0; index < g_uiElements.size(); ++index) {
        g_uiElements[index].name = "Unknown";
        g_uiElements[index].index = static_cast<std::int32_t>(index);
        g_uiElements[index].config.uiHandleIndex = -1;
    }
    // The Load Screen element is frame_pipeline's LoadScreen, which the render dispatch shows and closes; element 0x4E's record takes the
    // same callbacks.
    for (UIBaseRuntime *const loadScreen : {&LoadScreen, &g_uiElements[0x4e]}) {
        loadScreen->name = "Load Screen";
        loadScreen->index = 0x4e;
        loadScreen->parseDefinitionFromFile = [] { return LoadPresentationDefinition(0x4e, "/ui/loadscreen/LoadScreen.ui"); };
        loadScreen->render = LoadScreen_ResetUIStates;
        loadScreen->destroy = LoadScreen_Destroy;
        loadScreen->unknown80 = [] { return static_cast<std::int64_t>(CDTT_IsLoadingUIActive()); };
        loadScreen->tick = LoadScreen_UpdateNowLoading;
        loadScreen->postInit = LoadScreen_UpdateNowLoading;
        loadScreen->isSelected = LoadScreen_IsSelected;
        loadScreen->reset = LoadScreen_Reset;
        loadScreen->field_ac = 1;
    }


    UIBaseRuntime &gameHudSkip = g_uiElements[kUiGameHudSkip];
    gameHudSkip.name = "GAMEHUD_SKIP";
    gameHudSkip.parseDefinitionFromFile = LoadGameHudUI;
    gameHudSkip.unknown28 = UI_SCNFNoOpHandler;
    gameHudSkip.postInit = GameHudSkip_PostInit;
    gameHudSkip.initialize = UI_ScriptNoOp;
    gameHudSkip.unknown60 = UI_NullsubReturn1;
    gameHudSkip.reloadUI = GameHudSkip_ReloadUI;
    gameHudSkip.loadState = GameHudSkip_LoadState;
    UIBaseRuntime &notebookPages = g_uiElements[kUiNotebookPages];
    notebookPages.name = "NOTEBOOK_PAGES";
    notebookPages.parseDefinitionFromFile = NotebookPages_ParseDefinition;
    notebookPages.unknown28 = UI_SCNFNoOpHandler;
    notebookPages.postInit = PrepareNotebookBuffers;
    notebookPages.initialize = NotebookPages_Initialize;
    notebookPages.unknown60 = InitNotebookTextures;
    notebookPages.loadState = [] { return static_cast<std::int64_t>(NotebookPages_ParseDefinition()); };
    g_uiElements[kUiScreenFade].isPressed = UI_NullsubReturn1;
    g_uiElements[kUiScreenFade].tick = ScreenFade;
    g_uiElements[kUiScreenFade].name = "Game Screen Fade";
    g_currentFilmColour = 0;
    g_uiElements[4].name = "Main Menu";
    g_uiElements[4].parseDefinitionFromFile = LoadMainMenu;
    g_uiElements[4].isSelected = MainMenuSelected;
    g_uiElements[4].isPressed = MainMenuInput;
    g_uiElements[4].tick = MainMenuTick;
    g_uiElements[4].render = MainMenuRender;
    g_uiElements[4].destroy = MainMenuDestroy;
    g_uiElements[4].reset = MainMenuReset;
    g_uiElements[kMainMenuWheelElement].name = "Main Menu Wheel";
    g_uiElements[kMainMenuWheelElement].parseDefinitionFromFile = LoadMainMenuWheel;
    g_uiElements[kMainMenuWheelElement].isSelected = MainMenuWheelSelected;
    g_uiElements[kMainMenuWheelElement].isPressed = MainMenuWheelInput;
    g_uiElements[kMainMenuWheelElement].tick = MainMenuWheelTick;
    g_uiElements[kMainMenuWheelElement].render = MainMenuWheelRender;
    g_uiElements[kMainMenuWheelElement].destroy = MainMenuWheelDestroy;
    g_uiElements[kMainMenuWheelElement].getSearchType = MainMenuWheel_GetPromptIconSet;
    g_mainMenuWheelRefresh = 1;
    const auto bindPresentation = [](std::int32_t index, const char *name, std::uint32_t (*loader)()) {
        UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(index)];
        element.name = name;
        element.parseDefinitionFromFile = loader;
    };
    bindPresentation(kUiGameMenu, "Game Menu", [] { return LoadPresentationDefinition(kUiGameMenu, "/ui/gamemenu_New/GameMenu_New.ui"); });
    g_uiElements[kUiGameMenu].isSelected = GameMenu_IsSelected;
    g_uiElements[kUiGameMenu].reset = GameMenu_Reset;
    g_uiElements[kUiGameMenu].isPressed = GameMenu_IsPressed;
    g_uiElements[kUiGameMenu].tick = GameMenu_Tick;
    g_uiElements[kUiGameMenu].render = GameMenu_Render;
    g_uiElements[kUiGameMenu].destroy = GameMenu_Destroy;
    g_uiElements[kUiGameMenu].elementDraw = GameMenu_StartDraw;
    bindPresentation(kUiGameMenuBackground, "Game Menu Background",
        [] { return LoadPresentationDefinition(kUiGameMenuBackground, "/ui/gamemenu_New/GameMenu_Background.ui"); });

    g_uiElements[kUiGameMenuBackground].isPressed = UI_NullsubReturn1;
    g_uiElements[kUiGameMenuBackground].tick = GameBackground_Tick;
    g_uiElements[kUiGameMenuBackground].isSelected = UI_NullsubReturn1;
    g_uiElements[kUiGameMenuBackground].reset = UI_ScriptNoOpReset;
    bindPresentation(kUiSharedItems, "Shared Items", [] { return LoadPresentationDefinition(kUiSharedItems, "/ui/gamemenu_new/SharedItems.ui"); });
    g_uiElements[kUiSharedItems].isSelected = GetIconSet;
    g_uiElements[kUiSharedItems].isPressed = SharedItems_IsPressed;
    bindPresentation(kUiSubMenuItems, "Items", [] { return LoadPresentationDefinition(kUiSubMenuItems, "/ui/gamemenu_New/SubMenu_Items.ui"); });
    g_uiElements[kUiSubMenuItems].isPressed = ItemsMenu_IsPressed;
    g_uiElements[kUiSubMenuItems].tick = ItemsMenu_Tick;
    g_uiElements[kUiSubMenuItems].isSelected = ItemsMenu_IsSelected;
    g_uiElements[kUiSubMenuItems].render = ItemsMenu_Render;
    g_uiElements[kUiSubMenuItems].destroy = ItemsMenu_Destroy;
    bindPresentation(
        kUiSubMenuCollection, "Collection", [] { return LoadPresentationDefinition(kUiSubMenuCollection, "/ui/gamemenu_New/SubMenu_Collection.ui"); });
    g_uiElements[kUiSubMenuCollection].isSelected = CollectionMenu_IsSelected;
    g_uiElements[kUiSubMenuCollection].reset = CollectionMenu_Reset;
    g_uiElements[kUiSubMenuCollection].isPressed = CollectionMenu_IsPressed;
    g_uiElements[kUiSubMenuCollection].tick = CollectionMenu_Tick;
    g_uiElements[kUiSubMenuCollection].render = CollectionMenu_Render;
    g_uiElements[kUiSubMenuCollection].destroy = CollectionMenu_Destroy;
    g_uiElements[kUiSubMenuCollection].elementDraw = CollectionMenu_ElementDraw;
    g_uiElements[kUiSubMenuCollection].getSearchType = CollectionMenu_GetPromptIconSet;
    bindPresentation(kUiSubMenuGacha, "Gacha", [] { return LoadPresentationDefinition(kUiSubMenuGacha, "/ui/gamemenu_New/SubMenu_Collection_Gacha.ui"); });

    g_uiElements[kUiSubMenuGacha].isSelected = GachaMenu_IsSelected;
    g_uiElements[kUiSubMenuGacha].reset = GachaMenu_Reset;
    g_uiElements[kUiSubMenuGacha].isPressed = GachaMenu_IsPressed;
    g_uiElements[kUiSubMenuGacha].tick = GachaMenu_Tick;
    g_uiElements[kUiSubMenuGacha].render = GachaMenu_Render;
    g_uiElements[kUiSubMenuGacha].destroy = UI_NullsubReturn1;
    bindPresentation(
        kUiSubMenuCassettes, "Cassettes", [] { return LoadPresentationDefinition(kUiSubMenuCassettes, "/ui/gamemenu_New/SubMenu_Collection_Cassettes.ui"); });
    g_uiElements[kUiSubMenuCassettes].isSelected = CassettesMenu_IsSelected;
    g_uiElements[kUiSubMenuCassettes].reset = CassettesMenu_Reset;
    g_uiElements[kUiSubMenuCassettes].isPressed = CassettesMenu_IsPressed;
    g_uiElements[kUiSubMenuCassettes].tick = CassettesMenu_Tick;
    g_uiElements[kUiSubMenuCassettes].render = CassettesMenu_Render;
    g_uiElements[kUiSubMenuCassettes].destroy = CassettesMenu_Destroy;
    bindPresentation(
        kUiSubMenuScrolls, "Scrolls", [] { return LoadPresentationDefinition(kUiSubMenuScrolls, "/ui/gamemenu_New/SubMenu_Collection_Scrolls.ui"); });

    g_uiElements[kUiSubMenuScrolls].isSelected = ScrollsMenu_IsSelected;
    g_uiElements[kUiSubMenuScrolls].reset = ScrollsMenu_Reset;
    g_uiElements[kUiSubMenuScrolls].isPressed = ScrollsMenu_IsPressed;
    g_uiElements[kUiSubMenuScrolls].tick = ScrollsMenu_Tick;
    g_uiElements[kUiSubMenuScrolls].render = ScrollsMenu_Render;
    g_uiElements[kUiSubMenuScrolls].destroy = UI_NullsubReturn1;
    UIBaseRuntime &moves = g_uiElements[kUiSubMenuMoves];
    moves.name = "SubMenu: Moves";
    moves.parseDefinitionFromFile = MovesScreen_Load;
    moves.isPressed = MovesScreen_IsPressed;
    moves.tick = MovesScreen_Update;
    moves.isSelected = MovesScreen_IsSelected;
    moves.reset = MovesScreen_Reset;
    moves.render = MovesScreen_Render;
    moves.destroy = MovesScreen_Destroy;
    moves.getSearchType = MovesScreen_GetPromptIconSet;
    moves.getContentPanel = MovesScreen_GetContentPanel;
    bindPresentation(kUiSubMenuOptions, "Options", [] { return LoadPresentationDefinition(kUiSubMenuOptions, "/ui/gamemenu_New/SubMenu_Options.ui"); });
    g_uiElements[kUiSubMenuOptions].isSelected = OptionsMenu_IsSelected;
    g_uiElements[kUiSubMenuOptions].reset = OptionsMenu_Reset;
    g_uiElements[kUiSubMenuOptions].tick = OptionsMenu_Tick;
    g_uiElements[kUiSubMenuOptions].isPressed = OptionsMenu_IsPressed;
    g_uiElements[kUiSubMenuOptions].render = OptionsMenu_Render;
    g_uiElements[kUiSubMenuOptions].destroy = OptionsMenu_Destroy;
    g_uiElements[kUiSubMenuOptions].getContentPanel = OptionsMenu_GetContentPanel;
    g_uiElements[kUiSubMenuOptions].getSearchType = OptionsMenu_GetPromptIconSet;
    bindPresentation(kUiSubMenuOptionsAudio, "Options Audio",
        [] { return LoadPresentationDefinition(kUiSubMenuOptionsAudio, "/ui/gamemenu_New/SubMenu_Options_Audio.ui"); });

    g_uiElements[kUiSubMenuOptionsAudio].isSelected = OptionsAudio_IsSelected;
    g_uiElements[kUiSubMenuOptionsAudio].reset = OptionsAudio_Reset;
    g_uiElements[kUiSubMenuOptionsAudio].isPressed = OptionsAudio_IsPressed;
    g_uiElements[kUiSubMenuOptionsAudio].tick = OptionsAudio_Tick;
    g_uiElements[kUiSubMenuOptionsAudio].render = OptionsAudio_Render;
    g_uiElements[kUiSubMenuOptionsAudio].destroy = UI_NullsubReturn1;
    bindPresentation(kUiSubMenuOptionsControls, "Options Controls",
        [] { return LoadPresentationDefinition(kUiSubMenuOptionsControls, "/ui/gamemenu_New/SubMenu_Options_Controls.ui"); });
    g_uiElements[kUiSubMenuOptionsControls].isSelected = OptionsControls_IsSelected;
    g_uiElements[kUiSubMenuOptionsControls].reset = OptionsControls_Reset;
    g_uiElements[kUiSubMenuOptionsControls].isPressed = OptionsControls_IsPressed;
    g_uiElements[kUiSubMenuOptionsControls].tick = OptionsControls_Tick;
    g_uiElements[kUiSubMenuOptionsControls].render = OptionsControls_Render;
    g_uiElements[kUiSubMenuOptionsControls].destroy = UI_NullsubReturn1;
    bindPresentation(kUiSubMenuOptionsGraphics, "Options Graphics",
        [] { return LoadPresentationDefinition(kUiSubMenuOptionsGraphics, "/ui/gamemenu_New/SubMenu_Options_Graphics.ui"); });
    g_uiElements[kUiSubMenuOptionsGraphics].isSelected = OptionsGraphics_IsSelected;
    g_uiElements[kUiSubMenuOptionsGraphics].reset = OptionsGraphics_Reset;
    g_uiElements[kUiSubMenuOptionsGraphics].isPressed = OptionsGraphics_IsPressed;
    g_uiElements[kUiSubMenuOptionsGraphics].tick = OptionsGraphics_Tick;
    g_uiElements[kUiSubMenuOptionsGraphics].render = OptionsGraphics_Render;
    g_uiElements[kUiSubMenuOptionsGraphics].destroy = UI_NullsubReturn1;
    bindPresentation(kUiGameMessage, "Game Message", [] { return LoadPresentationDefinition(kUiGameMessage, "/ui/gamemessage/GameMessage.ui"); });
    g_uiElements[kUiGameMessage].isPressed = UI_NullsubReturn1;
    g_uiElements[kUiGameMessage].tick = GameMessage_Tick;
    g_uiElements[kUiGameMessage].isSelected = GameMessage_IsSelected;
    g_uiElements[kUiGameMessage].reset = GameMessage_Reset;
    g_uiElements[kUiGameMessage].render = GameMessage_Render;
    g_uiElements[kUiGameMessage].destroy = GameMessage_Destroy;
    bindPresentation(
        kUiActionSelector, "Action Selector", [] { return LoadPresentationDefinition(kUiActionSelector, "/ui/actionselector/ActionSelector.ui"); });
    g_uiElements[kUiActionSelector].render = ActionSelector_Render;
    g_uiElements[kUiActionSelector].destroy = ActionSelector_Destroy;
    g_uiElements[kUiActionSelector].tick = ActionSelector_Tick;
    g_uiElements[kUiActionSelector].isSelected = ActionSelector_IsSelected;
    g_uiElements[kUiActionSelector].isPressed = ActionSelector_IsPressed;
    g_uiElements[kUiActionSelector].getSearchType = ActionSelector_GetPromptIconSet;


    UIBaseRuntime &gameTraining = g_uiElements[kUiGameTraining];
    gameTraining.reloadUI = GameTraining_ReloadLegacyUI;
    gameTraining.postInit = GameTrainingWidgetUpdate;
    gameTraining.field_ac = 1;
    gameTraining.name = "Game Training";
    gameTraining.parseDefinitionFromFile = ParseGameTraining;
    gameTraining.isPressed = GameTraining_IsPressed;
    gameTraining.tick = GameTraining_Tick;
    gameTraining.isSelected = GameTraining_IsSelected;
    gameTraining.reset = UI_ScriptNoOpReset;
    gameTraining.render = GameTraining_Render;
    gameTraining.destroy = GameTraining_Destroy;
    gameTraining.unknown28 = UI_SCNFNoOpHandler;
    gameTraining.initialize = UI_ScriptNoOp;
    gameTraining.unknown60 = UI_NullsubReturn1;
    UIBaseRuntime &loadSave = g_uiElements[kUiLoadSave];
    loadSave.name = "Load Save";
    loadSave.index = kUiLoadSave;
    loadSave.parseDefinitionFromFile = LoadSave_ParseDefinition;
    loadSave.isSelected = LoadSave_IsSelected;
    loadSave.isPressed = LoadSave_IsPressed;
    loadSave.tick = LoadSave_Tick;
    loadSave.render = LoadSave_Render;
    loadSave.destroy = LoadSave_Destroy;
    loadSave.getContentPanel = LoadSave_GetContentPanel;
    bindPresentation(kUiMoveLearned, kEwzoUiElementName, [] { return LoadPresentationDefinition(kUiMoveLearned, "/ui/gamehud/GameHudMoveLearned.ui"); });
    g_uiElements[kUiMoveLearned].isPressed = UI_SCNFNoOpHandler;
    g_uiElements[kUiMoveLearned].tick = GameMoveLearned_Tick;
    g_uiElements[kUiMoveLearned].isSelected = GameMoveLearned_IsSelected;
    g_uiElements[kUiMoveLearned].reset = GameMoveLearned_Reset;
    bindPresentation(kUiYesNoPrompt, "Yes/No Prompt", [] { return LoadPresentationDefinition(kUiYesNoPrompt, "/ui/YesNoPrompt/YesNoPrompt_new.ui"); });
    g_uiElements[kUiYesNoPrompt].isPressed = YesNoPrompt_ProcessMessageBox;
    g_uiElements[kUiYesNoPrompt].tick = YesNoPrompt_Tick;
    g_uiElements[kUiYesNoPrompt].isSelected = YesNoPrompt_IsSelected;
    g_uiElements[kUiYesNoPrompt].reset = YesNoPrompt_Reset;
    g_uiElements[kUiYesNoPrompt].render = YesNoPrompt_Render;
    g_uiElements[kUiYesNoPrompt].destroy = YesNoPrompt_Destroy;
    g_uiElements[kUiYesNoPrompt].getSearchType = YesNoPrompt_GetPromptIconSet;
    bindPresentation(kUiGameMenuInformationBar, "Game Menu Information Bar",
        [] { return LoadPresentationDefinition(kUiGameMenuInformationBar, "/ui/gamemenu_New/GameMenu_InformationBar.ui"); });

    g_uiElements[kUiGameMenuInformationBar].isPressed = UI_NullsubReturn1;
    g_uiElements[kUiGameMenuInformationBar].isSelected = InformationBar_IsSelected;
    g_uiElements[kUiGameMenuInformationBar].reset = InformationBar_Reset;
    g_uiElements[kUiGameMenuInformationBar].tick = InformationBar_Tick;
    g_uiElements[kUiGameMenuInformationBar].render = InformationBar_Render;
    g_uiElements[kUiGameMenuInformationBar].destroy = UI_NullsubReturn1;
    g_uiElements[kUiGameMenuInformationBar].getContentPanel = InformationBar_GetContentPanel;
    bindPresentation(
        kUiGameHudBattery, "Game HUD Battery", [] { return LoadPresentationDefinition(kUiGameHudBattery, "/ui/gamemenu_New/GameHud_Battery.ui"); });

    g_uiElements[kUiGameHudBattery].isSelected = BatteryHud_IsSelected;
    g_uiElements[kUiGameHudBattery].isPressed = BatteryHud_IsPressed;
    g_uiElements[kUiGameHudBattery].tick = BatteryHud_Tick;
    g_uiElements[kUiGameHudBattery].getSearchType = BatteryHud_GetPromptIconSet;
    g_uiElements[kUiGameHudBattery].destroy = UI_NullsubReturn1;
    bindPresentation(kUiPlatformIcons, "Platform Icons", [] { return LoadPresentationDefinition(kUiPlatformIcons, "/ui/PlatformIcons.ui"); });
    g_uiElements[kUiPlatformIcons].isSelected = PlatformIcons_IsSelected;
    bindPresentation(kUiGameHud, "Game HUD", [] { return LoadPresentationDefinition(kUiGameHud, "/ui/gamehud/GameHud.ui"); });
    g_uiElements[kUiGameHud].isPressed = UI_NullsubReturn1;
    g_uiElements[kUiGameHud].tick = GameHud_Tick;
    g_uiElements[kUiGameHud].isSelected = GameHud_IsSelected;
    bindPresentation(kUiGameHudWatch, "Game HUD Watch", [] { return LoadPresentationDefinition(kUiGameHudWatch, "/ui/gamehud/GameHudWatch.ui"); });
    g_uiElements[kUiGameHudWatch].field_ac = 1;
    g_uiElements[kUiGameHudWatch].isSelected = GameHudWatch_IsSelected;
    g_uiElements[kUiGameHudWatch].tick = GameHudWatch_Tick;
    g_uiElements[kUiGameHudWatch].render = UI_ScriptNoOp;
    g_uiElements[kUiGameHudWatch].destroy = UI_NullsubReturn1;
    bindPresentation(kUiGameHudGauge, "Unknown", GameHudGauge_LoadUI);
    g_uiElements[kUiGameHudGauge].isSelected = GameHudGauge_IsSelected;
    g_uiElements[kUiGameHudGauge].isPressed = UI_NullsubReturn1;
    g_uiElements[kUiGameHudGauge].tick = GameHudGauge_Tick;
    g_uiElements[kUiGameHudGauge].render = UI_ScriptNoOp;
    g_uiElements[kUiGameHudGauge].destroy = UI_NullsubReturn1;
    bindPresentation(kUiHelpNew, "Help New", [] { return LoadPresentationDefinition(kUiHelpNew, "/ui/gamehelp/Help_New.ui"); });
    g_uiElements[kUiHelpNew].isSelected = HelpNew_IsSelected;
    g_uiElements[kUiHelpNew].isPressed = HelpNew_IsPressed;
    g_uiElements[kUiHelpNew].render = HelpNew_Render;
    g_uiElements[kUiHelpNew].destroy = ResetGlobalInput;
    g_uiElements[kUiHelpNew].tick = HelpNew_Tick;
    g_uiElements[kUiHelpNew].getSearchType = HelpNew_GetPromptIconSet;
    g_uiElements[kUiHelpNew].reset = HelpNew_Reset;
    UIBaseRuntime &wristwatch = g_uiElements[kUiWristwatchOverlay];
    wristwatch.name = "Wristwatch overlay";
    wristwatch.isPressed = WristwatchOverlay_IsPressed;
    wristwatch.render = UI_ScriptNoOp;
    wristwatch.destroy = UI_NullsubReturn1;
    wristwatch.getSearchType = WristwatchOverlay_GetPromptIconSet;
    bindPresentation(kUi70ManBattle, "70 Man Battle", SeventyManBattle_LoadUI);
    g_uiElements[kUi70ManBattle].isSelected = SeventyManBattle_SetupUI;
    g_uiElements[kUi70ManBattle].tick = SeventyManBattle_Tick;
    g_uiElements[kUi70ManBattle].isPressed = UI_NullsubReturn1;
    g_uiElements[kUi70ManBattle].getSearchType = SeventyManBattle_GetPromptIconSet;
    UIBaseRuntime &practiceHud = g_uiElements[kUiGameHudPractice];
    practiceHud.name = "Game HUD: Practice";
    practiceHud.parseDefinitionFromFile = PracticeHud_LoadUI;
    practiceHud.isSelected = UI_NullsubReturn1;
    practiceHud.isPressed = UI_NullsubReturn1;
    practiceHud.tick = PracticeHud_Tick;
    practiceHud.reset = UI_ScriptNoOpReset;
    practiceHud.getSearchType = PracticeHud_GetPromptIconSet;
    UIBaseRuntime &gameOver = g_uiElements[kUiVfGameOver];
    gameOver.parseDefinitionFromFile = GameOver_LoadUI;
    gameOver.isSelected = UI_NullsubReturn1;
    gameOver.tick = GameOver_Tick;
    bindPresentation(kUiCredits, "Credits", [] { return LoadPresentationDefinition(kUiCredits, "/ui/Credits/Credits.ui"); });
    UIBaseRuntime &creditsElement = g_uiElements[kUiCredits];
    creditsElement.isSelected = Credits_InitScrollSpace;
    creditsElement.isPressed = Credits_IsPressed;
    creditsElement.tick = Credits_Tick;
    creditsElement.render = ReadCreditsCSV;
    creditsElement.destroy = Credits_Destroy;
    creditsElement.reset = Credits_Reset;
    UIBaseRuntime &gameQte = g_uiElements[kUiGameQte];
    gameQte.name = "Unknown";
    gameQte.parseDefinitionFromFile = GameQte_ParseDefinition;
    gameQte.isSelected = GameQte_IsSelected;
    gameQte.tick = GameQte_Tick;
    UIBaseRuntime &qteMinigame = g_uiElements[kQteMinigameUiElement];
    qteMinigame.name = "QTE MiniGame";
    qteMinigame.parseDefinitionFromFile = QteMinigame_ParseDefinition;
    qteMinigame.isSelected = QteMinigame_IsSelected;
    qteMinigame.isPressed = UI_NullsubReturn1;
    qteMinigame.tick = QteMinigame_Tick;
    qteMinigame.render = QteMinigame_Render;
    qteMinigame.destroy = QteMinigame_Destroy;
    qteMinigame.reset = QteMinigame_Reset;

    UIBaseRuntime &raceHud = g_uiElements[kUiGameHudRace];
    raceHud.name = "Game HUD: Race";
    raceHud.parseDefinitionFromFile = RaceHud_ParseDefinition;
    raceHud.isSelected = RaceHud_IsSelected;
    raceHud.isPressed = UI_NullsubReturn1;
    raceHud.tick = RaceHud_Tick;
    raceHud.render = RaceHud_Render;
    raceHud.destroy = UI_NullsubReturn1;
    raceHud.reset = UI_ScriptNoOpReset;
    g_uiElements[0x3b].name = "Splash";
    g_uiElements[0x3b].parseDefinitionFromFile = [] { return static_cast<std::uint32_t>(LoadSplash()); };
    g_uiElements[0x3b].isSelected = Splash;
    g_uiElements[0x3b].isPressed = GetInput;
    g_uiElements[0x3b].tick = ProcessGlobalGameState;
    g_uiElements[0x3b].render = InitLoadingSplash;
    g_uiElements[0x3b].destroy = InitShaders;
    g_uiElements[0x3b].reset = ResetSplash;
    g_uiElements[0x3c].name = "Press Start";
    g_uiElements[0x3c].parseDefinitionFromFile = LoadPressStart;
    g_uiElements[0x3c].isSelected = PressStartSelected;
    g_uiElements[0x3c].isPressed = PressStartInput;
    g_uiElements[0x3c].tick = PressStartTick;
    g_uiElements[0x3c].render = PressStartRender;
    g_uiElements[0x3c].destroy = PressStartDestroy;
    UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiStorageIndex];
    subtitles.parseDefinitionFromFile = Subtitles_LoadUI;
    subtitles.render = Subtitles_Render;
    subtitles.destroy = ResetSubtitles;
    subtitles.tick = Subtitles_Tick;
    subtitles.postInit = Subtitles_Tick;
    subtitles.isSelected = Subtitles_IsSelected;
    subtitles.reset = Subtitles_Reset;
    subtitles.name = "Subtitles";
    return LoadScreen.name;
}

std::int32_t g_uiDebugElementDispatch = 0;

std::int32_t g_uiDebugSuspendElements = 0;

std::int32_t g_notebookDebugMode = 0;

std::array<std::uint32_t, 3> g_uiRegisteredCallbackRequests{};

std::array<std::uint32_t, 3> g_uiPreviousInputElements{};


std::int64_t TryInvokeRegisteredCallback(std::int32_t elementIndex) {
    if (g_currUIState != -1 && elementIndex != g_currUIState && (((elementIndex - 56) & ~9) != 0 || elementIndex == 64))
        return 0;
    UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(elementIndex)];
    std::uint32_t &requests = g_uiRegisteredCallbackRequests[static_cast<std::size_t>(elementIndex) >> 5];
    if (element.tick && g_uiDebugElementDispatch == 1) {
        requests |= 1U << (elementIndex & 31);
        return 0;
    }
    if (!element.unknown28)
        return 0;
    requests |= 1U << (elementIndex & 31);
    return element.unknown28();
}

} // namespace shenmue::s1

namespace shenmue::s1::d3t {

void ActivateElementByIdx(std::int32_t elementIndex) {
    if (elementIndex < 0 || elementIndex >= static_cast<std::int32_t>(g_uiElements.size())) {
        return;
    }
    const bool overlay = elementIndex == 56 || elementIndex == 57 || elementIndex == 65;
    if (g_currUIState != -1 && elementIndex != g_currUIState && !overlay) {
        return;
    }

    UI_ActivateElement(g_uiElements[static_cast<std::size_t>(elementIndex)]);
}

bool ResetElementByIdx(std::int32_t elementIndex) {
    if (static_cast<std::uint32_t>(elementIndex) >= 80U ||
        (g_currUIState != -1 && elementIndex != g_currUIState && (((elementIndex - 56) & ~9) != 0 || elementIndex == 64)))
        return false;
    UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(elementIndex)];
    if (element.isVisible && element.destroy) {
        if (element.destroy() != 1 && g_busy_reset != 1)
            return false;
        element.isVisible = 0;
    }
    element.isVisible = 0;
    return true;
}

} // namespace shenmue::s1::d3t

namespace shenmue::s1 {

bool UI_IsFrontendCaptureReady() noexcept {
    static std::uint16_t readyFrames = 0;
    const UIBaseRuntime &pressStart = g_uiElements[0x3c];
    const UIBaseRuntime &mainMenu = g_uiElements[4];
    const UIBaseRuntime &wheel = g_uiElements[kMainMenuWheelElement];
    // New Game and Continue are ready once gameplay is: both leave the main menu through the DISP pick.
    if (g_frontendLaunchTarget == FrontendLaunchTarget::NewGame || g_frontendLaunchTarget == FrontendLaunchTarget::Continue) {
        readyFrames = 0;
        return false;
    }
    bool ready = false;
    if (g_frontendLaunchTarget == FrontendLaunchTarget::MainMenu) {
        ready = mainMenu.isVisible && mainMenu.config.init == 3 && wheel.isVisible && wheel.config.init == 3;
    } else if (g_frontendLaunchTarget == FrontendLaunchTarget::Options) {
        const bool completed = g_frontendAutomationStage == FrontendAutomationStage::Complete && mainMenu.isVisible && mainMenu.config.init == 3 &&
            wheel.isVisible && wheel.config.init == 3;
        const bool failed = g_frontendAutomationStage == FrontendAutomationStage::Failed &&
            ((pressStart.isVisible && pressStart.config.init == 3) || (mainMenu.isVisible && mainMenu.config.init == 3));
        ready = completed || failed;
    } else {
        ready = (pressStart.isVisible && pressStart.config.init == 3) || (mainMenu.isVisible && mainMenu.config.init == 3);
    }
    readyFrames = ready ? static_cast<std::uint16_t>(readyFrames + 1U) : 0;
    return readyFrames >= 30;
}


std::int64_t PlatformIcons_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiPlatformIcons].config.uiHandleIndex);
    for (std::size_t index = 0; index < g_platformButtonIconImages.size(); ++index) {
        const std::int32_t image = UI_FindViewImage(slot, kPlatformButtonIconImageNames[index]);
        g_platformButtonIconImages[index] = image == -1 || slot > 0x4bU ? nullptr : &g_uiViewSlots[slot].images[static_cast<std::size_t>(image)];
    }
    g_platformIconsSheet = -1;
    for (std::size_t sheet = 0; sheet < g_uiSpriteSheets.size(); ++sheet) {
        const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
        if (candidate.imageName && candidate.used == 1 && !std::strncmp("ButtonIcons", candidate.imageName, 0x40)) {
            g_platformIconsSheet = static_cast<std::int64_t>(sheet);
            break;
        }
    }
    return 1;
}

} // namespace shenmue::s1
