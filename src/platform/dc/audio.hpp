#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || SHENMUE_IS_S2)

#include "platform/dc/aica_stream.hpp"

namespace shenmue { namespace dc { namespace sdk {

struct AudioPort;

struct AudioPortInterface {
    void *reserved[3];
    void (*close)(AudioPort *port);
    void (*play)(AudioPort *port);
    void (*stop)(AudioPort *port);
    std::int32_t (*getBufferSamples)(AudioPort *port);
    void (*setChannels)(AudioPort *port, std::int32_t channels);
    std::int32_t (*getPlayPosition)(AudioPort *port, std::int32_t channel);
    void (*setSampleRate)(AudioPort *port, std::int32_t rate);
    std::int32_t (*getSampleRate)(AudioPort *port);
    void (*setBitsPerSample)(AudioPort *port, std::int32_t bits);
    std::int32_t (*getBitsPerSample)(AudioPort *port);
    void (*setVolume)(AudioPort *port, std::int32_t volume);
    std::int32_t (*getVolume)(AudioPort *port);
    void (*setPan)(AudioPort *port, std::int32_t channel, std::int32_t pan);
    std::int32_t (*getPan)(AudioPort *port, std::int32_t channel);
    void (*setExtraParameter)(AudioPort *port, std::int32_t parameter, std::int32_t first, std::int32_t second);
    void (*getExtraParameter)(AudioPort *port, std::int32_t parameter, std::int32_t *first, std::int32_t *second);
    void (*transfer)(AudioPort *port, std::int32_t channel, std::int32_t position, const void *source, std::int32_t samples);
    std::int32_t (*getTransferStatus)(AudioPort *port, std::int32_t channel);
    std::int32_t (*adjustBufferOnPause)(AudioPort *port, std::int32_t position, std::int32_t *shift);
    std::int32_t (*adjustBufferOnDiscard)(AudioPort *port, std::int32_t samples);
};

struct AudioLibraryInterface {
    void *reserved[3];
    void (*initialize)(void *object);
    void (*finish)();
    AudioPort *(*open)(std::int32_t channels);
};

struct AudioPort {
    const AudioPortInterface *interface;
    bool allocated;
    bool playing;
    std::int16_t stream;
    std::int32_t allocatedChannels;
    std::int32_t channels;
    std::int32_t bitsPerSample;
    std::int32_t sampleRate;
    std::int32_t volume;
    std::int32_t pan[2];
    std::int32_t extraParameters[3][2];
    s1::AicaStreamConfiguration configuration;
    s1::AicaDmaWork *transfers[2];
    s1::AicaDmaWork *guardTransfers[2];
};

typedef char AudioLayoutCheck1[(offsetof(AudioLibraryInterface, initialize) == 0xc) ? 1 : -1];
typedef char AudioLayoutCheck2[(offsetof(AudioLibraryInterface, open) == 0x14) ? 1 : -1];
typedef char AudioLayoutCheck3[(offsetof(AudioPortInterface, transfer) == 0x4c) ? 1 : -1];
typedef char AudioLayoutCheck4[(offsetof(AudioPortInterface, adjustBufferOnDiscard) == 0x58) ? 1 : -1];

void BindAudioStreams();

} } } // namespace shenmue::dc::sdk

#endif
