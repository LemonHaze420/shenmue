#include "platform/dc/aica.hpp"
#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || SHENMUE_IS_S2)
#include "platform/dc/aica_stream.hpp"
#if SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
#else
namespace shenmue {
namespace s1 {
#endif
AicaBankRegion g_aicaBankRegions[kAicaBankCount];
AicaStreamSlot g_aicaStreamSlots[kAicaBankCount];
std::uint32_t g_aicaStreamBufferBytes = 0;
namespace {
volatile std::int32_t g_aicaCommandBusy;
volatile std::uint32_t *volatile g_aicaCommandWritePointer;
std::uintptr_t g_aicaBankDataBegin;
volatile std::uint32_t *g_aicaMemoryTable;
std::uint32_t g_aicaRamSize;
std::uint32_t g_aicaBankBytesLoaded;
#if SHENMUE_IS_S2
AicaBankUploadWork bankUploadWork;
AicaBankUploadWork *g_aicaUploadWork;
#else
std::uint32_t g_aicaBankUploadState = 0;
const std::uint32_t *g_aicaUploadSource = 0;
volatile std::uint32_t *g_aicaUploadDestination = 0;
std::uint32_t g_aicaUploadRemaining = 0;
std::uint32_t g_aicaUploadWordsPerFrame = 0;
std::uint32_t g_aicaUploadShiftedBytes = 0;
#endif
std::uint32_t g_aicaDriverVersion;
#if SHENMUE_IS_S2 || SHENMUE_IS_S1
volatile std::uint32_t *g_aicaDriverWork;
#endif
} // namespace

// @verified - d
std::uint32_t ReadAicaWord(const volatile std::uint32_t *source) {
#if SHENMUE_IS_S2
    std::uint32_t value;
    syG2Read(const_cast<const std::uint32_t *>(source), &value, 4, 1, 0, 0);
    return value;
#else
    return *source;
#endif
}

// @verified - d
void WriteAicaWord(std::uint32_t value, volatile std::uint32_t *destination) {
#if SHENMUE_IS_S2
    syG2Write(&value, const_cast<std::uint32_t *>(destination), 4, 1, 0, 0);
#else
    *destination = value;
#endif
}

// @verified - d
std::int32_t SOND_ExecuteRawAicaCommand(std::uint32_t command) {
    if (g_aicaCommandBusy)
        return -3;
    g_aicaCommandBusy = -1;
    std::int32_t result = 0;
    if ((command & 0x80U) == 0) {
        result = -2;
    } else if (ReadAicaWord(g_aicaCommandWritePointer)) {
        result = -1;
    } else {
        volatile std::uint32_t *const destination = g_aicaCommandWritePointer;
        g_aicaCommandWritePointer = destination + 1;
        WriteAicaWord(command, destination);
        if (reinterpret_cast<std::uintptr_t>(g_aicaCommandWritePointer) == kAicaCommandRingEnd)
            g_aicaCommandWritePointer = reinterpret_cast<volatile std::uint32_t *>(kAicaCommandRingBegin);
    }
    g_aicaCommandBusy = 0;
    return result;
}

// @verified - d
#if SHENMUE_IS_S2
std::int32_t nlsndExecuteAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume) {
#else
std::int32_t ExecuteAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume) {
#endif
    std::uint32_t packed = command + ((bank & 15U) << 24);
    if (command & 0xff00U) {
        switch (command) {
        case 0x1a0:
        case 0x4a0:
        case 0x9a0:
        case 0xaa0:
        case 0x10a0:
#if SHENMUE_IS_S2
        case 0x19a0:
        case 0x1ca0:
#endif
            packed += (static_cast<std::uint32_t>(volume) & 127U) << 16;
            break;
        case 0x5a0:
        case 0x6a0:
        case 0x7a0:
        case 0x11a0:
            packed += ((static_cast<std::uint32_t>(volume) + 64U) & 127U) << 16;
            break;
        case 0x1ba0:
            packed += (static_cast<std::uint32_t>(volume) & 15U) << 16;
            break;
        case 0x30a0:
#if SHENMUE_IS_S2 || SHENMUE_IS_S1
        case 0x28a0:
        case 0x29a0:
#endif
            packed += ((static_cast<std::uint32_t>(volume) - 1U) & 15U) << 16;
            break;
#if SHENMUE_IS_S2 || SHENMUE_IS_S1
        case 0x2a0:
        case 0x3a0:
            break;
        default:
            return -2;
#endif
        }
    }
    return SOND_ExecuteRawAicaCommand(packed);
}

// @verified - d
#if SHENMUE_IS_S2
std::int32_t nlsndExecuteAicaCommand2Ch(std::uint32_t bank, std::uint32_t channel, std::uint32_t command, std::int32_t volume) {
#else
std::int32_t ExecuteAicaCommand2Ch(std::uint32_t bank, std::uint32_t channel, std::uint32_t command, std::int32_t volume) {
#endif
    std::uint32_t packed = ((channel - 1U) & 15U) + ((bank & 15U) << 24) + command;
    switch (command) {
    case 0x1b0:
    case 0x2b0:
    case 0x7b0:
    case 0x27b0:
#if SHENMUE_IS_S2
    case 0x40b0:
    case 0x41b0:
#endif
        packed += (static_cast<std::uint32_t>(volume) & 127U) << 16;
        break;
    case 0xab0:
    case 0x2ab0:
        packed += ((static_cast<std::uint32_t>(volume) + 64U) & 127U) << 16;
        break;
#if SHENMUE_IS_S2
    case 0xcb0:
    case 0xdb0:
    case 0x2cb0:
    case 0x2db0:
        packed += (static_cast<std::uint32_t>(volume) & 15U) << 16;
        break;
    case 0x78b0:
    case 0x7bb0:
        break;
#endif
    default:
        return -2;
    }
    return SOND_ExecuteRawAicaCommand(packed);
}

// @verified - d
#if SHENMUE_IS_S2
std::int32_t nlsndExecuteEnvironmentAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume) {
#else
std::int32_t ExecuteEnvironmentAicaCommand(std::uint32_t bank, std::uint32_t command, std::int32_t volume) {
#endif
    const std::uint32_t value = static_cast<std::uint32_t>(volume);
    std::uint32_t packed = command + ((bank & 15U) << 8);
    switch (command) {
    case 0xa5: {
        const std::int32_t result = SOND_ExecuteRawAicaCommand(((value & 0x7f00U) << 8) + ((bank & 15U) << 8) + 0x70a5U);
        if (result)
            return result;
        packed = ((bank & 15U) << 8) + ((value & 127U) << 16) + 0xa5U;
        break;
    }
    case 0x10a5:
        packed += (value & 127U) << 16;
        break;
    case 0xa7:
    case 0x10a7:
    case 0x20a5:
    case 0x30a5:
    case 0x40a5:
        packed += ((value + 64U) & 127U) << 16;
        break;
    case 0x50a5:
        packed += (value & 15U) << 16;
        break;
    case 0xa6:
        packed += ((value & 127U) << 16) + ((value & 128U) << 5);
        break;
    default:
        return -2;
    }
    return SOND_ExecuteRawAicaCommand(packed);
}

#if SHENMUE_IS_S1 || SHENMUE_IS_S2
// @verified - d
std::int32_t SOND_QueryBankLevel(std::uint32_t bank, std::uint32_t command) {
    if (bank > 7)
        return -128;
#if SHENMUE_IS_S2
    const volatile std::uint32_t *const level =
        reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + ReadAicaWord(g_aicaDriverWork) + (bank * 8 + 5) * sizeof(std::uint32_t));
    if (command == 0x4a0)
        return static_cast<std::int32_t>(ReadAicaWord(level) & 127U);
    if (command == 0x10a0)
        return static_cast<std::int32_t>((ReadAicaWord(level) & 0x7f00U) >> 8U);
#else
    const volatile std::uint32_t *const level =
        reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + g_aicaDriverWork[0] + (bank * 8 + 5) * sizeof(std::uint32_t));
    if (command == 0x4a0)
        return static_cast<std::int32_t>(*level & 127U);
    if (command == 0x10a0)
        return static_cast<std::int32_t>((*level & 0x7f00U) >> 8U);
#endif
    return -128;
}
#endif

