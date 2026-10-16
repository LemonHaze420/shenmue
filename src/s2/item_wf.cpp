#include "s2/item_wf.hpp"

#include "d3t/object_store.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/help.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <cmath>
#include <cstdio>
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>

namespace shenmue::s2 {
namespace {
int ItemTextureClear_Nullsub(void *) {
    return 0;
}
HLib::Task *wfMainControl{};
constexpr std::size_t itemPtxPrefixSize = 0xb0;
constexpr std::size_t itemPtxSize = itemPtxPrefixSize + 256 * 16 * 4;
std::array<std::uint8_t, 384> guiItemActions{};
std::array<std::int16_t, 2> temporaryItemStatus{};
std::uint8_t temporarySelectedItemState{};
std::int16_t itemLookState{};
std::uint32_t itemLookNumber{};
std::uint32_t itemLookFlags{};
std::int32_t itemLookMode{};
std::uint32_t previousNpcLoadState{};
std::int32_t pendingNpcLoadRestore{};
bool showingItemGameMenu{};
PadControlEntry *itemLookPadControl{};
PadControlEntry *previousItemLookPadControl{};
void *itemModelFile{};
std::uint32_t itemModelFileSize{};
MT7Model itemModel;
MT7Node *itemRoot{};
Vector3i itemLookRotation{};
float itemLookScale{};
float itemLookMaximumScale{}, itemLookMinimumScale{};
std::int32_t itemLookWorldYaw{};
Vector3 itemLookEventPosition{};
Vector3 previousItemCameraEye{}, previousItemCameraTarget{};
CameraState previousItemCameraState{};
std::uint16_t previousItemPerspective{}, previousItemUiVisible{}, previousItemUiSelected{}, previousItemUiMode{};
std::uint8_t previousItemHelpTopic{}, previousItemHelpMode{};
bool previousItemPlayerDisplay{}, previousItemPlayerVisible{};
std::int32_t previousItemWeatherLight{}, previousItemPostFxBank{};
std::int32_t itemLookTexture{};
alignas(4) std::array<std::byte, itemPtxSize> itemLookPtx;
ItemPreviewDrawData itemPreview{};

void SetupItemInspectInView() {
    const float depth = -0.14999999f;
    const float factor = -(depth / g_cameraControl->viewHeightScale);
    const Vector3 point{0.0f * factor, 0.0f * factor, depth};
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&g_cameraControl->viewMatrix);
    Matrix4x4::TransformPoint(point, itemLookEventPosition);
    Matrix4x4::Pop(1);
    itemLookWorldYaw = -camCtrlGetNextRotY();
}

void SetupItemInspectTransform() {
    const MapControlData *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *ryo = static_cast<Character *>(control->ryo);
    if (!ryo)
        return;
    GetInventoryItemDistances(itemLookNumber, &itemLookMinimumScale, &itemLookMaximumScale);
    itemLookScale = (itemLookMaximumScale + itemLookMinimumScale) * 0.5f;
    const std::int16_t yaw = static_cast<std::int16_t>(-static_cast<std::int16_t>(GetCharacterMotionYaw(ryo)));
    const Vector3 position = GetCharacterWorldPosition(ryo);
    itemLookEventPosition = position;
    itemRoot->rotation = {};
    itemLookRotation = {};
    if (itemLookFlags & 8U) {
        SetupItemInspectInView();
        itemLookRotation.x = 60075;
        itemLookScale = itemLookMaximumScale;
    } else {
        std::uint32_t preset = (itemLookFlags >> 8U) & 7U;
        if (preset >= 5)
            preset = 0;
        itemLookRotation.x = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<float>(itemInspectAngles[preset].x) * 8192.0f) / 45.0f));
        itemLookRotation.y = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<float>(itemInspectAngles[preset].y) * 8192.0f) / 45.0f));
        if (itemLookMode == 1)
            SetupItemInspectInView();
        else {
            itemLookWorldYaw = static_cast<std::uint16_t>(-yaw);
            itemLookEventPosition.y += 1.6f;
            const std::int16_t facing = static_cast<std::int16_t>(yaw + 0x8000);
            const float sine = sin_fixed16(facing), cosine = cos_fixed16(static_cast<std::uint16_t>(facing));
            itemLookEventPosition.x += sine * 0.14999999f;
            itemLookEventPosition.z -= cosine * 0.14999999f;
            const Vector3 eye{position.x + sine * 0.34999999f, position.y + 1.6f, position.z - cosine * 0.34999999f};
            camCtrlSetNextPosition(eye);
        }
    }
}

