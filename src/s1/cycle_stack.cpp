#include "s1/cycle_stack.hpp"

#include "s1/frame_pipeline.hpp"
#include "s1/hlib.hpp"
#include "s1/subtitles.hpp"
#include "s1/task/scen.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
namespace shenmue::s1 {

#pragma pack(push, 1)
struct CycleRuntimeContext {
    std::int32_t *activeState;
    std::byte reserved_008[0x70];
    void *field_078;
    void *field_080;
    std::byte reserved_088[0x28];
    void *field_0b0;
    std::byte reserved_0b8[0x18];
    void *field_0d0;
    std::byte reserved_0d8[0xc3];
    std::uint16_t field_19b;
    std::uint8_t field_19d;
    std::byte reserved_19e[6];
    std::int64_t field_1a4;
    std::byte reserved_1ac[0x7c];
    std::uint8_t field_228;
};

struct CycleProgramEntry {
    void *field_00;
    CycleRuntimeContext *context;
    CycleProgramEntry *previous;
    CycleProgramEntry *next;
};
#pragma pack(pop)

static_assert(offsetof(CycleRuntimeContext, field_078) == 0x78);
static_assert(offsetof(CycleRuntimeContext, field_19b) == 0x19b);
static_assert(offsetof(CycleRuntimeContext, field_1a4) == 0x1a4);
static_assert(offsetof(CycleRuntimeContext, field_228) == 0x228);
static_assert(offsetof(CycleProgramEntry, context) == 8);
static_assert(offsetof(CycleProgramEntry, previous) == 0x10);
static_assert(offsetof(CycleProgramEntry, next) == 0x18);

CycleProgramEntry *g_runningCyclePrograms = nullptr;
CycleProgramEntry *g_dormantCyclePrograms = nullptr;
CycleProgramEntry *g_cycleSimulationSet = nullptr;
CycleProgramEntry *g_nearCycleQueue = nullptr;
CycleProgramEntry *g_midCycleQueue = nullptr;
CycleProgramEntry *g_farCycleQueue = nullptr;
CycleProgramEntry *g_cyclePromoteList = nullptr;
CycleProgramEntry *g_cycleLoadQueue = nullptr;

std::uint32_t g_qtePrngSeed = 1;

namespace {

CVWKRuntimeSlot *g_nozomiBikeSubtitle = nullptr;

void resetContext(CycleRuntimeContext &context) {
    context.field_1a4 = -1;
    context.field_0b0 = nullptr;
    context.field_0d0 = nullptr;
    context.field_19b = 0;
    context.field_19d = 0;
    context.field_078 = nullptr;
    context.field_080 = nullptr;
    context.field_228 = 0;
}

void moveList(CycleProgramEntry *&source, CycleProgramEntry *&destination, bool clearActiveState) {
    CycleProgramEntry *entry = source;
    while (entry) {
        CycleRuntimeContext &context = *entry->context;
        CycleProgramEntry *const next = entry->next;
        resetContext(context);

        CycleProgramEntry *const previous = entry->previous;
        if (previous) {
            previous->next = next;
            entry->previous = nullptr;
        } else {
            source = next;
        }
        if (next) {
            next->previous = previous;
        }

        entry->next = destination;
        if (destination) {
            destination->previous = entry;
        }
        destination = entry;
        if (clearActiveState) {
            *context.activeState = 0;
        }
        entry = next;
    }
    source = nullptr;
}

} // namespace

void CycleStack_ResetLists() {
    moveList(g_runningCyclePrograms, g_dormantCyclePrograms, true);
    moveList(g_cycleSimulationSet, g_nearCycleQueue, false);
    moveList(g_midCycleQueue, g_nearCycleQueue, false);
    moveList(g_farCycleQueue, g_nearCycleQueue, false);
    moveList(g_cyclePromoteList, g_nearCycleQueue, false);
    moveList(g_cycleLoadQueue, g_nearCycleQueue, false);
}

void Script_IncrementQTEMinigameRand16(ScenarioContext *context, void *rawParams) {
    constexpr std::int32_t kQteMinigameUiElement = 0x44;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (params[0] != 0) {
        g_qtePrngSeed = LcgStep(g_qtePrngSeed);
        const float unit = static_cast<float>(static_cast<double>((g_qtePrngSeed >> 16U) & 0x7fffU) * 0.000030517578125);
        context->stack->accumulator = std::bit_cast<std::uint32_t>(unit * std::bit_cast<float>(params[1]));
        return;
    }
    if (g_currUIState == -1 || g_currUIState == kQteMinigameUiElement) {
        UIBaseRuntime &qte = g_uiElements[kQteMinigameUiElement];
        if (!qte.isVisible) {
            std::int64_t (*callback)() = nullptr;
            if (qte.render) {
                qte.render();
                callback = qte.tick;
            } else if (qte.initialize) {
                qte.initialize();
                callback = qte.postInit;
            }
            if (callback) {
                g_waitingForNextTick = 0;
                callback();
                g_waitingForNextTick = 1;
            }
            qte.isVisible = 1;
        }
        qte.isHandledExternally = 0;
    }
    g_qtePrngSeed = params[1];
    context->stack->accumulator = 0;
}

void Script_StartNozomiBikeSeq(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const std::uint32_t start = *static_cast<const std::uint32_t *>(rawParams);
    if (start == 0) {
        if (g_nozomiBikeSubtitle)
            shenForceClearCurrentSubtitles(g_nozomiBikeSubtitle);
        return;
    }



    g_nozomiBikeSubtitle = ProcessSubtitleFile("NOZOMIBIKE", 0, 1, nullptr);
    if (!g_nozomiBikeSubtitle || !g_pCurrentTaskData) {
        context->stack->accumulator = 0;
        return;
    }

    const auto *const pointer = reinterpret_cast<const std::byte *>(g_nozomiBikeSubtitle);
    if (pointer < g_pCurrentTaskData) {
        context->stack->accumulator = 0;
        return;
    }
    const std::uint64_t offset = static_cast<std::uint64_t>(pointer - g_pCurrentTaskData);
    context->stack->accumulator = offset <= (std::numeric_limits<std::uint32_t>::max)() ? static_cast<std::uint32_t>(offset) : 0;
}

} // namespace shenmue::s1