#if SHENMUE_IS_S1 || SHENMUE_IS_S2
// @verified - d
std::int32_t ExecuteCdAicaCommand(std::uint32_t command, std::uint32_t volume) {
    if (command != 0xa3U)
        return -2;
    const std::uint32_t packed = (volume & 15U) * 0x10000U;
    std::int32_t result = SOND_ExecuteRawAicaCommand(packed + 0xa3U);
    if (!result)
        result = SOND_ExecuteRawAicaCommand(packed + 0x1a3U);
    return result;
}

#if SHENMUE_IS_S2
std::int32_t SOND_ConsumeDriverEvent() {
    volatile std::uint32_t *const eventWord = g_aicaDriverWork + 4;
    const std::int32_t event = static_cast<std::int32_t>(ReadAicaWord(eventWord));
    if (event)
        WriteAicaWord(0, eventWord);
    return event;
}
#else
// @verified - d
std::int32_t SOND_ConsumeDriverEvent() {
    const std::int32_t event = static_cast<std::int32_t>(g_aicaDriverWork[4]);
    if (event)
        g_aicaDriverWork[4] = 0;
    return event;
}
#endif

#endif

#if SHENMUE_IS_S1 || SHENMUE_IS_S2
// @verified - d
std::int32_t SOND_QueryBankPlaybackState(std::int32_t bank) {
#if SHENMUE_IS_S2
    const volatile std::uint32_t *state = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + ReadAicaWord(g_aicaDriverWork + 3));
    for (std::int32_t channel = 0; channel < 16; ++channel, state += 12) {
        const std::uint32_t flags = ReadAicaWord(state) & 255U;
        if ((flags & 0x80U) && static_cast<std::int32_t>((ReadAicaWord(state) >> 16U) & 255U) == bank)
            return (flags & 1U) ? 0x101 : 0x100;
    }
