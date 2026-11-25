#pragma once
#include "platform/dc/aica.hpp"
#include "common/types.hpp"
#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || SHENMUE_IS_S2)
namespace shenmue { namespace s1 {

struct AicaStreamChannelConfiguration {
    std::uint8_t directLevel;
    std::uint8_t volume;
    std::uint8_t pan;
    std::uint8_t effects;
};
struct AicaStreamConfiguration {
    std::uint32_t signature;
    std::uint32_t version;
    std::uint8_t format;
    std::uint8_t channelCode;
    std::uint16_t interleave;
    std::uint32_t dataBytes;
    std::uint16_t envelope[5];
    std::uint8_t retained1a[4];
    std::uint8_t parameter1e;
    std::uint8_t parameter1f;
    std::uint16_t retained20;
    std::uint16_t pitch;
    std::uint8_t retained24[6];
    std::uint16_t sampleRate;
    std::uint8_t retained2c[4];
    AicaStreamChannelConfiguration channels[4];
};
typedef char AicastreamLayoutCheck1[(sizeof(AicaStreamConfiguration) == 0x40) ? 1 : -1];
typedef char AicastreamLayoutCheck2[(offsetof(AicaStreamConfiguration, format) == 8) ? 1 : -1];
typedef char AicastreamLayoutCheck3[(offsetof(AicaStreamConfiguration, pitch) == 0x22) ? 1 : -1];
typedef char AicastreamLayoutCheck4[(offsetof(AicaStreamConfiguration, sampleRate) == 0x2a) ? 1 : -1];
typedef char AicastreamLayoutCheck5[(offsetof(AicaStreamConfiguration, channels) == 0x30) ? 1 : -1];
static const AicaStreamConfiguration kAicaStreamDefaultConfiguration = {
    SHEN_NAME4('S', 'P', 'S', 'D'), 0x04000100, 1, 0, 0, 0x00ffffff,
        {0x1ff7, 0x1ff7, 0x1ff7, 0x1ff7, 0x1ff7}, {}, 4, 1, 0, 0x7800,
        {0xdf, 7, 0x1f, 0x3c, 0, 0}, 22050, {},
        {{15, 127, 0, 0}, {15, 127, 0, 0}, {15, 127, 0, 0}, {15, 127, 0, 0}}};
enum AicaStreamConfigurationProperty {
    AicaStreamConfigurationProperty_SampleRate = 0,
    AicaStreamConfigurationProperty_BitsPerSample = 1,
    AicaStreamConfigurationProperty_SampleCount = 2,
    AicaStreamConfigurationProperty_DataBytes = 3,
    AicaStreamConfigurationProperty_Channels = 4,
    AicaStreamConfigurationProperty_Volume = 5,
    AicaStreamConfigurationProperty_Pan = 6,
    AicaStreamConfigurationProperty_EffectSend = 7,
    AicaStreamConfigurationProperty_EffectLevel = 8,
    AicaStreamConfigurationProperty_Unknown9 = 9,
    AicaStreamConfigurationProperty_Unknown10 = 10,
};
typedef char AicaStreamPropertySizeCheck[(sizeof(AicaStreamConfigurationProperty) == 4) ? 1 : -1];
struct AicaDmaWork {
    std::int32_t state;
#if SHENMUE_IS_S2
    const void *source;
    void *destination;
    std::uint32_t bytes;
#else
    std::uint8_t retained04[12];
#endif
};
typedef char AicastreamLayoutCheck6[(sizeof(AicaDmaWork) == 0x10) ? 1 : -1];
} } // namespace shenmue::s1
#if SHENMUE_IS_S2
namespace shenmue { namespace s2 {
using s1::AicaStreamChannelConfiguration;
using s1::AicaStreamConfiguration;
using s1::AicaStreamConfigurationProperty;
using s1::AicaStreamConfigurationProperty_SampleRate;
using s1::AicaStreamConfigurationProperty_BitsPerSample;
using s1::AicaStreamConfigurationProperty_SampleCount;
using s1::AicaStreamConfigurationProperty_DataBytes;
using s1::AicaStreamConfigurationProperty_Channels;
using s1::AicaStreamConfigurationProperty_Volume;
using s1::AicaStreamConfigurationProperty_Pan;
using s1::AicaStreamConfigurationProperty_EffectSend;
using s1::AicaStreamConfigurationProperty_EffectLevel;
using s1::AicaStreamConfigurationProperty_Unknown9;
using s1::AicaStreamConfigurationProperty_Unknown10;
using s1::AicaDmaWork;
using s1::kAicaStreamDefaultConfiguration;
#else
namespace shenmue { namespace s1 {
#endif
extern AicaDmaWork g_aicaDmaWork[16];
extern std::int32_t g_aicaDmaTransferMode;

void nlsndStreamConfigure(std::int16_t stream, const AicaStreamConfiguration *configuration);
void *nlsndStreamGetBuffer(std::int16_t stream, std::int16_t channel, std::uint32_t *bytes);
#if SHENMUE_IS_S2
std::uint32_t nlsndStreamGetSampleRate(std::int16_t stream);
std::int32_t nlsndStreamGetBitsPerSample(std::int16_t stream);
void nlsndStreamClose(std::int16_t stream);
#else
std::uint32_t nlsndStreamGetSampleRate(std::int8_t stream);
std::int32_t nlsndStreamGetBitsPerSample(std::int8_t stream);
void nlsndStreamClose(std::int8_t stream);
#endif
std::uint32_t nlsndStreamGetPlayPosition(std::int16_t stream);
std::int32_t nlsndStreamOpen(std::int16_t channels);
void nlsndStreamPlay(std::int16_t stream);
void nlsndStreamStop(std::int16_t stream);
void nlsndStreamConfigurationInit(AicaStreamConfiguration *configuration);
void nlsndStreamConfigurationSetPitch(AicaStreamConfiguration *configuration, std::int16_t octave, std::uint16_t pitch);
void nlsndStreamConfigurationSetChannels(AicaStreamConfiguration *configuration, std::int16_t channels);
void nlsndStreamConfigurationSetVolume(AicaStreamConfiguration *configuration, std::int16_t channel, std::uint8_t volume);
void nlsndStreamConfigurationSetPan(AicaStreamConfiguration *configuration, std::int16_t channel, std::uint8_t pan);
void nlsndStreamConfigurationSetBitsPerSample(AicaStreamConfiguration *configuration, std::int16_t bits);
#if SHENMUE_IS_S2
void nlsndStreamConfigurationSetEffects(AicaStreamConfiguration *configuration, std::uint8_t send, std::int16_t level);
#else
void nlsndStreamConfigurationSetEffects(AicaStreamConfiguration *configuration, std::int8_t level, std::uint8_t send);
#endif
void nlsndStreamConfigurationSetDirectLevel(AicaStreamConfiguration *configuration, std::uint8_t level);
std::int32_t nlsndStreamConfigurationGet(const AicaStreamConfiguration *configuration, AicaStreamConfigurationProperty property, std::int16_t channel);
void nlsndStreamSetVolume(std::int16_t stream, std::int16_t channel, std::int16_t volume);
void nlsndStreamSetPan(std::int16_t stream, std::int16_t channel, std::int16_t pan);
void nlsndStreamSetParameter0C(std::int16_t stream, std::int16_t value);
void nlsndStreamSetParameter08(std::int16_t stream, std::int16_t value);
void nlsndStreamSetParameter09(std::int16_t stream, std::int16_t value);
void nlsndStreamSetEffectsControl(std::int16_t stream, std::int16_t octave, std::int16_t pitch);
void nlsndStreamSetDirectControl(std::int16_t stream, std::int16_t first, std::int16_t second);
#if SHENMUE_IS_S2
std::int32_t nlsndDmaInitialize();
std::int32_t nlsndDmaFinish();
std::int32_t nlsndDmaSetTransferMode(std::int32_t mode);
#else
void nlsndDmaInitialize();
void nlsndDmaFinish();
void nlsndDmaSetTransferMode(std::int32_t mode);
#endif
#if SHENMUE_IS_S2
std::int32_t nlsndDmaCancel(AicaDmaWork *work);
std::int32_t nlsndDmaBegin(AicaDmaWork *work);
void nlsndDmaAdvance();
#endif
std::int32_t nlsndDmaGetTransferMode();
AicaDmaWork *nlsndDmaOpen();
std::int32_t nlsndDmaClose(AicaDmaWork *work);
std::int32_t nlsndDmaTransfer(AicaDmaWork *work, const void *source, void *destination, std::uint32_t bytes);
std::int32_t nlsndDmaGetStatus(const AicaDmaWork *work);
} } // namespace shenmue::s1
#endif