bool LoadItemModel() {
    const char *modelName = GetInventoryItemModel(itemLookNumber);
    const char *directory = Filepath_Gen(FilepathGenMode::ItemDirectory);
    if (!modelName)
        return false;
    char filename[32];
    std::snprintf(filename, sizeof(filename), itemLookFlags & 0x20U ? "%sG.MT7" : "%sI.MT7", modelName);
    itemModelFile = LoadLegacySceneFile(directory, filename, TASK_ITEM_MODEL, &itemModelFileSize);
    EV_NPC_NewLoadSw(previousNpcLoadState);
    pendingNpcLoadRestore = false;
    if (!itemModelFile)
        return false;
    char path[272];
    std::snprintf(path, sizeof(path), "%s/%s", directory, filename);
    if (!ReadModelFormat7({static_cast<const std::byte *>(itemModelFile), itemModelFileSize}, path, itemModel, 0x22))
        return false;
    d3t::RegisterObjectPlacement(GetMT7ReaderData(&itemModel), GetMT7ReaderData(&itemModel), 8);
    itemRoot = itemModel.nodes.empty() ? nullptr : &itemModel.nodes[0];
    return itemRoot != nullptr;
}

void DrawItemModelPreview(void *parameter) {
    ItemPreviewDrawData &preview = *static_cast<ItemPreviewDrawData *>(parameter);
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&preview.transform);
    const bool hashed = d3t::HashModel(preview.root, GetLegacyPerspectiveProjection());
    if (hashed) {
        UpdateModelFrame(&itemModel, nullptr, 0, 1);
        Matrix4x4::Pop(1);
        return;
    }
    Vector3 origin{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    SetLegacyGlobalLighting({0.40000001f, 0.40000001f, 0.40000001f, 0.40000001f});
    ResetLightEntry(0);
    SetLightKind(0, 2);
    SetDynamicLightEnabled(0, true);
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    Vector3 cameraTarget;
    camCtrlGetTargetPosition(&cameraTarget);
    const float dx = cameraTarget.x - eye.x, dz = cameraTarget.z - eye.z;
    const float distance = std::sqrt(dx * dx + dz * dz);
    const std::int16_t yaw = static_cast<std::int16_t>(preview.lightYaw - camCtrlGetNextRotY());
    if (std::fabs(origin.x) < 0.0000099999997f)
        origin.x = 0.0f;
    if (std::fabs(origin.y) < 0.0000099999997f)
        origin.y = 0.0f;
    if (std::fabs(origin.z) < 0.0000099999997f)
        origin.z = 0.0f;
    Vector3 direction{origin.x - (cameraTarget.x - sin_fixed16(yaw) * distance), origin.y - cameraTarget.y,
        origin.z - (cos_fixed16(static_cast<std::uint16_t>(yaw)) * distance + cameraTarget.z)};
    Vector3::NormalizeInPlace(direction);
    SetSelectedLightDirection(0, direction);
    shenSetLightColour(0, {preview.alpha, preview.colour.x, preview.colour.y, preview.colour.z});
    SetCurrentActiveLight(1);
    UpdateLegacyLightTransforms();
    Matrix4x4_Push();
    ItemTextureClear_Nullsub(nullptr);
    Matrix4x4::MultiplyWithPrePassMatrix(&preview.transform);
    g_scriptedModelScale.x = preview.scale;
    SetLegacyQueuedDrawDepthMode(0, 1000.0f);
    ConstructMatricesForObjectRecursive(itemModel, preview.root);
    SetLegacyQueuedDrawDepthMode(1, 0.0f);
    Matrix4x4::Pop(1);
    ItemTextureClear_Nullsub(nullptr);
    ImmediateLightWork(nullptr);
}

void icsuRenderItem() {
    if (!itemRoot)
        return;
    const std::uint16_t roll = static_cast<std::uint16_t>(itemLookRotation.z);
    const std::uint16_t yaw = static_cast<std::uint16_t>(itemLookRotation.y + ((itemLookFlags & 8U) ? 32768 : 0));
    const std::uint16_t pitch = static_cast<std::uint16_t>((itemLookFlags & 8U) ? -itemLookRotation.x : itemLookRotation.x);
    itemPreview.root = itemRoot;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&g_cameraControl->viewMatrix);
    Matrix4x4::preTranslate(0.0f, 0.0f, -0.2f);
    Matrix4x4::RotateAroundZ_Wrapper(roll);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::RotateAroundX_Wrapper(pitch);
    Matrix4x4::Scale(itemLookScale, itemLookScale, itemLookScale);
    Matrix4x4::GetTransformMatrix(&itemPreview.transform);
    Matrix4x4::Pop(1);
    itemPreview.scale = itemLookScale;
    itemPreview.alpha = 1.0f;
    itemPreview.lightYaw = static_cast<std::uint32_t>(itemLookWorldYaw);
    itemPreview.colour = {0.69999999f, 0.69999999f, 0.69999999f};
    HLib::ExecuteCallbackNow(DrawItemModelPreview, &itemPreview);
    SetItemPreviewGlareIntensity(1);
}

