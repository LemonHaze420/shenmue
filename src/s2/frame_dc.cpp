#include "s2/frame_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/pause.hpp"
#include "s2/system_modules.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/backup_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/initialization_dc.hpp"
#include "s2/input_dc.hpp"

#define SHENMUE_FRAME_NULL 0
#define SHENMUE_FRAME_COUNT(value) (sizeof(value) / sizeof(value[0]))
#define SHENMUE_FRAME_DATA(value) value
#define SHENMUE_FRAME_MODULE(value) SystemModule_##value
#include "s2/magic_room_dc.hpp"
#include "s2/system_modules_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/map_control_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/math.hpp"
#include "s2/stream_audio_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <cri_adxf.h>
#include <cstring>
#endif
#include "s2/scenario_flags_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/event_overlay.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/arcade_rankings_dc.hpp"
#include "s2/texture_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "s2/scene_data.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include <cstdio>
#include "s2/arcade_modules_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "platform/dc/aica.hpp"
#include "platform/dc/aica_stream.hpp"
#include "platform/dc/ninja.hpp"
#include <sg_gd.h>
#if SHENMUE_S2_GCC295
#define SHENMUE_SYSTEM_MODULE(name) SystemModule_##name
#define SHENMUE_SYSTEM_MODULE_NULL 0
#else
#define SHENMUE_SYSTEM_MODULE(name) SystemModule::name
#define SHENMUE_SYSTEM_MODULE_NULL nullptr
#endif
#include "s2/lcdm_dc.hpp"
#include "s2/startup_dc.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/area_event.hpp"
#include "s2/sound_dc.hpp"
#if SHENMUE_S2_GCC295
#else
#endif
#include "s2/scene_assets_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include <sg_lcd.h>
extern "C" const std::uint8_t *kdGetDevice(std::uint32_t port);
#include "shenmue.h"
#include <sg_pad.h>
namespace shenmue {
namespace s2 {

namespace {

void ExecuteRoom(void *);
void InitializeMagicRoomPlacements();
void GenerateRoom(std::int32_t seed);
void InstallRoomCollision(MagicRoomPlacement *placements, std::int32_t count, const Vector3 *origin, std::uint32_t yaw, const Vector3 *dimensions);
void AssignRoomLightVectors(const Vector3 &origin, std::uint32_t yaw);
void EnableRoomLights();

} // namespace

std::int32_t SelectRoomDescriptor(Vector3 *position, std::uint32_t *yaw, Vector3 *dimensions);
void InitializePeripheralInput();
void InitializeInputSlots();
void SnapshotPeripheralInputs();
void UpdateInputsFromInputDevices();

void InitializePeripheralInput();
void InitializeInputSlots();
void SnapshotPeripheralInputs();
void UpdateInputsFromInputDevices();
bool ExecuteSystemModule(SystemModule module, std::int32_t entry);
bool ChangeSystemModule(SystemModule module, std::int32_t entry);
bool ExecuteSystemModule(SystemModule module, std::int32_t entry);
bool ChangeSystemModule(SystemModule module, std::int32_t entry);
std::uint32_t g_defaultVblankInterval __attribute__((section(".data.dc_default_vblank"))) = 0;
std::uint32_t g_frameRateVblankInterval __attribute__((section(".data.dc_frame_vblank"))) = 0;
std::uint32_t g_frameCounter __attribute__((section(".data.dc_frame_counter"))) = 0;
volatile std::uint32_t g_vblankCounter __attribute__((section(".data.dc_vblank_counter"))) = 0;
std::uint32_t g_loadingFlag __attribute__((section(".data.dc_loading_flags"))) = 0;
std::uint32_t g_developerDisplayEnabled __attribute__((section(".data.dc_frame_unknown170"))) = 0;
std::uint32_t g_savedFrameRateVblankInterval = 0;
std::uint32_t g_gameSystemsInitialized = 0;
std::int32_t g_softResetProcessingEnabled;
std::int32_t g_backupProcessingEnabled;
std::uint32_t g_frameTimerStart;
std::uint32_t g_framePostUpdateTicks;
float g_frameMicroseconds __attribute__((section(".data.dc_frame_microseconds"))) = 0.0f;
float g_frameMicrosecondsHistory[32] = {};
std::uint32_t g_previousFrameTimer;
std::uint32_t g_previousVblankCounter = 0;
std::int32_t g_discTrayFrames;
std::uint32_t g_pvrTaOlBase, g_pvrTaIspBase, g_pvrTaOlLimit, g_pvrTaIspLimit, g_pvrTaNextOpb, g_pvrTaItpCurrent;
std::uint32_t g_vertexBufferUsage[5] = {};
std::uint32_t g_vertexBufferHighWater[5] = {};
std::uint32_t g_additionalVertexBufferUsage;
std::uint32_t g_additionalVertexBufferHighWater;
FrameInterrupt g_frameInterrupt = SHENMUE_FRAME_NULL;

namespace {

extern "C" std::int32_t syChainAddHandler(std::int16_t event, FrameInterrupt callback, std::uint32_t priority, void *parameter);
extern "C" void syRtcFinish();
extern "C" void gdFsFinish();
std::uint32_t pvrMetricFrame = 0;

} // namespace

AudioPartition *g_magicRoomArchive;
_adx_fs *g_magicRoomFile;
std::uint8_t g_magicRoomAreaJumpBlocked;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const MagicRoomFloorGroup *g_magicRoomFloors = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3 g_magicRoomOrigin = {}, g_magicRoomCollisionDimensions = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t g_magicRoomYaw = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_magicRoomSeedBase = 0, g_magicRoomSeed = 0;
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
HLib::Task *roomExecutionTask;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MagicRoomHeader *roomDatabase;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MagicRoomPlacement *roomPlacements, *roomLoadingPlacement = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MagicRoomEntityLight roomEntityLights[2] = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void *roomLoadingAllocation;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t roomState, roomLoadingIndex = 0, roomGeneratedCount = 0, roomPreviousSeed = -1;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t roomLoadingFrames;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t roomInitialized;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3 roomDrawOrigin = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t roomDrawYaw = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t roomDisabledLightCount;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int16_t roomDisabledLights[4];
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t ResetUnusedReloadState();
#else
std::int64_t ResetUnusedReloadState();
#endif
void ReloadModelResourcesNullsub();
void ResetColdRuntimeState();
void mstsInit();
void ResetTaskWaitFlags();
void DestroyEffectTextureReferences();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocateAndLoadMOTFiles();
#else
std::byte *AllocateAndLoadMOTFiles();
#endif
void AllocateARMM();
void AllocateFlagWork();
void LoadMemoData();
void npcInitData();
void AllocateFONT();
void InitAica();
extern "C" void njSetFogColor(std::uint32_t colour);
std::uint32_t g_eventTaskFlags1 __attribute__((section(".data.dc_event_task_flags1"))) = 0;
std::uint32_t g_eventTaskFlags2 __attribute__((section(".data.dc_event_task_flags2"))) = 0;
std::uint32_t g_eventTaskFlags3 __attribute__((section(".data.dc_event_task_flags3"))) = 0;
bool ChangeSystemModule(SystemModule module, std::int32_t entry);

namespace {

__attribute__((aligned(32))) Matrix4x4 primaryMatrixStorage[33] = {};
extern "C" std::int32_t syCblCheck();
extern "C" void syHwInit();
extern "C" void syHwInit2();
extern "C" void syRtcInit();
extern "C" void ConfigureCpuCache(std::uint32_t control) asm("_shenmue_dc_configure_cpu_cache");

} // namespace

void LoadFonts();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t ResetUnusedReloadState();
#else
std::int64_t ResetUnusedReloadState();
#endif

namespace {

// @verified - d
void FrameInterruptNullsub() {
}

// @verified - d
std::uint32_t GetAdditionalVertexBufferUsage() {
    return 0;
}

} // namespace

// @verified - d
void InitializeSystemLibrariesNullsub() {
}

// @verified - d
void ShutdownSystemLibrariesNullsub() {
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
AudioPartition *MountAudioPartitionDefault(const char *directory, const char *filename, std::int32_t entries);

// @verified - d
void QueueMagicRoom(const char *directory, const char *archive, const MagicRoomFloorGroup *groups, std::int32_t seed) {
    if (g_magicRoomArchive)
        return;
    const shen_name name = MAGIC_ROOM_TASK;
    roomInitialized = 1;
    g_magicRoomArchive = MountAudioPartitionDefault(directory, archive, 0x800);
    g_magicRoomFile = ADXF_OpenAfs(g_magicRoomArchive->id, 0);
    const std::int32_t sectors = ADXF_GetFsizeSct(g_magicRoomFile);
    roomDatabase = static_cast<MagicRoomHeader *>(HLib::AllocateTaskData(static_cast<std::uint32_t>(sectors) * 0x800U, name));
    ADXF_ReadNw32(g_magicRoomFile, sectors, roomDatabase);
    roomState = 0;
    roomLoadingFrames = 0;
    g_magicRoomSeedBase = seed;
    roomExecutionTask = HLib::EnqueueTaskWithoutParameter(ExecuteRoom, 12, 2, name);
    g_magicRoomFloors = groups;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void MagicRoomModelVariant_Callback(void *) {
    if ((g_inputBuffer[1].held & 0x802U) != 0x802U)
        return;
    switch (g_ryoModelVariant) {
    case 11:
        if (g_inputBuffer[0].pressedPriority & 0x200U)
            g_ryoModelVariant = 33;
        break;
    case 33:
        if (g_inputBuffer[0].pressedPriority & 0x200U)
            g_ryoModelVariant = 55;
        break;
    case 55:
        if (g_inputBuffer[0].pressedPriority & 0x400U)
            g_ryoModelVariant = 77;
        break;
    case 77:
        if (g_inputBuffer[0].pressedPriority & 0x600U)
            g_ryoModelVariant = 0;
        break;
    default:
        if (g_inputBuffer[0].pressedPriority & 0x400U)
            g_ryoModelVariant = 11;
        else
            g_ryoModelVariant = 0;
        break;
    }
}
#else
// @verified - d
void MagicRoomModelVariant_Callback(void *) {
    if ((g_inputBuffer[1].held & 0x802U) != 0x802U)
        return;
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    switch (g_ryoModelVariant) {
    case 11:
        if (pressed & 0x200U)
            g_ryoModelVariant = 33;
        break;
    case 33:
        if (pressed & 0x200U)
            g_ryoModelVariant = 55;
        break;
    case 55:
        if (pressed & 0x400U)
            g_ryoModelVariant = 77;
        break;
    case 77:
        if (pressed & 0x600U)
            g_ryoModelVariant = 0;
        break;
    default:
        g_ryoModelVariant = pressed & 0x400U ? 11 : 0;
        break;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void BlockMagicRoomAreaJump() {
    if (!g_magicRoomAreaJumpBlocked && EV_CanDoAreaJump()) {
        g_magicRoomAreaJumpBlocked = 1;
        EV_SetCanDoAreaJumpTo(0);
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SelectMagicRoomForPlayer() {
    if (!g_magicRoomArchive)
        return;
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    GetCharacterWorldPosition(control->ryo, &g_magicRoomOrigin);
    g_magicRoomSeed = SelectRoomDescriptor(&g_magicRoomOrigin, &g_magicRoomYaw, &g_magicRoomCollisionDimensions);
    if (g_magicRoomSeed != -1)
        BlockMagicRoomAreaJump();
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SelectMagicRoomAtPosition(const Vector3 *position) {
    if (!g_magicRoomArchive)
        return;
    g_magicRoomOrigin = *position;
    g_magicRoomSeed = SelectRoomDescriptor(&g_magicRoomOrigin, &g_magicRoomYaw, &g_magicRoomCollisionDimensions);
    if (g_magicRoomSeed != -1)
        BlockMagicRoomAreaJump();
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void LeaveMagicRoom() {
    if (!g_magicRoomArchive)
        return;
    g_magicRoomSeed = -1;
    if (g_magicRoomAreaJumpBlocked) {
        g_magicRoomAreaJumpBlocked = 0;
        EV_SetCanDoAreaJumpTo(1);
    }
}
#endif

void InitMagicRoomPtrs() {
    g_magicRoomArchive = 0;
    g_magicRoomFile = 0;
    g_magicRoomAreaJumpBlocked = 0;
    if (g_actNumber == 1 && g_currentMap == MAGIC_ROOM_DEBUG_AREA) {
        HLib::EnqueueTaskWithoutParameter(MagicRoomModelVariant_Callback, 12, 2, MAGIC_ROOM_TASK);
    } else if (g_actNumber == 1 && g_currentMap == MAGIC_ROOM_PLAYER_AREA) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        if (g_ryoModelVariant == 77)
            EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, MAGIC_ROOM_MODEL_VARIANT_FLAG, 1);
        else
            EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, MAGIC_ROOM_MODEL_VARIANT_FLAG, 0);
#else
        EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, MAGIC_ROOM_MODEL_VARIANT_FLAG, g_ryoModelVariant == 77 ? 1 : 0);
#endif
    }
}

// @verified - d
void RestoreMagicRoomModelVariant() {
    if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, MAGIC_ROOM_MODEL_VARIANT_FLAG))
        g_ryoModelVariant = 77;
}

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
const MagicRoomEntityLight *FindRoomLightChunk(const void *data, std::uint32_t tag, std::int32_t count) {
    const std::uint32_t *chunk = static_cast<const std::uint32_t *>(data);
    if (chunk) {
        for (std::int32_t index = 0; index < count; ++index) {
            if (chunk[0] == tag)
                return reinterpret_cast<const MagicRoomEntityLight *>(chunk);
            if (chunk[1] & 3U)
                return 0;
            chunk = reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(chunk) + (chunk[1] >> 2 << 2));
        }
    }
    return 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool LoadRoomModels(MagicRoomPlacement *placements, std::int32_t count, std::int32_t release, const Vector3 *origin, std::uint32_t yaw) {
    std::int32_t index;
    if (release) {
        if (g_magicRoomFile) {
            ADXF_Stop(g_magicRoomFile);
            ADXF_Close(g_magicRoomFile);
            g_magicRoomFile = 0;
            HLib::FreeTaskData(roomLoadingAllocation);
        }
        for (index = 0; index < count; ++index, ++placements) {
            if (placements->modelHandle && static_cast<std::int16_t>(placements->modelIndex) >= 0) {
                ReleaseMT7Model(placements->modelHandle);
                placements->modelHandle = 0;
            }
        }
        return false;
    }
    index = 0;
    if (!g_magicRoomFile) {
        roomLoadingPlacement = placements;
        for (; index < count; ++roomLoadingPlacement, ++index) {
            if (!roomLoadingPlacement->modelHandle) {
                roomLoadingIndex = index;
                g_magicRoomFile = ADXF_OpenAfs(g_magicRoomArchive->id, static_cast<std::int16_t>(roomLoadingPlacement->modelIndex) + 1);
                if (!g_magicRoomFile)
                    return false;
                const std::int32_t sectors = ADXF_GetFsizeSct(g_magicRoomFile);
                roomLoadingAllocation = HLib::AllocateTaskData(static_cast<std::uint32_t>(sectors) * 0x800U, shen_name(SHEN_NAME4('R', 'O', 'O', 'M')));
                if (!roomLoadingAllocation)
                    return false;
                ADXF_ReadNw32(g_magicRoomFile, sectors, roomLoadingAllocation);
                return true;
            }
        }
        return false;
    }
    switch (ADXF_GetStat(g_magicRoomFile)) {
    case 1:
    case 4:
        break;
    case 2:
        ++roomLoadingFrames;
        break;
    case 3:
        ADXF_Close(g_magicRoomFile);
        g_magicRoomFile = 0;
        roomLoadingFrames = 0;
        const std::uint32_t loadedTicks = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
        const float loadedTime = static_cast<float>(loadedTicks);
        roomLoadingPlacement->modelHandle = InitializeMT7Model(roomLoadingAllocation);
        for (index = roomLoadingIndex + 1; index < count; ++index) {
            const MagicRoomPlacement *const loaded = roomLoadingPlacement;
            if (static_cast<std::int16_t>(loaded->modelIndex) == static_cast<std::int16_t>(placements[index].modelIndex)) {
                reinterpret_cast<std::int16_t &>(placements[index].modelIndex) = -1;
                placements[index].modelHandle = loaded->modelHandle;
            }
        }
        if (roomLoadingPlacement->kind == 1) {
            const MagicRoomEntityLight *const source = FindRoomLightChunk(roomLoadingAllocation, SHEN_NAME4('L', 'I', 'G', 'T'), 2);
            if (source) {
                matrix_stack::Matrix4x4_Push(0);
                matrix_stack::UnitCurrentMatrix();
                matrix_stack::TranslateVector(origin);
                Matrix4x4::RotateAroundY_Wrapper(yaw);
                matrix_stack::TranslateVector(&roomLoadingPlacement->position);
                if (const std::int16_t rotation = static_cast<std::int16_t>(roomLoadingPlacement->extraRotationZ))
                    Matrix4x4::RotateAroundZ_Wrapper(rotation);
                if (const std::int16_t rotation = static_cast<std::int16_t>(roomLoadingPlacement->yaw))
                    Matrix4x4::RotateAroundY_Wrapper(rotation);
                if (const std::int16_t rotation = static_cast<std::int16_t>(roomLoadingPlacement->extraRotationX))
                    Matrix4x4::RotateAroundX_Wrapper(rotation);
                MagicRoomEntityLight light = *source;
                Matrix4x4::TransformPoint(source->position, light.position);
                Matrix4x4::Transform(source->direction, light.direction);
                light.direction.y = -light.direction.y;
                PopCurrentMatrix(1);
                AddMagicRoomEntityMatrix(reinterpret_cast<const Matrix4x4 *>(&light));
            }
            if (roomLoadingPlacement->modelHandle && FindMT7Node(roomLoadingPlacement->modelHandle->root, 0x98))
                roomLoadingPlacement->kind = 2;
            else
                roomLoadingPlacement->kind = 0;
        }
        const std::uint32_t lightTicks = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
        const float lightTime = static_cast<float>(lightTicks);
        break;
    }
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawRoom(void *) {
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::TranslateVector(&roomDrawOrigin);
    Matrix4x4::RotateAroundY_Wrapper(roomDrawYaw);
    for (std::int32_t index = 0; index < roomGeneratedCount; ++index) {
        MT7Model *model = roomPlacements[index].modelHandle;
        if (!model)
            continue;
        MT7Node *root = model->root;
        if (!root)
            continue;
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::TranslateVector(&roomPlacements[index].position);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int16_t>(roomPlacements[index].yaw));
        DrawCharacterModelBranch(root);
        PopCurrentMatrix(1);
    }
    PopCurrentMatrix(1);
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AssignLightParametersForMagicRoomEntity(std::uint32_t index, const MagicRoomEntityLight &light) {
    if (light.enabled) {
        EnableLightEntry(index);
        do {
            SetLightFlagForIndex(index, light.flags);
            SetLightRuntimeColour(index, light.colour);
            SetLightSceneIntensity(index, light.sceneIntensity);
        } while (0);
        SetLightRange(index, light.range * 1.2f);
        SetLightRuntimePosition(index, light.position);
        SetLightSunDirection(index, light.direction);
        SetLightConeAngle(index, light.coneAngle);
    } else {
        DisableLightEntry(index);
    }
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateRoomAnimations(const Vector3 *origin, std::uint32_t yaw) {
    MagicRoomEntityLight light;
    DateTime time;
    MT7Model *model;
    MT7Node *node;
    std::int32_t index;
    if (!roomGeneratedCount)
        return;
    const std::int32_t lightCount = static_cast<std::int32_t>(GetMagicRoomEntityCount());
    if (lightCount) {
        for (index = 0; index < lightCount; ++index) {
            const MagicRoomEntityLight *const parameters = GetMagicRoomEntityParameters(index);
            if (parameters)
                AssignLightParametersForMagicRoomEntity(index + 254U, *parameters);
        }
    } else {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::UnitCurrentMatrix();
        matrix_stack::TranslateVector(origin);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        const MagicRoomEntityLight *const defaults = reinterpret_cast<const MagicRoomEntityLight *>(magicRoomDefaultLightWords);
        light = *defaults;
        Matrix4x4::TransformPoint(defaults->position, light.position);
        Matrix4x4::Transform(defaults->direction, light.direction);
        PopCurrentMatrix(1);
        AssignLightParametersForMagicRoomEntity(254, light);
    }
    for (index = 0; index < roomGeneratedCount; ++index) {
        if (!roomPlacements[index].kind || !(model = roomPlacements[index].modelHandle) || !model->root)
            continue;
        switch (roomPlacements[index].kind) {
        case 2:
            node = FindMT7Node(model->root, 0x98);
            if (node && node != model->root)
                node->rotation.y += 0x200;
            node = FindMT7Node(model->root, 0x99);
            if (node && node != model->root)
                node->rotation.y += 0x200;
            break;
        case 3: {
            shenGetCurrentDateTime(time);
            std::int32_t seconds = time.minute * 60;
            seconds += time.second;
            std::int32_t total = static_cast<std::int8_t>(time.hour % 12);
            total = total * 720 + seconds;
            node = FindMT7Node(model->root, 0x96);
            if (node && node != model->root)
                node->rotation.z = -(total << 16) / 43200;
            node = FindMT7Node(model->root, 0x97);
            if (node && node != model->root)
                node->rotation.z = -(seconds << 16) / 3600;
            node = FindMT7Node(model->root, 0x98);
            if (node && node != model->root) {
                node->kind += 0x400;
                node->rotation.z = static_cast<std::int32_t>(dc::sdk::njSin(static_cast<std::uint16_t>(node->kind)) * 2048.0f);
            }
            break;
        }
        }
    }
    roomDrawOrigin = *origin;
    roomDrawYaw = yaw;
    HLib::ExecuteCallbackNow(DrawRoom, 0);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ExecuteRoom(void *) {
    if (!g_worldDrawingEnabled || !roomInitialized)
        return;
    switch (roomState) {
    case 0:
        switch (ADXF_GetStat(g_magicRoomFile)) {
        case 1:
            break;
        case 2:
            ++roomLoadingFrames;
            break;
        case 3:
            mapSetupLighting(1);
            roomState = 1;
            ADXF_Close(g_magicRoomFile);
            g_magicRoomFile = 0;
            if (ReadMagicRoomDatabase(roomDatabase)) {
                roomState = 4;
                break;
            }
            SetMagicRoomEntityBuffer(roomEntityLights, 2);
            InitializeMagicRoomPlacements();
            break;
        case 4:
            break;
        }
        break;
    case 1:
        if (g_magicRoomSeed == -1)
            break;
        AssignRoomLightVectors(g_magicRoomOrigin, g_magicRoomYaw);
        GenerateRoom(g_magicRoomSeed);
        InstallRoomCollision(roomPlacements, roomGeneratedCount, &g_magicRoomOrigin, g_magicRoomYaw, &g_magicRoomCollisionDimensions);
        roomPreviousSeed = g_magicRoomSeed;
        roomState = 2;
        break;
    case 2:
        if (g_magicRoomSeed == roomPreviousSeed) {
            if (!LoadRoomModels(roomPlacements, roomGeneratedCount, 0, &g_magicRoomOrigin, g_magicRoomYaw))
                roomState = 3;
            UpdateRoomAnimations(&g_magicRoomOrigin, g_magicRoomYaw);
            mapSetupLighting(0);
        } else {
            EnableRoomLights();
            flddClearAllGeneratedVolumes();
            LoadRoomModels(roomPlacements, roomGeneratedCount, 1, &g_magicRoomOrigin, g_magicRoomYaw);
            if ((roomPreviousSeed = g_magicRoomSeed) != -1) {
                AssignRoomLightVectors(g_magicRoomOrigin, g_magicRoomYaw);
                GenerateRoom(g_magicRoomSeed);
                InstallRoomCollision(roomPlacements, roomGeneratedCount, &g_magicRoomOrigin, g_magicRoomYaw, &g_magicRoomCollisionDimensions);
            } else {
                mapSetupLighting(1);
                roomState = 1;
            }
        }
        break;
    case 3:
        if (g_magicRoomSeed != roomPreviousSeed) {
            if (g_magicRoomSeed == -1) {
                roomPreviousSeed = g_magicRoomSeed;
                EnableRoomLights();
                flddClearAllGeneratedVolumes();
                LoadRoomModels(roomPlacements, roomGeneratedCount, 1, &g_magicRoomOrigin, g_magicRoomYaw);
                mapSetupLighting(1);
                roomState = 1;
            } else {
                roomState = 2;
            }
        } else {
            UpdateRoomAnimations(&g_magicRoomOrigin, g_magicRoomYaw);
        }
        break;
    case 4:
        break;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InitializeMagicRoomPlacements() {
    InitializeMagicRoomGeneration();
    g_magicRoomSeed = -1;
    roomGeneratedCount = 0;
    roomPlacements = static_cast<MagicRoomPlacement *>(HLib::AllocateTaskData(2000, TASK_MAGIC_ROOM));
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void GenerateRoom(std::int32_t seed) {
    FillMagicRoomCategorySeeds();
    roomGeneratedCount = GenerateMagicRoomPlacements(roomPlacements, 50, seed);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InstallRoomCollision(MagicRoomPlacement *placements, std::int32_t count, const Vector3 *origin, std::uint32_t yaw, const Vector3 *dimensions) {
    if (roomGeneratedCount) {
        flddClearAllGeneratedVolumes();
        flddAssignGeneratedBox(1, *origin, *dimensions, yaw);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::UnitCurrentMatrix();
        matrix_stack::TranslateVector(origin);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        for (std::int32_t index = 0; index < count; ++index) {
            if (placements[index].halfWidth == 0.0f)
                continue;
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::TranslateVector(&placements[index].position);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int16_t>(placements[index].yaw));
            Vector3 position;
            position.x = 0.0f;
            position.y = 0.0f;
            position.z = 0.0f;
            Matrix4x4::TransformPoint(position, position);
            Vector3 size;
            size.x = placements[index].halfWidth * 2.0f;
            size.y = placements[index].height;
            size.z = placements[index].halfDepth * 2.0f;
            flddAssignGeneratedBox(0, position, size, static_cast<std::uint16_t>(yaw + static_cast<std::int16_t>(placements[index].yaw)));
            PopCurrentMatrix(1);
        }
        PopCurrentMatrix(1);
    }
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static bool CheckRoomLightQuad(const Vector2 *q, float x, float z) {
    const Vector2 *a = q, *b = a + 1, *c = b + 1, *d = c + 1;
    float bx = b->x;
    float ax = a->x;
    float cross = bx - ax;
    float ay = a->y;
    cross *= z - ay;
    float by = b->y;
    cross -= (by - ay) * (x - ax);
    float zero = 0.0f;
    if (cross > zero) {
        float cx = c->x;
        cross = (cx - bx) * (z - by);
        float cy = c->y;
        cross -= (cy - by) * (x - bx);
        if (cross < zero)
            return false;
        float dx = d->x;
        cross = (dx - cx) * (z - cy);
        float dy = c[1].y;
        cross -= (dy - cy) * (x - cx);
        if (cross < zero)
            return false;
        cross = (ax - dx) * (z - dy) - (ay - dy) * (x - dx);
        if (cross < zero)
            return false;
    } else {
        float zero = 0.0f;
        if (cross < zero) {
            const float *pa = &a->x, *pb = &b->x, *pc = &c->x, *pd = &d->x;
            float cx = *pc++;
            float bx = *pb++;
            cross = cx - bx;
            float by = *pb;
            cross *= z - by;
            float cy = *pc;
            cross -= (cy - by) * (x - bx);
            if (cross > zero)
                return false;
            float dx = *pd++;
            cross = (dx - cx) * (z - cy);
            float dy = *pd;
            cross -= (dy - cy) * (x - cx);
            if (cross > zero)
                return false;
            float ax = *pa++;
            cross = (ax - dx) * (z - dy);
            float ay = *pa;
            cross -= (ay - dy) * (x - dx);
            if (cross > zero)
                return false;
        } else {
            const float *pa = &a->x, *pb = &b->x, *pc = &c->x, *pd = &d->x;
            const float *cursor = pc;
            float cx = *cursor++;
            float bx = *pb++;
            cross = cx - bx;
            float by = *pb;
            cross *= z - by;
            float cy = *cursor;
            cross -= (cy - by) * (x - bx);
            float zero = 0.0f;
            if (cross > zero) {
                float dx = *pd++;
                cross = (dx - cx) * (z - cy);
                float dy = *pd;
                cross -= (dy - cy) * (x - cx);
                if (cross < zero)
                    return false;
                float ax = *pa++;
                cross = (ax - dx) * (z - dy);
                float ay = *pa;
                cross -= (ay - dy) * (x - dx);
                if (cross < zero)
                    return false;
            } else {
                float dx = *pd++;
                cx = *pc++;
                cross = dx - cx;
                cy = *pc;
                cross *= z - cy;
                float dy = *pd;
                cross -= (dy - cy) * (x - cx);
                float zero = 0.0f;
                if (cross > zero)
                    return false;
                float ax = *pa++;
                cross = (ax - dx) * (z - dy);
                float ay = *pa;
                cross -= (ay - dy) * (x - dx);
                if (cross > zero)
                    return false;
            }
        }
    }
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t SelectRoomDescriptor(Vector3 *position, std::uint32_t *yaw, Vector3 *dimensions) {
    const MagicRoomFloorGroup *group = g_magicRoomFloors;
    const float x = position->x, y = position->y + 0.01f, z = position->z;
    std::uint32_t base = 0;
    for (; group->entryCount; ++group) {
        if (group->heightRange.x < y && y < group->heightRange.y) {
            const std::int32_t count = group->entryCount;
            const MagicRoomDescriptor *descriptor = group->entries;
            for (std::int32_t index = 0; index < count; ++index, ++descriptor) {
                if (!CheckRoomLightQuad(descriptor->quad, x, z))
                    continue;
                position->x = descriptor->center.x;
                position->z = descriptor->center.y;
                *yaw = descriptor->yaw;
                position->y = group->heightRange.x;
                dimensions->x = descriptor->size.x;
                dimensions->y = group->heightRange.y - position->y;
                dimensions->z = descriptor->size.y;
                SetMagicRoomDimensions(dimensions->x, dimensions->y, dimensions->z);
                return static_cast<std::int32_t>(base + static_cast<std::uint32_t>(index) + static_cast<std::uint32_t>(g_magicRoomSeedBase));
            }
        }
        base += static_cast<std::uint32_t>(group->entryCount);
    }
    return -1;
}
#endif

namespace {

// @verified - d
void ColdArchiveInitializationNullsub() {
}

// @verified - d
void ColdMapInitializationNullsub() {
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AssignRoomLightVectors(const Vector3 &origin, std::uint32_t yaw) {
    Vector3 position;
    Vector2 quad[4];
    Vector3 dimensions;
    GetMagicRoomDimensions(&dimensions);
    const float halfWidth = dimensions.x * 0.5f, halfDepth = dimensions.z * 0.5f;
    roomDisabledLightCount = 0;
    const float sine = dc::sdk::SinFixed(yaw), negativeCosine = -dc::sdk::CosFixed(yaw);
    quad[0].x = origin.x - (halfWidth * negativeCosine + halfDepth * sine);
    quad[0].y = origin.z - (halfWidth * sine - halfDepth * negativeCosine);
    quad[1].x = origin.x + (halfWidth * negativeCosine + halfDepth * sine);
    quad[1].y = origin.z - (halfWidth * sine - halfDepth * negativeCosine);
    quad[2].x = origin.x + (halfWidth * negativeCosine + halfDepth * sine);
    quad[2].y = origin.z + (halfWidth * sine - halfDepth * negativeCosine);
    quad[3].x = origin.x - (halfWidth * negativeCosine + halfDepth * sine);
    quad[3].y = origin.z + (halfWidth * sine - halfDepth * negativeCosine);
    const float floor = origin.y;
    for (std::int32_t index = 0; index < 256; ++index) {
        if (GetLightEntryIndex(index) != 1)
            continue;
        GetLightRuntimePosition(index, &position);
        if (position.y > floor && CheckRoomLightQuad(quad, position.x, position.z) && roomDisabledLightCount < 4) {
            DisableLightEntry(index);
            roomDisabledLights[roomDisabledLightCount++] = static_cast<std::int16_t>(index);
        }
    }
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void EnableRoomLights() {
    for (std::int32_t index = 0; index < roomDisabledLightCount; ++index)
        EnableLightEntry(static_cast<std::uint32_t>(roomDisabledLights[index]));
    roomDisabledLightCount = 0;
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SuspendMagicRoom(std::int32_t suspend) {
    if (suspend)
        roomInitialized = 0;
    else
        roomInitialized = 1;
}
#endif

// @verified - d
void SetSceneFolders() {
    ResetColdRuntimeState();
    ReadDiscNumber();
    InitializeSprites(0, 1);
    SetLoadingDrawFlags(0);
    dc::sdk::SubmitFrame();
    dc::sdk::SubmitFrame();
    char directory[256];
    std::sprintf(directory, "/SCENE/%02ld/STREAM", static_cast<long>(g_loadedActNumber));
    AllocateDIRE(directory, 1, 240);
    AllocateDIRE("MISC", 1, 35);
    AllocateDIRE("SPRITE", 1, 160);
    AllocateDIRE("MODEL/ITEM", 1, 20);
    std::sprintf(directory, "/SCENE/%02ld/SOUND", static_cast<long>(g_loadedActNumber));
    AllocateDIRE(directory, 1, 150);
    std::sprintf(directory, "/SCENE/%02ld/MODEL/CHARA", static_cast<long>(g_loadedActNumber));
    AllocateDIRE(directory, 1, 170);
    std::sprintf(directory, "/SCENE/%02ld/PACK", static_cast<long>(g_loadedActNumber));
    AllocateDIRE(directory, 1, 30);
    std::sprintf(directory, "/SCENE/%02ld/NPC", static_cast<long>(g_loadedActNumber));
    AllocateDIRE(directory, 1, 50);
    LoadColdStorageFromMisc();
    LoadAreaTable();
    LoadDoorTable();
    AllocateSTCH();
    LoadSYD();
    InitSYD();
    AllocateAndLoadMOTFiles();
    InitializeWeatherParameters();
    LoadSystemTalk();
    AllocateLCDM();
    ColdArchiveInitializationNullsub();
    ColdMapInitializationNullsub();
    AllocateARMM();
    InitializeSceneSoundBanks();
    AllocateFlagWork();
    LoadMemoData();
    npcInitData();
    AllocateFONT();
    FreeColdArchive();
    HLib::PartitionSceneHeap();
    HLib::ValidateOrResetHeap();
    ResetUnusedReloadState();
    RetireLoadingDraw();
}

// @verified - d
void ChangeModuleTask_Init(void *) {
    SetSceneFolders();
    ChangeSystemModule(SHENMUE_SYSTEM_MODULE(Hot), 0);
}

// @verified - d
void InitializeShenmue() {
    g_gameSystemsInitialized = 1;
    g_gamePauseState = 1;
    InitSceneEffects();
    ResetTaskWaitFlags();
    mstsInit();
    InitAica();
    InitializeStreamingAudio();
    InitializePlayerSystem();
    InitializeEventExecutionRegions();
    g_eventTaskFlags1 = g_eventTaskFlags2 = g_eventTaskFlags3 = 0;
    njSetFogColor(0);
}

// @verified - d
void ResetSceneRuntime() {
    FinishStreamingAudio();
    FinishAica();
    ResetSceneArchiveFileHandles();
}

// @verified - d
void ProcessSoftReset() {
    if (g_gameSystemsInitialized && (g_inputBuffer[0].pressedPriority & 0x800U) && (g_inputBuffer[0].held & 0x606U) == 0x606U) {
        dc::sdk::njSetBackColor(0xff000000, 0xff000000, 0xff000000);
        if (g_gamePauseState) {
            ShutdownSystemLibrariesNullsub();
            WaitForBackupMemoryIdle();
            syRtcFinish();
            gdFsFinish();
            typedef void (*BootExit)(std::int32_t);
            const BootExit exit = *reinterpret_cast<BootExit *>(0x8c0000e0);
            exit(1);
        } else {
            ResetSceneRuntime();
            ChangeSystemModule(SHENMUE_FRAME_MODULE(Hot), 0);
        }
    }
}

// @verified - d
void ProcessDiscTrayState() {
    if ((dc::sdk::gdFsGetDrvStat() == 6 || g_discTrayFrames > 0) && ++g_discTrayFrames > 5) {
        ShutdownSystemLibrariesNullsub();
        InitializeSystemLibrariesNullsub();
        ShutdownSystemLibrariesNullsub();
        WaitForBackupMemoryIdle();
        syRtcFinish();
        gdFsFinish();
        typedef void (*BootExit)(std::int32_t);
        const BootExit exit = *reinterpret_cast<BootExit *>(0x8c0000e0);
        exit(1);
    }
}

// @verified - d
std::int32_t IsDiscTrayOpen() {
    return g_discTrayFrames > 0;
}

// @verified - d
bool SetTargetFramerate(std::uint32_t framesPerSecond) {
    if (framesPerSecond > 60)
        return false;
    g_defaultVblankInterval = 60 / framesPerSecond;
    g_savedFrameRateVblankInterval = g_defaultVblankInterval;
    g_frameRateVblankInterval = g_defaultVblankInterval;
    return true;
}

inline std::int32_t GetInterruptMask() {
    std::int32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status &= 0xf0;
    return status >> 4;
}

inline void SetInterruptMask(std::int32_t mask) {
    std::int32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status &= ~0xf0;
    status |= mask << 4;
    asm volatile("ldc %0,sr" : : "r"(status) : "memory");
}

// @verified - d
void FrameInterruptCallback() {
    ++*const_cast<std::uint32_t *>(&g_vblankCounter);
    FrameInterruptNullsub();
}

// @verified - d
void WaitForVblankCounter(std::uint32_t counter) {
    while (g_vblankCounter < counter) {
    }
}

// @verified - d
void SetFrameInterruptCallback(FrameInterrupt callback) {
    const std::int32_t previousMask = GetInterruptMask();
    std::int32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status |= 0xf0;
    asm volatile("ldc %0,sr" : : "r"(status) : "memory");
    g_frameInterrupt = callback;
    SetInterruptMask(previousMask);
}

// @verified - d
void DispatchFrameInterrupt() {
    if (g_frameInterrupt)
        g_frameInterrupt();
}

// @verified - d
void InitializeFrameInterrupt() {
    g_frameInterrupt = SHENMUE_FRAME_NULL;
    const std::int32_t previousMask = GetInterruptMask();
    std::int32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status |= 0xf0;
    asm volatile("ldc %0,sr" : : "r"(status) : "memory");
    const std::int32_t handle = syChainAddHandler(0x900, DispatchFrameInterrupt, 0x38, SHENMUE_FRAME_NULL);
    SetInterruptMask(previousMask);
    if (!handle)
        asm volatile("trapa #0x33");
}

void ShutdownFrameInterruptNullsub() {
}

// @verified - d
void FramePostUpdateCallback() {
    g_framePostUpdateTicks = g_frameTimerStart - *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister);
    HLib::ProcessTaskPostUpdateCallbacks();
}

void RootTaskCallback(void *) {
    const std::uint32_t timer = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    const std::uint32_t historyIndex = g_frameCounter & 31U;
    g_frameMicrosecondsHistory[historyIndex] = static_cast<float>(g_previousFrameTimer - timer) / 12.5f;
    g_frameMicroseconds = g_frameMicrosecondsHistory[historyIndex];
    WaitForVblankCounter(g_previousVblankCounter + g_frameRateVblankInterval - 1U);
    pvrMetricFrame ^= 1U;
    if (pvrMetricFrame) {
        const volatile std::uint32_t *const metrics = reinterpret_cast<volatile std::uint32_t *>(0xa05f8124);
        g_pvrTaOlBase = metrics[0];
        g_pvrTaIspBase = metrics[1];
        g_pvrTaOlLimit = metrics[2];
        g_pvrTaIspLimit = metrics[3];
        g_pvrTaNextOpb = metrics[4];
        g_pvrTaItpCurrent = metrics[5];
    }
    dc::sdk::GetVertexBufferUsage(SHENMUE_FRAME_DATA(g_vertexBufferUsage));
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(SHENMUE_FRAME_COUNT(g_vertexBufferUsage)); ++index) {
        if (static_cast<std::int32_t>(g_vertexBufferHighWater[index]) < static_cast<std::int32_t>(g_vertexBufferUsage[index]))
            g_vertexBufferHighWater[index] = g_vertexBufferUsage[index];
    }
    g_additionalVertexBufferUsage = GetAdditionalVertexBufferUsage();
    if (static_cast<std::int32_t>(g_additionalVertexBufferHighWater) < static_cast<std::int32_t>(g_additionalVertexBufferUsage))
        g_additionalVertexBufferHighWater = g_additionalVertexBufferUsage;
    dc::sdk::CollectSystemMetrics();
    SnapshotPeripheralInputs();
    dc::sdk::SubmitFrame();
    g_frameTimerStart = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    dc::sdk::SnapshotVertexBufferPointers();
    const std::uint32_t vblankCounter = g_vblankCounter & 0xffffU;
    g_vblankCounter = vblankCounter;
    g_previousVblankCounter = vblankCounter;
    g_previousFrameTimer = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    UpdateInputsFromInputDevices();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    dc::sdk::ResetGraphicsWorkspace();
#else
    dc::sdk::ExecuteAudioMiddleware();
#endif
    ProcessDiscFileReads();
    if (g_softResetProcessingEnabled)
        ProcessSoftReset();
    if (g_backupProcessingEnabled)
        ProcessDiscTrayState();
    ++g_frameCounter;
}

// @verified - d
void RootTaskInitialize(void *) {
    ExecuteSystemModule(SHENMUE_FRAME_MODULE(Cold), 0);
    HLib::SetCurrentTaskTo(RootTaskCallback);
}

// @verified - d
void SetSoftResetProcessing(std::int32_t enabled) {
    g_softResetProcessingEnabled = enabled;
}

// @verified - d
void SetBackupProcessing(std::int32_t enabled) {
    g_backupProcessingEnabled = enabled;
}

// @verified - d
void InitializeFrameTiming() {
    SetTargetFramerate(30);
    g_frameCounter = 0;
    g_vblankCounter = 0;
    g_loadingFlag = 0;
    g_developerDisplayEnabled = 0;
    g_gameSystemsInitialized = 0;
    SetSoftResetProcessing(1);
    SetBackupProcessing(1);
}

// @verified - d
void InitializeMemoryAndHLib() {
    {
        volatile std::uint32_t *timer = reinterpret_cast<volatile std::uint32_t *>(kTimerStartRegister);
        *reinterpret_cast<volatile std::uint8_t *>(timer) &= 0xfeU;
        *++timer = UINT32_MAX;
        *++timer = UINT32_MAX;
        *reinterpret_cast<volatile std::uint16_t *>(kTimerControlRegister) = 0;
        *reinterpret_cast<volatile std::uint8_t *>(kTimerStartRegister) |= 1U;
    }
    std::uint32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status |= 0xf0U;
    asm volatile("ldc %0,sr" : : "r"(status) : "memory");
    syHwInit();
    InitializeFrameTiming();
    g_discTrayFrames = 0;
    switch (static_cast<std::uint32_t>(syCblCheck())) {
    case 1:
        dc::sdk::InitializeSystem(0, 0x80, 1);
        break;
    case 2:
        dc::sdk::InitializeSystem(19, 0x80, 1);
        break;
    case 0:
    default:
        dc::sdk::InitializeSystem(9, 0x80, 1);
        break;
    }
    syHwInit2();
    {
        volatile std::uint32_t *timer = reinterpret_cast<volatile std::uint32_t *>(kTimerStartRegister);
        *reinterpret_cast<volatile std::uint8_t *>(timer) &= 0xfeU;
        *++timer = UINT32_MAX;
        *++timer = UINT32_MAX;
        *reinterpret_cast<volatile std::uint16_t *>(kTimerControlRegister) = 0;
        *reinterpret_cast<volatile std::uint8_t *>(kTimerStartRegister) |= 1U;
    }
    typedef void (*CacheConfigurator)(std::uint32_t);
    const CacheConfigurator configureCache = reinterpret_cast<CacheConfigurator>(reinterpret_cast<std::uintptr_t>(&ConfigureCpuCache) | kUncachedAlias);
    configureCache(0x105);
    std::uint32_t restoredStatus;
    asm volatile("stc sr,%0" : "=r"(restoredStatus));
    restoredStatus &= ~0xf0U;
    asm volatile("ldc %0,sr" : : "r"(restoredStatus) : "memory");
    *reinterpret_cast<volatile std::uint32_t *>(kG2DsTimeoutRegister) = 0x1b;
    *reinterpret_cast<volatile std::uint32_t *>(kG2TrTimeoutRegister) = 0x27a;
    dc::sdk::InitializeRenderingMode();
    InitializeSprites(0, 0);
    InitializePeripheralInput();
    dc::sdk::RegisterWeatherTaskCallback(FramePostUpdateCallback);
    dc::sdk::InitializeMT7Materials();
    InitializeSystemLibrariesNullsub();
    InitializeBackupMemory();
    syRtcInit();
    matrix_stack::MatrixStack_InitTransformMatrixStack(primaryMatrixStorage, 32);
    matrix_stack::SetActiveStack(matrix_stack::Temporary2);
    matrix_stack::SetActiveStack(matrix_stack::Primary);
    matrix_stack::SetupStack();
    dc::sdk::InitializeMatrixConstantsNullsub();
    dc::sdk::njSetBackColor(0xff000000, 0xff000000, 0xff000000);
    InitializeTextureCellStorage();
    InitializeLightRuntimeTables();
    InitializeDiscFileSystem();
    InitializeFrameInterrupt();
    SetFrameInterruptCallback(FrameInterruptCallback);
    InitializeInputSlots();
    HLib::InitializeAllocator();
    HLib::InitializeTasks(RootTaskInitialize);
}

// @verified - d
void Initialization() {
    volatile std::uint32_t *timer = reinterpret_cast<volatile std::uint32_t *>(kTimerStartRegister);
    *reinterpret_cast<std::uint8_t *>(kTimerStartRegister) &= 0xfeU;
    *++timer = UINT32_MAX;
    *++timer = UINT32_MAX;
    *reinterpret_cast<std::uint16_t *>(kTimerControlRegister) = 0;
    *reinterpret_cast<std::uint8_t *>(kTimerStartRegister) |= 1U;
    ResetSceneRuntime();
    g_discTrayFrames = 0;
    dc::sdk::InitializeRenderingMode();
    InitializePeripheralInput();
    dc::sdk::InitializeMT7Materials();
    InitializeSystemLibrariesNullsub();
    InitializeBackupMemory();
    syRtcInit();
    matrix_stack::MatrixStack_InitTransformMatrixStack(primaryMatrixStorage, 32);
    matrix_stack::SetActiveStack(matrix_stack::Temporary2);
    matrix_stack::SetActiveStack(matrix_stack::Primary);
    matrix_stack::SetupStack();
    dc::sdk::InitializeMatrixConstantsNullsub();
    dc::sdk::njSetBackColor(0xff000000, 0xff000000, 0xff000000);
    dc::sdk::SubmitFrame();
    std::uint32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status |= 0xf0U;
    asm volatile("ldc %0,sr" : : "r"(status) : "memory");
    SetFrameInterruptCallback(FrameInterruptCallback);
    dc::sdk::RegisterWeatherTaskCallback(FramePostUpdateCallback);
    ConfigureCpuCache(0x105);
    std::uint32_t restoredStatus;
    asm volatile("stc sr,%0" : "=r"(restoredStatus));
    restoredStatus &= ~0xf0U;
    asm volatile("ldc %0,sr" : : "r"(restoredStatus) : "memory");
    InitializeInputSlots();
    InitAica();
    InitializeStreamingAudio();
    ResetUnusedReloadState();
    LoadFonts();
    scenInitCallbacks();
}

// @verified - d
void ResetMapRenderState() {
    ReloadModelResourcesNullsub();
    ReleaseWeatherTextureSurfaces();
    InitializeSprites(1, 1);
    ResetUnusedReloadState();
}

// @verified - d
void StartupNullsub() {
}

} // namespace s2
} // namespace shenmue

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void __attribute__((renesas)) shenmue_s2_event_GetMagicRoomDebugState() {
    shenmue::s2::SelectMagicRoomForPlayer();
}
extern "C" void __attribute__((renesas)) shenmue_s2_event_SetMagicRoomDebugState() {
    shenmue::s2::LeaveMagicRoom();
}
extern "C" void __attribute__((renesas)) shenmue_s2_event_MagicRoom_Enqueue_1(
    const char *directory, const char *archive, const shenmue::s2::MagicRoomFloorGroup *groups, std::int32_t seed) {
    shenmue::s2::QueueMagicRoom(directory, archive, groups, seed);
}
#endif
#undef SHENMUE_FRAME_NULL
#undef SHENMUE_FRAME_COUNT
#undef SHENMUE_FRAME_DATA
#undef SHENMUE_FRAME_MODULE

#endif
