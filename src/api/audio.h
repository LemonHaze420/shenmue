#ifndef SHEN_AUDIO_H
#define SHEN_AUDIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum shen_audio_buffer_flags { SHEN_AUDIO_END_OF_STREAM = 0x0040 };
enum shen_audio_state_flags { SHEN_AUDIO_NO_SAMPLES_PLAYED = 0x0100 };
enum shen_audio_stop_flags { SHEN_AUDIO_PLAY_TAILS = 0x0020 };
enum shen_audio_filter_type {
    SHEN_AUDIO_LOW_PASS = 0,
    SHEN_AUDIO_BAND_PASS = 1,
    SHEN_AUDIO_HIGH_PASS = 2,
    SHEN_AUDIO_NOTCH = 3,
    SHEN_AUDIO_LOW_PASS_ONE_POLE = 4,
    SHEN_AUDIO_HIGH_PASS_ONE_POLE = 5
};
enum { SHEN_AUDIO_LOOP_INFINITE = 255 };

#pragma pack(push, 1)
typedef struct shen_audio_wave_format {
    uint16_t format_tag;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t average_bytes_per_second;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint16_t extra_size;
} shen_audio_wave_format;
#pragma pack(pop)

typedef struct shen_audio_buffer {
    uint32_t flags;
    uint32_t audio_bytes;
    const uint8_t *audio_data;
    uint32_t play_begin;
    uint32_t play_length;
    uint32_t loop_begin;
    uint32_t loop_length;
    uint32_t loop_count;
    void *context;
} shen_audio_buffer;

typedef struct shen_audio_wma_buffer {
    const uint32_t *decoded_packet_bytes;
    uint32_t packet_count;
} shen_audio_wma_buffer;

typedef struct shen_audio_voice_state {
    void *buffer_context;
    uint32_t buffers_queued;
    uint64_t samples_played;
} shen_audio_voice_state;

typedef struct shen_audio_voice_details {
    uint32_t creation_flags;
    uint32_t active_flags;
    uint32_t input_channels;
    uint32_t input_sample_rate;
} shen_audio_voice_details;

typedef struct shen_audio_filter {
    uint32_t type;
    float frequency;
    float one_over_q;
} shen_audio_filter;

void shen_audio_voice_destroy(void *voice);
void shen_audio_voice_get_details(void *voice, shen_audio_voice_details *details);
void shen_audio_voice_get_volume(void *voice, float *volume);
int32_t shen_audio_voice_set_volume(void *voice, float volume, uint32_t operation_set);
int32_t shen_audio_voice_set_matrix(void *voice, void *output, uint32_t source_channels, uint32_t output_channels, const float *matrix, uint32_t operation_set);
int32_t shen_audio_voice_set_filter(void *voice, const shen_audio_filter *filter, uint32_t operation_set);
int32_t shen_audio_source_start(void *voice, uint32_t flags, uint32_t operation_set);
int32_t shen_audio_source_stop(void *voice, uint32_t flags, uint32_t operation_set);
int32_t shen_audio_source_flush(void *voice);
int32_t shen_audio_source_exit_loop(void *voice, uint32_t operation_set);
int32_t shen_audio_source_set_sample_rate(void *voice, uint32_t sample_rate);
int32_t shen_audio_source_set_frequency_ratio(void *voice, float ratio, uint32_t operation_set);
int32_t shen_audio_source_submit(void *voice, const shen_audio_buffer *buffer, const shen_audio_wma_buffer *packets);
void shen_audio_source_get_state(void *voice, shen_audio_voice_state *state, uint32_t flags);

#ifdef __cplusplus
}
#endif

#endif