void icsuMoveRotateAdjustItemScale() {
    std::int32_t *yaw = &itemLookRotation.y;
    std::int32_t step = 256;
    float *rootY = nullptr;
    const float zoomStep = (itemLookMaximumScale - itemLookMinimumScale) * 0.02f;
    const LegacyInputSlot *input = ctrlGetPadInputBuffer0(itemLookPadControl);
    std::uint16_t held = input->held;
    if (itemLookFlags & 4U) {
        yaw = &itemLookRotation.z;
        held &= 0xcfffU;
        step = -256;
    }
    if (itemLookFlags & 0x40U)
        rootY = &itemRoot->position.y;
    const std::uint16_t pressed = static_cast<std::uint16_t>(ItemTextureClear_Nullsub(reinterpret_cast<void *>(1))) | input->pressedPriority;
    temporaryItemStatus[1] = 0;
    const std::uint16_t accept =
        !temporarySelectedItemState && (GetGuiItemAction(static_cast<std::uint32_t>(static_cast<std::int16_t>(itemLookNumber))) & 2U) ? 0x400U : 0;
    set_icon_select_flags_to(accept | 0x200U);
    if (pressed & accept) {
        CloseItemLook();
        SetItemSelectorDelayedResult(2);
        return;
    }
    if ((pressed & 0x200U) && !IsHelpOverlayVisible(UIElement::GameHelp)) {
        CloseItemLook();
        return;
    }
    if (held & 0xf000U) {
        if (held & 0x1000U)
            itemLookRotation.x -= step;
        if (held & 0x2000U)
            itemLookRotation.x += step;
        if (rootY) {
            if (held & 0x8000U) {
                *rootY -= 0.0099999998f;
                if (*rootY <= -0.20999999f)
                    *rootY = -0.20999999f;
            }
            if (held & 0x4000U) {
                *rootY += 0.0099999998f;
                if (*rootY >= 0.20999999f)
                    *rootY = 0.20999999f;
            }
        } else {
            if (held & 0x8000U)
                *yaw += step;
            if (held & 0x4000U)
                *yaw -= step;
        }
    }
    if (held & 0x80U) {
        itemLookScale += zoomStep;
        if (!(itemLookScale < itemLookMaximumScale))
            itemLookScale = itemLookMaximumScale;
    } else if (held & 0x40U) {
        itemLookScale -= zoomStep;
        if (!(itemLookMinimumScale < itemLookScale))
            itemLookScale = itemLookMinimumScale;
    }
    const std::uint8_t yawByte = static_cast<std::uint8_t>(static_cast<std::uint32_t>(itemLookRotation.y) >> 8U);
    const std::uint8_t pitchByte = static_cast<std::uint8_t>(static_cast<std::uint32_t>(itemLookRotation.x) >> 8U);
    if (static_cast<std::int16_t>(itemLookNumber) == 8005 &&
        ((static_cast<std::uint16_t>(yawByte - 81U) <= 0x44U && static_cast<std::uint16_t>(pitchByte - 37U) > 0xc2U) ||
            (static_cast<std::uint16_t>(yawByte - 43U) > 0xc4U && static_cast<std::uint16_t>(pitchByte - 112U) <= 0x35U))) {
        LegacySpritePacket packet{.quad = {.flags = 0x400}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0, 0, 1, 1}, .reservedTail = 0};
        TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 320, 380, 256, 16, 1.0f, 1.0f, 0, ScreenMode::Centered);
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(itemLookTexture), 0.0f);
    }
}

WfTaskData *GetWfTaskData() {
    return wfMainControl ? static_cast<WfTaskData *>(HLib::GetTaskParameter(wfMainControl)) : nullptr;
}

std::uint32_t GetSpecialWfScroll() {
    for (const WfScrollDefinition &definition : wfSpecialScrollDefinitions) {
        const std::uint32_t item = static_cast<std::uint32_t>(3000 + definition.itemOffset);
        if (GetItemState(item))
            return item;
    }
    return 0;
}

