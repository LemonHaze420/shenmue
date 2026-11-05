#include "api/audio.h"

#include <xaudio2.h>

static_assert(SHEN_AUDIO_END_OF_STREAM == XAUDIO2_END_OF_STREAM);
static_assert(SHEN_AUDIO_NO_SAMPLES_PLAYED == XAUDIO2_VOICE_NOSAMPLESPLAYED);
static_assert(SHEN_AUDIO_PLAY_TAILS == XAUDIO2_PLAY_TAILS);
static_assert(SHEN_AUDIO_LOOP_INFINITE == XAUDIO2_LOOP_INFINITE);

extern "C" {

void shen_audio_voice_destroy(void *voice) {
    static_cast<IXAudio2Voice *>(voice)->DestroyVoice();
}

void shen_audio_voice_get_details(void *voice, shen_audio_voice_details *details) {
    XAUDIO2_VOICE_DETAILS native{};
    static_cast<IXAudio2Voice *>(voice)->GetVoiceDetails(&native);
    details->creation_flags = native.CreationFlags;
    details->active_flags = native.ActiveFlags;
    details->input_channels = native.InputChannels;
    details->input_sample_rate = native.InputSampleRate;
}

void shen_audio_voice_get_volume(void *voice, float *volume) {
    static_cast<IXAudio2Voice *>(voice)->GetVolume(volume);
}

int32_t shen_audio_voice_set_volume(void *voice, float volume, uint32_t operation_set) {
    return static_cast<IXAudio2Voice *>(voice)->SetVolume(volume, operation_set);
}

int32_t shen_audio_voice_set_matrix(
    void *voice, void *output, uint32_t source_channels, uint32_t output_channels, const float *matrix, uint32_t operation_set) {
    return static_cast<IXAudio2Voice *>(voice)->SetOutputMatrix(static_cast<IXAudio2Voice *>(output), source_channels, output_channels, matrix, operation_set);
}

int32_t shen_audio_voice_set_filter(void *voice, const shen_audio_filter *filter, uint32_t operation_set) {
    const XAUDIO2_FILTER_PARAMETERS native{static_cast<XAUDIO2_FILTER_TYPE>(filter->type), filter->frequency, filter->one_over_q};
    return static_cast<IXAudio2Voice *>(voice)->SetFilterParameters(&native, operation_set);
}

int32_t shen_audio_source_start(void *voice, uint32_t flags, uint32_t operation_set) {
    return static_cast<IXAudio2SourceVoice *>(voice)->Start(flags, operation_set);
}

int32_t shen_audio_source_stop(void *voice, uint32_t flags, uint32_t operation_set) {
    return static_cast<IXAudio2SourceVoice *>(voice)->Stop(flags, operation_set);
}

int32_t shen_audio_source_flush(void *voice) {
    return static_cast<IXAudio2SourceVoice *>(voice)->FlushSourceBuffers();
}

int32_t shen_audio_source_exit_loop(void *voice, uint32_t operation_set) {
    return static_cast<IXAudio2SourceVoice *>(voice)->ExitLoop(operation_set);
}

int32_t shen_audio_source_set_sample_rate(void *voice, uint32_t sample_rate) {
    return static_cast<IXAudio2SourceVoice *>(voice)->SetSourceSampleRate(sample_rate);
}

int32_t shen_audio_source_set_frequency_ratio(void *voice, float ratio, uint32_t operation_set) {
    return static_cast<IXAudio2SourceVoice *>(voice)->SetFrequencyRatio(ratio, operation_set);
}

int32_t shen_audio_source_submit(void *voice, const shen_audio_buffer *buffer, const shen_audio_wma_buffer *packets) {
    const XAUDIO2_BUFFER native{buffer->flags, buffer->audio_bytes, buffer->audio_data, buffer->play_begin, buffer->play_length, buffer->loop_begin,
        buffer->loop_length, buffer->loop_count, buffer->context};
    XAUDIO2_BUFFER_WMA native_packets{};
    if (packets) {
        native_packets.pDecodedPacketCumulativeBytes = packets->decoded_packet_bytes;
        native_packets.PacketCount = packets->packet_count;
    }
    return static_cast<IXAudio2SourceVoice *>(voice)->SubmitSourceBuffer(&native, packets ? &native_packets : nullptr);
}

void shen_audio_source_get_state(void *voice, shen_audio_voice_state *state, uint32_t flags) {
    XAUDIO2_VOICE_STATE native{};
    static_cast<IXAudio2SourceVoice *>(voice)->GetState(&native, flags);
    state->buffer_context = native.pCurrentBufferContext;
    state->buffers_queued = native.BuffersQueued;
    state->samples_played = native.SamplesPlayed;
}
}
