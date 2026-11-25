#include "platform/dc/aica_stream.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2

extern "C" {
std::int32_t syG2DmaOpen(void **handle, std::int32_t channel);
std::int32_t syG2DmaClose(void *handle);
std::int32_t syG2DmaGetStat(void *handle, std::uint32_t *status);
std::int32_t syG2DmaSetPrm(void *handle, void *destination, const void *source, std::uint32_t bytes, std::uint32_t direction, std::int32_t trigger,
    std::int32_t mode, void (*callback)(void *), void *user);
std::int32_t syG2DmaTrigger(void *handle, std::int32_t mode);
std::int32_t syG2DmaStop(void *handle);
std::int32_t syG2DmaResume(void *handle);
}

namespace shenmue { namespace s2 {
AicaDmaWork g_aicaDmaWork[16]= {};
std::int32_t g_aicaDmaTransferMode = 0;

namespace {
AicaDmaWork *g_aicaDmaQueue[16]= {};
AicaDmaWork **g_aicaDmaQueueRead = 0;
AicaDmaWork **g_aicaDmaQueueWrite = 0;
void *g_aicaDmaHandle = 0;
std::int32_t g_aicaDmaState = 0;

// @verified - d
std::uint32_t GetDmaInterruptMask() {
    std::uint32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    return (status & 0xf0U) >> 4;
}

// @verified - d
void AicaDmaCompletion(void *) {
    if (g_aicaDmaQueueWrite != g_aicaDmaQueueRead)
        nlsndDmaAdvance();
}
} // namespace

// @verified - d
std::int32_t nlsndDmaInitialize() {
    g_aicaDmaState = 0;
    for (std::int32_t index = 0; index < 16; ++index) {
        g_aicaDmaWork[index].state = 0;
        g_aicaDmaQueue[index] = 0;
    }
    nlsndDmaSetTransferMode(1);
    g_aicaDmaQueueRead = g_aicaDmaQueue;
    g_aicaDmaQueueWrite = g_aicaDmaQueue;
    return syG2DmaOpen(&g_aicaDmaHandle, 0) == 0 ? 0 : -1;
}

// @verified - d
std::int32_t nlsndDmaFinish() {
    for (std::int32_t index = 0; index < 16; ++index) {
        std::int32_t result = g_aicaDmaWork[0].state & 1;
        while (result)
            result = nlsndDmaClose(g_aicaDmaWork);
    }
    return syG2DmaClose(g_aicaDmaHandle) == 0 ? 0 : -1;
}

// @verified - d
std::int32_t nlsndDmaSetTransferMode(std::int32_t mode) {
    if (mode < 0 || mode >= 2)
        return -1;
    g_aicaDmaTransferMode = mode;
    return 0;
}

// @verified - d
AicaDmaWork *nlsndDmaOpen() {
    for (std::size_t rangeIndex0 = 0; rangeIndex0 < sizeof(g_aicaDmaWork) / sizeof((g_aicaDmaWork)[0]); ++rangeIndex0) {
        AicaDmaWork & work = (g_aicaDmaWork)[rangeIndex0];

        if (!work.state) {
            work.state |= 1;
            return &work;
        }
    
    }
    return 0;
}

// @verified - d
std::int32_t nlsndDmaClose(AicaDmaWork *work) {
    if (!(work->state & 1))
        return -1;
    if (nlsndDmaGetStatus(work) != 0x100) {
        if (work->state & 0x100) {
            nlsndDmaCancel(work);
        } else {
            if (!(work->state & 0x200))
                return -0x80;
            AicaDmaWork **previous = g_aicaDmaQueueRead;
            while (*previous != work) {
                ++previous;
                if (previous == g_aicaDmaQueue + 16)
                    previous = g_aicaDmaQueue;
            }
            AicaDmaWork **next = previous + 1;
            if (next == g_aicaDmaQueue + 16)
                next = g_aicaDmaQueue;
            while (next != g_aicaDmaQueueWrite) {
                *previous = *next;
                if (++next == g_aicaDmaQueue + 16)
                    next = g_aicaDmaQueue;
                if (++previous == g_aicaDmaQueue + 16)
                    previous = g_aicaDmaQueue;
            }
            g_aicaDmaQueueWrite = previous;
        }
    }
    work->state = 0;
    return 0;
}

// @verified - d
std::int32_t nlsndDmaTransfer(AicaDmaWork *work, const void *source, void *destination, std::uint32_t bytes) {
    if (!(work->state & 1))
        return -1;
    if (reinterpret_cast<std::uintptr_t>(source) & 31U)
        return -2;
    if (reinterpret_cast<std::uintptr_t>(destination) & 31U)
        return -3;
    if (bytes & 31U)
        return -4;
    if (!g_aicaDmaTransferMode) {
        syG2Write(source, destination, 4, static_cast<std::int32_t>(bytes >> 2), 1, 1);
        work->state &= ~0x300;
    } else {
        if (g_aicaDmaTransferMode != 1)
            return -0x80;
        if (nlsndDmaGetStatus(work) != 0x100)
            return -5;
        work->source = source;
        work->destination = destination;
        work->bytes = bytes;
        work->state |= 0x200;
        *g_aicaDmaQueueWrite = work;
        if (g_aicaDmaQueueWrite == g_aicaDmaQueueRead)
            nlsndDmaBegin(work);
        if (++g_aicaDmaQueueWrite == g_aicaDmaQueue + 16)
            g_aicaDmaQueueWrite = g_aicaDmaQueue;
    }
    return 0;
}

// @verified - d
std::int32_t nlsndDmaCancel(AicaDmaWork *work) {
    if (!(work->state & 1))
        return -1;
    if (!(work->state & 0x100))
        return -2;
    if (nlsndDmaGetStatus(work) != 0x101)
        return -3;
    syG2DmaStop(g_aicaDmaHandle);
    nlsndDmaAdvance();
    return 0;
}

// @verified - d
std::int32_t nlsndDmaGetStatus(const AicaDmaWork *work) {
    if (!g_aicaDmaTransferMode)
        return 0x100;
    if (g_aicaDmaTransferMode != 1)
        return -0x80;
    const std::uint32_t interruptMask = GetDmaInterruptMask();
    std::uint32_t hardwareStatus;
    syG2DmaGetStat(g_aicaDmaHandle, &hardwareStatus);
    std::int32_t result;
    if (!(work->state & 1))
        result = -1;
    else if (!(work->state & 0x100))
        result = work->state & 0x200 ? 0x101 : 0x100;
    else
        result = hardwareStatus == 1 ? 0x101 : 0x100;
    if (!hardwareStatus && g_aicaDmaQueueWrite != g_aicaDmaQueueRead) {
        if (!interruptMask)
            nlsndDmaAdvance();
        else
            syG2DmaResume(g_aicaDmaHandle);
    }
    return result;
}

// @verified - d
std::int32_t nlsndDmaBegin(AicaDmaWork *work) {
    syG2DmaSetPrm(g_aicaDmaHandle, work->destination, work->source, work->bytes, 0, 0, 1, AicaDmaCompletion, 0);
    work->state |= 0x100;
    syG2DmaTrigger(g_aicaDmaHandle, 0);
    return 0;
}

// @verified - d
void nlsndDmaAdvance() {
    (*g_aicaDmaQueueRead)->state &= ~0x300;
    if (++g_aicaDmaQueueRead == g_aicaDmaQueue + 16)
        g_aicaDmaQueueRead = g_aicaDmaQueue;
    if (g_aicaDmaQueueWrite != g_aicaDmaQueueRead)
        nlsndDmaBegin(*g_aicaDmaQueueRead);
}
} } // namespace shenmue::s2
#endif