void RefreshWfMasks(WfTaskData &wf) {
    wf.owned = 0;
    wf.learned = 0;
    for (std::uint32_t index = 0; index < wfScrollDefinitions.size(); ++index) {
        const WfScrollDefinition &definition = wfScrollDefinitions[index];
        if (index == 24 || !GetItemState(static_cast<std::uint32_t>(3000 + definition.itemOffset)))
            continue;
        wf.owned |= 1U << index;
        if (definition.move >= 0 && GetMoveProficiency(definition.category, static_cast<std::uint8_t>(definition.move)))
            wf.learned |= 1U << index;
    }
    if (GetSpecialWfScroll())
        wf.owned |= 1U << 24;
}

bool InitializeWfTextures(WfTaskData &wf) {
    wf.npcLoadState = static_cast<std::uint32_t>(GetNpcNewLoadState());
    wf.ptx = nullptr;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    wf.textures.fill(-1);
    wf.ptx = HLib::AllocateTaskData(GetTextureStorageSize(0x8089, 256, 16) + 48U, TASK_WFTX);
    if (!wf.ptx) {
        EV_NPC_NewLoadSw(wf.npcLoadState);
        return false;
    }
    for (std::uint32_t index = 0; index < wf.textures.size(); ++index) {
        ParseGuiPtx(static_cast<std::byte *>(wf.ptx), 256, 16, TASK_WFTX.name, make_shen_name('S', 'K', 'L', static_cast<char>('0' + index)));
        wf.textures[index] = static_cast<std::int32_t>(ReadTexnTexture(wf.ptx));
    }
    return true;
}

void ReleaseWf(WfTaskData &wf) {
    wf.npcLoadState = 1;
    EV_NPC_NewLoadSw(1);
    if (wf.ptx)
        HLib::FreeTaskData(wf.ptx);
    wf.ptx = nullptr;
    for (std::int32_t &texture : wf.textures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    wf.flags = 0;
    wf.state = -1;
    icsToggleIconState(8, 0);
    HLib::SetupTimedTask(wfMainControl, 0);
}

void RefreshWfText(WfTaskData &wf) {
    std::uint32_t index = static_cast<std::uint32_t>(wf.first);
    std::uint32_t mask = std::rotl(1U, wf.first);
    for (std::int32_t texture : wf.textures) {
        if (wf.owned & mask) {
            if (index == 24) {
                const std::uint32_t item = GetSpecialWfScroll();
                if (item)
                    (void)shenGetAmtOf(item);
            } else {
                (void)shenGetAmtOf(static_cast<std::uint32_t>(3000 + wfScrollDefinitions[index].itemOffset));
            }
        }
        ItemTextureClear_Nullsub(wf.ptx);
        ReplacePtxTexture(static_cast<std::uint32_t>(texture), wf.ptx);
        mask = std::rotl(mask, 1);
        ++index;
    }
}

void FinishWfLearn(WfTaskData &wf) {
    wf.remaining = 0.0f;
    SetNewEwzoTypeForMoveLearned(1, 0, 0);
    RefreshWfMasks(wf);
    ReleaseWf(wf);
    SetMovesMenuItemActionState(0);
    RefreshWazamakiLearnedGroups();
}

void DrawWf(void *parameter) {
    WfTaskData &wf = *static_cast<WfTaskData *>(parameter);
    if (!(wf.flags & 1U))
        return;
    DrawLegacyItemWindow({wf.x, wf.y}, 0.60000002f, 9, 6, ScreenMode::Centered);
    LegacyIconSprite label{.x = wf.x + 52.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 0.80000007f, .width = 256, .height = 32, .alignment = 5};
    LegacyIconSprite prompt;
    InitializeLegacyIconSprite(prompt);
    prompt.x = wf.x - 140.0f;
    prompt.depth = 0.81000006f;
    for (std::uint32_t row = 0; row < 6; ++row) {
        const std::uint32_t index = row + static_cast<std::uint32_t>(wf.first);
        label.y = (wf.y - 100.0f) + static_cast<float>(40U * row);
        label.icon = static_cast<std::uint16_t>(wf.textures[row]);
        PrepareDrawTextureWithMode(label, ScreenMode::Centered);
        prompt.y = label.y;
        prompt.icon = wfPromptIcons[wfScrollDefinitions[index].prompt][(wf.learned >> (index & 31U)) & 1U];
        PrepareDrawSimple(prompt, 0xff000000U, ScreenMode::Centered);
    }
    shenSubmitQuadsUI(label.x, static_cast<float>(40 * wf.row) + (wf.y - 100.0f), 0x1effffffU, 0xbeffffffU, 244, 40, 0.70000005f);
}

bool MoveWfPosition(WfTaskData &wf, bool advancing) {
    return advancing ? AdvanceIconPosition(wf.x, wf.velocity, 320.0f) : RetreatIconPosition(wf.x, wf.velocity, -320.0f);
}
}

