#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
inline constexpr shen_name QDTB_TASK{SHEN_NAME(EVNT)}, QDTB_SEQUENCE{SHEN_NAME(SEQP)}, QDTB_BATTLE{SHEN_NAME(BSBT)};
inline constexpr shen_name QDTB_RYO{SHEN_NAME(RYO_)}, QDTB_TGY{SHEN_NAME(TGY_)}, QDTB_YOH{SHEN_NAME(YOH_)}, QDTB_RKN{SHEN_NAME(RKN_)};
inline constexpr shen_name QDTB_III{SHEN_NAME(III_)}, QDTB_SEE{SHEN_NAME(SEE_)}, QDTB_SYE{SHEN_NAME(SYE_)}, QDTB_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name QDTB_TENT{SHEN_NAME(TENT)}, QDTB_A235{SHEN_NAME(a235)}, QDTB_A352{SHEN_NAME(a352)}, QDTB_A401{SHEN_NAME(a401)};
inline constexpr shen_name QDTB_A397{SHEN_NAME(a397)}, QDTB_A398{SHEN_NAME(a398)}, QDTB_RETURN{SHEN_NAME(0250)};
inline constexpr shen_name QDTB_AUTHORS[]{SHEN_NAME(AU_0), SHEN_NAME(AU_1), SHEN_NAME(AU_2)};
inline constexpr shen_name QDTB_CHARACTERS[]{QDTB_RYO, QDTB_TGY, QDTB_YOH, QDTB_RKN, QDTB_III, QDTB_SEE, QDTB_SYE, EVENT_CHARACTER_INVALID};
inline constexpr shen_name QDTB_OPPONENTS[]{QDTB_TGY, QDTB_YOH, QDTB_RKN, QDTB_III, QDTB_SEE, QDTB_SYE, EVENT_CHARACTER_INVALID};
inline constexpr shen_name QDTB_ALPHA_MODELS[]{QDTB_A235, QDTB_A352, QDTB_A397, QDTB_A398, QDTB_A401};
inline constexpr std::int32_t QDTB_TENT_PARTS[]{175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157, 166, 154, 173, 168, 153, 165, 160, 176, 171, 164,
    159, 161, 156, 192, 193, 180, 181, 182, 189, 190, 191, 186, 187, 188, 183, 184, 185, 177, 178, 179};
inline constexpr Vector3 QDTB_POSITIONS[]{{176.1300048828125f, 128.60000610351562f, 58.060001373291016f},
    {176.10000610351562f, 128.60000610351562f, 59.470001220703125f}, {178.5800018310547f, 128.60000610351562f, 56.970001220703125f},
    {173.47000122070312f, 128.60000610351562f, 57.29999923706055f}, {177.00999450683594f, 128.60000610351562f, 55.52000045776367f},
    {175.2899932861328f, 128.60000610351562f, 55.779998779296875f}};
inline constexpr Vector3i QDTB_ROTATIONS[]{{0, 34588, 0}, {0, 109, 0}, {0, 15561, 0}, {0, 49519, 0}, {0, 26460, 0}, {0, 45219, 0}};
inline constexpr shen_name QDTB_POSE_CHARACTERS[]{QDTB_RYO, QDTB_TGY, QDTB_RKN, QDTB_SEE, QDTB_YOH, QDTB_III};
inline constexpr shen_name QDTB_STANCE_CHARACTERS[]{QDTB_RYO, QDTB_TGY, QDTB_YOH, QDTB_III, QDTB_SEE, QDTB_RKN};
inline constexpr std::uint32_t QDTB_STANCES[]{306, 2070, 3632, 3636, 3640, 3644};

struct QdtbBoutWork {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::uint32_t sequenceActive, state, unused24, reserved28;
    std::uint32_t motions[16];
    EventReadCharactersWork *characters;
    std::byte reserved68[12];
    std::uint32_t battleFrames;
    std::int32_t countdown;
    std::uint8_t fadeStarted, closingFadeStarted, retryCount, reserved7f;
};
struct QdtbAuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t state;
    std::byte reserved12[6];
};
struct QdtbAuthorLoopWork {
    QdtbAuthorWork *parent;
    std::uint16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(QdtbBoutWork) == 0x80 && offsetof(QdtbBoutWork, characters) == 0x60 && offsetof(QdtbBoutWork, battleFrames) == 0x74);
static_assert(sizeof(QdtbAuthorWork) == 0x18 && offsetof(QdtbAuthorWork, state) == 0x10 && sizeof(QdtbAuthorLoopWork) == 0x10);
void QdtbInit();
void QdtbMain(std::uintptr_t);
extern const ScriptAreaDefinition qdtbScriptDefinition;
} // namespace shenmue::s2
