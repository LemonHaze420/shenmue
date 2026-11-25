#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
#include "s2/gauge.hpp"
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/file_loader_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "d3t/texture.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_runtime.hpp"
#include "s2/archive_data.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
#include "s2/hlib.hpp"
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/legacy_sprite_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/item_wf.hpp"
#include "s2/legacy_collection.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/pause.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/money_icon.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/math.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/memo_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/easy_option_state_dc.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/model_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/item_selection_queries_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/event_matrix_dc.hpp"
#include <stdio.h>
#include <string.h>
#define UI_STRLEN ::strlen
#define UI_NOEXCEPT
#define UI_ICON(name) name
#include "s2/navigation.hpp"
#include "s2/navi_data_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/qron_dc.hpp"
#include "s2/texture_cache_dc.hpp"
#include "s2/event_overlay.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "platform/dc/screen.hpp"
#include <float.h>
#include <math.h>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
#include <cstring>
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "common/math.hpp"
#include "s2/character_dc.hpp"
#include "s2/window.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#define UI_ZERO
#include "s2/motion_rig.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/hand_motion.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "s2/directory_cache.hpp"
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
#include "s2/map_render_profile.hpp"
#include "s2/legacy_sprite_slot.hpp"
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
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/world_render.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <vector>
#endif

namespace shenmue {

namespace dc {

namespace sdk {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
void SetMirroredModelDrawMode(std::int32_t enabled);
#endif

} // namespace sdk

} // namespace dc

namespace s2 {

namespace {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
HLib::Task *gaugeTask{};
std::array<std::int32_t, 6> gaugeTextures;
static_assert(sizeof(gaugeTextures) == 0x18);
float playerPercent;
float enemyPercent;
std::int32_t gaugeLayout{};
#endif

} // namespace

} // namespace s2

} // namespace shenmue

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace shenmue
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/character_transparency_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"
#include "auto_test.hpp"
#include "s2/title_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/pawn.hpp"
#endif

