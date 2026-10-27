#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

void SubmitArcadePcmVoice(std::uint32_t voiceId, std::uint32_t channels, std::uint32_t sampleRate, const std::byte *data, std::uint32_t size,
    std::uint32_t begin, std::uint32_t end, bool looped);
void GetArcadePcmVoiceOutputMatrix(std::uint32_t voiceId, float *matrix);

} // namespace shenmue::s2
