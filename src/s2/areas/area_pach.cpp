#include "s2/areas/area_pach.hpp"
#include "s2/area_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/map_info.hpp"
#include "s2/model_image.hpp"
#include "s2/player.hpp"
#include "s2/world_render.hpp"

#include <cstring>

namespace shenmue::s2::areas {
namespace {
std::int32_t pachImage;
std::array<PatiNailNode, 512> pachMirrorSource;


void PACHNativeDebugNullsub(std::int32_t, const void *) {
}
} // namespace

// @verified - p
void AreaPACHInit() {
}

// @verified - p
std::int32_t PACHFindProp(PachDebugLayout *layout) {
    for (std::int32_t index = 0; index < 512; ++index) {
        const Vector3 &position = layout->props[index].position;
        if (position.y >= 0.0f && position.x > layout->horizontalAxis - 0.0049999999f && position.x < layout->horizontalAxis + 0.0049999999f &&
            position.y > layout->verticalAxis - 0.0049999999f && position.y < layout->verticalAxis + 0.0049999999f) {
            layout->nextProp = index;
            layout->selectedProp = index;
            return 1;
        }
    }
    return 0;
}

// @verified - p
void PACHMirrorProps(PachDebugLayout *layout, std::int32_t vertical) {
    std::memcpy(pachMirrorSource.data(), layout->props.data(), sizeof(pachMirrorSource));
    std::int32_t destination = 0;
    for (std::int32_t index = 0; index < 512; ++index) {
        const PatiNailNode &source = pachMirrorSource[index];
        const float halfHeight = PACH_BOARD_HEIGHT[pachImage] * 0.5f;
        if (vertical ? source.position.y <= halfHeight : source.position.x >= 0.0f)
            continue;
        layout->props[destination] = source;
        PatiNailNode &mirror = layout->props[destination + 1];
        mirror = source;
        mirror.reserved00 = 0;
        mirror.position.x = vertical ? source.position.x : -source.position.x;
        mirror.position.y = vertical ? halfHeight - (source.position.y - halfHeight) : source.position.y;
        mirror.position.z = source.position.z;
        mirror.rotation = {};
        mirror.scale = {1.0f, 1.0f, 1.0f};
        destination += 2;
        if (destination >= 512)
            return;
    }
}

// @verified - p
void PACHDebugGrid() {
    Vector3 position{0.0f, 0.1f, 0.0060000001f};
    if (PACH_BOARD_HEIGHT[pachImage] > 0.1f) {
        do {
            position.x = -0.25f;
            do {
                PACHNativeDebugNullsub(5, nullptr);
                PACHNativeDebugNullsub(5, &position);
                position.x += 0.1f;
            } while (position.x <= 0.25f);
            position.y += 0.1f;
        } while (PACH_BOARD_HEIGHT[pachImage] > position.y);
    }
}

// @verified - p
void PACHDebugLoopCallback(void *taskData) {
    PachDebugTask *const task = static_cast<PachDebugTask *>(taskData);
    PachDebugLayout &layout = task->layouts[task->selectedLayout];
    if (!task->mode) {
        const bool selected = EVT_GetControllerInput(0, 3) != 0;
        if (EVT_GetControllerInput(0, 2) & PAD_RIGHT)
            layout.horizontalAxis += 0.0099999998f;
        if (EVT_GetControllerInput(0, 2) & PAD_LEFT)
            layout.horizontalAxis -= 0.0099999998f;
        if (EVT_GetControllerInput(0, 2) & PAD_DOWN)
            layout.verticalAxis -= 0.0099999998f;
        if (EVT_GetControllerInput(0, 2) & PAD_UP)
            layout.verticalAxis += 0.0099999998f;
        const std::uint32_t horizontal = EVT_GetControllerInput(0, 5);
        if (horizontal >= 192)
            layout.cameraPosition.x += 0.0099999998f;
        if (horizontal <= 64)
            layout.cameraPosition.x -= 0.0099999998f;
        const std::uint32_t vertical = EVT_GetControllerInput(0, 6);
        if (vertical >= 192)
            layout.cameraPosition.y -= 0.0099999998f;
        if (vertical <= 64)
            layout.cameraPosition.y += 0.0099999998f;
        if (EVT_GetControllerInput(0, 1) & 2U) {
            layout.camera = (layout.camera == 1) + 1;
            EVS_LoopCamera(layout.camera, PACH_CAMERA, PACH_CAMERA);
        }
        PACHNativeDebugNullsub(0, &layout.horizontalAxis);
        if (EVT_GetControllerInput(0, 1) & 0x100U) {
            if (!selected && !PACHFindProp(&layout)) {
                PatiNailNode &prop = layout.props[layout.nextProp];
                prop.reserved00 = 0;
                prop.position = {layout.horizontalAxis, layout.verticalAxis, layout.depth};
                prop.rotation = {};
                prop.scale = {1.0f, 1.0f, 1.0f};
                layout.selectedProp = layout.nextProp;
                std::int32_t index = 0;
                while (layout.props[index].position.y >= 0.0f) {
                    if (++index >= 512)
                        break;
                }
                if (index >= 512) {
                    ++layout.nextProp;
                    layout.propCount = 512;
                    if (layout.nextProp >= 512)
                        layout.nextProp = 0;
                } else {
                    ++layout.propCount;
                    layout.nextProp = index;
                    if (layout.propCount >= 512)
                        layout.propCount = 512;
                }
            } else if (layout.selectedProp >= 0) {
                --layout.propCount;
                layout.nextProp = layout.selectedProp;
                if (layout.propCount < 0)
                    layout.propCount = 0;
                layout.props[layout.selectedProp].position.y = -1.0f;
                layout.selectedProp = -1;
            }
        }
        if (EVT_GetControllerInput(0, 1) & 0x400U)
            PACHMirrorProps(&layout, 0);
        if (EVT_GetControllerInput(0, 1) & 0x200U)
            PACHMirrorProps(&layout, 1);
        EV_AT_SetPos(PACH_CAMERA, EventTransformFlags::AllAxes, layout.cameraPosition);
        if (layout.props[layout.nextProp].position.y > 0.0f)
            PACHNativeDebugNullsub(0, &layout.props[layout.nextProp].position);
        for (std::int32_t index = 0; index < 512; ++index) {
            if (layout.props[index].position.y > 0.0f)
                DrawPIN(PACH_PIN, 1, &layout.props[index].position, nullptr, &PACH_PIN_SCALE);
        }
    }
    PACHDebugGrid();
    if (EVT_GetControllerInput(0, 1) & 0x800U) {
        if (++task->selectedLayout >= 16)
            task->selectedLayout = 0;
        std::uint16_t angle = static_cast<std::uint16_t>(task->selectedLayout << 12);
        for (const shen_name board : PACH_BOARDS) {
            const Vector3 position{
                -sin_fixed16(static_cast<std::int16_t>(angle)), 0.0f, sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle))) - 1.0f};
            EV_AT_SetPos(board, EventTransformFlags::AllAxes, position);
            angle = static_cast<std::uint16_t>(angle + 4096);
        }
    }
    if (EVT_GetControllerInput(0, 1) & 64U) {
        if (++pachImage >= 3)
            pachImage = 0;
        for (const shen_name board : PACH_BOARDS)
            SetScenarioImageModule(board, PACH_IMAGES[pachImage]);
    }
}