void Queue_wf_MainControl(std::uint8_t nextFunctionIndex) {
    wfMainControl = HLib::EnqueueTaskWithParameter(wf_MainControl_Callback, nextFunctionIndex, 0, sizeof(WfTaskData), TASK_ICWF);
    if (WfTaskData *wf = GetWfTaskData()) {
        wf->flags = 0;
        wf->ptx = nullptr;
        wf->state = -1;
        HLib::SetupTimedTask(wfMainControl, 0);
    }
}

bool GetwfMainControlState() noexcept {
    const WfTaskData *wf = GetWfTaskData();
    return wf && wf->state != -1;
}

void wf_MainControl_Callback(void *parameter) {
    WfTaskData &wf = *static_cast<WfTaskData *>(parameter);
    const std::uint16_t pressed = GetPadControlData(1);
    const std::uint16_t repeated = GetPadControlData(2);
    switch (wf.state) {
    case 0:
        if (!InitializeWfTextures(wf)) {
            ReleaseWf(wf);
            break;
        }
        wf.flags |= 1U;
        wf.state = 1;
        RefreshWfMasks(wf);
        RefreshWfText(wf);
        break;
    case 1:
        if (MoveWfPosition(wf, true))
            wf.state = 2;
        break;
    case 2: {
        const std::int32_t oldFirst = wf.first, oldRow = wf.row;
        const std::uint32_t index = static_cast<std::uint32_t>(oldRow + oldFirst), mask = 1U << (index & 31U);
        if (pressed & 0x200U)
            wf.state = 7;
        else if (pressed & 0x400U) {
            if (!(wf.owned & mask))
                (void)ExecuteAicaCommand(0x800abU, 0, 0);
            else if (index != 24 && !(wf.learned & mask)) {
                wf.flags &= ~1U;
                wf.state = 4;
                wf.remaining = 90.0f;
            } else {
                const std::uint32_t item = index == 24 ? GetSpecialWfScroll() : static_cast<std::uint32_t>(3000 + wfScrollDefinitions[index].itemOffset);
                if (OpenItemLook(item, 1)) {
                    wf.flags &= ~1U;
                    wf.state = 3;
                } else
                    (void)ExecuteAicaCommand(0x800abU, 0, 0);
            }
        } else {
            if (repeated & 0x1000U) {
                wf.row = oldRow - 1;
                if (wf.row < 0) {
                    wf.row = 0;
                    wf.first = oldFirst > 0 ? oldFirst - 1 : 0;
                }
            }
            if ((repeated & 0x2000U) && ++wf.row >= 6) {
                ++wf.first;
                wf.row = 5;
                if (wf.first + 5 >= 25)
                    --wf.first;
            }
            if (wf.row != oldRow || wf.first != oldFirst)
                RefreshWfText(wf);
        }
        set_icon_select_flags_to(0x600U);
        break;
    }
    case 3:
        if (!(GetCurrentItemSelectState() & 1U)) {
            wf.flags |= 1U;
            wf.state = 2;
        }
        break;
    case 4: {
        const WfScrollDefinition &definition = wfScrollDefinitions[static_cast<std::size_t>(wf.row + wf.first)];
        SetMoveProficiency(0, definition.category, definition.move, 12);
        SetNewEwzoTypeForMoveLearned(0, definition.category, definition.move);
        wf.state = 5;
        wf.remaining = 90.0f;
        break;
    }
    case 5:
        wf.remaining -= 1.0f;
        if (!(wf.remaining > 0.0f)) {
            FinishWfLearn(wf);
            return;
        }
        break;
    case 7:
        if (MoveWfPosition(wf, false))
            ReleaseWf(wf);
        break;
    default:
        break;
    }
    if (wf.state != -1)
        HLib::ExecuteCallbackNow(DrawWf, &wf);
}

bool OpenWfScrollMenu() {
    WfTaskData *wf = GetWfTaskData();
    if (!wf)
        return false;
    RefreshWfMasks(*wf);
    if (!wf->owned)
        return false;
    HLib::ResolveDeferredCallback(wfMainControl);
    wf->x = -320.0f;
    wf->y = 240.0f;
    wf->velocity = 1.0f;
    wf->state = 0;
    wf->flags = 0;
    wf->row = 0;
    wf->first = 0;
    wf->reserved34 = 0;
    icsToggleIconState(8, 1);
    return true;
}