#else
    const volatile std::uint32_t *state = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + g_aicaDriverWork[3]);
    for (std::int32_t channel = 0; channel < 16; ++channel, state += 12) {
        if ((*state & 0x80U) && static_cast<std::int16_t>((*state >> 16U) & 255U) == bank)
            return (*state & 1U) ? 0x101 : 0x100;
    }
#endif
    return 0;
}
#endif

// @verified - d
std::int32_t SOND_InitializeAicaDriver(const std::uint32_t *driver, std::uint32_t mode) {
#if SHENMUE_IS_S2
    g_aicaDriverVersion = ReadAicaWord(driver + 0xc4 / 4);
    if (g_aicaDriverVersion < 0x6f4U)
#else
    g_aicaDriverVersion = driver[0xc4 / 4];
    if (g_aicaDriverVersion < (SHENMUE_IS_S1 ? 0x6ccU : 0x6a4U))
#endif
        return -1;
    if (mode == 0)
        g_aicaRamSize = 0x200000;
    else if (mode == 0x200)
        g_aicaRamSize = 0x800000;
    else
        return -2;
    volatile std::uint32_t *const armControl = reinterpret_cast<volatile std::uint32_t *>(kAicaArmControlRegister);
#if SHENMUE_IS_S2
    WriteAicaWord((ReadAicaWord(armControl) & 0xfffeU) | 1U, armControl);
#endif
    WriteAicaWord(mode, reinterpret_cast<volatile std::uint32_t *>(kAicaRamModeRegister));
    WriteAicaWord((ReadAicaWord(armControl) & 0xfffeU) | 1U, armControl);
    volatile std::uint32_t *const ram = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin);
#if SHENMUE_IS_S2
    syG2Write(driver, const_cast<std::uint32_t *>(ram), 4, static_cast<std::int32_t>(driver[0x54 / 4] >> 2), 1, 1);
#else
    for (std::uint32_t index = 0; index < (driver[0x54 / 4] >> 2); ++index)
        ram[index] = driver[index];
#endif
    WriteAicaWord(mode, ram + 0x50 / 4);
    for (std::size_t index = 0; index < kAicaBankCount; ++index) {
#if SHENMUE_IS_S2 || SHENMUE_IS_S1
        WriteAicaWord(UINT32_MAX, ram + 0x60 / 4 + index);
#else
        do {
            ram[0x60 / 4 + index] = 0xffffffffU;
        } while (ram[0x60 / 4 + index] != 0xffffffffU);
#endif
    }
    WriteAicaWord(ReadAicaWord(armControl) & 0xfffeU, armControl);
    for (std::size_t index = 0; index < kAicaBankCount; ++index) {
        AicaBankRegion &region = g_aicaBankRegions[index];
        region.bankId = -1;
        region.address = 0;
        region.size = 0;
    }
    for (std::size_t index = 0; index < kAicaBankCount; ++index) {
        AicaStreamSlot &slot = g_aicaStreamSlots[index];
        slot.state = -1;
        slot.flags = 0;
        slot.leftBuffer = UINT32_MAX;
        slot.rightBuffer = UINT32_MAX;
    }
    g_aicaBankDataBegin = kAicaRamBegin + driver[0x84 / 4];
    g_aicaMemoryTable = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x10 + driver[0x80 / 4]);
    g_aicaCommandWritePointer = reinterpret_cast<volatile std::uint32_t *>(kAicaCommandRingBegin);
    g_aicaBankBytesLoaded = 0;
#if SHENMUE_IS_S2
    g_aicaUploadWork = &bankUploadWork;
    g_aicaUploadWork->state = 0;
#else
    g_aicaBankUploadState = 0;
#endif
    g_aicaCommandBusy = 0;
#if SHENMUE_IS_S2 || SHENMUE_IS_S1
    g_aicaDriverWork = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + driver[0xc8 / 4]);
    for (std::uint32_t attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (ReadAicaWord(ram + 0x5c / 4)) {
            volatile std::uint32_t *const control = reinterpret_cast<volatile std::uint32_t *>(0xa070289c);
            WriteAicaWord(ReadAicaWord(control) & 0x7f7U, control);
#if SHENMUE_IS_S2
            return nlsndDmaInitialize() == 0 ? 0 : -0x80;
#else
            g_aicaCommandBusy = 0;
            g_aicaCommandWritePointer = reinterpret_cast<volatile std::uint32_t *>(kAicaCommandRingBegin);
            g_aicaBankBytesLoaded = 0;
            g_aicaBankUploadState = 0;
            return 0;
#endif
        }
    }
    return -0x100;
#else
    while (ram[0x5c / 4] == 0) {
    }
    return 0;
#endif
}