void PACHInitializeCallback(void *) {
    EVS_LoopCamera(0, {}, {});
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(PACHDebugLoopCallback, 9, 0x0b, sizeof(PachDebugTask), PACH_TASK);
    PachDebugTask *const parameter = static_cast<PachDebugTask *>(HLib::GetTaskParameter(task));
    pachImage = 0;
    parameter->mode = 0;
    parameter->selectedLayout = 0;
    for (std::int32_t index = 15; index >= 0; --index) {
        PachDebugLayout &layout = parameter->layouts[index];
        layout.mode = 0;
        layout.horizontalAxis = 0.0f;
        layout.verticalAxis = 0.5f;
        layout.depth = 0.0049999999f;
        layout.cameraPosition = {0.0f, 0.5f, 0.0f};
        layout.nextProp = 0;
        layout.selectedProp = -1;
        layout.camera = 1;
        layout.propCount = 0;
        layout.reserved2c = 0;
        for (PatiNailNode &prop : layout.props)
            prop.position = {0.0f, -1.0f, 0.0f};
        std::uint16_t angle = 0;
        for (const shen_name board : PACH_BOARDS) {
            const Vector3 position{
                -sin_fixed16(static_cast<std::int16_t>(angle)), 0.0f, sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle))) - 1.0f};
            EV_AT_SetPos(board, EventTransformFlags::AllAxes, position);
            angle = static_cast<std::uint16_t>(angle + 4096);
        }
    }
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const MapInfoChunk *const chunk = FindMapInfoChunk("PACH", control->mapInfo);
    const std::int32_t words = static_cast<std::int32_t>(chunk->nextByteOffset) / 4;
    if (words > 0)
        std::memcpy(parameter->layouts.data(), chunk + 1, static_cast<std::size_t>(words) * 4);
    CreateEventCharacter(CharacterCreationTable0, nullptr, PACH_CAMERA);
    EV_AT_SetPos(PACH_CAMERA, EventTransformFlags::AllAxes, parameter->layouts[0].cameraPosition);
    const Vector3i rotation{};
    EV_AT_SetAngY(PACH_CAMERA, EventTransformFlags::AllAxes, rotation, nullptr);
    EVS_LoopCamera(parameter->layouts[0].camera, PACH_CAMERA, PACH_CAMERA);
    if (g_currentMap.name == AREA_PACH.name) {
        QueueImmediateNextFadeTask(task);
        HLib::DestroyCurrentTask();
    }
}

// @verified - p
void AreaPACHMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    if (g_currentMap.name == AREA_PACH.name) {
        HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(PACHInitializeCallback, 3, 0x0b, static_cast<std::int32_t>(PACH_TASK.name));
        HLib::SetupTimedTask(task, 3);
    } else {
        PACHInitializeCallback(nullptr);
    }
}

const ScriptAreaDefinition areaPACHScriptDefinition{.areaId = AREA_PACH, .field04 = 0, .init = AreaPACHInit, .main = AreaPACHMain};

} // namespace shenmue::s2::areas