namespace shenmue {

namespace s2 {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
typedef bool (*IconInputSlotClose)(std::int32_t force);
typedef bool (*IconInputSlotClose)(std::int32_t force);
typedef std::int32_t (*ItemWindowShape)(std::int32_t cell, std::int32_t columns);
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
void ga_CtrlGauge(void *);
void ReleaseGaugeTextures();
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace

bool OpenSelectedItemLook(std::int16_t item, std::int32_t mode);
bool OpenItemLook(std::int32_t item);
void ICSU_CtrlItemLook_Callback(void *);
static void icsuMoveRotateAdjustItemScale();
static void icsuRenderItem();
void CloseItemLook();
std::int32_t camCtrlSetPerspectiveWrapper(std::int16_t perspective);
std::int32_t camCtrlGetPerspectiveWrapper();

const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::uint8_t GetItemStateFlags(std::uint32_t item);
void ResetItemStateFlags();
const char *GetInventoryItemModel(std::uint32_t item);
void GetInventoryItemDistances(std::uint32_t item, float *minimum, float *maximum);
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t icsToggleIconState(std::uint32_t index, std::int32_t enabled);
std::uint32_t GetCurrentItemSelectState();
std::uint8_t helpGetCurrentIconIdx(std::int32_t slot);
void *SafeTryLoadFile(const char *directory, const char *filename, shen_name allocationName, std::uint32_t requiredHeapBytes);
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
HLib::Task *gaugeTask = 0;
std::int32_t gaugeTextures[6];
float gaugePercent[2];
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
std::uint32_t gaugeFlags;
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int32_t gaugeLayout;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Fight-widget setup owns these independently of the legacy GAGE task globals.
d3t::UIDefinition *fightDefinition{};
std::array<std::int32_t, 3> fightWidgetIndices{-1, -1, -1};
std::int32_t fightComboDisplayWindow{-1};
std::int32_t fightComboLabel{};
std::int32_t fightComboDisplayTransition{};
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

d3t::UIGaugeState *FightWidget(std::size_t slot) {
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
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace

void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
static MemoModelDrawContext itemLookDraw;
std::uint8_t helpGetCurrentIconIdx(std::int32_t slot);
void *SafeTryLoadFile(const char *directory, const char *filename, shen_name allocationName, std::uint32_t requiredHeapBytes);
void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
void DrawReflectorBoardContext(void *parameter);
void ReleaseNamedTexture_Wrapper(std::int32_t index);
std::uint32_t GetUiFlag(std::uint8_t mode);
void AddHelpIconMenuFlags(std::uint16_t flags, std::uint8_t selected);
void SetHelpIconMenuFlags(std::uint16_t flags, std::uint8_t selected);
void ResetHelpIconsToItemPrompt();
std::uint16_t GetItemSelectorTemporaryStatus(std::int8_t index);
void SetItemSelectorTemporaryStatus(std::int8_t index, std::int16_t status);

// @verified - d
void ICSU_InitItemLook_Callback(void *) {
    g_itemLookState.phase = 0;
    g_itemLookState.pad = 0;
    g_itemLookState.pad = AllocatePadControlEntry();
    g_itemLookState.root = 0;
    g_itemLookState.model = 0;
    g_itemLookState.lightYaw = 0;
    g_itemLookShortcutRequest = 0;
    HLib::SetCurrentTaskTo(ICSU_CtrlItemLook_Callback);
}


// @verified - d
void UpdateSelectedItemStateFlag(std::int16_t item) {
    if (GetShouldRenderIcons(Any) || IsAnyItemSelectActive())
        return;
    if (OpenSelectedItemLook(item, 0)) {
        g_itemLookShortcutRequest = 1;
        ExecuteAicaCommand(0x500ab, 0, 0);
    }
}


bool OpenSelectedItemLook(std::int16_t item, std::int32_t) {
    return OpenItemLook(item);
}


// @verified - d
bool OpenItemLook(std::int32_t item) {
    const char *const model = GetInventoryItemModel(static_cast<std::uint32_t>(item));
    Filepath_Gen(FilepathGenMode::ItemDirectory);
    if (!model)
        return false;
    char filename[32];
    if (hasItemRegistered(static_cast<std::uint32_t>(item)) & 0x20U)
        ::sprintf(filename, "%sG.MT7", model);
    else
        ::sprintf(filename, "%sI.MT7", model);
    g_itemLookState.selectedItem = static_cast<std::uint32_t>(item);
    g_itemLookState.phase = 2;
    g_itemLookState.itemFlags = hasItemRegistered(static_cast<std::uint32_t>(item));
    icsToggleIconState(0, 1);
    return true;
}


// @verified - d
bool IsAnyItemSelectActive() {
    if (GetCurrentItemSelectState() & 1U)
        return true;
    return false;
}


// @verified - d
static bool LoadItemModel(NativeItemLookState *look) {
    const char *const model = GetInventoryItemModel(look->selectedItem);
    const char *const directory = Filepath_Gen(FilepathGenMode::ItemDirectory);
    if (model) {
        char filename[32];
        if (look->itemFlags & 0x20U)
            ::sprintf(filename, "%sG.MT7", model);
        else
            ::sprintf(filename, "%sI.MT7", model);
        void *const file = SafeTryLoadFile(directory, filename, SHEN_NAME(ITEM), 0);
        if (!file)
            return false;
        look->model = InitializeMT7Model(file);
        if (!look->model)
            return false;
        look->root = look->model->root;
        if (!look->root)
            return false;
        return true;
    }
    return false;
}


static void CalculateItemLookSinCos(std::int32_t angle, float *sine, float *cosine) {
    *sine = dc::sdk::njSin(angle);
    *cosine = dc::sdk::njCos(angle);
}


static void SetupItemInspectTransform(NativeItemLookState *look) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const player = control->ryo;
    if (!player)
        return;
    GetInventoryItemDistances(look->selectedItem, &look->minimumScale, &look->maximumScale);
    look->scale = (look->minimumScale + look->maximumScale) * 0.5f;
    std::int32_t yaw;
    yaw = -GetCharacterMotionWorldYaw(player) & 0xffff;
    Vector3 position;
    GetCharacterWorldPosition(player, &position);
    look->eventPosition = position;
    look->lightYaw = 0;
    const float viewOffset = 0.14999999105930328f;
    look->root->rotation.x &= 0xffff;
    look->root->rotation.y &= 0xffff;
    look->root->rotation.z &= 0xffff;
    g_itemLookRotation.x = g_itemLookRotation.y = g_itemLookRotation.z = 0;
    if (look->itemFlags & 8U) {
        Vector3 view;
        UnprojectViewPointToWorld(0.0f, 0.0f, -viewOffset, &view);
        look->eventPosition.x = view.x;
        look->eventPosition.z = view.z;
        look->eventPosition.y = view.y;
        look->worldYaw = -camCtrlGetNextRotY();
        look->lightYaw = 0;
        g_itemLookRotation.x = 0xeaab;
        look->scale = 0.94f;
        return;
    }
    if (!(look->itemFlags & 4U)) {
        g_itemLookRotation.y = 0x1555;
        g_itemLookRotation.x = -3640;
    }
    look->worldYaw = static_cast<std::uint16_t>(-yaw);
    look->eventPosition.y += 1.6f;
    float sine, cosine;
    CalculateItemLookSinCos(yaw + 0x8000, &sine, &cosine);
    look->eventPosition.x += sine * viewOffset;
    look->eventPosition.z += cosine * -viewOffset;
    position.y += 1.6f;
    position.x += sine * 0.35f;
    position.z += cosine * -0.35f;
    camCtrlSetNextPosition(position);
}


// @verified - d
void ICSU_CtrlItemLook_Callback(void *) {
    if (g_loadingFlag & 2U) {
        icsuRenderItem();
        return;
    }
    switch (static_cast<std::int16_t>(g_itemLookState.phase)) {
    case 0:
        break;
    case 2:
        if (LoadItemModel(&g_itemLookState)) {
            if (g_itemLookState.pad && !IsPadControlEntryBound(g_itemLookState.pad))
                g_itemLookState.previousPad = BindPadControlEntry(g_itemLookState.pad);
            g_itemLookState.phase = 1;
            camCtrlGetNextCamPos(&g_itemLookState.eye);
            camCtrlGetTargetPosition(&g_itemLookState.target);
            g_itemLookState.cameraState = camCtrlGetState();
            g_itemLookState.perspective = camCtrlGetPerspectiveWrapper();
            g_itemLookHelpTopic = helpGetCurrentIconIdx(1);
            std::uint8_t &helpMode = g_itemLookHelpMode;
            helpMode = helpGetCurrentIconIdx(0);
            g_itemLookState.uiSelected = static_cast<std::uint16_t>(GetUiFlag(1));
            g_itemLookState.uiVisible = static_cast<std::uint16_t>(GetUiFlag(0));
            Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
            if (player) {
                g_itemLookState.playerDisplay = IsOfThisVisibilityType(player, CVM_DisplayEnabled);
                g_itemLookState.playerVisible = IsCharacterCollisionCallbackEnabled(player);
                EV_SetCharDisplay(player, CVM_DisplayEnabled, 0);
                charaSetVisibilityFlagTo(player, 0);
            }
            camCtrlSetState(9);
            std::int32_t helpFlags = 0x21;
            camCtrlSetPerspectiveWrapper(0x2000);
            EV_EventSetUpHelpIcon_0(0x12, 0);
            g_itemLookState.uiMode = static_cast<std::int16_t>(GetUiFlag(2));
            if (g_itemLookState.uiMode)
                SetHelpIconMenuFlags(helpFlags);
            else
                AddHelpIconMenuFlags(helpFlags);
            SetupItemInspectTransform(&g_itemLookState);
            camCtrlSetTargetPosition(g_itemLookState.eventPosition);
        } else {
            g_itemLookState.phase = 0;
            ExecuteAicaCommand(0x800ab, 0, 0);
            SetItemSelectorDelayedResult(0);
            icsToggleIconState(0, 0);
        }
        break;
    case 1:
        icsuMoveRotateAdjustItemScale();
        icsuRenderItem();
        break;
    }
    ResetItemStateFlags();
}


static void icsuMoveRotateAdjustItemScale() {
    std::int32_t item, step;
    std::uint32_t prompt, accept;
    std::int32_t *yaw;
    float *rootY;
    prompt = 0x200;
    accept = 0;
    item = static_cast<std::int16_t>(g_itemLookState.selectedItem);
    const float minimum = g_itemLookState.minimumScale;
    float zoom = g_itemLookState.maximumScale - minimum;
    zoom *= 0.02f;
    yaw = &g_itemLookRotation.y;
    step = 256;
    rootY = 0;
    std::uint16_t held = ctrlGetPadInputBuffer0(g_itemLookState.pad)->held;
    std::uint16_t pressed = ctrlGetPadInputBuffer0(g_itemLookState.pad)->pressedPriority;
    if (g_itemLook->itemFlags & 4U) {
        do {
            yaw = &g_itemLookRotation.z;
        } while (0);
        step = -256;
        held &= 0xcfffU;
    }
    pressed |= GetItemSelectorTemporaryStatus(1);
    SetItemSelectorTemporaryStatus(1, 0);
    if (g_itemLookShortcutRequest)
        prompt |= 0x400U;
    else if (GetItemStateFlags(static_cast<std::uint32_t>(item)) & 2U)
        accept |= 0x400U;
    RequestHelpIconSelection(prompt | accept);
    if (pressed & accept) {
        ExecuteAicaCommand(0x500ab, 0, 0);
        CloseItemLook();
        SetItemSelectorDelayedResult(2);
        return;
    }
    if (pressed & prompt) {
        if (g_itemLookShortcutRequest)
            ExecuteAicaCommand(0x100ab, 0, 0);
        CloseItemLook();
        return;
    }
    if (held & 0xf000U) {
        if (held & 0x1000U)
            g_itemLookRotation.x -= step;
        if (held & 0x2000U)
            g_itemLookRotation.x += step;
        if (rootY) {
            if (held & 0x8000U) {
                *rootY -= 0.01f;
                if (!(*rootY > -0.14f))
                    *rootY = -0.14f;
            }
            if (held & 0x4000U) {
                *rootY += 0.01f;
                if (!(0.21f > *rootY))
                    *rootY = 0.21f;
            }
        } else {
            if (held & 0x8000U)
                *yaw += step;
            if (held & 0x4000U)
                *yaw -= step;
        }
    }
    g_itemLookState.root->rotation.x &= 0xffff;
    g_itemLookState.root->rotation.y &= 0xffff;
    g_itemLookState.root->rotation.z &= 0xffff;
    if (held & 0x80U) {
        float limit = g_itemLookState.maximumScale;
        const MT7MeshHeader *const mesh = static_cast<MT7MeshHeader *>(g_itemLookState.root->mesh);
        if (mesh) {
            const float radius = mesh->radius;
            const float fit = 0.1f / radius;
            if (!(fit > limit))
                limit = fit;
        }
        g_itemLookState.scale += zoom;
        if (!(limit > g_itemLookState.scale))
            g_itemLookState.scale = limit;
    } else if (held & 0x40U) {
        g_itemLookState.scale -= zoom;
        if (!(g_itemLookState.scale > g_itemLookState.minimumScale))
            g_itemLookState.scale = g_itemLookState.minimumScale;
    }
}


// @verified - d
static void icsuRenderItem() {
    if (!g_itemLookState.root)
        return;
    Vector3i rotation = g_itemLookRotation;
    if (g_itemLook->itemFlags & 8U) {
        rotation.x = -rotation.x;
        rotation.y += 0x8000;
    }
    rotation.z = g_itemLookRotation.z;
    itemLookDraw.root = g_itemLookState.root;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(camCtrlGetViewMatrix(0));
    Matrix4x4::preTranslate(0.0f, 0.0f, -0.2f);
    Matrix4x4::RotateAroundZ_Wrapper(rotation.z);
    Matrix4x4::RotateAroundY_Wrapper(rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(rotation.x);
    matrix_stack::GetTranslation(&itemLookDraw.position);
    Matrix4x4 transform;
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    ExtractMatrixEulerAnglesFixed(&transform, &itemLookDraw.rotation);
    itemLookDraw.scale = g_itemLookState.scale;
    itemLookDraw.style = g_itemLookState.lightYaw;
    itemLookDraw.colour.x = 2.0f;
    itemLookDraw.colour.y = 0.5f;
    itemLookDraw.colour.z = 0.5f;
    itemLookDraw.colour.w = 0.5f;
    HLib::ExecuteCallbackNow(DrawReflectorBoardContext, &itemLookDraw);
}


// @verified - d
void CloseItemLook() {
    if (g_itemLookState.model) {
        ReleaseMT7Model(g_itemLookState.model);
        g_itemLookState.model = 0;
        g_itemLookState.root = 0;
    }
    SetItemSelectorDelayedResult(0);
    if (!IsAnyItemSelectActive())
        return;
    EV_EventSetUpHelpIcon_0(g_itemLookHelpTopic, g_itemLookHelpMode);
    if (g_itemLookState.uiMode)
        SetHelpIconMenuFlags(g_itemLookState.uiVisible, static_cast<std::uint8_t>(g_itemLookState.uiSelected));
    else
        ResetHelpIconsToItemPrompt();
    g_itemLookShortcutRequest = 0;
    camCtrlSetPerspectiveWrapper(static_cast<std::int16_t>(g_itemLookState.perspective));
    camCtrlSetState(g_itemLookState.cameraState);
    camCtrlSetNextPosition(g_itemLookState.eye);
    camCtrlSetTargetPosition(g_itemLookState.target);
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (player) {
        EV_SetCharDisplay(player, CVM_DisplayEnabled, g_itemLookState.playerDisplay);
        charaSetVisibilityFlagTo(player, g_itemLookState.playerVisible);
    }
    if (g_itemLookState.pad && IsPadControlEntryBound(g_itemLookState.pad))
        BindPadControlEntry(g_itemLookState.previousPad);
    g_itemLookState.phase = 0;
    icsToggleIconState(0, 0);
}


bool CloseItemLookInputSlot(std::int32_t force) {
    if (!IsAnyItemSelectActive() || !force)
        return false;
    CloseItemLook();
    return true;
}


// @verified - d
void CtrlGauge_Init() {
    for (std::int32_t index = 0; index < 6; ++index)
        gaugeTextures[index] = -1;
    gaugeTask = 0;
    gaugePercent[0] = gaugePercent[1] = 0.0f;
    gaugeFlags = 1;
    gaugeLayout = 0;
}


// @verified - d
std::int32_t LoadGauge() {
    HLib::Task **task = &gaugeTask;
    if (!*task) {
        shen_name name;
        const std::uint32_t size = shenOpenFile("sprite", "GAUGE.SPR");
        name = TASK_FIGHT_GAUGE;
        void *const allocation = shenLoadFile("sprite", "GAUGE.SPR", name);
        if (!allocation)
            return 0;
        do {
            ReadTitleSpriteTextures(allocation, size, gaugeTextures);
            HLib::FreeTaskData(allocation);
        } while (0);
        *task = HLib::EnqueueTaskWithoutParameter(ga_CtrlGauge, 8, 0, static_cast<std::int32_t>(name));
        if (*task) {
            gaugeFlags |= 2U;
            return 1;
        }
        ReleaseGaugeTextures();
    }
    return 0;
}


// @verified - d
std::int32_t ReleaseGauge() {
    if (gaugeTask) {
        HLib::DestroyTaskInternal(gaugeTask);
        ReleaseGaugeTextures();
        CtrlGauge_Init();
        return 1;
    }
    return 0;
}


// @verified - d
std::int32_t SetFightGaugeVisible(std::int32_t visible) {
    std::int32_t previous;
    if (gaugeFlags & 2U)
        previous = 1;
    else
        previous = 0;
    if (visible)
        gaugeFlags |= 2U;
    else
        gaugeFlags &= ~2U;
    return previous;
}


// @verified - d
float UpdateDualFightGauge(float player, float enemy) {
    float result = 0.0f;
    gaugePercent[0] = player;
    gaugePercent[1] = enemy;
    gaugeFlags |= 1U;
    return result;
}


// @verified - d
void UpdateSingleFightGauge(float percent) {
    gaugePercent[0] = percent;
    gaugePercent[1] = 0.0f;
    gaugeFlags &= ~1U;
}


// @verified - d
bool IsFightHealthUIActive() {
    if (gaugeTask && (gaugeFlags & 2U))
        return true;
    return false;
}


// @verified - d
std::int32_t SetFightGaugeLayout(std::int32_t layout) {
    const std::int32_t previous = gaugeLayout;
    if (layout < 6)
        gaugeLayout = layout;
    return previous;
}
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
inline void InitializeGaugePacket(LegacySpritePacket &packet) {
    INIT_LEGACY_SPRITE_PACKET(packet);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
void DrawSingleFightGaugeOrbRow() {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const float (&base)[2] = FIGHT_GAUGE_BASE_POSITIONS[gaugeLayout];
#endif
    LegacySpritePacket packet;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    InitializeGaugePacket(packet);
    packet.quad.flags |= 0x400U;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    packet.quad.flags = 0x400;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    const std::array<float, 2> &base = FIGHT_GAUGE_BASE_POSITIONS[gaugeLayout];
#endif
    float threshold = 8.0f;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    for (std::int32_t orb = 0; orb < 12 && gaugePercent[0] > threshold; ++orb) {
        std::int32_t texture;
        if (orb <= 7)
            texture = 5;
        else if (orb <= 10)
            texture = 2;
        else
            texture = 4;
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(base[0] + FIGHT_GAUGE_ORB_POSITIONS[orb][0]),
            static_cast<std::int32_t>(base[1] + FIGHT_GAUGE_ORB_POSITIONS[orb][1]), 16, 16);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    for (std::size_t orb = 0; orb < FIGHT_GAUGE_ORB_POSITIONS.size(); ++orb) {
        if (threshold >= playerPercent)
            break;
        const std::int32_t texture = orb < 8 ? 5 : orb < 11 ? 2 : 4;
        const float y = (static_cast<float>(d3t::g_originalScreenHeight) * 0.5f - (base[1] + base[1])) + FIGHT_GAUGE_ORB_POSITIONS[orb][1];
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(base[0] + FIGHT_GAUGE_ORB_POSITIONS[orb][0]),
            TruncateSpriteCoordinate(y), 16, 16, static_cast<ScreenMode>(0));
#endif
        if (gaugeTextures[texture] != -1)

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
            DrawLegacySpritePacket(packet, gaugeTextures[texture], 6.0f);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
            LegacySpriteNullsub(packet, gaugeTextures[texture], 6.0f);
#endif
        threshold += 8.333333f;
    }
}


// @verified - d
void CtrlGauge_Callback(void *) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const float (&base)[2] = FIGHT_GAUGE_BASE_POSITIONS[gaugeLayout];
    if (!(gaugeFlags & 2U))
        return;
    LegacySpritePacket packet;
    InitializeGaugePacket(packet);
    packet.quad.flags |= 0x400U;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(base[0]), static_cast<std::int32_t>(base[1]), 128, 128);
    DrawLegacySpritePacket(packet, gaugeTextures[0], 3.0f);
    InitializeGaugePacket(packet);
    packet.quad.flags |= 0x400U;
    for (std::int32_t orb = 0; orb < 12; ++orb) {
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(base[0] + FIGHT_GAUGE_ORB_POSITIONS[orb][0]),
            static_cast<std::int32_t>(base[1] + FIGHT_GAUGE_ORB_POSITIONS[orb][1]), 16, 16);
        if (gaugeTextures[3] != -1)
            DrawLegacySpritePacket(packet, gaugeTextures[3], 5.0f);
    }
    if (gaugeFlags & 1U) {
        for (std::int32_t pass = 0; pass < 2; ++pass) {
            float threshold = 8.0f;
            const std::int32_t texture = FIGHT_GAUGE_DUAL_TEXTURES[pass];
            for (std::int32_t orb = 0; orb < 12 && gaugePercent[pass] > threshold; ++orb) {
                AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(base[0] + FIGHT_GAUGE_ORB_POSITIONS[orb][0]),
                    static_cast<std::int32_t>(base[1] + FIGHT_GAUGE_ORB_POSITIONS[orb][1]), 16, 16);
                if (gaugeTextures[texture] != -1)
                    DrawLegacySpritePacket(packet, gaugeTextures[texture], static_cast<float>(pass) + 6.0f);
                threshold += 8.333333f;
            }
        }
    } else
        DrawSingleFightGaugeOrbRow();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif
}