// @verified - d
#if SHENMUE_IS_S2
std::int32_t SOND_CheckBankUpload(const std::uint32_t *data, std::int32_t bank, std::uint32_t reservedBytes) {
#else
std::int32_t SOND_CheckBankUpload(const std::uint32_t *data, std::int32_t bank) {
#endif
    if (bank < 0 || bank >= static_cast<std::int32_t>(kAicaBankCount))
        return -1;
    const char *const magic = reinterpret_cast<const char *>(data);
    const char signature[] = "DTPK";
    for (std::int32_t index = 0; index < 4; ++index) {
        if (magic[index] != signature[index])
            return -2;
    }
    if (g_aicaBankRegions[bank].bankId != -1)
        return -3;
    std::uint32_t size = data[2];
#if SHENMUE_IS_S2
    const std::uint32_t available = ReadAicaWord(reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x88));
    if (reservedBytes) {
        if ((reservedBytes & 3U) || static_cast<std::int32_t>(reservedBytes) < static_cast<std::int32_t>(size))
            return -6;
        size = reservedBytes;
    }
#else
    const std::uint32_t available = *reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x88);
#endif
    if (static_cast<std::int32_t>(g_aicaBankBytesLoaded + size) > static_cast<std::int32_t>(available))
        return -4;
#if SHENMUE_IS_S2
    if (g_aicaUploadWork->state)
#else
    if (g_aicaBankUploadState)
#endif
        return -5;
    return 0;
}

// @verified - d
std::int32_t SOND_CheckBankRelease(std::int32_t bank) {
    if (bank < 0 || bank >= static_cast<std::int32_t>(kAicaBankCount))
        return -1;
    if (g_aicaBankRegions[bank].bankId == -1)
        return -2;
#if SHENMUE_IS_S2
    if (g_aicaUploadWork->state)
        return -3;
#endif
    return 0;
}

// @verified - d
std::uintptr_t SOND_ReserveBankRegion(std::int32_t bank, std::int32_t bankId, std::uint32_t size, std::uint32_t *movedBytes) {
    g_aicaBankBytesLoaded += size;
    g_aicaBankRegions[bank].bankId = bankId;
    g_aicaBankRegions[bank].size = size;
    WriteAicaWord(static_cast<std::uint32_t>(bankId), reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x60) + bank);
    std::uintptr_t address = 0;
    for (std::int32_t previous = bank - 1; previous >= 0; --previous) {
        if (g_aicaBankRegions[previous].bankId != -1) {
            address = g_aicaBankRegions[previous].address + g_aicaBankRegions[previous].size;
            break;
        }
    }
    if (address == 0)
        address = g_aicaBankDataBegin;
    g_aicaBankRegions[bank].address = address;
    std::uint32_t shifted = 0;
    for (std::int32_t next = bank + 1; next < static_cast<std::int32_t>(kAicaBankCount); ++next) {
        if (g_aicaBankRegions[next].bankId != -1) {
            shifted += g_aicaBankRegions[next].size;
            g_aicaBankRegions[next].address += size;
        }
    }
    *movedBytes = shifted;
    return address;
}

// @verified - d
std::uintptr_t SOND_ReleaseBankRegion(std::int32_t bank, std::uint32_t size, std::uint32_t *movedBytes) {
    g_aicaBankBytesLoaded -= size;
    g_aicaBankRegions[bank].bankId = -1;
    WriteAicaWord(UINT32_MAX, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x60) + bank);
    std::uint32_t shifted = 0;
    std::uintptr_t address = 0;
    for (std::int32_t next = bank + 1; next < static_cast<std::int32_t>(kAicaBankCount); ++next) {
        if (g_aicaBankRegions[next].bankId != -1) {
            if (address == 0)
                address = g_aicaBankRegions[next].address;
            shifted += g_aicaBankRegions[next].size;
            g_aicaBankRegions[next].address -= size;
        }
    }
    *movedBytes = shifted;
    return address;
}

// @verified - d
std::int32_t SOND_MoveAicaMemory(std::uintptr_t source, std::uintptr_t destination, std::uint32_t size) {
    WriteAicaWord(source - kAicaRamBegin, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x90));
    WriteAicaWord(destination - kAicaRamBegin, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x94));
    WriteAicaWord(size, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x98));
#if SHENMUE_IS_S2 || SHENMUE_IS_S1
    for (std::uint32_t attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (ReadAicaWord(reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x98)))
            break;
        if (attempt == kAicaPollLimit - 1)
            return -0x100;
    }
#endif
    std::uint32_t attempt = 0;
    for (; attempt < kAicaPollLimit; ++attempt) {
        if (SOND_ExecuteRawAicaCommand(0x2000a0) == 0)
            return 0;
    }
    return -0x100;
}

// @verified - d
std::int32_t SOND_IsMemoryMovePending() {
    return ReadAicaWord(reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x98)) != 0;
}


#if SHENMUE_IS_S2
// @verified - d
std::int32_t SOND_WaitBankUploadCommand() {
    for (std::uint32_t attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (SOND_ExecuteRawAicaCommand(0x18a0) == 0)
            return 0;
    }
    return -0x100;
}