void OpenWfScrollWidget() {
    WfTaskData *wf = GetWfTaskData();
    if (!wf)
        return;
    RefreshWfMasks(*wf);
    if (!wf->owned)
        return;
    HLib::ResolveDeferredCallback(wfMainControl);
    wf->state = 6;
    wf->flags = 0;
    wf->row = 0;
    wf->first = 0;
    wf->x = -320.0f;
    wf->y = 240.0f;
    wf->reserved34 = 0;
    wf->velocity = 1.0f;
    icsToggleIconState(8, 1);
    RefreshWfMasks(*wf);
}

std::int64_t CloseWfScrollMenu(std::int32_t force) {
    if (!force)
        return 1;
    WfTaskData *wf = GetWfTaskData();
    if (!wf || wf->state == -1)
        return 1;
    if (GetCurrentItemSelectState() & 1U)
        CloseItemLook();
    SetNewEwzoTypeForMoveLearned(1, 0, 0);
    ReleaseWf(*wf);
    return 1;
}

void StopWfScrollMenu() {
    if (WfTaskData *wf = GetWfTaskData()) {
        if (wf->state == 5)
            FinishWfLearn(*wf);
        else
            ReleaseWf(*wf);
    }
}

void LearnWfScroll(std::int32_t index) {
    WfTaskData *wf = GetWfTaskData();
    if (!wf)
        return;
    RefreshWfMasks(*wf);
    if (!wf->owned)
        return;
    HLib::ResolveDeferredCallback(wfMainControl);
    wf->row = index;
    wf->first = 0;
    wf->state = 4;
    wf->remaining = 90.0f;
}

std::uint32_t GetWfOwnedScrolls() {
    WfTaskData *wf = GetWfTaskData();
    if (!wf)
        return 0;
    RefreshWfMasks(*wf);
    return wf->owned;
}

std::uint32_t GetWfLearnedScrolls() {
    WfTaskData *wf = GetWfTaskData();
    if (!wf)
        return 0;
    RefreshWfMasks(*wf);
    return wf->learned;
}

std::uint16_t GetWfPromptIcon(std::uint16_t prompt, std::uint32_t scroll) {
    const std::uint32_t learned = GetWfLearnedScrolls();
    return wfPromptIcons[prompt][(learned >> (scroll & 31U)) & 1U];
}

void BuildWfMenuEntries(std::span<WfMenuEntry, 25> entries) {
    const std::uint32_t owned = GetWfOwnedScrolls();
    for (std::uint32_t index = 0; index < entries.size(); ++index) {
        const WfScrollDefinition &definition = wfScrollDefinitions[index];
        WfMenuEntry &entry = entries[index];
        entry.index = static_cast<std::int32_t>(index);
        entry.itemOffset = definition.itemOffset;
        entry.category = definition.category;
        entry.move = definition.move;
        entry.prompt = definition.prompt;
        entry.label = owned & (1U << index) ? shenGetAmtOf(static_cast<std::uint32_t>(3000 + definition.itemOffset)) : "UIGNRLQSTN"; // @text EN: "? ? ?" | JA: "？？？"
    }
}
// @verified - p
std::int32_t FindWfScrollDefinition(std::int32_t item) noexcept {
    const std::int32_t offset = item >= 3000 ? item - 3000 : item;
    for (std::int32_t index = 0; index < 25; ++index)
        if (wfScrollDefinitions[static_cast<std::size_t>(index)].itemOffset == offset)
            return index;
    return -1;
}

std::int32_t GetWfScrollPreviewRow(std::int32_t index) {
    if (index >= 3000)
        index = FindWfScrollDefinition(index);
    if (index == -1)
        return -1;
    const WfScrollDefinition &definition = wfScrollDefinitions[static_cast<std::size_t>(index)];

    if (definition.move == -1)
        return 0;
    return GetWazamakiMoveCategory(definition.category)[static_cast<std::uint16_t>(definition.move)].group;
}

// @verified - p
void BuildOwnedWfScrollSlots(std::span<std::int32_t, 26> slots) {
    std::size_t count = 0;
    for (std::int32_t index = 0; index < 25; ++index) {
        if (index != 24 && GetItemState(static_cast<std::uint32_t>(3000 + wfScrollDefinitions[static_cast<std::size_t>(index)].itemOffset)))
            slots[count++] = index;
    }
    for (const WfScrollDefinition &definition : wfSpecialScrollDefinitions) {
        const std::int32_t item = 3000 + definition.itemOffset;
        if (GetItemState(static_cast<std::uint32_t>(item)))
            slots[count] = item;
    }
}

