#include "s2/event_overlay.hpp"

#if SHENMUE_IS_DC

#if SHENMUE_S2_GCC295
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
namespace std {}
#define SHENMUE_EVENT_NULL 0
#else
#include <array>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define SHENMUE_EVENT_NULL nullptr
#endif
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/map_control_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/load_title.hpp"
#endif

// The cache-control tails must execute through P2, with no cached stack accesses while CCR changes.
asm("\n"
    "    .text\n"
    "    .align 2\n"
    "    .global s2_dc_copy_event_words\n"
    "    .type s2_dc_copy_event_words,@function\n"
    "s2_dc_copy_event_words:\n"
    "    add #3,r6\n"
    "    shlr2 r6\n"
    ".Levent_copy_words:\n"
    "    mov.l @r5+,r0\n"
    "    dt r6\n"
    "    mov.l r0,@r4\n"
    "    bf/s .Levent_copy_words\n"
    "    add #4,r4\n"
    "    rts\n"
    "    nop\n"
    "    .size s2_dc_copy_event_words,.-s2_dc_copy_event_words\n"
    "\n"
    "    .align 2\n"
    "    .global s2_dc_copy_event_aligned\n"
    "    .type s2_dc_copy_event_aligned,@function\n"
    "s2_dc_copy_event_aligned:\n"
    "    add #31,r6\n"
    "    mov #-5,r0\n"
    "    add #32,r4\n"
    "    shld r0,r6\n"
    "    fschg\n"
    ".Levent_copy_aligned:\n"
    "    fmov @r5+,dr0\n"
    "    fmov @r5+,dr2\n"
    "    fmov @r5+,dr4\n"
    "    fmov @r5+,dr6\n"
    "    dt r6\n"
    "    fmov dr6,@-r4\n"
    "    fmov dr4,@-r4\n"
    "    fmov dr2,@-r4\n"
    "    fmov dr0,@-r4\n"
    "    bf/s .Levent_copy_aligned\n"
    "    add #64,r4\n"
    "    rts\n"
    "    fschg\n"
    "    .size s2_dc_copy_event_aligned,.-s2_dc_copy_event_aligned\n"
    "\n"
    "    .align 2\n"
    "    .global s2_dc_flush_event_operand_cache\n"
    "    .type s2_dc_flush_event_operand_cache,@function\n"
    "s2_dc_flush_event_operand_cache:\n"
    "    stc sr,r5\n"
    "    mov.w .Levent_flush_mask,r1\n"
    "    or r5,r1\n"
    "    ldc r1,sr\n"
    "    mov #-12,r2\n"
    "    extu.b r2,r2\n"
    "    swap.b r2,r2\n"
    "    swap.w r2,r2\n"
    "    mov #2,r3\n"
    "    swap.b r3,r3\n"
    "    mov #-1,r4\n"
    "    extu.b r4,r4\n"
    "    swap.b r4,r4\n"
    "    swap.w r4,r4\n"
    "    mov #28,r0\n"
    "    or r0,r4\n"
    "    mov #62,r6\n"
    "    shll2 r6\n"
    "    shll2 r6\n"
    "    mov #-96,r1\n"
    "    extu.b r1,r1\n"
    "    swap.b r1,r1\n"
    "    swap.w r1,r1\n"
    "    mova .Levent_flush_p2,r0\n"
    "    or r0,r1\n"
    "    jmp @r1\n"
    "    nop\n"
    ".Levent_flush_mask:\n"
    "    .word 0x00e0\n"
    "    .align 2\n"
    ".Levent_flush_p2:\n"
    "    mov r2,r7\n"
    "    mov.l @r2,r0\n"
    "    and r6,r7\n"
    "    or r7,r0\n"
    "    dt r3\n"
    "    ocbp @r0\n"
    "    bf/s .Levent_flush_p2\n"
    "    add #32,r2\n"
    "    mov #0,r0\n"
    "    mov.l r0,@r4\n"
    "    .rept 9\n"
    "    nop\n"
    "    .endr\n"
    "    ldc r5,sr\n"
    "    nop\n"
    "    nop\n"
    "    rts\n"
    "    nop\n"
    "    .size s2_dc_flush_event_operand_cache,.-s2_dc_flush_event_operand_cache\n"
    "\n"
    "    .align 2\n"
    "    .global s2_dc_set_event_cache_control\n"
    "    .type s2_dc_set_event_cache_control,@function\n"
    "s2_dc_set_event_cache_control:\n"
    "    stc sr,r2\n"
    "    mov.w .Levent_control_mask,r1\n"
    "    or r2,r1\n"
    "    ldc r1,sr\n"
    "    mov.l .Levent_control_tail,r0\n"
    "    mov.l .Levent_p2_alias,r1\n"
    "    or r1,r0\n"
    "    mov.l .Levent_ccr,r1\n"
    "    mov.l .Levent_control_return,r3\n"
    "    jmp @r0\n"
    "    nop\n"
    ".Levent_control_mask:\n"
    "    .word 0x00e0\n"
    "    .align 2\n"
    ".Levent_control_tail:\n"
    "    .long .Levent_control_p2\n"
    ".Levent_p2_alias:\n"
    "    .long 0xa0000000\n"
    ".Levent_ccr:\n"
    "    .long 0xff00001c\n"
    ".Levent_control_return:\n"
    "    .long .Levent_control_done\n"
    ".Levent_control_p2:\n"
    "    mov.l @r1,r0\n"
    "    or r4,r0\n"
    "    mov.l r0,@r1\n"
    "    .rept 8\n"
    "    nop\n"
    "    .endr\n"
    "    ldc r2,sr\n"
    "    jmp @r3\n"
    "    nop\n"
    ".Levent_control_done:\n"
    "    rts\n"
    "    nop\n"
    "    .size s2_dc_set_event_cache_control,.-s2_dc_set_event_cache_control\n");