// @verified - d
std::int32_t SOND_WaitBankUploadBusy() {
    for (std::uint32_t attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (ReadAicaWord(reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb4)))
            return 0;
    }
    return -0x100;
}

// @verified - d
std::int32_t SOND_WaitMemoryMove() {
    for (std::uint32_t attempt = 0; attempt < 0x1000000; ++attempt) {
        if (!SOND_IsMemoryMovePending())
            return 0;
    }
    return -0x100;
}

// @verified - d
std::int32_t SOND_FinishBankUpload() {
    WriteAicaWord(1, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0));
    for (std::uint32_t attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (!ReadAicaWord(reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb4))) {
            WriteAicaWord(0, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0));
            return 0;
        }
    }
    return -0x100;
}

// @verified - d
std::int32_t SOND_UploadBank(const std::uint32_t *data, std::int32_t bank) {
    std::int32_t result = SOND_CheckBankUpload(data, bank, 0);
    if (result)
        return result;
    const std::uint32_t size = data[2];
    std::uint32_t shifted;
    const std::uintptr_t destination = SOND_ReserveBankRegion(bank, static_cast<std::int32_t>(data[1]), size, &shifted);
    result = SOND_WaitBankUploadCommand();
    if (result)
        return result;
    result = SOND_WaitBankUploadBusy();
    if (result)
        return result;
    if (shifted) {
        result = SOND_MoveAicaMemory(destination, destination + size, shifted);
        if (result)
            return result;
        result = SOND_WaitMemoryMove();
        if (result)
            return result;
    }
    syG2Write(data, reinterpret_cast<void *>(destination), 4, static_cast<std::int32_t>(size >> 2), 1, 1);
    return SOND_FinishBankUpload();
}

// @verified - d
std::int32_t SOND_UploadBank(const std::uint32_t *data, std::int32_t bank, std::uint32_t reservedBytes) {
    if (!reservedBytes)
        return -6;
    std::int32_t result = SOND_CheckBankUpload(data, bank, reservedBytes);
    if (result)
        return result;
    const std::uint32_t size = data[2];
    std::uint32_t shifted;
    const std::uintptr_t destination = SOND_ReserveBankRegion(bank, static_cast<std::int32_t>(data[1]), reservedBytes, &shifted);
    result = SOND_WaitBankUploadCommand();
    if (result)
        return result;
    result = SOND_WaitBankUploadBusy();
    if (result)
        return result;
    if (shifted) {
        result = SOND_MoveAicaMemory(destination, destination + reservedBytes, shifted);
        if (result)
            return result;
        result = SOND_WaitMemoryMove();
        if (result)
            return result;
    }
    syG2Write(data, reinterpret_cast<void *>(destination), 4, static_cast<std::int32_t>(size >> 2), 1, 1);
    WriteAicaWord(reservedBytes, reinterpret_cast<volatile std::uint32_t *>(destination + 8));
    return SOND_FinishBankUpload();
}

// @verified - d
std::int32_t SOND_CheckBankReplacement(const std::uint32_t *data, std::int32_t bank) {
    if (bank < 0 || bank >= static_cast<std::int32_t>(kAicaBankCount))
        return -1;
    const char *const magic = reinterpret_cast<const char *>(data);
    const char signature[] = "DTPK";
    for (std::int32_t index = 0; index < 4; ++index) {
        if (magic[index] != signature[index])
            return -2;
    }
    if (g_aicaBankRegions[bank].bankId == -1)
        return -3;
    if (static_cast<std::int32_t>(g_aicaBankRegions[bank].size) < static_cast<std::int32_t>(data[2]))
        return -4;
    return g_aicaUploadWork->state ? -5 : 0;
}

// @verified - d
std::int32_t SOND_ReplaceBank(const std::uint32_t *data, std::int32_t bank) {
    std::int32_t result = SOND_CheckBankReplacement(data, bank);
    if (result)
        return result;
    for (std::uint32_t attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (nlsndExecuteAicaCommand(static_cast<std::uint16_t>(bank), 0x1100a0, 0))
            continue;
        const std::uint32_t bytes = data[2];
        std::uint32_t bankId = data[1];
        AicaBankRegion &region = g_aicaBankRegions[bank];
        if (static_cast<std::uint32_t>(region.bankId) == bankId)
            bankId |= 0x80000000U;
        region.bankId = static_cast<std::int32_t>(bankId);
        WriteAicaWord(bankId, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x60) + bank);
        result = SOND_WaitBankUploadCommand();
        if (result)
            return result;
        result = SOND_WaitBankUploadBusy();
        if (result)
            return result;
        syG2Write(data, reinterpret_cast<void *>(region.address), 4, static_cast<std::int32_t>(bytes >> 2), 1, 1);
        WriteAicaWord(region.size, reinterpret_cast<volatile std::uint32_t *>(region.address + 8));
        return SOND_FinishBankUpload();
    }
    return -0x100;
}

