#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/ui_state_dc.hpp"
#include "s2/money_icon.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/gauge.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/pause.hpp"
#include "s2/math.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/map_control_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "enhancements/misc_mods.hpp"
#include "s2/enhancements/tai_chi.hpp"
#include "s2/ui.hpp"
#include "s2/easy_option.hpp"
#include "s2/opening_staff_roll.hpp"
#include "s2/magic_room.hpp"
#include "s2/ctrl_speak_icon.hpp"
#include "s2/now_qte.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/ui_transition.hpp"
#include "s2/ui_scene.hpp"
#include "s2/ui_grid.hpp"
#include "s2/ui_collection.hpp"
#include "s2/ui_moves.hpp"
#include "s2/ui_loadsave.hpp"
#include "s2/ui_prompt.hpp"
#include "s2/input_bindings.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/area_0001.hpp"
#include "s2/areas/area_armw.hpp"
#include "s2/areas/game/arcade.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/omak_collection_ui.hpp"

#include "d3t/content.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "platform/performance_counter.hpp"
#include "s2/input.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/moves_scroll.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/item_wf.hpp"
#include "s2/item_selector.hpp"
#include "s2/main_icon.hpp"
#include "s2/flag.hpp"
#include "s2/item_selector_actions.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/math.hpp"
#include "s2/screen_film.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/snapshot.hpp"
#include "s2/conversation.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/scene_data.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/sound_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/memo_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/font.hpp"
#include "s2/archive_data.hpp"
#include "s2/pak.hpp"
#include "s2/directory_cache.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/inventory.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/date_time.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/save_screen_state_dc.hpp"
#include "s2/easy_option_state_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/gauge.hpp"
#include "s2/npc_package.hpp"
#include "s2/sound_program.hpp"
#include "s2/navigation.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/pawn_shop.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/file_loader_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/item_selection_queries_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <stdio.h>
#include <string.h>
#define UI_STRLEN ::strlen

#define UI_NOEXCEPT
#define UI_ICON(name) name
#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#define UI_ZERO
#include <cstring>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/hand_motion.hpp"
#include "platform/dc/sdk.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/author.hpp"
#include "s2/help.hpp"
#include "s2/pause.hpp"
#include "s2/memo.hpp"
#include "s2/search.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"
#include "s2/display_configuration.hpp"
#include "s2/audio_data.hpp"
#include "s2/hand.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/legacy_collection.hpp"
#include "s2/pawn.hpp"
#include "s2/game_runtime.hpp"
#include <json/json.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <charconv>
#include <cctype>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cmath>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "s2/directory_cache.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title_dc.hpp"
#include <sg_gd.h>
#define SHENMUE_LOADER_NULL 0
#define SHENMUE_LOADER_ALIGN __attribute__((aligned(32)))
#define SHENMUE_LOADER_INIT =
#define SHENMUE_LOADER_MEMCPY ::memcpy
#define SHENMUE_LOADER_STRCPY ::strcpy
#define SHENMUE_LOADER_STRCMP ::strcmp
#define SHENMUE_LOADER_STRSTR ::strstr
#define SHENMUE_LOADER_STRLEN ::strlen
#define SHENMUE_LOADER_STRNCPY ::strncpy
#define SHENMUE_LOADER_FILE_COUNT (sizeof(g_files) / sizeof(g_files[0]))
#define SHENMUE_LOADER_CONST_BYTES(value) static_cast<const std::byte *>(static_cast<const void *>(value))
#include <zlib.h>
#include "s2/startup_assets_dc.hpp"
#include "s2/event_overlay.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/texture_dc.hpp"
#include "s2/legacy_sprite_slot.hpp"
#include "common/math.hpp"
#include "platform/dc/screen.hpp"
#define SPRITE_NULL 0
#define SPRITE_ZERO
#define SPRITE_INIT =
#define SPRITE_ARRAY(type, name, count) type name[count]
#define SPRITE_DATA(name) (name)
#define SPRITE_MEMCPY ::memcpy
#define SPRITE_ALIGN(name) name
#define SPRITE_MODE(name) name
#include <cstdio>
#define SHENMUE_MOTION_DATA(name) name
#define SHENMUE_MOTION_NAME(value) shenmue::shen_name(value)
#include "s2/character_yaw_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/motion_task_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/pak_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/world_render.hpp"
#include "s2/field_collision_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_model_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
#endif

namespace shenmue {
namespace dc {
namespace sdk {
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMirroredModelDrawMode(std::int32_t enabled);
#endif

} // namespace sdk
} // namespace dc
} // namespace shenmue
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_transparency_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/player_state_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"
#include "auto_test.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/title_dc.hpp"
#include "s2/speak_icon_labels_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <limits>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <stdexcept>
#include <vector>
#include "s2/area_names.hpp"

#include "s2/ui_internal.hpp"
#endif


namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct NativeItemSelectorContext {
    std::uint8_t helpTopic, selected;
    std::uint16_t visible;
    std::uint32_t playerDisplay, playerVisible;
    std::int32_t cameraState;
    std::uint32_t retained10, perspective;
    Vector3 eye, target;
};

typedef char NativeItemSelectorContextSize[sizeof(NativeItemSelectorContext) == 0x30 ? 1 : -1];
typedef char NativeItemSelectorContextEyeOffset[offsetof(NativeItemSelectorContext, eye) == 0x18 ? 1 : -1];

struct NativeItemSelectorActionCallbacks {
    std::int32_t (*begin)();
    std::int32_t (*update)();
    void (*complete)();
    void (*cancel)(std::int32_t);
};

struct ConversationBuffer;
struct TextureChunk;
typedef bool (*IconInputSlotClose)(std::int32_t force);
typedef std::int32_t (*ItemWindowShape)(std::int32_t cell, std::int32_t columns);

const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::int16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::int16_t GetInventoryItemIconById(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
void ProcessLegacyItemSelectorInput();
static bool NavigateLegacyItemSelector(NativeItemSelectorState &state, std::uint16_t pressed);
static void NavigateItemSelectorInventory(NativeItemSelectorState &state, std::uint16_t pressed);
static void FinishItemSelector();
static std::int32_t BeginSelectorWatch();
static std::int32_t UpdateSelectorWatch();
static std::int32_t CancelSelectorFullMap(std::int32_t);
static std::int32_t BeginSelectorItemLook();
static std::int32_t UpdateSelectorItemLook();
static void CancelSelectorItemLook(std::int32_t force);
static std::int32_t BeginSelectorSave();
static std::int32_t UpdateSelectorSave();
static void CompleteSelectorSave();
static std::int32_t BeginSelectorEasyOptions();
static std::int32_t UpdateSelectorEasyOptions();
static std::int32_t UpdateSelectorMoves();
static std::int32_t CancelSelectorMoves(std::int32_t force);
static std::int32_t BeginSelectorCollection();
static std::int32_t BeginSelectorPhotos();
static std::int32_t UpdateSelectorCollection();
static std::int32_t UpdateSelectorPhotos();
static std::int32_t BeginSelectorPawn();
static std::int32_t UpdateSelectorPawn();
static std::int32_t FindUniqueItemActionIndex(std::int32_t item);
void ResolveUniqueItemAction(std::uint32_t mode);
void ICIT_UniqAction_Callback(void *);
static void ReturnToItemSelectorList();
static std::int32_t BeginSelectorMemo();
static void CancelNativeSelectorMemo(std::int32_t force);
static void PrepareItemSelectorCamera(NativeItemSelectorCamera *camera);
static void CloseItemSelectorCamera(NativeItemSelectorCamera *camera);
static bool AdvanceItemSelectorCamera(NativeItemSelectorCamera *camera);
bool CloseItemSelectIcon(std::int32_t force);
std::int32_t PrepareLegacyInventoryItemLabel(std::int16_t *label, std::uint32_t item);
void DrawLegacyItemLabel(const std::int16_t *texture, const Vector2 *position, float depth, std::uint32_t secondaryColour);
static std::int32_t MoneyIconStep(std::int32_t delta);
bool AbortMoneyIcon(std::int32_t force);
static std::int32_t GetCommonIconTextureCount();
std::uint16_t GetPadControlData(std::uint8_t mode);
void InitQuickIcon_Callback(void *);
void InitializeQuickIconSprite();
bool ResetQuickIconState(std::int32_t force);
void InitSpeakIcon(HLib::Task *parent, std::uint8_t queue);
void InitializeCtrlSpeakIconState();
static void PrepareSpeakTextSlot(std::int8_t slot);
std::int32_t AcquireCtrlSpeakTextHandle(const char *text, std::int32_t centered);
void SetMoneyIconPhase(std::uint8_t phase);
void InitializeMoneyIconParameters();
bool ToggleItemEffect(std::uint32_t item, std::int32_t amount);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void GetCurrentCalendarRecord(DateTime &record);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void ApplyItemSelectorDelayedResult();
void RenderLegacyItemSelector(void *parameter);
static void SetupMoneyIconForRendering(void *);
static void UpdateMoneySettle();
void SetCashOrSegaCoins();
static void PrepareMoneyIconCurrency();
static void DrawMoneyOptionText(std::int32_t text, std::int32_t direction);
static void AdvanceMoneySelectionScale();
struct LegacyMoneyLayout {
    Vector2 position;
    float depth, scale;
};
static void DrawNativeMoneyAmount(
    const LegacyMoneyLayout *layout, std::int32_t amount, std::int8_t currency, std::uint32_t colour, std::uint32_t fullPrefix = 0);
static void AnimateMoneyUiElement(void *);
static void ChooseMoneyOptions(std::uint32_t flags);
static void ReleaseMoneyTextSlots();
static std::int32_t GetSelectorNumberDigits(std::int32_t amount);
bool AdvanceIconPosition(ItemEffectPosition *position, float target);
bool RetreatIconPosition(ItemEffectPosition *position, float target);
void iconToggleStateFor(std::uint8_t slot, std::uint8_t enabled);
std::uint32_t GetItemSelectionMode(std::uint8_t slot);
bool GetShouldRenderIcons(RenderIconSlot slot);
bool ToggleRenderIcons(RenderIconSlot slot, bool enabled);
static std::uint32_t RetireAllIconInputSlots(std::int32_t force);
bool padcRefreshCurrentInputs(std::int32_t bind);
void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour);
void PrepareDrawSimpleBlack(const LegacyIconSprite &sprite, std::uint32_t colour);
void DrawMoneyAmount(const Vector2 *position, float depth, std::int32_t amount, std::int8_t currency, std::uint32_t colour, float scale);
void RefreshItemSelectorOwnedMask();
std::int32_t ReleaseCtrlSpeakTextHandle(std::int32_t handle);
bool CloseSpeakIcon(std::int32_t force);
void CloseSpeakIcon();
void InspectCtrlSpeakChoicesNullsub(const std::int16_t *);
float GetCtrlSpeakChoiceWidth(const std::uint16_t *choice, std::int32_t form);
void CtrlSpeakIcon_Callback(void *);
HLib::Task *CtrlSleepActionIcon(std::int8_t queue);
void InitNowQte(std::int8_t queue);
static void BufferItemSelectorInput(std::uint32_t pressed);
static void MarkItemSelectorPlayerMoved();
void InitializeItemSelectorWindow();
static std::int32_t CheckMainIconItemSelection();
static std::int32_t CheckMainIconShortcut();
static std::int32_t IsMainIconInputAdmitted();
void ResetMainIconState();
void InitMainIcon_Callback(void *);
void SetItemSelectorClosingState();
static NativeItemSelectorContext *CaptureItemSelectorContext();
static void RestoreItemSelectorContext(NativeItemSelectorState *state);
static std::int32_t GetItemSelectorSelectedItem(std::int32_t itemId);
std::uint32_t GetCurrentItemSelectState();
void RefreshItemSelectorSelection();
static void ClampItemSelectorSelection();
HLib::Task *QueueItemEffectIconTask(std::uint8_t queue);
bool CloseItemEffect(std::int32_t force);

const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::int16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::int16_t GetInventoryItemIconById(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::int16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::int16_t GetInventoryItemIconById(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
void SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int16_t duration, std::int32_t step, std::uint32_t flags);
void ResetSleepActionDefaultState();
void doItemEffectStuff();
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::int16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::int16_t GetInventoryItemIconById(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
void SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int16_t duration, std::int32_t step, std::uint32_t flags);
void ResetSleepActionDefaultState();
void doItemEffectStuff();
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
void SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int16_t duration, std::int32_t step, std::uint32_t flags);
void ResetSleepActionDefaultState();
void doItemEffectStuff();
std::uint8_t UpdateSelectorMemo();
static void icsuMoveRotateAdjustItemScale();
static void icsuRenderItem();
void CloseItemLook();
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
static void icsuMoveRotateAdjustItemScale();
static void icsuRenderItem();
void CloseItemLook();
std::uint8_t UpdateSelectorMemo();
void ResetSleepActionDefaultState();
void doItemEffectStuff();
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
void ResetSleepActionDefaultState();
void doItemEffectStuff();
const std::int8_t *GetCommonIconRect(std::int32_t icon);
void InitializeLegacyIconSprite(LegacyIconSprite &sprite);

void DrawLegacyWindowHorizontal(float x, float y, std::int32_t columns, std::int32_t edgeType, std::int32_t flipped, float depth, std::int32_t flags);

void DrawLegacyWindowTiles(float x, float y, std::int32_t columns, std::int32_t rows, float depth, std::int32_t flags);
void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags);
void DrawLegacyWindowEdges(float x, float y, std::int32_t rows, std::int32_t columns, float depth, std::int32_t flags);
void DrawLegacySelectionGlow(const Vector2 &position, std::uint32_t &colour, bool &brightening, float depth);
void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour);
void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth);
void DrawLegacyColourPacket(const LegacySpritePacket &packet, float depth);
void DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth);
std::int32_t GetCommonIconTextureIndex(std::uint32_t icon);
void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour);
void PrepareDrawWithMode(const LegacyIconSprite &sprite);
void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite);

void AdjustScreenAspectRatio(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);

void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation);

const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
void ResetSleepActionDefaultState();
void doItemEffectStuff();
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
void ResetSleepActionDefaultState();
void doItemEffectStuff();
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
void ResetSleepActionDefaultState();
void doItemEffectStuff();
const std::int8_t *GetCommonIconRect(std::int32_t icon);
void InitializeLegacyIconSprite(LegacyIconSprite &sprite);

void DrawLegacyWindowHorizontal(float x, float y, std::int32_t columns, std::int32_t edgeType, std::int32_t flipped, float depth, std::int32_t flags);

void DrawLegacyWindowTiles(float x, float y, std::int32_t columns, std::int32_t rows, float depth, std::int32_t flags);
void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags);
void DrawLegacyWindowEdges(float x, float y, std::int32_t rows, std::int32_t columns, float depth, std::int32_t flags);
void DrawLegacySelectionGlow(const Vector2 &position, std::uint32_t &colour, bool &brightening, float depth);
void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour);
void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth);
void DrawLegacyColourPacket(const LegacySpritePacket &packet, float depth);
void DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth);
std::int32_t GetCommonIconTextureIndex(std::uint32_t icon);
void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour);
void PrepareDrawWithMode(const LegacyIconSprite &sprite);
void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite);

void AdjustScreenAspectRatio(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);

void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation);

std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
void ResetSleepActionDefaultState();
void doItemEffectStuff();
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
void ResetSleepActionDefaultState();
void doItemEffectStuff();
std::uint32_t shenOpenFile(const char *directory, const char *filename);
std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size);
void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName);
void ResetSleepActionDefaultState();
void ResetSleepActionDefaultState();
void ResetSleepActionDefaultState();
bool OpenItemLook(std::int32_t item);
std::int32_t EV_CanDoAreaJump();
void UpdateSelectedItemStateFlag(std::int16_t item);
void ICSU_InitItemLook_Callback(void *);
HLib::Task *mainIconTask;
std::int32_t nativeGuiActive, collectionSpriteOwners[2] = {0, 0};
void TickScenarioFlagNotification();
bool IsScenarioBusyForMainIcon();
void InitializeLegacyCollectionMenu();
void Queue_wf_MainControl(std::uint8_t queue);
static NativeItemSelectorContext itemSelectorContext;
std::int32_t OpenFullMapMenu();
std::int32_t UpdateFullMapMenu();
void CloseFullMapMenu();
std::int32_t OpenLegacyCollectionMenu(std::int32_t mode);
std::uint8_t UpdateLegacyCollectionMenu();
void CloseLegacyCollectionMenu(std::int32_t force);
std::int32_t OpenNaviWindow();
std::int32_t UpdateNaviWindow();
std::int32_t CloseNaviWindow(std::int32_t force);
std::int32_t OpenWfScrollMenu();
std::int32_t GetwfMainControlState();
void CloseWfScrollMenu(std::int32_t force);
bool UpdateWazamakiMenu();
void CloseWazamakiMenu();

static const NativeItemSelectorActionCallbacks itemSelectorActions[12] = {
    {BeginSelectorMemo, reinterpret_cast<std::int32_t (*)()>(UpdateSelectorMemo), 0, CancelNativeSelectorMemo}, {BeginSelectorWatch, UpdateSelectorWatch, 0, 0},
    {OpenFullMapMenu, UpdateFullMapMenu, CloseFullMapMenu, (void (*)(std::int32_t))CancelSelectorFullMap},
    {BeginSelectorItemLook, UpdateSelectorItemLook, 0, CancelSelectorItemLook}, {BeginSelectorSave, UpdateSelectorSave, CompleteSelectorSave, 0},
    {BeginSelectorEasyOptions, UpdateSelectorEasyOptions, 0, 0}, {0, UpdateSelectorMoves, 0, (void (*)(std::int32_t))CancelSelectorMoves},
    {BeginSelectorCollection, UpdateSelectorCollection, 0, CloseLegacyCollectionMenu}, {BeginSelectorPhotos, UpdateSelectorPhotos, 0, 0},
    {OpenNaviWindow, UpdateNaviWindow, 0, (void (*)(std::int32_t))CloseNaviWindow}, {BeginSelectorPawn, UpdateSelectorPawn, 0, 0},
    {OpenWfScrollMenu, GetwfMainControlState, 0, CloseWfScrollMenu}};

bool ParseGuiPtx(TextureChunk *texture, std::int32_t height, std::int32_t width, std::uint32_t idLow, std::uint32_t idHigh, std::uint32_t format);
std::int32_t ReplacePtxTexture(std::int32_t index, const TextureChunk *texture);
std::int32_t LoadAndFindTEXN_Wrapper(const TextureChunk *texture);
ConversationBuffer *shenPlayAudioWithFlags(const char *code, std::uint32_t flags);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t cvsHasValidConverseData(const ConversationBuffer *buffer) UI_NOEXCEPT;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
std::int32_t camCtrlGetPerspectiveWrapper();
void PrepareDrawWithMode(const LegacyIconSprite &sprite);

void AdjustScreenAspectRatio(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);

void DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth);
std::int32_t RasterizeWindowTextLine4bpp(std::byte *destination, std::int32_t width, const char *text, std::int32_t mode, std::int32_t centered);
ItemEffectPosition quickIconPosition;
static const std::uint16_t quickIconDirectionalStatuses[16] = {0, 1, 16, 0, 64, 128, 32, 0, 4, 2, 8, 0, 0x504e, 0x2e43, 0x4942, 0x004e};
static const std::uint32_t quickIconResultSounds[2][2] = {{0x900ab, 0xa00ab}, {0x15ab, 0x115ab}};
static ItemEffectPosition moneyIconPosition, moneyIconSelectorPosition;
static LegacyIconSprite moneyIconSprite, moneyIconDirections[4], moneyIconCurrencySprite;
static LegacyIconSprite moneyIconSigns[2], moneyIconTextSprites[2];
static std::uint8_t moneyIconCurrency;
static std::int32_t moneyIconLastIncrement;
static float moneyIconSelectionScale, moneyIconSettleFrames;
static std::uint16_t moneyIconSavedHelpSelected = 0, moneyIconSavedHelpVisible = 0;
static std::int16_t moneyIconSavedHelpPresent = 0;
std::uint32_t GetUiFlag(std::uint8_t mode);
void AddHelpIconMenuFlags(std::uint16_t flags, std::uint8_t selected);
void SetHelpIconMenuFlags(std::uint16_t flags, std::uint8_t selected);
void ResetHelpIconsToItemPrompt();
std::int32_t camCtrlSetPerspectiveWrapper(std::int16_t perspective);
static LegacyIconSprite selectorDateWeekday = {0, 0, 1, 1, 0, 1024, 0, 0, 0x507, 48, 16, 4, {0}};
static LegacyIconSprite selectorDateDigit = {0, 0, 1, 1, 0, 1024, 0, 0, 0x50f, 16, 16, 4, {0}};

static const IconInputSlotClose iconInputSlotClosers[7] = {
    (IconInputSlotClose)RetireAllIconInputSlots, CloseItemSelectIcon, CloseSpeakIcon, ResetQuickIconState, AbortMoneyIcon, CloseItemEffect, 0};

static std::uint32_t ctrlSpeakPulseAngle;
static float ctrlSpeakResizeFrame, ctrlSpeakRadialScale, ctrlSpeakRetainedMotion;
static std::int32_t ctrlSpeakSelectionAnimationFrame;
static std::int16_t ctrlSpeakTargetChoiceWidths[4], ctrlSpeakChoiceWidthDeltas[4];
static const std::int8_t selectorDirectionToSlot[12] = {0, 4, 3, 4, 2, 4, 1, 4, 4, 0, 2, 0};
static std::uint16_t selectorMarkers[5] = {45, 42, 55, 56, 62}, selectorRingIcons[3] = {1286, 42, 1543};
static LegacyIconSprite sleepActionArrowSprites[4], sleepActionTextSprites[4];
static const std::int16_t sleepArrowX[4] = {0, -61, 0, 62}, sleepArrowY[4] = {-49, 0, 50, 0};
static const std::int16_t sleepTextX[4] = {0, -55, 0, 56}, sleepTextY[4] = {-44, 0, 43, 0};
static const float sleepArrowDepth[4] = {1.0f, 2.0f, 1.0f, 3.0f}, sleepTextDepth[4] = {4.0f, 4.0f, 5.0f, 5.0f};
static std::int16_t helpIconPulseValue, helpIconPulseStep;
static std::int32_t helpIconBlinkFrames, helpIconBlinkVisible;
static std::int32_t helpIconHudDelay, helpIconRefreshDelay;
static std::uint32_t helpIconDrawVisible, helpIconDrawSelected, helpIconPublishedVisible, qteForcedHelp;
static float helpIconRemovalBlinkMask;
static std::uint32_t helpIconHintAngle0, helpIconHintAngle1, helpIconHintAngle2, helpIconHintAngle3;
static std::int16_t helpIconHintIds[4];
static HLib::Task *nowQteTask;
static const std::uint32_t helpButtonMasks[4] = {4, 2, 16, 8};
static const std::int16_t helpHintIcons[28] = {43, 42, 56, 45, 45, 51, 44, 52, 46, 57, 55, 58, 48, 49, 47, 50, 58, 53, 61, 59, 60, 45, 783, 782, 62, 63, 38, 0};
MotionWork *g_activeMotionWork;
std::uint8_t moneyIconPhase, moneyIconDrawMode = 0;
std::int32_t moneyIconTarget;
const std::int32_t inventoryItemCounts[6] = {42, 275, 18, 26, 26, 12};
const std::int32_t itemTypeByThousands[10] = {0, 0, 6, 4, 1, 6, 3, 6, 2, 5};

const InventoryItemDefinition keyItems[] = {
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 20, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1200, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\301\264\302\316\245\336\245\303\245\327", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 12, SHEN_FIELD(id) 0, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1200, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\341\245\342\304\242", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 3, SHEN_FIELD(id) 21, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\263\245\244\245\363", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 10, SHEN_FIELD(id) 2, SHEN_FIELD(minimumDistance) 1000, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\273\376\267\327", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 13, SHEN_FIELD(id) 3, SHEN_FIELD(minimumDistance) 640, SHEN_FIELD(maximumDistance) 2080, SHEN_FIELD(model) "TUBA1T1",
        SHEN_FIELD(label) "\305\341\244\316\304\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 14, SHEN_FIELD(id) 4, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "PNX02H6",
        SHEN_FIELD(label) "\313\261\321\340\266\300", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 22, SHEN_FIELD(id) 5, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 610, SHEN_FIELD(model) "LMAL200",
        SHEN_FIELD(label) "\304\304\302\347\277\315\244\316\276\322\262\360\276\365", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 15, SHEN_FIELD(id) 6, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1660, SHEN_FIELD(model) "OMMR1M1",
        SHEN_FIELD(label) "\244\252\274\351\244\352", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 16, SHEN_FIELD(id) 7, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1660, SHEN_FIELD(model) "HAFK100",
        SHEN_FIELD(label) "\307\362\244\244\315\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 20, SHEN_FIELD(id) 24, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "ZIPOHHV",
        SHEN_FIELD(label) "\245\330\245\326\245\363\245\272\245\270\245\303\245\335\241\274", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 17, SHEN_FIELD(id) 22, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "MAGA2WH",
        SHEN_FIELD(label) "\270\373\266\314", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 21, SHEN_FIELD(id) 10, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "QDKJASI",
        SHEN_FIELD(label) "\273\315\277\300\275\303\244\316\277\336\314\314", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 18, SHEN_FIELD(id) 11, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "IKSM2OM",
        SHEN_FIELD(label) "\245\263\245\244\245\363\311\275", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 18, SHEN_FIELD(id) 12, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "IKSM2UR",
        SHEN_FIELD(label) "\245\263\245\244\245\363\316\242", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 19, SHEN_FIELD(id) 13, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "OMMR2H1",
        SHEN_FIELD(label) "\267\260\244\316\244\252\274\351\244\352", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 512, SHEN_FIELD(id) 25, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "MEDA2GL",
        SHEN_FIELD(label) "\302\347\244\255\244\312\266\342\245\341\245\300\245\353", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 513, SHEN_FIELD(id) 26, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "MEDA2SV",
        SHEN_FIELD(label) "\302\347\244\255\244\312\266\344\245\341\245\300\245\353", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 514, SHEN_FIELD(id) 27, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "MEDA2CP",
        SHEN_FIELD(label) "\302\347\244\255\244\312\306\274\245\341\245\300\245\353", SHEN_FIELD(flags) 0x2U},
    {SHEN_FIELD(icon) 23, SHEN_FIELD(id) 1, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 500, SHEN_FIELD(model) "LETT1L1",
        SHEN_FIELD(label) "\277\306\311\343\260\270\244\316\274\352\273\346", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 24, SHEN_FIELD(id) 14, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "TMEM200",
        SHEN_FIELD(label) "\311\360\316\323\275\361\244\316\273\346", SHEN_FIELD(flags) 0x8U},
    {SHEN_FIELD(icon) 24, SHEN_FIELD(id) 14, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "TMEM200",
        SHEN_FIELD(label) "\303\343\317\322\277\330\244\316\245\341\245\342", SHEN_FIELD(flags) 0x8U},
    {SHEN_FIELD(icon) 25, SHEN_FIELD(id) 35, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "WS00ALE",
        SHEN_FIELD(label) "\275\241\300\364\244\316\274\352\273\346", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 26, SHEN_FIELD(id) 36, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "TYOM28J",
        SHEN_FIELD(label) "\243\270\273\376\306\246\276\255\270\256", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 26, SHEN_FIELD(id) 37, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "BSKM2HO",
        SHEN_FIELD(label) "\312\375\315\350\316\271\274\322", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 26, SHEN_FIELD(id) 38, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "BKOI200",
        SHEN_FIELD(label) "\312\270\311\360\270\370\261\340\241\304", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 26, SHEN_FIELD(id) 40, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "TOSM29J",
        SHEN_FIELD(label) "\243\271\273\376\312\270\311\360\270\370\261\340", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 26, SHEN_FIELD(id) 39, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "QEME203",
        SHEN_FIELD(label) "\311\264\273\322\317\260\243\265\243\260\243\263", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 4, SHEN_FIELD(id) 9000, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\274\301\262\260\245\321\245\363\245\325\241\312\262\276\241\313", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 263, SHEN_FIELD(id) 71, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN5",
        SHEN_FIELD(label) "\307\362\270\327\244\316\313\300", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 258, SHEN_FIELD(id) 72, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN3",
        SHEN_FIELD(label) "\270\274\311\360\244\316\313\300", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 260, SHEN_FIELD(id) 73, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN1",
        SHEN_FIELD(label) "\300\304\316\266\244\316\313\300", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 256, SHEN_FIELD(id) 74, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN4",
        SHEN_FIELD(label) "\265\373\244\316\313\300", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 257, SHEN_FIELD(id) 76, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN7",
        SHEN_FIELD(label) "\267\334\244\316\313\300", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 261, SHEN_FIELD(id) 77, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN0",
        SHEN_FIELD(label) "\274\353\277\375\244\316\313\300", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 262, SHEN_FIELD(id) 78, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN6",
        SHEN_FIELD(label) "\303\366\244\316\313\300", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 259, SHEN_FIELD(id) 79, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QDKJAN8",
        SHEN_FIELD(label) "\273\263\315\323\244\316\313\300", SHEN_FIELD(flags) 0x10U},
    {SHEN_FIELD(icon) 264, SHEN_FIELD(id) 29, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QFIXZK1",
        SHEN_FIELD(label) "\307\333\305\305\310\327\270\260\243\262\243\271", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 264, SHEN_FIELD(id) 30, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QFIXZK2",
        SHEN_FIELD(label) "\307\333\305\305\310\327\270\260\243\261\243\260", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 264, SHEN_FIELD(id) 31, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QFIXZK3",
        SHEN_FIELD(label) "\307\333\305\305\310\327\270\260\243\264\243\266", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 264, SHEN_FIELD(id) 32, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QFIXZK4",
        SHEN_FIELD(label) "\307\333\305\305\310\327\270\260\243\271", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 264, SHEN_FIELD(id) 33, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QFIXZK5",
        SHEN_FIELD(label) "\307\333\305\305\310\327\270\260\243\261\243\270", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 264, SHEN_FIELD(id) 34, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1300, SHEN_FIELD(model) "QFIXZK6",
        SHEN_FIELD(label) "\307\333\305\305\310\327\270\260\243\262\243\261", SHEN_FIELD(flags) 0x0U},
};

const InventoryItemDefinition capsuleToys[] = {
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4000, SHEN_FIELD(minimumDistance) 190, SHEN_FIELD(maximumDistance) 870, SHEN_FIELD(model) "GACIAK1",
        SHEN_FIELD(label) "\245\242\245\255\245\351\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2306, SHEN_FIELD(id) 4001, SHEN_FIELD(minimumDistance) 190, SHEN_FIELD(maximumDistance) 870, SHEN_FIELD(model) "GACIAK2",
        SHEN_FIELD(label) "\245\242\245\255\245\351\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2312, SHEN_FIELD(id) 4002, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 780, SHEN_FIELD(model) "GACOJK1",
        SHEN_FIELD(label) "\245\270\245\343\245\303\245\255\241\274\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2314, SHEN_FIELD(id) 4003, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 780, SHEN_FIELD(model) "GACOJK2",
        SHEN_FIELD(label) "\245\270\245\343\245\303\245\255\241\274\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2308, SHEN_FIELD(id) 4004, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 850, SHEN_FIELD(model) "GACOSA1",
        SHEN_FIELD(label) "\245\265\245\351\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2309, SHEN_FIELD(id) 4005, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 780, SHEN_FIELD(model) "GACOSA2",
        SHEN_FIELD(label) "\245\265\245\351\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2328, SHEN_FIELD(id) 4006, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 740, SHEN_FIELD(model) "GACOLA1",
        SHEN_FIELD(label) "\245\351\245\246\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2329, SHEN_FIELD(id) 4007, SHEN_FIELD(minimumDistance) 190, SHEN_FIELD(maximumDistance) 710, SHEN_FIELD(model) "GACOLA2",
        SHEN_FIELD(label) "\245\351\245\246\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2321, SHEN_FIELD(id) 4008, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 840, SHEN_FIELD(model) "GACOPA1",
        SHEN_FIELD(label) "\245\321\245\244\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2320, SHEN_FIELD(id) 4009, SHEN_FIELD(minimumDistance) 180, SHEN_FIELD(maximumDistance) 680, SHEN_FIELD(model) "GACOPA2",
        SHEN_FIELD(label) "\245\321\245\244\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2316, SHEN_FIELD(id) 4010, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 810, SHEN_FIELD(model) "GACGWF1",
        SHEN_FIELD(label) "\245\246\245\353\245\325\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2317, SHEN_FIELD(id) 4011, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 700, SHEN_FIELD(model) "GACGWF2",
        SHEN_FIELD(label) "\245\246\245\353\245\325\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2324, SHEN_FIELD(id) 4012, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "GACGJF1",
        SHEN_FIELD(label) "\245\270\245\247\245\325\245\352\241\274\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2325, SHEN_FIELD(id) 4013, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 970, SHEN_FIELD(model) "GACGJF2",
        SHEN_FIELD(label) "\245\270\245\247\245\325\245\352\241\274\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2332, SHEN_FIELD(id) 4014, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 850, SHEN_FIELD(model) "GACIKA1",
        SHEN_FIELD(label) "\245\253\245\262\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2333, SHEN_FIELD(id) 4015, SHEN_FIELD(minimumDistance) 180, SHEN_FIELD(maximumDistance) 720, SHEN_FIELD(model) "GACIKA2",
        SHEN_FIELD(label) "\245\253\245\262\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2593, SHEN_FIELD(id) 4016, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 810, SHEN_FIELD(model) "GACRDR1",
        SHEN_FIELD(label) "\245\307\245\345\245\351\245\353\245\267\245\353\245\320\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2592, SHEN_FIELD(id) 4017, SHEN_FIELD(minimumDistance) 150, SHEN_FIELD(maximumDistance) 630, SHEN_FIELD(model) "GACRDR2",
        SHEN_FIELD(label) "\245\307\245\345\245\351\245\353\245\264\241\274\245\353\245\311", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2313, SHEN_FIELD(id) 4018, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 820, SHEN_FIELD(model) "GACM401",
        SHEN_FIELD(label) "\314\332\277\315", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2358, SHEN_FIELD(id) 4019, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 810, SHEN_FIELD(model) "GACRREN",
        SHEN_FIELD(label) "\245\354\245\363\245\277\245\322\241\274\245\355\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2856, SHEN_FIELD(id) 4020, SHEN_FIELD(minimumDistance) 390, SHEN_FIELD(maximumDistance) 1030, SHEN_FIELD(model) "GACK6TZ",
        SHEN_FIELD(label) "\243\324\243\322\243\331\241\274\243\332", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2605, SHEN_FIELD(id) 4021, SHEN_FIELD(minimumDistance) 350, SHEN_FIELD(maximumDistance) 730, SHEN_FIELD(model) "GACRPAN",
        SHEN_FIELD(label) "\245\275\245\355\245\246\245\244\245\363\245\260", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2307, SHEN_FIELD(id) 4024, SHEN_FIELD(minimumDistance) 170, SHEN_FIELD(maximumDistance) 950, SHEN_FIELD(model) "GACIAKA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\242\245\255\245\351\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2337, SHEN_FIELD(id) 4025, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACIAKB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\242\245\255\245\351\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2315, SHEN_FIELD(id) 4026, SHEN_FIELD(minimumDistance) 100, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACGJKA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\270\245\343\245\303\245\255\241\274\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2345, SHEN_FIELD(id) 4027, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 850, SHEN_FIELD(model) "GACGJKB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\270\245\343\245\303\245\255\241\274\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2310, SHEN_FIELD(id) 4028, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 990, SHEN_FIELD(model) "GACOSAA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\265\245\351\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2311, SHEN_FIELD(id) 4029, SHEN_FIELD(minimumDistance) 180, SHEN_FIELD(maximumDistance) 1400, SHEN_FIELD(model) "GACOSAB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\265\245\351\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2331, SHEN_FIELD(id) 4030, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 910, SHEN_FIELD(model) "GACOLAA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\351\245\246\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2330, SHEN_FIELD(id) 4031, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACOLAB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\351\245\246\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2323, SHEN_FIELD(id) 4032, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 950, SHEN_FIELD(model) "GACOPAA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\321\245\244\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2322, SHEN_FIELD(id) 4033, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1100, SHEN_FIELD(model) "GACOPAB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\321\245\244\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2318, SHEN_FIELD(id) 4034, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 810, SHEN_FIELD(model) "GACIWLA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\246\245\353\245\325\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2355, SHEN_FIELD(id) 4035, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 840, SHEN_FIELD(model) "GACIWLB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\246\245\353\245\325\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2326, SHEN_FIELD(id) 4036, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 790, SHEN_FIELD(model) "GACGJFA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\270\245\247\245\325\245\352\241\274\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2353, SHEN_FIELD(id) 4037, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 840, SHEN_FIELD(model) "GACGJFB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\270\245\247\245\325\245\352\241\274\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2334, SHEN_FIELD(id) 4038, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACIKAA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\253\245\262\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2339, SHEN_FIELD(id) 4039, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 810, SHEN_FIELD(model) "GACIKAB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\253\245\262\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2319, SHEN_FIELD(id) 4040, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 850, SHEN_FIELD(model) "GACGSNA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\267\245\345\245\363\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2327, SHEN_FIELD(id) 4041, SHEN_FIELD(minimumDistance) 290, SHEN_FIELD(maximumDistance) 990, SHEN_FIELD(model) "GACGSNB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\267\245\345\245\363\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2335, SHEN_FIELD(id) 4042, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "GACILIA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\352\245\252\245\363\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2347, SHEN_FIELD(id) 4043, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACILIB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\352\245\252\245\363\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2361, SHEN_FIELD(id) 4044, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "GACIDUA",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\307\245\345\245\351\245\353\243\323", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2825, SHEN_FIELD(id) 4045, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 1150, SHEN_FIELD(model) "GACIDUB",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\307\245\345\245\351\245\353\243\307", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2343, SHEN_FIELD(id) 4046, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 920, SHEN_FIELD(model) "GACRNT1",
        SHEN_FIELD(label) "\245\312\245\244\245\304\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2348, SHEN_FIELD(id) 4047, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACRNT2",
        SHEN_FIELD(label) "\245\312\245\244\245\304\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2872, SHEN_FIELD(id) 4048, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "GACRSO1",
        SHEN_FIELD(label) "\245\275\245\313\245\303\245\257\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2561, SHEN_FIELD(id) 4049, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 830, SHEN_FIELD(model) "GACRSO2",
        SHEN_FIELD(label) "\245\275\245\313\245\303\245\257\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2563, SHEN_FIELD(id) 4050, SHEN_FIELD(minimumDistance) 180, SHEN_FIELD(maximumDistance) 660, SHEN_FIELD(model) "GACRSSC",
        SHEN_FIELD(label) "\245\271\241\274\245\321\241\274\245\275\245\313\245\303\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2568, SHEN_FIELD(id) 4051, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 840, SHEN_FIELD(model) "GACRTL1",
        SHEN_FIELD(label) "\245\306\245\244\245\353\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2569, SHEN_FIELD(id) 4052, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 840, SHEN_FIELD(model) "GACRNUC",
        SHEN_FIELD(label) "\245\312\245\303\245\257\245\353\245\272", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2579, SHEN_FIELD(id) 4053, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 780, SHEN_FIELD(model) "GACRBAK",
        SHEN_FIELD(label) "\245\320\241\274\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2578, SHEN_FIELD(id) 4054, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "GACRBIN",
        SHEN_FIELD(label) "\245\323\241\274\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2576, SHEN_FIELD(id) 4055, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 750, SHEN_FIELD(model) "GACRFNG",
        SHEN_FIELD(label) "\245\325\245\241\245\363\245\260", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2570, SHEN_FIELD(id) 4056, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 790, SHEN_FIELD(model) "GACRAMY",
        SHEN_FIELD(label) "\245\250\245\337\245\243", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2577, SHEN_FIELD(id) 4057, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACRESP",
        SHEN_FIELD(label) "\245\250\245\271\245\324\245\252", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2562, SHEN_FIELD(id) 4058, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 630, SHEN_FIELD(model) "GACRMET",
        SHEN_FIELD(label) "\245\341\245\277\245\353\245\275\245\313\245\303\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2571, SHEN_FIELD(id) 4059, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 880, SHEN_FIELD(model) "GACREGG",
        SHEN_FIELD(label) "\245\250\245\303\245\260\245\336\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2362, SHEN_FIELD(id) 4060, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACK6HO",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2366, SHEN_FIELD(id) 4061, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACK6HH",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2365, SHEN_FIELD(id) 4062, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACK6HY",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2363, SHEN_FIELD(id) 4063, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACK6HB",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\243\264", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2367, SHEN_FIELD(id) 4064, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACK6HG",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\243\265", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2823, SHEN_FIELD(id) 4065, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACK6HK",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\243\307", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2572, SHEN_FIELD(id) 4066, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 740, SHEN_FIELD(model) "GACK6SP",
        SHEN_FIELD(label) "\245\271\245\332\241\274\245\271\245\317\245\352\245\242\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2833, SHEN_FIELD(id) 4067, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 790, SHEN_FIELD(model) "GACRLOP",
        SHEN_FIELD(label) "\245\353\241\274\245\321\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2350, SHEN_FIELD(id) 4068, SHEN_FIELD(minimumDistance) 400, SHEN_FIELD(maximumDistance) 1360, SHEN_FIELD(model) "GACR0BN",
        SHEN_FIELD(label) "\245\323\245\363\245\272\245\323\241\274\245\363\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2834, SHEN_FIELD(id) 4069, SHEN_FIELD(minimumDistance) 400, SHEN_FIELD(maximumDistance) 1360, SHEN_FIELD(model) "GACR0B2",
        SHEN_FIELD(label) "\245\323\245\363\245\272\245\323\241\274\245\363\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2835, SHEN_FIELD(id) 4070, SHEN_FIELD(minimumDistance) 400, SHEN_FIELD(maximumDistance) 1360, SHEN_FIELD(model) "GACR0B3",
        SHEN_FIELD(label) "\245\323\245\363\245\272\245\323\241\274\245\363\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2600, SHEN_FIELD(id) 4071, SHEN_FIELD(minimumDistance) 400, SHEN_FIELD(maximumDistance) 1360, SHEN_FIELD(model) "GACRGB1",
        SHEN_FIELD(label) "\243\302\241\246\243\302\245\246\245\353\245\310\245\351", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2818, SHEN_FIELD(id) 4072, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 680, SHEN_FIELD(model) "GACODG1",
        SHEN_FIELD(label) "\245\335\245\301", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2816, SHEN_FIELD(id) 4073, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 700, SHEN_FIELD(model) "GACODG2",
        SHEN_FIELD(label) "\245\270\245\347\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2817, SHEN_FIELD(id) 4074, SHEN_FIELD(minimumDistance) 260, SHEN_FIELD(maximumDistance) 780, SHEN_FIELD(model) "GACODG3",
        SHEN_FIELD(label) "\245\261\245\352\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2623, SHEN_FIELD(id) 4075, SHEN_FIELD(minimumDistance) 350, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "GACOCT2",
        SHEN_FIELD(label) "\245\257\245\355", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2622, SHEN_FIELD(id) 4076, SHEN_FIELD(minimumDistance) 350, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "GACOCAT",
        SHEN_FIELD(label) "\245\310\245\351", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2613, SHEN_FIELD(id) 4077, SHEN_FIELD(minimumDistance) 290, SHEN_FIELD(maximumDistance) 710, SHEN_FIELD(model) "GACO1SH",
        SHEN_FIELD(label) "\245\323\245\303\245\260\245\246\245\243\245\352\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2612, SHEN_FIELD(id) 4078, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 600, SHEN_FIELD(model) "GACOKSH",
        SHEN_FIELD(label) "\245\352\245\310\245\353\245\246\245\243\245\352\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2614, SHEN_FIELD(id) 4079, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 780, SHEN_FIELD(model) "GACOOSH",
        SHEN_FIELD(label) "\245\246\245\243\245\352\241\274\245\272", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2606, SHEN_FIELD(id) 4080, SHEN_FIELD(minimumDistance) 320, SHEN_FIELD(maximumDistance) 680, SHEN_FIELD(model) "GACO1IR",
        SHEN_FIELD(label) "\245\323\245\303\245\260\245\325\245\243\245\352\245\303\245\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2604, SHEN_FIELD(id) 4081, SHEN_FIELD(minimumDistance) 390, SHEN_FIELD(maximumDistance) 710, SHEN_FIELD(model) "GACOKIR",
        SHEN_FIELD(label) "\245\352\245\310\245\353\245\325\245\243\245\352\245\303\245\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2615, SHEN_FIELD(id) 4082, SHEN_FIELD(minimumDistance) 320, SHEN_FIELD(maximumDistance) 780, SHEN_FIELD(model) "GACOOIR",
        SHEN_FIELD(label) "\245\325\245\243\245\352\245\303\245\327\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2616, SHEN_FIELD(id) 4083, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 770, SHEN_FIELD(model) "GACO1HA",
        SHEN_FIELD(label) "\245\301\245\303\245\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2621, SHEN_FIELD(id) 4084, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 770, SHEN_FIELD(model) "GACOHA2",
        SHEN_FIELD(label) "\245\351\245\303\245\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2620, SHEN_FIELD(id) 4085, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 770, SHEN_FIELD(model) "GACOHA3",
        SHEN_FIELD(label) "\245\335\245\303\245\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2352, SHEN_FIELD(id) 4086, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 770, SHEN_FIELD(model) "GACOHA4",
        SHEN_FIELD(label) "\245\324\245\303\245\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2824, SHEN_FIELD(id) 4087, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 750, SHEN_FIELD(model) "GACO1YG",
        SHEN_FIELD(label) "\245\341\245\352\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2819, SHEN_FIELD(id) 4088, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 700, SHEN_FIELD(model) "GACO1US",
        SHEN_FIELD(label) "\245\324\245\347\245\363\245\277", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2357, SHEN_FIELD(id) 4089, SHEN_FIELD(minimumDistance) 180, SHEN_FIELD(maximumDistance) 710, SHEN_FIELD(model) "GACRMYU",
        SHEN_FIELD(label) "\245\337\245\343\245\246", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2342, SHEN_FIELD(id) 4090, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 690, SHEN_FIELD(model) "GACRCKN",
        SHEN_FIELD(label) "\245\301\245\255\245\363\245\354\245\303\245\260", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2587, SHEN_FIELD(id) 4091, SHEN_FIELD(minimumDistance) 350, SHEN_FIELD(maximumDistance) 980, SHEN_FIELD(model) "GACRALF",
        SHEN_FIELD(label) "\245\301\245\343\245\252\241\365\245\324\245\242\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2359, SHEN_FIELD(id) 4092, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 850, SHEN_FIELD(model) "GACRRIS",
        SHEN_FIELD(label) "\245\352\245\271\245\277\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2586, SHEN_FIELD(id) 4093, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 750, SHEN_FIELD(model) "GACRARX",
        SHEN_FIELD(label) "\245\242\245\354\245\303\245\257\245\271\245\255\245\303\245\311", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2341, SHEN_FIELD(id) 4094, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "GACR0OP",
        SHEN_FIELD(label) "\245\252\245\321\245\252\245\321", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2340, SHEN_FIELD(id) 4095, SHEN_FIELD(minimumDistance) 380, SHEN_FIELD(maximumDistance) 1060, SHEN_FIELD(model) "GACR0O2",
        SHEN_FIELD(label) "\245\270\245\247\245\303\245\310\245\252\245\321\245\252\245\321", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2617, SHEN_FIELD(id) 4096, SHEN_FIELD(minimumDistance) 260, SHEN_FIELD(maximumDistance) 880, SHEN_FIELD(model) "GACRPOP",
        SHEN_FIELD(label) "\245\335\245\303\245\335\241\274\245\272", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2832, SHEN_FIELD(id) 4097, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 840, SHEN_FIELD(model) "GACRCOB",
        SHEN_FIELD(label) "\245\263\245\320\245\323\241\274\245\301", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2826, SHEN_FIELD(id) 4098, SHEN_FIELD(minimumDistance) 270, SHEN_FIELD(maximumDistance) 890, SHEN_FIELD(model) "GACRIDA",
        SHEN_FIELD(label) "\245\242\245\244\245\300\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2591, SHEN_FIELD(id) 4099, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 540, SHEN_FIELD(model) "GACRSHP",
        SHEN_FIELD(label) "\245\267\245\347\245\303\245\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2848, SHEN_FIELD(id) 4100, SHEN_FIELD(minimumDistance) 350, SHEN_FIELD(maximumDistance) 1050, SHEN_FIELD(model) "GACR016",
        SHEN_FIELD(label) "\245\330\245\323\241\274\245\334\245\340\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2849, SHEN_FIELD(id) 4101, SHEN_FIELD(minimumDistance) 350, SHEN_FIELD(maximumDistance) 1050, SHEN_FIELD(model) "GACK66B",
        SHEN_FIELD(label) "\245\330\245\323\241\274\245\334\245\340\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2850, SHEN_FIELD(id) 4102, SHEN_FIELD(minimumDistance) 350, SHEN_FIELD(maximumDistance) 1050, SHEN_FIELD(model) "GACK66R",
        SHEN_FIELD(label) "\245\330\245\323\241\274\245\334\245\340\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2589, SHEN_FIELD(id) 4103, SHEN_FIELD(minimumDistance) 400, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACRMOB",
        SHEN_FIELD(label) "\243\315\243\317\243\302\243\317", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2590, SHEN_FIELD(id) 4104, SHEN_FIELD(minimumDistance) 330, SHEN_FIELD(maximumDistance) 850, SHEN_FIELD(model) "GACRROB",
        SHEN_FIELD(label) "\243\322\243\317\243\302\243\317", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2356, SHEN_FIELD(id) 4105, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 750, SHEN_FIELD(model) "GACRBON",
        SHEN_FIELD(label) "\245\334\245\312\245\363\245\266\245\326\245\351\245\266\241\274\245\272", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2582, SHEN_FIELD(id) 4106, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "GACH5BK",
        SHEN_FIELD(label) "\245\271\245\257\241\274\245\277\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2583, SHEN_FIELD(id) 4107, SHEN_FIELD(minimumDistance) 290, SHEN_FIELD(maximumDistance) 690, SHEN_FIELD(model) "KAB02FS",
        SHEN_FIELD(label) "\245\323\245\270\245\315\245\271\245\320\245\244\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2573, SHEN_FIELD(id) 4108, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 890, SHEN_FIELD(model) "GACO1K2",
        SHEN_FIELD(label) "\245\257\241\274\245\332\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2595, SHEN_FIELD(id) 4109, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 890, SHEN_FIELD(model) "GACO1K3",
        SHEN_FIELD(label) "\245\257\241\274\245\332\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2865, SHEN_FIELD(id) 4110, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 890, SHEN_FIELD(model) "GACO1K7",
        SHEN_FIELD(label) "\245\257\241\274\245\332\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2866, SHEN_FIELD(id) 4111, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 890, SHEN_FIELD(model) "GACO1K8",
        SHEN_FIELD(label) "\245\257\241\274\245\332\243\264", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2867, SHEN_FIELD(id) 4112, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 890, SHEN_FIELD(model) "GACO1K9",
        SHEN_FIELD(label) "\245\257\241\274\245\332\243\265", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2857, SHEN_FIELD(id) 4113, SHEN_FIELD(minimumDistance) 270, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "KABK6DY",
        SHEN_FIELD(label) "\245\333\241\274\245\315\245\303\245\310", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2574, SHEN_FIELD(id) 4114, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 920, SHEN_FIELD(model) "GACO1K1",
        SHEN_FIELD(label) "\245\357\245\264\245\363\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2859, SHEN_FIELD(id) 4115, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 920, SHEN_FIELD(model) "GACO1K4",
        SHEN_FIELD(label) "\245\357\245\264\245\363\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2858, SHEN_FIELD(id) 4116, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 920, SHEN_FIELD(model) "GACO1K5",
        SHEN_FIELD(label) "\245\357\245\264\245\363\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2864, SHEN_FIELD(id) 4117, SHEN_FIELD(minimumDistance) 280, SHEN_FIELD(maximumDistance) 920, SHEN_FIELD(model) "GACO1K6",
        SHEN_FIELD(label) "\245\357\245\264\245\363\243\264", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2580, SHEN_FIELD(id) 4118, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 630, SHEN_FIELD(model) "GACGTRX",
        SHEN_FIELD(label) "\245\310\245\351\245\303\245\257\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2364, SHEN_FIELD(id) 4119, SHEN_FIELD(minimumDistance) 270, SHEN_FIELD(maximumDistance) 830, SHEN_FIELD(model) "GACO1T1",
        SHEN_FIELD(label) "\245\310\245\351\245\303\245\257\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2594, SHEN_FIELD(id) 4120, SHEN_FIELD(minimumDistance) 260, SHEN_FIELD(maximumDistance) 790, SHEN_FIELD(model) "GACO1T2",
        SHEN_FIELD(label) "\245\310\245\351\245\303\245\257\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2351, SHEN_FIELD(id) 4121, SHEN_FIELD(minimumDistance) 270, SHEN_FIELD(maximumDistance) 830, SHEN_FIELD(model) "GACH5TK",
        SHEN_FIELD(label) "\245\310\245\351\245\303\245\257\243\264", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2575, SHEN_FIELD(id) 4122, SHEN_FIELD(minimumDistance) 290, SHEN_FIELD(maximumDistance) 870, SHEN_FIELD(model) "GACH5TR",
        SHEN_FIELD(label) "\245\325\241\274\245\311\245\253\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2581, SHEN_FIELD(id) 4123, SHEN_FIELD(minimumDistance) 320, SHEN_FIELD(maximumDistance) 920, SHEN_FIELD(model) "GACS5BS",
        SHEN_FIELD(label) "\245\320\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2608, SHEN_FIELD(id) 4124, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "GACO1FO",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\245\352\245\325\245\310", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2828, SHEN_FIELD(id) 4125, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACS5N1",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\243\316\243\357\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2829, SHEN_FIELD(id) 4126, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACS5N2",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\243\316\243\357\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2830, SHEN_FIELD(id) 4127, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACS5N3",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\243\316\243\357\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2831, SHEN_FIELD(id) 4128, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACS5N4",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\243\316\243\357\243\264", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2836, SHEN_FIELD(id) 4129, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACS5N5",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\243\316\243\357\243\265", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2837, SHEN_FIELD(id) 4130, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACS5FR",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\245\352\245\325\245\310\300\326", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2838, SHEN_FIELD(id) 4131, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 1000, SHEN_FIELD(model) "GACS5FB",
        SHEN_FIELD(label) "\245\325\245\251\241\274\245\257\245\352\245\325\245\310\300\304", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2611, SHEN_FIELD(id) 4132, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 610, SHEN_FIELD(model) "GACO1KU",
        SHEN_FIELD(label) "\245\257\245\354\241\274\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2305, SHEN_FIELD(id) 4133, SHEN_FIELD(minimumDistance) 270, SHEN_FIELD(maximumDistance) 670, SHEN_FIELD(model) "GACGSHE",
        SHEN_FIELD(label) "\245\267\245\247\245\363\245\340\245\263\245\363\245\306\245\312", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2851, SHEN_FIELD(id) 4134, SHEN_FIELD(minimumDistance) 270, SHEN_FIELD(maximumDistance) 670, SHEN_FIELD(model) "GACO1KO",
        SHEN_FIELD(label) "\245\263\245\363\245\306\245\312", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2619, SHEN_FIELD(id) 4135, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 690, SHEN_FIELD(model) "GACO1KB",
        SHEN_FIELD(label) "\314\332\310\242", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2344, SHEN_FIELD(id) 4136, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 740, SHEN_FIELD(model) "GACH5IK",
        SHEN_FIELD(label) "\245\244\245\253\245\352", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2601, SHEN_FIELD(id) 4137, SHEN_FIELD(minimumDistance) 400, SHEN_FIELD(maximumDistance) 960, SHEN_FIELD(model) "GACO1FE",
        SHEN_FIELD(label) "\245\325\245\247\245\352\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2603, SHEN_FIELD(id) 4138, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "GACO1KJ",
        SHEN_FIELD(label) "\302\311\316\330", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2336, SHEN_FIELD(id) 4139, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "GACH5UK",
        SHEN_FIELD(label) "\244\246\244\255\244\357", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2354, SHEN_FIELD(id) 4140, SHEN_FIELD(minimumDistance) 450, SHEN_FIELD(maximumDistance) 1350, SHEN_FIELD(model) "GACGXBN",
        SHEN_FIELD(label) "\243\330\245\334\245\277\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2346, SHEN_FIELD(id) 4141, SHEN_FIELD(minimumDistance) 450, SHEN_FIELD(maximumDistance) 1350, SHEN_FIELD(model) "GACGYBN",
        SHEN_FIELD(label) "\243\331\245\334\245\277\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2360, SHEN_FIELD(id) 4142, SHEN_FIELD(minimumDistance) 450, SHEN_FIELD(maximumDistance) 1350, SHEN_FIELD(model) "GACGABN",
        SHEN_FIELD(label) "\243\301\245\334\245\277\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2338, SHEN_FIELD(id) 4143, SHEN_FIELD(minimumDistance) 450, SHEN_FIELD(maximumDistance) 1350, SHEN_FIELD(model) "GACGBBN",
        SHEN_FIELD(label) "\243\302\245\334\245\277\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2854, SHEN_FIELD(id) 4144, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 660, SHEN_FIELD(model) "GACGCRY",
        SHEN_FIELD(label) "\245\301\245\247\245\352\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2839, SHEN_FIELD(id) 4145, SHEN_FIELD(minimumDistance) 270, SHEN_FIELD(maximumDistance) 730, SHEN_FIELD(model) "GACH5JK",
        SHEN_FIELD(label) "\245\337\245\313\245\270\245\345\241\274\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2847, SHEN_FIELD(id) 4146, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "GACH5SL",
        SHEN_FIELD(label) "\245\337\245\313\245\271\245\355\245\303\245\310", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2845, SHEN_FIELD(id) 4147, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 830, SHEN_FIELD(model) "GACM400",
        SHEN_FIELD(label) "\245\337\245\313\245\277\245\244\245\310\245\353", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2852, SHEN_FIELD(id) 4148, SHEN_FIELD(minimumDistance) 180, SHEN_FIELD(maximumDistance) 660, SHEN_FIELD(model) "GACGSPH",
        SHEN_FIELD(label) "\245\337\245\313\245\317\245\352\245\242\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2596, SHEN_FIELD(id) 4149, SHEN_FIELD(minimumDistance) 230, SHEN_FIELD(maximumDistance) 720, SHEN_FIELD(model) "GACS5HG",
        SHEN_FIELD(label) "\245\337\245\313\245\317\245\363\245\260\245\252\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2853, SHEN_FIELD(id) 4150, SHEN_FIELD(minimumDistance) 190, SHEN_FIELD(maximumDistance) 850, SHEN_FIELD(model) "GACG360",
        SHEN_FIELD(label) "\243\322\241\335\243\263\243\266\243\260", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2846, SHEN_FIELD(id) 4151, SHEN_FIELD(minimumDistance) 220, SHEN_FIELD(maximumDistance) 1120, SHEN_FIELD(model) "GACGDT1",
        SHEN_FIELD(label) "\245\337\245\313\245\300\241\274\245\304", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2599, SHEN_FIELD(id) 4152, SHEN_FIELD(minimumDistance) 240, SHEN_FIELD(maximumDistance) 820, SHEN_FIELD(model) "GACS5EQ",
        SHEN_FIELD(label) "\245\337\245\313\243\321\243\324\243\305", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2844, SHEN_FIELD(id) 4153, SHEN_FIELD(minimumDistance) 210, SHEN_FIELD(maximumDistance) 860, SHEN_FIELD(model) "GACGBIR",
        SHEN_FIELD(label) "\245\337\245\313\245\323\245\352\245\344\241\274\245\311", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2868, SHEN_FIELD(id) 4154, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "SYOGMD1",
        SHEN_FIELD(label) "\276\276\273\263\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2869, SHEN_FIELD(id) 4155, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "SYOGMD5",
        SHEN_FIELD(label) "\273\260\304\315\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2841, SHEN_FIELD(id) 4160, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "SYOGSH0",
        SHEN_FIELD(label) "\245\317\245\352\245\242\241\274\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2840, SHEN_FIELD(id) 4161, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "SYOGHG0",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2584, SHEN_FIELD(id) 4162, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SPBK6RE",
        SHEN_FIELD(label) "\245\271\241\274\245\321\241\274\245\334\241\274\245\353\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2566, SHEN_FIELD(id) 4163, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SPBK6BR",
        SHEN_FIELD(label) "\245\271\241\274\245\321\241\274\245\334\241\274\245\353\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2567, SHEN_FIELD(id) 4164, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SPBK6YE",
        SHEN_FIELD(label) "\245\271\241\274\245\321\241\274\245\334\241\274\245\353\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2585, SHEN_FIELD(id) 4165, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6WH",
        SHEN_FIELD(label) "\245\265\245\244\245\263\245\355\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2564, SHEN_FIELD(id) 4166, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6BK",
        SHEN_FIELD(label) "\245\265\245\244\245\263\245\355\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2565, SHEN_FIELD(id) 4167, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6RE",
        SHEN_FIELD(label) "\245\265\245\244\245\263\245\355\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2873, SHEN_FIELD(id) 4168, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "6GA1KEJ",
        SHEN_FIELD(label) "\262\320\261\352\274\371", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2874, SHEN_FIELD(id) 4169, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "6GA5000",
        SHEN_FIELD(label) "\305\364\302\346", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2875, SHEN_FIELD(id) 4170, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GA5001",
        SHEN_FIELD(label) "\277\345\276\345\300\270\263\350\301\245", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2876, SHEN_FIELD(id) 4171, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GA5003",
        SHEN_FIELD(label) "\270\325\265\335", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2877, SHEN_FIELD(id) 4172, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GA5002",
        SHEN_FIELD(label) "\266\327", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2878, SHEN_FIELD(id) 4173, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GAAJET",
        SHEN_FIELD(label) "\245\270\245\343\245\363\245\334\245\270\245\247\245\303\245\310\265\241", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2879, SHEN_FIELD(id) 4174, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GATCHN",
        SHEN_FIELD(label) "\245\301\245\247\245\363\245\275\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3072, SHEN_FIELD(id) 4175, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GAAKEN",
        SHEN_FIELD(label) "\300\304\316\265\305\341", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3073, SHEN_FIELD(id) 4176, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GAAMZ1",
        SHEN_FIELD(label) "\270\274\311\360\277\345\276\354", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3074, SHEN_FIELD(id) 4177, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "6GAAMZ3",
        SHEN_FIELD(label) "\307\362\270\327\277\345\276\354", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3075, SHEN_FIELD(id) 4178, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "6GAAMZ4",
        SHEN_FIELD(label) "\274\353\277\375\277\345\276\354", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3076, SHEN_FIELD(id) 4179, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "6GAAMZ2",
        SHEN_FIELD(label) "\300\304\316\266\277\345\276\354", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3077, SHEN_FIELD(id) 4180, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "7GATOUT",
        SHEN_FIELD(label) "\245\242\245\246\245\310\245\351\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3078, SHEN_FIELD(id) 4181, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "7GAHF14",
        SHEN_FIELD(label) "\245\242\245\325\245\277\241\274\245\320\241\274\245\312\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3079, SHEN_FIELD(id) 4182, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "4GA1TR1",
        SHEN_FIELD(label) "\243\261\243\270\245\333\245\244\245\351\241\274\243\261", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3080, SHEN_FIELD(id) 4183, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "4GA9TR2",
        SHEN_FIELD(label) "\243\261\243\270\245\333\245\244\245\351\241\274\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3081, SHEN_FIELD(id) 4184, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "4GA1TR3",
        SHEN_FIELD(label) "\243\261\243\270\245\333\245\244\245\351\241\274\243\263", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3082, SHEN_FIELD(id) 4185, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "4GA1TR4",
        SHEN_FIELD(label) "\243\261\243\270\245\333\245\244\245\351\241\274\243\264", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3083, SHEN_FIELD(id) 4186, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "4GA1TR5",
        SHEN_FIELD(label) "\243\261\243\270\245\333\245\244\245\351\241\274\243\265", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3084, SHEN_FIELD(id) 4187, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "4GA1CH1",
        SHEN_FIELD(label) "\245\306\245\255\245\265\245\271\245\333\241\274\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3085, SHEN_FIELD(id) 4188, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "4GA1CH2",
        SHEN_FIELD(label) "\245\357\245\244\245\353\245\311\245\355\241\274\245\272", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3086, SHEN_FIELD(id) 4189, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "4GA1CH3",
        SHEN_FIELD(label) "\245\336\245\303\245\311\245\326\245\353", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3087, SHEN_FIELD(id) 4190, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "4GA1CH4",
        SHEN_FIELD(label) "\245\340\241\274\245\363\245\351\245\244\245\310", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3088, SHEN_FIELD(id) 4191, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "5GARJAY",
        SHEN_FIELD(label) "\245\270\245\247\245\244", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3089, SHEN_FIELD(id) 4192, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "5GASALN",
        SHEN_FIELD(label) "\245\242\245\351\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3090, SHEN_FIELD(id) 4193, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "5GASLNA",
        SHEN_FIELD(label) "\245\352\245\312", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3091, SHEN_FIELD(id) 4194, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "5GASTRN",
        SHEN_FIELD(label) "\245\277\245\355\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3092, SHEN_FIELD(id) 4195, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "5GASTRS",
        SHEN_FIELD(label) "\245\306\245\354\245\267\245\263\245\357", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3093, SHEN_FIELD(id) 4196, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "3GA1JS2",
        SHEN_FIELD(label) "\245\261\245\244\245\310", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3094, SHEN_FIELD(id) 4197, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "3GA1JS4",
        SHEN_FIELD(label) "\245\336\245\303\245\310", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3095, SHEN_FIELD(id) 4198, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "3GA1JS5",
        SHEN_FIELD(label) "\245\336\245\303\245\257\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3096, SHEN_FIELD(id) 4199, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "3GA1JS6",
        SHEN_FIELD(label) "\245\351\245\244\245\242\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3097, SHEN_FIELD(id) 4200, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "3GA1JS3",
        SHEN_FIELD(label) "\245\271\245\321\245\244\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3098, SHEN_FIELD(id) 4201, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "3GA1JS1",
        SHEN_FIELD(label) "\245\346\245\255", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3099, SHEN_FIELD(id) 4202, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GAS2A1",
        SHEN_FIELD(label) "\267\353\276\353\241\241\276\275\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3100, SHEN_FIELD(id) 4203, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASLU1",
        SHEN_FIELD(label) "\245\351\245\246\241\246\245\301\245\247\245\363\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3101, SHEN_FIELD(id) 4204, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GAS2P1",
        SHEN_FIELD(label) "\245\321\245\244\241\246\245\301\245\247\245\363\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3102, SHEN_FIELD(id) 4205, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASWF1",
        SHEN_FIELD(label) "\245\246\245\353\245\325\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3103, SHEN_FIELD(id) 4206, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASJF1",
        SHEN_FIELD(label) "\245\270\245\247\245\325\245\352\241\274\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3104, SHEN_FIELD(id) 4207, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASSR1",
        SHEN_FIELD(label) "\245\265\245\351\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3105, SHEN_FIELD(id) 4208, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "1GAS2J1",
        SHEN_FIELD(label) "\245\270\245\343\245\303\245\255\241\274\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3106, SHEN_FIELD(id) 4209, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "1GASKG1",
        SHEN_FIELD(label) "\261\306\264\335\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3107, SHEN_FIELD(id) 4210, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASSN1",
        SHEN_FIELD(label) "\275\330\304\353\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3108, SHEN_FIELD(id) 4211, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASLN1",
        SHEN_FIELD(label) "\245\352\245\252\245\363\243\261\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3109, SHEN_FIELD(id) 4212, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GAS2A2",
        SHEN_FIELD(label) "\267\353\276\353\241\241\276\275\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3110, SHEN_FIELD(id) 4213, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASLU2",
        SHEN_FIELD(label) "\245\351\245\246\241\246\245\301\245\247\245\363\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3111, SHEN_FIELD(id) 4214, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GAS2P2",
        SHEN_FIELD(label) "\245\321\245\244\241\246\245\301\245\247\245\363\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3112, SHEN_FIELD(id) 4215, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASWF2",
        SHEN_FIELD(label) "\245\246\245\353\245\325\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3113, SHEN_FIELD(id) 4216, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASJF2",
        SHEN_FIELD(label) "\245\270\245\247\245\325\245\352\241\274\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3114, SHEN_FIELD(id) 4217, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASSR2",
        SHEN_FIELD(label) "\245\265\245\351\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3115, SHEN_FIELD(id) 4218, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "1GAS2J2",
        SHEN_FIELD(label) "\245\270\245\343\245\303\245\255\241\274\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3116, SHEN_FIELD(id) 4219, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "1GASKG2",
        SHEN_FIELD(label) "\261\306\264\335\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3117, SHEN_FIELD(id) 4220, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASSN2",
        SHEN_FIELD(label) "\275\330\304\353\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3118, SHEN_FIELD(id) 4221, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "1GASLN2",
        SHEN_FIELD(label) "\245\352\245\252\245\363\243\262\243\320", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3119, SHEN_FIELD(id) 4222, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GARTAN",
        SHEN_FIELD(label) "\245\277\245\363\245\260\245\351\245\340", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3120, SHEN_FIELD(id) 4223, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAFBT1",
        SHEN_FIELD(label) "\245\326\245\351\245\303\245\310\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3121, SHEN_FIELD(id) 4224, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GARTEM",
        SHEN_FIELD(label) "\245\306\245\340\245\270\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3122, SHEN_FIELD(id) 4225, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAETE1",
        SHEN_FIELD(label) "\243\261\243\260\241\277\243\270\243\260", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3123, SHEN_FIELD(id) 4226, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAGTM1",
        SHEN_FIELD(label) "\245\306\245\340\245\270\245\363\273\356\272\356\267\277", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3124, SHEN_FIELD(id) 4227, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAGAS1",
        SHEN_FIELD(label) "\245\242\245\325\245\241\241\274\245\340\245\311\241\246\243\323", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3125, SHEN_FIELD(id) 4228, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "2GADAB1",
        SHEN_FIELD(label) "\245\242\245\325\245\241\241\274\245\340\245\311\241\246\243\302", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3126, SHEN_FIELD(id) 4229, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "2GAFAC1",
        SHEN_FIELD(label) "\245\242\245\325\245\241\241\274\245\340\245\311\241\246\243\303", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3127, SHEN_FIELD(id) 4230, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GABCY1",
        SHEN_FIELD(label) "\245\265\245\244\245\325\245\241\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3128, SHEN_FIELD(id) 4231, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GABRA1",
        SHEN_FIELD(label) "\245\351\245\244\245\307\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3129, SHEN_FIELD(id) 4232, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAFGR1",
        SHEN_FIELD(label) "\245\260\245\352\245\271\245\334\245\303\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3130, SHEN_FIELD(id) 4233, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAGST1",
        SHEN_FIELD(label) "\245\267\245\345\245\277\245\244\245\363\245\334\245\303\245\257", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3131, SHEN_FIELD(id) 4234, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GADDO1",
        SHEN_FIELD(label) "\245\311\245\353\245\311\245\354\245\244", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3132, SHEN_FIELD(id) 4235, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAEBO1",
        SHEN_FIELD(label) "\245\320\245\353\241\246\245\320\245\311\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3133, SHEN_FIELD(id) 4236, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GACBA1",
        SHEN_FIELD(label) "\245\320\245\353\241\246\245\261\245\355\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3134, SHEN_FIELD(id) 4237, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GABBR1",
        SHEN_FIELD(label) "\245\320\245\353\241\246\245\320\245\355\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3135, SHEN_FIELD(id) 4238, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "2GACFE1",
        SHEN_FIELD(label) "\245\325\245\247\245\244\241\246\245\244\245\247\245\363\243\304", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3328, SHEN_FIELD(id) 4239, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "2GACFE2",
        SHEN_FIELD(label) "\245\325\245\247\245\244\241\246\245\244\245\247\245\363\243\362", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3329, SHEN_FIELD(id) 4240, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GACFE3",
        SHEN_FIELD(label) "\245\325\245\247\245\244\241\246\245\317\245\244\245\321\241\274", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3330, SHEN_FIELD(id) 4241, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GADSP1",
        SHEN_FIELD(label) "\245\271\245\332\245\267\245\315\245\325", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3331, SHEN_FIELD(id) 4242, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GAEAM1",
        SHEN_FIELD(label) "\245\250\245\363\245\270\245\247\245\351\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3332, SHEN_FIELD(id) 4243, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "2GACAJ1",
        SHEN_FIELD(label) "\245\242\245\270\245\340", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4244, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6BK",
        SHEN_FIELD(label) "\243\301\243\302\243\262\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4245, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6RE",
        SHEN_FIELD(label) "\245\242\245\246\245\310\245\351\245\363\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4246, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6BK",
        SHEN_FIELD(label) "\245\317\245\352\245\242\241\274\307\247\304\352\275\361\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4247, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6RE",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\307\247\304\352\275\361\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4248, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "SYOGSH0",
        SHEN_FIELD(label) "\243\263\300\351\313\347\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4249, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "SYOGHG0",
        SHEN_FIELD(label) "\243\261\313\374\313\347\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4250, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SPBK6RE",
        SHEN_FIELD(label) "\243\265\313\374\313\347\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4251, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SPBK6BR",
        SHEN_FIELD(label) "\243\262\243\260\313\374\313\347\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4252, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SPBK6YE",
        SHEN_FIELD(label) "\243\265\243\260\313\374\313\347\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2304, SHEN_FIELD(id) 4253, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SAIK6WH",
        SHEN_FIELD(label) "\243\261\243\260\243\260\313\374\313\347\307\247\304\352\275\361", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2871, SHEN_FIELD(id) 4254, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "AFBR701",
        SHEN_FIELD(label) "\245\337\245\313\243\301\243\302\243\262", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2870, SHEN_FIELD(id) 4255, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "OUTR701",
        SHEN_FIELD(label) "\245\337\245\313\245\242\245\246\245\310\245\351\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 3343, SHEN_FIELD(id) 4256, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "GACRDUC",
        SHEN_FIELD(label) "\245\255\245\303\245\272\245\307\245\345\245\351\245\353\243\303", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2860, SHEN_FIELD(id) 4257, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "DENI2T1",
        SHEN_FIELD(label) "\303\261\243\261\305\305\303\323", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2855, SHEN_FIELD(id) 4258, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "DENI2T3",
        SHEN_FIELD(label) "\303\261\243\263\305\305\303\323", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2588, SHEN_FIELD(id) 4259, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "MATI200",
        SHEN_FIELD(label) "\245\336\245\303\245\301", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2618, SHEN_FIELD(id) 4260, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "RMP02H1",
        SHEN_FIELD(label) "\305\305\265\345", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2842, SHEN_FIELD(id) 4261, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "HNGS500",
        SHEN_FIELD(label) "\245\317\245\363\245\260\245\252\245\363\243\303\243\304", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2843, SHEN_FIELD(id) 4262, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "SPHS701",
        SHEN_FIELD(label) "\245\317\245\352\245\242\241\274\243\303\243\304", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2609, SHEN_FIELD(id) 4263, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "HOSK2GR",
        SHEN_FIELD(label) "\316\320\300\320", SHEN_FIELD(flags) 0x21U},
    {SHEN_FIELD(icon) 2602, SHEN_FIELD(id) 4264, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "HOSK2RE",
        SHEN_FIELD(label) "\300\326\300\320", SHEN_FIELD(flags) 0x21U},
    {SHEN_FIELD(icon) 2560, SHEN_FIELD(id) 4265, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "HOSK2YE",
        SHEN_FIELD(label) "\262\253\300\320", SHEN_FIELD(flags) 0x21U},
    {SHEN_FIELD(icon) 2597, SHEN_FIELD(id) 4266, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "HOSK2PP",
        SHEN_FIELD(label) "\273\347\300\320", SHEN_FIELD(flags) 0x21U},
    {SHEN_FIELD(icon) 2598, SHEN_FIELD(id) 4267, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "HOSK2BL",
        SHEN_FIELD(label) "\300\304\300\320", SHEN_FIELD(flags) 0x21U},
    {SHEN_FIELD(icon) 2607, SHEN_FIELD(id) 4268, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "HOSK2CL",
        SHEN_FIELD(label) "\306\251\300\320", SHEN_FIELD(flags) 0x21U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4269, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "ZIPOHW3",
        SHEN_FIELD(label) "\245\340\241\274\245\363\245\246\245\353\245\325", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4270, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHW2",
        SHEN_FIELD(label) "\245\364\245\241\245\363\245\321\245\244\245\242", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4271, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHG2",
        SHEN_FIELD(label) "\245\246\245\244\245\363\245\260", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4272, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHW1",
        SHEN_FIELD(label) "\245\270\245\343\245\303\245\253\245\353", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4273, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHM1",
        SHEN_FIELD(label) "\245\340\241\274\245\255\245\343\245\271", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4274, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHS1",
        SHEN_FIELD(label) "\245\265\245\275\245\352", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4275, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHG1",
        SHEN_FIELD(label) "\245\255\245\363\245\260", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4276, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHD1",
        SHEN_FIELD(label) "\245\311\245\351\245\264\245\363", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4277, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHK1",
        SHEN_FIELD(label) "\245\330\245\353\245\340", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4278, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "ZIPOHR1",
        SHEN_FIELD(label) "\245\320\245\351", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4279, SHEN_FIELD(minimumDistance) 130, SHEN_FIELD(maximumDistance) 530, SHEN_FIELD(model) "ZIPOHH1",
        SHEN_FIELD(label) "\245\317\241\274\245\310", SHEN_FIELD(flags) 0x1U},
    {SHEN_FIELD(icon) 2610, SHEN_FIELD(id) 4280, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 900, SHEN_FIELD(model) "ZIPOHG3",
        SHEN_FIELD(label) "\266\330\261\354", SHEN_FIELD(flags) 0x1U},
};

const InventoryItemDefinition photos[] = {
    {SHEN_FIELD(icon) 30, SHEN_FIELD(id) 8006, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS503",
        SHEN_FIELD(label) "\270\266\272\352", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 30, SHEN_FIELD(id) 8007, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS506",
        SHEN_FIELD(label) "\270\266\272\352", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 30, SHEN_FIELD(id) 8008, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS504",
        SHEN_FIELD(label) "\270\266\272\352", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 30, SHEN_FIELD(id) 8009, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS505",
        SHEN_FIELD(label) "\270\266\272\352", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 30, SHEN_FIELD(id) 8010, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS507",
        SHEN_FIELD(label) "\270\266\272\352", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 30, SHEN_FIELD(id) 8011, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS508",
        SHEN_FIELD(label) "\270\266\272\352", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 31, SHEN_FIELD(id) 8005, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS1S1",
        SHEN_FIELD(label) "\277\306\311\343", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 32, SHEN_FIELD(id) 8013, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS1IR",
        SHEN_FIELD(label) "\307\316\267\356\262\310", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 33, SHEN_FIELD(id) 8014, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "SHAS1RS",
        SHEN_FIELD(label) "\315\247\277\315", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 34, SHEN_FIELD(id) 8018, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "PHFS2WO",
        SHEN_FIELD(label) "\245\246\245\251\245\363", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 35, SHEN_FIELD(id) 8012, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "PHFS2BU",
        SHEN_FIELD(label) "\275\250\261\315\244\277\244\301", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 41, SHEN_FIELD(id) 8000, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "PCHN200",
        SHEN_FIELD(label) "\245\271\245\253\245\246\245\310\245\336\245\363", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 36, SHEN_FIELD(id) 8015, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "PHFS2JR",
        SHEN_FIELD(label) "\245\270\245\347\245\244", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 37, SHEN_FIELD(id) 8016, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "PHFS2K2",
        SHEN_FIELD(label) "\267\260", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 37, SHEN_FIELD(id) 8017, SHEN_FIELD(minimumDistance) 540, SHEN_FIELD(maximumDistance) 1260, SHEN_FIELD(model) "PHFS2K1",
        SHEN_FIELD(label) "\267\260", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 39, SHEN_FIELD(id) 8002, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "PSF72AW",
        SHEN_FIELD(label) "\245\355\245\303\245\311", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 38, SHEN_FIELD(id) 8001, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "PSF6200",
        SHEN_FIELD(label) "\245\260\245\354\245\303\245\260", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 40, SHEN_FIELD(id) 8003, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "PSF3200",
        SHEN_FIELD(label) "\275\325\261\355", SHEN_FIELD(flags) 0x0U},
};

const InventoryItemDefinition mapItems[] = {
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6015, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 550, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\263\253\261\277\342\373\306\254\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6012, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 550, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\317\253\317\321\342\373\306\254\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6013, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 550, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\243\321\241\245\243\323\243\324\241\245\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6014, SHEN_FIELD(minimumDistance) 200, SHEN_FIELD(maximumDistance) 550, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\243\302\241\245\243\310\241\245\243\327\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6011, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\272\332\304\301\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6006, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\261\323\262\253\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6010, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\312\241\300\347\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6007, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\307\362\277\301\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6009, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\302\300\317\267\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6008, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\271\310\306\356\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6027, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\271\310\262\332\302\346", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6019, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\316\266\261\306\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6020, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\307\362\300\351\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6021, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\305\300\277\264\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6022, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\262\260\302\346\263\271\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6000, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\257\241\274\245\355\245\363\263\260\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6001, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\257\241\274\245\355\245\363\243\301\245\323\245\353\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6002, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\257\241\274\245\355\245\363\243\302\245\323\245\353\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6003, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\257\241\274\245\355\245\363\243\303\245\323\245\353\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6004, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\257\241\274\245\355\245\363\243\304\245\323\245\353\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6005, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\257\241\274\245\355\245\363\243\305\245\323\245\353\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6023, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\245\257\241\274\245\355\245\363\243\306\245\323\245\353\245\336\245\303\245\327", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6025, SHEN_FIELD(minimumDistance) 800, SHEN_FIELD(maximumDistance) 300, SHEN_FIELD(model) "ABR02MP",
        SHEN_FIELD(label) "\245\354\245\363\244\316\303\317\277\336", SHEN_FIELD(flags) 0x4U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6026, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "WMEM200",
        SHEN_FIELD(label) "\245\246\245\251\245\363\244\316\303\317\277\336", SHEN_FIELD(flags) 0x4U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6018, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "WT001M1",
        SHEN_FIELD(label) "\302\347\270\265\317\260\244\330\244\316\303\317\277\336", SHEN_FIELD(flags) 0x4U},
    {SHEN_FIELD(icon) 11, SHEN_FIELD(id) 6017, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) "HKSM201",
        SHEN_FIELD(label) "\307\362\300\351\317\260\244\330\244\316\303\317\277\336", SHEN_FIELD(flags) 0x4U},
};

const InventoryItemDefinition moveItems[] = {
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3000, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS1SJ",
        SHEN_FIELD(label) "\301\320\277\317\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3001, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "MKIS101",
        SHEN_FIELD(label) "\261\306\274\355\244\352\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3002, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "MKIS103",
        SHEN_FIELD(label) "\263\273\304\314\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3003, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS1HE",
        SHEN_FIELD(label) "\310\364\261\355\317\242\264\242\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3004, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS1TG",
        SHEN_FIELD(label) "\305\332\303\330\351\341\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3005, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS1SK",
        SHEN_FIELD(label) "\316\366\271\270\301\256\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3006, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS1CS",
        SHEN_FIELD(label) "\300\373\311\367\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3007, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS1HZ",
        SHEN_FIELD(label) "\270\327\277\310\312\370\244\267\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3008, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS1OB",
        SHEN_FIELD(label) "\333\260\314\265\301\320\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3009, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "MKIS104",
        SHEN_FIELD(label) "\306\346\244\316\264\254\312\252", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3010, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "MKIS105",
        SHEN_FIELD(label) "\262\316\267\355\244\316\264\254\312\252", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3011, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2KR",
        SHEN_FIELD(label) "\271\337\316\266\265\323\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3012, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2JS",
        SHEN_FIELD(label) "\274\330\277\310\305\275\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3013, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2DH",
        SHEN_FIELD(label) "\302\347\305\270\307\354\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3014, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2SK",
        SHEN_FIELD(label) "\274\320\270\314\331\331\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3015, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2SH",
        SHEN_FIELD(label) "\274\320\312\342\321\374\273\302\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3016, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2SS",
        SHEN_FIELD(label) "\301\320\276\270\302\307\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3017, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2BT",
        SHEN_FIELD(label) "\307\317\304\375\265\323\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3018, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2RH",
        SHEN_FIELD(label) "\317\242\264\304\321\374\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3019, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2JH",
        SHEN_FIELD(label) "\275\347\312\342\331\331\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3020, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2SN",
        SHEN_FIELD(label) "\300\373\266\365\265\323\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3021, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2FS",
        SHEN_FIELD(label) "\311\372\277\310\265\323\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3022, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2HE",
        SHEN_FIELD(label) "\307\362\261\356\315\356\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3023, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2RS",
        SHEN_FIELD(label) "\317\242\274\352\313\244\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3024, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZIS2RT",
        SHEN_FIELD(label) "\317\242\264\304\302\334\244\316\265\273\275\361", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 266, SHEN_FIELD(id) 3025, SHEN_FIELD(minimumDistance) 250, SHEN_FIELD(maximumDistance) 400, SHEN_FIELD(model) "WZKM2ES",
        SHEN_FIELD(label) "\357\345\262\246\273\260\305\300\274\352\244\316\264\254\312\252", SHEN_FIELD(flags) 0x0U},
};

const InventoryItemDefinition pawnItems[] = {
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9000, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\316\311\277\264\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9001, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\272\332\304\301\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9002, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\271\310\306\356\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9003, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\270\367\315\350\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9004, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\303\346\277\264\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9005, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\314\300\317\257\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9006, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\276\267\312\241\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9007, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\266\342\262\332\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9008, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\302\300\317\267\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9009, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\307\362\277\301\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9010, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\266\345\316\266\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
    {SHEN_FIELD(icon) 29, SHEN_FIELD(id) 9011, SHEN_FIELD(minimumDistance) 300, SHEN_FIELD(maximumDistance) 800, SHEN_FIELD(model) INV_NULL,
        SHEN_FIELD(label) "\260\302\270\253\305\366\312\336\245\321\245\363\245\325", SHEN_FIELD(flags) 0x0U},
};

const InventoryItemDefinition *const itemBanks[6] = {keyItems, capsuleToys, photos, mapItems, moveItems, pawnItems};
const std::uint16_t initialItems[] = {0, 2, 5, 1, 6, 8006, 4, 8005, 3, 20, 21, UINT16_MAX};
void doItemEffectStuff();
InventoryItemState g_inventoryItemStates[64] INV_ZERO;
std::uint8_t g_keyItemStock[384] INV_ZERO;
std::uint8_t g_capsuleStock[512] INV_ZERO;
std::uint8_t g_photoStock[64] INV_ZERO;
std::uint8_t g_mapStock[128] INV_ZERO;
std::uint8_t g_moveStock[28] INV_ZERO;
std::uint8_t g_pawnStock[16] INV_ZERO;
std::uint8_t *const inventoryStockBanks[6] INV_INIT{g_keyItemStock, g_capsuleStock, g_photoStock, g_mapStock, g_moveStock, g_pawnStock};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t g_keyItemStateFlags[kKeyItemStateCount] INV_ZERO;
NativeItemSelectorState g_itemSelectorState;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t GetParentMenu();
std::uint32_t shenOpenFile(const char *directory, const char *filename);
std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size);
void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define SPRITE_BITS(type, value) SpriteBits<type>(value)
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawLegacyColourPacket(const LegacySpritePacket &packet, float depth);
void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour);

void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define SPRITE_BITS(type, value) SpriteBits<type>(value)
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t currentItemSelectState;
std::int32_t moneyIconMaximum, moneyIconStepAmount;
std::uint32_t moneyIconFlags;
std::int8_t ctrlSpeakIconState;
std::uint8_t ctrlSpeakOpenRequested = 0;
std::uint16_t ctrlSpeakMode, ctrlSpeakResultChoiceId;
ItemEffectPosition ctrlSpeakSelectorPosition;
std::uint32_t ctrlSpeakSavedLoadingFlags;
std::int32_t ctrlSpeakTexturePoolHandles[4], ctrlSpeakChoiceTextHandles[4] = {};
std::uint32_t ctrlSpeakTexturePoolFlags[4];
std::int32_t ctrlSpeakMarkerGlyphIndex = 0;
std::int32_t ctrlSpeakChoiceCount = CTRL_SPEAK_CHOICE_COUNT;
std::int16_t ctrlSpeakChoiceIds[4], ctrlSpeakChoiceWidths[4] = {};
std::int8_t ctrlSpeakChoiceMode = 0, ctrlSpeakLabelPhase = 0, ctrlSpeakChoiceSpecial = 0, ctrlSpeakLabelMode = 0;
std::int16_t ctrlSpeakElapsedFrames = 0, ctrlSpeakTimeoutFrames = 0;
std::uint16_t ctrlSpeakDefaultChoiceId;
float ctrlSpeakSpecialLabelWidthScale = 0.0f, ctrlSpeakHalfWidthExpansion = 0.0f, ctrlSpeakLeftOriginX = 0.0f, ctrlSpeakRightOriginX = 0.0f;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacyIconSprite ctrlSpeakLabelSprites[4] = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t renderIconFlags, activeIconFlags = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t mainIconPhase;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
IconPadControls g_iconPads;
static const Vector2 helpButtonPositions[4] = {{601.0f, 441.0f}, {561.0f, 457.0f}, {545.0f, 425.0f}, {505.0f, 441.0f}};
static const std::int32_t helpButtonHints[4] = {1, 0, 3, 2};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AllocateGuiPtxBacking() {
    guiPtxBacking = static_cast<std::byte *>(HLib::AllocateTaskData(0x2034, SHEN_NAME(ITPH)));
    if (guiPtxBacking)
        HLib::AssignDynamicAlignmentForTaskData(guiPtxBacking);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT {
    const std::int32_t type = itemTypeByThousands[item / 1000];
    if (type < 6) {
        for (std::int32_t slot = 0; slot < inventoryItemCounts[type]; ++slot) {
            if (itemBanks[type][slot].id == item) {
                index = slot;
                bank = type;
                return &itemBanks[type][slot];
            }
        }
    }
    index = -1;
    bank = -1;
    return INV_NULL;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t UpdateItemStateFlags(std::uint32_t item, std::uint8_t mask, std::int32_t enabled) {
    std::uint8_t previous = 0;
    std::int32_t bank, index;
    FindInventorySlot(item, bank, index);
    if (bank == IB_KeyItems && index >= 0) {
        previous = g_keyItemStateFlags[index];
        if (enabled)
            g_keyItemStateFlags[index] |= mask;
        else
            g_keyItemStateFlags[index] &= static_cast<std::uint8_t>(~mask);
    }
    return previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t GetItemStateFlags(std::uint32_t item) {
    std::int32_t bank, index;
    FindInventorySlot(item, bank, index);
    std::uint8_t result;
    if (bank != IB_KeyItems)
        result = 0;
    else
        result = g_keyItemStateFlags[index];
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetItemStateFlags() {
    for (std::int32_t index = 0; index < kKeyItemStateCount; ++index)
        g_keyItemStateFlags[index] = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetInventoryItemId(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT {
    return category < 6 && index < static_cast<std::uint32_t>(inventoryItemCounts[category]) ? itemBanks[category][index].id : 0xffffffffU;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t AddInventoryItem(std::uint32_t item, std::uint8_t amount, std::uint32_t flags) {
    std::int32_t result = 0;
    if (GetItemState(item) <= 254) {
        std::int32_t bank;
        std::int32_t index;
        const InventoryItemDefinition *const record = FindInventorySlot(item, bank, index);
        if (record) {
            std::uint8_t *const stock = &inventoryStockBanks[bank][index];
            std::int32_t count = *stock + amount;
            if ((record->flags & 1U) == 0)
                count = 1;
            else if (count > 255)
                count = 255;
            *stock = static_cast<std::uint8_t>(count);
            result = 1;
        }
    }
    if (result) {
        if (flags & 1U)
            ToggleItemEffect(item, amount);
        if (hasItemRegistered(item) & 0x10U)
            doItemEffectStuff();
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t RemoveInventoryItem(std::uint32_t item, std::uint8_t amount) {
    std::int32_t result = -1;
    if (GetItemState(item)) {
        std::int32_t bank, index;
        const InventoryItemDefinition *const record = FindInventorySlot(item, bank, index);
        if (record) {
            std::uint8_t *const stock = &inventoryStockBanks[bank][index];
            if (*stock - amount > 0) {
                inventoryStockBanks[bank][index] = static_cast<std::uint8_t>(*stock - amount);
                result = 1;
            } else if (record->flags & IIF_RetainOne) {
                *stock = 1;
                result = 1;
            } else {
                *stock = 0;
                result = 0;
            }
        }
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT {
    std::int32_t bank;
    std::int32_t index;
    FindInventorySlot(item, bank, index);
    return bank < 6 && bank != -1 && index != -1 ? inventoryStockBanks[bank][index] : 0;
}

// @verified - d
std::int16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT {
    return category < 6 && index < static_cast<std::uint32_t>(inventoryItemCounts[category]) ? itemBanks[category][index].icon : 0;
}

// @verified - d
std::int16_t GetInventoryItemIconById(std::uint32_t item) INV_NOEXCEPT {
    std::int32_t bank;
    std::int32_t index;
    FindInventorySlot(item, bank, index);
    return bank != -1 && index != -1 ? static_cast<std::int16_t>(itemBanks[bank][index].icon) : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const char *GetInventoryItemLabelByIndex(std::uint32_t category, std::uint32_t index, std::int32_t) {
    return category < 6 && index < static_cast<std::uint32_t>(inventoryItemCounts[category]) ? itemBanks[category][index].label : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
const char *shenGetAmtOf(std::uint32_t item, std::int32_t) {
    std::int32_t bank, index;
    FindInventorySlot(item, bank, index);
    return bank != -1 && index != -1 ? itemBanks[bank][index].label : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const char *GetInventoryItemModelByIndex(std::uint32_t category, std::uint32_t index) {
    return category < 6 && index < static_cast<std::uint32_t>(inventoryItemCounts[category]) ? itemBanks[category][index].model : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
const char *GetInventoryItemModel(std::uint32_t item) {
    std::int32_t bank, index;
    if (FindInventorySlot(item, bank, index))
        return itemBanks[bank][index].model;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void GetInventoryItemDistances(std::uint32_t item, float *minimum, float *maximum) {
    std::int32_t bank, index;
    FindInventorySlot(item, bank, index);
    if (index != -1 && bank != -1) {
        *minimum = itemBanks[bank][index].minimumDistance * 0.001f;
        *maximum = itemBanks[bank][index].maximumDistance * 0.001f;
    } else {
        *minimum = 0.5f;
        *maximum = 2.0f;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t CountOwnedInventoryItems(std::uint32_t category) {
    std::int32_t count = 0;
    if (category < 6) {
        const std::uint8_t *const stock = inventoryStockBanks[category];
        for (std::int32_t index = 0; index < inventoryItemCounts[category]; ++index)
            if (stock[index])
                ++count;
    }
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT {
    std::int32_t bank;
    std::int32_t index;
    const InventoryItemDefinition *const record = FindInventorySlot(item, bank, index);
    if (record)
        return record->flags;
    return 0;
}

// @verified - d
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT {
    return category < 6 && index < static_cast<std::uint32_t>(inventoryItemCounts[category]) ? itemBanks[category][index].flags : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t GetItemEffectIcon(std::uint32_t item) {
    static const std::int32_t itemEffectIcons[10] = {5, 5, 5, 2, 8, 8, 6, 5, 7, 4};
    const std::uint32_t group = item / 1000U;
    if (group < 10)
        return itemEffectIcons[group];
    return itemEffectIcons[0];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ImportInheritedInventory(const std::uint8_t *items) {
    const std::int16_t mappings[12][2] = {
        {4, 3}, {3, 2}, {5, 4}, {6, 5}, {8, 8005}, {7, 6}, {10, 8006}, {11, 8007}, {12, 8008}, {13, 8009}, {14, 8010}, {15, 8011}};
    for (std::int32_t index = 0; index < 12; ++index)
        if (items[mappings[index][0]])
            AddInventoryItem(static_cast<std::uint32_t>(mappings[index][1]), items[mappings[index][0]], 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ImportCapsuleStock(const std::uint8_t *capsules) {
    std::int32_t destination = 0;
    for (std::int32_t index = 0; index < 256; ++index) {
        if (index == 0x5c)
            continue;
        if (index == 0x5d)
            continue;
        if (index == 0xe2)
            continue;
        if (index == 0xe3)
            continue;
        if (index == 0xe4)
            continue;
        if (index == 0xe5)
            continue;
        g_capsuleStock[destination++] = capsules[index];
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RegisterItems() {
    for (std::int32_t index = 0; index < 4; ++index) {
        const std::uint32_t item = guiRegisteredItems[index];
        if (GetItemState(item) < 3)
            RemoveInventoryItem(item, 255);
        const std::uint32_t nextItem = guiRegisteredItems[index];
        if (GetItemState(nextItem) != 1)
            AddInventoryItem(nextItem, 1, 0);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeInventoryStock() {
    for (std::int32_t index = 0; initialItems[index] != UINT16_MAX; ++index)
        AddInventoryItem(initialItems[index], 1, 0);
    for (std::int32_t item = 4000; item < 4016; ++item)
        AddInventoryItem(item, 1, 0);
    for (std::int32_t item = 4024; item < 4044; ++item)
        AddInventoryItem(item, 1, 0);
    for (std::int32_t item = 4060; item < 4072; ++item)
        AddInventoryItem(item, 1, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT {
    std::uint32_t result;
    if (category > 5)
        result = 0;
    else
        result = inventoryItemCounts[category];
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetItemSelectorDelayedResult(std::int16_t result) {
    g_itemSelectorDelayedResult = result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t GetItemSelectorDelayedResult() {
    return g_itemSelectorDelayedResult;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void SetItemSelectorPhase(std::uint8_t phase) {
    g_itemSelectorState.phase = phase;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint16_t GetItemSelectorTemporaryStatus(std::int8_t index) {
    return g_itemSelectorTemporaryStatus[index];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetItemSelectorTemporaryStatus(std::int8_t index, std::int16_t status) {
    g_itemSelectorTemporaryStatus[index] = status;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
TextureChunk *SetupPtxTexture(std::int32_t width, std::uint32_t idLow, std::uint32_t idHigh) {
    TextureChunk *const texture = reinterpret_cast<TextureChunk *>(guiPtxBacking);
    *reinterpret_cast<std::uint32_t *>(guiPtxBacking + 0x2c) = 0;
    ParseGuiPtx(texture, 16, width, idLow, idHigh, 0x502);
    return reinterpret_cast<TextureChunk *>(guiPtxBacking);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t PublishLegacyItemLabel(const char *text, std::int32_t width, std::int32_t texture, std::int32_t mode) {
    const std::int32_t result = RasterizeWindowTextLine4bpp(guiPtxBacking + 0x2c, width, text, mode, 1);
    ReplacePtxTexture(texture, reinterpret_cast<TextureChunk *>(guiPtxBacking));
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawItemEffectWindow(std::int32_t columns, float x, float y, float depth) {
    const ScreenMode mode = static_cast<ScreenMode>(0);
    DrawLegacyWindowHorizontal(x, y, columns, 1, 0, depth, 1024);
    DrawLegacyWindowHorizontal(x, y + 48.0f, columns, 1, 1, depth, 1024);
    DrawLegacyWindowTiles((x + 24.0f) + 11.0f, y + 24.0f, columns, 1, depth - 2.0f, 1024);
    DrawLegacyWindowBackground(
        (x + 11.0f) + 4.0f, y + 4.0f, static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(columns) * 40U)), 40.0f, depth - 1.0f, 1024);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ItemEffectRender_Callback(void *) {
    std::int32_t columns = itemEffectColumns;
    LegacyIconSprite *sprite = &itemEffectSprite;
    if (itemEffectLabelTexture < 0)
        return;
    sprite->x = itemEffectPosition.x;
    sprite->y = itemEffectPosition.y;
    PrepareDrawWithMode(*sprite);
    sprite->x += -33.0f;
    if (itemEffectAmount > 1)
        ++columns;
    if (itemEffectAmount > 99)
        ++columns;
    DrawItemEffectWindow(columns, sprite->x, itemEffectSprite.y + -25.0f, 3.0f);
    Vector2 labelPosition;
    labelPosition.x = (static_cast<float>((static_cast<std::int32_t>(itemEffectColumns) - 1) * 20) + sprite->x) + 40.0f;
    labelPosition.y = sprite->y;
    DrawLegacyItemLabel(&itemEffectLabelTexture, &labelPosition, 3.0f, 0xff000000U);
    if (itemEffectAmount > 1) {
        labelPosition.x = static_cast<float>(columns * 40) + sprite->x;
        DrawLegacyItemQuantity(labelPosition.x, labelPosition.y, itemEffectAmount, 3.1f);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void KeepMoneyIconOpen() {
    moneyIconFlags &= ~0x10U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t GetDefaultMotionTableIndex(std::uint32_t rig) {
    switch (rig) {
    case 0:
        return 2;
    case 1:
        return 3;
    case 2:
        return 4;
    case 3:
        return 5;
    case 4:
        return 6;
    case 5:
        return 7;
    case 6:
        return 8;
    case 7:
        return 9;
    case 9:
        return 10;
    case 10:
        return 11;
    case 11:
        return 12;
    case 12:
        return 13;
    case 13:
        return 14;
    case 14:
        return 15;
    case 15:
        return 16;
    case 8:
    default:
        return 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId) {
    if (!work)
        return 0;
    if (!work->defaultAnimations[0])
        std::memcpy(work->defaultAnimations, defaultMotionIds[GetDefaultMotionTableIndex(work->rigBoneId)], sizeof(work->defaultAnimations));
    if (mode == 1) {
        if (slot > 5)
            return 0;
        if (motionId && MOTN_GetDataForAnimId(motionId)) {
            work->defaultAnimations[slot] = motionId;
            if (slot == 5)
                work->copyFlags |= 0x1000U;
            return 1;
        }
        if (slot == 5) {
            work->defaultAnimations[5] = work->defaultAnimations[0];
            work->copyFlags &= ~0x1000U;
            return 1;
        }
    } else if (mode) {
        if (mode > 16)
            return 0;
        std::memcpy(work->defaultAnimations, defaultMotionIds[mode], sizeof(work->defaultAnimations));
        return 1;
    } else
        std::memcpy(work->defaultAnimations, defaultMotionIds[GetDefaultMotionTableIndex(work->rigBoneId)], sizeof(work->defaultAnimations));
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawItemSelectorInventory(std::int32_t columns, std::int32_t maximum, float x, float y, float depth, std::int32_t first, std::uint32_t bank) {
    std::int32_t column = 0;
    LegacyIconSprite &sprite = itemEffectSprite;
    sprite.depth = depth;
    std::int32_t drawn = 0;
    sprite.x = x;
    sprite.y = y;
    const std::int32_t count = static_cast<std::int32_t>(GetMaxItemsForItemType(bank));
    for (std::int32_t item = first; item < count; ++item) {
        if ((g_itemSelectorOwnedMask[item / 8] >> (item & 7)) & 1) {
            sprite.icon = GetInventoryItemIcon(bank, item);
            PrepareDrawWithMode(sprite);
            sprite.x += 40.0f;
            if (++column >= columns) {
                sprite.x = x;
                sprite.y += 40.0f;
                column = 0;
            }
            if (++drawn >= maximum)
                return;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void DrawItemSelectorLowerActions(const Vector2 &position, float depth) {
    static const std::uint16_t icons[8] = {0, 1, 2, 8, 7, 6, 4, 9};
    LegacyIconSprite sprite = itemEffectSprite;
    LegacyIconSprite *work = &sprite;
    work->depth = depth;
    work->x = position.x;
    work->y = position.y;
    for (std::int32_t index = 0; index < 8; ++index) {
        work->icon = icons[index];
        PrepareDrawWithMode(*work);
        work->x += 40.0f;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void RefreshItemSelectorLabel() {
    static const char *const itemSelectorLabels[8] = {"\xa5\xbb\xa1\xbc\xa5\xd6", "\xc0\xdf\xc4\xea", "\xc1\xb4\xb5\xbb\xb4\xac\xca\xaa",
        "\xa5\xb3\xa5\xec\xa5\xaf\xa5\xb7\xa5\xe7\xa5\xf3", "\xa5\xa2\xa5\xeb\xa5\xd0\xa5\xe0", "\xa5\xde\xa5\xc3\xa5\xd7",
        "\xbc\xc1\xb2\xb0\xa5\xd1\xa5\xf3\xa5\xd5\xa5\xec\xa5\xc3\xa5\xc8", "\xb4\xac\xca\xaa"};
    NativeItemSelectorState &state = g_itemSelectorState;
    const std::int32_t menu = state.activeMenu;
    itemEffectLabelTexture = -1;
    if (menu == 8)
        PrepareLegacyInventoryItemLabel(&itemEffectLabelTexture, static_cast<std::uint32_t>(state.selectedItem));
    else {
        PublishLegacyItemLabel(itemSelectorLabels[menu], 512, commonItemTexture, 1);
        itemEffectLabelTexture = static_cast<std::int16_t>(commonItemTexture);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CtrlItemSelectIcon_Callback(void *) {
    NativeItemSelectorState &state = g_itemSelectorState;
    MarkItemSelectorPlayerMoved();
    if (!(g_loadingFlag & 2U)) {
        switch (static_cast<std::int8_t>(state.phase)) {
        case 0:
            break;
        case 1:
            SetItemSelectorPhase(2);
            state.exitFlags = 0;
            state.context = CaptureItemSelectorContext();
            itemEffectPosition.x = -672.0f;
            itemEffectPosition.y = 55.0f;
            g_itemSelectorCamera.mode = 1;
            camCtrlSetState(9);
            PrepareItemSelectorCamera(&g_itemSelectorCamera);
            state.selectedItem = GetItemSelectorSelectedItem(1);
            state.selectedIndex = GetItemSelectorSelectedItem(0);
            RefreshItemSelectorLabel();
            break;
        case 2:
            BufferItemSelectorInput(GetPadControlData(1));
            if (AdvanceItemSelectorCamera(&g_itemSelectorCamera)) {
                iconToggleStateFor(1, true);
                itemEffectPosition.x = -55.0f;
                SetItemSelectorPhase(3);
                EV_EventSetUpHelpIcon_0(0x11, 0);
                if (static_cast<NativeItemSelectorContext *>(state.context)->helpTopic)
                    SetHelpIconMenuFlags(0x21);
                else
                    AddHelpIconMenuFlags(0x21);
                if (GetItemSelectorTemporaryStatus(0)) {
                    const std::int32_t item = state.selectedItem;
                    if ((state.activeMenu == 8 || FindUniqueItemActionIndex(item) >= 0) && GetInventoryItemModel(item))
                        iconToggleStateFor(1, false);
                } else
                    ExecuteAicaCommand(0x400ab, 0, 0);
            }
            break;
        case 4:
            SetItemSelectorPhase(5);
            break;
        case 5:
            if ((state.exitFlags & 1U) == 0) {
                if (!RetreatIconPosition(&itemEffectPosition, -672.0f))
                    break;
            }
            iconToggleStateFor(1, false);
            if (state.context && static_cast<NativeItemSelectorContext *>(state.context)->cameraState == 0) {
                g_itemSelectorCamera.mode = 0;
                camCtrlSetFovTransitionTo(0);
                FinishItemSelector();
            } else {
                CloseItemSelectorCamera(&g_itemSelectorCamera);
                SetItemSelectorPhase(7);
            }
            break;
        case 6:
            if (state.context && static_cast<NativeItemSelectorContext *>(state.context)->cameraState == 0) {
                g_itemSelectorCamera.mode = 0;
                camCtrlSetFovTransitionTo(0);
                FinishItemSelector();
            } else {
                CloseItemSelectorCamera(&g_itemSelectorCamera);
                SetItemSelectorPhase(7);
            }
            break;
        case 7:
            if (g_itemSelectorCamera.mode && AdvanceItemSelectorCamera(&g_itemSelectorCamera)) {
                g_itemSelectorCamera.mode = 0;
                FinishItemSelector();
            }
            break;
        case 3:
            if (state.exitFlags & 1U) {
                iconToggleStateFor(1, false);
                state.phase = 4;
            } else
                ProcessLegacyItemSelectorInput();
            RequestHelpIconSelection(0x600);
            break;
        default:
            break;
        }
    }
    if (GetItemSelectionMode(1))
        HLib::ExecuteCallbackNow(RenderLegacyItemSelector, &state);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ProcessLegacyItemSelectorInput() {
    NativeItemSelectorState &state = g_itemSelectorState;
    const std::int8_t oldRow = state.row, oldColumn = state.column;
    const std::int32_t oldMenu = state.activeMenu;
    std::uint16_t pressed = GetPadControlData(1);
    if (GetItemSelectorTemporaryStatus(0) && (pressed & 0xf6c6U)) {
        SetItemSelectorTemporaryStatus(0, 0);
        SetItemSelectorTemporaryStatus(1, 0);
        iconToggleStateFor(1, true);
    }
    pressed |= GetItemSelectorTemporaryStatus(0);
    SetItemSelectorTemporaryStatus(0, 0);
    if (GetItemSelectorDelayedResult())
        pressed = 0;
    pressed &= 0xf600U;
    if (pressed & 0x200U) {
        ExecuteAicaCommand(0x100ab, 0, 0);
        SetItemSelectorPhase(4);
    } else {
        if (state.activeMenu != 8)
            SetItemSelectorTemporaryStatus(1, 0);
        if (pressed && NavigateLegacyItemSelector(state, pressed)) {
            state.selectedItem = GetItemSelectorSelectedItem(1);
            state.selectedIndex = GetItemSelectorSelectedItem(0);
            RefreshItemSelectorLabel();
        }
    }
    if ((oldColumn ^ state.column) | (state.row ^ oldRow) | (state.activeMenu ^ oldMenu))
        ExecuteAicaCommand(0x200ab, 0, 0);
    if (GetItemSelectorDelayedResult())
        ApplyItemSelectorDelayedResult();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static bool NavigateLegacyItemSelector(NativeItemSelectorState &state, std::uint16_t pressed) {
    static const std::int32_t menuActions[8] = {4, 5, 6, 7, 8, 9, 10, 11};
    if (state.activeMenu == 8) {
        if (state.count <= 1)
            pressed &= 0x3fffU;
        if (state.selectedItem < 0)
            pressed &= 0xfbffU;
        if (pressed & 0x400U) {
            const std::int32_t action = FindUniqueItemActionIndex(state.selectedItem);
            if (action >= 0)
                ResolveUniqueItemAction(static_cast<std::uint32_t>(action));
            else
                ResolveUniqueItemAction(3);
            pressed &= 0x400U;
        } else
            NavigateItemSelectorInventory(state, pressed);
    } else if (pressed & 0x400U) {
        if (state.activeMenu != 8)
            ResolveUniqueItemAction(static_cast<std::uint32_t>(menuActions[state.activeMenu]));
        pressed &= 0x400U;
    } else if (pressed & 0xf000U) {
        if (pressed & 0x1000U) {
            state.row = static_cast<std::int8_t>(state.visibleRows - 1);
            state.column = static_cast<std::int8_t>(state.activeMenu);
            state.activeMenu = 8;
            if (state.row * state.columns + state.column >= state.count) {
                if (state.row > 0)
                    --state.row;
                ClampItemSelectorSelection();
            }
        } else if (pressed & 0x2000U) {
            state.row = 0;
            state.column = static_cast<std::int8_t>(state.activeMenu);
            state.activeMenu = 8;
            ClampItemSelectorSelection();
        } else if (pressed & 0x8000U) {
            if (static_cast<std::uint32_t>(state.activeMenu++) > 6U)
                state.activeMenu = 0;
        } else if (pressed & 0x4000U) {
            if (static_cast<std::uint32_t>(state.activeMenu--) < 1U)
                state.activeMenu = 7;
        }
    }
    if (pressed & 0xf000U)
        return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void NavigateItemSelectorInventory(NativeItemSelectorState &state, std::uint16_t pressed) {
    if (pressed & 0x4000U) {
        --state.column;
        if (state.column < 0)
            state.column = static_cast<std::int8_t>(state.columns - 1);
        ClampItemSelectorSelection();
    } else if (pressed & 0x8000U) {
        ++state.column;
        if (state.row * state.columns + state.column >= state.count)
            state.column = 0;
        if (state.column >= state.columns)
            state.column = 0;
    } else if (pressed & 0x2000U) {
        if ((state.row + 1) * state.columns + state.column >= state.count || state.row == state.visibleRows - 1) {
            if (state.column > 7)
                state.activeMenu = 7;
            else
                state.activeMenu = state.column;
        } else
            ++state.row;
    } else if (pressed & 0x1000U) {
        --state.row;
        if (state.row < 0) {
            if (state.column > 7)
                state.activeMenu = 7;
            else
                state.activeMenu = state.column;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool InitializeCommonItemTexture() {
    commonItemTexture = -1;
    TextureChunk *const texture = SetupPtxTexture(512, SHEN_NAME(ITEM), SHEN_NAME(COMN));
    if (texture) {
        commonItemTexture = LoadAndFindTEXN_Wrapper(texture);
        if (commonItemTexture >= 0)
            return true;
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitItemSelectIcon_Callback(void *) {
    HLib::EnqueueTaskWithoutParameterWrapper(ICSU_InitItemLook_Callback, 0, SHEN_NAME(ICLI));
    g_uniqueItemActionTask = HLib::EnqueueTaskWithParameterWrapper(ICIT_UniqAction_Callback, 0, sizeof(NativeUniqueItemAction), SHEN_NAME(ICSS));
    if (g_uniqueItemActionTask) {
        NativeUniqueItemAction *const action = static_cast<NativeUniqueItemAction *>(HLib::GetTaskParameter(g_uniqueItemActionTask));
        if (action) {
            action->begin = 0;
            action->update = 0;
            action->complete = 0;
            action->cancel = 0;
            action->item = -1;
        }
        HLib::SetupTimedTask(g_uniqueItemActionTask, 0);
    }
    InitializeItemSelectorWindow();
    InitializeLegacyCollectionMenu();
    SetItemSelectorPhase(0);
    iconToggleStateFor(1, false);
    NativeItemSelectorState &state = g_itemSelectorState;
    state.actionResult = 0;
    state.count = 0;
    state.row = 0;
    state.column = 0;
    state.visibleRows = 1;
    state.columns = 8;
    state.activeMenu = 8;
    state.glowColour = 0;
    state.glowBrightening = 1;
    state.exitFlags = 0;
    itemEffectIconPhase = 0;
    SetItemSelectorTemporaryStatus(0, 0);
    SetItemSelectorTemporaryStatus(1, 0);
    currentItemSelectState = 0;
    g_itemSelectorCamera.mode = 0;
    g_itemSelectTask = HLib::GetCurrentTask();
    Queue_wf_MainControl(7);
    HLib::SetCurrentTaskTo(CtrlItemSelectIcon_Callback);
    HLib::SetupTimedTask(g_itemSelectTask, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t helpGetCurrentIconIdx(std::int32_t slot) {
    std::uint8_t index;
    if (slot != 1)
        index = helpIconMode;
    else
        index = helpPreviousIconIndex;
    return index;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static Character *ItemSelectorPlayer() {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control->ryo;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BeginItemSelector() {
    if (GetShouldRenderIcons(ItemSelect) || !g_itemSelectTask)
        return;
    HLib::ResolveDeferredCallback(g_itemSelectTask);
    SetItemSelectorPhase(1);
    RefreshItemSelectorSelection();
    SetItemSelectorTemporaryStatus(0, 0);
    SetItemSelectorTemporaryStatus(1, 0);
    padcRefreshCurrentInputs(1);
    ToggleRenderIcons(ItemSelect, true);
    SetItemSelectorDelayedResult(0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void FinishItemSelector() {
    NativeItemSelectorState &state = g_itemSelectorState;
    if (!GetShouldRenderIcons(ItemSelect))
        return;
    if (g_itemSelectTask)
        HLib::SetupTimedTask(g_itemSelectTask, 0);
    SetItemSelectorPhase(0);
    padcRefreshCurrentInputs(0);
    ToggleRenderIcons(ItemSelect, false);
    state.exitFlags = 0;
    RestoreItemSelectorContext(&state);
    itemEffectLabelTexture = -1;
    SetItemSelectorClosingState();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorWatch() {
    return OpenWristwatch(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t UpdateSelectorWatch() {
    return IsWristwatchOpenable();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t CancelSelectorFullMap(std::int32_t) {
    CloseFullMapMenu();
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorItemLook() {
    return OpenItemLook(g_itemSelectorState.selectedItem);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t UpdateSelectorItemLook() {
    return (GetCurrentItemSelectState() & 1U) != 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void CancelSelectorItemLook(std::int32_t force) {
    if ((GetCurrentItemSelectState() & 1U) && force)
        CloseItemLook();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorSave() {
    if (IsSaveAllowed() || !EV_CanDoAreaJump())
        return 0;
    shenSetTimeSimulationTo(0);
    StartShowScreenOfId(10);
    CheckHelpIcon(0, 0, CheckHelpIconOp_Set);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t UpdateSelectorSave() {
    return static_cast<std::int32_t>(IsSaveAllowed());
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void CompleteSelectorSave() {
    shenSetTimeSimulationTo(1);
    if (GetItemSelectorDelayedResult() == 32)
        CheckHelpIcon(0, 0, CheckHelpIconOp_Set);
    else
        CheckHelpIcon(33, 0, CheckHelpIconOp_Set);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorEasyOptions() {
    SetNewEasyOptionState(0);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t UpdateSelectorEasyOptions() {
    return IsEasyOptionActive();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t UpdateSelectorMoves() {
    return UpdateWazamakiMenu() == false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t CancelSelectorMoves(std::int32_t force) {
    if (force) {
        CloseWazamakiMenu();
        return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorCollection() {
    return OpenLegacyCollectionMenu(0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorPhotos() {
    return OpenLegacyCollectionMenu(2);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t UpdateSelectorCollection() {
    return UpdateLegacyCollectionMenu() == 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t UpdateSelectorPhotos() {
    return UpdateLegacyCollectionMenu() == 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorPawn() {
    return OpenPawnWindow(0.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t UpdateSelectorPawn() {
    return IsPawnWindowOpen();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t FindUniqueItemActionIndex(std::int32_t item) {
    static const std::int32_t uniqueItems[3] = {0, 2, 20};
    for (std::int32_t index = 0; index < 3; ++index)
        if (item == uniqueItems[index])
            return index;
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResolveUniqueItemAction(std::uint32_t mode) {
    NativeUniqueItemAction *const action = static_cast<NativeUniqueItemAction *>(HLib::GetTaskParameter(g_uniqueItemActionTask));
    if (mode > 11 || !action)
        return;
    action->item = static_cast<std::int32_t>(mode);
    action->begin = itemSelectorActions[mode].begin;
    action->update = itemSelectorActions[mode].update;
    action->complete = itemSelectorActions[mode].complete;
    action->cancel = itemSelectorActions[mode].cancel;
    HLib::ResolveDeferredCallback(g_uniqueItemActionTask);
    HLib::SetupTimedTask(g_itemSelectTask, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ICIT_UniqAction_Callback(void *) {
    NativeUniqueItemAction &action = *static_cast<NativeUniqueItemAction *>(HLib::GetTaskParameter(g_uniqueItemActionTask));
    if (action.begin) {
        if (!action.begin()) {
            ExecuteAicaCommand(0x800ab, 0, 0);
            action.update = 0;
            action.complete = 0;
            action.cancel = 0;
            action.item = -1;
            RefreshItemSelectorInventory(0);
            HLib::SetupTimedTask(g_uniqueItemActionTask, 0);
            HLib::ResolveDeferredCallback(g_itemSelectTask);
        } else
            ExecuteAicaCommand(0x500ab, 0, 0);
        action.begin = 0;
        return;
    }
    if (action.update && action.update())
        return;
    if (action.complete)
        action.complete();
    ReturnToItemSelectorList();
    RefreshItemSelectorInventory(0);
    ExecuteAicaCommand(0x500ab, 0, 0);
    action.begin = 0;
    action.complete = 0;
    action.update = 0;
    action.cancel = 0;
    action.item = -1;
    HLib::SetupTimedTask(g_uniqueItemActionTask, 0);
    HLib::ResolveDeferredCallback(g_itemSelectTask);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void ReturnToItemSelectorList() {
    iconToggleStateFor(1, true);
    RefreshItemSelectorLabel();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t BeginSelectorMemo() {
    return RefreshPadTask(1, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void CancelNativeSelectorMemo(std::int32_t force) {
    CancelSelectorMemo(force);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CancelUniqueItemAction() {
    if (!g_uniqueItemActionTask)
        return;
    NativeUniqueItemAction *const action = static_cast<NativeUniqueItemAction *>(HLib::GetTaskParameter(g_uniqueItemActionTask));
    if (!action)
        return;
    if (action->cancel)
        action->cancel(1);
    action->begin = 0;
    action->update = 0;
    action->complete = 0;
    action->cancel = 0;
    action->item = -1;
    HLib::SetupTimedTask(g_uniqueItemActionTask, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void PrepareItemSelectorCamera(NativeItemSelectorCamera *camera) {
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (!player)
        return;
    camera->duration = 8;
    camera->frame = 0;
    camera->mode = 2;
    camCtrlGetNextCamPos(&camera->savedEye);
    camCtrlGetTargetPosition(&camera->savedTarget);
    g_itemSelectorCamera.savedAlpha = player->alpha;
    camera->eye.x = camera->savedEye.x;
    camera->eye.y = camera->savedEye.y;
    camera->eye.z = camera->savedEye.z;
    camera->target.x = camera->savedTarget.x;
    camera->target.y = camera->savedTarget.y;
    camera->target.z = camera->savedTarget.z;
    const std::int32_t yaw = -GetCharacterWorldYaw(player);
    Vector3 position;
    GetCharacterWorldPosition(player, &position);
    position.y += 1.6f;
    camera->goalEye.x = position.x;
    camera->goalEye.y = position.y;
    camera->goalEye.z = position.z;
    camera->goalTarget.x = position.x;
    camera->goalTarget.y = position.y;
    camera->goalTarget.z = position.z;
    camera->goalTarget.x = CalculateFixedSine(yaw) * 0.2f + camera->goalTarget.x;
    camera->goalTarget.z = CalculateFixedCosine(yaw) * -0.2f + camera->goalTarget.z;
    camera->eyeStep.x = (camera->goalEye.x - camera->eye.x) / static_cast<float>(camera->duration);
    camera->eyeStep.y = (camera->goalEye.y - camera->eye.y) / static_cast<float>(camera->duration);
    camera->eyeStep.z = (camera->goalEye.z - camera->eye.z) / static_cast<float>(camera->duration);
    camera->targetStep.x = (camera->goalTarget.x - camera->target.x) / static_cast<float>(camera->duration);
    camera->targetStep.y = (camera->goalTarget.y - camera->target.y) / static_cast<float>(camera->duration);
    camera->targetStep.z = (camera->goalTarget.z - camera->target.z) / static_cast<float>(camera->duration);
    camCtrlSetNextPosition(camera->eye);
    camCtrlSetTargetPosition(camera->target);
    camera->alpha = camera->savedAlpha;
    camera->alphaStep = (camera->alpha / static_cast<float>(camera->duration / 2)) * -1.0f;
    player->alpha = camera->alpha;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void CloseItemSelectorCamera(NativeItemSelectorCamera *camera) {
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (!player)
        return;
    camera->duration = 8;
    camera->frame = 0;
    camera->mode = 3;
    camera->goalEye.x = camera->savedEye.x;
    camera->goalEye.y = camera->savedEye.y;
    camera->goalEye.z = camera->savedEye.z;
    camera->goalTarget.x = camera->savedTarget.x;
    camera->goalTarget.y = camera->savedTarget.y;
    camera->goalTarget.z = camera->savedTarget.z;
    camera->eyeStep.x = (camera->goalEye.x - camera->eye.x) / static_cast<float>(camera->duration);
    camera->eyeStep.y = (camera->goalEye.y - camera->eye.y) / static_cast<float>(camera->duration);
    camera->eyeStep.z = (camera->goalEye.z - camera->eye.z) / static_cast<float>(camera->duration);
    camera->targetStep.x = (camera->goalTarget.x - camera->target.x) / static_cast<float>(camera->duration);
    camera->targetStep.y = (camera->goalTarget.y - camera->target.y) / static_cast<float>(camera->duration);
    camera->targetStep.z = (camera->goalTarget.z - camera->target.z) / static_cast<float>(camera->duration);
    camCtrlSetNextPosition(camera->eye);
    camCtrlSetTargetPosition(camera->target);
    g_itemSelectorCamera.alphaStep = camera->savedAlpha / static_cast<float>(camera->duration / 2);
    player->alpha = camera->alpha;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static bool AdvanceItemSelectorCamera(NativeItemSelectorCamera *camera) {
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (!player || srchCheckFlags(3) >= 0)
        return true;
    bool finished = false;
    if (++camera->frame > camera->duration) {
        camera->eye.x = camera->goalEye.x;
        camera->eye.y = camera->goalEye.y;
        camera->eye.z = camera->goalEye.z;
        camera->target.x = camera->goalTarget.x;
        camera->target.y = camera->goalTarget.y;
        camera->target.z = camera->goalTarget.z;
        finished = true;
        if (camera->mode)
            camera->alpha = 0.0f;
        else
            camera->alpha = camera->savedAlpha;
    } else {
        if (static_cast<std::int8_t>(camera->mode) == 2) {
            const std::int32_t yaw = -GetCharacterWorldYaw(player);
            Vector3 position;
            GetCharacterWorldPosition(player, &position);
            position.y += 1.6f;
            camera->goalEye.x = position.x;
            camera->goalEye.y = position.y;
            camera->goalEye.z = position.z;
            camera->goalTarget.x = position.x;
            camera->goalTarget.y = position.y;
            camera->goalTarget.z = position.z;
            camera->goalTarget.x = CalculateFixedSine(yaw) * 0.2f + camera->goalTarget.x;
            camera->goalTarget.z = CalculateFixedCosine(yaw) * -0.2f + camera->goalTarget.z;
            std::int16_t remaining = static_cast<std::int16_t>(static_cast<std::int16_t>(camera->duration) - static_cast<std::int16_t>(camera->frame));
            if (remaining <= 0)
                remaining = 1;
            camera->eyeStep.x = (camera->goalEye.x - camera->eye.x) / static_cast<float>(remaining);
            camera->eyeStep.y = (camera->goalEye.y - camera->eye.y) / static_cast<float>(remaining);
            camera->eyeStep.z = (camera->goalEye.z - camera->eye.z) / static_cast<float>(remaining);
            camera->targetStep.x = (camera->goalTarget.x - camera->target.x) / static_cast<float>(remaining);
            camera->targetStep.y = (camera->goalTarget.y - camera->target.y) / static_cast<float>(remaining);
            camera->targetStep.z = (camera->goalTarget.z - camera->target.z) / static_cast<float>(remaining);
        }
        camera->eye.x += camera->eyeStep.x;
        camera->eye.y += camera->eyeStep.y;
        camera->eye.z += camera->eyeStep.z;
        camera->target.x += camera->targetStep.x;
        camera->target.y += camera->targetStep.y;
        camera->target.z += camera->targetStep.z;
        camera->alpha += camera->alphaStep;
        if (camera->alpha < 0.0f)
            camera->alpha = 0.0f;
        else if (camera->alpha > 1.0f)
            camera->alpha = 1.0f;
    }
    camCtrlSetNextPosition(camera->eye);
    camCtrlSetTargetPosition(camera->target);
    player->alpha = camera->alpha;
    return finished;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool CloseItemSelectIcon(std::int32_t force) {
    if (GetShouldRenderIcons(ItemSelect) && force) {
        if (g_itemSelectTask)
            HLib::SetupTimedTask(g_itemSelectTask, 0);
        CancelUniqueItemAction();
        CloseWristwatchImmediate2();
        SetItemSelectorDelayedResult(0);
        camCtrlSetFovTransitionTo(1);
        camCtrlSetState(0);
        g_itemSelectorCamera.mode = 0;
        SetItemSelectorClosingState();
        itemEffectPosition.x = -672.0f;
        SetItemSelectorPhase(0);
        iconToggleStateFor(1, false);
        ToggleRenderIcons(ItemSelect, false);
        itemEffectLabelTexture = -1;
        RestoreItemSelectorContext(&g_itemSelectorState);
        return true;
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t PrepareLegacyInventoryItemLabel(std::int16_t *label, std::uint32_t item) {
    const char *text = shenGetAmtOf(item);
    if (!text)
        text = ITEM_EFFECT_MISSING_NAME;
    const std::int32_t length = static_cast<std::int32_t>(UI_STRLEN(text));
    *label = -1;
    PublishLegacyItemLabel(text, 512, commonItemTexture, 1);
    *label = static_cast<std::int16_t>(commonItemTexture);
    return length;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawLegacyItemLabel(const std::int16_t *texture, const Vector2 *position, float depth, std::uint32_t secondaryColour) {
    if (*texture == -1)
        return;
    LegacySpritePacket packet;
    do {
        packet.quad.flags = 0;
        packet.colour = 0xffffffffU;
        packet.secondaryColour = 0;
        packet.uv[0] = packet.uv[1] = 0.0f;
        packet.uv[2] = packet.uv[3] = 1.0f;
        packet.reservedTail = 0;
    } while (0);
    packet.quad.flags |= 0x400U;
    packet.colour = 0xcdcdcdcdU;
    packet.secondaryColour = secondaryColour;
    AdjustScreenAspectRatio(
        packet.quad, static_cast<ScreenAlignment>(5), static_cast<std::int32_t>(position->x), static_cast<std::int32_t>(position->y), 512, 32);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(static_cast<std::int32_t>(*texture)), depth);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetLegacyItemSelectorExitFlag() {
    g_itemSelectorState.exitFlags |= 1U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static bool UpdateMoneyCash() {
    bool complete = false;
    float *settleFrames = &moneyIconSettleFrames;
    *settleFrames -= 1.0f;
    if (moneyIconDisplay == moneyIconTarget) {
        if (!(*settleFrames > 0.0f)) {
            *settleFrames = 0.0f;
            return true;
        }
        return false;
    }
    if (moneyIconDisplay > moneyIconTarget) {
        moneyIconLastIncrement =
            MoneyIconStep(static_cast<std::int32_t>(static_cast<std::uint32_t>(moneyIconDisplay) - static_cast<std::uint32_t>(moneyIconTarget)));
        moneyIconDisplay = static_cast<std::int32_t>(static_cast<std::uint32_t>(moneyIconDisplay) - static_cast<std::uint32_t>(moneyIconLastIncrement));
        if (moneyIconTarget >= moneyIconDisplay) {
            moneyIconDisplay = moneyIconTarget;
            complete = true;
        }
    } else if (moneyIconDisplay < moneyIconTarget) {
        moneyIconLastIncrement =
            MoneyIconStep(static_cast<std::int32_t>(static_cast<std::uint32_t>(moneyIconTarget) - static_cast<std::uint32_t>(moneyIconDisplay)));
        moneyIconDisplay = static_cast<std::int32_t>(static_cast<std::uint32_t>(moneyIconDisplay) + static_cast<std::uint32_t>(moneyIconLastIncrement));
        if (moneyIconTarget <= moneyIconDisplay) {
            moneyIconDisplay = moneyIconTarget;
            complete = true;
        }
    }
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    SetCashOrSegaCoins();
    ExecuteAicaCommand(0xd00ab, 0, 0);
#else
    if (!(moneyIconFlags & 0x20U)) {
        if (moneyIconDrawMode)
            g_nativePlayerStats.cash = moneyIconDisplay;
        else
            g_nativePlayerStats.segaCoins = moneyIconDisplay;
    }
    shenExecuteAICACommand(0xd00ab, 0, 0);
#endif
    return complete;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void UpdateMoneySettle() {
    bool complete = true;
    if (moneyIconFlags & 0x10U)
        complete = UpdateMoneyCash();
    if (complete && (moneyIconFlags & 0x10U)) {
        moneyIconFrames -= 1.0f;
        if (!(moneyIconFrames > 0.0f)) {
            SetMoneyIconPhase(4);
            moneyIconFrames = 0.0f;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t MoneyIconStep(std::int32_t delta) {
    std::int32_t sign = 1;
    if (delta < 0)
        sign = -1;
    if (delta < 0)
        delta = -delta;
    std::int32_t step = 0;
    for (std::int32_t index = 0; index < 8; ++index) {
        if (delta >= moneyIconIncrements[index])
            step += moneyIconIncrements[index];
        while (delta >= moneyIconIncrements[index])
            delta -= moneyIconIncrements[index];
    }
    step *= sign;
    return step;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void PrepareMoneyIconCurrency() {
    LegacyIconSprite *sprite = &moneyIconSprite;
    if (moneyIconDrawMode) {
        sprite->icon = 0x608;
        sprite->width = 80;
        sprite->height = 40;
        if (!(moneyIconFlags & 0x20U))
            moneyIconDisplay = g_nativePlayerStats.cash;
    } else {
        sprite->icon = 3;
        sprite->width = 40;
        sprite->height = 40;
        if (!(moneyIconFlags & 0x20U))
            moneyIconDisplay = g_nativePlayerStats.segaCoins;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void UpdateMoneySelection() {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t pressed = GetPadControlData(1), repeated = GetPadControlData(2), hints = 0x600;
    if (static_cast<std::int16_t>(moneyIconPosition.selectedDirection) != 4)
        pressed = repeated = 0;
    const std::int32_t previous = moneyIconAmount;
    std::uint32_t disabled = 0;
    if (!(moneyIconFlags & 2))
        pressed &= 0xdfffU;
    if ((moneyIconFlags & 1) == 0)
        repeated &= 0xefffU;
    if (moneyIconAmount >= moneyIconMaximum) {
        disabled |= 0x5000;
        repeated = (repeated & 0xafffU) | (pressed & 0x5000U);
    }
    if (moneyIconAmount < 1) {
        disabled |= 0xa400;
        hints &= 0xfbffU;
        repeated = (repeated & 0x5fffU) | (pressed & 0xa000U);
    }
    if ((pressed >> 10) & 1) {
        if (pressed & disabled)
            ExecuteAicaCommand(0x800ab, 0, 0);
        else
            SetMoneyIconPhase(4);
    } else if (repeated & 0x4000U) {
        if (disabled & 0x4000U)
            ExecuteAicaCommand(0x800ab, 0, 0);
        else {
            moneyIconAmount += moneyIconStepAmount;
            if (moneyIconAmount >= moneyIconMaximum)
                moneyIconAmount = moneyIconMaximum;
            moneyIconPosition.selectedDirection = 1;
        }
    } else if (repeated & 0x8000U) {
        if (disabled & 0x8000U)
            ExecuteAicaCommand(0x800ab, 0, 0);
        else {
            moneyIconAmount -= moneyIconStepAmount;
            if (moneyIconAmount < 1)
                moneyIconAmount = 0;
            moneyIconPosition.selectedDirection = 0;
        }
    } else if (pressed & 0x1000U) {
        if (disabled & 0x1000U)
            ExecuteAicaCommand(0x800ab, 0, 0);
        else {
            moneyIconAmount = moneyIconMaximum;
            moneyIconPosition.selectedDirection = 3;
        }
    } else if (pressed & 0x2000U) {
        if (disabled & 0x2000U)
            ExecuteAicaCommand(0x800ab, 0, 0);
        else {
            moneyIconAmount = 0;
            moneyIconPosition.selectedDirection = 2;
        }
    } else if ((pressed >> 9) & 1) {
        if (!moneyIconAmount) {
            SetMoneyIconPhase(4);
            moneyIconResult = -2;
        } else
            moneyIconAmount = 0;
    }
    if (previous != moneyIconAmount)
        ExecuteAicaCommand(0x500ab, 0, 0);
    RequestHelpIconSelection(hints);
#else
    std::uint32_t pressed = GetPadControlData(1), repeated = GetPadControlData(2);
    const std::int32_t previous = moneyIconAmount;
    std::uint32_t disabled = 0, hints = 0x600;
    if (moneyIconPosition.selectedDirection != 4)
        pressed = repeated = 0;
    if (!(moneyIconFlags & 2U))
        pressed &= 0xdfffU;
    if (!(moneyIconFlags & 1U))
        repeated &= 0xefffU;
    if (moneyIconMaximum <= moneyIconAmount) {
        disabled = 0x5000;
        repeated = (repeated & 0xafffU) | (pressed & 0x5000U);
    }
    if (moneyIconAmount < 1) {
        disabled |= 0xa400;
        hints = 0x200;
        repeated = (repeated & 0x5fffU) | (pressed & 0xa000U);
    }
    bool invalid = false;
    if (pressed & 0x400U) {
        if (!(pressed & disabled))
            SetMoneyIconPhase(4);
        else
            invalid = true;
    } else if (repeated & 0x4000U) {
        if (!(disabled & 0x4000U)) {
            moneyIconAmount = static_cast<std::int32_t>(static_cast<std::uint32_t>(moneyIconAmount) + static_cast<std::uint32_t>(moneyIconStepAmount));
            if (moneyIconMaximum <= moneyIconAmount)
                moneyIconAmount = moneyIconMaximum;
            moneyIconPosition.selectedDirection = 1;
        } else
            invalid = true;
    } else if (repeated & 0x8000U) {
        if (!(disabled & 0x8000U)) {
            moneyIconAmount = static_cast<std::int32_t>(static_cast<std::uint32_t>(moneyIconAmount) - static_cast<std::uint32_t>(moneyIconStepAmount));
            if (moneyIconAmount < 1)
                moneyIconAmount = 0;
            moneyIconPosition.selectedDirection = 0;
        } else
            invalid = true;
    } else if (pressed & 0x1000U) {
        if (!(disabled & 0x1000U)) {
            moneyIconAmount = moneyIconMaximum;
            moneyIconPosition.selectedDirection = 3;
        } else
            invalid = true;
    } else if (pressed & 0x2000U) {
        if (!(disabled & 0x2000U)) {
            moneyIconAmount = 0;
            moneyIconPosition.selectedDirection = 2;
        } else
            invalid = true;
    } else if (pressed & 0x200U) {
        if (!moneyIconAmount) {
            SetMoneyIconPhase(4);
            moneyIconResult = -2;
        } else
            moneyIconAmount = 0;
    }
    if (invalid)
        shenExecuteAICACommand(0x800ab, 0, 0);
    if (previous != moneyIconAmount)
        shenExecuteAICACommand(0x500ab, 0, 0);
    RequestHelpIconSelection(hints);
#endif
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CtrlMoneyIcon_Callback(void *) {
    switch (moneyIconPhase) {
    case 2:
        ExecuteAicaCommand(0x400ab, 0, 0);
        PrepareMoneyIconCurrency();
        SetMoneyIconPhase(1);
        iconToggleStateFor(4, true);
        moneyIconPosition.x = -516.0f;
        moneyIconPosition.selectedDirection = 4;
        moneyIconSelectionScale = 1.0f;
        moneyIconAmount = 0;
        moneyIconResult = 0;
        moneyIconSettleFrames = 4.0f;
        ChooseMoneyOptions(moneyIconFlags);
        if (moneyIconFlags & 8U)
            padcRefreshCurrentInputs(1);
        break;
    case 1:
        if (AdvanceIconPosition(&moneyIconPosition, 80.0f)) {
            if (moneyIconFlags & 8U) {
                moneyIconSavedHelpSelected = static_cast<std::uint16_t>(GetUiFlag(1));
                moneyIconSavedHelpVisible = static_cast<std::uint16_t>(GetUiFlag(0));
                moneyIconSavedHelpPresent = static_cast<std::int16_t>(GetUiFlag(2));
                if (moneyIconSavedHelpPresent)
                    SetHelpIconMenuFlags(0x21);
                else
                    AddHelpIconMenuFlags(0x21);
            }
            SetMoneyIconPhase(3);
        }
        break;
    case 5:
        if (RetreatIconPosition(&moneyIconPosition, -516.0f)) {
            SetMoneyIconPhase(0);
            iconToggleStateFor(4, false);
            ToggleRenderIcons(MoneyMessage, false);
            moneyIconDrawMode = 2;
            if (moneyIconFlags & 8U) {
                if (moneyIconSavedHelpPresent)
                    SetHelpIconMenuFlags(moneyIconSavedHelpVisible, static_cast<std::uint8_t>(moneyIconSavedHelpSelected));
                else
                    ResetHelpIconsToItemPrompt();
                padcRefreshCurrentInputs(0);
            }
            moneyIconFlags = 0;
            ReleaseMoneyTextSlots();
        }
        break;
    case 4:
        if (static_cast<std::int16_t>(moneyIconPosition.selectedDirection) == 4) {
            ExecuteAicaCommand(0x400ab, 0, 0);
            SetMoneyIconPhase(5);
        }
        break;
    case 3:
        if (moneyIconFlags & 8U) {
            moneyIconFrames -= 1.0f;
            if (!(moneyIconFrames > 0.0f)) {
                moneyIconPhase = 4;
                ExecuteAicaCommand(0x100ab, 0, 0);
                moneyIconAmount = 0;
                moneyIconResult = -1;
            } else
                UpdateMoneySelection();
        } else {
            if (!(moneyIconFrames < 30.0f))
                moneyIconFrames -= 1.0f;
            if (moneyIconFrames < 30.0f)
                UpdateMoneySettle();
        }
        break;
    case 0:
        break;
    }
    if (GetItemSelectionMode(4)) {
        HLib::ExecuteCallbackNow(AnimateMoneyUiElement, 0);
        if (moneyIconFlags & 8U) {
            AdvanceMoneySelectionScale();
            HLib::ExecuteCallbackNow(SetupMoneyIconForRendering, 0);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool AbortMoneyIcon(std::int32_t force) {
    if (!GetShouldRenderIcons(MoneyMessage) || !force)
        return false;
    iconToggleStateFor(4, false);
    ReleaseMoneyTextSlots();
    SetMoneyIconPhase(0);
    if (moneyIconFlags & 8U) {
        if (moneyIconSavedHelpPresent)
            SetHelpIconMenuFlags(moneyIconSavedHelpVisible, static_cast<std::uint8_t>(moneyIconSavedHelpSelected));
        else
            ResetHelpIconsToItemPrompt();
    }
    moneyIconFlags = 0;
    moneyIconDrawMode = 2;
    ToggleRenderIcons(MoneyMessage, false);
    moneyIconSelectionScale = 1.0f;
    moneyIconAmount = 0;
    moneyIconPosition.x = -516.0f;
    moneyIconPosition.selectedDirection = 4;
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern const std::uint16_t itemQuantityDigitIcons[10];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawSelectorDate(const Vector2 &position, float depth, const DateTime &date) {
    static const std::uint16_t weekdayIcons[7] = {0x507, 0x508, 0x50b, 0x50a, 0x509, 0x50c, 0x50d};
    selectorDateWeekday.depth = selectorDateDigit.depth = depth;
    selectorDateWeekday.y = selectorDateDigit.y = position.y;
    float x = position.x;
    std::int8_t digit = date.month / 10;
    if (digit) {
        selectorDateDigit.icon = itemQuantityDigitIcons[digit];
        selectorDateDigit.x = x;
        PrepareDrawSimple(selectorDateDigit, 0xff656565U);
    }
    x += selectorDateDigit.width;
    selectorDateDigit.icon = itemQuantityDigitIcons[static_cast<std::int8_t>(date.month % 10)];
    selectorDateDigit.x = x;
    PrepareDrawSimple(selectorDateDigit, 0xff656565U);
    x += selectorDateDigit.width;
    selectorDateDigit.icon = 0x41a;
    selectorDateDigit.x = x;
    PrepareDrawSimple(selectorDateDigit, 0xff656565U);
    x += selectorDateDigit.width;
    digit = date.day / 10;
    if (digit) {
        selectorDateDigit.icon = itemQuantityDigitIcons[digit];
        selectorDateDigit.x = x;
        PrepareDrawSimple(selectorDateDigit, 0xff656565U);
        x += selectorDateDigit.width;
    }
    selectorDateDigit.icon = itemQuantityDigitIcons[static_cast<std::int8_t>(date.day % 10)];
    selectorDateDigit.x = x;
    PrepareDrawSimple(selectorDateDigit, 0xff656565U);
    x += selectorDateDigit.width;
    x += selectorDateDigit.width * 0.5f;
    selectorDateWeekday.icon = weekdayIcons[static_cast<std::int8_t>(date.week)];
    selectorDateWeekday.depth = depth;
    selectorDateWeekday.x = x;
    PrepareDrawSimple(selectorDateWeekday, 0xff656565U);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void *SafeTryLoadFile(const char *directory, const char *filename, shen_name allocationName, std::uint32_t requiredHeapBytes) {
    const std::uint32_t npcLoading = GetNpcNewLoadState();
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const std::uint32_t size = shenOpenFile(directory, filename);
    if (size && !HLib::WouldAllocationFail(requiredHeapBytes, size)) {
        void *const data = HLib::AllocateTaskData(size, allocationName);
        if (data) {
            if (shenLoadFile(directory, filename, data, size)) {
                EV_NPC_NewLoadSw(npcLoading);
                return data;
            }
            HLib::FreeTaskData(data);
        }
    }
    EV_NPC_NewLoadSw(npcLoading);
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void GUI_Tasks_Enqueue() {
    for (std::int32_t slot = 0; slot < 2; ++slot)
        collectionSpriteOwners[slot] = 0;
    if (!GetCommonIconTextureCount())
        return;
    ResetMainIconState();
    ResetItemStateFlags();
    mainIconTask = HLib::EnqueueTaskWithoutParameter(InitMainIcon_Callback, 14, 0, SHEN_NAME(ICON));
    nativeGuiActive = 1;
    AllocateGuiPtxBacking();
    HLib::QueueTaskAfterCurrentTask(mainIconTask, InitItemSelectIcon_Callback, 15, 0, SHEN_NAME(ICIS));
    InitializeCommonItemTexture();
    InitializeMoneyIconParameters();
    HLib::QueueTaskAfterCurrentTask(mainIconTask, CtrlMoneyIcon_Callback, 8, 0, SHEN_NAME4('I', 'C', 'M', 'o'));
    InitSpeakIcon(mainIconTask, 8);
    HLib::QueueTaskAfterCurrentTask(mainIconTask, InitQuickIcon_Callback, 8, 0, SHEN_NAME4('I', 'C', 'Q', 'u'));
    InitNowQte(8);
    HP_SetUpHelpIcon();
    tmx_CtrlHandWatch(3);
    MM_MemoCtrl(3);
    NAVI_DummyWait_Enqueue(7);
    QueueItemEffectIconTask(3);
    CtrlSleepActionIcon(8);
    CtrlGauge_Init();
    NVW_CtrlNaviWindow_Enqueue(12);
    RegisterItems();
    CreatePAWNShops();
    Queue_pw_CtrlPawnWindow();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t GetCommonIconTextureCount() {
    return g_commonIconTextureCount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t FindLegacyCollectionSpriteSlot(std::int32_t owner) {
    for (std::int32_t slot = 0; slot < 2; ++slot)
        if (collectionSpriteOwners[slot] == owner)
            return slot;
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t HasLegacyCollectionSpriteSlot() {
    for (std::int32_t slot = 0; slot < 2; ++slot)
        if (collectionSpriteOwners[slot])
            return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint16_t GetPadControlData(std::uint8_t mode) {
    std::uint16_t result = 0;
    if (g_iconPads.current) {
        switch (mode) {
        case 1:
            result = ctrlGetPadInputBuffer0(g_iconPads.current)->pressedPriority;
            break;
        case 2:
            result = ctrlGetPadInputBuffer0(g_iconPads.current)->repeatedPresses;
            break;
        case 0:
            result = ctrlGetPadInputBuffer0(g_iconPads.current)->held;
            break;
        }
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void PlayQuickIconResult(std::uint16_t flags, std::int32_t success) {
    if ((flags & 1U) == 0)
        return;
    const std::uint32_t variant = (flags >> 10U) & 1U;
    if (flags & 0x80U)
        return;
    if (success) {
        if (flags & 8U)
            ExecuteAicaCommand(quickIconResultSounds[variant][1], 0, 0);
        else
            ExecuteAicaCommand(quickIconResultSounds[variant][0], 0, 0);
    } else if (flags & 8U)
        ExecuteAicaCommand(quickIconResultSounds[variant][0], 0, 0);
    else
        ExecuteAicaCommand(quickIconResultSounds[variant][1], 0, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void UpdateQuickIconBlink() {
    std::int32_t voiced;
    if (g_quickIconState.flags & 0x400U)
        voiced = 0;
    else
        voiced = 1;
    if (g_quickIconState.phase == 3) {
        if (g_quickIconUnpressed == 1) {
            if (++g_quickIconBlinkPhase > 2) {
                g_quickIconUnpressed = 0;
                g_quickIconBlinkPhase = 0;
            }
            if (g_quickIconBlinkPhase == 1 && (g_quickIconState.flags & 1U) && voiced)
                ExecuteAicaCommand(0x700ab, 0, 0);
        } else if (++g_quickIconBlinkPhase > 1) {
            g_quickIconUnpressed = 1;
            g_quickIconBlinkPhase = 0;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void UpdateQuickIconSingleButton() {
    std::uint16_t pressed = GetPadControlData(1);
    pressed &= 0xf6c6U;
    pressed &= g_quickIconButtonMask;
    std::uint16_t status = quickIconDirectionalStatuses[static_cast<std::uint8_t>(pressed >> 12U)];
    std::int8_t faceButtons = 0;
    if ((pressed >> 10U) & 1U) {
        status |= 0x100;
        ++faceButtons;
    }
    if (pressed & 0x200U) {
        status |= 0x200;
        ++faceButtons;
    }
    if (pressed & 2U) {
        status |= 0x800;
        ++faceButtons;
    }
    if (pressed & 4U) {
        status |= 0x400;
        ++faceButtons;
    }
    if (!(status & 0xf00U)) {
        if (g_quickIconState.requiredStatus == 1 && (pressed & 0x1000U))
            status = 1;
        if (g_quickIconState.requiredStatus == 16 && (pressed & 0x2000U))
            status = 16;
        if (g_quickIconState.requiredStatus == 64 && (pressed & 0x4000U))
            status = 64;
        if (g_quickIconState.requiredStatus == 4 && (pressed & 0x8000U))
            status = 4;
    }
    if (status) {
        g_quickIconState.currentStatus = static_cast<std::int16_t>(status);
        if (g_quickIconState.flags & 0x40U) {
            PlayQuickIconResult(g_quickIconState.flags, 0);
            g_quickIconState.currentStatus = 0;
        } else if (faceButtons > 1 || ((g_quickIconState.currentStatus & 0xf00) && (g_quickIconState.currentStatus & 0xff))) {
            PlayQuickIconResult(g_quickIconState.flags, 0);
            g_quickIconState.currentStatus = 0;
        } else if (static_cast<std::uint16_t>(g_quickIconState.currentStatus) == g_quickIconState.requiredStatus) {
            g_quickIconState.currentStatus |= 0x2000;
            PlayQuickIconResult(g_quickIconState.flags, 1);
        } else {
            g_quickIconState.currentStatus = 0;
            PlayQuickIconResult(g_quickIconState.flags, 0);
        }
        g_quickIconState.phase = 4;
        g_quickIconUnpressed = 1;
    } else if (g_quickIconState.maximumFrames > 0.0f) {
        g_quickIconState.elapsedFrames += 1.0f;
        if (g_quickIconState.elapsedFrames > g_quickIconState.maximumFrames) {
            g_quickIconState.currentStatus = static_cast<std::int16_t>(0xa000U);
            g_quickIconTerminalStatus = 0xa000;
            g_quickIconState.phase = 4;
            g_quickIconState.maximumFrames = 0.0f;
            if (g_quickIconState.flags & 0x40U)
                PlayQuickIconResult(g_quickIconState.flags, 1);
            else
                PlayQuickIconResult(g_quickIconState.flags, 0);
            g_quickIconUnpressed = 1;
        }
    }
    UpdateQuickIconBlink();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void UpdateQuickIconSplitDirections() {
    std::uint32_t pressed = GetPadControlData(1);
    pressed &= 0xf6c6U;
    std::uint16_t status = quickIconDirectionalStatuses[static_cast<std::uint8_t>(pressed >> 12U)];
    if (g_quickIconState.requiredStatus & 0x40U) {
        status &= 0xeeU;
        if (pressed & 0x4000U)
            status = 64;
        if (pressed & 0x8000U)
            status = 4;
    } else {
        status &= 0xbbU;
        if (pressed & 0x1000U)
            status = 1;
        if (pressed & 0x2000U)
            status = 16;
    }
    if (g_quickIconState.requiredStatus & status) {
        g_quickIconState.currentStatus = status | 0x2000U;
        PlayQuickIconResult(g_quickIconState.flags, 1);
        g_quickIconState.phase = 4;
    } else if (g_quickIconState.maximumFrames > 0.0f) {
        g_quickIconState.elapsedFrames += 1.0f;
        if (g_quickIconState.elapsedFrames > g_quickIconState.maximumFrames) {
            g_quickIconState.currentStatus = 0xa000;
            g_quickIconTerminalStatus = 0xa000;
            g_quickIconState.phase = 4;
            g_quickIconState.maximumFrames = 0.0f;
            PlayQuickIconResult(g_quickIconState.flags, 0);
            g_quickIconUnpressed = 1;
        }
    }
    UpdateQuickIconBlink();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void DrawQuickIcon(void *) {
    g_quickIconSprite.x = quickIconPosition.x;
    if (g_quickIconState.flags & 0x20U) {
        for (std::int8_t bit = 0; bit < 8; ++bit) {
            if ((g_quickIconState.requiredStatus >> bit) & 1) {
                g_quickIconSprite.rotation = bit * 0x2000;
#if SHENMUE_S2_GCC295
                g_quickIconSprite.x = CalculateFixedSine(g_quickIconSprite.rotation) * 55.0f + quickIconPosition.x;
                g_quickIconSprite.y = CalculateFixedCosine(g_quickIconSprite.rotation) * -55.0f + quickIconPosition.y;
#else
                g_quickIconSprite.x = __builtin_fmaf(CalculateFixedSine(g_quickIconSprite.rotation), 55.0f, quickIconPosition.x);
                g_quickIconSprite.y = __builtin_fmaf(CalculateFixedCosine(g_quickIconSprite.rotation), -55.0f, quickIconPosition.y);
#endif
                PrepareDrawWithMode(g_quickIconSprite);
            }
        }
    } else
        PrepareDrawWithMode(g_quickIconSprite);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static inline void RetireQuickIcon() {
    padcRefreshCurrentInputs(0);
    ToggleRenderIcons(Qte, false);
    iconToggleStateFor(3, false);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CtrlQuickIcon_Callback(void *) {
    switch (g_quickIconState.phase) {
    case 0:
        break;
    case 2:
        if ((g_quickIconState.flags & 1U) && !(g_quickIconState.flags & 0x400U))
            ExecuteAicaCommand(0x700ab, 0, 0);
        g_quickIconState.phase = 1;
        if (!(g_quickIconState.flags & 0x200U))
            padcRefreshCurrentInputs(1);
        iconToggleStateFor(3, true);
        g_quickIconUnpressed = 1;
        g_quickIconTerminalStatus = 0xffff;
        quickIconPosition.x = 320.0f;
        break;
    case 1:
        g_quickIconState.phase = 3;
        break;
    case 5:
        if ((g_quickIconState.flags & 4U) && g_quickIconTerminalStatus != 0xa000U) {
            g_quickIconState.phase = 0;
            RetireQuickIcon();
        } else if (RetreatIconPosition(&quickIconPosition, -176.0f)) {
            g_quickIconState.phase = 0;
            RetireQuickIcon();
        }
        break;
    case 4:
        if (g_quickIconState.flags & 0x10U) {
            g_quickIconState.phase = 0;
            RetireQuickIcon();
            quickIconPosition.x = -176.0f;
        } else
            g_quickIconState.phase = 5;
        break;
    case 3:
        if (g_quickIconState.flags & 0x100U)
            UpdateQuickIconBlink();
        else if (g_quickIconState.flags & 0x20U)
            UpdateQuickIconSplitDirections();
        else
            UpdateQuickIconSingleButton();
        break;
    }
    if ((g_quickIconState.flags & 2U) && GetItemSelectionMode(3) && g_quickIconUnpressed)
        HLib::ExecuteCallbackNow(DrawQuickIcon, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitQuickIcon_Callback(void *) {
    g_quickIconState.phase = 0;
    InitializeQuickIconSprite();
    g_quickIconLoadingReleaseFlag = 0;
    g_quickIconState.legacyMode = 4;
    g_quickIconState.legacyReserved = 0;
    g_quickIconState.legacyTexture = -1;
    g_quickIconState.legacyWidth = 32;
    HLib::SetCurrentTaskTo(CtrlQuickIcon_Callback);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool ResetQuickIconState(std::int32_t force) {
    if (GetShouldRenderIcons(Qte) && force) {
        g_quickIconButtonMask = 0;
        g_quickIconState.phase = 0;
        iconToggleStateFor(3, false);
        quickIconPosition.x = -176.0f;
        g_quickIconTerminalStatus = 0xffff;
        return true;
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::uint8_t DecodeSelectorInput(std::uint16_t pressed, std::uint8_t allowed) {
    static const std::uint8_t selectorInputDirections[16] = {13, 0, 4, 13, 6, 7, 5, 13, 2, 1, 3, 13, 13, 13, 13, 13};
    std::uint8_t direction = selectorInputDirections[static_cast<std::uint16_t>(pressed >> 12U)];
    std::int8_t index;
    for (index = 0; index < 12; ++index) {
        if ((allowed & 1U) && direction == index)
            break;
        allowed >>= 1U;
    }
    if (index > 11) {
        direction = 13;
        if (pressed & 0x200U)
            direction = 9;
        else if (pressed & 0x400U)
            direction = 8;
        else if (pressed & 2U)
            direction = 11;
        else if (pressed & 4U)
            direction = 10;
    }
    return direction;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitSpeakIcon(HLib::Task *parent, std::uint8_t queue) {
    const std::int8_t signedQueue = static_cast<std::int8_t>(queue);
    InitializeCtrlSpeakIconState();
    HLib::QueueTaskAfterCurrentTask(parent, CtrlSpeakIcon_Callback, signedQueue, 0, SHEN_NAME4('I', 'C', 'S', 'p'));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InitializeCtrlSpeakIconState() {
    std::int8_t slot;
    ctrlSpeakIconState = 0;
    ctrlSpeakResultChoiceId = 0xffff;
    ctrlSpeakOpenRequested = 0;
    ctrlSpeakTimeoutFrames = 10800;
    ctrlSpeakElapsedFrames = 0;
    ctrlSpeakChoiceMode = 0;
    ctrlSpeakLabelPhase = 0;
    ctrlSpeakChoiceSpecial = 0;
    ctrlSpeakLabelMode = 0;
    ctrlSpeakSpecialLabelWidthScale = 2.0f;
    ctrlSpeakDefaultChoiceId = 0xffff;
    ctrlSpeakSelectorPosition.x = -176.0f;
    ctrlSpeakSelectorPosition.y = 91.0f;
    ctrlSpeakSelectorPosition.velocity = 1.0f;
    ctrlSpeakSelectorPosition.reservedScale = 0.0f;
    ctrlSpeakSelectorPosition.selectedDirection = 4;
    ctrlSpeakSelectorPosition.depth = 1.0f;
    for (slot = 0; slot < 4; ++slot) {
        ctrlSpeakLabelSprites[slot].icon = 0xffff;
        ctrlSpeakLabelSprites[slot].flags = 0;
        ctrlSpeakLabelSprites[slot].x = ctrlSpeakSelectorPosition.x + ctrlSpeakLabelOffsetX[slot];
        ctrlSpeakLabelSprites[slot].y = ctrlSpeakSelectorPosition.y + ctrlSpeakLabelOffsetY[slot];
        ctrlSpeakLabelSprites[slot].width = 256;
        ctrlSpeakLabelSprites[slot].height = 32;
        ctrlSpeakLabelSprites[slot].scaleX = 1.0f;
        ctrlSpeakLabelSprites[slot].scaleY = 1.0f;
        ctrlSpeakLabelSprites[slot].rotation = 0;
        ctrlSpeakLabelSprites[slot].depth = ctrlSpeakLabelDepths[slot] + 1.0f;
        ctrlSpeakLabelSprites[slot].alignment = 5;
    }
    for (slot = 0; slot < 4; ++slot) {
        ctrlSpeakTexturePoolHandles[slot] = ctrlSpeakChoiceTextHandles[slot] = -1;
        ctrlSpeakTexturePoolFlags[slot] = 0;
    }
    for (std::int32_t slot = 0; slot < 4; ++slot)
        ctrlSpeakTexturePoolHandles[slot] =
            LoadAndFindTEXN_Wrapper(SetupPtxTexture(256, SHEN_NAME4('A', 'S', 'A', 'S'), SHEN_NAME4('0', '0', '0', '0' + slot)));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void ProcessSpeakSelection() {
    const std::uint16_t pressed = GetPadControlData(1);
    std::uint8_t input = DecodeSelectorInput(pressed, 0x55);
    switch (input) {
    case 0:
    case 2:
    case 4:
    case 6: {
        const std::uint8_t slot = selectorDirectionToSlot[input];
        if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) != 0xffff) {
            ExecuteAicaCommand(0x500ab, 0, 0);
            ctrlSpeakIconState = 3;
            ctrlSpeakSelectorPosition.selectedDirection = slot;
            ctrlSpeakResultChoiceId = static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]);
            ctrlSpeakRetainedMotion = 0.0f;
            ctrlSpeakSelectionAnimationFrame = -1;
            ctrlSpeakLabelSprites[slot].depth = 7.0f;
        } else
            input = 13;
        break;
    }
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
        break;
    case 9:
        if (static_cast<std::int16_t>(ctrlSpeakMode) == 2 || static_cast<std::int16_t>(ctrlSpeakMode) == 5) {
            ExecuteAicaCommand(0x100ab, 0, 0);
            ctrlSpeakIconState = 3;
            ctrlSpeakResultChoiceId = 0xffff;
        }
        break;
    }
    if ((pressed & 0xc0U) && !ctrlSpeakLabelPhase) {
        ctrlSpeakChoiceMode = (ctrlSpeakChoiceMode + 1) & 3;
        if (ctrlSpeakChoiceSpecial && ctrlSpeakChoiceMode == 1) {
            ++ctrlSpeakChoiceMode;
            ctrlSpeakLabelPhase = -1;
        } else {
            ctrlSpeakLabelPhase = 1;
            for (std::int32_t slot = 0; slot < 4; ++slot) {
                ctrlSpeakTargetChoiceWidths[slot] = static_cast<std::int32_t>(
                    GetCtrlSpeakChoiceWidth(reinterpret_cast<const std::uint16_t *>(&ctrlSpeakChoiceIds[slot]), ctrlSpeakChoiceMode & 1U));
                if (ctrlSpeakTargetChoiceWidths[slot] != ctrlSpeakChoiceWidths[slot])
                    ctrlSpeakChoiceWidthDeltas[slot] = ctrlSpeakTargetChoiceWidths[slot] - ctrlSpeakChoiceWidths[slot];
                else
                    ctrlSpeakChoiceWidthDeltas[slot] = 0;
            }
            ctrlSpeakIconState = 5;
            ctrlSpeakResizeFrame = 0.0f;
        }
    }
    if (input == 13) {
        if (++ctrlSpeakElapsedFrames >= ctrlSpeakTimeoutFrames) {
            ctrlSpeakIconState = 3;
            if (ctrlSpeakDefaultChoiceId == 0xffff) {
                ExecuteAicaCommand(0x100ab, 0, 0);
                ctrlSpeakResultChoiceId = 0xffff;
            } else {
                ExecuteAicaCommand(0x500ab, 0, 0);
                for (std::int8_t slot = 0; slot < 4; ++slot)
                    if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) == ctrlSpeakDefaultChoiceId) {
                        ctrlSpeakResultChoiceId = ctrlSpeakDefaultChoiceId;
                        ctrlSpeakSelectorPosition.selectedDirection = slot;
                        ctrlSpeakLabelSprites[slot].depth = 7.0f;
                        break;
                    }
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void PrepareDrawWithUVOffsets(const LegacyIconSprite &sprite, float u, float v, float width, float height, std::uint32_t colour) {
    const std::int8_t *const uv = GetCommonIconRect(sprite.icon);
    LegacySpritePacket packet;
    packet.quad.flags = 0;
    packet.colour = colour | 0x00ffffffU;
    packet.secondaryColour = colour & 0xffffffU;
    packet.reservedTail = 0;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateToInt32(sprite.x), TruncateToInt32(sprite.y), sprite.width,
        sprite.height, sprite.scaleX, sprite.scaleY, sprite.rotation);
    packet.uv[0] = static_cast<float>(uv[0]) * 0.125f + u;
    packet.uv[1] = static_cast<float>(uv[1]) * 0.125f + v;
    packet.uv[2] = static_cast<float>(uv[2]) * 0.125f + width;
    packet.uv[3] = static_cast<float>(uv[3]) * 0.125f + height;
    packet.quad.flags |= sprite.flags | 0x400U;
    DrawLegacySpritePacket(packet, GetCommonIconTextureIndex(sprite.icon), sprite.depth);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void PrepareSpeakTextSlots() {
    for (std::int8_t slot = 0; slot < 4; ++slot) {
        if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) == 0xffffU)
            ctrlSpeakLabelSprites[slot].icon = 0xffff;
        else
            PrepareSpeakTextSlot(slot);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void PrepareSpeakTextSlot(std::int8_t slot) {
    static const CtrlSpeakLabel alternate[2] = {{"\xa4\xcf\xa4\xa4", "\xa5\xcf\xa5\xa4"}, {"\xa4\xa4\xa4\xa4\xa4\xa8", "\xa5\xa4\xa5\xa4\xa5\xa8"}};
    const CtrlSpeakLabel *label;
    if (static_cast<std::int16_t>(ctrlSpeakMode) > 2)
        label = &alternate[static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot])];
    else
        label = &ctrlSpeakLabels[static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot])];
    std::int32_t form = 0;
    const char *text;
    if ((ctrlSpeakChoiceMode & 1) == 0)
        form = 1;
    if (form) {
        text = label->primary;
        if (!text) {
            form = 0;
            text = label->alternate;
        }
    } else {
        text = label->alternate;
        if (!text) {
            form = 1;
            text = label->primary;
        }
    }
    if (text && ctrlSpeakTexturePoolHandles[slot] >= 0)
        ctrlSpeakChoiceTextHandles[slot] = AcquireCtrlSpeakTextHandle(text, form);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t FindFreeSpeakTextSlot() {
    for (std::int32_t slot = 0; slot < 4; ++slot)
        if ((ctrlSpeakTexturePoolFlags[slot] & 1U) == 0)
            return slot;
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t AcquireCtrlSpeakTextHandle(const char *text, std::int32_t centered) {
    std::int32_t handle = -1;
    const std::int32_t slot = FindFreeSpeakTextSlot();
    if (slot >= 0) {
        PublishLegacyItemLabel(text, 256, ctrlSpeakTexturePoolHandles[slot], centered);
        handle = ctrlSpeakTexturePoolHandles[slot];
        ctrlSpeakTexturePoolFlags[slot] |= 1U;
    }
    return handle;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetMoneyIconPhase(std::uint8_t phase) {
    moneyIconPhase = phase;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void InitializeMoneyIconSelector() {
    moneyIconSelectorPosition.x = -176.0f;
    moneyIconSelectorPosition.y = 91.0f;
    moneyIconSelectorPosition.velocity = 1.0f;
    moneyIconSelectorPosition.reservedScale = 0.0f;
    moneyIconSelectorPosition.selectedDirection = 4;
    moneyIconSelectionScale = 1.0f;
    std::int32_t count;
    for (count = 0; count < 2; ++count) {
        moneyIconTextSprites[count].scaleX = 1.0f;
        moneyIconTextSprites[count].scaleY = 1.0f;
        moneyIconTextSprites[count].width = 256;
        moneyIconTextSprites[count].height = 32;
        moneyIconTextSprites[count].depth = 2.0f;
        moneyIconTextSprites[count].alignment = 5;
        moneyIconTextSprites[count].rotation = 0;
        moneyIconTextSprites[count].flags = 0;
        moneyIconTextSprites[count].x = moneyIconSelectorPosition.x;
        moneyIconTextSprites[count].y = moneyIconSelectorPosition.y;
        moneyIconTextSprites[count].icon = 0xffff;
    }
    LegacyIconSprite *sprite = moneyIconDirections;
    count = 4;
    do {
        sprite->y = moneyIconSelectorPosition.y;
        sprite->x = moneyIconSelectorPosition.x;
        sprite->width = 96;
        sprite->height = 50;
        sprite->scaleX = 1.0f;
        sprite->scaleY = 1.0f;
        sprite->rotation = 0;
        sprite->depth = 2.0f;
        sprite->alignment = 5;
        sprite->icon = 0x608;
        ++sprite;
    } while (--count);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int8_t GetMoneyIconCurrency() {
    return static_cast<std::int8_t>(moneyIconCurrency);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void InitializeMoneyIconSprites() {
    moneyIconPosition.x = -516.0f;
    moneyIconPosition.y = 340.0f;
    moneyIconPosition.velocity = 1.0f;
    moneyIconPosition.reservedScale = 0.0f;
    moneyIconPosition.selectedDirection = 4;
    moneyIconPosition.depth = 3.0f;
    moneyIconCurrencySprite.flags = 0;
    moneyIconCurrencySprite.x = moneyIconPosition.x;
    moneyIconCurrencySprite.y = moneyIconPosition.y + 10.0f;
    moneyIconCurrencySprite.width = 16;
    moneyIconCurrencySprite.height = 16;
    moneyIconCurrencySprite.scaleX = 1.0f;
    moneyIconCurrencySprite.scaleY = 1.0f;
    moneyIconCurrencySprite.rotation = 0;
    moneyIconCurrencySprite.depth = moneyIconPosition.depth;
    moneyIconCurrencySprite.alignment = 5;
    moneyIconSprite.icon = 0x608;
    moneyIconSprite.flags = 0;
    moneyIconSprite.x = moneyIconPosition.x;
    moneyIconSprite.y = moneyIconPosition.y;
    moneyIconSprite.width = 80;
    moneyIconSprite.height = 40;
    moneyIconSprite.scaleX = 1.0f;
    moneyIconSprite.scaleY = 1.0f;
    moneyIconSprite.rotation = 0;
    moneyIconSprite.depth = moneyIconPosition.depth - 1.0f;
    moneyIconSprite.alignment = 5;
    LegacyIconSprite *sprite = quantityDigits;
    std::int32_t count = 8;
    do {
        sprite->icon = 0x406;
        sprite->flags = 0;
        sprite->x = moneyIconPosition.x;
        sprite->y = moneyIconPosition.y + 10.0f;
        sprite->width = 16;
        sprite->height = 16;
        sprite->scaleX = 1.0f;
        sprite->scaleY = 1.0f;
        sprite->rotation = 0;
        sprite->depth = moneyIconPosition.depth;
        sprite->alignment = 5;
        ++sprite;
    } while (--count);
    sprite = quantityBrackets;
    count = 2;
    do {
        sprite->icon = count != 1 ? 0x41e : 0x41d;
        sprite->flags = 0;
        sprite->x = moneyIconPosition.x;
        sprite->y = moneyIconPosition.y;
        sprite->width = 16;
        sprite->height = 16;
        sprite->scaleX = 1.0f;
        sprite->scaleY = 1.5f;
        sprite->rotation = 0;
        sprite->depth = moneyIconPosition.depth;
        sprite->alignment = 5;
        ++sprite;
    } while (--count);
    for (count = 0; count < 2; ++count) {
        moneyIconSigns[count].flags = 0;
        moneyIconSigns[count].rotation = 0;
        moneyIconSigns[count].width = 16;
        moneyIconSigns[count].height = 16;
        moneyIconSigns[count].scaleX = 1.0f;
        moneyIconSigns[count].scaleY = 1.0f;
        moneyIconSigns[count].alignment = 5;
        moneyIconSigns[count].depth = 3.0f;
    }
    moneyIconSigns[0].icon = 0x415;
    moneyIconSigns[1].icon = 0x41b;
    InitializeMoneyIconSelector();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeMoneyIconParameters() {
    moneyIconDisplay = 0;
    moneyIconCurrency = 1;
    moneyIconTarget = 0;
    moneyIconAmount = 0;
    InitializeMoneyIconSprites();
    SetMoneyIconPhase(0);
    moneyIconDrawMode = 2;
    moneyIconMaximum = 0;
    moneyIconStepAmount = 5;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CtrlItemEffectIcon_Callback(void *) {
    switch (static_cast<std::int8_t>(itemEffectIconPhase)) {
    case 0:
        break;
    case 2:
        itemEffectIconPhase = 1;
        ExecuteAicaCommand(0x400ab, 0, 0);
        itemEffectAudio = shenPlayAudioWithFlags("JINGURU01", 2);
        break;
    case 1:
        if (AdvanceIconPosition(&itemEffectPosition, 50.0f))
            itemEffectIconPhase = 3;
        break;
    case 3:
        if (++itemEffectIconFrames > 29 && !cvsHasValidConverseData(itemEffectAudio)) {
            itemEffectIconPhase = 4;
            itemEffectIconFrames = 0;
        }
        break;
    case 4:
        if (RetreatIconPosition(&itemEffectPosition, -672.0f))
            itemEffectIconPhase = 5;
        break;
    case 5:
        itemEffectIconPhase = 0;
        itemEffectPosition.x = -672.0f;
        itemEffectPosition.y = 55.0f;
        itemEffectLabelTexture = -1;
        ToggleRenderIcons(UI_ICON(ItemEffectMessage), false);
        iconToggleStateFor(5, false);
        if (itemEffectIconTask)
            HLib::SetupTimedTask(itemEffectIconTask, 0);
        break;
    default:
        itemEffectIconPhase = 5;
        break;
    }
    if (GetItemSelectionMode(5))
        HLib::ExecuteCallbackNow(ItemEffectRender_Callback, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool GiveItemOf(std::uint32_t item) {
    return ToggleItemEffect(item, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool ToggleItemEffect(std::uint32_t item, std::int32_t amount) {
    if (GetShouldRenderIcons(UI_ICON(Any)) || !itemEffectIconTask)
        return false;
    const std::uint32_t length = static_cast<std::uint32_t>(PrepareLegacyInventoryItemLabel(&itemEffectLabelTexture, item));
    if (itemEffectLabelTexture >= 0) {
        itemEffectColumns = static_cast<std::uint8_t>(length * 14U / 40U + 1U);
        if (length * 14U % 40U)
            itemEffectColumns = static_cast<std::uint8_t>(itemEffectColumns + 1U);
        if (itemEffectColumns < 6)
            itemEffectColumns = 6;
        itemEffectSprite.x = -672.0f;
        itemEffectSprite.y = 55.0f;
        itemEffectPosition.x = -672.0f;
        itemEffectPosition.y = 340.0f;
        HLib::ResolveDeferredCallback(itemEffectIconTask);
        itemEffectIconPhase = 2;
        iconToggleStateFor(5, true);
        itemEffectSprite.icon = GetItemEffectIcon(item);
        ToggleRenderIcons(UI_ICON(ItemEffectMessage), true);
        itemEffectAmount = amount;
        return true;
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void ApplyItemSelectorDelayedResult() {
    NativeItemSelectorState *state = &g_itemSelectorState;
    switch (GetItemSelectorDelayedResult()) {
    case 2:
        state->actionResult = (static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(state->selectedItem))) << 16U) | 2U;
        SetItemSelectorPhase(4);
        break;
    case 4:
        break;
    case 32:
        SetItemSelectorPhase(6);
        iconToggleStateFor(1, false);
        CheckHelpIcon(0, 0, CheckHelpIconOp_Set);
        break;
    }
    SetItemSelectorDelayedResult(0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RenderLegacyItemSelector(void *parameter) {
    NativeItemSelectorState &state = *static_cast<NativeItemSelectorState *>(parameter);
    DateTime date;
    LegacyMoneyLayout layout;
    Vector2 cursor;
    float left, x, width, height, y, gridX, lowerY;
    const std::int32_t columns = state.columns, rows = state.visibleRows;
    width = static_cast<float>(40 * columns);
    height = static_cast<float>(40 * rows);
    left = itemEffectPosition.x + 64.0f;
    y = itemEffectPosition.y + -20.0f;
    x = left + 11.0f;
    gridX = x + 24.0f;
    const std::int32_t menu = state.activeMenu;
    lowerY = (height + y) + 32.0f;
    DrawItemSelectorInventory(columns, 384, gridX, y + 24.0f, 5.0f, 0, 0);
    if (menu == 8 && state.count > 0) {
#if SHENMUE_S2_GCC295
        cursor.x = static_cast<float>(state.column) * 40.0f + gridX;
        cursor.y = static_cast<float>(state.row) * 40.0f + (y + 24.0f);
#else
        cursor.x = __builtin_fmaf(static_cast<float>(state.column), 40.0f, gridX);
        cursor.y = __builtin_fmaf(static_cast<float>(state.row), 40.0f, y + 24.0f);
#endif
    } else {
#if SHENMUE_S2_GCC295
        cursor.x = static_cast<float>(menu) * 40.0f + gridX;
#else
        cursor.x = __builtin_fmaf(static_cast<float>(menu), 40.0f, gridX);
#endif
        cursor.y = lowerY;
    }
    DrawLegacySelectionGlow(cursor, state.glowColour, *reinterpret_cast<bool *>(&state.glowBrightening), 8.0f);
    cursor.x = gridX;
    cursor.y = lowerY;
    DrawItemSelectorLowerActions(cursor, 5.0f);
    DrawLegacyWindowTiles(cursor.x, cursor.y, columns, 1, 2.0f, 1024);
    DrawLegacyWindowBackground(x + 4.0f, y + 4.0f, width, height + 144.0f, 3.0f, 1024);
    DrawLegacyWindowEdges(x, y + 27.0f, rows + 3, columns, 5.0f, 1024);
    DrawLegacyWindowTiles(gridX, y + 24.0f, columns, rows, 2.0f, 1024);
    DrawLegacyWindowHorizontal(left, y, columns, 1, 0, 5.0f, 1024);
    y += height + 8.0f;
    for (std::int32_t index = 0; index < 3; ++index, y += 48.0f)
        DrawLegacyWindowHorizontal(left, y, columns, 0, 0, 5.0f, 1024);
    DrawLegacyWindowHorizontal(left, y, columns, 1, 1, 5.0f, 1024);
#if SHENMUE_S2_GCC295
    cursor.x = width * 0.5f + x;
#else
    cursor.x = __builtin_fmaf(width, 0.5f, x);
#endif
    cursor.y = y - 24.0f;
    if (menu == 8) {
        float offset = 0.0f;
        if (state.selectedIndex >= 0) {
            const std::int32_t quantity = g_keyItemStock[state.selectedIndex];
            if (quantity > 1) {
                const char *const text = GetInventoryItemLabelByIndex(0, static_cast<std::uint32_t>(state.selectedIndex), 0);
                std::uint32_t length = 8U;
                if (text)
                    length = UI_STRLEN(text);
                offset = static_cast<float>(length * 2U);
                DrawLegacyItemQuantity((x + width) - 4.0f, cursor.y + 4.0f, quantity, 5.0f);
            }
        }
        cursor.x -= offset;
        if (state.selectedItem == 21) {
            Vector2 coin = cursor;
            coin.x -= 72.0f;
            coin.x += 164.0f;
            coin.y += 12.0f;
            DrawMoneyAmount(&coin, 5.0f, GetPlayerStats(8), 0, 0, 1.0f);
        }
    }
    DrawLegacyItemLabel(&itemEffectLabelTexture, &cursor, 5.0f, 0xff000000U);
    DrawLegacyWindowTiles(gridX, cursor.y, columns, 1, 2.0f, 1024);
    DrawLegacyWindowTiles(gridX, cursor.y - 48.0f, columns, 1, 2.0f, 1024);
    itemSelectorHeadingSprite.y = y - 72.0f;
    itemSelectorHeadingSprite.x = (x + 50.0f) + static_cast<float>(20 * (columns - 8));
    PrepareDrawWithMode(itemSelectorHeadingSprite);
    cursor.x = itemSelectorHeadingSprite.x + 82.0f;
    cursor.y = itemSelectorHeadingSprite.y + 10.0f;
    Vector2 datePosition = cursor;
    datePosition.x += 48.0f;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    GetCurrentCalendarRecord(date);
#else
    date = g_currentDateTime;
#endif
    DrawSelectorDate(datePosition, 7.0f, date);
    LegacyMoneyLayout *const money = &layout;
    money->position = cursor;
    money->scale = 1.0f;
    money->depth = 6.0f;
    DrawNativeMoneyAmount(money, GetPlayerStats(2), 1, 0, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void SetupMoneyIconForRendering(void *) {
    std::int32_t visible[4];
    visible[0] = 1;
    visible[1] = 1;
    visible[2] = 0;
    visible[3] = 0;
    float x = moneyIconPosition.x + 43.0f;
    const float y = moneyIconSelectorPosition.y + -14.6875f;
    LegacyIconSprite *sprite = moneyIconDirections;
    if (moneyIconFlags & 2U)
        visible[2] = 1;
    if (moneyIconFlags & 1U)
        visible[3] = 1;
    for (std::int32_t index = 0; index < 4; ++index, ++sprite) {
        sprite->x = static_cast<float>(MONEY_DIRECTION_X[index]) + x;
        sprite->y = static_cast<float>(MONEY_DIRECTION_Y[index]) + y;
        if (static_cast<std::int16_t>(moneyIconPosition.selectedDirection) == index) {
            sprite->scaleX = moneyIconSelectionScale;
            sprite->scaleY = moneyIconSelectionScale;
            if (!(moneyIconSelectionScale > 1.0f))
                moneyIconPosition.selectedDirection = 4;
        }
        if (visible[index])
            PrepareDrawSimpleBlack(*sprite, 0xb0ffffffU);
    }
    float lead = -16.0f;
    float scale;
    const float digits = GetSelectorNumberDigits(moneyIconStepAmount);
    if (!(digits < 0.0f))
        lead *= digits;
    x = moneyIconPosition.x - -54.0f;
    DrawMoneyDigits(moneyIconDirections[1].x + 16.0f, moneyIconDirections[1].y, moneyIconDirections[1].scaleX, moneyIconDirections[1].depth + 1.0f,
        moneyIconStepAmount, 0xff656565U);
    sprite = moneyIconSigns;
    sprite->scaleX = sprite->scaleY = scale = moneyIconDirections[1].scaleX;
    sprite->x = lead * scale + (moneyIconDirections[1].x + 16.0f);
    sprite->y = moneyIconDirections[1].y;
    PrepareDrawSimple(*sprite, 0xff656565U);
    DrawMoneyDigits(moneyIconDirections[0].x + 16.0f, moneyIconDirections[0].y, moneyIconDirections[0].scaleX, moneyIconDirections[0].depth + 1.0f,
        moneyIconStepAmount, 0xff656565U);
    ++sprite;
    sprite->scaleX = sprite->scaleY = scale = moneyIconDirections[0].scaleX;
    sprite->x = lead * scale + (moneyIconDirections[0].x + 16.0f);
    sprite->y = moneyIconDirections[0].y;
    PrepareDrawSimple(*sprite, 0xff656565U);
    Vector2 position;
    position.x = x + -6.0f;
    position.y = 171.0f;
    DrawMoneyAmount(&position, 3.0f, moneyIconAmount, static_cast<std::int8_t>(moneyIconCurrency), 0, 1.0f);
    if (moneyIconFlags & 1U)
        DrawMoneyOptionText(1, 3);
    if (moneyIconFlags & 2U)
        DrawMoneyOptionText(0, 2);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void DrawNativeMoneyAmount(const LegacyMoneyLayout *layout, std::int32_t amount, std::int8_t currency, std::uint32_t colour, std::uint32_t fullPrefix) {
    LegacyIconSprite &symbol = moneyIconCurrencySprite;
    symbol.depth = layout->depth;
    if (!colour)
        colour = 0xff656565U;
    float digitX;
    std::int16_t width;
    if (currency) {
        std::int32_t count = 1;
        float digits = static_cast<float>(GetSelectorNumberDigits(amount));
        if (digits < 1.0f)
            digits = 1.0f;
        width = static_cast<std::int16_t>(layout->scale * 16.0f);
        symbol.width = symbol.height = width;
        digitX = layout->position.x + width;
        symbol.x = digitX - digits * width;
        symbol.y = layout->position.y;
        static const std::uint16_t moneyAmountCurrencyIcons[3] = {0x410, 0x41c, 0x416};
        if (fullPrefix & 1U)
            count = 3;
        digitX += width * 0.5f;
        for (std::int32_t index = 0; index < count; ++index) {
            symbol.icon = moneyAmountCurrencyIcons[index];
            PrepareDrawSimple(symbol, colour);
            symbol.x -= width;
        }
    } else {
        float verticalOffset = 4.0f;
        digitX = layout->position.x;
        width = static_cast<std::int16_t>(layout->scale * 24.0f);
        symbol.width = symbol.height = width;
        symbol.icon = 0x411;
        symbol.x = (layout->scale * 8.0f + layout->position.x) + static_cast<float>(symbol.width >> 1);
        symbol.y = layout->position.y - layout->scale * verticalOffset;
        PrepareDrawSimple(symbol, colour);
    }
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    DrawMoneyDigits(digitX, layout->position.y, layout->scale, layout->depth, amount, colour);
#else
    DrawNativeMoneyDigits(digitX, layout->position.y, layout->scale, layout->depth, amount, colour);
#endif
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawMoneyAmount(const Vector2 *position, float depth, std::int32_t amount, std::int8_t currency, std::uint32_t colour, float scale) {
    LegacyMoneyLayout layout;
    layout.position.x = position->x;
    layout.position.y = position->y;
    layout.depth = depth;
    layout.scale = scale;
    DrawNativeMoneyAmount(&layout, amount, currency, colour, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void AnimateMoneyUiElement(void *) {
    if (!(moneyIconFlags & 4U))
        return;
    std::int32_t amount;
    if (moneyIconFlags & 8U)
        amount = moneyIconDisplay - moneyIconAmount;
    else
        amount = moneyIconDisplay;
    const std::int8_t currency = moneyIconDrawMode ? 2 : 0;
    moneyIconSprite.x = moneyIconPosition.x;
    moneyIconSprite.y = moneyIconPosition.y;
    PrepareDrawWithMode(moneyIconSprite);
    DrawItemEffectWindow(6, moneyIconPosition.x + -63.0f, moneyIconPosition.y + -21.0f, 2.0f);
    Vector2 position;
    position.x = (moneyIconPosition.x + 128.0f) + 20.0f;
    position.y = moneyIconPosition.y + 10.0f;
    if (static_cast<std::int8_t>(moneyIconDrawMode) == 3)
        DrawMoneyAmount(&position, moneyIconPosition.depth, amount, currency, 0xe0105010U, 1.0f);
    else
        DrawMoneyAmount(&position, moneyIconPosition.depth, amount, currency, 0, 1.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void ChooseMoneyOptions(std::uint32_t flags) {
    static const char *const moneyOptionLabels[2] = {"\xa5\xad\xa5\xe3\xa5\xf3\xa5\xbb\xa5\xeb", "\xa3\xcd\xa3\xc1\xa3\xd8\xa3\xc2\xa3\xc5\xa3\xd4"};
    for (std::int32_t index = 0; index < 2; ++index) {
        if (flags & MONEY_TEXT_OPTION_FLAGS[index]) {
            if (static_cast<std::int16_t>(moneyIconTextSprites[index].icon) != -1)
                ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(moneyIconTextSprites[index].icon));
            moneyIconTextSprites[index].icon = static_cast<std::uint16_t>(AcquireCtrlSpeakTextHandle(moneyOptionLabels[index], 0));
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void ReleaseMoneyTextSlots() {
    for (std::int32_t index = 0; index < 2; ++index) {
        if (static_cast<std::int16_t>(moneyIconTextSprites[index].icon) != -1)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(moneyIconTextSprites[index].icon));
        moneyIconTextSprites[index].icon = 0xffff;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void DrawMoneyOptionText(std::int32_t text, std::int32_t direction) {
    LegacyIconSprite &sprite = moneyIconTextSprites[text];
    const LegacyIconSprite &source = moneyIconDirections[direction];
    sprite.x = source.x;
    sprite.y = source.y;
    sprite.scaleX = source.scaleX;
    sprite.scaleY = source.scaleY;
    if (static_cast<std::int16_t>(sprite.icon) != -1)
        PrepareDrawTextureWithMode(sprite);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void AdvanceMoneySelectionScale() {
    if (static_cast<std::int16_t>(moneyIconPosition.selectedDirection) == 4)
        return;
    moneyIconSelectionScale += 0.25f;
    if (!(moneyIconSelectionScale < 2.0f))
        moneyIconSelectionScale = 1.0f;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t GetSelectorNumberDigits(std::int32_t amount) {
    if (amount > 0)
        for (std::int32_t index = 0; index < 8; ++index)
            if (amount >= moneyIconIncrements[index])
                return 8 - index;
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetNumBet() UI_NOEXCEPT {
    return static_cast<std::uint32_t>(moneyIconResult ? moneyIconResult : moneyIconAmount);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetMoneyRelatedUIElement(std::int16_t maximum, std::int16_t duration, std::int16_t step) {
    SetMoneyRelatedUIElement_0(maximum & 0x7fff, static_cast<std::int16_t>(duration & 0x7fff), step & 0x7fff, 15);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t AnimateMoneyAmount(std::int32_t initial, std::int32_t target, std::int32_t modeIndex) UI_NOEXCEPT {
    moneyIconFlags = 0x34;
    moneyIconDrawMode = static_cast<std::uint8_t>(MONEY_ANIMATION_MODES[modeIndex]);
    moneyIconFrames = 75.0f;
    moneyIconDisplay = initial;
    moneyIconTarget = target;
    SetMoneyIconPhase(2);
    return ToggleRenderIcons(UI_ICON(MoneyMessage), true);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool SetSleepActionIcon(std::int32_t flags) {
    if (GetShouldRenderIcons(UI_ICON(Any)))
        return false;
    const std::uint32_t requested = static_cast<std::uint32_t>(flags) & 0x11bU;
    if (requested)
        sleepActionFlags = requested;
    if (sleepActionFlags & 0x100U)
        sleepActionFlags |= 0x10U;
    if (!(sleepActionFlags & 0x1bU))
        return false;
    ToggleRenderIcons(UI_ICON(SleepAction), true);
    g_sleepAction.inputMode = 1;
    g_sleepAction.result = 0;
    sleepActionPublishedSelection = -1;
    sleepActionScale = 0.0f;
    g_sleepAction.x = 180.0f;
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetSleepActionIconResult() {
    return g_sleepAction.result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t ShowSleepActionSaveIcon() {
    const std::int32_t shown = SetSleepActionIcon(0x10f);
    return shown || IsSaveAllowed();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool SetSleepActionIconMode(std::uint8_t mode) {
    std::int32_t flags;
    if (mode == 0xff)
        flags = 0;
    else
        switch (mode) {
        case 0:
            flags = 15;
            break;
        case 1:
            flags = 0x10f;
            break;
        case 2:
            flags = 10;
            break;
        case 3:
            flags = 0x10a;
            break;
        default:
            flags = 0;
            break;
        }
    return SetSleepActionIcon(flags);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t GetSleepActionIconSelection() {
    return g_sleepAction.result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetHelpIconState() UI_NOEXCEPT {
    g_helpIconState = 1;
}

// @verified - d
void SetShowQteFlag(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint16_t buttonFlags) UI_NOEXCEPT {
    std::int8_t bit;
    g_quickIconState.flags = buttonFlags;
    g_quickIconState.maximumFrames = maximumFrames > 0 ? static_cast<float>(maximumFrames) : 1.0f;
    float frames;
    if (maximumFrames == 0x7fff)
        frames = -1.0f;
    else
        frames = maximumFrames > 0 ? static_cast<float>(maximumFrames) : 1.0f;
    g_quickIconState.maximumFrames = frames;
    g_quickIconState.elapsedFrames = 0.0f;
    g_quickIconState.phase = 2;
    ToggleRenderIcons(UI_ICON(Qte), true);
    g_quickIconState.currentStatus = 0xffff;
    if (g_quickIconState.flags & 0x20U)
        g_quickIconState.requiredStatus = requiredStatus;
    else {
        for (bit = 0; bit < 12; ++bit)
            if ((requiredStatus >> bit) & 1)
                break;
        g_quickIconState.requiredStatus = 1 << bit;
    }
    g_quickIconUnpressed = 1;
    g_quickIconBlinkPhase = 0;
    if (g_quickIconState.requiredStatus & 0xff) {
        g_quickIconSprite.icon = 0x800;
        for (bit = 0; bit < 7; ++bit)
            if ((g_quickIconState.requiredStatus >> bit) & 1)
                break;
        g_quickIconSprite.rotation = bit * 0x2000;
        if (bit == 0 || bit == 4)
            g_quickIconSprite.scaleY = 0.9f;
        else
            g_quickIconSprite.scaleY = 1.0f;
    } else {
        for (bit = 0; bit < 4; ++bit)
            if (((g_quickIconState.requiredStatus >> 8) >> bit) & 1)
                break;
        g_quickIconSprite.icon = kQuickIconFaceIcons[bit];
        g_quickIconSprite.rotation = 0;
        g_quickIconSprite.scaleY = 1.0f;
    }
}

// @verified - d
void SetShowQteFlagForButtonType(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint32_t buttonType) UI_NOEXCEPT {
    if (buttonType >= (sizeof(kQuickIconButtonTypes) / sizeof(kQuickIconButtonTypes[0])))
        buttonType = 0;
    SetShowQteFlag(requiredStatus, maximumFrames, kQuickIconButtonTypes[buttonType]);
    g_quickIconButtonMask = 0xffff;
}

// @verified - d
void SetQteButtonState(std::uint16_t mask) UI_NOEXCEPT {
    g_quickIconButtonMask = mask;
}

// @verified - d
std::int32_t GetCurrentQteStatus() UI_NOEXCEPT {
    const std::uint16_t result = static_cast<std::uint16_t>(g_quickIconState.currentStatus);
    g_quickIconState.currentStatus = 0xffff;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t GetQuickIconLoadingReleaseState() {
    return g_quickIconLoadingReleaseFlag;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool AdvanceIconPosition(ItemEffectPosition *position, float target) {
    bool done = false;
    float distance;
    if (target - position->x < 0.0f)
        distance = -(target - position->x);
    else
        distance = target - position->x;
    distance *= 0.5f;
    if (distance > 64.0f)
        position->velocity += position->velocity;
    else
        position->velocity = distance;
    if (position->velocity < 1.0f)
        position->velocity = 1.0f;
    else if (position->velocity > 64.0f)
        position->velocity = 64.0f;
    position->x += position->velocity;
    if (!(position->x < target)) {
        position->x = target;
        done = true;
    }
    return done;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool RetreatIconPosition(ItemEffectPosition *position, float target) {
    bool done = false;
    float distance;
    if (target - position->x < 0.0f)
        distance = -(target - position->x);
    else
        distance = target - position->x;
    distance *= 0.5f;
    if (distance > 64.0f)
        position->velocity += position->velocity;
    else
        position->velocity = distance;
    if (position->velocity < 1.0f)
        position->velocity = 1.0f;
    else if (position->velocity > 64.0f)
        position->velocity = 64.0f;
    position->x -= position->velocity;
    if (!(target < position->x)) {
        position->x = target;
        done = true;
    }
    return done;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t GetParentMenu() {
    if (GetShouldRenderIcons(UI_ICON(ItemSelect)))
        return 0;
    if (GetShouldRenderIcons(UI_ICON(Qte)))
        return 2;
    if (GetShouldRenderIcons(UI_ICON(ActionSelector)))
        return 4;
    if (GetShouldRenderIcons(UI_ICON(ItemEffectMessage)))
        return 6;
    return 7;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetNativeGuiActive(std::int32_t active) {
    nativeGuiActive = active;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void iconToggleStateFor(std::uint8_t slot, std::uint8_t enabled) {
    if (!slot) {
        if (enabled)
            activeIconFlags = 0xffffffffU;
        else
            activeIconFlags = 0;
    } else if (enabled)
        activeIconFlags |= 1U << slot;
    else
        activeIconFlags &= ~(1U << slot);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetItemSelectionMode(std::uint8_t slot) {
    return activeIconFlags & (1U << slot);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool GetShouldRenderIcons(RenderIconSlot slot) {
    const std::uint32_t index = static_cast<std::uint32_t>(slot);
    if (index) {
        if (index > 6)
            return false;
        if (renderIconFlags & (1U << index))
            return true;
        return false;
    }
    if (renderIconFlags)
        return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool ToggleRenderIcons(RenderIconSlot slot, bool enabled) {
    bool previous = false;
    if (static_cast<std::uint32_t>(slot) <= 6) {
        previous = GetShouldRenderIcons(slot);
        if (enabled)
            renderIconFlags |= 1U << slot;
        else
            renderIconFlags &= ~(1U << slot);
        renderIconFlags &= ~1U;
    }
    return previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t RetireIconInputSlots(std::uint32_t mask, std::int32_t force) {
    std::uint32_t closed = 0;
    mask &= ~1U;
    for (std::int32_t slot = 1; slot < 7; ++slot)
        if ((mask & (1U << slot)) && iconInputSlotClosers[slot])
            closed |= iconInputSlotClosers[slot](force);
    return closed;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::uint32_t RetireAllIconInputSlots(std::int32_t force) {
    return RetireIconInputSlots(0xffffffffU, force);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t RetireIconInputSlot(std::uint32_t slot, std::int32_t force) {
    std::int32_t result = 0;
    if (slot < 7 && iconInputSlotClosers[slot])
        result = iconInputSlotClosers[slot](force);
    if (result)
        padcRefreshCurrentInputs(0);
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool padcRefreshCurrentInputs(std::int32_t bind) {
    if (g_iconPads.current) {
        if (bind) {
            if (!IsPadControlEntryBound(g_iconPads.current))
                g_iconPads.previous = BindPadControlEntry(g_iconPads.current);
        } else if (IsPadControlEntryBound(g_iconPads.current) && g_iconPads.previous) {
            BindPadControlEntry(g_iconPads.previous);
            g_iconPads.previous = 0;
        }
        return IsPadControlEntryBound(g_iconPads.current);
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t CloseWristwatchTransitionIcons() {
    if (IsHandWatchActive()) {
        std::int32_t closed = static_cast<std::int32_t>(RetireIconInputSlots(~2U, 1));
        if (closed)
            closed = padcRefreshCurrentInputs(0);
        return closed;
    }
    return RetireIconInputSlot(0, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t CloseHeldStateIcons() {
    return RetireIconInputSlot(0, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void PrepareDrawTextureWithSecondaryColour(const LegacyIconSprite &sprite, std::uint32_t secondaryColour) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.secondaryColour = secondaryColour;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), static_cast<std::int32_t>(sprite.x), static_cast<std::int32_t>(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, static_cast<std::int32_t>(sprite.rotation));
    packet.quad.flags |= sprite.flags | 0x400U;
    DrawLegacySpritePacket(packet, static_cast<std::int16_t>(sprite.icon), sprite.depth);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void PrepareDrawTextureWithColour(const LegacyIconSprite &sprite, std::uint32_t colour) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.colour = colour;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), static_cast<std::int32_t>(sprite.x), static_cast<std::int32_t>(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, static_cast<std::int32_t>(sprite.rotation));
    packet.quad.flags |= sprite.flags | 0x400U;
    DrawLegacySpritePacket(packet, static_cast<std::int16_t>(sprite.icon), sprite.depth);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour) {
    PrepareDrawSimple(sprite, (colour & 0x00ffffffU) | 0xff000000U);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void PrepareDrawSimpleBlack(const LegacyIconSprite &sprite, std::uint32_t colour) {
    PrepareDrawSimple(sprite, colour & 0xff000000U);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawMainIconOverlay(std::uint32_t colour, float depth) {
    LegacySpritePacket packet;
    do {
        packet.quad.flags = 0;
        packet.colour = 0xffffffffU;
        packet.secondaryColour = 0;
        packet.uv[0] = packet.uv[1] = 0.0f;
        packet.uv[2] = packet.uv[3] = 1.0f;
        packet.reservedTail = 0;
    } while (0);
    packet.colour = colour;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(5), 320, 240, 640, 480);
    packet.quad.flags |= 0x400U;
    DrawLegacyColourPacket(packet, depth);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CtrlMainIcon_Callback(void *) {
    HLib::ExecuteCallbackNow(reinterpret_cast<HLib::TaskCallback>(TickScenarioFlagNotification), 0);
    if (GetMapPadControlEntry())
        ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    switch (mainIconPhase) {
    case 0:
        if (CheckMainIconShortcut()) {
            if (btnCheck(SHEN_NAME4('B', 'T', 'M', 'E'))) {
                RefreshPadTask(0, 1);
                return;
            }
            RegisterButtonMediation(SHEN_NAME4('B', 'T', 'M', 'E'), 0, 0x40000);
        }
        if (CheckMainIconItemSelection() && nativeGuiActive == 1) {
            BeginItemSelector();
            mainIconPhase = 3;
        }
        break;
    case 3:
        break;
    case 5:
        mainIconPhase = 0;
        break;
    default:
        break;
    }
    if (renderIconFlags & 2U) {
        const std::uint32_t state = GetCurrentItemSelectState();
        if (!IsWazamakiInactive() && !(state & 0x23U))
            DrawMainIconOverlay(0x76000000U, 0.0f);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RefreshItemSelectorOwnedMask() {
    const std::uint32_t limit = GetMaxItemsForItemType(0);
    for (std::int32_t index = 0; index < 64; ++index)
        g_itemSelectorOwnedMask[index] = 0;
    const bool restricted = GetInternalFlag(0x56);
    for (std::uint32_t index = 0; index < limit; ++index)
        if (g_keyItemStock[index] && (!restricted || (GetInventoryItemFlags(0, index) & 2U)))
            g_itemSelectorOwnedMask[index >> 3U] |= static_cast<std::uint8_t>(1U << (index & 7U));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t CountItemSelectorOwnedItems() {
    RefreshItemSelectorOwnedMask();
    std::int32_t count = 0;
    const std::uint32_t limit = GetMaxItemsForItemType(0);
    for (std::uint32_t index = 0; index < limit; ++index)
        if (g_itemSelectorOwnedMask[index >> 3U] & (1U << (index & 7U)))
            ++count;
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t ReleaseCtrlSpeakTextHandle(std::int32_t handle) {
    for (std::int32_t slot = 0; slot < 4; ++slot)
        if (ctrlSpeakTexturePoolHandles[slot] == handle)
            ctrlSpeakTexturePoolFlags[slot] &= ~1U;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ReleaseSpeakTextSlots() {
    for (std::int8_t slot = 0; slot < 4; ++slot) {
        ReleaseCtrlSpeakTextHandle(ctrlSpeakChoiceTextHandles[slot]);
        ctrlSpeakChoiceTextHandles[slot] = -1;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void UpdateSpeakLabelTransition() {
    const std::int8_t phase = ctrlSpeakLabelPhase;
    for (std::int8_t slot = 0; slot < 4; ++slot) {
        if (phase == 1) {
            ctrlSpeakLabelSprites[slot].height = ctrlSpeakLabelSprites[slot].height - 10;
            if (ctrlSpeakLabelSprites[slot].height <= 0) {
                ctrlSpeakLabelPhase = 3;
                ctrlSpeakLabelSprites[slot].height = 0;
            }
        } else if (phase == 2) {
            ctrlSpeakLabelSprites[slot].height = ctrlSpeakLabelSprites[slot].height + 10;
            if (ctrlSpeakLabelSprites[slot].height > 31) {
                ctrlSpeakLabelSprites[slot].height = 32;
                ctrlSpeakLabelPhase = 0;
            }
        }
    }
    ctrlSpeakSpecialLabelWidthScale = static_cast<float>(ctrlSpeakLabelSprites[0].height) * 0.03125f * 2.0f;
    if (ctrlSpeakLabelPhase == 3) {
        ReleaseSpeakTextSlots();
        PrepareSpeakTextSlots();
        ctrlSpeakLabelPhase = 2;
        ctrlSpeakLabelMode = ctrlSpeakChoiceMode & 2;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void UpdateSpeakLabelReverseTransition() {
    const std::int8_t phase = ctrlSpeakLabelPhase;
    for (std::int8_t slot = 0; slot < 4; ++slot) {
        if (phase == -1) {
            ctrlSpeakLabelSprites[slot].height = ctrlSpeakLabelSprites[slot].height - 10;
            if (ctrlSpeakLabelSprites[slot].height <= 0) {
                ctrlSpeakLabelPhase = -2;
                ctrlSpeakLabelSprites[slot].height = 0;
                ctrlSpeakLabelMode = ctrlSpeakChoiceMode & 2;
            }
        } else if (phase == -2) {
            ctrlSpeakLabelSprites[slot].height = ctrlSpeakLabelSprites[slot].height + 10;
            if (ctrlSpeakLabelSprites[slot].height > 31) {
                ctrlSpeakLabelSprites[slot].height = 32;
                ctrlSpeakLabelPhase = 0;
            }
        }
    }
    ctrlSpeakSpecialLabelWidthScale = static_cast<float>(ctrlSpeakLabelSprites[0].height) * 0.03125f * 2.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
using namespace ui_detail;
#endif

namespace ui_detail {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void StartHelpBlinkTransition() {
    if (helpWidgetBlinkTransition == -1)
        helpWidgetBlinkTransition = UIAnimAllocSlot();
    StartUIIntegerTransition(helpWidgetBlinkTransition, 0, 2, 1.0f, 0.0f);
    SetUITransitionLoopMode(helpWidgetBlinkTransition, 1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
constexpr std::uint32_t HelpColour(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    return static_cast<std::uint32_t>(a) << 24U | static_cast<std::uint32_t>(b) << 16U | static_cast<std::uint32_t>(g) << 8U | r;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// helpIconInputType.
std::int32_t HelpIconInputType() {
    switch (helpGetCurrentHelp()) {
    case 0x04:
    case 0x1b:
        return 5;
    case 0x12:
    case 0x13:
    case 0x18:
    case 0x33:
    case 0x53:
        return 1;
    case 0x23:
        return 4;
    default:
        return 2;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void HelpWidgetMakeButtonIcon(UIButtonIconWidget &icon, std::int32_t code) {
    icon = {};
    icon.inUse = 1;
    icon.action = helpWidgetMode == 1 ? 3 : UI_GetActionForId(code);
    std::sprintf(icon.id.data(), "UIHelp::PlatformButtonIcon_ID:%i_Action:%i", code, icon.action);
    std::sprintf(icon.image.data(), "%s", g_platformButtonIconImages[0]->name);
    icon.field88 = 1;
    icon.iconFontSize = 4;
    icon.allowDoubleWidth = 1;
    icon.controlMethod = -1;
    icon.controlState = HelpIconInputType();
    icon.fontColour = HelpColour(0, 0, 0, 255);
    icon.alignment = 18;
    icon.sheet = g_platformIconsSheet;
    icon.colour = HelpColour(255, 255, 255, 255);
    icon.window = UI_FindViewWindow(g_uiElements[static_cast<std::size_t>(UIElement::GameHelp)].config.uiHandleIndex, "Help_MainPanel");
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawHelpBox(nk_context &context, const d3t::UIRect &rect, std::uint32_t colour, std::uint8_t alpha) {
    const float x = rect.x + (context.current ? context.current->bounds.x : 0.0f);
    const float y = rect.y + (context.current ? context.current->bounds.y : 0.0f);
    const struct nk_rect bounds = nk_rect(x, y, rect.width, rect.height);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, bounds);
    nk_color stroke{};
    std::memcpy(&stroke, &colour, sizeof(stroke));
    nk_stroke_rect(&context.current->buffer, bounds, 0.0f, 1.0f, stroke);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, bounds);
    nk_fill_rect(&context.current->buffer, bounds, 0.0f, nk_rgba(0, 0, 0, alpha));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawHelpLabel(nk_context &context, const d3t::UIRect &rect, const char *text, nk_flags alignment) {
    const struct nk_rect bounds = nk_rect(rect.x, rect.y, rect.width, rect.height);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, bounds);
    if (g_uiFontFace != 1) {
        g_uiFontFace = 1;
        UI_SelectFont();
    }
    context.style.text.color = nk_rgba(0, 0, 0, 255);
    if (g_waitingForNextTick == 1)
        nk_label(&context, text, alignment);
    g_uiFontFace = 0;
    UI_SelectFont();
    context.style.text.color = nk_rgba(255, 255, 255, 255);
    if (g_waitingForNextTick == 1) {
        nk_layout_space_push(&context, bounds);
        nk_label(&context, text, alignment);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// helpWidgetResolveLocation: the location label of the current map, SCEN region or author.
void HelpWidgetResolveLocation() {
    const char *label = GetLocationLabelId(g_currentMap.name);
    const char *const regionLabel = GetLocationLabelId(GetScenarioEventObjectMapNames()[0].name);
    const char *const authorLabel = GetLocationLabelId(GetCurrentAuthorId().name);
    std::string_view location = label ? GetLocalizedSceneText(label) : std::string_view{};
    const std::string_view region = regionLabel ? GetLocalizedSceneText(regionLabel) : std::string_view{};
    const std::string_view author = authorLabel ? GetLocalizedSceneText(authorLabel) : std::string_view{};
    if (!author.empty())
        location = author;
    if (!location.empty()) {
        if (!region.empty() && reinterpret_cast<std::uintptr_t>(region.data()) < reinterpret_cast<std::uintptr_t>(location.data()))
            location = region;
    } else {
        location = region;
    }
    std::sprintf(helpWidgetLocationText.data(), "%.*s", static_cast<int>(location.size()), location.data() ? location.data() : "");
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// helpWidgetLayout.
void HelpWidgetLayout(const d3t::UIRect &area, float lineHeight) {
    helpWidgetPauseRect = {area.width * 0.725f + area.x, area.height + area.y - lineHeight * 0.5f, area.width * 0.25f, lineHeight};
    helpWidgetLocationRect = {area.width * 0.0099999998f + area.x, area.height + area.y - lineHeight, area.width * 0.5f, lineHeight};
    const std::string_view pause = GetLocalizedSceneText("UIHELPPAUS"); // @text EN: "Game Paused" | JA: "ポーズ中"
    std::sprintf(helpWidgetPauseText.data(), "%.*s", static_cast<int>(pause.size()), pause.data());
    HelpWidgetResolveLocation();
    const nk_user_font &font = *GNuklearWindow.style.font;
    std::array<float, 45> cells{};
    for (std::int32_t index = 0; index < helpWidgetRunCount; ++index) {
        HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(index)];
        const std::int32_t cell = run.line + 15 * run.column;
        if (cell >= 45)
            break;
        const float offset = run.column == 1 ? area.width * 0.16f : run.column == 2 ? area.width * 0.50999999f : 0.0f;
        float &cursor = cells[static_cast<std::size_t>(cell)];
        run.rect.height = lineHeight;
        run.rect.x = cursor + offset;
        run.rect.y = static_cast<float>(run.line) * lineHeight;
        if (run.kind == 1) {
            run.rect.width = font.width(font.userdata, font.height, run.text.data(), run.length);
        } else {
            const UIButtonIconWidget &icon = helpWidgetIcons[static_cast<std::size_t>(run.icon)];
            const d3t::UIViewImage *const image = g_platformButtonIconImages[icon.field88 == 1 ? 0 : 2];
            const std::int32_t glyph = ResolveButtonIconGlyph(icon.action, icon.controlMethod, icon.controlState, icon.allowDoubleWidth);
            const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(icon.sheet), glyph, image);
            UI_ScaleSize(
                run.rect.width, run.rect.height, 2, 1000.0f, lineHeight, static_cast<float>(sprite.region[2]), static_cast<float>(sprite.region[3]), nullptr);
            run.rect.x += 3.0f;
            cursor += 6.0f;
        }
        cursor += run.rect.width;
    }
    for (std::int32_t index = 0; index < helpWidgetRunCount; ++index) {
        HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(index)];
        if (run.column == 0 && run.line >= 0 && run.line < 45)
            run.rect.x += (area.width - cells[static_cast<std::size_t>(run.line)]) * 0.5f;
        run.rect.x += area.x;
        run.rect.y += area.y + 80.0f;
    }
    for (std::int32_t index = 0; index < helpWidgetBoxCount && index < static_cast<std::int32_t>(helpWidgetBoxes.size()); ++index) {
        HelpWidgetBox &box = helpWidgetBoxes[static_cast<std::size_t>(index)];
        if (box.widthMode == 0) {
            box.rect.x = area.width * 0.15000001f + area.x;
            box.rect.width = area.width * 0.69999999f;
        } else if (box.widthMode == 2) {
            box.rect.x = area.width * 0.15000001f + area.x;
            box.rect.width = area.width * 0.64999998f;
        } else {
            box.rect.x = (box.column ? 0.505f : 0.15000001f) * area.width + area.x;
            box.rect.width = area.width * 0.345f;
        }
        box.rect.y = static_cast<float>(box.row) * lineHeight + (area.y + 80.0f);
        box.rect.height = static_cast<float>(box.rows) * lineHeight;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t HelpWidgetRender() {
    if (helpWidgetPeekHidden)
        return 0;
    helpWidgetPanels[0]->rectCached = 0;
    helpWidgetPanels[1]->rectCached = 0;
    nk_context &context = *GetNativeUiContext();
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::GameHelp)];
    if (helpWidgetMode == 0)
        DrawUIEntity(context, element.config.uiHandleIndex, element.index);
    UILayoutRect panel{};
    UI_GetWindowRect(helpWidgetPanels[1], panel);
    if (helpWidgetMode == 1)
        panel.y += panel.height * 0.5f;
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &helpWidgetPanel, "UIHelpBack", "UIHelpBack", nk_rect(panel.x, panel.y, panel.width, panel.height), NK_WINDOW_NO_SCROLLBAR);
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    }
    const d3t::UIRect area{50.0f, 20.0f, panel.width - 100.0f, panel.height - 50.0f};
    const float lineHeight = area.height / 15.0f;
    if (g_uiFontFace != 0) {
        g_uiFontFace = 0;
        UI_SelectFont();
    }
    if (g_uiFontSize != 3) {
        g_uiFontSize = 3;
        UI_SelectFont();
    }
    const std::int32_t keyboard = static_cast<std::int32_t>(GetIsKeyboardActive());
    if (helpWidgetRelayout == 1 || helpWidgetKeyboardType != keyboard) {
        helpWidgetRelayout = 0;
        helpWidgetKeyboardType = keyboard;
        HelpWidgetLayout(area, lineHeight);
    }
    if (helpWidgetMode == 0) {
        DrawHelpBox(context, area, HelpColour(255, 255, 255, 255), 216);
        for (std::int32_t index = 0; index < helpWidgetBoxCount; ++index) {
            const HelpWidgetBox &box = helpWidgetBoxes[static_cast<std::size_t>(index)];
            DrawHelpBox(context, box.rect, box.colour, 255);
        }
    }
    for (std::int32_t index = 0; index < helpWidgetRunCount; ++index) {
        const HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(index)];
        if (run.kind == 1) {
            DrawHelpLabel(context, run.rect, run.text.data(), NK_TEXT_LEFT);
        } else {
            const UILayoutRect rect{.x = run.rect.x, .y = run.rect.y, .width = run.rect.width, .height = run.rect.height};
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(rect.x, rect.y, rect.width, rect.height));
            UI_DrawButtonIcon(context, helpWidgetIcons[static_cast<std::size_t>(run.icon)], helpWidgetMode == 1 ? &rect : nullptr);
        }
    }
    if (helpWidgetMode == 0) {
        DrawHelpBox(context, helpWidgetPauseRect, HelpColour(255, 255, 255, 255), 255);
        if (static_cast<std::uint16_t>(std::bit_cast<std::int32_t>(GetUITransitionValue(helpWidgetBlinkTransition))) == 1)
            DrawHelpLabel(context, helpWidgetPauseRect, helpWidgetPauseText.data(), NK_TEXT_CENTERED);
        DrawHelpLabel(context, helpWidgetLocationRect, helpWidgetLocationText.data(), NK_TEXT_LEFT);
    }
    if (g_waitingForNextTick == 1) {
        nk_layout_space_end(&context);
        nk_end(&context);
    }
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t LoadHelpDefinition() {
    return ReadUIDefinitionFile(UIElement::GameHelp, "/ui/gamehelp/Help_New.ui", 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t LoadPlatformIcons() {
    return ReadUIDefinitionFile(UIElement::PlatformIcons, "/ui/PlatformIcons.ui", 1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// helpWidgetIsPressed.
std::int64_t HelpWidgetInput() {
    if (g_inputBuffer[0].pressedPriority & 0x200U)
        DisableHelpIcon();
    helpWidgetPeekHidden = false;
    if ((g_inputBuffer[0].held & 4U) && (g_inputBuffer[0].held & 2U))
        helpWidgetPeekHidden = g_inputBuffer[0].connectionState != 0;
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t HelpWidgetBind() {
    for (std::size_t index = 0; index < helpWidgetPanels.size(); ++index)
        helpWidgetPanels[index] = UI_FindWindowById(HELP_PANEL_NAMES[index]);
    helpWidgetBlinkTransition = UIAnimAllocSlot();
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void HelpWidgetShow() {
    helpWidgetMode = 0;
    helpWidgetArcadeContext = 0;
    auto &watch = g_uiElements[static_cast<std::size_t>(UIElement::WristwatchOverlay)];
    if (!watch.isRenderable) {
        watch.isRenderable = 1;
        if (watch.resetRender)
            watch.resetRender();
    }
    StartHelpBlinkTransition();
    for (const auto transition : nativeHudTransitions)
        if (transition != -1)
            RestartUITransition(transition);
    if (nativeHudBlink != -1)
        RestartUITransition(nativeHudBlink);
    for (auto &hint : nativeHudRequestedHints)
        hint = (hint & 0xffff0000U) | 0xffffU;
    for (auto &hint : nativeHudShownHints)
        hint = (hint & 0xffff0000U) | 0xffffU;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t HelpWidgetDestroy() {
    helpWidgetMode = 0;
    helpWidgetArcadeContext = 0;
    ClearWindowInput();
    ResetInputHandler();
    DisableElement(UIElement::WristwatchOverlay);
    ClearMenuInputHints();
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
constexpr std::array<std::int16_t, 4> WRISTWATCH_OVERLAY_HINTS{3852, 44, -1, -1};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t WristwatchOverlayHints() {
    return CopyHudHintIds(WRISTWATCH_OVERLAY_HINTS);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t WristwatchOverlayDebugGate() {
    return GetDebugForceUIEnabled(UIElement::WristwatchOverlay);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t WristwatchOverlayTickNullsub() {
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t HelpWidgetHints() {
    const std::array<std::int16_t, 4> hints{static_cast<std::int16_t>(helpWidgetMode == 1 && helpWidgetArcadeContext == 1 ? 45 : -1), 44, -1,
        static_cast<std::int16_t>(helpWidgetMode == 1 ? 3845 : -1)};
    return CopyHudHintIds(hints);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint8_t ConfigureHelpDirectionBlink(std::uint32_t visible) {
    std::size_t count = 0, missing = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        if ((visible & (1U << (12U + index))) != 0)
            ++count;
        else
            missing = HELP_DIRECTION_MISSING_ORDER[index];
    }
    if (!count)
        return 0;
    if (count == 3) {
        for (std::size_t index = 0; index < 4; ++index) {
            HelpDirectionBlink &blink = helpDirectionBlinks[HELP_DIRECTION_RING_ORDER[(index + missing) % 4]];
            blink.onFrames = HELP_DIRECTION_ON_FRAMES[8 + index];
            blink.offFrames = blink.onFrames;
            blink.remaining = HELP_DIRECTION_PHASES[8 + index];
            blink.mode = index == 1 ? 2 : (blink.remaining != 0);
        }
        return static_cast<std::uint8_t>(HELP_DIRECTION_PHASES[11] != 0);
    }
    std::size_t selected = 0;
    std::uint8_t result = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        HelpDirectionBlink &blink = helpDirectionBlinks[index];
        blink.mode = 0;
        result = static_cast<std::uint8_t>(1U << (12U + index));
        if ((visible & (1U << (12U + index))) == 0)
            continue;
        const std::size_t offset = (count - 1) * 4 + selected++;
        blink.onFrames = HELP_DIRECTION_ON_FRAMES[offset];
        blink.offFrames = blink.onFrames;
        blink.remaining = HELP_DIRECTION_PHASES[offset];
        result = static_cast<std::uint8_t>(blink.remaining);
        blink.mode = 1;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native RVA 4feff0 translates the existing BTNM owner, not controller buttons.
std::int32_t NativeHelpHint(std::uint32_t button) {
    switch (GetMediatedButtonOwner(button).name) {
    case SHEN_NAME(BTDR):
    case SHEN_NAME(BTD2):
        return 0;
    case SHEN_NAME(BTFT):
        return 1;
    case SHEN_NAME(BTMT):
        return 2;
    case SHEN_NAME(BTLD):
        return 3;
    case SHEN_NAME(BTEV):
        return 4;
    case SHEN_NAME(BTME):
        return 5;
    case SHEN_NAME(BTIL):
        return IsAnyItemSelectActive() ? 5 : 9;
    case SHEN_NAME(BTCC):
        return 6;
    case SHEN_NAME(BTBT):
        return 7;
    case SHEN_NAME(BTSW):
        return 8;
    case SHEN_NAME(BTSP):
        return 10;
    case SHEN_NAME(BTAD):
        return 11;
    case SHEN_NAME(BTKK):
        return 12;
    case SHEN_NAME(BTTT):
        return 13;
    case SHEN_NAME(BTPP):
        return 14;
    case SHEN_NAME(BTDD):
        return 15;
    case SHEN_NAME(BTAA):
        return 16;
    case SHEN_NAME(BTIF):
        return 17;
    case SHEN_NAME(BTNV):
        return 18;
    case SHEN_NAME(BTMK):
        return button == 0 ? 21 : button == 2 ? 19 : button == 3 ? 20 : 6;
    case SHEN_NAME(BTBB):
    case SHEN_NAME(BTBA):
    case SHEN_NAME(BTBX):
    case SHEN_NAME(BTBY):
        return 22;
    case SHEN_NAME(BTSD):
        return 23;
    case SHEN_NAME(BTMR):
        return 24;
    case SHEN_NAME(BTMP):
        return 25;
    case SHEN_NAME(BTMM):
        return 26;
    case SHEN_NAME(BTBF):
        return button == 0 ? 12 : button == 1 ? 13 : button == 2 ? 14 : 15;
    default:
        return -1;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void PrepareNativeHudHelp() {
    const bool persistentOnly = GetShouldRenderIcons(RenderIconSlot::ItemSelect) || IsItemSelectable();
    helpIconDrawVisible = helpIconState.persistentVisible | (persistentOnly ? 0 : helpIconState.frameOverlayVisible);
    helpIconDrawSelected = helpIconState.persistentSelected | (persistentOnly ? 0 : helpIconState.frameOverlaySelected);
    const MapControlData *const map = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const bool author = authGetLoaded() != 0, film = !IsScreenFilmNotShowing(map->screenFilmTask);
    const bool searchable = plymIsSearchable(), parent = GetParentMenu() != 0;
    if (author || film || (!searchable && parent))
        helpIconHudDelay = 5;
    else
        helpIconHudDelay = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconHudDelay) - 1U);
    helpIconHintAngle += static_cast<std::uint32_t>(TruncateSpriteCoordinate(shenGetScaledDeltaTickTime(1638.0f)));
    for (std::uint32_t &angle : helpIconHintAngles)
        angle = helpIconHintAngle;
    qteForcedHelp = (helpIconDrawVisible & 0x100000U) != 0;
    bool forceAdmission = qteForcedHelp != 0;
    for (std::int16_t &hint : helpIconHintIds)
        hint = -1;
    if ((GetSleepActionIconFlags() & 3U) == 1 || (!IsCtrlSpeakIconInactive() && IsSpeakIconCancellable())) {
        helpIconDrawVisible |= 2U;
        helpIconDrawSelected &= ~2U;
        qteForcedHelp = 1;
        forceAdmission = true;
    }
    std::array<std::int32_t, 4> hints{};
    for (std::size_t index = 0; index < hints.size(); ++index) {
        hints[index] = NativeHelpHint(static_cast<std::uint32_t>(index));
        if (hints[index] >= 0)
            continue;
        if (index == 1)
            hints[index] = 6;
        else if ((index == 0 || index == 2) && (helpIconDrawVisible & HELP_BUTTON_MASKS[index]))
            hints[index] = 27;
        if (iconSelectFlags & HELP_SELECTED_BUTTON_MASKS[index]) {
            if (index != 1)
                hints[index] = 22;
            helpIconDrawVisible |= HELP_BUTTON_MASKS[index];
        }
    }
#if SHENMUE_IS_PC && SHENMUE_IS_S2 && SHENMUE_ENHANCEMENT_S2_TAI_CHI
    if (GetIsKeyboardActive() == 1 && TaiChiHintActive()) {
        hints[2] = 4; // BTEV action hint, also used by FLAGS interactions.
        helpIconDrawVisible |= HELP_BUTTON_MASKS[2];
        helpIconDrawSelected &= ~HELP_BUTTON_MASKS[2];
    }
#endif
    if (helpIconHudDelay < 0 && searchable && !IsScenarioBusy() && !(GetSleepActionIconFlags() & 1U) && IsCtrlSpeakIconInactive()) {
        if (!(helpIconDrawVisible & 4U) && !srchCheckFlags(6) && IsWristwatchOpenable()) {
            helpIconDrawVisible |= 4U;
            helpIconDrawSelected &= ~4U;
        }
        if (!(helpIconDrawVisible & 2U) && (srchCheckFlags(3) > 0 || srchCheckFlags(6))) {
            helpIconDrawVisible |= 2U;
            helpIconDrawSelected &= ~2U;
        }
    }
    if (helpIconHudDelay < 0 || forceAdmission || (!author && !film && !searchable && parent)) {
        for (std::size_t index = 0; index < hints.size(); ++index) {
            if (!(helpIconDrawVisible & HELP_BUTTON_MASKS[index])) {
                helpIconHintAngles[index] = 0;
                continue;
            }
            if (hints[index] < 0 || (index == 2 && helpIconHudDelay < 0 && IsItemSelectable()))
                continue;
            helpIconHintIds[index] = HELP_HINT_ICONS[static_cast<std::size_t>(hints[index])];
            if (helpIconHudDelay >= 0)
                forceAdmission = true;
        }
    }
    if (hints[0] == 1 && scenarioTalkPromptResumed)
        helpIconHintAngles[0] = 0;
    if (hints[2] == 5 && GetScenarioFlagNotificationShown() == 0)
        helpIconHintAngles[2] = 0;
    if (hints[3] == 2) {
        if (scenarioFollowPromptShown)
            helpIconHintAngles[3] = 0;
    } else if (hints[3] == 8 && helpIconItemEffectState == 0)
        helpIconHintAngles[3] = 0;
    for (std::size_t index = 0; index < hints.size(); ++index)
        if (helpIconHintIds[index] < 0)
            helpIconDrawVisible &= ~HELP_BUTTON_MASKS[index];
    helpIconHintSelected = {};
    for (std::size_t bit = 0; bit < HELP_HINT_BLINK_SLOTS.size(); ++bit) {
        const std::int32_t slot = HELP_HINT_BLINK_SLOTS[bit];
        if (slot != -1 && ((helpIconDrawVisible & helpIconDrawSelected) & (1U << bit)))
            helpIconHintSelected[static_cast<std::size_t>(slot)] = 1;
    }
    if (forceAdmission)
        helpIconHudDelay = -1;
    helpIconDrawVisible &= ~0x200U;
    helpIconPublishedVisible = helpIconDrawVisible;
    std::array<std::uint32_t, 5> output{};
    for (std::size_t index = 0; index < hints.size(); ++index)
        output[index] = static_cast<std::uint16_t>(helpIconHintIds[index]) | (static_cast<std::uint32_t>(helpIconHintSelected[index]) << 16U);
    if (!g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].isVisible && !g_uiElements[static_cast<std::size_t>(UIElement::ActionSelector)].isVisible)
        PublishHudHints(output.data(), false);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DoQteDraw(void *) {
    if (nowQteDrawSuppression == 1)
        return;
    helpIconRefreshDelay = (g_loadingFlag & 2U) ? 3 : std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconRefreshDelay) - 1U);
    const float cardinalX = IsFightHealthUIActive() ? 160.0f : 52.0f;
    const MapControlData *const map = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    std::uint32_t blocked = IsSaveAllowed() != 0;
    if (!IsScreenFilmNotShowing(map->screenFilmTask) || GetIsLoadScreenActive())
        blocked |= 2U;
    if (authGetLoaded() && !qteForcedHelp)
        blocked |= 4U;
    if (IsWazamakiInactive())
        blocked |= 0x10U;
    if (blocked)
        helpIconDrawVisible = 0;
    if (helpIconHudDelay < 0 && helpIconDrawVisible && GetDebugForceUIEnabled(UIElement::GameHud)) {
        ConstructHudCameraState(qteControlMask, nativeHudSpriteWidgets[5]);
        EnableElement(UIElement::GameHud);
    }
    LegacyIconSprite cardinal{.y = 445.0f, .icon = 2048, .width = 38, .height = 38};
    float offset = 0.0f;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::uint32_t bit = 0x10000U << index;
        cardinal.rotation = static_cast<std::uint16_t>(TruncateSpriteCoordinate(static_cast<float>(HELP_CARDINAL_ROTATIONS[index]) * 8192.0f / 45.0f));
        if (!(helpIconDrawVisible & bit) || ((helpIconDrawSelected & bit) && !helpIconBlinkVisible))
            continue;
        cardinal.x = cardinalX + offset;
        PrepareDrawWithMode(cardinal, ScreenMode::AdjustedWithOffset);
        offset += 48.0f;
    }
    if (qteControlState != 1)
        return;
    LegacyIconSprite camera{};
    InitializeLegacyIconSprite(camera);
    camera.depth = 1.1f;
    camera.icon = 2305;
    std::int32_t stripX = 510, stripY = 330;
    if (qteCameraAtTop == 1 || authGetLoaded() == 1) {
        camera.x = 600.0f;
        camera.y = 50.0f;
        stripX = 480;
        stripY = 50;
        ConstructHudCameraState(qteControlMask, hudSnapshotCameraState);
        EnableElement(UIElement::HudSnapshot);
    } else {
        camera.x = 580.0f;
        camera.y = 350.0f;
    }
    PrepareDrawWithMode(camera, ScreenMode::Stretched);
    if (qteControlMask == 1 || qteControlMask == 2) {
        camera.icon = qteControlMask == 1 ? 2306 : 2307;
        PrepareDrawWithMode(camera, ScreenMode::Stretched);
        qteDrawFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(qteDrawFrames) - 1U);
        if (qteDrawFrames <= 0)
            qteControlMask = 0;
    } else if (qteControlMask == 3) {
        LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = 0xffffffffU, .uv = {0.0f, 0.0f, 1.0f, 1.0f}};
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, stripX, stripY, 256, 16, ScreenMode::Stretched);
        LegacySpriteNullsub(packet, nowQteTexture, camera.depth);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void NQ_CtrlButtonIcon(void *) {
    if (helpIconItemPromptFrames >= 0 && (--helpIconItemPromptFrames < 0 || IsItemSelectable())) {
        helpIconItemPromptFrames = -1;
        CheckHelpIcon(0x200, 0x200, CheckHelpIconOp::Remove);
        SetHudItemPromptIcon(1);
    }
    if (helpIconBlinkFrames > 0) {
        helpIconBlinkVisible = 1;
        if (--helpIconBlinkFrames <= 0)
            helpIconBlinkFrames = -3;
    } else {
        helpIconBlinkVisible = 0;
        helpIconBlinkFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconBlinkFrames) + 1U);
        if (helpIconBlinkFrames >= 0)
            helpIconBlinkFrames = 5;
    }
    if (!GetParentMenu()) {
        helpIconItemEffectState = 0;
        helpIconItemEffectFrames = -1;
    } else if (helpIconItemEffectState <= 0) {
        helpIconItemEffectFrames = -1;
    } else {
        helpIconItemEffectFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconItemEffectFrames) - 1U);
        if (helpIconItemEffectFrames < 0) {
            helpIconItemEffectState = -1;
            helpIconItemEffectFrames = -1;
        }
    }
    if (helpIconDeferredRemoval && !helpIconBlinkVisible) {
        helpIconState.persistentVisible &= ~helpIconDeferredRemoval;
        helpIconState.persistentSelected &= ~helpIconDeferredRemoval;
        helpIconDeferredRemoval = 0;
    }
    for (HelpDirectionBlink &blink : helpDirectionBlinks) {
        if (blink.remaining > 0) {
            blink.visible = 1;
            if (--blink.remaining <= 0)
                blink.remaining = std::bit_cast<std::int32_t>(0U - static_cast<std::uint32_t>(blink.offFrames));
        } else {
            blink.visible = 0;
            blink.remaining = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(blink.remaining) + 1U);
            if (blink.remaining >= 0)
                blink.remaining = blink.onFrames;
        }
    }
    if (helpIconRefreshDelay < 0)
        PrepareNativeHudHelp();
    HLib::ExecuteCallbackNow(DoQteDraw, nullptr);
    currentIconDeltaTime += shenGetScaledDeltaTickTime(static_cast<float>(iconDeltaTime));
    if (currentIconDeltaTime <= 0.0f) {
        currentIconDeltaTime = 0.0f;
        iconDeltaTime = 7;
    } else if (currentIconDeltaTime >= 127.0f) {
        currentIconDeltaTime = 127.0f;
        iconDeltaTime = -7;
    }
    helpIconState.frameOverlayVisible = helpIconState.frameOverlaySelected = 0;
    iconSelectFlags = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t ProcessSpeakSelection() {
    std::uint8_t input = GetDebugForceUIEnabled(UIElement::ActionSelector) ? static_cast<std::uint8_t>(ConsumeActionSelectorInput()) :
                                                                             DecodeSelectorInput(GetPadControlData(1) & 0xff3fU, 0x55);
    std::int64_t result = input;
    if (input == 0 || input == 2 || input == 4 || input == 6) {
        const std::int8_t slot = ACTION_SELECTOR_DIRECTION_TO_SLOT[input];
        const std::uint16_t choice = static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]);
        if (choice != 0xffffU) {
            ctrlSpeakSelectedChoiceSlot = static_cast<std::uint16_t>(slot);
            ctrlSpeakChoiceDrawSprites[slot].depth = 7.0f;
            ctrlSpeakRetainedMotion = 0.0f;
            ctrlSpeakIconState = 3;
            ctrlSpeakResultChoiceId = choice;
            ctrlSpeakSelectionAnimationFrame = -1;
            return 9 * slot;
        }
        input = 13;
    } else if (input == 9) {
        result = ctrlSpeakMode;
        if (IsSpeakIconCancellable()) {
            ctrlSpeakIconState = 3;
            ctrlSpeakResultChoiceId = 0xffffU;
            return result;
        }
    } else if (input == 11 && ctrlSpeakDedicatedCancelEnabled) {
        ctrlSpeakIconState = 3;
        ctrlSpeakResultChoiceId = 470;
        return 470;
    }
    if (ctrlSpeakIconState == 3)
        return result;
    if (input == 13) {
        ctrlSpeakElapsedFrames = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(ctrlSpeakElapsedFrames) + 1U));
        result = static_cast<std::uint16_t>(ctrlSpeakElapsedFrames);
        if (ctrlSpeakElapsedFrames >= ctrlSpeakTimeoutFrames) {
            ctrlSpeakIconState = 3;
            dialogueSelectionTimedOut = true;
            if (ctrlSpeakDefaultChoiceId == 0xffffU) {
                ctrlSpeakResultChoiceId = 0xffffU;
            } else {
                for (std::int8_t slot = 0; slot < 4; ++slot) {
                    result = slot;
                    if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) == ctrlSpeakDefaultChoiceId) {
                        ctrlSpeakSelectedChoiceSlot = static_cast<std::uint16_t>(slot);
                        ctrlSpeakResultChoiceId = ctrlSpeakDefaultChoiceId;
                        ctrlSpeakChoiceDrawSprites[slot].depth = 7.0f;
                        return result;
                    }
                }
            }
            return result;
        }
    }
    if (ctrlSpeakDedicatedCancelEnabled)
        return RegisterButtonMediation(BUTTON_CTRL_SPEAK_CANCEL, nullptr, (ButtonMediationShowButtonHint | ButtonMediationBindB | 0x1U));
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void CtrlSpeakIcon_Callback(void *) {
    ctrlSpeakPulseAngle = static_cast<std::uint16_t>(ctrlSpeakPulseAngle + 1638U);
    switch (ctrlSpeakIconState) {
    case 0:
        if (ctrlSpeakOpenRequested) {
            ctrlSpeakIconState = 1;
            PrepareSpeakTextSlots();
            padcRefreshCurrentInputs(true);
            iconToggleStateFor(2, true);
            ctrlSpeakResultChoiceId = 0xffff;
            ctrlSpeakSavedLoadingFlags = g_loadingFlag;
            if ((g_loadingFlag & 0x20U) != 0)
                FinishLoading();
            ctrlSpeakRadialScale = 0.0f;
            ctrlSpeakSelectorOrigin[0] = ctrlSpeakRightOriginX;
            ctrlSpeakRetainedMotion = 10.0f;
            ctrlSpeakSelectionAnimationFrame = -1;
            ShowElementByIndex(UIElement::ActionSelector);
        }
        break;
    case 1:
        ctrlSpeakIconState = 2;
        break;
    case 2:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ProcessSpeakSelection();
        break;
    case 3:
        if (ctrlSpeakSelectedChoiceSlot == 4) {
            ctrlSpeakIconState = 4;
            DestroyElementByIndex(UIElement::ActionSelector);
            DestroyElementByIndex(UIElement::GameMoves);
        }
        break;
    case 4:
        if (!(ActionSelectorPosition() > 0.0f)) {
            ctrlSpeakIconState = 0;
            padcRefreshCurrentInputs(false);
            ToggleRenderIcons(RenderIconSlot::ActionSelector, false);
            iconToggleStateFor(2, false);
            ReleaseSpeakTextSlots();
            ctrlSpeakOpenRequested = false;
            if ((ctrlSpeakSavedLoadingFlags & 0x20U) != 0) {
                g_loadingFlag = ctrlSpeakSavedLoadingFlags;
                ctrlSpeakSavedLoadingFlags = 0;
            }
        }
        break;
    case 5: {
        const float interpolation = 1.0f - ctrlSpeakResizeFrame / 5.0f;
        for (std::size_t slot = 0; slot < 4; ++slot) {
            ctrlSpeakChoiceWidths[slot] = ctrlSpeakChoiceWidthDeltas[slot] ?
                static_cast<std::int16_t>(static_cast<std::int32_t>(
                    static_cast<float>(ctrlSpeakTargetChoiceWidths[slot]) - static_cast<float>(ctrlSpeakChoiceWidthDeltas[slot]) * interpolation)) :
                ctrlSpeakTargetChoiceWidths[slot];
        }
        ctrlSpeakResizeFrame += 1.0f;
        if (ctrlSpeakResizeFrame > 5.0f) {
            ctrlSpeakChoiceWidths = ctrlSpeakTargetChoiceWidths;
            ctrlSpeakIconState = 2;
        }
        if (ctrlSpeakChoiceWidths[1] != ctrlSpeakChoiceWidths[3]) {
            if (ctrlSpeakChoiceWidths[1] <= ctrlSpeakChoiceWidths[3])
                ctrlSpeakChoiceWidths[1] = ctrlSpeakChoiceWidths[3];
            else
                ctrlSpeakChoiceWidths[3] = ctrlSpeakChoiceWidths[1];
        }
        const float expansion = static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f;
        ctrlSpeakHalfWidthExpansion = expansion * 0.5f;
        ctrlSpeakRightOriginX = expansion + 180.0f;
        ctrlSpeakSelectorOrigin[0] = expansion + 180.0f;
        ctrlSpeakLeftOriginX = -176.0f - expansion;
        break;
    }
    default:
        break;
    }
    if (GetItemSelectionMode(2)) {
        if (ctrlSpeakSelectedChoiceSlot != 4) {
            ctrlSpeakSelectionAnimationFrame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(ctrlSpeakSelectionAnimationFrame) + 1U);
            if (ctrlSpeakSelectionAnimationFrame >= 20) {
                ctrlSpeakSelectionAnimationFrame = 0;
                ctrlSpeakSelectedChoiceSlot = 4;
            }
        }
        if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
            EnableElement(UIElement::ActionSelector);
            ExecuteElementCallback(UIElement::ActionSelector);
        } else {
            HLib::ExecuteCallbackNow(DrawCtrlSpeakSelector, nullptr);
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t ClearCtrlSpeakRasterizerResultNullsub(std::byte *) {
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t AcquireCtrlSpeakTextHandle(const char *) {
    for (std::size_t slot = 0; slot < ctrlSpeakTexturePoolHandles.size(); ++slot) {
        if (ctrlSpeakTexturePoolFlags[slot] & 1U)
            continue;
        GuiPtxHeader *const header = reinterpret_cast<GuiPtxHeader *>(guiPtxBacking);
        header->width = 256;
        header->height = 16;
        const std::uint32_t height = 16, width = 256;
        std::memcpy(guiPtxBacking + 60, &height, sizeof(height));
        std::memcpy(guiPtxBacking + 64, &width, sizeof(width));
        (void)ClearCtrlSpeakRasterizerResultNullsub(guiPtxBacking);
        (void)ReplacePtxTexture(static_cast<std::uint32_t>(ctrlSpeakTexturePoolHandles[slot]), guiPtxBacking);
        ctrlSpeakTexturePoolFlags[slot] |= 1U;
        return ctrlSpeakTexturePoolHandles[slot];
    }
    return -1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void PrepareSpeakTextSlots() {
    for (std::size_t slot = 0; slot < 4; ++slot) {
        const std::uint16_t choice = static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]);
        if (choice == 0xffffU) {
            ctrlSpeakChoiceDrawSprites[slot].icon = 0xffffU;
        } else {
            const std::string_view label = ctrlSpeakMode < 3 ? GetActionSelectorLabel(choice) : GetLocalizedSceneText(CTRL_SPEAK_ALTERNATE_LABELS[choice]);
            if (label.data() && ctrlSpeakTexturePoolHandles[slot] >= 0)
                ctrlSpeakChoiceTextHandles[slot] = AcquireCtrlSpeakTextHandle(label.data());
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t RenderItemSelectorCounterNullsub(std::byte *) {
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint64_t BeginItemSelector() {
    const std::uint32_t claimed = GetShouldRenderIcons(RenderIconSlot::ItemSelect);
    if (claimed || !itemSelectTask)
        return claimed;
    HLib::ResolveDeferredCallback(itemSelectTask);
    itemSelectPhase = 1;
    RefreshItemSelectorSelection();
    itemSelectorNavigationInput = 0;
    padcRefreshCurrentInputs(true);
    ToggleRenderIcons(RenderIconSlot::ItemSelect, true);
    itemSelectorDelayedResult = 0;
    char text[24];
    const std::int32_t photos = GetSnapshotControlPhotoCount();
    if (photos == -1)
        std::sprintf(text, "???/%3d", 126);
    else
        std::sprintf(text, "%3d/%3d", photos, 126);
    RenderItemSelectorCounterNullsub(scenarioItemPtx);
    d3t::CreateMipmappedScaledTexture(scenarioItemTexture, 0, 1, scenarioItemPtx + guiPtxPrefixSize, 256 * 16 * 4);
    return scenarioItemTexture;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t RequestItemSelector() {
    if (renderIconFlags || nativeGuiActive != 1)
        return 0;
    mainIconPostFxBank = static_cast<std::int32_t>(GetPostFxBankValue(1));
    SelectPostFxBank(1, 0);
    BeginItemSelector();
    mainIconPhase = 3;
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t CheckMainIconShortcut() {
    if (mainIconDelay >= 0)
        return 0;
    for (const MainIconShortcut &shortcut : MAIN_ICON_SHORTCUTS) {
        if (!EV_GetShenmueFlag(shortcut.flag))
            continue;
        if (btnCheck(BUTTON_MAIN_ICON_SHORTCUT)) {
            UpdateSelectedItemStateFlag(shortcut.item);
            return 0;
        }
        RegisterButtonMediation(BUTTON_MAIN_ICON_SHORTCUT, nullptr, ButtonMediationBindA);
        return 1;
    }
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool DrawMainIconOverlay(std::uint32_t colour, float depth) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = colour};
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 320, 240, 640, 480, ScreenMode::Adjusted);
    packet.quad.flags |= 0x1400U;
    return shenSubmitQuad(packet, depth);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CtrlMainIcon_Callback(void *) {
    mainIconOverlayActive = 0;
    HLib::ExecuteCallbackNow([](void *) { TickScenarioFlagNotification(); }, nullptr);
    if (ctrlGetPadData())
        (void)ctrlGetPadInputBuffer0(ctrlGetPadData());
    if (nativeGuiActive && plymIsSearchable() && !plymIsSearchBlocked() && (g_loadingFlag & 2U) == 0 && renderIconFlags == 0 && !IsScenarioBusyForMainIcon() &&
        g_fightUiActive == 0 && srchCheckFlags(6) == 0 && srchCheckFlags(3) <= 0 && GetCurrentItemSelectState() == 0) {
        mainIconDelay = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(mainIconDelay) - 1U);
    } else if (mainIconDelay < 1) {
        mainIconDelay = 0;
        if ((g_loadingFlag & 2U) == 0)
            mainIconDelay = 5;
    } else {
        mainIconDelay = 5;
    }

    if (mainIconPhase) {
        if (mainIconPhase == 5)
            mainIconPhase = 0;
    } else {
        if (CheckMainIconShortcut()) {
            if (btnCheck(BUTTON_MAIN_ICON_MEMO)) {
                (void)RefreshPadTask(0, 1);
                return;
            }
            RegisterButtonMediation(BUTTON_MAIN_ICON_MEMO, nullptr, ButtonMediationBindA);
        }
        if (btnCheck(BUTTON_MAIN_ICON_ITEM_HELP) && helpGetCurrentIconIdx(1) == 67) {
            UpdateSelectedItemStateFlag(14);
        } else if (mainIconDelay < 0) {
            if (btnCheck(BUTTON_MAIN_ICON_ITEM_SELECT)) {
                if (nativeGuiActive == 1) {
                    mainIconPostFxBank = static_cast<std::int32_t>(GetPostFxBankValue(1));
                    SelectPostFxBank(1, 0);
                    BeginItemSelector();
                    mainIconPhase = 3;
                }
            } else {
                RegisterButtonMediation(BUTTON_MAIN_ICON_ITEM_SELECT, nullptr, ButtonMediationBindB);
            }
        }
    }

    const std::uint32_t itemState = GetCurrentItemSelectState() & 0x23U;
    if ((GetShouldRenderIcons(RenderIconSlot::ItemSelect) || (itemState & 0x21U) != 0) && IsWazamakiInactive() == 0 && (itemState & 2U) == 0) {
        DrawMainIconOverlay(0x76000000U, 0.3f);
        mainIconOverlayActive = 1;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void InitMainIcon_Callback(void *) {
    mainIconDelay = 5;
    iconPadControl = AllocatePadControlEntry();
    HLib::SetCurrentTaskTo(CtrlMainIcon_Callback);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorMemo() {
    return RefreshPadTask(1, 1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorMemo() {
    return IsItemSelectable();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CancelSelectorMemo(std::int32_t force) {
    if (force)
        RequestMemoClose();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorWatch() {
    return OpenGameWatchMenuItem();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorWatch() {
    return IsWristwatchOpenable();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CancelSelectorFullMap(std::int32_t) {
    CloseFullMapMenu();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorItemLook() {
    return OpenItemLook(itemSelectorSelection.selectedItem, 1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorItemLook() {
    return IsAnyItemSelectActive();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void CancelSelectorItemLook(std::int32_t force) {
    if ((GetCurrentItemSelectState() & 1U) && force)
        CloseItemLook();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorSave() {
    return OpenSaveGameMenuItem();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorSave() {
    return IsSaveAllowed();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CompleteSelectorSave() {
    shenSetTimeSimulationTo(true);
    CheckHelpIcon(itemSelectorDelayedResult == 32 ? 0 : 33, 0, CheckHelpIconOp::Set);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorEasyOptions() {
    SetNewEasyOptionState(0);
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorEasyOptions() {
    return IsEasyOptionActive();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CancelSelectorEasyOptions(std::int32_t force) {
    AbortEasyOptionState(force);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorMoves() {
    return !UpdateWazamakiMenu();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorCollection() {
    return OpenLegacyCollectionMenu(0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorPhotos() {
    return OpenLegacyCollectionMenu(2);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t UpdateSelectorCollection() {
    return static_cast<std::uint8_t>(UpdateLegacyCollectionMenu()) == 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t UpdateSelectorPhotos() {
    PublishItemSelectorOwnedMask(itemSelectorOwnedMask.data());
    return static_cast<std::uint8_t>(UpdateLegacyCollectionMenu()) == 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CancelSelectorCollection(std::int32_t force) {
    CloseLegacyCollectionMenu(force);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorNavigation() {
    return (GetCurrentItemSelectState() & 8U) != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CancelSelectorNavigation(std::int32_t force) {
    CloseNaviWindow(force);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorPawn() {
    return OpenPawnWindow(0.0f);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorPawn() {
    return IsPawnWindowOpen();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CancelSelectorPawn(std::int32_t force) {
    ClosePawnWindow(force);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t BeginSelectorMoveHandbook() {
    return OpenWfScrollMenu();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t UpdateSelectorMoveHandbook() {
    return GetwfMainControlState();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CancelSelectorMoveHandbook(std::int32_t force) {
    CloseWfScrollMenu(force);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ResolveUniqueItemAction(std::int32_t mode) {
    UniqueItemActionData *const action = static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
    if (!action || mode >= static_cast<std::int32_t>(ITEM_SELECTOR_CALLBACKS.size()))
        return;
    action->item = mode;
    action->begin = ITEM_SELECTOR_CALLBACKS[mode].begin;
    action->update = ITEM_SELECTOR_CALLBACKS[mode].update;
    action->complete = ITEM_SELECTOR_CALLBACKS[mode].complete;
    action->cancel = ITEM_SELECTOR_CALLBACKS[mode].cancel;
    HLib::ResolveDeferredCallback(uniqueItemActionTask);
    HLib::SetupTimedTask(itemSelectTask, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ResetUniqueItemAction(UniqueItemActionData &action) {
    action.begin = nullptr;
    action.update = nullptr;
    action.complete = nullptr;
    action.cancel = nullptr;
    action.item = -1;
    if (uniqueItemActionTask)
        HLib::SetupTimedTask(uniqueItemActionTask, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ICIT_UniqAction_Callback(void *) {
    UniqueItemActionData &action = *static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
    if (action.begin) {
        if (static_cast<std::uint32_t>(action.begin()) != 0) {
            (void)ExecuteAicaCommand(0x900abU, 0, 0);
        } else {
            (void)ExecuteAicaCommand(0x800abU, 0, 0);
            action.update = nullptr;
            action.complete = nullptr;
            action.cancel = nullptr;
            action.item = -1;
            RefreshItemSelectorInventory(0);
            HLib::SetupTimedTask(uniqueItemActionTask, 0);
            HLib::ResolveDeferredCallback(itemSelectTask);
            ToggleGameMenu();
        }
        action.begin = nullptr;
        return;
    }
    if (action.update && static_cast<std::uint32_t>(action.update()) != 0)
        return;
    if (action.complete)
        action.complete();
    iconToggleStateFor(1, true);
    RefreshItemSelectorLabel();
    RefreshItemSelectorInventory(0);
    (void)ExecuteAicaCommand(0x900abU, 0, 0);
    ResetUniqueItemAction(action);
    HLib::ResolveDeferredCallback(itemSelectTask);
    ToggleGameMenu();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
Character *ItemSelectorPlayer() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return static_cast<Character *>(control->ryo);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
ItemSelectorContext *CaptureItemSelectorContext() {
    ItemSelectorContext &context = itemSelectorContext;
    context.perspective = camCtrlGetPerspective();
    context.cameraState = camCtrlGetState();
    camCtrlGetNextCamPos(&context.eye);
    camCtrlGetTargetPosition(&context.target);
    context.helpTopic = helpGetCurrentIconIdx(1);
    context.visible = static_cast<std::uint16_t>(GetUiFlag(0) & 0xfdffU);
    context.selected = static_cast<std::uint8_t>(GetUiFlag(1));
    Character *const player = ItemSelectorPlayer();
    if (!player)
        return nullptr;
    context.playerDisplay = IsOfThisVisibilityType(player, 3);
    context.playerVisible = charaGetWasVisible(player);
    context.weather = SetWeatherLightManager(0);
    return &context;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateItemSelectorCameraGoal(ItemSelectorCamera &camera, Character &player) {
    const std::int32_t yaw = -static_cast<std::int16_t>(GetCharacterMotionYaw(&player));
    camera.goalEye = GetCharacterWorldPosition(&player);
    camera.goalEye.y += 1.6f;
    camera.goalTarget = camera.goalEye;
    camera.goalTarget.x += sin_fixed16(yaw) * 0.2f;
    camera.goalTarget.z = cos_fixed16(static_cast<std::uint16_t>(yaw)) * -0.2f + camera.goalTarget.z;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetItemSelectorCameraSteps(ItemSelectorCamera &camera, float frames) {
    camera.eyeStep = {(camera.goalEye.x - camera.eye.x) / frames, (camera.goalEye.y - camera.eye.y) / frames, (camera.goalEye.z - camera.eye.z) / frames};
    camera.targetStep = {
        (camera.goalTarget.x - camera.target.x) / frames, (camera.goalTarget.y - camera.target.y) / frames, (camera.goalTarget.z - camera.target.z) / frames};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void PrepareItemSelectorCamera() {
    using enum CameraState;
    Character *const player = ItemSelectorPlayer();
    if (!player)
        return;
    ItemSelectorCamera &camera = itemSelectorCamera;
    camera.duration = 8;
    camera.frame = 0;
    camera.mode = 2;
    camCtrlGetNextCamPos(&camera.savedEye);
    camCtrlGetTargetPosition(&camera.savedTarget);
    camera.savedAlpha = player->alpha;
    camCtrlSetState(Event);
    camera.eye = camera.savedEye;
    camera.target = camera.savedTarget;
    UpdateItemSelectorCameraGoal(camera, *player);
    SetItemSelectorCameraSteps(camera, static_cast<float>(camera.duration));
    camCtrlSetNextPosition(camera.eye);
    camCtrlSetTargetPosition(camera.target);
    camera.alpha = camera.savedAlpha;
    camera.alphaStep = -camera.savedAlpha / static_cast<float>(camera.duration / 2);
    player->alpha = camera.alpha;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool AdvanceItemSelectorCamera() {
    Character *const player = ItemSelectorPlayer();
    if (!player || srchCheckFlags(3) >= 0)
        return true;
    ItemSelectorCamera &camera = itemSelectorCamera;
    ++camera.frame;
    const bool finished = camera.frame > camera.duration;
    if (!finished) {
        if (camera.mode == 2) {
            UpdateItemSelectorCameraGoal(camera, *player);
            const std::int32_t remaining = camera.duration - camera.frame;
            SetItemSelectorCameraSteps(camera, static_cast<float>(remaining > 0 ? remaining : 1));
        }
        camera.eye.x += camera.eyeStep.x;
        camera.eye.y += camera.eyeStep.y;
        camera.eye.z += camera.eyeStep.z;
        camera.target.x += camera.targetStep.x;
        camera.target.y += camera.targetStep.y;
        camera.target.z += camera.targetStep.z;
        camera.alpha += camera.alphaStep;
        if (!(camera.alpha >= 0.0f))
            camera.alpha = 0.0f;
        else if (camera.alpha > 1.0f)
            camera.alpha = 1.0f;
    } else {
        camera.eye = camera.goalEye;
        camera.target = camera.goalTarget;
        camera.alpha = camera.mode == 0 ? camera.savedAlpha : 0.0f;
    }
    camCtrlSetNextPosition(camera.eye);
    camCtrlSetTargetPosition(camera.target);
    player->alpha = camera.alpha;
    return finished;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CloseItemSelectorCamera() {
    Character *const player = ItemSelectorPlayer();
    if (!player)
        return;
    ItemSelectorCamera &camera = itemSelectorCamera;
    camera.goalEye = camera.savedEye;
    camera.goalTarget = camera.savedTarget;
    camera.duration = 8;
    camera.frame = 0;
    camera.mode = 3;
    SetItemSelectorCameraSteps(camera, 8.0f);
    camCtrlSetNextPosition(camera.eye);
    camCtrlSetTargetPosition(camera.target);
    camera.alphaStep = camera.savedAlpha / static_cast<float>(camera.duration / 2);
    player->alpha = camera.alpha;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void RestoreItemSelectorContext() {
    if (!itemSelectorSelection.context)
        return;
    const ItemSelectorContext &context = *itemSelectorSelection.context;
    if (Character *const player = ItemSelectorPlayer()) {
        EV_SetCharDisplay(player, CVM_DisplayEnabled, context.playerDisplay);
        charaSetVisibilityFlagTo(player, context.playerVisible);
    }
    camCtrlSetState(context.cameraState);
    camCtrlSetPerspective(context.perspective);
    camCtrlSetNextPosition(context.eye);
    camCtrlSetTargetPosition(context.target);
    EV_EventSetUpHelpIcon_0(context.helpTopic, 0);
    SetHelpIconMenuFlags(context.visible);
    if (!context.visible)
        ResetHelpIconsToItemPrompt();
    SetWeatherLightManager(context.weather);
    itemSelectorSelection.context = nullptr;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void FinishItemSelector() {
    if (!GetShouldRenderIcons(RenderIconSlot::ItemSelect))
        return;
    if (itemSelectTask)
        HLib::SetupTimedTask(itemSelectTask, 0);
    itemSelectPhase = 0;
    padcRefreshCurrentInputs(false);
    ToggleRenderIcons(RenderIconSlot::ItemSelect, false);
    itemSelectorSelection.exitFlags = 0;
    RestoreItemSelectorContext();
    itemSelectorLabelTexture = -1;
    SelectPostFxBank(1, mainIconPostFxBank);
    mainIconPhase = 5;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool ConfirmGameMenuClose() {
    if (AskKeepOrRevertChanges())
        return false;
    if (DestroyElementByIndex(UIElement::GameMenu) != 1)
        return false;
    itemSelectPhase = 4;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ResetLegacySelectorSubmenu() {
    if (legacySelectorSubmenuPhase == 1) {
        // Both authored transition IDs are -1;
        // the transition owner is left untouched for those IDs.
        static constexpr std::array<std::int32_t, 8> elements{44, 45, -1, 46, 47, 30, 48, 57};
        DestroyElementByIndex(static_cast<UIElement>(elements[legacySelectorSubmenuSelection]));
    }
    legacySelectorSubmenuPhase = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool NavigateLegacyItemSelector(std::uint16_t pressed) {
    ItemSelectorSelection &selection = itemSelectorSelection;
    if (selection.activeMenu == 9 && selection.actionResult == 0) {
        if (selection.count < 2)
            pressed &= 0x3fffU;
        if (selection.selectedItem < 0)
            pressed &= 0xfbffU;
        if (pressed & 0x400U) {
            std::int32_t mode = 3;
            for (std::int32_t index = 0; index < 3; ++index)
                if (selection.selectedItem == ITEM_SELECTOR_SPECIAL_ITEMS[index]) {
                    mode = index;
                    break;
                }
            ResolveUniqueItemAction(mode);
            return false;
        }
        if (pressed & 0x4000U) {
            if (--selection.column < 0)
                selection.column = selection.columns - 1;
            ClampItemSelectorSelection();
        } else if (pressed & 0x8000U) {
            ++selection.column;
            if (selection.column + selection.row * selection.columns >= selection.count)
                selection.column = 0;
            if (selection.column >= selection.columns)
                selection.column = 0;
        } else if (pressed & 0x2000U) {
            if (selection.column + (selection.row + 1) * selection.columns >= selection.count || selection.row == selection.visibleRows - 1) {
                selection.lowerRow = 0;
                selection.activeMenu = selection.column < ITEM_SELECTOR_LOWER_COUNTS[0] ? selection.column : ITEM_SELECTOR_LOWER_COUNTS[0] - 1;
            } else
                ++selection.row;
        } else if (pressed & 0x1000U) {
            if (--selection.row < 0) {
                selection.lowerRow = 1;
                selection.activeMenu = selection.column < ITEM_SELECTOR_LOWER_COUNTS[1] ? selection.column : ITEM_SELECTOR_LOWER_COUNTS[1] - 1;
            }
        }
    } else if (pressed & 0x400U) {
        ResolveUniqueItemAction(ITEM_SELECTOR_ACTIONS[ITEM_SELECTOR_MENU_MAP[selection.lowerRow][selection.activeMenu]]);
        return false;
    } else if (pressed & 0x1000U) {
        if (--selection.lowerRow < 0) {
            selection.row = selection.visibleRows - 1;
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
            selection.activeMenu = 9;
            if (selection.column + selection.row * selection.columns >= selection.count && selection.row >= 1)
                --selection.row;
            ClampItemSelectorSelection();
        } else {
            if (selection.activeMenu >= ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow])
                selection.activeMenu = ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow] - 1;
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
        }
    } else if (pressed & 0x2000U) {
        if (++selection.lowerRow < 2) {
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
        } else {
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
            selection.row = 0;
            selection.activeMenu = 9;
            ClampItemSelectorSelection();
        }
    } else if (pressed & 0x8000U) {
        selection.activeMenu = (selection.activeMenu + 1) % ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow];
    } else if (pressed & 0x4000U) {
        selection.activeMenu = selection.activeMenu ? selection.activeMenu - 1 : ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow] - 1;
    }
    return (pressed & 0xf000U) != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ProcessLegacyItemSelectorInput() {
    ItemSelectorSelection &selection = itemSelectorSelection;
    const std::uint16_t rawInput = GetPadControlData(1);
    std::uint16_t pressed = itemSelectorDelayedResult ? 0 : rawInput & 0xf600U;
    itemSelectorNavigationInput &= 0xffff0000U;
    if (pressed & 0x200U)
        itemSelectPhase = 4;
    else {
        if (selection.activeMenu != 9 || selection.actionResult != 0)
            itemSelectorNavigationInput &= 0xffffU;
        if (pressed && NavigateLegacyItemSelector(pressed)) {
            RefreshItemSelectorSelectedItem();
            RefreshItemSelectorLabel();
        }
    }
    if (itemSelectorDelayedResult == 2) {
        itemSelectPhase = 4;
        selection.actionResult = (static_cast<std::uint32_t>(selection.selectedItem) << 16U) | 2U;
    } else if (itemSelectorDelayedResult == 32) {
        itemSelectPhase = 6;
        iconToggleStateFor(1, false);
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    }
    itemSelectorDelayedResult = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CtrlItemSelectIcon_Callback(void *) {
    if (Character *const player = ItemSelectorPlayer())
        charaSetFlagFor(player, 1);
    if (!(g_loadingFlag & 2U)) {
        switch (itemSelectPhase) {
        case 1:
            itemSelectPhase = 2;
            itemSelectorSelection.exitFlags = 0;
            itemSelectorSelection.context = CaptureItemSelectorContext();
            itemSelectorWindowPosition = {-672.0f, 55.0f};
            itemSelectorCameraActive = true;
            PrepareItemSelectorCamera();
            RefreshItemSelectorSelectedItem();
            RefreshItemSelectorLabel();
            break;
        case 2: {
            const std::uint16_t pendingInput = static_cast<std::uint16_t>(GetPadControlData(1) & 0x202U);
            if (!(pendingInput & 0x200U) && (pendingInput & 2U))
                itemSelectorNavigationInput = (itemSelectorNavigationInput & 0xffff0000U) | pendingInput;
            if (AdvanceItemSelectorCamera()) {
                iconToggleStateFor(1, true);
                itemSelectorWindowPosition.x = -55.0f;
                itemSelectPhase = 3;
                EV_EventSetUpHelpIcon_0(0x11, 0);
                if (itemSelectorSelection.context->helpTopic)
                    SetHelpIconMenuFlags(0x21);
                else
                    AddHelpIconMenuFlags(0x21);
                ShowElementByIndex(UIElement::GameMenu);
            }
            break;
        }
        case 3:
            if (itemSelectorSelection.exitFlags & 1U) {
                iconToggleStateFor(1, false);
                itemSelectPhase = 4;
            } else if (GetDebugForceUIEnabled(UIElement::GameMenu)) {
                if (((itemSelectPhase - 4) & 0xfc) != 0 || itemSelectPhase == 6) {
                    if (ExecuteElementCallback(UIElement::GameMenu) == 0 && g_debugImGuiRenderMode == 0)
                        itemSelectPhase = 4;
                }
            } else
                ProcessLegacyItemSelectorInput();
            set_icon_select_flags_to(0x600U);
            break;
        case 4:
            itemSelectPhase = 5;
            break;
        case 5:
            iconToggleStateFor(1, false);
            ResetLegacySelectorSubmenu();
            [[fallthrough]];
        case 6:
            if (!itemSelectorSelection.context || static_cast<std::int32_t>(itemSelectorSelection.context->cameraState) != 0) {
                CloseItemSelectorCamera();
                itemSelectPhase = 7;
            } else {
                itemSelectorCameraActive = false;
                camCtrlSetFovTransitionTo(false);
                FinishItemSelector();
            }
            break;
        case 7:
            if (itemSelectorCameraActive && AdvanceItemSelectorCamera()) {
                itemSelectorCameraActive = false;
                FinishItemSelector();
            }
            break;
        default:
            break;
        }
    }
    if (GetItemSelectionMode(1))
        HLib::ExecuteCallbackNow(RenderLegacyItemSelector, &itemSelectorSelection);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitItemSelectIcon_Callback(void *) {
    itemLookTask = HLib::EnqueueTaskWithoutParameter(ICSU_InitItemLook_Callback, HLib::GetCurrentTaskIndex(), 0, static_cast<std::int32_t>(TASK_ICLI.name));
    uniqueItemActionTask = HLib::EnqueueTaskWithParameter(ICIT_UniqAction_Callback, HLib::GetCurrentTaskIndex(), 0, sizeof(UniqueItemActionData), TASK_ICSS);
    if (uniqueItemActionTask) {
        UniqueItemActionData *const action = static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
        if (action) {
            action->begin = nullptr;
            action->update = nullptr;
            action->complete = nullptr;
            action->cancel = nullptr;
            action->item = -1;
        }
        HLib::SetupTimedTask(uniqueItemActionTask, 0);
    }
    InitializeItemSelectorWindow();
    InitializeLegacyCollectionMenu();
    itemSelectPhase = 0;
    iconToggleStateFor(1, false);
    itemSelectorSelection.activeMenu = 9;
    itemSelectorSelection.actionResult = 0;
    itemSelectorSelection.count = 0;
    itemSelectorSelection.row = itemSelectorSelection.column = 0;
    itemSelectorSelection.visibleRows = 1;
    itemSelectorSelection.columns = 9;
    itemSelectorSelection.glowColour = itemSelectorSelection.exitFlags = 0;
    itemSelectorSelection.glowBrightening = 1;
    itemSelectorSelection.context = nullptr;
    itemSelectorSelection.selectedItem = -1;
    helpIconItemEffectState = 0;
    itemSelectorNavigationInput = 0;
    currentItemSelectState = 0;
    itemSelectorCameraActive = false;
    itemSelectTask = HLib::GetCurrentTask();
    Queue_wf_MainControl(7);
    HLib::SetCurrentTaskTo(CtrlItemSelectIcon_Callback);
    HLib::SetupTimedTask(itemSelectTask, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t MoneyIconStep(std::int32_t delta) {
    if (moneyIconFlags & 0x40) {
        const std::uint32_t raw = static_cast<std::uint32_t>(moneyIconFixedStep);
        const std::uint32_t magnitude = moneyIconFixedStep < 0 ? 0U - raw : raw;
        return std::bit_cast<std::int32_t>((delta < 0 ? 0U - magnitude : magnitude));
    }
    const std::int32_t sign = delta < 0 ? -1 : 1;
    const std::uint32_t raw = static_cast<std::uint32_t>(delta);
    std::int32_t remaining = std::bit_cast<std::int32_t>(delta < 0 ? 0U - raw : raw);
    std::int32_t step = 0;
    for (const std::int32_t increment : moneyIconIncrements) {
        if (remaining < increment)
            continue;
        step += increment;
        remaining %= increment;
    }
    return sign * step;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t ReleaseCtrlSpeakTextHandle(std::int32_t handle) {
    for (std::size_t slot = 0; slot < ctrlSpeakTexturePoolHandles.size(); ++slot)
        if (ctrlSpeakTexturePoolHandles[slot] == handle)
            ctrlSpeakTexturePoolFlags[slot] &= ~1U;
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ReleaseMoneyTextSlots() {
    for (LegacyIconSprite &sprite : moneyIconTextSprites) {
        if (sprite.icon != 0xffffU)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
        sprite.icon = 0xffffU;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ChooseMoneyOptions(std::int32_t flags) {
    for (std::size_t index = 0; index < moneyIconTextSprites.size(); ++index) {
        if (!(flags & MONEY_TEXT_OPTION_FLAGS[index]))
            continue;
        LegacyIconSprite &sprite = moneyIconTextSprites[index];
        if (sprite.icon != 0xffffU)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
        sprite.icon = static_cast<std::uint16_t>(AcquireCtrlSpeakTextHandle(MONEY_TEXT_OPTION_NAMES[index]));
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// The numeric mode borrows the dialogue Action Selector's input/transition owner.
void SetNumericActionSelector(const std::array<std::int32_t, 4> &choices, std::int32_t maximum) {
    NumericActionSelector &selector = numericActionSelector;
    selector.choices = choices;
    selector.mode = 1;
    selector.amount = 0;
    selector.step = choices[1];
    selector.maximum = maximum;
    SetGameMessageContent(1, 0, "0");
    ShowElementByIndex(UIElement::GameMessage);
    for (std::size_t slot = 0; slot < choices.size(); ++slot) {
        std::array<char, 256> &label = selector.labels[slot];
        if (slot == 1 || slot == 2) {
            FormatMoneyAmount(label.data(), choices[slot], true);
        } else {
            const std::string_view text = GetActionSelectorLabel(static_cast<std::uint16_t>(choices[slot]));
            std::memcpy(label.data(), text.data(), text.size());
            char *output = label.data() + text.size();
            if (choices[slot] == 485) {
                *output++ = ' ';
                *output++ = '(';
                FormatMoneyAmount(output, maximum, true);
                output += std::strlen(output);
                *output++ = ')';
            }
            *output = '\0';
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t GetNumericActionSelectorAmount() {
    return static_cast<std::uint32_t>(numericActionSelector.amount);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t ConsumeActionSelectorInput() {
    const std::uint32_t input = actionSelectorInput;
    actionSelectorInput = 13;
    return input;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void UpdateMoneySelection() {
    const std::int32_t previous = moneyIconAmount;
    moneyIconAmount = static_cast<std::int32_t>(GetNumericActionSelectorAmount());
    const std::uint32_t input = ConsumeActionSelectorInput();
    if ((input == 4 || input == 9) && previous == 0) {
        moneyIconResult = -2;
        moneyIconPhase = 4;
    } else if (input == 8) {
        moneyIconPhase = 4;
    }
    if (previous == moneyIconAmount &&
        ((input == 2 && moneyIconAmount <= 0) || (input == 6 && moneyIconAmount >= moneyIconMaximum) || (input == 0 && moneyIconAmount >= moneyIconMaximum)))
        (void)ExecuteAicaCommand(0x800abU, 0, 0);
    set_icon_select_flags_to(input);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool UpdateMoneyCash() {
    moneyIconSettleFrames -= 1.0f;
    if (moneyIconDisplay == moneyIconTarget) {
        if (moneyIconSettleFrames > 0.0f)
            return false;
        moneyIconSettleFrames = 0.0f;
        return true;
    }
    const bool decreasing = moneyIconDisplay > moneyIconTarget;
    const std::uint32_t display = static_cast<std::uint32_t>(moneyIconDisplay);
    const std::uint32_t target = static_cast<std::uint32_t>(moneyIconTarget);
    moneyIconLastIncrement = MoneyIconStep(std::bit_cast<std::int32_t>(decreasing ? display - target : target - display));
    const std::uint32_t step = static_cast<std::uint32_t>(moneyIconLastIncrement);
    moneyIconDisplay = std::bit_cast<std::int32_t>(decreasing ? display - step : display + step);
    const bool complete = decreasing ? moneyIconDisplay <= moneyIconTarget : moneyIconDisplay >= moneyIconTarget;
    if (complete)
        moneyIconDisplay = moneyIconTarget;
    if (!(moneyIconFlags & 0x20)) {
        if (moneyIconDrawMode)
            SetCurrentCash(moneyIconDisplay);
        else
            SetCurrentSegaCoins(moneyIconDisplay);
    }
    SetGameMessageContent(moneyIconDrawMode ? 1 : 2, moneyIconDisplay, "0");
    (void)ExecuteAicaCommand(0xd00abU, 0, 0);
    return complete;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void AnimateMoneyUiElement(void *) {
    if (!(moneyIconFlags & 4))
        return;
    const std::int32_t amount = moneyIconDisplay - ((moneyIconFlags & 8) ? moneyIconAmount : 0);
    std::uint32_t colour = 0xff656565U;
    std::uint8_t cash = 2;
    LegacyIconSprite &icon = moneyIconSprite;
    icon.x = moneyIconPosition.x;
    icon.y = moneyIconPosition.y;
    icon.icon = 1544;
    icon.width = 80;
    icon.height = 40;
    if (!moneyIconDrawMode || moneyIconDrawMode == 3) {
        icon.icon = moneyIconDrawMode ? 515 : 3;
        icon.width = icon.height = 36;
        icon.y += 4.0f;
        if (moneyIconDrawMode)
            colour = 0xe0105010U;
        else
            cash = 0;
    }
    PrepareDrawWithMode(icon, static_cast<ScreenMode>(0));
    const float x = moneyIconPosition.x - 63.0f, y = moneyIconPosition.y - 21.0f;
    DrawLegacyWindowHorizontal(x, y, 6, 1, 0, 0.2f, 1024, static_cast<ScreenMode>(0));
    DrawLegacyWindowHorizontal(x, y + 48.0f, 6, 1, 1, 0.2f, 1024, static_cast<ScreenMode>(0));
    DrawLegacyWindowTiles(x + 35.0f, y + 24.0f, 6, 1, 0.18f, 1024, static_cast<ScreenMode>(0));
    DrawLegacyWindowBackground(x + 15.0f, y + 4.0f, 240.0f, 40.0f, 0.19f, 1024, static_cast<ScreenMode>(0));
    DrawMoneyAmount(moneyIconPosition.x + 148.0f, moneyIconPosition.y + 10.0f, moneyIconPosition.depth, 1.0f, amount, cash, colour, static_cast<ScreenMode>(0));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void SetupMoneyIconForRendering(void *) {
    const std::array<bool, 4> visible{true, true, (moneyIconFlags & 2) != 0, (moneyIconFlags & 1) != 0};
    for (std::size_t index = 0; index < moneyIconDirections.size(); ++index) {
        LegacyIconSprite &sprite = moneyIconDirections[index];
        sprite.x = moneyIconPosition.x + 43.0f + MONEY_DIRECTION_X[index];
        sprite.y = moneyIconSelectorPosition.y - 2.6875f + MONEY_DIRECTION_Y[index];
        if (moneyIconPosition.selectedDirection == index) {
            sprite.scaleX = sprite.scaleY = moneyIconSelectionScale;
            if (moneyIconSelectionScale <= 1.0f)
                moneyIconPosition.selectedDirection = 4;
        }
        if (visible[index])
            PrepareDrawSimple(sprite, 0xb0000000U, static_cast<ScreenMode>(0));
    }
    const std::int32_t digits = GetSelectorNumberDigits(moneyIconStepAmount);
    const float leadingOffset = digits >= 0 ? static_cast<float>(digits) * -16.0f : -16.0f;
    for (std::int32_t index = 1; index >= 0; --index) {
        const LegacyIconSprite &sprite = moneyIconDirections[index];
        DrawMoneyDigits(sprite.x + 16.0f, sprite.y, sprite.scaleX, sprite.depth + 1.0f, moneyIconStepAmount, 0xff656565U, static_cast<ScreenMode>(0));
        LegacyIconSprite &sign = moneyIconSigns[index];
        sign.x = sprite.x + 16.0f + sprite.scaleX * leadingOffset;
        sign.y = sprite.y;
        sign.scaleX = sign.scaleY = sprite.scaleX;
        PrepareDrawSimple(sign, 0xff656565U, static_cast<ScreenMode>(0));
    }
    DrawMoneyAmount(moneyIconPosition.x + 48.0f, 170.0f, 3.0f, 1.0f, moneyIconAmount, moneyIconCurrency, 0, static_cast<ScreenMode>(0));
    for (std::int32_t index = 1; index >= 0; --index) {
        if (!(moneyIconFlags & MONEY_TEXT_RENDER_FLAGS[index]))
            continue;
        const LegacyIconSprite &direction = moneyIconDirections[index + 2];
        LegacyIconSprite &text = moneyIconTextSprites[index];
        text.x = direction.x;
        text.y = direction.y;
        text.scaleX = direction.scaleX;
        text.scaleY = direction.scaleY;
        if (text.icon != 0xffffU)
            PrepareDrawTextureWithMode(text, static_cast<ScreenMode>(0));
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool MoneyMessageTransitionBusy() {
    return gameMessageTransition == -1 || IsUITransitionActive(gameMessageTransition);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void RestoreMoneyHelpState() {
    if (moneyIconSavedHelpPresent)
        SetHelpIconMenuFlags(moneyIconSavedHelpVisible);
    else
        ResetHelpIconsToItemPrompt();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CtrlMoneyIcon_Callback(void *) {
    switch (moneyIconPhase) {
    case 2:
        moneyIconSprite.icon = moneyIconDrawMode ? 1544 : 3;
        moneyIconSprite.width = moneyIconSprite.height = moneyIconDrawMode ? 40 : 36;
        if (moneyIconDrawMode)
            moneyIconSprite.width = 80;
        if (!(moneyIconFlags & 0x20))
            moneyIconDisplay = moneyIconDrawMode ? GetCurrentCash() : GetCurrentSegaCoins();
        moneyIconPhase = 1;
        iconToggleStateFor(4, true);
        moneyIconPosition.x = -516.0f;
        moneyIconPosition.selectedDirection = 4;
        moneyIconSettleFrames = 4.0f;
        moneyIconAmount = 0;
        moneyIconResult = 0;
        moneyIconSelectionScale = 1.0f;
        ChooseMoneyOptions(moneyIconFlags);
        if (moneyIconFlags & 8) {
            padcRefreshCurrentInputs(true);
            if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
                ShowElementByIndex(UIElement::ActionSelector);
                SetNumericActionSelector({485, moneyIconStepAmount, -moneyIconStepAmount, 483}, moneyIconMaximum);
            }
        } else if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            SetGameMessageContent(moneyIconDrawMode ? 1 : 2, moneyIconDisplay, "0");
            ShowElementByIndex(UIElement::GameMessage);
        }
        break;
    case 1:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (MoneyMessageTransitionBusy())
                break;
        } else if (!AdvanceIconPosition(moneyIconPosition.x, moneyIconPosition.velocity, 80.0f)) {
            break;
        }
        if (moneyIconFlags & 8) {
            moneyIconSavedHelpSelected = static_cast<std::uint16_t>(GetUiFlag(1));
            moneyIconSavedHelpVisible = static_cast<std::uint16_t>(GetUiFlag(0));
            moneyIconSavedHelpPresent = static_cast<std::uint16_t>(GetUiFlag(2));
            if (moneyIconSavedHelpPresent)
                SetHelpIconMenuFlags(33);
            else
                AddHelpIconMenuFlags(33);
        }
        moneyIconPhase = 3;
        break;
    case 3:
        if (moneyIconFlags & 8) {
            moneyIconFrames -= 1.0f;
            if (moneyIconFrames > 0.0f)
                UpdateMoneySelection();
            else {
                moneyIconPhase = 4;
                moneyIconAmount = 0;
                moneyIconResult = -1;
                DestroyElementByIndex(UIElement::GameMessage);
                DestroyElementByIndex(UIElement::ActionSelector);
            }
        } else {
            if (moneyIconFrames >= 30.0f)
                moneyIconFrames -= 1.0f;
            if (moneyIconFrames < 30.0f && (moneyIconFlags & 0x10) && UpdateMoneyCash() && (moneyIconFlags & 0x10)) {
                moneyIconFrames -= 1.0f;
                if (moneyIconFrames <= 0.0f) {
                    moneyIconPhase = 4;
                    moneyIconFrames = 0.0f;
                }
            }
        }
        break;
    case 4:
        if (moneyIconPosition.selectedDirection == 4) {
            moneyIconPhase = 5;
            DestroyElementByIndex(UIElement::GameMessage);
            if (moneyIconFlags & 8)
                DestroyElementByIndex(UIElement::ActionSelector);
        }
        break;
    case 5:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (MoneyMessageTransitionBusy())
                break;
        } else if (!RetreatIconPosition(moneyIconPosition.x, moneyIconPosition.velocity, -516.0f)) {
            break;
        }
        moneyIconPhase = 0;
        iconToggleStateFor(4, false);
        ToggleRenderIcons(RenderIconSlot::MoneyMessage, false);
        moneyIconDrawMode = 2;
        if (moneyIconFlags & 8) {
            RestoreMoneyHelpState();
            padcRefreshCurrentInputs(false);
        }
        moneyIconFlags = 0;
        ReleaseMoneyTextSlots();
        break;
    default:
        break;
    }
    if (!GetItemSelectionMode(4))
        return;
    if (GetDebugForceUIEnabled(UIElement::GameMessage))
        EnableElement(UIElement::GameMessage);
    HLib::ExecuteCallbackNow(AnimateMoneyUiElement, nullptr);
    if (moneyIconFlags & 8) {
        if (moneyIconPosition.selectedDirection != 4) {
            moneyIconSelectionScale += 0.25f;
            if (moneyIconSelectionScale >= 2.0f)
                moneyIconSelectionScale = 1.0f;
        }
        HLib::ExecuteCallbackNow(SetupMoneyIconForRendering, nullptr);
        EnableElement(UIElement::ActionSelector);
        ExecuteElementCallback(UIElement::ActionSelector);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void PlayQuickIconResult(bool firstTable) {
    if ((quickIconFlags & 1U) == 0 || (quickIconFlags & 0x80U) != 0)
        return;
    const std::size_t variant = (quickIconFlags >> 10U) & 1U;
    (void)ExecuteAicaCommand(firstTable ? QUICK_ICON_FIRST_SOUND[variant] : QUICK_ICON_SECOND_SOUND[variant], 0, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateQuickIconBlink() {
    quickIconPreviousDrawFrames = quickIconDrawFrames;
    if (quickIconPhase != 3)
        return;
    quickIconDrawFrames += shenGetScaledDeltaTickTime(1.0f);
    if (quickIconUnpressed == 1) {
        if (quickIconDrawFrames >= 3.0f) {
            quickIconUnpressed = false;
            quickIconDrawFrames = 0.0f;
        }
        if (quickIconPreviousDrawFrames < 1.0f && quickIconDrawFrames >= 1.0f && (quickIconFlags & 1U) != 0 && (quickIconFlags & 0x400U) == 0) {
            (void)ExecuteAicaCommand(0x700abU, 0, 0);
        }
    } else if (quickIconDrawFrames >= 2.0f) {
        quickIconUnpressed = true;
        quickIconDrawFrames = 0.0f;
        quickIconPreviousDrawFrames = 0.0f;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateQuickIconSplitDirections() {
    const std::uint16_t pressed = GetPadControlData(1) & 0xf6c6U;
    std::uint16_t status{};
    if ((quickIconRequiredStatus & 0x40U) != 0) {
        status = quickIconDirectionalStatuses[pressed >> 12U] & 0xeeU;
        if ((pressed & 0x4000U) != 0)
            status = 64;
        if ((pressed & 0x8000U) != 0)
            status = 4;
    } else {
        status = quickIconDirectionalStatuses[pressed >> 12U] & 0xbbU;
        if ((pressed & 0x1000U) != 0)
            status = 1;
        if ((pressed & 0x2000U) != 0)
            status = 16;
    }
    if (g_forcePlayerGodmode != 0
#if SHENMUE_MODS_ENABLED && SHENMUE_ENHANCEMENT_MISC_MODS_QTE
        || mods::Enabled(mods::Id::QteSuccess)
#endif
    )
        status = quickIconRequiredStatus;
    if ((status & quickIconRequiredStatus) != 0) {
        quickIconCurrentStatus = static_cast<std::int16_t>(status | 0x2000U);
        PlayQuickIconResult((quickIconFlags & 8U) == 0);
        quickIconPhase = 4;
    } else if (quickIconMaximumFrames > 0.0f) {
        quickIconElapsedFrames += shenGetScaledDeltaTickTime(1.0f);
        if (quickIconElapsedFrames > quickIconMaximumFrames) {
            quickIconCurrentStatus = static_cast<std::int16_t>(0xa000U);
            quickIconTerminalStatus = static_cast<std::int16_t>(0xa000U);
            quickIconPhase = 4;
            quickIconMaximumFrames = 0.0f;
            PlayQuickIconResult((quickIconFlags & 8U) != 0);
            quickIconUnpressed = 1;
        }
    }
    UpdateQuickIconBlink();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateQuickIconSingleButton() {
    const std::uint16_t pressed = GetPadControlData(1) & quickIconButtonMask & 0xf6c6U;
    std::uint16_t status = quickIconDirectionalStatuses[pressed >> 12U];
    std::uint8_t faceButtonCount{};
    if ((pressed & 0x0400U) != 0) {
        ++faceButtonCount;
        status |= 0x0100U;
    }
    if ((pressed & 0x0200U) != 0) {
        ++faceButtonCount;
        status |= 0x0200U;
    }
    if ((pressed & 0x0002U) != 0) {
        ++faceButtonCount;
        status |= 0x0800U;
    }
    if ((pressed & 0x0004U) != 0) {
        ++faceButtonCount;
        status |= 0x0400U;
    }
    if ((status & 0x0f00U) == 0) {
        if (quickIconRequiredStatus == 1 && (pressed & 0x1000U) != 0)
            status = 1;
        else if (quickIconRequiredStatus == 16 && (pressed & 0x2000U) != 0)
            status = 16;
        else if (quickIconRequiredStatus == 64 && (pressed & 0x4000U) != 0)
            status = 64;
        else if (quickIconRequiredStatus == 4 && (pressed & 0x8000U) != 0)
            status = 4;
    }
    if (g_forcePlayerGodmode != 0
#if SHENMUE_MODS_ENABLED && SHENMUE_ENHANCEMENT_MISC_MODS_QTE
        || mods::Enabled(mods::Id::QteSuccess)
#endif
    )
        status = quickIconRequiredStatus;
#if SHENMUE_MODS_ENABLED && SHENMUE_ENHANCEMENT_MISC_MODS_QTE
    if (mods::Enabled(mods::Id::QteSuccess))
        faceButtonCount = 0;
#endif
    if (status != 0) {
        quickIconCurrentStatus = static_cast<std::int16_t>(status);
        bool invalid = (quickIconFlags & 0x40U) != 0 || faceButtonCount >= 2 || ((status & 0x0f00U) != 0 && static_cast<std::uint8_t>(status) != 0);
#if SHENMUE_MODS_ENABLED && SHENMUE_ENHANCEMENT_MISC_MODS_QTE
        invalid &= !mods::Enabled(mods::Id::QteSuccess);
#endif
        if (invalid) {
            PlayQuickIconResult((quickIconFlags & 8U) != 0);
            quickIconCurrentStatus = 0;
        } else if (status == quickIconRequiredStatus) {
            quickIconCurrentStatus = static_cast<std::int16_t>(status | 0x2000U);
            PlayQuickIconResult((quickIconFlags & 8U) == 0);
        } else {
            quickIconCurrentStatus = 0;
            PlayQuickIconResult((quickIconFlags & 8U) != 0);
        }
        quickIconPhase = 4;
        quickIconUnpressed = 1;
    } else if (quickIconMaximumFrames > 0.0f) {
        quickIconElapsedFrames += shenGetScaledDeltaTickTime(1.0f);
        if (quickIconElapsedFrames > quickIconMaximumFrames) {
            quickIconPhase = 4;
            quickIconCurrentStatus = static_cast<std::int16_t>(0xa000U);
            quickIconTerminalStatus = static_cast<std::int16_t>(0xa000U);
            quickIconMaximumFrames = 0.0f;
            const bool strict = (quickIconFlags & 0x40U) != 0;
            PlayQuickIconResult(strict ? (quickIconFlags & 8U) == 0 : (quickIconFlags & 8U) != 0);
            quickIconUnpressed = 1;
        }
    }
    UpdateQuickIconBlink();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void RetireQuickIcon(bool resetPosition) {
    quickIconPhase = 0;
    padcRefreshCurrentInputs(false);
    ToggleRenderIcons(RenderIconSlot::Qte, false);
    iconToggleStateFor(3, false);
    if (resetPosition)
        quickIconPosition[0] = -176.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawQuickIcon(void *) {
    quickIconSprite.x = quickIconPosition[0];
    if (quickIconFlags & 0x20U) {
        for (std::int8_t bit = 0; bit < 8; ++bit) {
            if (!(quickIconRequiredStatus & (1U << bit)))
                continue;
            quickIconSprite.rotation = static_cast<std::uint32_t>(bit) << 13U;
            quickIconSprite.x = FixedAngleSin(static_cast<std::uint16_t>(quickIconSprite.rotation)) * 55.0f + quickIconPosition[0];
            quickIconSprite.y = quickIconPosition[1] - FixedAngleCos(static_cast<std::uint16_t>(quickIconSprite.rotation)) * 55.0f;
            PrepareDrawWithMode(quickIconSprite, ScreenMode::AdjustedWithOffset);
        }
    } else
        PrepareDrawWithMode(quickIconSprite, ScreenMode::Centered);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CtrlQuickIcon_Callback(void *) {
    switch (quickIconPhase) {
    case 1:
        quickIconPhase = 3;
        break;
    case 2:
        if ((quickIconFlags & 1U) != 0 && (quickIconFlags & 0x400U) == 0)
            (void)ExecuteAicaCommand(0x700abU, 0, 0);
        quickIconPhase = 1;
        if ((quickIconFlags & 0x200U) == 0)
            padcRefreshCurrentInputs(true);
        iconToggleStateFor(3, true);
        quickIconPosition[0] = 320.0f;
        quickIconTerminalStatus = -1;
        quickIconUnpressed = true;
        break;
    case 3:
        if ((quickIconFlags & 0x100U) != 0)
            UpdateQuickIconBlink();
        else if ((quickIconFlags & 0x20U) != 0)
            UpdateQuickIconSplitDirections();
        else
            UpdateQuickIconSingleButton();
        break;
    case 4:
        if ((quickIconFlags & 0x10U) != 0)
            RetireQuickIcon(true);
        else {
            quickIconPhase = 5;
            if ((quickIconFlags & 4U) == 0 || quickIconTerminalStatus == static_cast<std::int16_t>(0xa000U))
                BeginBattleQteDeparture();
        }
        break;
    case 5:
        if (((quickIconFlags & 4U) != 0 && quickIconTerminalStatus != static_cast<std::int16_t>(0xa000U)) || !IsBattleQteTransitionActive())
            RetireQuickIcon(false);
        break;
    default:
        break;
    }
    if (GetDebugForceUIEnabled(UIElement::GameQte)) {
        if ((quickIconFlags & 2U) != 0 && GetItemSelectionMode(3) != 0) {
            SetupBattleQteIcon(quickIconRequiredStatus, static_cast<std::uint8_t>(quickIconFlags));
            if (quickIconUnpressed)
                EnableElement(UIElement::GameQte);
        }
    } else if ((quickIconFlags & 2U) != 0 && GetItemSelectionMode(3) != 0 && quickIconUnpressed) {
        HLib::ExecuteCallbackNow(DrawQuickIcon, nullptr);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitQuickIcon_Callback(void *) {
    quickIconPositionFlags = 0;
    quickIconSprite.height = 64;
    quickIconLegacyTexture = -1;
    quickIconPosition = QUICK_ICON_POSITION;
    quickIconSprite.flags = 0;
    quickIconSprite.rotation = 0;
    quickIconLegacyReserved = 0;
    quickIconLoadingReleaseFlag = 0;
    quickIconLegacyWidth = 32;
    quickIconSprite.x = -176.0f;
    quickIconSprite.y = 240.0f;
    quickIconPhase = 0;
    quickIconPositionDepth = 3.0f;
    quickIconSprite.icon = 0x700;
    quickIconSprite.width = 64;
    quickIconSprite.scaleX = 1.0f;
    quickIconSprite.scaleY = 1.0f;
    quickIconSprite.depth = 3.0f;
    quickIconSprite.alignment = 5;
    quickIconLegacyMode = 4;
    HLib::SetCurrentTaskTo(CtrlQuickIcon_Callback);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void DrawItemEffectWindow(std::int32_t columns, float x, float y, float depth) {
    const ScreenMode mode = static_cast<ScreenMode>(0);
    DrawLegacyWindowHorizontal(x, y, columns, 1, 0, depth, 1024, mode);
    DrawLegacyWindowHorizontal(x, y + 48.0f, columns, 1, 1, depth, 1024, mode);
    DrawLegacyWindowTiles((x + 24.0f) + 11.0f, y + 24.0f, columns, 1, depth - 0.02f, 1024, mode);
    DrawLegacyWindowBackground((x + 11.0f) + 4.0f, y + 4.0f, static_cast<float>(columns) * 40.0f, 40.0f, depth - 0.01f, 1024, mode);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ItemEffectRender_Callback(void *) {
    std::int32_t columns = itemEffectColumns;
    if (itemEffectLabelTexture <= -1)
        return;
    itemEffectSprite.x = itemEffectPosition.x;
    itemEffectSprite.y = itemEffectPosition.y;
    PrepareDrawWithMode(itemEffectSprite, static_cast<ScreenMode>(0));
    itemEffectSprite.x += -33.0f;
    if (itemEffectAmount > 1)
        ++columns;
    if (itemEffectAmount >= 100)
        ++columns;
    DrawItemEffectWindow(columns, itemEffectSprite.x, itemEffectSprite.y - 25.0f, 0.3f);
    if (itemEffectLabelTexture != -1) {
        LegacySpritePacket packet{};
        packet.quad.flags = 1024;
        packet.colour = 0xcdcdcdcdU;
        packet.secondaryColour = 0xff000000U;
        packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
        const float x = ((static_cast<float>(20 * itemEffectColumns - 20) + itemEffectSprite.x) + 40.0f);
        TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(itemEffectSprite.y), 512,
            32, 1.0f, 1.0f, 0, static_cast<ScreenMode>(0));
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int32_t>(itemEffectLabelTexture)), 0.3f);
    }
    if (itemEffectAmount > 1)
        DrawLegacyItemQuantity(static_cast<float>(40 * columns) + itemEffectSprite.x, itemEffectSprite.y, itemEffectAmount, 0.4f);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CtrlItemEffectIcon_Callback(void *) {
    switch (itemEffectIconPhase) {
    case 0:
        break;
    case 1:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (!MoneyMessageTransitionBusy())
                itemEffectIconPhase = 3;
        } else if (AdvanceIconPosition(itemEffectPosition.x, itemEffectPosition.velocity, 50.0f))
            itemEffectIconPhase = 3;
        break;
    case 2:
        ShowElementByIndex(UIElement::GameMessage);
        itemEffectIconPhase = 1;
        itemEffectAudio = shenPlayAudioWithFlags("JINGURU01", 2);
        break;
    case 3:
        itemEffectIconFrames = static_cast<std::int8_t>(static_cast<std::uint8_t>(itemEffectIconFrames) + 1U);
        if (itemEffectIconFrames >= 75) {
            itemEffectIconPhase = 4;
            itemEffectIconFrames = 0;
            if (GetDebugForceUIEnabled(UIElement::GameMessage))
                DestroyElementByIndex(UIElement::GameMessage);
        }
        break;
    case 4:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (!MoneyMessageTransitionBusy())
                itemEffectIconPhase = 5;
        } else if (RetreatIconPosition(itemEffectPosition.x, itemEffectPosition.velocity, -672.0f))
            itemEffectIconPhase = 5;
        break;
    case 5:
        itemEffectPosition.x = -672.0f;
        itemEffectPosition.y = 55.0f;
        itemEffectIconPhase = 0;
        itemEffectLabelTexture = -1;
        ToggleRenderIcons(RenderIconSlot::ItemEffectMessage, false);
        iconToggleStateFor(5, false);
        if (itemEffectIconTask)
            HLib::SetupTimedTask(itemEffectIconTask, 0);
        break;
    default:
        itemEffectIconPhase = 5;
        break;
    }
    if (!GetItemSelectionMode(5))
        return;
    if (GetDebugForceUIEnabled(UIElement::GameMessage))
        EnableElement(UIElement::GameMessage);
    else
        HLib::ExecuteCallbackNow(ItemEffectRender_Callback, nullptr);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void PrepareSleepActionLabels() {
    dialogueSelectionPrompt.flagIds = {0xffff, 481, 482, 0xffff};
    if (sleepActionFlags & 1)
        dialogueSelectionPrompt.flagIds[0] = 484;
    if (sleepActionFlags & 0x100)
        dialogueSelectionPrompt.flagIds[3] = 483;
    numericActionSelector.mode = 0;
    dialogueSelectionPrompt.textAvailable = false;
    for (std::size_t slot = 0; slot < 4; ++slot) {
        const std::uint16_t label = dialogueSelectionPrompt.flagIds[slot];
        PublishActionSelectorText(slot, label == 0xffff ? std::string_view{} : GetActionSelectorLabel(label));
        dialogueSelectionPrompt.textAvailable |= !dialogueSelectionPrompt.labels[slot].empty();
    }
    dialogueSelectionPrompt.focusedSlot = -1;
    dialogueSelectionPrompt.active = true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ChooseSleepActionText(std::int32_t flags) {
    for (std::size_t slot = 0; slot < 4; ++slot) {
        LegacyIconSprite &sprite = sleepActionTextSprites[slot];
        if (sprite.icon != 0xffffU) {
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
            sprite.icon = 0xffffU;
        }
        if (!(flags & (1U << SLEEP_TEXT_FLAG_BITS[slot])))
            continue;
        sprite.icon = static_cast<std::uint16_t>(AcquireCtrlSpeakTextHandle(SLEEP_TEXT_NAMES[slot]));
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ReleaseSleepActionLabels() {
    for (LegacyIconSprite &sprite : sleepActionTextSprites) {
        if (sprite.icon != 0xffffU)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
        sprite.icon = 0xffffU;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint8_t GetSleepActionInput(std::uint16_t pressed) {
    if (!(sleepActionFlags & 1))
        pressed &= 0xefffU;
    if (!(sleepActionFlags & 2))
        pressed &= 0xbfffU;
    if (!(sleepActionFlags & 8))
        pressed &= 0x7fffU;
    if (!(sleepActionFlags & 0x100))
        pressed &= 0xddffU;
    return DecodeSelectorInput(pressed, 0x55);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void PrepareDrawWithUVOffsets(const LegacyIconSprite &sprite, float u, float v, float width, float height, std::uint32_t colour, ScreenMode mode) {
    const std::array<std::uint8_t, 4> uv = GetCommonIconRect(sprite.icon);
    const CommonIconAtlas *const atlas = GetCommonIconAtlas(sprite.icon);
    if (!atlas->textureHandle || atlas->textureHandle == 0xffffffffU)
        return;
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = colour | 0x00ffffffU, .secondaryColour = colour & 0x00ffffffU, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.uv = {static_cast<float>(static_cast<std::int8_t>(uv[0])) * 0.125f + u, static_cast<float>(static_cast<std::int8_t>(uv[1])) * 0.125f + v,
        static_cast<float>(static_cast<std::int8_t>(uv[2])) * 0.125f + width, static_cast<float>(static_cast<std::int8_t>(uv[3])) * 0.125f + height};
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, static_cast<std::uint16_t>(atlas->textureHandle), sprite.depth);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawActionSelectorTextFrame(const Vector2 &position, float depth, std::int32_t width, std::int16_t height, std::uint8_t selected) {
    ctrlSpeakSelectedFrameIds = ACTION_SELECTOR_FRAME_IDS.data() + 2 * (selected & 1);
    const std::int32_t edge = width <= 32 ? width / 2 : 16;
    const std::int32_t centre = width - 2 * edge;
    const float halfCentre = static_cast<float>(centre / 2), halfEdge = static_cast<float>(edge / 2);
    LegacyIconSprite sprite{.x = (position.x - halfCentre) - halfEdge,
        .y = position.y,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = depth,
        .flags = 0x400,
        .rotation = 0,
        .icon = ctrlSpeakSelectedFrameIds[1],
        .width = static_cast<std::int16_t>(edge),
        .height = height,
        .alignment = 5};
    PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
    sprite.flags = 0x500;
    sprite.x = (halfCentre + position.x) + halfEdge;
    PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
    sprite.icon = ctrlSpeakSelectedFrameIds[0];
    sprite.depth = depth - 0.05f;
    sprite.x = position.x;
    sprite.width = static_cast<std::int16_t>(centre);
    sprite.flags = 0x400;
    PrepareDrawWithUVOffsets(sprite, 0.0f, 0.0f, -0.005f, 0.0f, 0xff000000U, static_cast<ScreenMode>(0));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void DrawActionSelectorRadial(const Vector2 &position, const std::array<std::int32_t, 4> &handles, float depth, float scale,
    const std::array<std::int16_t, 4> &widths, std::int32_t selected, std::int32_t marker, std::int32_t forceText) {
    ctrlSpeakRadialFrameIds[1] = ACTION_SELECTOR_MARKERS[marker];
    const std::int16_t extent = static_cast<std::int16_t>(static_cast<std::int32_t>((scale + 0.5f) * 64.0f));
    LegacyIconSprite sprite{.x = position.x,
        .y = position.y,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = depth,
        .flags = 0,
        .rotation = 0,
        .icon = 1280,
        .width = extent,
        .height = extent,
        .alignment = 5};
    PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
    sprite.width = sprite.height = 40;
    sprite.depth = depth + 3.0f;
    for (std::size_t ring = 0; ring < 3; ++ring) {
        sprite.scaleX = ring == 1 ? cos_fixed16(ctrlSpeakPulseAngle) : 1.0f;
        sprite.icon = ctrlSpeakRadialFrameIds[ring];
        PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
        sprite.depth += -0.5f;
    }
    LegacyIconSprite &textSprite = ctrlSpeakChoiceDrawSprites[0];
    const float retainedScale = textSprite.scaleX;
    sprite.icon = 1285;
    sprite.scaleX = sprite.scaleY = 1.0f;
    sprite.width = sprite.height = 32;
    sprite.depth = depth;
    const std::int32_t inset = static_cast<std::int32_t>(scale * 3.0f);
    for (std::size_t slot = 0; slot < 4; ++slot) {
        textSprite.scaleX = textSprite.scaleY = scale;
        if (handles[slot] != -1) {
            const float x = ACTION_SELECTOR_RADIAL_X[slot], y = ACTION_SELECTOR_RADIAL_Y[slot];
            sprite.flags = 0;
            sprite.x = x * 16.0f + position.x;
            sprite.y = y * 16.0f + position.y;
            sprite.depth = depth + 0.2f;
            const float degrees = static_cast<float>(180 - 90 * static_cast<std::int32_t>(slot));
            sprite.rotation = static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
            PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
            const float halfHeight = (scale * 36.0f) * 0.5f, scaledWidth = static_cast<float>(widths[slot]) * scale;
            const float halfWidth = scaledWidth * 0.5f;
            const float radialY = (halfHeight + 24.0f) - static_cast<float>(inset);
            const float radialX = (halfWidth + 24.0f) - static_cast<float>(inset);
            const float textX = x * radialX + position.x, textY = radialY * y + position.y;
            DrawActionSelectorTextFrame({textX, textY}, depth + 0.1f, static_cast<std::int32_t>(scaledWidth),
                static_cast<std::int16_t>(static_cast<std::int32_t>(scale * 64.0f)), static_cast<std::int32_t>(slot) == selected ? 1 : 0);
            textSprite.x = textX;
            textSprite.y = textY;
            sprite.x = (((halfWidth + radialX) + 8.0f) - static_cast<float>(inset)) * x + position.x;
            sprite.y = (((halfHeight + radialY) + 8.0f) - static_cast<float>(inset)) * y + position.y;
            sprite.depth = depth + 0.2f;
            sprite.flags = 0x200;
            PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
            std::uint16_t glyph = 0;
            if (!forceText) {
                for (const std::array<std::int32_t, 2> &label : CTRL_SPEAK_SPECIAL_LABELS) {
                    if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) == label[0]) {
                        glyph = static_cast<std::uint16_t>(label[1]);
                        break;
                    }
                }
                if (glyph)
                    ctrlSpeakHasSpecialLabel = true;
            }
            if (forceText || ctrlSpeakForceTextLabels || !glyph) {
                textSprite.icon = static_cast<std::uint16_t>(handles[slot]);
                PrepareDrawTextureWithMode(textSprite, static_cast<ScreenMode>(0));
            } else {
                LegacyIconSprite special{.x = textSprite.x,
                    .y = textSprite.y,
                    .scaleX = textSprite.scaleX + textSprite.scaleX,
                    .scaleY = textSprite.scaleY * ctrlSpeakSpecialLabelWidthScale,
                    .depth = textSprite.depth + 0.2f,
                    .flags = 0,
                    .rotation = 0,
                    .icon = glyph,
                    .width = 32,
                    .height = 32,
                    .alignment = 5};
                PrepareDrawWithMode(special, static_cast<ScreenMode>(0));
            }
        }
        textSprite.scaleX = textSprite.scaleY = retainedScale;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void DrawCtrlSpeakSelector(void *) {
    const float offset = ctrlSpeakChoiceTextHandles[0] == -1 && ctrlSpeakChoiceTextHandles[2] != -1 ? -18.0f : 0.0f;
    const std::int32_t selected = ctrlSpeakSelectionAnimationFrame % 5 == 0 ? static_cast<std::int16_t>(ctrlSpeakSelectedChoiceSlot) : 4;
    DrawActionSelectorRadial({ctrlSpeakRightOriginX, ctrlSpeakSelectorOrigin[1] + offset}, ctrlSpeakChoiceTextHandles, 1.0f, ctrlSpeakRadialScale,
        ctrlSpeakChoiceWidths, selected, ctrlSpeakMarkerGlyphIndex, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawSleepActionSelector(void *) {
    if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
        EnableElement(UIElement::ActionSelector);
        ExecuteElementCallback(UIElement::ActionSelector);
        return;
    }
    std::array<std::int32_t, 4> handles;
    for (std::size_t slot = 0; slot < 4; ++slot)
        handles[slot] = (sleepActionFlags & (1U << SLEEP_TEXT_FLAG_BITS[slot])) ? static_cast<std::int16_t>(sleepActionTextSprites[slot].icon) : -1;
    const std::int32_t selected = sleepActionRetargetFrames % 5 == 0 ? sleepActionCurrentSelection : sleepActionNoSelection;
    DrawActionSelectorRadial(
        {sleepActionPosition[0], sleepActionPosition[1]}, handles, sleepActionPosition[2], sleepActionScale, SLEEP_LABEL_WIDTHS, selected, 2, 1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CtrlSleepActionIcon_Callback(void *) {
    if (sleepActionSubmenuState == 1 && GetDebugForceUIEnabled(UIElement::LoadSave) && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        g_inputBuffer[0].pressedPriority = 0;
        DestroyElementByIndex(UIElement::LoadSave);
        DestroyElementByIndex(UIElement::GameBackground);
        DestroyElementByIndex(UIElement::GameInfoBar);
    } else if (sleepActionSubmenuState == 2 && GetDebugForceUIEnabled(UIElement::Options) && (g_inputBuffer[0].pressedPriority & 0x200U) &&
        !AskKeepOrRevertChanges()) {
        sleepActionPhase &= 0xff00U;
        sleepActionStatus = static_cast<std::uint16_t>((sleepActionStatus & 0xff00U) | 1U);
        sleepActionSubmenuState = 0;
        g_inputBuffer[0].pressedPriority = 0;
        DestroyElementByIndex(UIElement::SubMenuOptions);
        DestroyElementByIndex(UIElement::GameBackground);
        DestroyElementByIndex(UIElement::GameInfoBar);
    }
    const std::uint16_t pressed = ctrlGetPadInputBuffer0(sleepActionPadControl)->pressedPriority;
    switch (static_cast<std::int8_t>(sleepActionPhase)) {
    case 0:
        if (!(sleepActionStatus & 0xffU))
            break;
        if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
            PrepareSleepActionLabels();
            ShowElementByIndex(UIElement::ActionSelector);
        }
        ResetHelpIconsToItemPrompt();
        CheckHelpIcon((static_cast<std::uint32_t>(sleepActionFlags) >> 7U & 2U) | 1U, 0, CheckHelpIconOp::Add);
        sleepActionPreviousPadControl = BindPadControlEntry(sleepActionPadControl);
        sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 1U);
        sleepActionTargetSelection = sleepActionNoSelection;
        iconToggleStateFor(6, true);
        ChooseSleepActionText(sleepActionFlags & 0x1b);
        break;
    case 1:
        sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 2U);
        break;
    case 2: {
        const std::uint8_t input = GetSleepActionInput(pressed);
        if (input == 0 || input == 2 || input == 6) {
            sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 3U);
            if (GetDebugForceUIEnabled(UIElement::ActionSelector))
                DestroyElementByIndex(UIElement::ActionSelector);
            sleepActionRetargetFrames = -1;
            sleepActionCurrentSelection = sleepActionTargetSelection = ACTION_SELECTOR_DIRECTION_TO_SLOT[input];
            sleepActionArrowSprites[sleepActionCurrentSelection].depth = 5.0f;
            sleepActionTextSprites[sleepActionCurrentSelection].depth = 6.0f;
        } else if (input == 4 || input == 9) {
            if (input == 4) {
                sleepActionRetargetFrames = -1;
                sleepActionCurrentSelection = ACTION_SELECTOR_DIRECTION_TO_SLOT[input];
                sleepActionArrowSprites[sleepActionCurrentSelection].depth = 5.0f;
                sleepActionTextSprites[sleepActionCurrentSelection].depth = 6.0f;
            }
            sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 3U);
            SelectActionSelectorButton(3);
            if (GetDebugForceUIEnabled(UIElement::ActionSelector))
                DestroyElementByIndex(UIElement::ActionSelector);
        }
        break;
    }
    case 3:
        if (sleepActionCurrentSelection == sleepActionNoSelection && !(ActionSelectorPosition() > 0.0f)) {
            ResetHelpIconsToItemPrompt();
            sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | (sleepActionTargetSelection == 4 ? 6U : 4U));
        }
        break;
    case 4:
        sleepActionPhase &= 0xff00U;
        ToggleRenderIcons(RenderIconSlot::SleepAction, false);
        ReleaseSleepActionLabels();
        BindPadControlEntry(sleepActionPreviousPadControl);
        iconToggleStateFor(6, false);
        sleepActionPosition[0] = -176.0f;
        sleepActionStatus &= 0xff00U;
        switch (sleepActionTargetSelection) {
        case 0:
            SelectActionSelectorButton(0);
            sleepActionStatus = static_cast<std::uint16_t>((sleepActionStatus & 0x00ffU) | 0x0200U);
            ResetHelpIconsToItemPrompt();
            sleepActionPublishedSelection = 0;
            break;
        case 1:
            SelectActionSelectorButton(1);
            EnableSaveScreenAdmission();
            sleepActionSubmenuState = 1;
            ShowElementByIndex(UIElement::GameBackground);
            SetSaveFrontendMode(1);
            ShowElementByIndex(UIElement::LoadSave);
            ShowElementByIndex(UIElement::GameInfoBar);
            break;
        case 2:
            SelectActionSelectorButton(3);
            break;
        case 3:
            SelectActionSelectorButton(2);
            options.cameraRequested = true;
            ShowElementByIndex(UIElement::GameBackground);
            ShowElementByIndex(UIElement::SubMenuOptions);
            ShowElementByIndex(UIElement::GameInfoBar);
            sleepActionSubmenuState = 2;
            break;
        default:
            break;
        }
        break;
    case 6:
        sleepActionPhase &= 0xff00U;
        sleepActionScale = 0.0f;
        ToggleRenderIcons(RenderIconSlot::SleepAction, false);
        iconToggleStateFor(6, false);
        ReleaseSleepActionLabels();
        BindPadControlEntry(sleepActionPreviousPadControl);
        sleepActionStatus = 0x0100U;
        sleepActionPublishedSelection = sleepActionNoSelection;
        break;
    default:
        break;
    }
    if (GetItemSelectionMode(6)) {
        if (sleepActionCurrentSelection != sleepActionNoSelection) {
            sleepActionRetargetFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(sleepActionRetargetFrames) + 1U);
            if (sleepActionRetargetFrames >= 20) {
                sleepActionCurrentSelection = sleepActionNoSelection;
                sleepActionRetargetFrames = 20;
            }
        }
        HLib::ExecuteCallbackNow(DrawSleepActionSelector, nullptr);
    }
}
#endif

} // namespace ui_detail
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour, ScreenMode mode) {
    const float advance = static_cast<float>(static_cast<std::int16_t>(TruncateSpriteCoordinate(scale * 16.0f)));
    float currentX = x - 7.0f * advance;
    bool significant = false;
    for (std::int32_t index = 0; index < 8; ++index) {
        LegacyIconSprite &sprite = selectorNumberSprites[index];
        sprite.x = currentX;
        sprite.y = y;
        const std::int32_t digit = amount / moneyIconIncrements[index];
        amount %= moneyIconIncrements[index];
        sprite.depth = depth;
        sprite.scaleX = sprite.scaleY = scale;
        currentX += advance;
        if (digit != 0)
            significant = true;
        sprite.icon = SELECTOR_DIGIT_ICONS[digit];
        if (significant || index == 7)
            PrepareDrawSimple(sprite, colour, mode);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMoneyAmount(float x, float y, float depth, float scale, std::int32_t amount, std::uint8_t cash, std::uint32_t colour, ScreenMode mode) {
    const float work[4]{x, y, depth, scale};
    DrawSelectorCash(work, amount, cash, colour, 0, mode);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
static void WriteGuiDds(std::byte *buffer, std::uint32_t width, std::uint32_t height, std::uint32_t pixelBytes, std::uint8_t format) {
    std::array<std::uint32_t, 32> dds{};
    dds[0] = make_shen_name('D', 'D', 'S', ' ');
    dds[1] = 124;
    dds[2] = 0x1007;
    dds[3] = height;
    dds[4] = width;
    dds[27] = 0x1000;
    const std::array<std::uint32_t, 8> *pixelFormat = nullptr;
    switch (format) {
    case 1:
        pixelFormat = &DDS_PIXEL_RGB565;
        dds[2] = 0x100f;
        dds[5] = 2 * width;
        break;
    case 2:
    case 133:
        pixelFormat = &DDS_PIXEL_ARGB4444;
        dds[2] = 0x100f;
        dds[5] = 2 * width;
        break;
    case 128:
        pixelFormat = &DDS_PIXEL_DXT1;
        dds[2] = 0x81007;
        dds[5] = pixelBytes;
        break;
    case 129:
        pixelFormat = &DDS_PIXEL_DXT3;
        dds[2] = 0x81007;
        dds[5] = pixelBytes;
        break;
    case 130:
        pixelFormat = &DDS_PIXEL_DXT5;
        dds[2] = 0x81007;
        dds[5] = pixelBytes;
        break;
    case 131:
    case 137:
        pixelFormat = &DDS_PIXEL_BGRA8;
        dds[2] = 0x100f;
        dds[5] = 4 * width;
        break;
    default:
        break;
    }
    if (pixelFormat)
        std::memcpy(dds.data() + 19, pixelFormat->data(), 32);
    std::memcpy(buffer, dds.data(), sizeof(dds));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ParseGuiPtx(std::byte *buffer, std::uint16_t width, std::uint16_t height, std::uint32_t idLow, std::uint32_t idHigh, std::uint16_t format) {
    const std::uint32_t storageBytes = GetTextureStorageSize(format, width, height);
    const std::uint32_t pixelBytes = (format & 0xff00U) == 0x8000U ? storageBytes - 128U : storageBytes;
    const GuiPtxHeader header{.tag = SHEN_NAME(TEXN), .size = storageBytes + 48U, .idLow = idLow, .idHigh = idHigh,
                                  .gbixTag = SHEN_NAME(GBIX), .gbixSize = 4,
                                      .pvrTag = SHEN_NAME(PVRT), .pvrSize = pixelBytes + 8, .format = format, .width = width, .height = height};
    std::memcpy(buffer, &header, sizeof(header));
    if ((format & 0xff00U) == 0x8000U)
        WriteGuiDds(buffer + sizeof(header), width, height, pixelBytes, static_cast<std::uint8_t>(format));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
float GetScreenAlignmentOffset() noexcept {
    const float aspect = static_cast<float>(d3t::g_originalScreenWidth) / static_cast<float>(d3t::g_originalScreenHeight);
    return ((1.0f - (1.3333334f / aspect)) * 640.0f) * 0.5f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitializeLegacyIconSprite(LegacyIconSprite &sprite) noexcept {
    sprite.x = 0.0f;
    sprite.y = 0.0f;
    sprite.scaleX = 1.0f;
    sprite.scaleY = 1.0f;
    sprite.depth = 1.0f;
    sprite.flags = 0;
    sprite.rotation = 0;
    sprite.icon = 0;
    sprite.width = 32;
    sprite.height = 32;
    sprite.alignment = 5;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint8_t DecodeSelectorInput(std::uint16_t pressed, std::uint8_t allowedDirections) noexcept {
    const std::uint8_t direction = selectorDirectionIcons[pressed >> 12U];
    for (std::uint8_t index = 0; index < 12; ++index) {
        if ((allowedDirections & 1U) != 0 && direction == index)
            return direction;
        allowedDirections >>= 1U;
    }
    if ((pressed & 0x0200U) != 0)
        return 9;
    if ((pressed & 0x0400U) != 0)
        return 8;
    if ((pressed & 0x0002U) != 0)
        return 11;
    return (pressed & 0x0004U) != 0 ? 10 : 13;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool AdvanceIconPosition(float &position, float &velocity, float target) noexcept {
    float distance = target - position;
    if (distance < 0.0f)
        distance = -distance;
    distance *= 0.5f;
    velocity = distance <= 64.0f ? distance : velocity + velocity;
    if (velocity >= 1.0f) {
        if (velocity > 64.0f)
            velocity = 64.0f;
    } else
        velocity = 1.0f;
    position += velocity;
    if (position < target)
        return false;
    position = target;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool RetreatIconPosition(float &position, float &velocity, float target) noexcept {
    float distance = target - position;
    if (distance < 0.0f)
        distance = -distance;
    distance *= 0.5f;
    velocity = distance <= 64.0f ? distance : velocity + velocity;
    if (velocity >= 1.0f) {
        if (velocity > 64.0f)
            velocity = 64.0f;
    } else
        velocity = 1.0f;
    position -= velocity;
    if (target < position)
        return false;
    position = target;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t GetTextureStorageSize(std::uint16_t format, std::int32_t width, std::int32_t height) noexcept {
    const std::uint32_t pixels = static_cast<std::uint32_t>(width) * static_cast<std::uint32_t>(height);
    switch (format & 0xff00U) {
    case 0x0100:
    case 0x0900:
        return 2U * pixels;
    case 0x0700:
        return pixels;
    case 0x0500:
        return static_cast<std::uint32_t>(std::bit_cast<std::int32_t>(pixels) / 2);
    case 0x0300:
        if (width != height)
            return 0;
        switch (width) {
        case 64:
            return 0x0c00;
        case 128:
            return 0x1800;
        case 256:
            return 0x4800;
        case 512:
            return 0x10800;
        default:
            return 0;
        }
    case 0x8000:
        break;
    default:
        return 0;
    }
    const std::uint32_t blocksX = static_cast<std::uint32_t>(width / 4 < 1 ? 1 : width / 4);
    const std::uint32_t blocksY = static_cast<std::uint32_t>(height / 4 < 1 ? 1 : height / 4);
    switch (format & 0xffU) {
    case 1:
    case 2:
    case 133:
        return 128U + 2U * pixels;
    case 128:
        return 128U + 8U * blocksX * blocksY;
    case 129:
    case 130:
        return 16U * (blocksX * blocksY + 8U);
    case 137:
        return 128U + 4U * pixels;
    default:
        return 128U;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// helpWidgetResetRuns.
void HelpWidgetResetRuns() {
    helpWidgetRunCount = 0;
    helpWidgetLineCount = 0;
    helpWidgetBoxesLocked = false;
    helpWidgetIconRunCount = 0;
    helpWidgetBoxCount = 0;
    helpWidgetIconCount = 0;
}
#endif

namespace ui_detail {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void HelpWidgetCommitText(const std::array<char, 256> &pending, std::int32_t length, std::int32_t column) {
    HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(helpWidgetRunCount++)];
    for (std::size_t index = 0; index < run.text.size(); ++index)
        run.text[index] = index < static_cast<std::size_t>(length) ? pending[index] : 0;
    run.length = length;
    run.column = column;
    run.line = helpWidgetLineCount;
    run.kind = 1;
}
#endif

} // namespace ui_detail
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// helpWidgetParseLine: splits one help line into text runs at "\t" column tabs and
// button-icon runs at '^'; leading spaces of a run are dropped.
void HelpWidgetParseLine(std::string_view line) {
    const std::size_t terminator = line.find('\0');
    if (terminator != std::string_view::npos)
        line = line.substr(0, terminator);
    std::array<char, 256> pending{};
    std::int32_t pendingLength = 0;
    std::int32_t column = 0;
    std::size_t offset = 0;
    while (offset < line.size()) {
        std::uint32_t codepoint = 0;
        const std::size_t length = DecodeHelpCodepoint(line.substr(offset), codepoint);
        if (!length)
            break;
        if (codepoint == '\\' && offset + 1 < line.size() && line[offset + 1] == 't') {
            if (pendingLength > 0 && helpWidgetRunCount < 64)
                HelpWidgetCommitText(pending, pendingLength, column);
            pendingLength = 0;
            ++column;
            offset += 2;
            continue;
        }
        if (codepoint == '^') {
            if (pendingLength > 0 && helpWidgetRunCount < 64)
                HelpWidgetCommitText(pending, pendingLength, column);
            const std::int32_t icon = helpWidgetIconRunCount++;
            if (icon != -1 && helpWidgetRunCount < 64) {
                HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(helpWidgetRunCount++)];
                run.column = column;
                run.line = helpWidgetLineCount;
                run.icon = icon;
                run.kind = 2;
            }
            pendingLength = 0;
            ++offset;
            continue;
        }
        if (pendingLength || codepoint != ' ') {
            std::memcpy(pending.data() + pendingLength, line.data() + offset, length);
            pendingLength += static_cast<std::int32_t>(length);
        }
        offset += length;
    }
    if (pendingLength > 0 && helpWidgetRunCount < 64)
        HelpWidgetCommitText(pending, pendingLength, column);
    ++helpWidgetLineCount;
    helpWidgetRelayout = true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// helpWidgetAddButtonIcon.
void HelpWidgetAddButtonIcon(std::int32_t code) {
    HelpWidgetMakeButtonIcon(helpWidgetIcons[static_cast<std::size_t>(helpWidgetIconCount)], code);
    helpWidgetIconActions[static_cast<std::size_t>(helpWidgetIconCount++)] = UI_GetActionForId(code);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// FormatColoursForHelpTopic: the coloured boxes behind topic columns, as
// {colour, column, row, rows, width mode}.
void FormatColoursForHelpTopic(std::uint8_t topic) {
    using Box = HelpWidgetBoxDefinition;
    constexpr std::uint32_t red = HelpColour(140, 0, 0, 255);
    constexpr std::uint32_t green = HelpColour(0, 140, 0, 255);
    constexpr std::uint32_t blue = HelpColour(0, 0, 140, 255);
    constexpr std::uint32_t yellow = HelpColour(140, 140, 0, 255);
    auto add = [](std::initializer_list<Box> boxes) {
        for (const Box &box : boxes) {
            if (helpWidgetBoxesLocked)
                return;
            HelpWidgetBox &target = helpWidgetBoxes[static_cast<std::size_t>(helpWidgetBoxCount++)];
            target.colour = box.colour;
            target.column = box.column;
            target.row = box.row;
            target.rows = box.rows;
            target.widthMode = box.widthMode;
        }
    };
    switch (topic) {
    case 0x00:
    case 0x1e:
        add({{red, 0, 3, 3, 1}, {green, 1, 3, 3, 3}, {blue, 0, 7, 4, 1}, {yellow, 1, 7, 4, 3}});
        break;
    case 0x26:
    case 0x2b:
        add({{red, 0, 3, 3, 1}, {green, 1, 3, 3, 3}, {blue, 0, 7, 4, 1}, {yellow, 1, 7, 3, 3}});
        break;
    case 0x01:
        add({{green, 0, 7, 1, 1}, {blue, 1, 7, 2, 3}});
        break;
    case 0x02:
        add({{green, 0, 7, 2, 1}, {blue, 1, 7, 2, 3}});
        break;
    case 0x03:
    case 0x37:
        add({{green, 0, 9, 1, 1}});
        break;
    case 0x04:
    case 0x72:
        add({{green, 1, 6, 1, 0}});
        break;
    case 0x06:
        add({{green, 0, 7, 1, 1}, {blue, 1, 7, 4, 3}});
        break;
    case 0x07:
        add({{red, 0, 8, 1, 1}, {blue, 1, 8, 1, 3}, {green, 1, 10, 1, 3}});
        break;
    case 0x0a:
        add({{green, 1, 5, 3, 0}});
        break;
    case 0x0b:
    case 0x3d:
        add({{red, 0, 7, 1, 1}, {green, 1, 7, 2, 3}});
        break;
    case 0x0c:
        add({{red, 0, 3, 1, 1}, {green, 1, 3, 1, 3}});
        break;
    case 0x0e:
        add({{red, 0, 7, 1, 1}, {green, 1, 7, 4, 3}});
        break;
    case 0x0f:
        add({{blue, 0, 8, 2, 1}});
        break;
    case 0x10:
        add({{green, 1, 6, 2, 0}});
        break;
    case 0x11:
    case 0x4f:
    case 0x57:
        add({{red, 0, 7, 1, 1}, {green, 1, 7, 1, 3}});
        break;
    case 0x12:
    case 0x53:
        add({{blue, 0, 7, 3, 1}, {green, 1, 7, 1, 3}});
        break;
    case 0x13:
        add({{green, 1, 6, 4, 0}});
        break;
    case 0x14:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7c:
        add({{green, 1, 6, 3, 0}});
        break;
    case 0x16:
        add({{red, 1, 8, 3, 0}});
        break;
    case 0x17:
    case 0x35:
        add({{red, 1, 8, 2, 0}});
        break;
    case 0x18:
        add({{yellow, 1, 8, 1, 3}, {green, 0, 8, 1, 1}});
        break;
    case 0x19:
    case 0x4e:
        add({{red, 0, 6, 1, 1}, {green, 1, 6, 2, 3}});
        break;
    case 0x1b:
    case 0x23:
    case 0x42:
    case 0x70:
    case 0x73:
        add({{green, 1, 7, 1, 0}});
        break;
    case 0x1c:
        add({{green, 1, 9, 2, 3}, {red, 0, 9, 2, 1}});
        break;
    case 0x24:
    case 0x32:
        add({{red, 0, 7, 1, 1}});
        break;
    case 0x27:
        add({{green, 0, 8, 1, 1}});
        break;
    case 0x2a:
        add({{green, 1, 5, 2, 0}});
        break;
    case 0x2f:
    case 0x4c:
        add({{green, 1, 7, 4, 0}});
        break;
    case 0x30:
        add({{green, 0, 10, 1, 1}});
        break;
    case 0x31:
    case 0x55:
        add({{red, 0, 8, 1, 1}, {green, 1, 8, 2, 3}});
        break;
    case 0x33:
        add({{red, 0, 5, 2, 1}, {blue, 1, 5, 2, 3}});
        break;
    case 0x34:
        add({{red, 1, 8, 2, 2}});
        break;
    case 0x3e:
        add({{green, 1, 8, 2, 0}});
        break;
    case 0x43:
        add({{red, 0, 6, 2, 1}, {green, 1, 6, 1, 3}});
        break;
    case 0x50:
        add({{red, 0, 7, 2, 1}, {green, 1, 7, 1, 3}});
        break;
    case 0x51:
        add({{red, 1, 8, 1, 0}});
        break;
    case 0x59:
    case 0x5d:
    case 0x5f:
    case 0x61:
    case 0x63:
    case 0x64:
    case 0x6d:
    case 0x7d:
    case 0x7e:
    case 0x83:
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
    case 0x90:
        add({{green, 1, 7, 3, 0}});
        break;
    case 0x5a:
        add({{green, 1, 8, 1, 0}});
        break;
    case 0x5b:
    case 0x66:
    case 0x67:
    case 0x6a:
    case 0x7f:
    case 0x80:
    case 0x81:
    case 0x84:
    case 0x85:
    case 0x8f:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
        add({{green, 1, 8, 3, 0}});
        break;
    case 0x86:
        add({{red, 1, 7, 3, 0}});
        break;
    case 0x87:
    case 0x89:
        add({{red, 1, 7, 2, 0}});
        break;
    case 0x88:
        add({{red, 1, 7, 2, 2}});
        break;
    case 0x94:
        add({{green, 1, 9, 2, 0}});
        break;
    case 0x99:
        add({{red, 0, 8, 1, 1}, {green, 1, 8, 1, 3}});
        break;
    case 0x9a:
        add({{red, 1, 6, 2, 0}, {green, 1, 9, 2, 0}});
        break;
    default:
        break;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void SetHelpIconState() noexcept {
    g_helpIconState = 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t IsCameraIconHidden() noexcept {
    return currentCameraIconMode;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native SetDebugHideCameraIcon: CTRL construction and the Game Menu pass 1, the main-menu wheel 0; SetupCameraIcon
// falls back to it while the debug default requests no mode (2).
void SetDebugHideCameraIcon(std::int32_t hidden) noexcept {
    previousCameraIconMode = hidden;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetupCameraIcon() noexcept {
    std::int32_t resolvedMode = previousCameraIconMode;
    if (requestedCameraIconMode != 2)
        resolvedMode = requestedCameraIconMode;
    previousCameraIconMode = resolvedMode;
    if (resolvedMode == currentCameraIconMode)
        return;

    currentCameraIconMode = resolvedMode;
    for (UIElementState &element : g_uiElements) {
        if ((element.setup == 0 || element.config.init == 3) && element.unknown88) {
            element.unknown88();
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int32_t GetCurrentUIInputType() noexcept {
    constexpr std::uint32_t kCameraIconRange = 0x03ff0000U;
    constexpr std::uint32_t kAlwaysBlockedWord0 = 0x2c000020U;
    constexpr std::uint32_t kAlwaysBlockedWord1 = 0x30000000U;
    constexpr std::uint32_t kMenuWord0 = 0x0000fe40U;
    constexpr std::uint32_t kMenuWord1 = 0x0003f000U;
    constexpr std::uint32_t kCameraBlockedWord1 = 0x06000000U;
    constexpr std::uint32_t kCameraMenuWord1 = 0x06000080U;
    const std::array<std::uint32_t, 3> &input = g_uiPreviousInputElements;
    if ((input[0] & kAlwaysBlockedWord0) || (input[1] & kAlwaysBlockedWord1) ||
        (IsCameraIconHidden() == 0 && ((input[0] & kCameraIconRange) || (input[1] & kCameraBlockedWord1))))
        return static_cast<std::int32_t>(InputType::Menu);
    if ((input[0] & kMenuWord0) || (input[1] & kMenuWord1) || (IsCameraIconHidden() == 1 && ((input[0] & kCameraIconRange) || (input[1] & kCameraMenuWord1))))
        return static_cast<std::int32_t>(InputType::GameUi);
    constexpr std::uint32_t kHelpOverlay = 1U << 16U;
    if ((input[2] & kHelpOverlay) && IsGameUIActive())
        return static_cast<std::int32_t>(InputType::GameUi);
    constexpr std::uint32_t kQte2 = 1U << 22U;
    if ((input[1] & kQte2) || (g_uiRefreshedElements[1] & kQte2) || GetIsQuickIconShowing())
        return static_cast<std::int32_t>((input[2] & kHelpOverlay) ? InputType::Game : InputType::Qte2);
    constexpr std::uint32_t kQte = 1U << 3U;
    if ((input[2] & kQte) || (g_uiRefreshedElements[2] & kQte) || areas::GetArmwGameplayActive() == 1)
        return static_cast<std::int32_t>(InputType::Qte);
    constexpr std::uint32_t kFightRoot = 1U << 2U;
    if ((input[0] & kFightRoot) && (IsFightGaugeInputActive() || IsFightGaugeOverlayInputActive()))
        return static_cast<std::int32_t>(InputType::Fight);
    if (g_uiElements[static_cast<std::size_t>(UIElement::FightGauge)].isVisible && (IsFightGaugeInputActive() || IsFightGaugeOverlayInputActive()))
        return static_cast<std::int32_t>(InputType::Fight);
    if ((g_uiRefreshedElements[0] & kFightRoot) && (IsFightGaugeInputActive() || IsFightGaugeOverlayInputActive()))
        return static_cast<std::int32_t>(InputType::Fight);
    constexpr std::uint32_t kFightOverlay = 1U << 3U;
    if ((input[1] & kFightOverlay) || IsHudFightSpriteActive() == 1)
        return static_cast<std::int32_t>(InputType::Fight);
    return gameMenuActive != 0 && g_gameArcadeRunning == 0 && !GetIsGameHangOnHandleValid() ? 0 : 2;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool DrawLegacyUISpritePrimitive(const LegacyUISpritePrimitive &primitive) {
    if (!primitive.texture || primitive.width <= 0.0f || primitive.height <= 0.0f) {
        return false;
    }
    const float sx = static_cast<float>(d3t::g_originalScreenWidth) / 640.0f;
    const float sy = static_cast<float>(d3t::g_originalScreenHeight) / 480.0f;
    const float left = (primitive.centerX - primitive.width * 0.5f) * sx;
    const float right = (primitive.centerX + primitive.width * 0.5f) * sx;
    const float top = (primitive.centerY - primitive.height * 0.5f) * sy;
    const float bottom = (primitive.centerY + primitive.height * 0.5f) * sy;
    const std::array<d3t::UITexturedVertex, 4> vertices{{
        {left, top, primitive.u0, primitive.v0, primitive.colour},
        {right, top, primitive.u1, primitive.v0, primitive.colour},
        {right, bottom, primitive.u1, primitive.v1, primitive.colour},
        {left, bottom, primitive.u0, primitive.v1, primitive.colour},
    }};
    constexpr std::array<std::uint16_t, 6> indices{0, 1, 2, 0, 2, 3};
    const std::array<std::int32_t, 4> scissor{
        0, 0, static_cast<std::int32_t>(d3t::g_originalScreenWidth), static_cast<std::int32_t>(d3t::g_originalScreenHeight)};
    return d3t::DrawUITexturedGeometry(primitive.texture, vertices, indices, scissor);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool DrawLegacyUIColourPrimitive(float centerX, float centerY, float width, float height, std::uint32_t colour) {
    return DrawLegacyUISpritePrimitive({d3t::g_defaultWhiteTexture, centerX, centerY, width, height, 0.0f, 0.0f, 1.0f, 1.0f, colour});
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t LoadCommonIconSprites(PakGlobalData *package) {
    // The common bank survives InitEffects; native reloads reset the published
    // indices, not the persistent bank's acquired references.
    for (CommonIconAtlas &atlas : commonIconAtlases) {
        if (atlas.textureHandle && atlas.textureHandle != ResolveNamedTextureHandle(atlas.textureIndex))
            d3t::InitTexture2DResource(atlas.textureHandle);
        atlas = {};
    }
    commonIconAtlasCount = 0;
    constexpr shen_name binExtension{make_shen_name('B', 'I', 'N', ' ')};
    std::span<const std::byte> bytes = FileLoaderView(package, "COMICON.SPR", binExtension);
    while (bytes.size() >= 8 && commonIconAtlasCount < commonIconAtlases.size()) {
        std::uint32_t tag{}, size{};
        std::memcpy(&tag, bytes.data(), sizeof(tag));
        std::memcpy(&size, bytes.data() + 4, sizeof(size));
        if (!tag || size < 8 || size > bytes.size())
            break;
        CommonIconAtlas &atlas = commonIconAtlases[commonIconAtlasCount++];
        atlas.textureIndex = static_cast<std::int32_t>(ReadTexnTexture(bytes.data()));
        atlas.textureHandle = ResolveNamedTextureHandle(atlas.textureIndex);
        bytes = bytes.subspan(size);
    }
    return commonIconAtlasCount;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t LoadLoadScreenSpriteTexture(PakGlobalData *package) {
    constexpr shen_name binExtension{make_shen_name('B', 'I', 'N', ' ')};
    const std::span<const std::byte> bytes = FileLoaderView(package, "LD_nl.SPR", binExtension);
    if (bytes.empty())
        return 0;
    const std::int64_t result = ReadTexnTexture(bytes.data());
    g_loadScreenSpriteTexture = static_cast<std::int32_t>(result);
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ReleaseLoadScreenSpriteTexture() {
    SetNamedTextureBank(2);
    if (g_loadScreenSpriteTexture != -1)
        ReleaseNamedTexture(g_loadScreenSpriteTexture);
    g_loadScreenSpriteTexture = -1;
    SetNamedTextureBank(0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ReleaseCommonIconSprites() {
    SetNamedTextureBank(2);
    for (CommonIconAtlas &atlas : commonIconAtlases) {
        if (atlas.textureHandle && atlas.textureHandle != ResolveNamedTextureHandle(atlas.textureIndex))
            d3t::InitTexture2DResource(atlas.textureHandle);
        if (atlas.textureIndex != -1)
            ReleaseNamedTexture(atlas.textureIndex);
        atlas = {};
    }
    commonIconAtlasCount = 0;
    SetNamedTextureBank(0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
CommonIconAtlas *GetCommonIconAtlas(std::uint16_t icon) {
    const std::size_t index = icon >> 8U;
    return &commonIconAtlases[index < commonIconAtlases.size() ? index : 0];
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::array<std::uint8_t, 4> GetCommonIconRect(std::uint16_t icon) {
    const std::uint32_t bank = icon >> 8U;
    // FindCommonIconRectangle treats the low byte as a bank-local authored index.
    // A high byte outside the native table selects the first bank's base.
    const std::size_t offset = bank < commonIconRectangleOffsets.size() ? commonIconRectangleOffsets[bank] + (icon & 0xffU) * 4U : 0x140U;
    return {commonIconRectangles[offset], commonIconRectangles[offset + 1], commonIconRectangles[offset + 2], commonIconRectangles[offset + 3]};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool ReplaceCommonIconAtlasImage(std::uint16_t icon, std::string_view path) {
    CommonIconAtlas &atlas = *GetCommonIconAtlas(icon);
    if (!atlas.textureHandle || path.empty())
        return false;
    const std::string source(path);
    const std::string filename(path.substr(0, path.find('?')));
    if (!ArchiveData_GetFileSize(filename.c_str()))
        return false;
    std::uint32_t replacement = 0;
    d3t::LoadImageTexture(source.c_str(), false, &replacement, 0);
    if (!replacement)
        return false;
    if (atlas.textureHandle != ResolveNamedTextureHandle(atlas.textureIndex))
        d3t::InitTexture2DResource(atlas.textureHandle);
    atlas.textureHandle = replacement;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool GetShouldRenderIcons(RenderIconSlot slot) {
    if (slot == RenderIconSlot::Any)
        return renderIconFlags != 0;
    const auto index = static_cast<std::uint32_t>(slot);
    return index < static_cast<std::uint32_t>(RenderIconSlot::Count) && (renderIconFlags & (1U << index)) != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool ToggleRenderIcons(RenderIconSlot slot, bool enabled) {
    const auto index = static_cast<std::uint32_t>(slot);
    if (index >= static_cast<std::uint32_t>(RenderIconSlot::Count))
        return false;
    const bool previous = GetShouldRenderIcons(slot);
    if (enabled)
        renderIconFlags |= 1U << index;
    else
        renderIconFlags &= ~(1U << index);
    renderIconFlags &= ~1U;
    return previous;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetShowQteFlag(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint16_t buttonFlags) noexcept {
    quickIconFlags = buttonFlags;
    quickIconMaximumFrames = maximumFrames == 0x7fff ? -1.0f : (maximumFrames > 0 ? static_cast<float>(maximumFrames) : 1.0f);
    quickIconElapsedFrames = 0.0f;
    quickIconPhase = 2;
    ToggleRenderIcons(RenderIconSlot::Qte, true);
    quickIconCurrentStatus = -1;
    if ((buttonFlags & 0x20U) == 0) {
        std::uint16_t normalized = 1;
        std::uint8_t bit = 0;
        while (bit < 12 && (requiredStatus & normalized) == 0) {
            normalized = static_cast<std::uint16_t>(normalized << 1U);
            ++bit;
        }
        requiredStatus = normalized;
    }
    quickIconRequiredStatus = requiredStatus;
    quickIconUnpressed = true;
    quickIconDrawFrames = 0.0f;
    quickIconPreviousDrawFrames = 0.0f;
    std::uint8_t bit = 0;
    if (static_cast<std::uint8_t>(requiredStatus)) {
        quickIconSprite.icon = 0x800;
        while (bit < 7 && !(requiredStatus & (1U << bit)))
            ++bit;
        quickIconSprite.rotation = static_cast<std::uint32_t>(bit) << 13U;
        quickIconSprite.scaleY = (bit & 0xfbU) == 0 ? 0.9f : 1.0f;
    } else {
        const std::uint8_t buttons = static_cast<std::uint8_t>(requiredStatus >> 8U);
        while (bit < 4 && !(buttons & (1U << bit)))
            ++bit;
        quickIconSprite.rotation = 0;
        quickIconSprite.icon = QUICK_ICON_FACE_ICONS[bit];
        quickIconSprite.scaleY = 1.0f;
    }
    ResetBattleQteTransition();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetShowQteFlagForButtonType(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint32_t buttonType) noexcept {
    if (buttonType >= QUICK_ICON_BUTTON_TYPES.size())
        buttonType = 0;
    SetShowQteFlag(requiredStatus, maximumFrames, QUICK_ICON_BUTTON_TYPES[buttonType]);
    quickIconButtonMask = 0xffffU;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetCurrentQteStatus() noexcept {
    const std::uint16_t status = static_cast<std::uint16_t>(quickIconCurrentStatus);
    quickIconCurrentStatus = -1;
    return status;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool GetIsQuickIconShowing() noexcept {
    return quickIconPhase != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool ResetQuickIconState(bool enabled) noexcept {
    if (!GetShouldRenderIcons(RenderIconSlot::Qte) || !enabled)
        return false;
    quickIconButtonMask = 0;
    quickIconPhase = 0;
    iconToggleStateFor(3, false);
    quickIconTerminalStatus = -1;
    quickIconPosition[0] = -176.0f;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetQteButtonState(std::uint16_t mask) noexcept {
    quickIconButtonMask = mask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetQteMinigameModel(MT7Model *model) noexcept {
    qteMinigame.model = model;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetQteMinigameTitleButtons(const std::uint32_t *buttons) noexcept {
    qteMinigame.titleButtons = buttons;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint8_t GetParentMenu() {
    // Native returns a low-byte UI_Elements value.
    if ((renderIconFlags & 2U) != 0)
        return 0;
    if ((renderIconFlags & 8U) != 0)
        return 2;
    if ((renderIconFlags & 4U) != 0)
        return 4;
    return (renderIconFlags & 32U) != 0 ? 6 : 7;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint8_t SetPersistentHelpIconState(std::uint32_t visible, std::uint32_t selected, std::int32_t merge) {
    HelpIconState &help = helpIconState;
    if (merge) {
        visible |= help.persistentVisible;
        selected |= help.persistentSelected;
    }
    help.persistentVisible = visible;
    help.persistentSelected = selected & visible;
    help.frameOverlaySelected &= help.frameOverlayVisible;
    return ConfigureHelpDirectionBlink(visible);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CheckHelpIcon(std::uint32_t visible, std::uint32_t selected, CheckHelpIconOp operation) {
    HelpIconState &help = helpIconState;
    switch (operation) {
    case CheckHelpIconOp::Set:
        SetPersistentHelpIconState(visible, selected, 0);
        return;
    case CheckHelpIconOp::Add:
        SetPersistentHelpIconState(visible, selected, 1);
        return;
    case CheckHelpIconOp::Remove: {
        const std::uint32_t blink = selected & visible & help.persistentVisible & help.persistentSelected;
        helpIconDeferredRemoval |= blink & help.persistentSelected;
        help.persistentVisible &= ~(visible & ~blink);
        break;
    }
    case CheckHelpIconOp::FrameOverlay:
        help.frameOverlayVisible |= visible;
        help.frameOverlaySelected |= selected;
        break;
    }
    help.frameOverlaySelected &= help.frameOverlayVisible;
    help.persistentSelected &= help.persistentVisible;
    ConfigureHelpDirectionBlink(help.persistentVisible);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
const HelpIconState &GetHelpIconState() {
    return helpIconState;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t GetPersistentHelpIconFlags(std::uint32_t index) {
    if (index == 0)
        return helpIconState.persistentVisible;
    if (index == 1)
        return helpIconState.persistentSelected;
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetHelpIconItemEffectState() {
    return helpIconItemEffectState;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void doItemEffectStuff() {
    helpIconItemEffectFrames = 150;
    helpIconItemEffectState = 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void RequestHelpIconItemPrompt() {
    helpIconItemPromptFrames = 150;
    CheckHelpIcon(0x200, 0x200, CheckHelpIconOp::Add);
    SetHudItemPromptIcon(1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void SetHudItemPromptIcon(std::int32_t enabled) {
    nativeHudItemPromptIcon = enabled;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t GetUiFlag(std::uint8_t mode) {
    if (!mode)
        return static_cast<std::uint16_t>(helpIconState.persistentVisible);
    return mode == 1 || (mode == 2 && helpIconState.persistentVisible);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetQteControlState() noexcept {
    return qteControlState;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetQteControlState(std::int32_t state) noexcept {
    if (qteControlState != 1 && state == 1)
        qteControlMask = 0;
    qteControlState = state;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void SetQteCameraAtTop(std::int32_t mode) noexcept {
    qteCameraAtTop = mode;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void qteSetupMaskFor90(std::uint32_t mask) noexcept {
    qteControlMask = mask;
    qteDrawFrames = 90;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t GetQteMaskFor90() noexcept {
    return qteControlMask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t GetQuickIconLoadingReleaseState() noexcept {
    return quickIconLoadingReleaseFlag;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetNowQteDrawEnabled(bool enabled) noexcept {
    nowQteDrawSuppression = enabled;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsNowQteDrawEnabled() noexcept {
    return nowQteDrawSuppression;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetHelpIconMenuFlags(std::uint16_t flags) {
    if (GetQuickIconLoadingReleaseState() == 0 && (g_loadingFlag & 0x20U) != 0)
        flags = static_cast<std::uint16_t>(flags & ~1U);
    helpIconState.frameOverlaySelected &= helpIconState.frameOverlayVisible;
    helpIconState.persistentVisible = flags;
    helpIconState.persistentSelected = 0;
    ConfigureHelpDirectionBlink(flags);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void AddHelpIconMenuFlags(std::uint16_t flags) {
    const bool releaseLoading = GetQuickIconLoadingReleaseState() != 0 && !(helpIconPreviousLoadingFlags & 0x20U) && (g_loadingFlag & 0x20U);
    helpIconPreviousLoadingFlags = g_loadingFlag;
    if (releaseLoading)
        FinishLoading();
    if ((flags & 0x400U) != 0)
        ExecuteAicaCommand(0xaabU, 0, 0);
    const std::uint32_t visible = static_cast<std::uint32_t>(flags) | helpIconState.persistentVisible;
    if (helpIconState.persistentVisible)
        helpIconState.persistentSelected &= visible;
    else
        helpIconState.persistentSelected = 0;
    helpIconState.frameOverlaySelected &= helpIconState.frameOverlayVisible;
    helpIconState.persistentVisible = visible;
    ConfigureHelpDirectionBlink(visible);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ResetHelpIconsToItemPrompt() {
    const std::uint32_t visible = helpIconState.persistentVisible & 0x200U;
    CheckHelpIcon(visible, visible, CheckHelpIconOp::Set);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t GetIconSelectFlags() noexcept {
    return iconSelectFlags;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void set_icon_select_flags_to(std::uint32_t flags) {
    iconSelectFlags |= flags;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetGameMessageContent(std::int32_t mode, std::int32_t value, std::string_view text) {
    gameMessageMode = mode;
    gameMessageIcon = mode == 0 ? value : -1;
    if (mode == 1 || mode == 2) {
        FormatGameMessageAmount(value, mode == 1);
    } else {
#ifdef _WIN32
        sprintf_s(gameMessageText.data(), gameMessageText.size(), "%s", text.data());
#else
        const std::size_t length = std::strlen(text.data());
        if (length >= gameMessageText.size())
            std::abort();
        std::memcpy(gameMessageText.data(), text.data(), length + 1);
#endif
    }
    ApplyGameMessageContent();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateGameMessageMoney(std::int32_t value) {
    FormatGameMessageAmount(value, true);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsGameMessageVisible() {
    return GameMessagePosition() > 0.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetGameMessageDelay(float seconds) {
    gameMessageNextDelay = seconds;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ClearGameMessageMode() {
    gameMessageMode = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeNativeGuiResources() {
    // Native GUI_Tasks_Enqueue allocates ITPH afresh for every CTRL: the
    // previous block was released with the module's use space.
    guiPtxBacking = static_cast<std::byte *>(HLib::AllocateTaskData(guiPtxBackingSize, SHEN_NAME(ITPH)));
    if (!guiPtxBacking)
        return false;
    HLib::AssignDynamicAlignmentForTaskData(guiPtxBacking);
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t RasterizeItemLabelNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) noexcept {
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint16_t PublishLegacyItemLabel(const char *text, std::uint16_t width) noexcept {
    (void)SetPtxDimensions(guiPtxBacking, 16, width, 0x8089);
    (void)RasterizeItemLabelNullsub(guiPtxBacking, width, text, 0, 1);
    (void)ReplacePtxTexture(commonItemTexture, guiPtxBacking);
    return static_cast<std::uint16_t>(commonItemTexture);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t PrepareLegacyInventoryItemLabel(std::uint16_t &label, std::uint32_t item) {
    itemSelectorLabel = shenGetAmtOf(item);
    if (!itemSelectorLabel)
        itemSelectorLabel = "";
    const std::int64_t length = static_cast<std::int64_t>(std::strlen(itemSelectorLabel));
    label = 0xffff;
    label = PublishLegacyItemLabel(itemSelectorLabel, 512);
    return length;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetLegacyItemSelectorExitFlag() noexcept {
    itemSelectorSelection.exitFlags |= 1U;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetItemSelectorDelayedResult(std::uint16_t result) noexcept {
    itemSelectorDelayedResult = result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetItemSelectorNavigationInputHalf(std::int8_t half, std::int16_t value) noexcept {
    std::memcpy(reinterpret_cast<std::byte *>(&itemSelectorNavigationInput) + half * 2, &value, sizeof(value));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool GiveItemOf(std::uint32_t itemId) {
    return ToggleItemEffect(itemId, 1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool initSCNT_item_data() {
    commonItemTexture = 0xffffffffU;
    const std::span<const std::byte> common = SetupPtxTexture(512, ITEM_TEXTURE_TYPE.name, ITEM_COMMON_TEXTURE.name);
    if (common.data()) {
        commonItemTexture = static_cast<std::uint32_t>(ReadTexnTexture(common.data()));
        if (static_cast<std::int32_t>(commonItemTexture) < 0)
            return false;
    }
    scenarioItemPtx = static_cast<std::byte *>(HLib::AllocateTaskData(0x8030, ITEM_COUNTER_TEXTURE));
    if (!scenarioItemPtx)
        return true;
    ParseGuiPtx(scenarioItemPtx, 256, 16, ITEM_TEXTURE_TYPE.name, ITEM_COUNTER_TEXTURE.name);
    scenarioItemTexture = UploadTexn({scenarioItemPtx, guiPtxTextSize});
    return scenarioItemTexture != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t InitializeCtrlSpeakIconState() {
    ctrlSpeakSpecialLabelWidthScale = 2.0f;
    ctrlSpeakResultChoiceId = ctrlSpeakDefaultChoiceId = 0xffffU;
    ctrlSpeakTimeoutFrames = 10800;
    ctrlSpeakSelectorOrigin = CTRL_SPEAK_ORIGIN_INITIAL;
    ctrlSpeakIconState = 0;
    ctrlSpeakOpenRequested = false;
    ctrlSpeakElapsedFrames = 0;
    ctrlSpeakDedicatedCancelEnabled = 0;
    ctrlSpeakHasSpecialLabel = ctrlSpeakForceTextLabels = false;
    ctrlSpeakSelectedChoiceSlot = 4;
    ctrlSpeakSelectorScale = 1.0f;
    for (std::size_t slot = 0; slot < 4; ++slot) {
        LegacyIconSprite &sprite = ctrlSpeakChoiceDrawSprites[slot];
        sprite.icon = 0xffffU;
        sprite.flags = 0;
        sprite.width = 256;
        sprite.height = 32;
        sprite.scaleX = sprite.scaleY = 1.0f;
        sprite.x = static_cast<float>(SLEEP_ARROW_X[slot]) - 176.0f;
        sprite.y = ACTION_SELECTOR_CHOICE_Y[slot] + 91.0f;
        sprite.rotation = 0;
        sprite.alignment = 5;
        sprite.depth = SLEEP_TEXT_DEPTH[slot] + 1.0f;
    }
    ctrlSpeakTexturePoolFlags.fill(0);
    ctrlSpeakTexturePoolHandles.fill(-1);
    ctrlSpeakChoiceTextHandles.fill(-1);
    std::int64_t result = -1;
    for (std::size_t index = 0; index < ctrlSpeakTexturePoolHandles.size(); ++index) {
        const std::span<const std::byte> texture =
            SetupPtxTexture(256, CTRL_SPEAK_TEXTURE_TYPE.name, make_shen_name('0', '0', '0', static_cast<char>('0' + index)));
        result = ReadTexnTexture(texture.data());
        ctrlSpeakTexturePoolHandles[index] = static_cast<std::int32_t>(result);
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
HLib::Task *InitSpeakIcon(HLib::Task *parent, std::uint8_t nextFunctionIndex) {
    InitializeCtrlSpeakIconState();
    speakIconTask = HLib::QueueTaskAfterCurrentTask(parent, CtrlSpeakIcon_Callback, nextFunctionIndex, 0, TASK_CTRL_SPEAK.name);
    return speakIconTask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
HLib::Task *InitNowQte(std::uint8_t nextFunctionIndex) {
    helpIconPreviousLoadingFlags = 0;
    helpIconItemPromptFrames = -1;
    helpIconItemEffectFrames = -1;
    helpIconItemEffectState = 0;
    currentIconDeltaTime = 0.0f;
    iconDeltaTime = 7;
    helpIconBlinkFrames = 5;
    helpIconDeferredRemoval = 0;
    helpIconState.frameOverlayVisible = helpIconState.frameOverlaySelected = 0;
    helpIconState.persistentVisible = helpIconState.persistentSelected = 0;
    iconSelectFlags = 0;
    helpIconPublishedVisible = 0;
    helpIconState.removalBlinkMask = 0;
    helpIconHudDelay = 5;
    helpIconRefreshDelay = -1;
    qteControlState = 0;
    qteCameraAtTop = 0;
    ParseGuiPtx(nowQtePtx.data(), 256, 16, NOW_QTE_TEXTURE_TYPE.name, NOW_QTE_TEXTURE_NAME.name);
    nowQteTexture = static_cast<std::uint32_t>(ReadTexnTexture(nowQtePtx.data()));
    (void)RasterizeItemLabelNullsub(nowQtePtx.data(), 256, "Now processing...", 0, 1);
    (void)ReplacePtxTexture(nowQteTexture, nowQtePtx.data());
    nowQteDrawSuppression = 0;
    nowQteTask = HLib::EnqueueTaskWithoutParameter(NQ_CtrlButtonIcon, nextFunctionIndex, 0, static_cast<std::int32_t>(TASK_NOW_QTE.name));
    return nowQteTask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int32_t SetupControlSpeakAndUI(std::int16_t *choices, std::int32_t timeout, std::int16_t defaultChoice, std::uint16_t mode) {
    return static_cast<std::int32_t>(SetupControlSpeakAndUI_Internal(choices, timeout, defaultChoice, mode, 0));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t SetupControlSpeakAndUI_Internal(std::int16_t *choices, std::int32_t, std::int16_t defaultChoice, std::uint16_t mode, std::int32_t style) {
    dialogueSelectionOwned = false;
    ctrlSpeakDedicatedCancelEnabled = 0;
    for (std::size_t index = 0; index < 4; ++index)
        if (choices[index] == 470)
            choices[index] = -1;
    // Native PC widget labels are assigned before legacy normalization.
    dialogueSelectionPrompt = {};
    for (std::size_t index = 0; index < 4; ++index) {
        dialogueSelectionPrompt.flagIds[index] = static_cast<std::uint16_t>(choices[index]);
        PublishActionSelectorText(index, GetActionSelectorLabel(dialogueSelectionPrompt.flagIds[index]));
        dialogueSelectionPrompt.textAvailable |= !dialogueSelectionPrompt.labels[index].empty();
    }
    numericActionSelector.mode = 0;
    dialogueSelectionPrompt.active = true;
    dialogueSelectionPrompt.focusedSlot = FirstDialogueSelectionSlot();
    ctrlSpeakResultChoiceId = 0xffff;
    ctrlSpeakOpenRequested = true;
    ctrlSpeakMarkerGlyphIndex = style >= 5 ? 0 : style;
    ctrlSpeakTimeoutFrames = 10800;
    ctrlSpeakElapsedFrames = 0;
    dialogueSelectionTimedOut = false;
    for (std::size_t index = 0; index < 4; ++index)
        if (choices[index] >= 486)
            choices[index] = -1;
    if (choices[0] == choices[1] && choices[2] == choices[3] && choices[0] == choices[2]) {
        const std::uint64_t zero = 0;
        std::memcpy(choices, &zero, sizeof(zero));
    }
    ctrlSpeakChoiceIds = {choices[0], choices[1], choices[3], choices[2]};
    ctrlSpeakSpecialLabelWidthScale = 2.0f;
    ctrlSpeakHasSpecialLabel = ctrlSpeakForceTextLabels = false;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::string_view label = GetActionSelectorLabel(static_cast<std::uint16_t>(ctrlSpeakChoiceIds[index]));
        ctrlSpeakChoiceWidths[index] = static_cast<std::int16_t>(label.size() > 7 ? 14 * label.size() + 30 : 128);
    }
    if (ctrlSpeakChoiceWidths[1] != ctrlSpeakChoiceWidths[3]) {
        if (ctrlSpeakChoiceWidths[1] <= ctrlSpeakChoiceWidths[3])
            ctrlSpeakChoiceWidths[1] = ctrlSpeakChoiceWidths[3];
        else
            ctrlSpeakChoiceWidths[3] = ctrlSpeakChoiceWidths[1];
    }
    ctrlSpeakMode = mode;
    ctrlSpeakDefaultChoiceId = mode ? static_cast<std::uint16_t>(defaultChoice) : 0xffffU;
    const float expansion = static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f;
    ctrlSpeakHalfWidthExpansion = expansion * 0.5f;
    ctrlSpeakRightOriginX = expansion + 180.0f;
    ctrlSpeakLeftOriginX = -176.0f - expansion;
    ToggleRenderIcons(RenderIconSlot::ActionSelector, true);
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t CloseSpeakIcon(std::int32_t force) {
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector) || !force)
        return 0;
    ctrlSpeakSelectorOrigin[0] = -176.0f;
    ctrlSpeakIconState = 0;
    ctrlSpeakOpenRequested = false;
    ctrlSpeakDedicatedCancelEnabled = 0;
    iconToggleStateFor(2, false);
    ReleaseSpeakTextSlots();
    ToggleRenderIcons(RenderIconSlot::ActionSelector, false);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
std::uint32_t shenGetCtrlSpeakIconFlag() {
    return ctrlSpeakResultChoiceId;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t SetupControlSpeakAndUI_Internal(std::int16_t *choices, std::int32_t, std::uint16_t defaultChoice, std::uint16_t mode, std::int32_t style) {
    ctrlSpeakResultChoiceId = 0xffff;
    ctrlSpeakOpenRequested = 1;
    ctrlSpeakMarkerGlyphIndex = style;
    if (static_cast<std::uint32_t>(style) > 4U)
        ctrlSpeakMarkerGlyphIndex = 0;
    ctrlSpeakTimeoutFrames = 10800;
    ctrlSpeakElapsedFrames = 0;
    if (choices[0] >= ctrlSpeakChoiceCount)
        choices[0] = -1;
    if (choices[1] >= ctrlSpeakChoiceCount)
        choices[1] = -1;
    if (choices[2] >= ctrlSpeakChoiceCount)
        choices[2] = -1;
    if (choices[3] >= ctrlSpeakChoiceCount)
        choices[3] = -1;
    if (choices[0] == choices[1] && choices[2] == choices[3] && choices[0] == choices[2]) {
        choices[3] = 0;
        choices[2] = 0;
        choices[1] = 0;
        choices[0] = 0;
    }
    InspectCtrlSpeakChoicesNullsub(choices);
    ctrlSpeakChoiceIds[0] = choices[0];
    ctrlSpeakChoiceIds[1] = choices[1];
    ctrlSpeakChoiceIds[3] = choices[2];
    ctrlSpeakChoiceIds[2] = choices[3];
    ctrlSpeakLabelPhase = 0;
    ctrlSpeakChoiceMode = 0;
    ctrlSpeakChoiceSpecial = 0;
    ctrlSpeakLabelMode = 0;
    ctrlSpeakSpecialLabelWidthScale = 2.0f;
    for (std::int32_t index = 0; index < 4; ++index)
        ctrlSpeakChoiceWidths[index] =
            static_cast<std::int32_t>(GetCtrlSpeakChoiceWidth(reinterpret_cast<const std::uint16_t *>(&ctrlSpeakChoiceIds[index]), 0));
    if (ctrlSpeakChoiceWidths[1] != ctrlSpeakChoiceWidths[3]) {
        if (ctrlSpeakChoiceWidths[1] > ctrlSpeakChoiceWidths[3])
            ctrlSpeakChoiceWidths[3] = ctrlSpeakChoiceWidths[1];
        else
            ctrlSpeakChoiceWidths[1] = ctrlSpeakChoiceWidths[3];
    }
    ctrlSpeakHalfWidthExpansion = (static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f) * 0.5f;
    ctrlSpeakRightOriginX = (static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f) + 180.0f;
    ctrlSpeakLeftOriginX = -176.0f - (static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f);
    std::int16_t &signedMode = reinterpret_cast<std::int16_t &>(ctrlSpeakMode);
    signedMode = mode;
    if (!signedMode)
        ctrlSpeakDefaultChoiceId = 0xffff;
    else
        ctrlSpeakDefaultChoiceId = defaultChoice;
    ToggleRenderIcons(ActionSelector, true);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t SetupControlSpeakAndUI(std::int16_t *choices, std::int16_t timeout, std::uint16_t defaultChoice, std::uint16_t mode) {
    return static_cast<std::uint8_t>(SetupControlSpeakAndUI_Internal(choices, timeout, defaultChoice, mode, 0));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t IsCtrlSpeakIconInactive() {
    if (ctrlSpeakIconState == 0)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t IsSpeakIconCancellable() {
    const std::int32_t mode = static_cast<std::int16_t>(ctrlSpeakMode);
    if (mode == 2 || mode == 5)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool CloseSpeakIcon(std::int32_t force) {
    if (GetShouldRenderIcons(ActionSelector) && force) {
        ctrlSpeakIconState = 0;
        ctrlSpeakSelectorPosition.x = -176.0f;
        ctrlSpeakOpenRequested = 0;
        iconToggleStateFor(2, false);
        ReleaseSpeakTextSlots();
        ToggleRenderIcons(ActionSelector, false);
        return true;
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CloseSpeakIcon() {
    if (GetParentMenu() != 4)
        return;
    ctrlSpeakIconState = 0;
    padcRefreshCurrentInputs(0);
    ToggleRenderIcons(ActionSelector, false);
    iconToggleStateFor(2, false);
    ReleaseSpeakTextSlots();
    ctrlSpeakOpenRequested = 0;
    ctrlSpeakSelectorPosition.x = -176.0f;
    if (ctrlSpeakSavedLoadingFlags & 0x20U) {
        g_loadingFlag = ctrlSpeakSavedLoadingFlags;
        ctrlSpeakSavedLoadingFlags = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InspectCtrlSpeakChoicesNullsub(const std::int16_t *choices) {
    for (std::int32_t index = 0; index < 4; ++index, ++choices) {
        if (*choices) {
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float GetCtrlSpeakChoiceWidth(const std::uint16_t *choice, std::int32_t form) {
    std::int32_t characterWidths[2] = {28, 14};
    std::int16_t width = 128;
    if (*choice != 0xffff) {
        const char *text;
        if (form) {
            text = ctrlSpeakLabels[*choice].alternate;
            if (!text) {
                text = ctrlSpeakLabels[*choice].primary;
                form = 0;
            }
        } else {
            text = ctrlSpeakLabels[*choice].primary;
            if (!text) {
                text = ctrlSpeakLabels[*choice].alternate;
                form = 1;
            }
        }
        const std::int32_t threshold = ctrlSpeakLabelBaseCharacters[form];
        const std::int32_t length = static_cast<std::int32_t>(std::strlen(text));
        if ((length >> 1) > threshold)
            width = static_cast<std::int16_t>(((length >> 1) - threshold) * characterWidths[form] + width);
    }
    return static_cast<float>(width);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void DrawActionSelectorTextFrame(const Vector2 &position, float depth, std::int32_t width, std::int32_t height, std::uint32_t selected) {
    static const std::int32_t textFrameIcons[4] = {1284, 1283, 1282, 1281};
    static const std::int32_t *textFramePair;
    textFramePair = textFrameIcons + (selected & 1) * 2;
    LegacyIconSprite sprite;
    sprite.depth = depth;
    std::int32_t edge;
    if (width > 32)
        edge = 16;
    else
        edge = width / 2;
    const std::int32_t centre = width - 2 * edge;
    sprite.icon = textFramePair[1];
    sprite.scaleX = 1.0f;
    sprite.scaleY = 1.0f;
    sprite.flags = 0;
    sprite.y = position.y;
    sprite.width = edge;
    sprite.height = height;
    sprite.alignment = 5;
    sprite.rotation = 0;
    sprite.x = (position.x - static_cast<float>(centre / 2)) - static_cast<float>(edge / 2);
    PrepareDrawWithMode(sprite);
    sprite.x = (static_cast<float>(centre / 2) + position.x) + static_cast<float>(edge / 2);
    sprite.flags = 0x100;
    sprite.alignment = 5;
    PrepareDrawWithMode(sprite);
    sprite.icon = textFramePair[0];
    sprite.alignment = 5;
    sprite.flags = 0x400;
    sprite.x = position.x;
    sprite.width = centre;
    PrepareDrawWithMode(sprite);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void DrawActionSelectorRadial(const Vector2 &position, const std::int32_t *handles, float depth, float scale, const std::int16_t *widths,
    std::int32_t selected, std::int32_t marker, std::int32_t forceText) {
    float radialX[4] = {0.0f, -1.0f, 0.0f, 1.0f}, radialY[4] = {-1.0f, 0.0f, 1.0f, 0.0f};
    LegacyIconSprite sprite;
    selectorRingIcons[1] = selectorMarkers[marker];
    sprite.icon = 1280;
    sprite.width = sprite.height = static_cast<std::int16_t>((scale + 0.5f) * 64.0f);
    sprite.scaleX = sprite.scaleY = 1.0f;
    sprite.rotation = 0;
    sprite.alignment = 5;
    sprite.flags = 0;
    sprite.depth = depth;
    sprite.x = position.x;
    sprite.y = position.y;
    PrepareDrawWithMode(sprite);
    sprite.width = sprite.height = 40;
    sprite.depth = depth + 3.0f;
    std::uint32_t *angles[3] = {0, &ctrlSpeakPulseAngle, 0};
    for (std::int32_t ring = 0; ring < 3; ++ring) {
        if (!angles[ring])
            sprite.scaleX = 1.0f;
        else
            sprite.scaleX = dc::sdk::CosFixed(*angles[ring]);
        sprite.icon = selectorRingIcons[ring];
        PrepareDrawWithMode(sprite);
        sprite.depth -= 0.5f;
    }
    sprite.depth = depth;
    sprite.scaleY = sprite.scaleX = 1.0f;
    sprite.width = 32;
    sprite.height = 16;
    sprite.icon = 1285;
    const std::int32_t inset = static_cast<std::int32_t>(scale * 3.0f);
    LegacyIconSprite &text = ctrlSpeakLabelSprites[0];
    LegacyIconSprite special;
    std::int32_t specials[7];
    Vector2 label;
    for (std::int32_t slot = 0; slot < 4; ++slot) {
        const float retainedScale = text.scaleX;
        text.scaleX = text.scaleY = scale;
        if (handles[slot] != -1) {
            sprite.x = radialX[slot] * 16.0f + position.x;
            sprite.y = position.y + radialY[slot] * 16.0f;
            sprite.rotation = static_cast<std::uint16_t>(static_cast<float>((180 - 90 * slot) * 0x2000) / 45.0f);
            sprite.depth = depth + 0.2f;
            sprite.flags = 0;
            PrepareDrawWithMode(sprite);
            const float height = scale * 36.0f;
            float width = static_cast<float>(widths[slot]);
            width *= scale;
            float radialW = width * 0.5f + 24.0f - static_cast<float>(inset), radialH = height * 0.5f + 24.0f - static_cast<float>(inset);
            label.x = radialX[slot] * radialW + position.x;
            label.y = radialY[slot] * radialH + position.y;
            if (slot == selected)
                DrawActionSelectorTextFrame(label, depth + 0.1f, static_cast<std::int32_t>(width), static_cast<std::int32_t>(scale * 64.0f), 1);
            else
                DrawActionSelectorTextFrame(label, depth + 0.1f, static_cast<std::int32_t>(width), static_cast<std::int32_t>(scale * 64.0f), 0);
            text.y = radialY[slot] * radialH + position.y;
            text.x = radialX[slot] * radialW + position.x;
            radialW = width * 0.5f + radialW + 8.0f - static_cast<float>(inset);
            radialH = height * 0.5f + radialH + 8.0f - static_cast<float>(inset);
            sprite.x = radialX[slot] * radialW + position.x;
            sprite.y = radialY[slot] * radialH + position.y;
            sprite.flags = 0x200;
            sprite.depth = depth + 0.2f;
            PrepareDrawWithMode(sprite);
            if (forceText) {
                text.icon = static_cast<std::uint16_t>(handles[slot]);
                PrepareDrawTextureWithMode(text);
            } else {
                specials[0] = 347;
                specials[1] = 39;
                specials[2] = 348;
                specials[3] = 38;
                specials[4] = 349;
                specials[5] = 40;
                specials[6] = 0;
                std::int32_t index = 0;
                special.icon = 0;
                for (; specials[index]; index += 2)
                    if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) == specials[index]) {
                        special.icon = static_cast<std::uint16_t>(specials[index + 1]);
                        break;
                    }
                if (special.icon)
                    ctrlSpeakChoiceSpecial = 1;
                if (!ctrlSpeakLabelMode && special.icon) {
                    special.rotation = 0;
                    special.flags = 0;
                    special.alignment = 5;
                    special.width = special.height = 32;
                    special.scaleX = text.scaleX + text.scaleX;
                    special.scaleY = text.scaleY * ctrlSpeakSpecialLabelWidthScale;
                    special.depth = text.depth + 0.2f;
                    special.x = text.x;
                    special.y = text.y;
                    PrepareDrawWithMode(special);
                } else {
                    text.icon = static_cast<std::uint16_t>(handles[slot]);
                    PrepareDrawTextureWithMode(text);
                }
            }
        }
        text.scaleX = text.scaleY = retainedScale;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void DrawCtrlSpeakSelector(void *) {
    float offset = 0.0f;
    if (ctrlSpeakChoiceTextHandles[0] == -1 && ctrlSpeakChoiceTextHandles[2] != -1)
        offset = -18.0f;
    Vector2 position;
    position.x = ctrlSpeakRightOriginX;
    position.y = ctrlSpeakSelectorPosition.y + offset;
    const std::int32_t selected = ctrlSpeakSelectionAnimationFrame % 5 == 0 ? static_cast<std::int16_t>(ctrlSpeakSelectorPosition.selectedDirection) : 4;
    DrawActionSelectorRadial(position, ctrlSpeakChoiceTextHandles, 1.0f, ctrlSpeakRadialScale, ctrlSpeakChoiceWidths, selected, ctrlSpeakMarkerGlyphIndex, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CtrlSpeakIcon_Callback(void *) {
    ctrlSpeakPulseAngle = (ctrlSpeakPulseAngle + 1638U) & 0xffffU;
    switch (ctrlSpeakIconState) {
    case 0:
        if (ctrlSpeakOpenRequested) {
            ExecuteAicaCommand(0x400ab, 0, 0);
            ctrlSpeakIconState = 1;
            PrepareSpeakTextSlots();
            padcRefreshCurrentInputs(1);
            iconToggleStateFor(2, true);
            ctrlSpeakResultChoiceId = 0xffff;
            ctrlSpeakSavedLoadingFlags = g_loadingFlag;
            if (g_loadingFlag & 0x20U)
                FinishLoading();
            ctrlSpeakRadialScale = 0.0f;
            ctrlSpeakRetainedMotion = 10.0f;
            ctrlSpeakSelectionAnimationFrame = -1;
            ctrlSpeakSelectorPosition.x = ctrlSpeakRightOriginX;
        }
        break;
    case 1:
        ctrlSpeakRetainedMotion -= 1.0f;
        if (!(ctrlSpeakRetainedMotion > 0.0f)) {
            ctrlSpeakRetainedMotion = 0.0f;
            ctrlSpeakRadialScale += 0.2f;
            if (!(ctrlSpeakRadialScale < 1.0f)) {
                ctrlSpeakRadialScale = 1.0f;
                ctrlSpeakIconState = 2;
            }
        }
        break;
    case 4:
        ctrlSpeakRadialScale -= 0.2f;
        if (!(ctrlSpeakRadialScale > 0.0f)) {
            ctrlSpeakIconState = 0;
            padcRefreshCurrentInputs(0);
            ToggleRenderIcons(ActionSelector, false);
            iconToggleStateFor(2, false);
            ReleaseSpeakTextSlots();
            ctrlSpeakOpenRequested = 0;
            if (ctrlSpeakSavedLoadingFlags & 0x20U) {
                g_loadingFlag = ctrlSpeakSavedLoadingFlags;
                ctrlSpeakSavedLoadingFlags = 0;
            }
        }
        break;
    case 3:
        if (static_cast<std::int16_t>(ctrlSpeakSelectorPosition.selectedDirection) == 4) {
            ExecuteAicaCommand(0x400ab, 0, 0);
            ctrlSpeakIconState = 4;
        }
        break;
    case 2:
        if (GetShouldRenderIcons(ActionSelector))
            ProcessSpeakSelection();
        break;
    case 5: {
        float interpolation = ctrlSpeakResizeFrame / 5.0f;
        interpolation = 1.0f - interpolation;
        for (std::int32_t slot = 0; slot < 4; ++slot) {
            if (ctrlSpeakChoiceWidthDeltas[slot])
                ctrlSpeakChoiceWidths[slot] = static_cast<std::int16_t>(static_cast<std::int32_t>(
                    static_cast<float>(ctrlSpeakTargetChoiceWidths[slot]) - static_cast<float>(ctrlSpeakChoiceWidthDeltas[slot]) * interpolation));
            else
                ctrlSpeakChoiceWidths[slot] = ctrlSpeakTargetChoiceWidths[slot];
        }
        ctrlSpeakResizeFrame += 1.0f;
        if (ctrlSpeakResizeFrame > 5.0f) {
            for (std::int32_t slot = 0; slot < 4; ++slot)
                ctrlSpeakChoiceWidths[slot] = ctrlSpeakTargetChoiceWidths[slot];
            ctrlSpeakIconState = 2;
        }
        if (ctrlSpeakChoiceWidths[1] != ctrlSpeakChoiceWidths[3]) {
            if (ctrlSpeakChoiceWidths[1] > ctrlSpeakChoiceWidths[3])
                ctrlSpeakChoiceWidths[3] = ctrlSpeakChoiceWidths[1];
            else
                ctrlSpeakChoiceWidths[1] = ctrlSpeakChoiceWidths[3];
        }
        ctrlSpeakHalfWidthExpansion = (static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f) * 0.5f;
        ctrlSpeakRightOriginX = static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f + 180.0f;
        ctrlSpeakLeftOriginX = -176.0f - (static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f);
        ctrlSpeakSelectorPosition.x = ctrlSpeakRightOriginX;
        break;
    }
    default:
        break;
    }
    if (ctrlSpeakLabelPhase > 0)
        UpdateSpeakLabelTransition();
    if (ctrlSpeakLabelPhase < 0)
        UpdateSpeakLabelReverseTransition();

    if (GetItemSelectionMode(2)) {
        if (static_cast<std::int16_t>(ctrlSpeakSelectorPosition.selectedDirection) != 4 && ++ctrlSpeakSelectionAnimationFrame > 19) {
            ctrlSpeakSelectorPosition.selectedDirection = 4;
            ctrlSpeakSelectionAnimationFrame = 0;
        }
        HLib::ExecuteCallbackNow(DrawCtrlSpeakSelector, 0);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void ReleaseSleepActionLabels() {
    for (std::int8_t slot = 0; slot < 4; ++slot) {
        if (static_cast<std::int16_t>(sleepActionTextSprites[slot].icon) != -1)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sleepActionTextSprites[slot].icon));
        sleepActionTextSprites[slot].icon = -1;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const char *const sleepActionTextLabels[4] = {"\xec\xa8\xea\xc7", "\xda\xce\xea\xc3", "\xd6\xdb\xef\xd5\xea\xc7", "\xec\xa7\xed\xe1"};
static const std::int32_t sleepActionTextBits[4] = {0, 1, 4, 3};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void ChooseSleepActionText(std::uint32_t flags) {
    for (std::int32_t slot = 0; slot < 4; ++slot) {
        if (static_cast<std::int16_t>(sleepActionTextSprites[slot].icon) != -1) {
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sleepActionTextSprites[slot].icon));
            sleepActionTextSprites[slot].icon = static_cast<std::uint16_t>(-1);
        }
        if (flags & (1U << sleepActionTextBits[slot]))
            sleepActionTextSprites[slot].icon = static_cast<std::uint16_t>(AcquireCtrlSpeakTextHandle(sleepActionTextLabels[slot], 1));
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void DrawSleepActionSelector(void *) {
    std::int32_t handles[4];
    std::int16_t widths[4] = {128, 128, 128, 128};
    Vector2 position;
    position.x = g_sleepAction.x;
    position.y = g_sleepAction.y;
    const std::uint32_t enabled = sleepActionFlags & 0x1bU;
    for (std::int8_t slot = 0; slot < 4; ++slot) {
        if (enabled & (1U << sleepActionTextBits[slot]))
            handles[slot] = static_cast<std::int16_t>(sleepActionTextSprites[slot].icon);
        else
            handles[slot] = -1;
    }
    const std::int32_t selected = sleepActionRetargetFrames % 5 == 0 ? g_sleepAction.targetSelection : 4;
    DrawActionSelectorRadial(position, handles, g_sleepAction.depth, sleepActionScale, widths, selected, 2, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void CtrlSleepActionIcon_Callback(void *) {
    std::uint16_t pressed = ctrlGetPadInputBuffer0(g_sleepActionPads.current)->pressedPriority;
    switch (static_cast<std::int8_t>(g_sleepAction.phase)) {
    case 0:
        if (g_sleepAction.inputMode) {
            ResetHelpIconsToItemPrompt();
            AddHelpIconMenuFlags((sleepActionFlags & 0x100U) ? 3 : 1);
            ExecuteAicaCommand(0x400ab, 0, 0);
            g_sleepActionPads.previous = BindPadControlEntry(g_sleepActionPads.current);
            g_sleepAction.phase = 1;
            g_sleepAction.currentSelection = 4;
            iconToggleStateFor(6, true);
            ChooseSleepActionText(sleepActionFlags & 0x1bU);
        }
        break;
    case 1:
        if (sleepActionScale >= 1.0f)
            g_sleepAction.phase = 2;
        else
            sleepActionScale += 0.16666667f;
        break;
    case 6:
        g_sleepAction.phase = 0;
        sleepActionScale = 0.0f;
        ToggleRenderIcons(SleepAction, false);
        iconToggleStateFor(6, false);
        ReleaseSleepActionLabels();
        BindPadControlEntry(g_sleepActionPads.previous);
        g_sleepAction.inputMode = 0;
        g_sleepAction.result = 1;
        sleepActionPublishedSelection = 4;
        break;
    case 3:
        if (g_sleepAction.targetSelection == 4) {
            if (g_sleepAction.currentSelection == 4) {
                if (sleepActionScale <= 0.0f) {
                    ResetHelpIconsToItemPrompt();
                    g_sleepAction.phase = 6;
                } else
                    sleepActionScale -= 0.16666667f;
            } else
                g_sleepAction.phase = 4;
        }
        break;
    case 4:
        g_sleepAction.phase = 0;
        ToggleRenderIcons(SleepAction, false);
        ReleaseSleepActionLabels();
        BindPadControlEntry(g_sleepActionPads.previous);
        iconToggleStateFor(6, false);
        g_sleepAction.inputMode = 0;
        g_sleepAction.x = -176.0f;
        switch (g_sleepAction.currentSelection) {
        case 3:
            SetNewEasyOptionState(1);
            g_sleepAction.result = 0;
            ResetHelpIconsToItemPrompt();
            AddHelpIconMenuFlags(0x21);
            sleepActionPublishedSelection = 3;
            break;
        case 0:
            g_sleepAction.result = 2;
            ResetHelpIconsToItemPrompt();
            sleepActionPublishedSelection = 0;
            break;
        case 1:
            if (!IsSaveAllowed()) {
                StartShowScreenOfId(2);
                g_sleepAction.result = 0;
                sleepActionPublishedSelection = 1;
            }
            ResetHelpIconsToItemPrompt();
            break;
        case 2:
            break;
        }
        break;
    case 2: {
        if ((sleepActionFlags & 1U) == 0)
            pressed &= 0xefffU;
        if (!(sleepActionFlags & 2U))
            pressed &= 0xbfffU;
        if (!(sleepActionFlags & 8U))
            pressed &= 0x7fffU;
        if (!(sleepActionFlags & 0x100U))
            pressed &= 0xddffU;
        const std::uint8_t input = DecodeSelectorInput(pressed, 0x55);
        switch (input) {
        case 0:
        case 2:
        case 6: {
            ExecuteAicaCommand(0x500ab, 0, 0);
            g_sleepAction.phase = 3;
            const std::uint8_t slot = selectorDirectionToSlot[input];
            g_sleepAction.targetSelection = slot;
            g_sleepAction.currentSelection = slot;
            sleepActionRetargetFrames = -1;
            sleepActionArrowSprites[slot].depth = 5.0f;
            sleepActionTextSprites[slot].depth = 6.0f;
            break;
        }
        case 4:
        case 9:
            ExecuteAicaCommand(0x100ab, 0, 0);
            g_sleepAction.phase = 3;
            break;
        }
        break;
    }
    }
    if (GetItemSelectionMode(6)) {
        if (g_sleepAction.targetSelection != 4 && ++sleepActionRetargetFrames > 19) {
            g_sleepAction.targetSelection = 4;
            sleepActionRetargetFrames = 20;
        }
        HLib::ExecuteCallbackNow(DrawSleepActionSelector, 0);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void InitializeSleepActionIcon() {
    g_sleepActionPads.current = AllocatePadControlEntry();
    ResetSleepActionDefaultState();
    g_sleepAction.x = -176.0f;
    g_sleepAction.y = 118.0f;
    g_sleepAction.velocity = 1.0f;
    g_sleepAction.reservedScale = 0.0f;
    g_sleepAction.targetSelection = 4;
    for (std::int8_t slot = 0; slot < 4; ++slot) {
        sleepActionArrowSprites[slot].icon = 0;
        sleepActionArrowSprites[slot].flags = 0;
        sleepActionArrowSprites[slot].x = sleepArrowX[slot] + g_sleepAction.x;
        sleepActionArrowSprites[slot].y = sleepArrowY[slot] + g_sleepAction.y;
        sleepActionArrowSprites[slot].width = 64;
        sleepActionArrowSprites[slot].height = 64;
        sleepActionArrowSprites[slot].scaleX = 1.2f;
        sleepActionArrowSprites[slot].scaleY = 1.2f;
        sleepActionArrowSprites[slot].rotation = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>((360 - slot * 90) * 0x2000) / 45.0f));
        sleepActionArrowSprites[slot].depth = sleepArrowDepth[slot] + 1.0f;
        sleepActionArrowSprites[slot].alignment = 5;
        sleepActionTextSprites[slot].icon = 0xffff;
        sleepActionTextSprites[slot].flags = 0;
        sleepActionTextSprites[slot].x = sleepTextX[slot] + g_sleepAction.x;
        sleepActionTextSprites[slot].y = sleepTextY[slot] + g_sleepAction.y;
        sleepActionTextSprites[slot].width = 256;
        sleepActionTextSprites[slot].height = 32;
        sleepActionTextSprites[slot].scaleX = 1.0f;
        sleepActionTextSprites[slot].scaleY = 1.0f;
        sleepActionTextSprites[slot].rotation = 0;
        sleepActionTextSprites[slot].depth = sleepTextDepth[slot] + 1.0f;
        sleepActionTextSprites[slot].alignment = 5;
    }
    sleepActionFlags = 0;
    sleepActionPublishedSelection = 0xffff;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
HLib::Task *CtrlSleepActionIcon(std::int8_t queue) {
    InitializeSleepActionIcon();
    return HLib::EnqueueTaskWithoutParameter(CtrlSleepActionIcon_Callback, queue, 0, SHEN_NAME4('I', 'C', 'S', 'V'));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t PulseHelpBlinkCounter(std::int32_t *counter, std::int32_t onFrames, std::int32_t offFrames) {
    std::int32_t result;
    if (*counter > 0) {
        result = 1;
        if (--*counter < 1)
            *counter = -offFrames;
    } else {
        result = 0;
        if (++*counter >= 0)
            *counter = onFrames;
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t GetSleepActionIconFlags() {
    std::int32_t menu, sleeping;
    if (sleepActionFlags & 0x100U)
        menu = 0;
    else
        menu = 1;
    sleeping = (sleepActionFlags & 5U) != 0;
    return g_sleepAction.inputMode | menu << 1 | sleeping << 2;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t ResolveHelpHintForButton(std::uint32_t button) {
    switch (GetMediatedButtonOwner(button)) {
    case SHEN_NAME4('B', 'T', 'M', 'T'):
        return 2;
    case SHEN_NAME4('B', 'T', 'E', 'V'):
        return 4;
    case SHEN_NAME4('B', 'T', 'M', 'M'):
        return 25;
    case SHEN_NAME4('B', 'T', 'L', 'D'):
        return 3;
    case SHEN_NAME4('B', 'T', 'I', 'F'):
        return 16;
    case SHEN_NAME4('B', 'T', 'S', 'D'):
        return 22;
    case SHEN_NAME4('B', 'T', 'K', 'K'):
        return 12;
    case SHEN_NAME4('B', 'T', 'A', 'D'):
        return 11;
    case SHEN_NAME4('B', 'T', 'D', 'D'):
        return 15;
    case SHEN_NAME4('B', 'T', 'C', 'C'):
        return 6;
    case SHEN_NAME4('B', 'T', 'D', '2'):
    case SHEN_NAME4('B', 'T', 'D', 'R'):
        return 0;
    case SHEN_NAME4('B', 'T', 'F', 'T'):
        return 1;
    case SHEN_NAME4('B', 'T', 'S', 'P'):
        return 10;
    case SHEN_NAME4('B', 'T', 'M', 'R'):
        return 23;
    case SHEN_NAME4('B', 'T', 'B', 'T'):
        return 7;
    case SHEN_NAME4('B', 'T', 'M', 'P'):
        return 24;
    case SHEN_NAME4('B', 'T', 'P', 'P'):
        return 14;
    case SHEN_NAME4('B', 'T', 'N', 'V'):
        return 17;
    case SHEN_NAME4('B', 'T', 'T', 'T'):
        return 13;
    case SHEN_NAME4('B', 'T', 'S', 'W'):
        return 8;
    case SHEN_NAME4('B', 'T', 'B', 'B'):
    case SHEN_NAME4('B', 'T', 'B', 'A'):
    case SHEN_NAME4('B', 'T', 'B', 'X'):
    case SHEN_NAME4('B', 'T', 'B', 'Y'):
        return 21;
    case SHEN_NAME4('B', 'T', 'I', 'L'):
        if (!IsAnyItemSelectActive())
            return 9;
    case SHEN_NAME4('B', 'T', 'M', 'E'):
        return 5;
    case SHEN_NAME4('B', 'T', 'B', 'F'):
        switch (static_cast<std::int32_t>(button)) {
        case 0:
            return 12;
        case 1:
            return 13;
        case 2:
            return 14;
        default:
            return 15;
        }
    case SHEN_NAME4('B', 'T', 'M', 'K'):
        switch (static_cast<std::int32_t>(button)) {
        case 0:
            return 20;
        case 2:
            return 18;
        case 3:
            return 19;
        default:
            return 6;
        }
    default:
        return -1;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t ResolveFirstHelpHint() {
    return ResolveHelpHintForButton(0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void UpdateHelpIconHints() {
    std::int32_t forced = 0;
    if (GetShouldRenderIcons(ItemSelect) || IsItemSelectable()) {
        helpIconDrawVisible = helpIconPersistent.visible;
        helpIconDrawSelected = helpIconPersistent.selected;
    } else {
        helpIconDrawVisible = helpIconPersistent.visible | helpIconFrameOverlay.visible;
        helpIconDrawSelected = helpIconPersistent.selected | helpIconFrameOverlay.selected;
    }
    if (!authGetLoaded() && IsScreenFilmNotShowing(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask) &&
        (plymIsSearchable() || !GetParentMenu()))
        helpIconHudDelay--;
    else
        helpIconHudDelay = 5;
    qteForcedHelp = 0;
    if (helpIconDrawVisible & 0x100000U)
        qteForcedHelp = forced = 1;
    helpIconHintIds[0] = helpIconHintIds[1] = helpIconHintIds[2] = helpIconHintIds[3] = -1;
    if ((GetSleepActionIconFlags() & 3U) == 1 || (!IsCtrlSpeakIconInactive() && IsSpeakIconCancellable())) {
        helpIconDrawVisible |= 2U;
        helpIconDrawSelected &= ~2U;
    }
    std::int32_t hint0 = ResolveFirstHelpHint();
    if (hint0 < 0) {
        if (helpIconDrawVisible & 4U)
            hint0 = 26;
        if (iconSelectFlags & 0x400U) {
            hint0 = 21;
            helpIconDrawVisible |= 4U;
        }
    }
    std::int32_t hint1 = ResolveHelpHintForButton(1);
    if (hint1 < 0) {
        hint1 = 6;
        if (iconSelectFlags & 0x200U)
            helpIconDrawVisible |= 2U;
    }
    std::int32_t hint2 = ResolveHelpHintForButton(2);
    if (hint2 < 0) {
        if (helpIconDrawVisible & 16U)
            hint2 = 26;
        if (iconSelectFlags & 4U) {
            hint2 = 21;
            helpIconDrawVisible |= 16U;
        }
    }
    std::int32_t hint3 = ResolveHelpHintForButton(3);
    if (hint3 < 0 && (iconSelectFlags & 2U)) {
        hint3 = 21;
        helpIconDrawVisible |= 8U;
    }
    if (helpIconHudDelay < 0) {
        if (plymIsSearchable() && !IsScenarioBusy() && (GetSleepActionIconFlags() & 1) == 0 && IsCtrlSpeakIconInactive()) {
            if (!(helpIconDrawVisible & 4U) && !srchCheckFlags(6) && IsWristwatchOpenable()) {
                helpIconDrawVisible |= 4U;
                helpIconDrawSelected &= ~4U;
            }
            if (!(helpIconDrawVisible & 2U) && (srchCheckFlags(3) > 0 || srchCheckFlags(6))) {
                helpIconDrawVisible |= 2U;
                helpIconDrawSelected &= ~2U;
            }
        }
        if (helpIconDrawVisible & 4U) {
            if (hint0 >= 0) {
                helpIconHintIds[0] = helpHintIcons[hint0];
                helpIconHintAngle0 += 0x666U;
            }
        } else
            helpIconHintAngle0 = 0;
        if (helpIconDrawVisible & 2U) {
            if (hint1 >= 0) {
                helpIconHintIds[1] = helpHintIcons[hint1];
                helpIconHintAngle1 += 0x666U;
            }
        } else
            helpIconHintAngle1 = 0;
        if (helpIconDrawVisible & 16U) {
            if (hint2 >= 0 && !UpdateSelectorMemo()) {
                helpIconHintIds[2] = helpHintIcons[hint2];
                helpIconHintAngle2 += 0x666U;
            }
        } else
            helpIconHintAngle2 = 0;
        if (helpIconDrawVisible & 8U) {
            if (hint3 >= 0) {
                helpIconHintIds[3] = helpHintIcons[hint3];
                helpIconHintAngle3 += 0x666U;
            }
        } else
            helpIconHintAngle3 = 0;
    } else if (forced ||
        (!authGetLoaded() && IsScreenFilmNotShowing(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask) && !plymIsSearchable() &&
            GetParentMenu())) {
        if (helpIconDrawVisible & 4U) {
            if (hint0 >= 0) {
                helpIconHintIds[0] = helpHintIcons[hint0];
                helpIconHintAngle0 += 0x666U;
                forced = 1;
            }
        } else
            helpIconHintAngle0 = 0;
        if (helpIconDrawVisible & 2U) {
            if (hint1 >= 0) {
                helpIconHintIds[1] = helpHintIcons[hint1];
                helpIconHintAngle1 += 0x666U;
                forced = 1;
            }
        } else
            helpIconHintAngle1 = 0;
        if (helpIconDrawVisible & 16U) {
            if (hint2 >= 0) {
                helpIconHintIds[2] = helpHintIcons[hint2];
                helpIconHintAngle2 += 0x666U;
                forced = 1;
            }
        } else
            helpIconHintAngle2 = 0;
        if (helpIconDrawVisible & 8U) {
            if (hint3 >= 0) {
                helpIconHintIds[3] = helpHintIcons[hint3];
                helpIconHintAngle3 += 0x666U;
                forced = 1;
            }
        } else
            helpIconHintAngle3 = 0;
    }
    if (hint0 == 1 && g_scenarioTalkPromptResumed)
        helpIconHintAngle0 = 0;
    if (hint2 == 5 && !GetScenarioFlagNotificationShown())
        helpIconHintAngle2 = 0;
    if (hint3 == 2 && g_scenarioFollowPromptShown)
        helpIconHintAngle3 = 0;
    if (hint3 == 8 && !helpIconItemEffectState)
        helpIconHintAngle3 = 0;
    if (helpIconHintIds[0] < 0)
        helpIconDrawVisible &= ~4U;
    if (helpIconHintIds[1] < 0)
        helpIconDrawVisible &= ~2U;
    if (helpIconHintIds[2] < 0)
        helpIconDrawVisible &= ~16U;
    if (helpIconHintIds[3] < 0)
        helpIconDrawVisible &= ~8U;
    if (forced)
        helpIconHudDelay = -1;
    helpIconDrawVisible &= ~0x200U;
    helpIconPublishedVisible = helpIconDrawVisible;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::uint32_t GetHelpPulseColour(std::int32_t icon, std::uint32_t value) {
    std::uint32_t colour = value | (value << 8U) | (value << 16U);
    switch (icon) {
    case 1:
        colour = value;
        break;
    case 2:
    case 10:
        colour = value << 16U;
        break;
    case 3:
        colour = value << 8U;
        break;
    case 4:
        colour = (value << 8U) | (value << 16U);
        break;
    case 9:
        colour = (value >> 1U) | ((value >> 1U) << 8U) | ((value >> 1U) << 16U);
        break;
    }
    return colour;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void GetHelpButtonLayout(std::int32_t button, LegacyIconSprite &sprite, LegacyIconSprite &label) {
    if (static_cast<std::uint32_t>(button - 1) < 4) {
        sprite.x = (helpButtonPositions + button - 1)->x;
        sprite.y = (helpButtonPositions + button - 1)->y;
        label.icon = static_cast<std::uint16_t>(helpIconHintIds[helpButtonHints[button - 1]]);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void DrawHelpDirectionGlows(std::int32_t x, std::int32_t y, std::uint16_t visible) {
    static const std::int32_t offsets[4][2] = {{0, -12}, {0, 12}, {-12, 0}, {12, 0}};
    static const std::int32_t (*positions)[2];
    positions = offsets;
    LegacySpritePacket packet;
    do {
        packet.quad.flags = 0;
        packet.colour = 0xffffffffU;
        packet.secondaryColour = 0;
        packet.uv[0] = packet.uv[1] = 0.0f;
        packet.uv[2] = packet.uv[3] = 1.0f;
        packet.reservedTail = 0;
    } while (0);
    packet.quad.flags |= 0x400U;
    const HelpDirectionBlink *blink = helpDirectionBlinks;
    std::int16_t alpha = helpIconPulseValue * 4;
    if (alpha > 255)
        alpha = 255;
    const std::uint32_t pulseColour = (alpha << 24) | 0xc00000U;
    for (std::int32_t bit = 12; bit < 16; ++bit, ++positions, ++blink) {
        if ((visible >> bit) & 1) {
            if (blink->mode & 2U)
                packet.colour = pulseColour;
            else if (!blink->visible)
                continue;
            else
                packet.colour = 0xffc00000U;
            AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(5), x + (*positions)[0], y + (*positions)[1], 10, 10);
            DrawLegacyColourPacket(packet, 15.0f);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void DrawHelpItemPrompt(const LegacyIconSprite &source) {
    LegacyIconSprite sprite = source;
    sprite.icon = 0x60c;
    sprite.scaleX = sprite.scaleY = 1.7f;
    sprite.width = sprite.height = 32;
    sprite.y -= 15.0f;
    sprite.depth -= 1.0f;
    PrepareDrawWithMode(sprite);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void DrawHelpIconShadow(const LegacyIconSprite &source) {
    LegacyIconSprite sprite = source;
    sprite.icon = 12;
    sprite.depth -= 1.0f;
    sprite.width += 4;
    sprite.height += 4;
    PrepareDrawWithMode(sprite);
    sprite = source;
    sprite.icon = 12;
    sprite.depth -= 2.0f;
    sprite.x += 6.0f;
    sprite.y += 6.0f;
    PrepareDrawSimpleBlack(sprite, 0xbf000000U);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void DoQteDraw(void *) {
    static const std::uint16_t icons[12] = {0x801, 0x603, 0x601, 0x607, 0x605, 0x300, 0x305, 0x306, 0x304, 12, 0, 0};
    static const std::int16_t angles[4] = {0, 180, 270, 90};
    LegacyIconSprite sprite, label;
    std::int32_t index, show, hidden0, hidden1, hidden2, hidden3;
    std::uint32_t colour;
    std::int16_t pulse;
    float startX, offset;
    if (g_loadingFlag & 2U)
        helpIconRefreshDelay = 3;
    else
        helpIconRefreshDelay--;
    if (!(g_loadingFlag & 2U) && IsSaveAllowed()) {
        helpIconHintAngle0 += 0x666U;
        helpIconHintAngle1 += 0x666U;
    }
    offset = 0.0f;
    pulse = helpIconPulseValue;
    hidden0 = hidden1 = hidden2 = hidden3 = 0;
    startX = IsFightHealthUIActive() ? 140.0f : 32.0f;
    index = IsSaveAllowed() != 0;
    if (!IsScreenFilmNotShowing(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
        index |= 2;
    if (authGetLoaded() && !qteForcedHelp)
        index |= 4;
    if (index) {
        helpIconDrawVisible = 0;
        hidden0 = hidden1 = hidden2 = hidden3 = 1;
    }
    sprite.rotation = 0;
    sprite.flags = 0;
    sprite.alignment = 9;
    sprite.y = 457.0f;
    label = sprite;
    label.width = label.height = 32;
    for (index = 0; index < 12; index++) {
        sprite.scaleX = 1.0f;
        sprite.scaleY = 1.0f;
        sprite.depth = 3.0f;
        reinterpret_cast<std::int16_t &>(label.icon) = -1;
        show = 1;
        colour = 0xff000000U;
        colour |= GetHelpPulseColour(index, pulse);
        if (!(helpIconDrawVisible & (1U << index)))
            continue;
        if (helpIconDrawSelected & (1U << index))
            show = helpIconBlinkVisible;
        sprite.y = 457.0f;
        sprite.width = sprite.height = 38;
        sprite.x = startX + offset;
        switch (index) {
        case 1:
        case 2:
        case 3:
        case 4:
            GetHelpButtonLayout(index, sprite, label);
            break;
        case 10:
            sprite.x = 80.0f;
            sprite.width = 128;
            sprite.height = 64;
            break;
        case 0:
            sprite.y += 15.0f;
            sprite.width = sprite.height = 64;
        default:
            offset -= -48.0f;
            break;
        }
        sprite.icon = icons[index];
        if (show) {
            label.x = sprite.x;
            label.y = sprite.y - static_cast<float>((sprite.height - label.height) / 2) * sprite.scaleY;
            label.scaleX = sprite.scaleX;
            label.scaleY = sprite.scaleY;
            label.depth = sprite.depth + 1.0f;
            if (index == 2)
                label.scaleX *= dc::sdk::CosFixed(helpIconHintAngle0);
            if (index == 1)
                label.scaleX *= dc::sdk::CosFixed(helpIconHintAngle1);
            if (index == 4)
                label.scaleX *= dc::sdk::CosFixed(helpIconHintAngle2);
            if (index == 3)
                label.scaleX *= dc::sdk::CosFixed(helpIconHintAngle3);
            if (static_cast<std::int16_t>(label.icon) >= 0)
                PrepareDrawWithMode(label);
            PrepareDrawSimpleOpaque(sprite, colour);
            switch (index) {
            case 0:
                DrawHelpItemPrompt(sprite);
                break;
            case 5:
                DrawHelpDirectionGlows(static_cast<std::int32_t>(sprite.x),
                    static_cast<std::int32_t>(sprite.y - static_cast<float>(sprite.height) * sprite.scaleY * 0.5f),
                    static_cast<std::uint16_t>(helpIconDrawVisible));
                break;
            case 9:
                DrawHelpIconShadow(sprite);
                break;
            }
        }
    }
    if (helpIconHudDelay < 0) {
        sprite.width = 38;
        sprite.height = 38;
        sprite.alignment = 9;
        if (!hidden0) {
            sprite.x = 561.0f;
            sprite.y = 457.0f;
            sprite.icon = 0x600;
            PrepareDrawSimpleBlack(sprite, 0xa0000000U);
        }
        if (!hidden1) {
            sprite.x = 601.0f;
            sprite.y = 441.0f;
            sprite.icon = 0x602;
            PrepareDrawSimpleBlack(sprite, 0xa0000000U);
        }
        if (!hidden2) {
            sprite.x = 505.0f;
            sprite.y = 441.0f;
            sprite.icon = 0x604;
            PrepareDrawSimpleBlack(sprite, 0xa0000000U);
        }
        if (!hidden3) {
            sprite.x = 545.0f;
            sprite.y = 425.0f;
            sprite.icon = 0x606;
            PrepareDrawSimpleBlack(sprite, 0xa0000000U);
        }
    }
    sprite.height = sprite.width = 38;
    for (index = 16; index < 20; index++) {
        std::uint32_t mask;
        show = 1;
        sprite.icon = 0x800;
        sprite.rotation = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(angles[index - 16] * 8192) / 45.0f));
        mask = 1U << index;
        if (helpIconDrawVisible & mask) {
            if (helpIconDrawSelected & mask)
                show = helpIconBlinkVisible;
            if (show) {
                sprite.x = startX + offset;
                PrepareDrawWithMode(sprite);
                offset -= -48.0f;
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void NQ_CtrlButtonIcon(void *) {
    if (helpIconItemPromptFrames >= 0 && (--helpIconItemPromptFrames < 0 || UpdateSelectorMemo())) {
        helpIconItemPromptFrames = -1;
        CheckHelpIcon(0x200, 0x200, CheckHelpIconOp_Remove);
    }
    helpIconBlinkVisible = PulseHelpBlinkCounter(&helpIconBlinkFrames, 5, 3);
    if (!GetParentMenu())
        helpIconItemEffectState = 0;
    if (helpIconItemEffectState > 0) {
        if (--helpIconItemEffectFrames < 0) {
            helpIconItemEffectFrames = -1;
            helpIconItemEffectState = -1;
        }
    } else
        helpIconItemEffectFrames = -1;
    if (helpIconDeferredRemoval && !helpIconBlinkVisible) {
        helpIconPersistent.visible &= ~helpIconDeferredRemoval;
        helpIconPersistent.selected &= ~helpIconDeferredRemoval;
        helpIconDeferredRemoval = 0;
    }
    HelpDirectionBlink *blink = helpDirectionBlinks;
    for (std::int32_t slot = 0; slot < 4; ++slot, ++blink)
        blink->visible = PulseHelpBlinkCounter(&blink->remaining, blink->onFrames, blink->offFrames);
    if (helpIconRefreshDelay < 0)
        UpdateHelpIconHints();
    HLib::ExecuteCallbackNow(DoQteDraw, 0);
    helpIconPulseValue = static_cast<std::int16_t>(helpIconPulseValue + helpIconPulseStep);
    if (helpIconPulseValue < 1) {
        helpIconPulseValue = 0;
        helpIconPulseStep = 3;
    } else if (helpIconPulseValue > 62) {
        helpIconPulseValue = 63;
        helpIconPulseStep = -3;
    }
    helpIconFrameOverlay.selected = helpIconFrameOverlay.visible = 0;
    iconSelectFlags = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InitNowQte(std::int8_t queue) {
    g_quickIconSavedLoadingFlags = 0;
    helpIconItemPromptFrames = -1;
    helpIconItemEffectFrames = -1;
    helpIconItemEffectState = 0;
    helpIconPulseValue = 0;
    helpIconPulseStep = 3;
    helpIconBlinkFrames = 5;
    helpIconDeferredRemoval = 0;
    for (std::int32_t index = 0; index < 2; ++index) {
        reinterpret_cast<std::uint32_t *>(&helpIconPersistent)[index] = 0;
        reinterpret_cast<std::uint32_t *>(&helpIconFrameOverlay)[index] = 0;
    }
    iconSelectFlags = 0;
    helpIconPublishedVisible = 0;
    helpIconRemovalBlinkMask = 0.0f;
    helpIconHudDelay = 5;
    helpIconRefreshDelay = -1;
    nowQteTask = HLib::EnqueueTaskWithoutParameter(NQ_CtrlButtonIcon, queue, 0, SHEN_NAME(BTTN));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void BufferItemSelectorInput(std::uint32_t pressed) {
    const std::int16_t pending = pressed & 0x202U;
    if (pending >> 9) {
        if (GetItemSelectorTemporaryStatus(1))
            SetItemSelectorTemporaryStatus(1, 0);
        else if (GetItemSelectorTemporaryStatus(0))
            SetItemSelectorTemporaryStatus(0, 0);
    } else if (pending & 2U) {
        if (!GetItemSelectorTemporaryStatus(0))
            SetItemSelectorTemporaryStatus(0, pending);
        else if (!GetItemSelectorTemporaryStatus(1))
            SetItemSelectorTemporaryStatus(1, pending);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void MarkItemSelectorPlayerMoved() {
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (player)
        SetCharacterPositionDeltaApplied(player, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeItemSelectorWindow() {
    const std::int16_t edgeIcons[4] = {1025, 1026, 1027, 1028};
    itemEffectPosition.x = -672.0f;
    itemEffectPosition.y = 55.0f;
    itemEffectPosition.velocity = 1.0f;
    itemEffectPosition.reservedScale = 0.0f;
    itemEffectPosition.selectedDirection = 4;
    itemEffectPosition.depth = 3.0f;
    itemEffectLabelTexture = -1;
    InitializeLegacyIconSprite(itemWindowSelection);
    LegacyIconSprite *sprite = &itemWindowSelection;
    sprite->icon = 1029;
    sprite->depth = 8.0f;
    sprite->width = 38;
    sprite->height = 38;
    sprite = itemWindowEdges;
    const std::int16_t *icon = edgeIcons;
    for (std::int32_t count = 4; count; --count, ++sprite) {
        InitializeLegacyIconSprite(*sprite);
        if (*icon == 1026) {
            sprite->width = 40;
            sprite->height = 16;
        }
        sprite->depth = 5.0f;
        sprite->icon = *icon++;
    }
    sprite = &itemEffectSprite;
    InitializeLegacyIconSprite(*sprite);
    sprite->depth = 5.0f;
    sprite = &itemSelectorHeadingSprite;
    InitializeLegacyIconSprite(*sprite);
    sprite->depth = 5.0f;
    sprite->width = 80;
    sprite->height = 40;
    sprite->icon = 1544;
    const std::int8_t *rectangle = GetCommonIconRect(1547);
    INIT_LEGACY_SPRITE_PACKET(itemWindowTiles);
    itemWindowTiles.quad.flags |= 0x400U;
    itemWindowTiles.uv[0] = static_cast<float>(rectangle[0]) * 0.125f;
    itemWindowTiles.uv[1] = static_cast<float>(rectangle[1]) * 0.125f;
    itemWindowTiles.uv[2] = static_cast<float>(rectangle[2]) * 0.125f;
    itemWindowTiles.uv[3] = static_cast<float>(rectangle[3]) * 0.125f;
    rectangle = GetCommonIconRect(1026);
    INIT_LEGACY_SPRITE_PACKET(itemWindowSeparators);
    itemWindowSeparators.uv[0] = static_cast<float>(rectangle[0]) * 0.125f;
    itemWindowSeparators.uv[1] = static_cast<float>(rectangle[1]) * 0.125f;
    itemWindowSeparators.uv[2] = static_cast<float>(rectangle[2]) * 0.125f;
    itemWindowSeparators.uv[3] = static_cast<float>(rectangle[3]) * 0.125f;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawLegacyColourRectAspect(const LegacyColourRect &rect) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.colour = rect.colour;
    AdjustScreenAspectRatio(
        packet.quad, static_cast<ScreenAlignment>(5), static_cast<std::int32_t>(rect.x), static_cast<std::int32_t>(rect.y), rect.width, rect.height);
    packet.quad.flags |= 0x400U;
    DrawLegacyColourPacket(packet, rect.depth);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawLegacyColourRect(const LegacyColourRect &rect) {
    LegacySpritePacket packet;
    do {
        packet.quad.flags = 0;
        packet.colour = 0xffffffffU;
        packet.secondaryColour = 0;
        packet.uv[0] = packet.uv[1] = 0.0f;
        packet.uv[2] = packet.uv[3] = 1.0f;
        packet.reservedTail = 0;
    } while (0);
    packet.colour = rect.colour;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(rect.alignment), static_cast<std::int32_t>(rect.x), static_cast<std::int32_t>(rect.y),
        rect.width, rect.height, rect.scaleX, rect.scaleY, rect.rotation);
    packet.quad.flags |= 0x400U;
    DrawLegacyColourPacket(packet, rect.depth);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void shenSubmitQuadsUI(float x, float y, std::uint32_t fill, std::uint32_t border, std::int32_t width, std::int32_t height, float depth) {
    LegacySpritePacket packet;
    const float innerWidth = static_cast<float>(width - 4), innerHeight = static_cast<float>(height - 4);
    do {
        packet.quad.flags = 0;
        packet.colour = 0xffffffffU;
        packet.secondaryColour = 0;
        packet.uv[0] = packet.uv[1] = 0;
        packet.uv[2] = packet.uv[3] = 1;
        packet.reservedTail = 0;
    } while (0);
    packet.secondaryColour = packet.colour = fill;
    TransformScreenSpace(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>(y), static_cast<std::int32_t>(innerWidth),
        static_cast<std::int32_t>(innerHeight), 1, 1, 0);
    packet.quad.flags |= 0x400;
    DrawLegacyColourPacket(packet, depth);
    packet.secondaryColour = packet.colour = border;
    const float halfHeight = innerHeight * 0.5f;
    TransformScreenSpace(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>((y - halfHeight) - 1), width, 2, 1, 1, 0);
    DrawLegacyColourPacket(packet, depth + 1);
    TransformScreenSpace(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>((y + halfHeight) + 1), width, 2, 1, 1, 0);
    DrawLegacyColourPacket(packet, depth + 1);
    const float halfWidth = innerWidth * 0.5f;
    TransformScreenSpace(packet.quad, HorizontalStretch, static_cast<std::int32_t>((x - halfWidth) - 1), static_cast<std::int32_t>(y), 2, height, 1, 1, 0);
    DrawLegacyColourPacket(packet, depth + 1);
    TransformScreenSpace(packet.quad, HorizontalStretch, static_cast<std::int32_t>((x + halfWidth) + 1), static_cast<std::int32_t>(y), 2, height, 1, 1, 0);
    DrawLegacyColourPacket(packet, depth + 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t CheckMainIconItemSelection() {
    if (btnCheck(SHEN_NAME4('B', 'T', 'I', 'L')) && helpGetCurrentIconIdx(1) == 67) {
        UpdateSelectedItemStateFlag(14);
        return false;
    }
    if (!IsMainIconInputAdmitted())
        return false;
    std::uint32_t pressed;
    do {
        pressed = btnCheck(SHEN_NAME4('B', 'T', 'S', 'W'));
    } while (0);
    if (pressed)
        return true;
    RegisterButtonMediation(SHEN_NAME4('B', 'T', 'S', 'W'), 0, 0x80000);
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t CheckMainIconShortcut() {
    static const std::int32_t shortcuts[4][2] = {{532, 6025}, {533, 6026}, {534, 6018}, {535, 6017}};
    if (IsMainIconInputAdmitted()) {
        const std::int32_t (*shortcut)[2] = shortcuts;
        for (std::int32_t index = 0; index < 4; ++index, ++shortcut) {
            if (EV_GetShenmueFlag((*shortcut)[0])) {
                if (btnCheck(SHEN_NAME4('B', 'T', 'M', 'P'))) {
                    UpdateSelectedItemStateFlag(static_cast<std::int16_t>((*shortcut)[1]));
                    return 0;
                }
                RegisterButtonMediation(SHEN_NAME4('B', 'T', 'M', 'P'), 0, 0x40000);
                return 1;
            }
        }
        return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t IsMainIconInputAdmitted() {
    if (!nativeGuiActive || !plymIsSearchable() || plymIsSearchBlocked() || (g_loadingFlag & 2U) || GetShouldRenderIcons(Any) || IsScenarioBusyForMainIcon() ||
        g_fightUiActive)
        return 0;
    if (srchCheckFlags(6) || srchCheckFlags(3) > 0)
        return 0;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetMainIconState() {
    g_iconPads.current = 0;
    g_iconPads.previous = 0;
    mainIconPhase = 0;
    iconToggleStateFor(0, false);
    renderIconFlags = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitMainIcon_Callback(void *) {
    g_iconPads.current = AllocatePadControlEntry();
    HLib::SetCurrentTaskTo(CtrlMainIcon_Callback);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetItemSelectorClosingState() {
    mainIconPhase = 5;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t RequestTitleMenu() {
    if (!GetShouldRenderIcons(Any) && nativeGuiActive == 1) {
        BeginItemSelector();
        mainIconPhase = 3;
        return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static NativeItemSelectorContext *CaptureItemSelectorContext() {
    NativeItemSelectorContext &context = itemSelectorContext;
    context.perspective = static_cast<std::uint32_t>(camCtrlGetPerspectiveWrapper());
    context.cameraState = camCtrlGetState();
    camCtrlGetNextCamPos(&context.eye);
    camCtrlGetTargetPosition(&context.target);
    context.helpTopic = helpGetCurrentIconIdx(1);
    context.visible = static_cast<std::uint16_t>(GetUiFlag(0) & 0xfdffU);
    context.selected = static_cast<std::uint8_t>(GetUiFlag(1));
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (player) {
        context.playerDisplay = IsOfThisVisibilityType(player, CVM_DisplayEnabled);
        context.playerVisible = IsCharacterCollisionCallbackEnabled(player);
        return &context;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void RestoreItemSelectorContext(NativeItemSelectorState *state) {
    NativeItemSelectorContext *const context = static_cast<NativeItemSelectorContext *>(state->context);
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (context) {
        if (player) {
            EV_SetCharDisplay(player, CVM_DisplayEnabled, context->playerDisplay);
            charaSetVisibilityFlagTo(player, context->playerVisible);
        }
        camCtrlSetState(context->cameraState);
        camCtrlSetPerspectiveWrapper(static_cast<std::int16_t>(context->perspective));
        camCtrlSetNextPosition(context->eye);
        camCtrlSetTargetPosition(context->target);
        EV_EventSetUpHelpIcon_0(context->helpTopic, 0);
        SetHelpIconMenuFlags(context->visible, context->selected);
        if (!context->visible)
            ResetHelpIconsToItemPrompt();
    }
    state->context = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static std::int32_t GetItemSelectorSelectedItem(std::int32_t itemId) {
    std::int32_t found = 0;
    std::int32_t target = g_itemSelectorState.row * g_itemSelectorState.columns + g_itemSelectorState.column;
    std::int32_t index = 0;
    std::uint8_t *mask = g_itemSelectorOwnedMask;
    for (; index < 384; ++index) {
        if ((mask[index >> 3] >> (index & 7)) & 1) {
            if (target == found) {
                if (itemId == 1)
                    return GetInventoryItemId(0, index);
                return index;
            }
            ++found;
        }
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t RefreshItemSelectorInventory(std::uint32_t bank) {
    std::int32_t restricted = 0;
    const std::int32_t limit = static_cast<std::int32_t>(GetMaxItemsForItemType(bank));
    const std::uint8_t *stock = inventoryStockBanks[bank];
    std::int32_t index;
    for (index = 0; index < 64; ++index)
        g_itemSelectorOwnedMask[index] = 0;
    if (GetInternalFlag(0x56))
        restricted = 1;
    std::uint32_t count = 0;
    for (index = 0; index < limit; ++index, ++stock) {
        if (*stock && (!restricted || (GetInventoryItemFlags(bank, index) & 2U))) {
            g_itemSelectorOwnedMask[index / 8] |= static_cast<std::uint8_t>(1 << (index & 7));
            ++count;
        }
    }
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t icsToggleIconState(std::uint32_t index, std::int32_t enabled) {
    const std::uint32_t value = currentItemSelectState;
    if (index < 9) {
        if (enabled)
            currentItemSelectState = value | 1U << index;
        else
            currentItemSelectState &= ~(1U << index);
    }
    return value;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void speakSetFlagTo(std::uint16_t timeout) {
    ctrlSpeakTimeoutFrames = std::bit_cast<std::int16_t>(timeout);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsCtrlSpeakIconInactive() {
#else
bool IsCtrlSpeakIconInactive() {
#endif
    return ctrlSpeakIconState == 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsSpeakIconCancellable() {
    return ctrlSpeakMode == 2 || ctrlSpeakMode == 5;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void iconToggleStateFor(std::uint8_t slot, bool enabled) {
    if (slot == 0)
        activeIconFlags = enabled ? 0xffffffffU : 0;
    else if (enabled)
        activeIconFlags |= 1U << (slot & 31U);
    else
        activeIconFlags &= ~(1U << (slot & 31U));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t GetItemSelectionMode(std::uint8_t slot) {
    return activeIconFlags & (1U << (slot & 31U));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t RefreshItemSelectorInventory(std::uint32_t bank) noexcept {
    const std::span<const std::byte> stock = S2Save_GetInventoryBank(static_cast<S2InventoryBank>(bank));
    const std::uint32_t count = GetMaxItemsForItemType(bank);
    const bool filtered = GetInternalFlag(86);
    itemSelectorOwnedMask.fill(0);
    std::uint32_t owned = 0;
    for (std::uint32_t index = 0; index < count; ++index) {
        if (std::to_integer<std::uint8_t>(stock[index]) == 0 || (filtered && !(GetInventoryItemFlags(bank, index) & 2U)))
            continue;
        itemSelectorOwnedMask[index >> 3U] |= static_cast<std::uint8_t>(1U << (index & 7U));
        ++owned;
    }
    PublishItemSelectorOwnedMask(itemSelectorOwnedMask.data());
    return owned;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::span<const std::uint8_t, 64> GetItemSelectorOwnedMask() noexcept {
    return itemSelectorOwnedMask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void PublishItemSelectorOwnedMask(const std::uint8_t *mask) noexcept {
    itemSelectorPublishedMask = mask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
const std::uint8_t *GetPublishedItemSelectorOwnedMask() noexcept {
    return itemSelectorPublishedMask;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
std::uint32_t GetCurrentItemSelectState() {
    return currentItemSelectState;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t ConsumeItemMenuRequest() {
    if (GetParentMenu()) {
        const std::uint32_t request = g_itemSelectorState.actionResult;
        g_itemSelectorState.actionResult = 0;
        return request;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RefreshItemSelectorSelection() {
    const std::uint8_t *stock = g_keyItemStock;
    NativeItemSelectorState &state = g_itemSelectorState;
    state.count = 0;
    state.quantitySum = 0;
    for (std::int32_t index = 384; index != 0; --index, ++stock) {
        state.quantitySum = static_cast<std::int16_t>(static_cast<std::uint16_t>(state.quantitySum) + *stock);
        if (*stock)
            state.count = static_cast<std::int16_t>(static_cast<std::uint16_t>(state.count) + 1U);
    }
    state.count = static_cast<std::int16_t>(RefreshItemSelectorInventory(0));
    state.visibleRows = 1;
    state.columns = 8;
    while (state.visibleRows * state.columns < state.count) {
        if (++state.visibleRows > 5) {
            state.visibleRows = 1;
            ++state.columns;
        }
    }
    ClampItemSelectorSelection();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void ClampItemSelectorSelection() {
    NativeItemSelectorState &state = g_itemSelectorState;
    if (state.row + 1 > state.visibleRows) {
        for (;;) {
            if (state.row * state.columns <= state.count)
                break;
            if (--state.row < 0) {
                state.row = 0;
                break;
            }
        }
    }
    while (state.row * state.columns + state.column + 1 > state.count) {
        if (--state.column >= 0)
            continue;
        state.column = static_cast<std::int8_t>(state.columns - 1);
        if (--state.row >= 0)
            continue;
        state.row = 0;
        state.column = 0;
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
HLib::Task *QueueItemEffectIconTask(std::uint8_t queue) {
    itemEffectIconTask = HLib::EnqueueTaskWithoutParameter(CtrlItemEffectIcon_Callback, queue, 0, SHEN_NAME(IEET));
    if (itemEffectIconTask) {
        HLib::SetupTimedTask(itemEffectIconTask, 0);
        itemEffectIconPhase = 2;
    }
    return itemEffectIconTask;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool CloseItemEffect(std::int32_t force) {
    if (!GetShouldRenderIcons(UI_ICON(ItemEffectMessage)) || !force)
        return false;
    itemEffectIconPhase = 0;
    itemEffectPosition.x = -672.0f;
    itemEffectPosition.y = 55.0f;
    itemEffectLabelTexture = -1;
    iconToggleStateFor(5, false);
    ToggleRenderIcons(UI_ICON(ItemEffectMessage), false);
    if (itemEffectIconTask)
        HLib::SetupTimedTask(itemEffectIconTask, 0);
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsModeBitActive(std::int32_t mode) {
    return (static_cast<std::uint32_t>(mode) & currentItemSelectState) != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsAnyItemSelectActive() {
    return (GetCurrentItemSelectState() & 1U) != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t icsToggleIconState(std::uint32_t index, std::int32_t enabled) {
    const std::uint32_t previous = currentItemSelectState;
    if (index < 9) {
        if (enabled)
            currentItemSelectState |= 1U << index;
        else
            currentItemSelectState &= ~(1U << index);
    }
    return previous;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsItemSelectable() {
    return (GetCurrentItemSelectState() & 0x20U) != 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint16_t GetPadControlData(std::uint8_t mode) {
    if (!iconPadControl)
        return 0;
    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(iconPadControl);
    switch (mode) {
    case 0:
        return input->held;
    case 1:
        return input->pressedPriority;
    case 2:
        return input->repeatedPresses;
    default:
        return 0;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void padcRefreshCurrentInputs(bool bind) {
    if (!iconPadControl)
        return;
    if (bind) {
        if ((iconPadControl->flags & 2U) == 0)
            previousIconPadControl = BindPadControlEntry(iconPadControl);
    } else if ((iconPadControl->flags & 2U) != 0 && previousIconPadControl) {
        BindPadControlEntry(previousIconPadControl);
        previousIconPadControl = nullptr;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
static std::int64_t RetireIconInputSlot(std::uint32_t slot, std::int32_t force) {
    using enum CameraState;
    if (slot >= 8)
        return 0;
    if (slot == 0) {
        std::int64_t result = 0;
        for (std::uint32_t index = 1; index < 8; ++index)
            result |= RetireIconInputSlot(index, force);
        return result;
    }
    std::int64_t result = 0;
    switch (slot) {
    case 1:
        if (force && GetShouldRenderIcons(RenderIconSlot::ItemSelect) && !IsSaveAllowed()) {
            if (itemSelectTask)
                HLib::SetupTimedTask(itemSelectTask, 0);
            if (uniqueItemActionTask) {
                UniqueItemActionData *const action = static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
                if (action) {
                    if (action->cancel)
                        action->cancel(1);
                    ResetUniqueItemAction(*action);
                }
            }
            CloseWristwatch(1);
            itemSelectorDelayedResult = 0;
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            itemSelectorCameraActive = false;
            SelectPostFxBank(1, mainIconPostFxBank);
            mainIconPhase = 5;
            itemSelectorWindowPosition.x = -672.0f;
            itemSelectPhase = 0;
            iconToggleStateFor(1, false);
            ToggleRenderIcons(RenderIconSlot::ItemSelect, false);
            itemSelectorLabelTexture = -1;
            RestoreItemSelectorContext();
            result = 1;
        }
        break;
    case 2:
        result = CloseSpeakIcon(force);
        break;
    case 3:
        result = ResetQuickIconState(force != 0);
        break;
    case 4:
        result = AbortMoneyIcon(force);
        break;
    case 5:
        result = CloseItemEffect(force);
        break;
    case 7:
        if (force == 1) {
            ReinitTextureData();
            result = 1;
        }
        break;
    default:
        break;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t CloseHeldStateIcons() {
    const std::int64_t result = RetireIconInputSlot(0, 1);
    if (result && iconPadControl) {
        if ((iconPadControl->flags & 2U) != 0 && previousIconPadControl) {
            BindPadControlEntry(previousIconPadControl);
            previousIconPadControl = nullptr;
        }
        return (iconPadControl->flags & 2U) != 0;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int64_t CloseWristwatchTransitionIcons() {
    std::int64_t result = IsWristwatchOpenable();
    std::uint32_t closed = 0;
    if (result) {
        for (std::uint32_t slot = 2; slot < 8; ++slot) {
            if (slot == 6)
                continue;
            result = RetireIconInputSlot(slot, 1);
            closed |= static_cast<std::uint32_t>(result);
        }
    } else {
        result = RetireIconInputSlot(0, 1);
        closed = static_cast<std::uint32_t>(result);
    }
    if (closed && iconPadControl) {
        if ((iconPadControl->flags & 2U) != 0 && previousIconPadControl) {
            BindPadControlEntry(previousIconPadControl);
            previousIconPadControl = nullptr;
        }
        return (iconPadControl->flags & 2U) != 0;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t RefreshPadControlData(std::uint32_t slot, std::int32_t force) {
    const std::int64_t result = RetireIconInputSlot(slot, force);
    if (result && iconPadControl && (iconPadControl->flags & 2U) && previousIconPadControl) {
        BindPadControlEntry(previousIconPadControl);
        previousIconPadControl = nullptr;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
static void SetMoneyAnimationTarget(std::int32_t amount, std::int32_t mode) noexcept {
    const std::int32_t current = mode ? GetCurrentCash() : GetCurrentSegaCoins();
    const std::uint32_t wrapped = static_cast<std::uint32_t>(current) + static_cast<std::uint32_t>(amount);
    const std::int32_t requested = std::bit_cast<std::int32_t>(wrapped);
    const std::int32_t limit = mode ? 999999 : 99999999;
    moneyIconDrawMode = mode ? 2 : 0;
    moneyIconTarget = requested > limit ? limit : requested < 0 ? 0 : requested;
    moneyIconPhase = 2;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t AddThisToCashOrSegaCoins(std::int32_t amount, ECashOrCoins mode) noexcept {
    SetMoneyAnimationTarget(amount, static_cast<std::int32_t>(mode));
    const std::int64_t result = ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
    moneyIconFrames = 75.0f;
    moneyIconFlags = 0x14;
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t AddCashOrCoinsWithStep(std::int32_t amount, std::int32_t mode, std::int32_t step) noexcept {
    SetMoneyAnimationTarget(amount, mode);
    const std::int64_t result = ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
    moneyIconFrames = 75.0f;
    moneyIconFlags = 0x54;
    moneyIconFixedStep = step;
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t AnimateMoneyAmount(std::int32_t initial, std::int32_t target, std::int32_t modeIndex) noexcept {
    moneyIconFrames = 75.0f;
    moneyIconFlags = 0x34;
    moneyIconPhase = 2;
    moneyIconDrawMode = static_cast<std::uint8_t>(MONEY_ANIMATION_MODES[modeIndex]);
    const std::int32_t limit = moneyIconDrawMode ? 999999 : 99999999;
    moneyIconDisplay = initial >= limit ? limit : initial;
    moneyIconTarget = target >= limit ? limit : target;
    return ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int64_t, std::int32_t step, std::int32_t flags) noexcept {
    const std::int64_t result = GetShouldRenderIcons(RenderIconSlot::Any);
    if (result)
        return result;
    moneyIconFrames = 10800.0f;
    if (maximum <= 0)
        maximum = 1;
    moneyIconFlags = flags;
    moneyIconPhase = 2;
    if (maximum > GetCurrentCash())
        maximum = GetCurrentCash();
    if (step <= 0)
        step = 1;
    if (step > maximum)
        step = maximum;
    moneyIconStepAmount = step;
    // Native caller precondition: available cash is positive; no zero-divisor fallback.
    moneyIconMaximum = maximum - maximum % step;
    return ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t SetMoneyRelatedUIElement(std::int16_t maximum, std::int64_t unused, std::int16_t step) noexcept {
    unused = (unused & ~std::int64_t{0xffff}) | (unused & 0x7fff);
    return SetMoneyRelatedUIElement_0(maximum & 0x7fff, unused, step & 0x7fff, 15);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint64_t GetNumBet() noexcept {
    return static_cast<std::uint32_t>(moneyIconResult ? moneyIconResult : moneyIconAmount);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetMoneyHudCashAdjustment(std::int32_t amount) noexcept {
    moneyHudCashAdjustment = amount;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void KeepMoneyIconOpen() noexcept {
    moneyIconFlags &= ~0x10;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t AbortMoneyIcon(std::int32_t force) noexcept {
    if (!GetShouldRenderIcons(RenderIconSlot::MoneyMessage) || !force)
        return 0;
    iconToggleStateFor(4, false);
    ReleaseMoneyTextSlots();
    moneyIconPhase = 0;
    if (moneyIconFlags & 8)
        RestoreMoneyHelpState();
    moneyIconDrawMode = 2;
    moneyIconFlags = 0;
    ToggleRenderIcons(RenderIconSlot::MoneyMessage, false);
    moneyIconSelectionScale = 1.0f;
    moneyIconPosition.x = -516.0f;
    moneyIconAmount = 0;
    moneyIconPosition.selectedDirection = 4;
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CloseMoneyIcon() noexcept {
    moneyIconPhase = 4;
    moneyIconFrames = 0.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool OpenGameWatchMenuItem() {
    return OpenWristwatch();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CloseGameWatchMenuItem(std::int32_t immediate) {
    CloseWristwatch(immediate);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool OpenSaveGameMenuItem() {
    if (IsSaveAllowed() != 0 || !EV_CanDoAreaJump())
        return false;
    shenSetTimeSimulationTo(false);
    (void)StartShowScreenOfId(10);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CloseSaveGameMenuItem() {
    shenSetTimeSimulationTo(true);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool ToggleItemEffect(std::uint32_t itemId, std::int32_t amount) {
    if (GetShouldRenderIcons(RenderIconSlot::Any) || !itemEffectIconTask)
        return false;
    const char *const name = shenGetAmtOf(itemId);
    itemEffectName = name ? name : ITEM_EFFECT_MISSING_NAME;
    const std::size_t length = std::strlen(itemEffectName);
    itemEffectLabelTexture = -1;
    (void)PublishLegacyItemLabel(itemEffectName, 512);
    itemEffectLabelTexture = static_cast<std::int16_t>(commonItemTexture);
    if (itemEffectLabelTexture < 0)
        return false;
    const std::size_t cells = 14 * length / 40;
    std::uint8_t columns = static_cast<std::uint8_t>(cells + 1);
    if (length >= 12)
        ++columns;
    if (14 * length != 40 * cells)
        ++columns;
    itemEffectColumns = columns < 6 ? 6 : columns;
    itemEffectSprite.y = 55.0f;
    itemEffectPosition.y = 340.0f;
    itemEffectSprite.x = -672.0f;
    itemEffectPosition.x = -672.0f;
    HLib::ResolveDeferredCallback(itemEffectIconTask);
    itemEffectIconPhase = 2;
    iconToggleStateFor(5, true);
    itemEffectSprite.icon = GetInventoryItemIconById(itemId);
    ToggleRenderIcons(RenderIconSlot::ItemEffectMessage, true);
    itemEffectAmount = amount;
    SetGameMessageContent(0, static_cast<std::int16_t>(itemEffectSprite.icon), GetLocalizedSceneText(itemEffectName));
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool CloseItemEffect(std::int32_t force) {
    if (!GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage) || !force)
        return false;
    itemEffectPosition.x = -672.0f;
    itemEffectPosition.y = 55.0f;
    itemEffectLabelTexture = -1;
    itemEffectIconPhase = 0;
    iconToggleStateFor(5, false);
    ToggleRenderIcons(RenderIconSlot::ItemEffectMessage, false);
    if (itemEffectIconTask)
        HLib::SetupTimedTask(itemEffectIconTask, 0);
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool SetSleepActionIcon(std::int16_t flags) {
    if (GetShouldRenderIcons(RenderIconSlot::Any))
        return false;
    const std::int32_t requested = flags & 0x11b;
    if (requested != 0)
        sleepActionFlags = requested;
    if ((sleepActionFlags & 0x100) != 0)
        sleepActionFlags |= 0x10;
    if ((sleepActionFlags & 0x1b) == 0)
        return false;
    ToggleRenderIcons(RenderIconSlot::SleepAction, true);
    sleepActionPosition[0] = 180.0f;
    sleepActionScale = 0.0f;
    sleepActionPublishedSelection = -1;
    sleepActionStatus = 1;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ResetSleepActionIcon() {
    sleepActionPhase &= 0xff00U;
    sleepActionStatus &= 0xff00U;
    sleepActionSubmenuState = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ResumeSleepActionIconAfterSave() {
    if (sleepActionSubmenuState != 1)
        return;
    sleepActionPhase &= 0xff00U;
    sleepActionStatus = static_cast<std::uint16_t>((sleepActionStatus & 0xff00U) | 1U);
    sleepActionSubmenuState = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t GetSleepActionIconFlags() {
    const std::uint32_t shifted = static_cast<std::uint32_t>(sleepActionFlags) >> 7U;
    const std::uint8_t flags = static_cast<std::uint8_t>(sleepActionStatus | ((sleepActionFlags & 5) ? 4 : 0) | (~shifted & 2U));
    return (shifted & 0xffffff00U) | flags;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t GetSleepActionIconResult() {
    return sleepActionStatus >> 8U;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ResetSleepActionDefaultState() {
    sleepActionPhase = 0;
    sleepActionTargetSelection = 4;
    sleepActionStatus = 0;
    ToggleRenderIcons(RenderIconSlot::SleepAction, false);
    iconToggleStateFor(6, false);
    sleepActionFlags = 0x10f;
    sleepActionPublishedSelection = -1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
HLib::Task *QueueMainIconTask() {
    // Native GUI_Tasks_Enqueue clears g_pPadControlData_0 before queueing ICON.
    iconPadControl = nullptr;
    previousIconPadControl = nullptr;
    mainIconTask = HLib::EnqueueTaskWithoutParameter(InitMainIcon_Callback, 14, 0, TASK_MAIN_ICON.name);
    return mainIconTask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void QueueItemSelectTask(HLib::Task *parent) {
    itemSelectTask = HLib::QueueTaskAfterCurrentTask(parent, InitItemSelectIcon_Callback, 0x0f, 0, static_cast<std::int32_t>(TASK_ICIS.name));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void InitializeMoneyIconSprites() {
    moneyIconPosition = MONEY_POSITION_INITIAL;
    moneyIconSprite = {.x = -516.0f, .y = 340.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 2.0f, .icon = 1544, .width = 80, .height = 40, .alignment = 5};
    selectorCurrencySprite.x = -516.0f;
    selectorCurrencySprite.y = 350.0f;
    selectorCurrencySprite.scaleX = selectorCurrencySprite.scaleY = 1.0f;
    selectorCurrencySprite.depth = 3.0f;
    selectorCurrencySprite.flags = 0;
    selectorCurrencySprite.rotation = 0;
    selectorCurrencySprite.width = selectorCurrencySprite.height = 16;
    selectorCurrencySprite.alignment = 5;
    for (LegacyIconSprite &sprite : selectorNumberSprites) {
        sprite.x = -516.0f;
        sprite.y = 350.0f;
        sprite.scaleX = sprite.scaleY = 1.0f;
        sprite.depth = 3.0f;
        sprite.flags = 0;
        sprite.rotation = 0;
        sprite.icon = SELECTOR_DIGIT_ICONS[0];
        sprite.width = sprite.height = 16;
        sprite.alignment = 5;
    }
    InitializeLegacyQuantitySprites();
    for (std::size_t index = 0; index < moneyIconSigns.size(); ++index) {
        LegacyIconSprite &sign = moneyIconSigns[index];
        sign.scaleX = sign.scaleY = 1.0f;
        sign.depth = 3.0f;
        sign.flags = 0;
        sign.rotation = 0;
        sign.icon = static_cast<std::uint16_t>(index ? 1045 : 1051);
        sign.width = sign.height = 16;
        sign.alignment = 5;
    }
    moneyIconSelectionScale = 1.0f;
    moneyIconSelectorPosition = MONEY_SELECTOR_POSITION_INITIAL;
    for (LegacyIconSprite &sprite : moneyIconTextSprites)
        sprite = {.x = -176.0f, .y = 91.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 2.0f, .icon = 0xffffU, .width = 256, .height = 32, .alignment = 5};
    for (LegacyIconSprite &sprite : moneyIconDirections) {
        sprite.x = -176.0f;
        sprite.y = 91.0f;
        sprite.scaleX = sprite.scaleY = 1.0f;
        sprite.depth = 2.0f;
        sprite.rotation = 0;
        sprite.icon = 1544;
        sprite.width = 96;
        sprite.height = 50;
        sprite.alignment = 5;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void InitializeMoneyIconParameters() {
    moneyIconCurrency = 1;
    moneyIconDisplay = moneyIconTarget = moneyIconAmount = 0;
    InitializeMoneyIconSprites();
    moneyIconPhase = 0;
    moneyIconDrawMode = 2;
    moneyIconMaximum = 0;
    moneyIconStepAmount = 5;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void QueueMoneyIconTask(HLib::Task *parent) {
    InitializeMoneyIconParameters();
    moneyIconTask = HLib::QueueTaskAfterCurrentTask(parent, CtrlMoneyIcon_Callback, 8, 0, static_cast<std::int32_t>(TASK_ICMO.name));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void QueueQuickIconTask(HLib::Task *parent) {
    (void)HLib::QueueTaskAfterCurrentTask(parent, InitQuickIcon_Callback, 8, 0, static_cast<std::int32_t>(TASK_ICQU.name));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
HLib::Task *QueueItemEffectIconTask(std::uint8_t queue) {
    itemEffectIconTask = HLib::EnqueueTaskWithoutParameter(CtrlItemEffectIcon_Callback, queue, 0, static_cast<std::int32_t>(TASK_IEET.name));
    if (itemEffectIconTask) {
        HLib::SetupTimedTask(itemEffectIconTask, 0);
        itemEffectIconPhase = 2;
    }
    return itemEffectIconTask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
HLib::Task *CtrlSleepActionIcon(std::uint8_t nextFunctionIndex) {
    sleepActionPadControl = AllocatePadControlEntry();
    sleepActionPhase = 0;
    sleepActionTargetSelection = 4;
    sleepActionStatus = 0;
    ToggleRenderIcons(RenderIconSlot::SleepAction, false);
    iconToggleStateFor(6, false);
    sleepActionPosition = SLEEP_POSITION_INITIAL;
    sleepActionCurrentSelection = 4;
    for (std::size_t slot = 0; slot < 4; ++slot) {
        LegacyIconSprite &arrow = sleepActionArrowSprites[slot];
        LegacyIconSprite &text = sleepActionTextSprites[slot];
        arrow.flags = 0;
        arrow.icon = 0;
        arrow.width = arrow.height = 64;
        arrow.scaleX = arrow.scaleY = 1.2f;
        arrow.alignment = 5;
        text.icon = 0xffffU;
        text.flags = 0;
        text.width = 256;
        text.height = 32;
        text.scaleX = text.scaleY = 1.0f;
        text.rotation = 0;
        text.alignment = 5;
        arrow.x = static_cast<float>(SLEEP_ARROW_X[slot]) - 176.0f;
        arrow.y = static_cast<float>(SLEEP_ARROW_Y[slot]) + 118.0f;
        const float degrees = static_cast<float>(90 * (4 - static_cast<std::int32_t>(slot)));
        arrow.rotation = static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
        arrow.depth = SLEEP_ARROW_DEPTH[slot] + 1.0f;
        text.x = static_cast<float>(SLEEP_TEXT_X[slot]) - 176.0f;
        text.y = static_cast<float>(SLEEP_TEXT_Y[slot]) + 118.0f;
        text.depth = SLEEP_TEXT_DEPTH[slot] + 1.0f;
    }
    sleepActionFlags = 0;
    sleepActionPublishedSelection = -1;
    sleepActionIconTask = HLib::EnqueueTaskWithoutParameter(CtrlSleepActionIcon_Callback, nextFunctionIndex, 0, TASK_SLEEP_ACTION.name);
    return sleepActionIconTask;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t IsMainIconInputReady() {
    return mainIconDelay < 0 ? 1ULL : 0ULL;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t IsMainIconOverlayActive() {
    return mainIconOverlayActive;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void ResetMainIconState() {
    mainIconPhase = 0;
    activeIconFlags = 0;
    iconPadControl = nullptr;
    previousIconPadControl = nullptr;
    renderIconFlags = 0;
    mainIconDelay = 5;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
HLib::Task *GUI_Tasks_Enqueue() {
    ResetLegacyCollectionSpriteOwners();
    if (!commonIconAtlasCount)
        return nullptr;
    mainIconPhase = 0;
    mainIconDelay = 5;
    itemSelectPhase = 0;
    moneyIconPhase = 0;
    itemEffectIconPhase = 0;
    renderIconFlags = 0;
    activeIconFlags = 0;
    ClearGuiItemActions();

    HLib::Task *const main = QueueMainIconTask();
    nativeGuiActive = 1;
    (void)InitializeNativeGuiResources();
    QueueItemSelectTask(main);
    (void)initSCNT_item_data();
    QueueMoneyIconTask(main);
    (void)InitSpeakIcon(main, 8);
    QueueQuickIconTask(main);
    (void)InitNowQte(8);
    helpIconTask = HP_SetUpHelpIcon();
    tmx_CtrlHandWatch(3);
    MM_MemoCtrl(3);
    (void)NAVI_DummyWait_Enqueue(7);
    (void)QueueItemEffectIconTask(3);
    (void)CtrlSleepActionIcon(8);
    (void)CtrlGauge_Init();
    (void)NVW_CtrlNaviWindow_Enqueue();
    RegisterItems();
    (void)CreatePAWNShops();
    return Queue_pw_CtrlPawnWindow();
}
#endif


} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#undef INV_NOEXCEPT
#undef INV_NULL
#undef INV_INIT
#undef INV_ZERO
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_GiveItemOf(std::uint32_t item) {
    return shenmue::s2::GiveItemOf(item);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_RemoveInventoryItem(std::uint32_t item, std::uint32_t amount) {
    return shenmue::s2::RemoveInventoryItem(item, static_cast<std::uint8_t>(amount));
}

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_UpdateItemStateFlags(std::uint32_t item, std::uint32_t mask, std::int32_t enabled) {
    return shenmue::s2::UpdateItemStateFlags(item, static_cast<std::uint8_t>(mask), enabled);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#undef UI_ICON
#undef UI_NOEXCEPT
#undef UI_STRLEN
#endif
