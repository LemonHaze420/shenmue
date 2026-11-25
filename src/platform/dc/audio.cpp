#include "platform/dc/audio.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || SHENMUE_IS_S2)

#include <cstring>

extern "C" void mwRnaSetupSnd(const shenmue::dc::sdk::AudioLibraryInterface *interface, void *object);
extern "C" void ADXT_ConfigVsyncSvr(std::int32_t enabled);

namespace shenmue { namespace dc { namespace sdk {
#if SHENMUE_IS_S2
namespace audio = shenmue::s2;
#else
namespace audio = shenmue::s1;
#endif
namespace {

AudioPort g_audioPorts[audio::kAicaBankCount] = {};

void SetVolume(AudioPort *port, std::int32_t volume);
void SetPan(AudioPort *port, std::int32_t channel, std::int32_t pan);
void SetExtraParameter(AudioPort *port, std::int32_t parameter, std::int32_t first, std::int32_t second);

void Initialize(void *) {
    std::memset(g_audioPorts, 0, sizeof(g_audioPorts));
}

void Finish() {
}

void Close(AudioPort *port) {
    if (port->playing)
        audio::nlsndStreamStop(port->stream);
    for (std::int32_t channel = 0; channel < port->allocatedChannels; ++channel)
        audio::nlsndDmaClose(port->transfers[channel]);
    for (std::int32_t channel = 0; channel < port->allocatedChannels; ++channel)
        audio::nlsndDmaClose(port->guardTransfers[channel]);
#if SHENMUE_IS_S2
    audio::nlsndStreamClose(port->stream);
#else
    audio::nlsndStreamClose(static_cast<std::int8_t>(port->stream));
#endif
    std::memset(port, 0, sizeof(*port));
}

void Play(AudioPort *port) {
    audio::nlsndStreamConfigurationInit(&port->configuration);
    std::uint32_t quotient = 44100U / (static_cast<std::uint32_t>(port->sampleRate) + 1U);
    std::int16_t octave = 0;
    while (quotient) {
        --octave;
        quotient >>= 1;
    }
    const std::int32_t baseRate = 44100 >> -octave;
    const std::uint16_t pitch = static_cast<std::uint16_t>((port->sampleRate - baseRate) * 1024 / baseRate) & 0x3ffU;
    audio::nlsndStreamConfigurationSetPitch(&port->configuration, octave & 15, pitch);
    audio::nlsndStreamConfigurationSetBitsPerSample(&port->configuration, static_cast<std::int16_t>(port->bitsPerSample));
    audio::nlsndStreamConfigurationSetChannels(&port->configuration, static_cast<std::int16_t>(port->channels));
    SetVolume(port, port->volume);
    SetPan(port, 0, port->pan[0]);
    SetPan(port, 1, port->pan[1]);
    for (std::int32_t parameter = 0; parameter < 3; ++parameter)
        SetExtraParameter(port, parameter, port->extraParameters[parameter][0], port->extraParameters[parameter][1]);
    volatile std::int32_t delay = 10000;
    while (delay)
        delay = delay - 1;
    audio::nlsndStreamConfigure(port->stream, &port->configuration);
    audio::nlsndStreamPlay(port->stream);
    port->playing = true;
}

void Stop(AudioPort *port) {
    audio::nlsndStreamStop(port->stream);
    port->playing = false;
}

std::int32_t GetBufferSamples(AudioPort *port) {
    std::uint32_t bytes;
    audio::nlsndStreamGetBuffer(port->stream, 0, &bytes);
    return static_cast<std::int32_t>(bytes) / port->bitsPerSample * 8;
}

void SetChannels(AudioPort *port, std::int32_t channels) {
    port->channels = channels;
    audio::nlsndStreamConfigurationSetChannels(&port->configuration, static_cast<std::int16_t>(channels));
}

std::int32_t GetPlayPosition(AudioPort *port, std::int32_t) {
    return static_cast<std::int32_t>(audio::nlsndStreamGetPlayPosition(port->stream));
}

void SetSampleRate(AudioPort *port, std::int32_t rate) {
    port->sampleRate = rate;
}

std::int32_t GetSampleRate(AudioPort *port) {
    return port->sampleRate;
}

void SetBitsPerSample(AudioPort *port, std::int32_t bits) {
    port->bitsPerSample = bits;
}

std::int32_t GetBitsPerSample(AudioPort *port) {
    return port->bitsPerSample;
}

void SetVolume(AudioPort *port, std::int32_t volume) {
    port->volume = volume;
    std::int32_t level;
    if (volume <= -959)
        level = 0;
    else if (volume >= 0)
        level = 127;
    else {
        level = 127 - ((volume * 255 - 128) / -959) * 2;
        if (level > 127)
            level = 127;
        else if (level < 0)
            level = 0;
    }
    for (std::int16_t channel = 0; channel < 2; ++channel) {
        if (port->playing)
            audio::nlsndStreamSetVolume(port->stream, channel, static_cast<std::int16_t>(level));
        else
            audio::nlsndStreamConfigurationSetVolume(&port->configuration, channel, static_cast<std::uint8_t>(level));
    }
}

std::int32_t GetVolume(AudioPort *port) {
    return port->volume;
}

void SetPan(AudioPort *port, std::int32_t channel, std::int32_t pan) {
    port->pan[channel] = pan;
    const std::int32_t encoded = pan < 0 ? -pan | 0x10 : pan;
    if (port->playing)
        audio::nlsndStreamSetPan(port->stream, static_cast<std::int16_t>(channel), static_cast<std::int16_t>(encoded));
    else
        audio::nlsndStreamConfigurationSetPan(&port->configuration, static_cast<std::int16_t>(channel), static_cast<std::uint8_t>(encoded));
}

std::int32_t GetPan(AudioPort *port, std::int32_t channel) {
    return port->pan[channel];
}

void SetExtraParameter(AudioPort *port, std::int32_t parameter, std::int32_t first, std::int32_t second) {
    port->extraParameters[parameter][0] = first;
    port->extraParameters[parameter][1] = second;
    if (parameter == 0) {
        const std::int16_t level = second < -42 ? 0 : second < 0 ? static_cast<std::int16_t>(second / 3 + 15) : 15;
        if (port->playing)
            audio::nlsndStreamSetEffectsControl(port->stream, static_cast<std::int16_t>(first), level);
        else {
#if SHENMUE_IS_S2
            audio::nlsndStreamConfigurationSetEffects(&port->configuration, static_cast<std::uint8_t>(first), level);
#else
            audio::nlsndStreamConfigurationSetEffects(&port->configuration, static_cast<std::int8_t>(first), static_cast<std::uint8_t>(level));
#endif
        }
    } else if (parameter == 1) {
        const std::int16_t coarse = first <= -100 ? -64 : first >= 100 ? 63 : static_cast<std::int16_t>(first * 64 / 100);
        const std::int16_t fine = second <= -100 ? -64 : second >= 100 ? 63 : static_cast<std::int16_t>(second * 64 / 100);
        audio::nlsndStreamSetParameter08(port->stream, coarse);
        audio::nlsndStreamSetParameter09(port->stream, fine);
    } else if (parameter == 2) {
        const std::int16_t direct = first < 1 ? 0 : first > 126 ? 127 : static_cast<std::int16_t>(first);
        const std::int16_t pan = second < 1 ? 0 : second > 30 ? 31 : static_cast<std::int16_t>(second);
        audio::nlsndStreamSetDirectControl(port->stream, direct, pan);
    }
}

void GetExtraParameter(AudioPort *port, std::int32_t parameter, std::int32_t *first, std::int32_t *second) {
    if (parameter >= 0 && parameter < 3) {
        *first = port->extraParameters[parameter][0];
        *second = port->extraParameters[parameter][1];
    } else {
        *first = 0;
        *second = 0;
    }
}

void Transfer(AudioPort *port, std::int32_t channel, std::int32_t position, const void *source, std::int32_t samples) {
    std::uint32_t bufferBytes;
    std::uint8_t *const buffer = static_cast<std::uint8_t *>(audio::nlsndStreamGetBuffer(port->stream, static_cast<std::int16_t>(channel), &bufferBytes));
    const std::int32_t offset = position * port->bitsPerSample / 8;
    std::int32_t bytes = samples * port->bitsPerSample / 8;
    if (bytes == 4)
        bytes = 32;
    audio::nlsndDmaTransfer(port->transfers[channel], source, buffer + offset, static_cast<std::uint32_t>(bytes));
    if (position == 0)
        audio::nlsndDmaTransfer(port->guardTransfers[channel], source, buffer + bufferBytes, 32);
}

std::int32_t GetTransferStatus(AudioPort *port, std::int32_t channel) {
    const std::int32_t status = audio::nlsndDmaGetStatus(port->transfers[channel]);
    return status == 0x100 ? 1 : status == 0x101 ? 2 : 0;
}

std::int32_t AdjustBufferOnPause(AudioPort *, std::int32_t, std::int32_t *shift) {
    *shift = 0;
    return 0;
}

std::int32_t AdjustBufferOnDiscard(AudioPort *, std::int32_t) {
    return 0;
}

const AudioPortInterface kAudioPortInterface = {{},
    Close,
    Play,
    Stop,
    GetBufferSamples,
    SetChannels,
    GetPlayPosition,
    SetSampleRate,
    GetSampleRate,
    SetBitsPerSample,
    GetBitsPerSample,
    SetVolume,
    GetVolume,
    SetPan,
    GetPan,
    SetExtraParameter,
    GetExtraParameter,
    Transfer,
    GetTransferStatus,
    AdjustBufferOnPause,
    AdjustBufferOnDiscard};

AudioPort *Open(std::int32_t channels) {
    if (channels < 1 || channels > 2)
        return 0;
    AudioPort *port = 0;
    for (std::size_t index = 0; index < audio::kAicaBankCount; ++index) {
        AudioPort &candidate = g_audioPorts[index];
        if (!candidate.allocated) {
            port = &candidate;
            break;
        }
    }
    if (!port)
        return 0;
    for (std::int32_t channel = 0; channel < channels; ++channel) {
        port->transfers[channel] = audio::nlsndDmaOpen();
        if (!port->transfers[channel]) {
            for (std::int32_t opened = 0; opened < channel; ++opened)
                audio::nlsndDmaClose(port->transfers[opened]);
            return 0;
        }
    }
    for (std::int32_t channel = 0; channel < channels; ++channel) {
        port->guardTransfers[channel] = audio::nlsndDmaOpen();
        if (!port->guardTransfers[channel]) {
            for (std::int32_t opened = 0; opened < channel; ++opened) {
                audio::nlsndDmaClose(port->guardTransfers[opened]);
                port->transfers[opened] = 0;
            }
            return 0;
        }
    }
    port->stream = static_cast<std::int16_t>(audio::nlsndStreamOpen(static_cast<std::int16_t>(channels)));
    if (port->stream < 0 || port->stream >= static_cast<std::int16_t>(audio::kAicaBankCount))
        return 0;
    port->interface = &kAudioPortInterface;
    audio::nlsndStreamConfigurationInit(&port->configuration);
    port->allocatedChannels = channels;
    port->channels = channels;
    port->bitsPerSample = 16;
    port->sampleRate = 44100;
    port->volume = 0;
    port->playing = false;
    audio::nlsndStreamConfigurationSetChannels(&port->configuration, static_cast<std::int16_t>(channels));
    SetPan(port, 0, channels == 2 ? -15 : 0);
    SetPan(port, 1, channels == 2 ? 15 : 0);
    port->extraParameters[0][0] = -1;
    port->allocated = true;
    return port;
}

const AudioLibraryInterface kAudioLibraryInterface = {{}, Initialize, Finish, Open};

} // namespace

void BindAudioStreams() {
    mwRnaSetupSnd(&kAudioLibraryInterface, 0);
}

} } } // namespace shenmue::dc::sdk

#endif
