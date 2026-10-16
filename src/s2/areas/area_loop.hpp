#pragma once
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct ConversationBuffer;
using AreaLoopCompletion = std::int64_t (*)(std::int32_t, std::int64_t, HLib::Task *);
struct AreaLoopPositionWork {
    HLib::Task *parent;
    shen_name character;
    std::uint32_t flags;
    Vector3 position;
    Vector3i rotation;
    AreaLoopCompletion completion;
};
struct AreaLoopMotionWork {
    HLib::Task *parent;
    shen_name character;
    std::uint32_t flags;
    std::uint32_t motion;
    std::int32_t duration;
    std::uint32_t elapsed;
    float startFrame, endFrame, speed;
    std::uint32_t frameTime;
    std::int32_t reserved2c;
    AreaLoopCompletion completion;
};
struct AreaLoopVoiceWork {
    HLib::Task *parent;
    ConversationBuffer *conversation;
    std::uint32_t flags;
    std::int32_t duration;
    std::uint32_t elapsed, reserved1c;
    AreaLoopCompletion completion;
    const char *voice;
};
struct AreaLoopCameraWork {
    HLib::Task *parent;
    std::uint32_t flags;
    std::int32_t camera;
    shen_name character;
    std::uint32_t reserved14;
    AreaLoopCompletion completion;
    const char *area;
};
struct AreaLoopNpcWork {
    HLib::Task *parent;
    std::uint64_t previousLoadState;
    std::int32_t remaining;
    std::uint32_t reserved14;
    shen_name *characters;
    AreaLoopCompletion completion;
};
struct AreaLoopAttachmentWork {
    HLib::Task *parent;
    std::uint32_t elapsed;
    shen_name character;
    std::uint32_t attachmentFlags;
    shen_name target;
    std::uint32_t flags;
    Vector3 position;
    Vector3i rotation;
    std::int32_t duration;
    AreaLoopCompletion completion;
};
struct AreaLoopLookAtWork {
    HLib::Task *parent;
    std::uint32_t flags;
    shen_name character, target;
    std::int32_t bone;
    Vector3 position;
    std::uint32_t elapsed;
    std::int32_t duration, reserved2c;
    AreaLoopCompletion completion;
};
static_assert(sizeof(AreaLoopAttachmentWork) == 0x40);
static_assert(offsetof(AreaLoopAttachmentWork, flags) == 0x18);
static_assert(offsetof(AreaLoopAttachmentWork, completion) == 0x38);
static_assert(sizeof(AreaLoopLookAtWork) == 0x38);
static_assert(offsetof(AreaLoopLookAtWork, position) == 0x18);
static_assert(offsetof(AreaLoopLookAtWork, completion) == 0x30);
struct AreaLoopAuthorWork {
    HLib::Task *parent;
    std::uint32_t flags, reserved0c;
    const char *package;
    shen_name author;
    std::int32_t endFrame;
    AreaLoopCompletion completion;
};
static_assert(sizeof(AreaLoopAuthorWork) == 0x28);
static_assert(offsetof(AreaLoopAuthorWork, package) == 0x10);
static_assert(offsetof(AreaLoopAuthorWork, completion) == 0x20);
struct AreaLoopPackageWork {
    HLib::Task *parent;
    std::uint32_t flags, reserved0c;
    const char *package;
    const char *folder;
    AreaLoopCompletion completion;
};
static_assert(sizeof(AreaLoopPackageWork) == 0x28);
static_assert(offsetof(AreaLoopPackageWork, package) == 0x10);
static_assert(offsetof(AreaLoopPackageWork, completion) == 0x20);
static_assert(sizeof(AreaLoopPositionWork) == 0x30);
static_assert(offsetof(AreaLoopPositionWork, rotation) == 0x1c);
static_assert(sizeof(AreaLoopMotionWork) == 0x38);
static_assert(offsetof(AreaLoopMotionWork, completion) == 0x30);
static_assert(sizeof(AreaLoopVoiceWork) == 0x30);
static_assert(offsetof(AreaLoopVoiceWork, voice) == 0x28);
static_assert(sizeof(AreaLoopCameraWork) == 0x28);
static_assert(offsetof(AreaLoopCameraWork, area) == 0x20);
static_assert(sizeof(AreaLoopNpcWork) == 0x28);
static_assert(offsetof(AreaLoopNpcWork, remaining) == 0x10);
void AreaLoopPositionCallback(void *);
void AreaLoopMotionCallback(void *);
void AreaLoopVoiceCallback(void *);
void AreaLoopCameraCallback(void *);
void AreaLoopNpcCallback(void *);
void AreaLoopAttachmentCallback(void *);
void AreaLoopLookAtCallback(void *);
void AreaLoopAuthorCallback(void *);
bool QueueAreaLoopAuthor(const char *package, shen_name author, AreaLoopCompletion completion);
void AreaLoopPackageCallback(void *);
bool QueueAreaLoopPackage(const char *folder, const char *package);
AreaLoopPositionWork *QueueAreaLoopPosition(const AreaLoopPositionWork &, std::int32_t delay);
AreaLoopMotionWork *QueueAreaLoopMotion(const AreaLoopMotionWork &, std::int32_t delay);
AreaLoopVoiceWork *QueueAreaLoopVoice(const char *voice, std::uint32_t flags, std::int32_t duration, AreaLoopCompletion completion, std::int32_t delay);
AreaLoopCameraWork *QueueAreaLoopCamera(const char *area, std::int32_t camera, shen_name character, AreaLoopCompletion completion, std::int32_t delay);
bool QueueAreaLoopNpcs(const shen_name *characters, AreaLoopCompletion completion);
void SetAreaLoopCharacterControl(shen_name character, std::int32_t scripted);
void RestoreAreaLoopCharacters(const shen_name *characters);
std::int64_t ResolveAreaLoopParent(std::int32_t phase, std::int64_t, HLib::Task *parent);
} // namespace shenmue::s2