std::uint32_t GetWfScrollItem(std::int32_t index) noexcept {
    const std::int8_t offset =
        index == 3009 || index == 3010 ? wfMenuSpecialReadThroughOffset : wfScrollDefinitions[static_cast<std::size_t>(index)].itemOffset;
    return static_cast<std::uint32_t>(3000 + offset);
}

void ICSU_InitItemLook_Callback(void *) {
    itemLookState = 0;
    itemLookPadControl = nullptr;
    itemLookPadControl = AllocatePadControlEntry();
    itemRoot = nullptr;
    itemModelFile = nullptr;
    itemLookWorldYaw = 0;
    temporarySelectedItemState = 0;
    pendingNpcLoadRestore = false;
    HLib::SetCurrentTaskTo(ICSU_CtrlItemLook_Callback);
}

void ICSU_CtrlItemLook_Callback(void *) {
    if (g_loadingFlag & 2U) {
        icsuRenderItem();
        return;
    }
    switch (itemLookState) {
    case 1:
        icsuMoveRotateAdjustItemScale();
        icsuRenderItem();
        break;
    case 2:
        if (LoadItemModel()) {
            itemLookState = 1;
            camCtrlGetNextCamPos(&previousItemCameraEye);
            camCtrlGetTargetPosition(&previousItemCameraTarget);
            previousItemCameraState = camCtrlGetState();
            previousItemPerspective = camCtrlGetPerspective();
            previousItemHelpTopic = helpGetCurrentIconIdx(1);
            previousItemHelpMode = helpGetCurrentIconIdx(0);
            previousItemUiSelected = static_cast<std::uint16_t>(GetUiFlag(1));
            previousItemUiVisible = static_cast<std::uint16_t>(GetUiFlag(0));
            MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            Character *ryo = static_cast<Character *>(control->ryo);
            if (ryo) {
                previousItemPlayerDisplay = IsOfThisVisibilityType(ryo, 3);
                previousItemPlayerVisible = charaGetWasVisible(ryo);
                EV_SetCharDisplay(ryo, 3, 0);
                charaSetVisibilityFlagTo(ryo, 0);
            }
            camCtrlSetState(CameraState::Event);
            camCtrlSetPerspective(8192);
            EV_EventSetUpHelpIcon_0(0x12, 0);
            previousItemUiMode = static_cast<std::uint16_t>(GetUiFlag(2));
            if (previousItemUiMode)
                SetHelpIconMenuFlags(0x21);
            else
                CheckHelpIcon(0x21, 0, CheckHelpIconOp::Add);
            previousItemWeatherLight = SetWeatherLightManager(0);
            SetupItemInspectTransform();
            camCtrlSetTargetPosition(itemLookEventPosition);
        } else {
            itemLookState = 0;
            (void)ExecuteAicaCommand(0x800abU, 0, 0);
            SetItemSelectorDelayedResult(0);
            icsToggleIconState(0, 0);
        }
        break;
    case 3:
        itemLookState = 4;
        break;
    case 4:
        itemLookState = 2;
        break;
    default:
        break;
    }
    ClearGuiItemActions();
}

bool OpenItemLook(std::uint32_t item, std::int32_t mode) {
    previousItemPostFxBank = static_cast<std::int32_t>(GetPostFxBankValue(1));
    SelectPostFxBank(1, 0);
    const char *modelName = GetInventoryItemModel(item);
    (void)Filepath_Gen(FilepathGenMode::ItemDirectory);
    if (!modelName)
        return false;
    char filename[32];
    std::snprintf(filename, sizeof(filename), hasItemRegistered(item) & 0x20U ? "%sG.MT7" : "%sI.MT7", modelName);
    itemLookNumber = item;
    itemLookState = 3;
    itemLookFlags = hasItemRegistered(item);
    itemLookMode = mode;
    previousNpcLoadState = static_cast<std::uint32_t>(GetNpcNewLoadState());
    EV_NPC_NewLoadSw(0);
    pendingNpcLoadRestore = true;
    icsToggleIconState(0, 1);
    if (itemLookPadControl && !(itemLookPadControl->flags & 2U))
        previousItemLookPadControl = BindPadControlEntry(itemLookPadControl);
    ParseGuiPtx(itemLookPtx.data(), 256, 16, SHEN_NAME(ICSU), SHEN_NAME(IWAO));
    itemLookTexture = static_cast<std::int32_t>(ReadTexnTexture(itemLookPtx.data()));
    ItemTextureClear_Nullsub(itemLookPtx.data());
    ReplacePtxTexture(static_cast<std::uint32_t>(itemLookTexture), itemLookPtx.data());
    return true;
}

