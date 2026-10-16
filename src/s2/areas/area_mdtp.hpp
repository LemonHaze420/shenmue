#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/area_loop.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct ConversationBuffer;
inline constexpr shen_name MDTP_AREA{SHEN_NAME(MDTP)}, MDTP_PLAYER{SHEN_NAME(RYO_)}, MDTP_RECORDER{SHEN_NAME(rec_)}, MDTP_TAPE{SHEN_NAME(tap0)};
inline constexpr shen_name MDTP_EMPTY_RECORDER{SHEN_NAME(recw)}, MDTP_LOADED_RECORDER{SHEN_NAME(rece)}, MDTP_REN{SHEN_NAME(REN_)};
inline constexpr shen_name MDTP_DESK{SHEN_NAME(DESK)}, MDTP_LAMP{SHEN_NAME(LAMP)}, MDTP_CHAIR{SHEN_NAME(CHIR)}, MDTP_DOOR{SHEN_NAME(R101)};
inline constexpr shen_name MDTP_BASE{SHEN_NAME(mdtp)}, MDTP_FREE_BASE{SHEN_NAME($fre)}, MDTP_OUTDOOR{SHEN_NAME(QR00)};
inline constexpr shen_name MDTP_INIT_AUTHOR{SHEN_NAME(INIT)}, MDTP_QUIT_AUTHOR{SHEN_NAME(QUIT)}, MDTP_NEXT_DAY{SHEN_NAME(NDAY)};
inline constexpr shen_name MDTP_ACCEPT{SHEN_NAME(BTEV)}, MDTP_CANCEL{SHEN_NAME(BTCC)}, MDTP_TAPE_SIGNATURE{SHEN_NAME(TAPE)};
inline constexpr char MDTP_PACKAGE[] = "MDTP", MDTP_AUTHOR_PACKAGE[] = "0225\0", MDTP_MEDIA[] = "0225A000";
inline constexpr std::uint16_t MDTP_MOTION = 0x1f42;
inline constexpr std::uint16_t MDTP_COUNTER_PARTS[]{0xa2, 0xa1, 0xa0};
inline constexpr std::int32_t MDTP_BUTTON_PARTS[]{152, 153, 154, 155, 156, -1};
struct MdtpCommand {
    std::int32_t command;
    std::uint32_t tape;
};
struct MdtpTapeEntry {
    std::uint32_t flags, offset;
};
struct MdtpTape {
    shen_name signature;
    std::int32_t count;
};
struct MdtpTapeClip {
    std::int32_t start, duration;
    std::uint32_t reserved08, bankOffset, voiceOffset;
};

struct MdtpWork {
    HLib::Task *parent;
    HLib::Task *task;
    HLib::Task *pauseTask;
    std::uint32_t tapeIndex, flags;
    std::int32_t button, buttonFrames[5], tapeFrame, reserved3c;
    const char *bank;
    ConversationBuffer *voice;
    ConversationBuffer *pausedVoice;
    std::int32_t voiceClip, pausedClip, seekClip, bingoClip;
    const char *package;
    const char *media;
    std::int32_t selectedButton, handBlendFrames, pressFrames;
    Vector3 handPosition;
    const MdtpTape *tape;
};
#pragma pack(push, 4)
struct MdtpPauseWork {
    MdtpWork *owner;
    std::int32_t phase;
};
#pragma pack(pop)
struct MdtpPackageWork {
    HLib::Task *parent;
    HLib::Task *task;
    std::int32_t recorder, npcState;
};
struct MdtpAuthorWork {
    HLib::Task *parent;
    std::uint32_t flags, reserved0c;
    const char *package;
    shen_name author;
    std::int32_t frame, endFrame, reserved24;
    AreaLoopCompletion completion;
};
struct MdtpSubtitle {
    const char *token;
    float duration, alternateDuration;
};
struct MdtpSubtitleWork {
    std::int32_t state, reserved04;
    float deadline;
    std::int32_t reserved0c;
    const MdtpSubtitle *cursor;
};
static_assert(sizeof(MdtpCommand) == 8 && sizeof(MdtpTape) == 8 && sizeof(MdtpTapeEntry) == 8 && sizeof(MdtpTapeClip) == 20);
static_assert(sizeof(MdtpWork) == 0x98 && offsetof(MdtpWork, flags) == 0x1c && offsetof(MdtpWork, voice) == 0x48);
static_assert(offsetof(MdtpWork, tapeFrame) == 0x38 && offsetof(MdtpWork, package) == 0x68 && offsetof(MdtpWork, handPosition) == 0x84);
static_assert(offsetof(MdtpWork, tape) == 0x90 && sizeof(MdtpPauseWork) == 0x0c && sizeof(MdtpPackageWork) == 0x18);
static_assert(sizeof(MdtpAuthorWork) == 0x30 && offsetof(MdtpAuthorWork, frame) == 0x1c && offsetof(MdtpAuthorWork, completion) == 0x28);
static_assert(sizeof(MdtpSubtitle) == 0x10 && sizeof(MdtpSubtitleWork) == 0x18 && offsetof(MdtpSubtitleWork, cursor) == 0x10);
extern HLib::Task *mdtpSubtitleTask;
extern float mdtpSubtitleSeconds;
void MDTPInit();
void MDTPMain(std::uintptr_t argument);
extern const ScriptAreaDefinition mdtpScriptDefinition;
void SetupAuthorOfIdAtTime(std::int32_t index);
bool QueueMdtp(std::int32_t command, std::uint32_t tape);
void MdtpSubtitleCallback(void *data);
MdtpSubtitleWork *QueueMdtpSubtitles(std::int32_t clip);
void MdtpAuthorCallback(void *data);
bool QueueMdtpAuthor(shen_name author, AreaLoopCompletion completion);
const char *ResolveMdtpSubtitle(const char *token);
const MdtpTape *FindMdtpTape(const char *package, std::uint32_t index);
std::int32_t FindMdtpTapeClip(const MdtpTape *tape, std::int32_t frame);
const MdtpTapeClip *GetMdtpTapeClip(const MdtpTape *tape, std::int32_t index);
void MdtpQuitCallback(void *data);
void MdtpInitializeCallback(void *data);
void MdtpInitializeAuthorCallback(void *data);
void MdtpBeginAuthorCallback(void *data);
void MdtpInitializeFlagsCallback(void *data);
void MdtpRecorderCallback(void *data);
void MdtpBeginExitQuestionCallback(void *data);
void MdtpExitQuestionCallback(void *data);
void MdtpFinishExitCallback(void *data);
void MdtpBeginBingoCallback(void *data);
void MdtpBingoCallback(void *data);
void MdtpOutdoorCallback(void *data);
void MdtpPauseCallback(void *data);
void MdtpPackageCallback(void *data);
std::int64_t MdtpInitAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent);
std::int64_t MdtpQuitAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent);
void UpdateMdtpHand(MdtpWork *work);
bool UpdateMdtpButtons(std::int32_t selected, std::int32_t *press, std::int32_t *active, std::int32_t *frames, std::int32_t *stopping);
void MaintainMdtpMotion(std::int32_t transition);
} // namespace shenmue::s2