// @verified - d
void ga_CtrlGauge(void *) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    HLib::ExecuteCallbackNow(CtrlGauge_Callback, 0);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    HLib::ExecuteCallbackNow(CtrlGauge_Callback, nullptr);
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
void ReleaseGaugeTextures() {
    for (std::int32_t index = 0; index < 6; ++index) {
        if (gaugeTextures[index] != -1)
            ReleaseNamedTexture_Wrapper(gaugeTextures[index]);
        gaugeTextures[index] = -1;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetFilledOrbCount(d3t::UIGaugeState &widget, float percent) {
    const float count = (percent / 100.0f) * static_cast<float>(widget.count);
    const std::int32_t whole = TruncateSpriteCoordinate(count);
    widget.filledCount = whole;
    if (count - static_cast<float>(whole) > 0.0f && whole < widget.count)
        widget.filledCount = whole + 1;
}
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace

// @verified - d
void ReleaseWfResources(NativeWfTaskData &wf) {
    EV_NPC_NewLoadSw(wf.npcLoadState);
    if (wf.ptx)
        HLib::FreeTaskData(wf.ptx);
    wf.ptx = 0;
    for (std::int32_t index = 0; index < 6; ++index) {
        if (wf.textures[index] != -1)
            ReleaseNamedTexture_Wrapper(wf.textures[index]);
        wf.textures[index] = -1;
    }
}


const char *GetSpecialWfScrollLabel() {
    const NativeWfScrollDefinition *definition = wfSpecialScrollDefinitions;
    for (std::int32_t index = 0; index < 2; ++index, ++definition) {
        if (GetItemState(static_cast<std::uint32_t>(3000 + definition->itemOffset)))
            return shenGetAmtOf(static_cast<std::uint32_t>(3000 + definition->itemOffset));
    }
    return "\241\251\241\251\241\251\241\251";
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

std::int64_t CtrlGauge_Init() {
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

bool IsFightHealthUIActive() {
    return gaugeTask && (gaugeFlags & 2U) != 0;
}

std::int64_t SetFightGaugeVisible(std::int32_t visible) {
    const std::int64_t previous = (gaugeFlags >> 1U) & 1U;
    if (visible)
        gaugeFlags |= 2U;
    else
        gaugeFlags &= ~2U;
    return previous;
}

// @verified - p
std::int64_t SetFightGaugeLayout(std::int32_t layout) {
    const std::int32_t previous = gaugeLayout;
    if (layout < 6)
        gaugeLayout = layout;
    return static_cast<std::uint32_t>(previous);
}

// @verified - p
double UpdateDualFightGauge(float player, float enemy) {
    gaugeFlags |= 1U;
    playerPercent = player;
    enemyPercent = enemy;
    if (GetDebugForceUIEnabled(UIElement::FightGauge))
        SetPlayerEnemyPercents(player, enemy);
    return 0.0;
}

d3t::UIGaugeState *UpdateSingleFightGauge(float percent) {
    gaugeFlags &= ~1U;
    enemyPercent = 0.0f;
    playerPercent = percent;
    return GetDebugForceUIEnabled(UIElement::FightGauge) ? SetFightGaugePercentSingle(percent) : nullptr;
}

void BindFightGaugeDefinition(d3t::UIDefinition &definition) {
    fightDefinition = &definition;
    fightWidgetIndices[0] = definition.gauges.empty() ? -1 : 0;
    fightWidgetIndices[1] = definition.gauges.size() < 2 ? -1 : 1;
    const std::uint32_t viewSlot = static_cast<std::uint32_t>(g_uiElements[static_cast<std::size_t>(UIElement::FightGauge)].config.uiHandleIndex);
    if (const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(viewSlot))
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget)
            if (view->widgetKinds[widget] == 15)
                fightComboLabel = view->widgetPoolIndices[widget];
    fightComboDisplayWindow = UI_FindViewWindow(viewSlot, "HUDGauge_ComboDisp");
    fightComboDisplayTransition = UIWidget_Bind(fightComboDisplayWindow, 0, 0, {0.0f, 0.0f}, {255.0f, 0.0f});
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

void ReleaseFightGaugeDefinition() {
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

bool IsFightGaugeInputActive() {
    return (fightPercentModeFlags & 1U) == 0 && fightDisplayLayout == 1;
}

// @verified - p
bool IsFightGaugeOverlayInputActive() {
    return fightDisplayLayout != 1;
}

std::int32_t GetFightGaugeDisplayLayout() {
    return fightDisplayLayout;
}

void SetFightGaugeDisplayLayout(std::int32_t layout) {
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

void SetPlayerEnemyPercents(float player, float enemy) {
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

d3t::UIGaugeState *SetFightGaugePercentSingle(float percent) {
    fightPrimaryPercent = percent < 0.0f ? 0.0f : percent > 100.0f ? 100.0f : percent;
    fightPercentModeFlags = (fightPercentModeFlags & ~3U) | 1U;
    d3t::UIGaugeState *const primary = FightWidget(0);
    if (primary)
        primary->filledCount = TruncateSpriteCoordinate(static_cast<float>(primary->count) * (fightPrimaryPercent / 100.0f));
    return primary;
}

// @verified - p
void SetFightGaugeSecondaryOverlay(std::int32_t enabled) {
    fightSecondaryOverlay = enabled;
}

void SetFightGaugePrimaryUniform(std::int32_t uniform) {
    fightPrimaryUniform = uniform;
}

void SetFightGaugePrimaryVisuals(std::int32_t uniform, float alpha) {
    fightPrimaryUniform = uniform;
    fightPrimaryAlpha = alpha;
}

void FillFightGaugeSecondaryOrbStates(std::int32_t colour) {
    if (auto *secondary = FightWidget(1))
        secondary->orbColours.fill(colour);
}
// @verified - p
void ActivateFightGaugePrimary() {
    if (fightDefinition && fightRoundImageIndices[0] >= 0)
        fightDefinition->textureObjects[fightRoundImageIndices[0]].hidden = false;
}
// @verified - p
void SetFightGaugeTrainingProgress(float progress) {
    fightPercentModeFlags |= 4U;
    fightTrainingProgress = progress;
}
// @verified - p
void ClearFightGaugeTrainingVisibility() {
    if (!fightDefinition)
        return;
    if (fightRoundImageIndices[0] >= 0)
        fightDefinition->textureObjects[fightRoundImageIndices[0]].hidden = true;
    if (fightPanelIndices[2] >= 0)
        fightDefinition->panels[fightPanelIndices[2]].imageOpacity = 0.0f;
}
#endif

} // namespace s2

} // namespace shenmue

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace shenmue

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_LoadGauge() {
    return shenmue::s2::LoadGauge();
}


extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetFightGaugeVisible(std::int32_t visible) {
    return shenmue::s2::SetFightGaugeVisible(visible);
}


extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetFightGaugeLayout(std::int32_t layout) {
    return shenmue::s2::SetFightGaugeLayout(layout);
}


extern "C" float __attribute__((renesas)) shenmue_s2_event_UpdateDualFightGauge(float player, float enemy) {
    return shenmue::s2::UpdateDualFightGauge(player, enemy);
}


extern "C" void __attribute__((renesas)) shenmue_s2_event_UpdateSingleFightGauge(float percent) {
    shenmue::s2::UpdateSingleFightGauge(percent);
}
#endif
