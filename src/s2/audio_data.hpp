#pragma once

#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace shenmue::s2 {

struct Character;


struct FootstepAudioRequest {
    const Character *node;
    std::uint32_t flags;
};

using FootstepAudioRequestSink = void (*)(const FootstepAudioRequest &request) noexcept;

void SetFootstepAudioRequestSink(FootstepAudioRequestSink sink) noexcept;
bool SubmitFootstepAudioRequest(const FootstepAudioRequest &request) noexcept;



struct DialogueVoice;

inline constexpr shen_name CONVERSATION_VOICE_BUFFER{SHEN_NAME(VBUF)}, CONVERSATION_COMPRESSED_BUFFER{SHEN_NAME(AHXB)};
inline constexpr shen_name CONVERSATION_SECONDARY_VOICE_BUFFER{SHEN_NAME(VBU2)}, CONVERSATION_SECONDARY_COMPRESSED_BUFFER{SHEN_NAME(AHX2)};
inline constexpr std::uint32_t CONVERSATION_VOICE_BUFFER_SIZE = 0x23124, CONVERSATION_SECONDARY_VOICE_BUFFER_SIZE = 0x459e4;
inline constexpr std::uint32_t CONVERSATION_COMPRESSED_BUFFER_SIZE = 0x3000;
inline constexpr std::uint32_t CONVERSATION_SOURCE_COUNT = 4, CONVERSATION_WAVE_ARENA_COUNT = 2, CONVERSATION_WAVE_ARENA_SIZE = 0x400000;
inline constexpr float CONVERSATION_VOICE_SPAWN_GAIN = 1.5f;
inline constexpr std::array<float, 6> CONVERSATION_MONO_OUTPUT_MATRIX{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
inline constexpr std::array<float, 12> CONVERSATION_STEREO_OUTPUT_MATRIX{1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
inline constexpr std::array<const char *, 14> CONVERSATION_PAUSE_CATEGORY_CODES{"0007C001", "0015D001", "JINGURU01", "JINGURU03", "MEMO", "ADV", "JOY",
    "WAZAMAKIMONO", "YSMARK", "AM2LOGO", "0077C001", "WONJUNK", "stuffroll", "FUNATUKIBA2"};
extern void *g_conversationVoiceBuffer;
extern void *g_conversationCompressedBuffer;
extern void *g_conversationSecondaryVoiceBuffer;
extern void *g_conversationSecondaryCompressedBuffer;
void AllocateConversationStreamBuffers();

enum class DialogueVoiceState : std::uint32_t {
    Stopped = 0,
    Playing = 3,
};

bool InitializeDialogueAudio() noexcept;
DialogueVoice *StartDialogueVoice(std::int32_t fileId, std::uint16_t entry, float gain) noexcept;
DialogueVoiceState PollDialogueVoice(const DialogueVoice *voice) noexcept;
// Real backend sample cursor; unavailable voices leave the outputs untouched.
bool GetDialogueVoicePosition(const DialogueVoice *voice, std::uint64_t &samples, std::uint32_t &sampleRate) noexcept;
void StopDialogueVoice(DialogueVoice *voice) noexcept;
bool PauseDialogueVoice(DialogueVoice *voice, std::int32_t paused) noexcept;
bool SetDialogueVoiceVolume(DialogueVoice *voice, float gain) noexcept;
void SetDialogueVoiceReverbAttenuation(DialogueVoice *voice, std::int32_t unused, std::int32_t attenuation) noexcept;
bool IsDialogueAudioActive() noexcept;
// The conversation owner releases every CVBF voice before its audio device.
void ReleaseDialogueAudio() noexcept;
std::string_view GetS2AudioOutputName(std::int32_t index) noexcept;
void QueueS2AudioOutputChange(std::int32_t index);
void PublishS2AudioOutputAcknowledgement();
void ApplyS2AudioVolumeSettings() noexcept;
extern float g_s2CurrentSfxVolume;
extern float g_s2CurrentMusicVolume;

// The opening author owns a separate Dolby AIX program.  It is not an IDX
// voice bank and is therefore kept on its own game-side lifecycle boundary.
struct OpeningAixRequest {
    std::string_view path;
    std::byte *taskData;
    std::size_t taskDataSize;
};

enum class OpeningAixOperation : std::uint8_t {
    Start,
    Poll,
    Stop,
};

enum class OpeningAixState : std::uint8_t {
    Unsupported,
    Started,
    Playing,
    Complete,
    Failed,
};

using OpeningAixRequestSink = OpeningAixState (*)(OpeningAixOperation operation, const OpeningAixRequest &request) noexcept;

void SetOpeningAixRequestSink(OpeningAixRequestSink sink) noexcept;
bool InitializeOpeningAix() noexcept;
OpeningAixState PollOpeningAix() noexcept;
void FinalizeOpeningAix() noexcept;
bool IsAuthorStreamingAudioLoaded() noexcept;

std::string_view OpeningAixPath() noexcept;



enum class AicaBackendOperation : std::uint8_t {
    SetBankProperty,
    SetBankAllocation,
    LoadBank,
    UnloadBank,
    BankCommand,
    WideBankCommand,
    MainCommand,
    TwoChannelCommand,
    QueryBankVolume,
    Synchronize,
    PauseStreams,
    ResumeStreams,
    QueryBankSequenceState,
};

struct AicaBackendRequest {
    AicaBackendOperation operation;
    std::uint32_t command;
    std::uint32_t bank;
    std::int32_t channel;
    std::int32_t value;
    float scalar;
    std::string_view path;
    const std::byte *data{};
    std::size_t dataSize{};
};

using AicaBackendRequestSink = std::int32_t (*)(const AicaBackendRequest &request) noexcept;

void SetAicaBackendRequestSink(AicaBackendRequestSink sink) noexcept;
bool InitializeS2AicaMiddleware() noexcept;
void ReleaseS2AicaMiddleware() noexcept;
std::int32_t SubmitAicaBackendRequest(const AicaBackendRequest &request) noexcept;

} // namespace shenmue::s2