void SetItemLookShowingGameMenu() noexcept {
    showingItemGameMenu = true;
}

void ToggleGameMenu() noexcept {
    if (showingItemGameMenu) {
        DisableElement(UIElement::GameMenu);
        DestroySharedItemsWidget();
        g_inputBuffer[0].pressedPriority = 0;
    }
    showingItemGameMenu = false;
}

void CloseItemLook() {
    SelectPostFxBank(1, previousItemPostFxBank);
    if (itemModelFile) {
        ReleaseModelFormat7(itemModel);
        HLib::FreeTaskData(itemModelFile);
        itemModelFile = nullptr;
        itemRoot = nullptr;
    }
    SetItemSelectorDelayedResult(0);
    if (GetCurrentItemSelectState() & 1U) {
        EV_EventSetUpHelpIcon_0(previousItemHelpTopic, previousItemHelpMode);
        if (previousItemUiMode)
            SetHelpIconMenuFlags(previousItemUiVisible);
        else
            ResetHelpIconsToItemPrompt();
        temporarySelectedItemState = 0;
        camCtrlSetPerspective(previousItemPerspective);
        camCtrlSetState(previousItemCameraState);
        camCtrlSetNextPosition(previousItemCameraEye);
        camCtrlSetTargetPosition(previousItemCameraTarget);
        MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Character *ryo = static_cast<Character *>(control->ryo);
        if (ryo) {
            EV_SetCharDisplay(ryo, 3, previousItemPlayerDisplay);
            charaSetVisibilityFlagTo(ryo, previousItemPlayerVisible);
        }
        if (itemLookPadControl && (itemLookPadControl->flags & 2U))
            BindPadControlEntry(previousItemLookPadControl);
        itemLookState = 0;
        SetWeatherLightManager(previousItemWeatherLight);
        icsToggleIconState(0, 0);
        (void)ExecuteAicaCommand(0x900abU, 0, 0);
        if (pendingNpcLoadRestore)
            EV_NPC_NewLoadSw(previousNpcLoadState);
        pendingNpcLoadRestore = false;
        ReleaseNamedTexture(itemLookTexture);
    }
    ToggleGameMenu();
}

void InspectWfScroll(std::int32_t index) {
    const std::uint32_t item = index == 24 ? GetSpecialWfScroll() : GetWfScrollItem(index);
    showingItemGameMenu = true;
    if (!OpenItemLook(item, 1)) {
        (void)ExecuteAicaCommand(0x800abU, 0, 0);
        DisableElement(UIElement::GameMenu);
    }
}

std::uint32_t GetGuiItemAction(std::uint32_t item) noexcept {
    const std::span<const InventoryItemDefinition> definitions = GetInventoryItemDefinitions(S2InventoryBank::Type0);
    for (std::uint32_t index = 0; index < definitions.size(); ++index)
        if (definitions[index].id == item)
            return guiItemActions[index];
    return 0;
}

std::uint8_t SetGuiItemAction(std::uint32_t item, std::uint8_t flags, std::int32_t enabled) noexcept {
    const std::span<const InventoryItemDefinition> definitions = GetInventoryItemDefinitions(S2InventoryBank::Type0);
    for (std::uint32_t index = 0; index < definitions.size(); ++index) {
        if (definitions[index].id != item)
            continue;
        const std::uint8_t previous = guiItemActions[index];
        guiItemActions[index] = enabled ? static_cast<std::uint8_t>(previous | flags) : static_cast<std::uint8_t>(previous & ~flags);
        return previous;
    }
    return 0;
}

void ClearGuiItemActions() noexcept {
    guiItemActions.fill(0);
}

std::int64_t UpdateSelectedItemStateFlag(std::int16_t item) {
    std::int64_t result = GetShouldRenderIcons(RenderIconSlot::Any);
    if (!result) {
        result = GetCurrentItemSelectState();
        if (!(result & 1)) {
            result = OpenItemLook(static_cast<std::uint32_t>(item), 0);
            if (result)
                temporarySelectedItemState = 1;
        }
    }
    return result;
}

void SetItemLookRotation(std::int16_t pitch, std::int16_t yaw, std::int16_t roll) noexcept {
    itemLookRotation = {pitch, yaw, roll};
}

void ReleaseItemWfOwners() {
    if (itemLookState || itemModelFile)
        CloseItemLook();
    if (wfMainControl) {
        StopWfScrollMenu();
        wfMainControl = nullptr;
    }
    itemLookPadControl = nullptr;
    previousItemLookPadControl = nullptr;
}

} // namespace shenmue::s2