namespace shenmue {
namespace s2 {

EventOverlaySlot g_eventOverlaySlots[4];

// @verified - d
const char *GetAreaSlotName(std::uint32_t slot) {
    if (slot < 4)
        return g_eventOverlaySlots[slot].name;
    return SHENMUE_EVENT_NULL;
}

// @verified - d
void scenFreeEvent(const char *requestedScenario) {
    for (std::int32_t slot = 1; slot < 4; ++slot) {
        if (std::strcmp(requestedScenario, g_eventOverlaySlots[slot].name) == 0) {
            std::memset(g_eventOverlaySlots[slot].name, 0, sizeof(g_eventOverlaySlots[slot].name));
            return;
        }
    }
}

// @verified - d
std::int32_t shenGetAreaSlotByName(const char *name) {
    for (std::int32_t slot = 0; slot < 4; ++slot) {
        if (std::strcmp(name, g_eventOverlaySlots[slot].name) == 0)
            return slot;
    }
    return -1;
}

// @verified - d
void scenFreeEventByIndex(std::uint32_t slot) {
    scenFreeEvent(GetAreaSlotName(slot));
}

// @verified - d
bool shenLoadEventFrom(const char *name, EventFunctionType functionType) {
    const std::uint32_t slot = static_cast<std::uint32_t>(functionType);
    bool result;
    if (slot - 1U >= 3U) {
        result = false;
    } else {
        std::strncpy(g_eventOverlaySlots[slot].name, name, sizeof(g_eventOverlaySlots[slot].name));
        result = true;
    }
    return result;
}

// @verified - d
void scenInitCallbacks() {
    InitializeEventExecutionRegions();
}

// @verified - d
bool LoadAreaFunc_Wrapper_0(const char *name, std::int32_t force) {
    const std::size_t length = std::strlen(name);
    const std::uint32_t slot = static_cast<std::uint32_t>(std::atoi(name + (length - 1)));
    if (slot - 1U >= 3U)
        return false;
    if (force != 1 && std::strcmp(g_eventOverlaySlots[slot].name, name) == 0)
        return true;
    char directory[64];
    if (std::strncmp(name + (length - 3), "MD", 2) == 0)
        std::strcpy(directory, "module");
    else
        std::strcpy(directory, Filepath_Gen(FilepathGenMode::SceneDirectory));
    if (!shenLoadFile(directory, name, reinterpret_cast<void *>(eventExecutionRegions[slot]), UINT32_MAX))
        return false;
    std::strncpy(g_eventOverlaySlots[slot].name, name, sizeof(g_eventOverlaySlots[slot].name));
    return true;
}

inline void SetEventInterruptMask(std::uint32_t level) {
    std::uint32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status = (status & ~0xf0U) | level;
    asm volatile("ldc %0,sr" : : "r"(status));
}

inline void RunEventOverlay(std::uint32_t slot, std::uintptr_t argument) {
    SetEventInterruptMask(0xe0U);
    FlushEventOperandCache();
    SetEventCacheControl(0x105);
    SetEventInterruptMask(0);
    reinterpret_cast<ScriptAreaMainFunction>(eventExecutionRegions[slot])(argument);
}

// @verified - d
bool ExecuteAreaMainFunction(const char *name, std::uintptr_t argument) {
    const std::int32_t slot = shenGetAreaSlotByName(name);
    if (slot != -1) {
        RunEventOverlay(static_cast<std::uint32_t>(slot), argument);
        return true;
    }
    return false;
}

// @verified - d
bool ExecuteCurrentAreaMainFunction(EventFunctionType functionType, std::uintptr_t argument) {
    const char *const name = GetAreaSlotName(static_cast<std::uint32_t>(functionType));
    return name ? ExecuteAreaMainFunction(name, argument) : false;
}

#if SHENMUE_S2_GCC295
std::uintptr_t eventExecutionRegions[4] = {
#else
constexpr std::array<std::uintptr_t, 4> eventExecutionRegions{
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    0x0df80000U, 0x0c328000U, 0x0c338000U, 0x0c348000U
#else
    0x8df80000U, 0x8c328000U, 0x8c338000U, 0x8c348000U
#endif
};

void *GetEventExecutionRegion(EventFunctionType functionType) {
    const std::uint32_t type = static_cast<std::uint32_t>(functionType);
    if (type - 1U < 3U)
        return reinterpret_cast<void *>(eventExecutionRegions[type]);
    return SHENMUE_EVENT_NULL;
}

void InitializeEventExecutionRegions() {
    std::size_t size = sizeof(g_eventOverlaySlots);
    std::memset(g_eventOverlaySlots, 0, size);
    std::memset(reinterpret_cast<void *>(eventExecutionRegions[1]), 0, 0x10000);
    std::memset(reinterpret_cast<void *>(eventExecutionRegions[2]), 0, 0x10000);
    std::memset(reinterpret_cast<void *>(eventExecutionRegions[3]), 0, 0x18000);
}

const void *InstallEventOverlay(const void *payload, const char *stem, std::size_t length, EventFunctionType functionType) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    void *const destination = GetEventExecutionRegion(functionType);
    if (!destination)
        return SHENMUE_EVENT_NULL;
    if (reinterpret_cast<std::uintptr_t>(payload) & 7U)
        CopyEventOverlayWords(destination, payload, length);
    else
        CopyEventOverlayAligned(destination, payload, length);
    char eventFile[16];
    char suffix[8];
    ::strncpy(eventFile, stem, 8);
    eventFile[8] = '\0';
    ::sprintf(suffix, ".EV%01d", static_cast<std::int32_t>(functionType));
    ::strcat(eventFile, suffix);
    shenLoadEventFrom(eventFile, functionType);
    return payload;
#else
    void *const destination = GetEventExecutionRegion(functionType);
    if (!destination)
        return SHENMUE_EVENT_NULL;
    if ((reinterpret_cast<std::uintptr_t>(payload) & 7U) == 0)
        CopyEventOverlayAligned(destination, payload, length);
    else
        CopyEventOverlayWords(destination, payload, length);
    char eventFile[16];
    ::sprintf(eventFile, "%.8s.EV%01u", stem, static_cast<unsigned>(functionType));
    shenLoadEventFrom(eventFile, functionType);
    return payload;
#endif
}

void ExecuteEventOverlay(std::uint32_t slot, std::uintptr_t argument) {
    RunEventOverlay(slot, argument);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static HLib::Task *areaEventEntryTask = SHENMUE_EVENT_NULL;
static HLib::Task *startupFadeTask = SHENMUE_EVENT_NULL;
static AreaStartupFadeState startupFadeState;

// @verified - d
HLib::Task *QueueAreaEventEntry() {
    areaEventEntryTask = HLib::EnqueueTaskWithoutParameter(Startup_EventBase_Callback, 3, 0x0b, static_cast<std::int32_t>(AREA_STARTUP_TASK));
    return areaEventEntryTask;
}

void Startup_EventBase_Callback(void *) {
    stepUpdateFlagUtil(500, 0);
    const AreaTableEntry *const entry = GetAreaTableEntry(g_currentMap);
    switch (entry->startupMode) {
    case 0:
    default: {
        const char *const name = entry->startupPayload;
        LoadAreaFunc_Wrapper_0(name, 0);
        ExecuteAreaMainFunction(name, 0);
        break;
    }
    case 1:
        break;
    case 2:
    case 3:
    case 4:
        ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(entry->startupMode - 1U), 0);
        break;
    case 5:
    case 6:
    case 7:
        RequestMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), entry->startupPayload);
        pakLoadFlush();
        ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(entry->startupMode - 4U), 0);
        break;
    }
    HLib::DestroyCurrentEventTask();
}
// @verified - d
static void InitMapAndCharaFlags() {
    for (const AreaTableEntry *area = g_areaTableEntries; area->area != UINT32_MAX; ++area) {
        if (area->area != g_currentMap)
            continue;
        for (const MapEventTransition *entry = area->transitions; entry->eventId != -1; ++entry) {
            if (entry->eventId != g_entryPoint)
                continue;
            Vector3 position;
            Vector3i rotation = {};
            const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            Character *const character = control->ryo;
            if (MoveObjectToPosFromThisScene(AREA_ENTRY_PLACEMENT, g_entryPoint, position, rotation.y)) {
                SetCharacterWorldPosition(character, &position);
                charaSetWorldRot(character, rotation);
                if (HasCharacterMotionWork(character))
                    mtwkSetEntityRotation(charFindMotionwork(character), &rotation);
            } else {
                rotation.y = entry->yaw;
                SetCharacterWorldPosition(character, &entry->position);
                charaSetWorldRot(character, rotation);
                if (HasCharacterMotionWork(character))
                    mtwkSetEntityRotation(charFindMotionwork(character), &rotation);
            }
            camCtrlSetState(0);
            camCtrlSetFovTransitionTo(1);
            return;
        }
    }
}