// @verified - d
std::int32_t SOND_BeginBankUpload(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame) {
    const std::int32_t result = SOND_CheckBankUpload(data, bank, 0);
    if (result)
        return result;
    const std::uint32_t bytes = data[2];
    std::uint32_t shifted;
    const std::uintptr_t destination = SOND_ReserveBankRegion(bank, static_cast<std::int32_t>(data[1]), bytes, &shifted);
    g_aicaUploadWork->beginAddress = destination;
    g_aicaUploadWork->destination = destination;
    g_aicaUploadWork->source = data;
    g_aicaUploadWork->state = 1;
    g_aicaUploadWork->wordsPerFrame = wordsPerFrame;
    g_aicaUploadWork->remainingBytes = bytes;
    g_aicaUploadWork->allocationBytes = bytes;
    g_aicaUploadWork->shiftedBytes = shifted;
    return 0;
}

// @verified - d
std::int32_t SOND_BeginBankUpload(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame, std::uint32_t reservedBytes) {
    if (!reservedBytes)
        return -6;
    const std::int32_t result = SOND_CheckBankUpload(data, bank, reservedBytes);
    if (result)
        return result;
    std::uint32_t shifted;
    const std::uintptr_t destination = SOND_ReserveBankRegion(bank, static_cast<std::int32_t>(data[1]), reservedBytes, &shifted);
    g_aicaUploadWork->beginAddress = destination;
    g_aicaUploadWork->destination = destination;
    g_aicaUploadWork->source = data;
    g_aicaUploadWork->state = 0x8001;
    g_aicaUploadWork->wordsPerFrame = wordsPerFrame;
    g_aicaUploadWork->allocationBytes = reservedBytes;
    g_aicaUploadWork->remainingBytes = data[2];
    g_aicaUploadWork->shiftedBytes = shifted;
    return 0;
}

// @verified - d
std::int32_t SOND_BeginBankReplacement(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame) {
    const std::int32_t result = SOND_CheckBankReplacement(data, bank);
    if (result)
        return result;
    for (std::uint32_t attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (nlsndExecuteAicaCommand(static_cast<std::uint16_t>(bank), 0x1100a0, 0))
            continue;
        std::uint32_t bankId = data[1];
        AicaBankRegion &region = g_aicaBankRegions[bank];
        if (static_cast<std::uint32_t>(region.bankId) == bankId)
            bankId |= 0x80000000U;
        region.bankId = static_cast<std::int32_t>(bankId);
        WriteAicaWord(bankId, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x60) + bank);
        g_aicaUploadWork->beginAddress = region.address;
        g_aicaUploadWork->destination = region.address;
        g_aicaUploadWork->source = data;
        g_aicaUploadWork->state = 0x8001;
        g_aicaUploadWork->wordsPerFrame = wordsPerFrame;
        g_aicaUploadWork->allocationBytes = region.size;
        g_aicaUploadWork->remainingBytes = data[2];
        g_aicaUploadWork->shiftedBytes = 0;
        return 0;
    }
    return -0x100;
}

// @verified - d
std::int32_t SOND_BeginStreamReplacement(const std::uint32_t *data, std::int32_t bank, std::uint32_t wordsPerFrame) {
    const std::int32_t result = SOND_BeginBankReplacement(data, bank, wordsPerFrame);
    if (result)
        return result;
    g_aicaUploadWork->state = 0x9001;
    return 0;
}

// @verified - d
std::int32_t SOND_WriteStreamChunk(const std::uint32_t *data) {
    if ((g_aicaUploadWork->state & 0x10ffU) != 0x1004U)
        return -1;
    const std::uint32_t words = static_cast<std::int32_t>(g_aicaUploadWork->remainingBytes) < static_cast<std::int32_t>(g_aicaUploadWork->wordsPerFrame << 2) ?
        g_aicaUploadWork->remainingBytes >> 2 :
        g_aicaUploadWork->wordsPerFrame;
    syG2Write(data, reinterpret_cast<void *>(g_aicaUploadWork->destination), 4, static_cast<std::int32_t>(words), 1, 1);
    g_aicaUploadWork->remainingBytes -= words * 4;
    if (!g_aicaUploadWork->remainingBytes) {
        if (g_aicaUploadWork->state & 0x8000U)
            WriteAicaWord(g_aicaUploadWork->allocationBytes, reinterpret_cast<volatile std::uint32_t *>(g_aicaUploadWork->beginAddress + 8));
        WriteAicaWord(1, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0));
        ++g_aicaUploadWork->state;
        return 0;
    }
    g_aicaUploadWork->destination += words * 4;
    return 1;
}

// @verified - d
std::int32_t SOND_SetUploadWords(std::uint32_t wordsPerFrame) {
    if (!wordsPerFrame)
        return -1;
    g_aicaUploadWork->wordsPerFrame = wordsPerFrame;
    return 0;
}

