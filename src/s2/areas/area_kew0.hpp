#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/seqp_event.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name KEW0_TASK{SHEN_NAME(EVNT)}, KEW0_SEQUENCE{SHEN_NAME(SEQP)};
inline constexpr shen_name KEW0_RYO{SHEN_NAME(RYO_)}, KEW0_KEK{SHEN_NAME(KEK_)}, KEW0_IKO{SHEN_NAME(IKO_)}, KEW0_MOK{SHEN_NAME(MOK_)};
inline constexpr shen_name KEW0_BASE{SHEN_NAME(BASE)}, KEW0_PROP{SHEN_NAME(a073)}, KEW0_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name KEW0_INTRO{SHEN_NAME(AU_0)}, KEW0_LEARN{SHEN_NAME(AU_1)}, KEW0_WIN{SHEN_NAME(AU_2)}, KEW0_LOSE{SHEN_NAME(AU_3)};
inline constexpr shen_name KEW0_BATTLE_SEQUENCE{SHEN_NAME(KEBT)}, KEW0_LCD_SEQUENCE{SHEN_NAME(DLCD)};
inline constexpr shen_name KEW0_CHARACTERS[]{KEW0_RYO, KEW0_KEK, KEW0_IKO, KEW0_MOK, EVENT_CHARACTER_INVALID};
inline constexpr Vector4 KEW0_REFLECTOR{1, .3f, .3f, .3f};
inline constexpr Vector3 KEW0_RYO_POSITION{74.9000015f, 102.5f, 292.899994f}, KEW0_MOK_POSITION{74.0999985f, 102.5f, 295.5f};
inline constexpr Vector3 KEW0_KEK_POSITION{73.5800018f, 102.5f, 289.630005f}, KEW0_IKO_POSITION{69.5999985f, 102.5f, 295.399994f};
inline constexpr Vector3i KEW0_PROP_ROTATION{0, 0, 49152};
inline Vector3 kew0PropAttachmentPosition;
inline constexpr Vector3 KEW0_CAMERA_POSITION{72.235001f, 103.9506f, 290.49091f}, KEW0_CAMERA_TARGET{74.893898f, 103.415f, 292.72119f};
inline std::int32_t kew0MokYaw;
inline constexpr std::uint16_t KEW0_IDLE = 0xf003, KEW0_ANGRY_A = 6593, KEW0_ANGRY_B = 6594, KEW0_ANGRY_C = 6595, KEW0_IKO_MOTION = 6596;
inline constexpr std::uint8_t KEW0_LCD_BITMAP[]{0x06, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0e, 0x03, 0xc0, 0x7f, 0x80, 0x04, 0x92, 0x02, 0x40, 0x40, 0x40,
    0x04, 0x92, 0x02, 0x40, 0x40, 0x20, 0x04, 0x92, 0x02, 0x40, 0x4f, 0x20, 0x02, 0x64, 0x1e, 0x78, 0x4f, 0x20, 0x02, 0x64, 0x10, 0x08, 0x40, 0x20, 0x01, 0x08,
    0x10, 0x08, 0x40, 0x40, 0x01, 0x08, 0x10, 0x08, 0x4f, 0x20, 0x00, 0x90, 0x1e, 0x78, 0x49, 0x20, 0x00, 0x90, 0x02, 0x40, 0x4f, 0x20, 0x00, 0x90, 0x02, 0x40,
    0x40, 0x20, 0x00, 0x90, 0x02, 0x40, 0x40, 0x40, 0x00, 0xf0, 0x03, 0xc0, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
struct Kew0EventWork {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::int32_t sequenceActive, state, reserved18, reserved1c;
    std::uint32_t motions[16];
    EventReadCharactersWork *characters;
    std::int32_t entry;
    std::uint8_t battlePhase;
    std::int8_t learnedResult;
    std::int16_t learnedFrames;
    std::uint8_t fightResult, dialogueIndex, reserved72, reserved73;
    std::uint32_t dialogueFrames, targetFrames, battleFrames, lcdFrames, reserved84;
    const std::byte *lcdBitmap;
    CompiledSequenceWork *lcdSequence;
    std::uint64_t reserved98;
    std::int32_t adviceShown, reserveda4;
};
struct Kew0AuthorWork {
    HLib::Task *task;
    std::uint32_t motion;
    shen_name author;
    std::uint16_t state, reserved12;
    std::int32_t savedPostFx;
};
struct Kew0AuthorLoopWork {
    Kew0AuthorWork *parent;
    std::uint16_t unused08;
    std::byte reserved0a[6];
};
static_assert(sizeof(Kew0EventWork) == 0xa8 && offsetof(Kew0EventWork, characters) == 0x60 && offsetof(Kew0EventWork, entry) == 0x68);
static_assert(offsetof(Kew0EventWork, learnedResult) == 0x6d && offsetof(Kew0EventWork, lcdSequence) == 0x90);
static_assert(sizeof(Kew0AuthorWork) == 0x18 && offsetof(Kew0AuthorWork, state) == 0x10 && sizeof(Kew0AuthorLoopWork) == 0x10);
void Kew0Init();
void Kew0Main(std::uintptr_t argument);
extern const ScriptAreaDefinition kew0ScriptDefinition;
} // namespace shenmue::s2