// @verified - d
void Queue_EI_Startup_EventFade1() {
    startupFadeTask = HLib::EnqueueTaskWithoutParameter(EI_Startup_EventFade1_Callback, 3, 0x0b, static_cast<std::int32_t>(AREA_STARTUP_TASK));
    startupFadeState.retained[0] = 0;
    startupFadeState.retained[1] = 0;
    startupFadeState.nextTask = SHENMUE_EVENT_NULL;
    shenSetTimeSimulationTo(0);
    EV_setHoldStatesFlags(0);
    InitMapAndCharaFlags();
    HLib::SetupTimedTask(startupFadeTask, 0);
}

// @verified - d
void EI_Startup_EventFade1_Callback(void *) {
    EI_Startup_BeginEventFade();
    HLib::SetCurrentTaskTo(EI_Startup_EventFade2_Callback);
}

// @verified - d
void EI_Startup_EventFade2_Callback(void *) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (DoesScreenFilmColourExist(control->filmTask))
        return;
    shenSetTimeSimulationTo(1);
    EV_setHoldStatesFlags(1);
    if (startupFadeState.nextTask)
        HLib::ResolveDeferredCallback(startupFadeState.nextTask);
    HLib::DestroyCurrentEventTask();
}

void EI_Startup_BeginEventFade() {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    HLib::Task *const film = control->filmTask;
    ScreenFilmColourRequest request;
    request.durationFrames = 20;
    request.start[0] = 255;
    request.start[1] = 0;
    request.start[2] = 0;
    request.start[3] = 0;
    request.target[0] = 0;
    request.target[1] = 0;
    request.target[2] = 0;
    request.target[3] = 0;
    WaitForNextTask(film, request, 0);
    EV_spmBeginSampleExecute(0);
    ST_LoadTitleFade();
    pakLoadFlush();
}

// @verified - d
void QueueImmediateNextFadeTask(HLib::Task *task) {
    HLib::ResolveDeferredCallback(startupFadeTask);
    startupFadeState.nextTask = task;
    if (task)
        HLib::SetupTimedTask(task, 0);
}

#endif

} // namespace s2
} // namespace shenmue

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void __attribute__((renesas)) shenmue_s2_event_QueueImmediateNextFadeTask(shenmue::s2::HLib::Task *task) {
    shenmue::s2::QueueImmediateNextFadeTask(task);
}
#endif

#undef SHENMUE_EVENT_NULL

#endif