// @verified - d
std::int32_t SOND_PollBankUpload() {
    switch (g_aicaUploadWork->state & 0xffU) {
    case 0:
        return -1;
    case 1:
        if (!SOND_WaitBankUploadCommand())
            ++g_aicaUploadWork->state;
        return 1;
    case 2:
        if (ReadAicaWord(reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb4))) {
            if (g_aicaUploadWork->shiftedBytes)
                SOND_MoveAicaMemory(
                    g_aicaUploadWork->destination, g_aicaUploadWork->destination + g_aicaUploadWork->allocationBytes, g_aicaUploadWork->shiftedBytes);
            ++g_aicaUploadWork->state;
        }
        return 1;
    case 3:
        if (SOND_IsMemoryMovePending())
            return 1;
        ++g_aicaUploadWork->state;
    case 4:
        if (!(g_aicaUploadWork->state & 0x7f00U)) {
            const std::uint32_t words =
                static_cast<std::int32_t>(g_aicaUploadWork->remainingBytes) < static_cast<std::int32_t>(g_aicaUploadWork->wordsPerFrame << 2) ?
                g_aicaUploadWork->remainingBytes >> 2 :
                g_aicaUploadWork->wordsPerFrame;
            syG2Write(g_aicaUploadWork->source, reinterpret_cast<void *>(g_aicaUploadWork->destination), 4, static_cast<std::int32_t>(words), 1, 1);
            g_aicaUploadWork->remainingBytes -= words * 4;
            if (!g_aicaUploadWork->remainingBytes) {
                if (g_aicaUploadWork->state & 0x8000U)
                    WriteAicaWord(g_aicaUploadWork->allocationBytes, reinterpret_cast<volatile std::uint32_t *>(g_aicaUploadWork->beginAddress + 8));
                WriteAicaWord(1, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0));
                ++g_aicaUploadWork->state;
                return 1;
            }
            g_aicaUploadWork->destination += words * 4;
            g_aicaUploadWork->source += words;
        } else {
            if ((g_aicaUploadWork->state & 0x7f00U) != 0x100U)
                return 0;
            WriteAicaWord(1, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0));
            ++g_aicaUploadWork->state;
        }
        return 1;
    case 5:
        if (!ReadAicaWord(reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb4))) {
            WriteAicaWord(0, reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0));
            g_aicaUploadWork->state = 0;
            return 0;
        }
        return 1;
    default:
        return -1;
    }
}

// @verified - d
std::int32_t SOND_ReleaseBank(std::int32_t bank) {
    std::int32_t result = SOND_CheckBankRelease(bank);
    if (result)
        return result;
    const std::uint32_t bytes = g_aicaBankRegions[bank].size;
    std::uint32_t shifted;
    const std::uintptr_t source = SOND_ReleaseBankRegion(bank, bytes, &shifted);
    result = SOND_WaitBankUploadCommand();
    if (result)
        return result;
    result = SOND_WaitBankUploadBusy();
    if (result)
        return result;
    if (source) {
        result = SOND_MoveAicaMemory(source, source - bytes, shifted);
        if (result)
            return result;
        result = SOND_WaitMemoryMove();
        if (result)
            return result;
    }
    return SOND_FinishBankUpload();
}

// @verified - d
std::int32_t SOND_FinishAicaDriver() {
    nlsndExecuteAicaCommand(0, 0x100a0, 0);
    nlsndExecuteAicaCommand(0, 0x400a0, 0);
    return nlsndDmaFinish() == 0 ? 0 : -0x80;
}
#else
// @verified - d
std::int32_t SOND_UploadBank(const std::uint32_t *data, std::int32_t bank) {
    const std::int32_t admitted = SOND_CheckBankUpload(data, bank);
    if (admitted)
        return admitted;
    const std::uint32_t size = data[2];
    std::uint32_t shifted;
    const std::uintptr_t destination = SOND_ReserveBankRegion(bank, static_cast<std::int32_t>(data[1]), size, &shifted);
    std::uint32_t attempt = 0;
    for (; attempt < kAicaPollLimit; ++attempt) {
        if (SOND_ExecuteRawAicaCommand(0x18a0) == 0)
            break;
    }
    if (attempt == kAicaPollLimit)
        return -0x100;
    volatile std::uint32_t *const busy = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb4);
    for (attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (*busy)
            break;
    }
    if (attempt == kAicaPollLimit)
        return -0x100;
    if (shifted) {
        const std::int32_t result = SOND_MoveAicaMemory(destination, destination + size, shifted);
        if (result)
            return result;
        while (SOND_IsMemoryMovePending()) {
        }
    }
    volatile std::uint32_t *const ram = reinterpret_cast<volatile std::uint32_t *>(destination);
    for (std::uint32_t index = 0; index < (size >> 2); ++index)
        ram[index] = data[index];
    *reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0) = 1;
    for (attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (*busy == 0)
            break;
    }
    if (attempt == kAicaPollLimit)
        return -0x100;
    *reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0) = 0;
    return 0;
}

