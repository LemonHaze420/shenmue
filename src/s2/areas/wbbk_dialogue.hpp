#pragma once
#include <cstdint>
namespace shenmue::s2::areas {
extern const char *const bookMinorDropVoices[3];
extern const char *const bookDropPlayerVoices[4];
extern const char *const bookDropCompanionVoices[4];
extern const char *const bookDropReplyVoices[4];
std::int32_t QueueBookDialogue(std::int32_t sequence, std::int32_t books);
} // namespace shenmue::s2::areas
