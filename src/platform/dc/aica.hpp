#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || SHENMUE_IS_S2)
#include "common/types.hpp"
#include <stddef.h>

extern "C" {
std::int32_t syG2Read(
    const void *source, void *destination, std::int32_t width, std::int32_t count, std::int32_t sourceIncrement, std::int32_t destinationIncrement);
std::int32_t syG2Write(
    const void *source, void *destination, std::int32_t width, std::int32_t count, std::int32_t sourceIncrement, std::int32_t destinationIncrement);
}
namespace shenmue {
namespace s1 {
static const std::uintptr_t kAicaRamBegin = 0xa0800000;
static const std::uintptr_t kAicaRamModeRegister = 0xa0702800;
static const std::uintptr_t kAicaArmControlRegister = 0xa0702c00;
static const std::uintptr_t kAicaCommandRingBegin = kAicaRamBegin + 0x400;
static const std::uintptr_t kAicaCommandRingEnd = kAicaRamBegin + 0x500;
static const std::uint32_t kAicaPollLimit = 0x400000;
static const std::size_t kAicaBankCount = 8;
struct AicaBankRegion {
    std::int32_t bankId;
    std::uintptr_t address;
    std::uint32_t size;
};

struct AicaStreamSlot {
    std::int8_t state;
    std::uint8_t flags;
    std::int16_t bitsPerSample;
    std::uintptr_t leftBuffer;
    std::uintptr_t rightBuffer;
    std::uint32_t sampleRate;
    volatile std::uint32_t *bufferState;
    volatile std::uint32_t *playState;
#if SHENMUE_IS_S2
    volatile std::uint32_t *hardware;
#endif
};

typedef char AicaLayoutCheck1[(sizeof(AicaBankRegion) == 0xc) ? 1 : -1];
typedef char AicaLayoutCheck2[(offsetof(AicaBankRegion, address) == 4) ? 1 : -1];
typedef char AicaLayoutCheck3[(offsetof(AicaBankRegion, size) == 8) ? 1 : -1];
typedef char AicaLayoutCheck4[(sizeof(AicaStreamSlot) == (SHENMUE_IS_S2 ? 0x1c : 0x18)) ? 1 : -1];
typedef char AicaLayoutCheck5[(offsetof(AicaStreamSlot, leftBuffer) == 4) ? 1 : -1];
typedef char AicaLayoutCheck6[(offsetof(AicaStreamSlot, rightBuffer) == 8) ? 1 : -1];
typedef char AicaLayoutCheck7[(offsetof(AicaStreamSlot, bufferState) == 0x10) ? 1 : -1];
typedef char AicaLayoutCheck8[(offsetof(AicaStreamSlot, playState) == 0x14) ? 1 : -1];

} // namespace s1
} // namespace shenmue
#if SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
using s1::AicaBankRegion;
using s1::AicaStreamSlot;
using s1::kAicaRamBegin;
using s1::kAicaRamModeRegister;
using s1::kAicaArmControlRegister;
using s1::kAicaCommandRingBegin;
using s1::kAicaCommandRingEnd;
using s1::kAicaPollLimit;
using s1::kAicaBankCount;
#else
namespace shenmue {
namespace s1 {
#endif
#if SHENMUE_IS_S2
struct AicaBankUploadWork {
    std::uint32_t state;
    std::uint32_t wordsPerFrame;
    std::uint32_t allocationBytes;
    std::uint32_t remainingBytes;
    std::uint32_t shiftedBytes;
    const std::uint32_t *source;
    std::uintptr_t destination;
    std::uintptr_t beginAddress;
};
typedef char AicaLayoutCheck9[(sizeof(AicaBankUploadWork) == 0x20) ? 1 : -1];
typedef char AicaLayoutCheck10[(offsetof(AicaBankUploadWork, source) == 0x14) ? 1 : -1];
typedef char AicaLayoutCheck11[(offsetof(AicaBankUploadWork, beginAddress) == 0x1c) ? 1 : -1];
#endif
extern AicaBankRegion g_aicaBankRegions[kAicaBankCount];
extern AicaStreamSlot g_aicaStreamSlots[kAicaBankCount];
extern std::uint32_t g_aicaStreamBufferBytes;
std::uint32_t ReadAicaWord(const volatile std::uint32_t *source);
void WriteAicaWord(std::uint32_t value, volatile std::uint32_t *destination);
std::int32_t SOND_ExecuteRawAicaCommand(std::uint32_t command);
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
std::int32_t ExecuteCdAicaCommand(std::uint32_t command, std::uint32_t volume);
std::int32_t SOND_ConsumeDriverEvent();
#endif
#if SHENMUE_IS_S2
std::int32_t SOND_QueryBankPlaybackState(std::int32_t bank);
std::int32_t SOND_QueryBankLevel(std::uint32_t bank, std::uint32_t command);
std::int32_t nlsndExecuteAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume);
std::int32_t nlsndExecuteAicaCommand2Ch(std::uint32_t bank, std::uint32_t channel, std::uint32_t command, std::int32_t volume);
std::int32_t nlsndExecuteEnvironmentAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume);
#else
std::int32_t ExecuteAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume);
std::int32_t ExecuteAicaCommand2Ch(std::uint32_t bank, std::uint32_t channel, std::uint32_t command, std::int32_t volume);
std::int32_t ExecuteEnvironmentAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume);
#endif
std::int32_t SOND_InitializeAicaDriver(const std::uint32_t *driver, std::uint32_t mode);
std::int32_t SOND_ConfigureStreamSlots(std::int16_t mono, std::int16_t stereo, std::int16_t mode);
std::int32_t SOND_CheckBankRelease(std::int32_t bank);
std::uintptr_t SOND_ReserveBankRegion(std::int32_t bank, std::int32_t bankId, std::uint32_t size, std::uint32_t *movedBytes);
std::uintptr_t SOND_ReleaseBankRegion(std::int32_t bank, std::uint32_t size, std::uint32_t *movedBytes);
std::int32_t SOND_MoveAicaMemory(std::uintptr_t source, std::uintptr_t destination, std::uint32_t size);
std::int32_t SOND_IsMemoryMovePending();
std::int32_t SOND_ReleaseBank(std::int32_t bank);
#if SHENMUE_IS_S2
std::int32_t SOND_CheckBankUpload(const std::uint32_t *data, std::int32_t bank, std::uint32_t reservedBytes);
std::int32_t SOND_UploadBank(const std::uint32_t *data, std::int32_t bank);
std::int32_t SOND_UploadBank(const std::uint32_t *data, std::int32_t bank, std::uint32_t reservedBytes);
std::int32_t SOND_CheckBankReplacement(const std::uint32_t *data, std::int32_t bank);
std::int32_t SOND_ReplaceBank(const std::uint32_t *data, std::int32_t bank);
std::int32_t SOND_BeginBankUpload(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame);
std::int32_t SOND_BeginBankUpload(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame, std::uint32_t reservedBytes);
std::int32_t SOND_BeginBankReplacement(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame);
std::int32_t SOND_BeginStreamReplacement(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame);
std::int32_t SOND_WriteStreamChunk(const std::uint32_t *data);
std::int32_t SOND_SetUploadWords(std::uint32_t wordsPerFrame);
std::int32_t SOND_PollBankUpload();
std::int32_t SOND_WaitBankUploadCommand();
std::int32_t SOND_WaitBankUploadBusy();
std::int32_t SOND_WaitMemoryMove();
std::int32_t SOND_FinishBankUpload();
std::int32_t SOND_FinishAicaDriver();
#else
std::int32_t SOND_CheckBankUpload(const std::uint32_t *data, std::int32_t bank);
std::int32_t SOND_UploadBank(const std::uint32_t *data, std::int32_t bank);
std::int32_t SOND_BeginBankUpload(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame);
std::int32_t SOND_PollBankUpload();
#endif
}
} // namespace shenmue
#endif
