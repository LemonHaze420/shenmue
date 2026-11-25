#include "platform/dc/aica_stream.hpp"
#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || SHENMUE_IS_S2)
#include "platform/dc/runtime_arithmetic.hpp"
#include <cstring>
#if SHENMUE_IS_S2
namespace shenmue { namespace s2 {
#else
namespace shenmue { namespace s1 {
#endif
namespace {
AicaStreamSlot &StreamSlot(std::int32_t stream) {
#if SHENMUE_IS_S2
    return g_aicaStreamSlots[stream];
#else
    const std::uint8_t offset = static_cast<std::uint8_t>(stream * sizeof(AicaStreamSlot));
    return *reinterpret_cast<AicaStreamSlot *>(reinterpret_cast<std::uint8_t *>(g_aicaStreamSlots) + offset);
#endif
}
} // namespace

// @verified - d
void nlsndStreamConfigure(std::int16_t stream, const AicaStreamConfiguration *configuration) {
    AicaStreamSlot &slot = StreamSlot(stream);
    if (slot.state != 1 || slot.flags)
        return;
    volatile std::uint32_t *const destination =
        reinterpret_cast<volatile std::uint32_t *>(kAicaRamBegin + 0x120U + static_cast<std::uint32_t>(static_cast<std::int32_t>(stream) * 0x60));
#if SHENMUE_IS_S2
    syG2Write(configuration, const_cast<std::uint32_t *>(destination), 4, 16, 1, 1);
#else
    for (std::uint32_t word = 0; word < 16; ++word) {
        std::uint32_t value;
        std::memcpy(&value, reinterpret_cast<const std::uint8_t *>(configuration) + word * 4, sizeof(value));
        destination[word] = value;
    }
#endif
    slot.sampleRate = configuration->sampleRate;
    switch (configuration->format) {
    case 0:
        slot.bitsPerSample = 16;
        break;
    case 1:
        slot.bitsPerSample = 8;
        break;
    case 2:
    case 3:
        slot.bitsPerSample = 4;
        break;
    }
}

// @verified - d
void *nlsndStreamGetBuffer(std::int16_t stream, std::int16_t channel, std::uint32_t *bytes) {
#if SHENMUE_IS_S2
    const AicaStreamSlot &slot = StreamSlot(stream);
#else
    const AicaStreamSlot &slot = StreamSlot(static_cast<std::int8_t>(stream));
#endif
    if (slot.state != 1)
        return 0;
    *bytes = g_aicaStreamBufferBytes;
    std::uintptr_t buffer;
    std::memcpy(&buffer, reinterpret_cast<const std::uint8_t *>(&slot.leftBuffer) + channel * 4, sizeof(buffer));
    return reinterpret_cast<void *>(buffer);
}

// @verified - d
#if SHENMUE_IS_S2
std::uint32_t nlsndStreamGetSampleRate(std::int16_t stream) {
#else
std::uint32_t nlsndStreamGetSampleRate(std::int8_t stream) {
#endif
    const AicaStreamSlot &slot = StreamSlot(stream);
    return slot.state == 1 ? slot.sampleRate : 0;
}

// @verified - d
#if SHENMUE_IS_S2
std::int32_t nlsndStreamGetBitsPerSample(std::int16_t stream) {
#else
std::int32_t nlsndStreamGetBitsPerSample(std::int8_t stream) {
#endif
    const AicaStreamSlot &slot = StreamSlot(stream);
    return slot.state == 1 ? slot.bitsPerSample : 0;
}


// @verified - d
std::uint32_t nlsndStreamGetPlayPosition(std::int16_t stream) {
    const AicaStreamSlot &slot = StreamSlot(stream);
    if (slot.state != 1 || !slot.flags)
        return 0;
    return ReadAicaWord(slot.bufferState);
}

// @verified - d
std::int32_t nlsndStreamOpen(std::int16_t channels) {
    std::int16_t index;
    if (channels == 1) {
        for (index = 0; index < 8; ++index) {
            if (g_aicaStreamSlots[index].state == 0 && g_aicaStreamSlots[index].rightBuffer == 0)
                break;
        }
    } else if (channels == 2) {
        for (index = 0; index < 8; ++index) {
            if (g_aicaStreamSlots[index].state == 0 && g_aicaStreamSlots[index].rightBuffer != 0)
                break;
        }
    } else {
        return -1;
    }
    if (index >= 8)
        return -1;
    g_aicaStreamSlots[index].state = 1;
    return index;
}

// @verified - d
#if SHENMUE_IS_S2
void nlsndStreamClose(std::int16_t stream) {
#else
void nlsndStreamClose(std::int8_t stream) {
#endif
    AicaStreamSlot &slot = StreamSlot(stream);
    if (slot.state == 1 && !slot.flags)
        slot.state = 0;
}

// @verified - d
void nlsndStreamPlay(std::int16_t stream) {
    AicaStreamSlot &slot = StreamSlot(stream);
    if (slot.state != 1 || slot.flags)
        return;
    SOND_ExecuteRawAicaCommand((static_cast<std::uint32_t>(stream) << 12U) + 0x1a1U);
    slot.flags = 1;
}

// @verified - d
void nlsndStreamStop(std::int16_t stream) {
    AicaStreamSlot &slot = StreamSlot(stream);
    if (slot.state != 1 || !slot.flags)
        return;
#if SHENMUE_IS_S2 || SHENMUE_IS_S1
    for (std::uint32_t attempt = 0; attempt < 0x40000; ++attempt) {
        if (!ReadAicaWord(slot.playState))
            continue;
        volatile std::uint32_t *const primary = reinterpret_cast<volatile std::uint32_t *>(ReadAicaWord(slot.playState) | 0xa0000000U);
        const std::uint32_t value = ReadAicaWord(primary) & 0x7ffU;
        WriteAicaWord(value, primary);
        const volatile std::uint32_t *const configuration =
            reinterpret_cast<const volatile std::uint32_t *>(kAicaRamBegin + 0x128U + static_cast<std::uint32_t>(static_cast<std::int32_t>(stream) * 0x60));
        if ((ReadAicaWord(configuration) & 0x300U) != 0x100U) {
            WriteAicaWord(value | 0x8000U, primary);
        } else {
            for (std::uint32_t secondaryAttempt = 0; secondaryAttempt < 0x40000; ++secondaryAttempt) {
                if (!ReadAicaWord(slot.playState + 1))
                    continue;
                volatile std::uint32_t *const secondary = reinterpret_cast<volatile std::uint32_t *>(ReadAicaWord(slot.playState + 1) | 0xa0000000U);
                WriteAicaWord(ReadAicaWord(secondary) & 0x7ffU, secondary);
                WriteAicaWord(value | 0x8000U, primary);
                break;
            }
        }
        break;
    }
#else
    while (*slot.playState == 0) {
    }
    volatile std::uint32_t *const primary = reinterpret_cast<volatile std::uint32_t *>(*slot.playState | 0xa0000000U);
    const std::uint32_t value = *primary & 0x7ffU;
    *primary = value;
    const volatile std::uint32_t *const configuration =
        reinterpret_cast<const volatile std::uint32_t *>(kAicaRamBegin + 0x128U + static_cast<std::uint32_t>(static_cast<std::int32_t>(stream) * 0x60));
    if ((*configuration & 0x300U) == 0x100U) {
        while (slot.playState[1] == 0) {
        }
        volatile std::uint32_t *const secondary = reinterpret_cast<volatile std::uint32_t *>(slot.playState[1] | 0xa0000000U);
        *secondary = *secondary & 0x7ffU;
    }
    *primary = value | 0x8000U;
#endif
    SOND_ExecuteRawAicaCommand((static_cast<std::uint32_t>(stream) << 12U) + 0x2a1U);
    slot.flags = 0;
}


// @verified - d
void nlsndStreamConfigurationInit(AicaStreamConfiguration *configuration) {
    const std::uint8_t *const source = reinterpret_cast<const std::uint8_t *>(&kAicaStreamDefaultConfiguration);
    std::uint8_t *const destination = reinterpret_cast<std::uint8_t *>(configuration);
    for (std::uint32_t byte = 0; byte < sizeof(AicaStreamConfiguration); ++byte)
        destination[byte] = source[byte];
}

// @verified - d
void nlsndStreamConfigurationSetPitch(AicaStreamConfiguration *configuration, std::int16_t octave, std::uint16_t pitch) {
    configuration->pitch = static_cast<std::uint16_t>(pitch | (static_cast<std::uint32_t>(octave) << 11U));
}

// @verified - d
void nlsndStreamConfigurationSetChannels(AicaStreamConfiguration *configuration, std::int16_t channels) {
    if (channels == 1)
        configuration->channelCode &= 0x80U;
    else if (channels == 2)
        configuration->channelCode = (configuration->channelCode & 0x80U) | 1U;
    else if (channels == 4)
        configuration->channelCode = (configuration->channelCode & 0x80U) | 2U;
}

// @verified - d
void nlsndStreamConfigurationSetVolume(AicaStreamConfiguration *configuration, std::int16_t channel, std::uint8_t volume) {
    configuration->channels[channel].volume = volume;
}

// @verified - d
void nlsndStreamConfigurationSetPan(AicaStreamConfiguration *configuration, std::int16_t channel, std::uint8_t pan) {
    configuration->channels[channel].pan = pan;
}

// @verified - d
void nlsndStreamConfigurationSetBitsPerSample(AicaStreamConfiguration *configuration, std::int16_t bits) {
    if (bits == 4)
        configuration->format = 3;
    else if (bits == 8)
        configuration->format = 1;
    else if (bits == 16)
        configuration->format = 0;
}

// @verified - d
#if SHENMUE_IS_S2
void nlsndStreamConfigurationSetEffects(AicaStreamConfiguration *configuration, std::uint8_t send, std::int16_t level) {
#else
void nlsndStreamConfigurationSetEffects(AicaStreamConfiguration *configuration, std::int8_t level, std::uint8_t send) {
#endif
    for (std::uint32_t index = 0; index < 4; ++index)
        configuration->channels[index].effects = static_cast<std::uint8_t>((static_cast<std::uint32_t>(level) << 4U) | send);
}

// @verified - d
void nlsndStreamConfigurationSetDirectLevel(AicaStreamConfiguration *configuration, std::uint8_t level) {
    for (std::uint32_t index = 0; index < 4; ++index)
        configuration->channels[index].directLevel = level;
}

// @verified - d
std::int32_t nlsndStreamConfigurationGet(const AicaStreamConfiguration *configuration, AicaStreamConfigurationProperty property, std::int16_t channel) {
    std::int32_t channels;
    switch (configuration->channelCode & 3U) {
    case 0:
        channels = 1;
        break;
    case 1:
        channels = 2;
        break;
    case 2:
        channels = 4;
        break;
    }
    std::int32_t bits;
    switch (configuration->format) {
    case 0:
        bits = 16;
        break;
    case 1:
        bits = 8;
        break;
    case 2:
    case 3:
        bits = 4;
        break;
    }
    switch (property) {
    case AicaStreamConfigurationProperty_SampleRate:
        return configuration->sampleRate;
    case AicaStreamConfigurationProperty_BitsPerSample:
        return bits;
    case AicaStreamConfigurationProperty_SampleCount:
        return dc::SignedDivide(dc::SignedDivide(static_cast<std::int32_t>(configuration->dataBytes << 3U), bits), channels);
    case AicaStreamConfigurationProperty_DataBytes:
        return static_cast<std::int32_t>(configuration->dataBytes);
    case AicaStreamConfigurationProperty_Channels:
        return channels;
    case AicaStreamConfigurationProperty_Volume:
        return configuration->channels[channel].volume;
    case AicaStreamConfigurationProperty_Pan:
        return configuration->channels[channel].pan;
    case AicaStreamConfigurationProperty_EffectSend:
        return configuration->channels[0].effects & 15U;
    case AicaStreamConfigurationProperty_EffectLevel:
        return configuration->channels[0].effects >> 4U;
    case AicaStreamConfigurationProperty_Unknown9:
        if (configuration->parameter1f)
            return 0;
        return (configuration->envelope[3] >> 6U) ? configuration->envelope[3] >> 6U : 1;
    case AicaStreamConfigurationProperty_Unknown10:
        return configuration->parameter1e;
    default:
        return -1;
    }
}

// @verified - d
void nlsndStreamSetVolume(std::int16_t stream, std::int16_t channel, std::int16_t volume) {
    if (StreamSlot(stream).state == 1)
        SOND_ExecuteRawAicaCommand(((static_cast<std::uint32_t>(stream) << 12U) | (static_cast<std::uint32_t>(volume) << 16U) | 0xa2U) + channel * 0x800U);
}

// @verified - d
void nlsndStreamSetPan(std::int16_t stream, std::int16_t channel, std::int16_t pan) {
    if (StreamSlot(stream).state == 1)
        SOND_ExecuteRawAicaCommand(((static_cast<std::uint32_t>(stream) << 12U) | (static_cast<std::uint32_t>(pan) << 16U) | 0x1a2U) + channel * 0x800U);
}

// @verified - d
void nlsndStreamSetParameter0C(std::int16_t stream, std::int16_t value) {
    if (StreamSlot(stream).state == 1)
        SOND_ExecuteRawAicaCommand((static_cast<std::uint32_t>(value + 64) << 16U) | (static_cast<std::uint32_t>(stream) << 12U) | 0xca1U);
}


// @verified - d
void nlsndStreamSetParameter08(std::int16_t stream, std::int16_t value) {
    if (StreamSlot(stream).state == 1)
        SOND_ExecuteRawAicaCommand((static_cast<std::uint32_t>(value + 64) << 16U) | (static_cast<std::uint32_t>(stream) << 12U) | 0x8a1U);
}

// @verified - d
void nlsndStreamSetParameter09(std::int16_t stream, std::int16_t value) {
    if (StreamSlot(stream).state == 1)
        SOND_ExecuteRawAicaCommand((static_cast<std::uint32_t>(value + 64) << 16U) | (static_cast<std::uint32_t>(stream) << 12U) | 0x9a1U);
}

// @verified - d
void nlsndStreamSetEffectsControl(std::int16_t stream, std::int16_t octave, std::int16_t pitch) {
    const AicaStreamSlot &slot = StreamSlot(stream);
    if (slot.state != 1)
        return;
    const std::uint32_t pitchCommand = (static_cast<std::uint32_t>(stream) << 12U) | (static_cast<std::uint32_t>(pitch) << 16U) | 0x2a2U;
    const std::uint32_t octaveCommand = (static_cast<std::uint32_t>(stream) << 12U) | (static_cast<std::uint32_t>(octave) << 16U) | 0x3a2U;
    SOND_ExecuteRawAicaCommand(pitchCommand);
    SOND_ExecuteRawAicaCommand(octaveCommand);
    if (slot.rightBuffer) {
        SOND_ExecuteRawAicaCommand(pitchCommand + 0x800U);
        SOND_ExecuteRawAicaCommand(octaveCommand + 0x800U);
    }
}

// @verified - d
void nlsndStreamSetDirectControl(std::int16_t stream, std::int16_t first, std::int16_t second) {
    if (StreamSlot(stream).state != 1)
        return;
    SOND_ExecuteRawAicaCommand((static_cast<std::uint32_t>(stream) << 12U) | (static_cast<std::uint32_t>(first) << 16U) | 0xaa1U);
    SOND_ExecuteRawAicaCommand((static_cast<std::uint32_t>(stream) << 12U) | (static_cast<std::uint32_t>(second) << 16U) | 0xba1U);
}

} } // namespace shenmue
#endif