// @verified - d
std::int32_t SOND_ReleaseBank(std::int32_t bank) {
    const std::int32_t admitted = SOND_CheckBankRelease(bank);
    if (admitted)
        return admitted;
    const std::uint32_t size = g_aicaBankRegions[bank].size;
    std::uint32_t shifted;
    const std::uintptr_t source = SOND_ReleaseBankRegion(bank, size, &shifted);
    std::uint32_t attempt = 0;
    for (; attempt < kAicaPollLimit; ++attempt) {
        if (SOND_ExecuteRawAicaCommand(0x18a0) == 0)
            break;
    }
    if (attempt == kAicaPollLimit)
        return -0x100;
    volatile std::uint32_t *const busy = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb4);
    for (attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (*busy)
            break;
    }
    if (attempt == kAicaPollLimit)
        return -0x100;
    if (source) {
        const std::int32_t result = SOND_MoveAicaMemory(source, source - size, shifted);
        if (result)
            return result;
        while (SOND_IsMemoryMovePending()) {
        }
    }
    *reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0) = 1;
    for (attempt = 0; attempt < kAicaPollLimit; ++attempt) {
        if (*busy == 0)
            break;
    }
    if (attempt == kAicaPollLimit)
        return -0x100;
    *reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0xb0) = 0;
    return 0;
}

#endif

// @verified - d
std::int32_t SOND_ConfigureStreamSlots(std::int16_t mono, std::int16_t stereo, std::int16_t mode) {
    if (mode)
        return -2;
    const std::int16_t channels = static_cast<std::int16_t>(mono + 2 * stereo);
    if (channels > 8)
        return -1;
    for (std::size_t index = 0; index < kAicaBankCount; ++index) {
        const AicaStreamSlot &slot = g_aicaStreamSlots[index];
        if (slot.state == 1)
            return -3;
    }
    volatile std::uint32_t *const ram = reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin);
    WriteAicaWord(static_cast<std::uint32_t>(mono), ram + 0xa0 / 4);
    WriteAicaWord(static_cast<std::uint32_t>(stereo), ram + 0xa4 / 4);
    const std::uint32_t available = ReadAicaWord(ram + 0x8c / 4);
    std::uint32_t span;
    std::uint32_t position = 0;
    if (channels) {
        const std::int32_t numerator = static_cast<std::int32_t>(available - static_cast<std::uint32_t>(channels) * 64U);
        g_aicaStreamBufferBytes = static_cast<std::uint32_t>(static_cast<std::int64_t>(numerator) / channels) & 0x7fffffe0U;
        if (g_aicaStreamBufferBytes > 0x4000U)
            g_aicaStreamBufferBytes = 0x4000;
        span = g_aicaStreamBufferBytes + 0x40;
        std::uint32_t reserved = ReadAicaWord(g_aicaMemoryTable);
        if (reserved & 31U)
            reserved = (reserved & ~31U) + 32;
        position = g_aicaRamSize - span - reserved;
    } else {
        span = 0;
        g_aicaStreamBufferBytes = 0;
    }
    WriteAicaWord(span, ram + 0xa8 / 4);
    volatile std::uint32_t *hardware = ram + 0x100 / 4;
    std::int16_t index = 0;
    for (; index < mono; ++index) {
        AicaStreamSlot &slot = g_aicaStreamSlots[index];
        slot.flags = 0;
        slot.state = 0;
#if SHENMUE_IS_S2
        slot.hardware = hardware;
#endif
        slot.leftBuffer = kAicaRamBegin + position;
        WriteAicaWord(position, hardware + 0xc / 4);
        position -= span;
        slot.rightBuffer = 0;
        WriteAicaWord(0, hardware + 0x10 / 4);
        slot.bufferState = hardware + 0x14 / 4;
        slot.playState = hardware + 4 / 4;
        hardware += 0x60 / 4;
    }
    for (; index < mono + stereo; ++index) {
        AicaStreamSlot &slot = g_aicaStreamSlots[index];
        slot.flags = 0;
        slot.state = 0;
#if SHENMUE_IS_S2
        slot.hardware = hardware;
#endif
        slot.rightBuffer = kAicaRamBegin + position;
        WriteAicaWord(position, hardware + 0x10 / 4);
        position -= span;
        slot.leftBuffer = kAicaRamBegin + position;
        WriteAicaWord(position, hardware + 0xc / 4);
        position -= span;
        slot.bufferState = hardware + 0x14 / 4;
        slot.playState = hardware + 4 / 4;
        hardware += 0x60 / 4;
    }
    for (; index < static_cast<std::int16_t>(kAicaBankCount); ++index) {
        AicaStreamSlot &slot = g_aicaStreamSlots[index];
        slot.state = -1;
        slot.flags = 0;
        slot.rightBuffer = UINT32_MAX;
        slot.leftBuffer = UINT32_MAX;
    }
    SOND_ExecuteRawAicaCommand(0xa1);
    return 0;
}
}
} // namespace shenmue
#endif
