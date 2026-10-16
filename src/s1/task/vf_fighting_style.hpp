#pragma once
#include "s1/task/vf_internal.hpp"

namespace shenmue::s1::vf_detail {

#pragma pack(push, 1)
struct VFFightScriptParameters {
    std::uint32_t reserved00;
    std::int32_t ryoCharacterId;
    std::int32_t opponentCharacterId;
    std::uint16_t ryoCommandId;
    std::uint16_t reserved0e;
    std::uint16_t opponentCommandId;
    std::uint16_t reserved12;
    std::uint32_t resultOffset;
};

struct VFFightScriptResult {
    std::uint8_t phase;
    std::uint8_t commandState;
    std::uint8_t resultCode;
    std::uint8_t inputIndex;
    std::uint8_t reserved04;
    std::uint8_t classificationLength;
    std::uint8_t classificationReferenceCount;
    std::uint8_t frameCounter;
    std::uint32_t classificationMask;
};
#pragma pack(pop)

static_assert(sizeof(VFFightScriptParameters) == 0x18);

static_assert(sizeof(VFFightScriptResult) == 0x0c);

struct FightCommandRow {
    FightCommandOpcode opcode{};
    std::uint32_t argument{};
};

struct FightCommandProgram {
    std::array<FightCommandRow, 10> rows{};
    std::uint8_t count{};
    std::uint8_t stageId{};
    std::uint8_t battleId{};
};

constexpr FightCommandRow FightRow(FightCommandOpcode opcode, std::uint32_t argument = 0) {
    return {opcode, argument};
}

constexpr FightCommandProgram kDummyFightCommand{{
                                                     FightRow(FightCommandOpcode::Window, 8),
                                                     FightRow(FightCommandOpcode::InputMask, 128),
                                                     FightRow(FightCommandOpcode::SelectAltA, 208),
                                                     FightRow(FightCommandOpcode::End, 0x3fa66666),
                                                 },
    4, 0, 0};

constexpr std::array<FightCommandProgram, 17> kFightCommandPrograms{{
    {{{FightRow(FightCommandOpcode::Window, 8), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::SelectAltA, 215),
         FightRow(FightCommandOpcode::End, 0xffffffff)}},
        4, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 8), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::Window, 3),
         FightRow(FightCommandOpcode::InputMask, 1), FightRow(FightCommandOpcode::ForceEnd, 1), FightRow(FightCommandOpcode::SelectAltA, 1546),
         FightRow(FightCommandOpcode::End, 0x22180b04)}},
        7, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 8), FightRow(FightCommandOpcode::InputMask, 128), FightRow(FightCommandOpcode::SelectAltA, 208),
         FightRow(FightCommandOpcode::End, 0)}},
        4, 3, 0},
    {{{FightRow(FightCommandOpcode::Window, 8), FightRow(FightCommandOpcode::InputMask, 129), FightRow(FightCommandOpcode::SelectAltA, 1178),
         FightRow(FightCommandOpcode::End, 0x447a0000)}},
        4, 3, 0},
    {{{FightRow(FightCommandOpcode::Window, 12), FightRow(FightCommandOpcode::InputMask, 129), FightRow(FightCommandOpcode::SelectContinue, 1178),
         FightRow(FightCommandOpcode::InputMask, 2), FightRow(FightCommandOpcode::SelectAltA, 1177), FightRow(FightCommandOpcode::End, 0x3fa66666)}},
        6, 3, 0},
    kDummyFightCommand,
    kDummyFightCommand,
    {{{FightRow(FightCommandOpcode::Window, 12), FightRow(FightCommandOpcode::InputMask, 129), FightRow(FightCommandOpcode::SelectContinue, 1178),
         FightRow(FightCommandOpcode::InputMask, 2), FightRow(FightCommandOpcode::SelectAltA, 1177), FightRow(FightCommandOpcode::End, 0x5a595800)}},
        6, 3, 2},
    {{{FightRow(FightCommandOpcode::Window, 15), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::InputMask, 64),
         FightRow(FightCommandOpcode::SelectAltA, 215), FightRow(FightCommandOpcode::End, 0x030f1817)}},
        5, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 15), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::InputMask, 64),
         FightRow(FightCommandOpcode::Window, 15), FightRow(FightCommandOpcode::InputMask, 2), FightRow(FightCommandOpcode::SelectAltA, 1213),
         FightRow(FightCommandOpcode::End, 0x14090a04)}},
        7, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 15), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::InputMask, 64),
         FightRow(FightCommandOpcode::Window, 15), FightRow(FightCommandOpcode::InputMask, 2), FightRow(FightCommandOpcode::SelectContinue, 1213),
         FightRow(FightCommandOpcode::Window, 10), FightRow(FightCommandOpcode::InputMask, 2), FightRow(FightCommandOpcode::SelectAltA, 1214),
         FightRow(FightCommandOpcode::End, 0x2aaa)}},
        10, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 8), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::SelectAltA, 215),
         FightRow(FightCommandOpcode::End, 0x3f99999a)}},
        4, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 4), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::Window, 3),
         FightRow(FightCommandOpcode::InputMask, 3), FightRow(FightCommandOpcode::ForceEnd, 1), FightRow(FightCommandOpcode::SelectAltA, 1208),
         FightRow(FightCommandOpcode::End, 0x0e091301)}},
        7, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 4), FightRow(FightCommandOpcode::InputMask, 128), FightRow(FightCommandOpcode::SelectAltA, 208),
         FightRow(FightCommandOpcode::End, 1)}},
        4, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 4), FightRow(FightCommandOpcode::InputMask, 128), FightRow(FightCommandOpcode::InputMask, 2),
         FightRow(FightCommandOpcode::SelectAltA, 1288), FightRow(FightCommandOpcode::End, 0x200e1201)}},
        5, 0, 0},
    {{{FightRow(FightCommandOpcode::Window, 8), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::SelectAltA, 215),
         FightRow(FightCommandOpcode::End, 0x40466666)}},
        4, 1, 0},
    {{{FightRow(FightCommandOpcode::Window, 4), FightRow(FightCommandOpcode::InputMask, 64), FightRow(FightCommandOpcode::Window, 3),
         FightRow(FightCommandOpcode::InputMask, 12), FightRow(FightCommandOpcode::SelectAltA, 1539), FightRow(FightCommandOpcode::End, 0)}},
        6, 1, 0},
}};

struct FightInputHistoryRow {
    std::uint16_t held{};
    std::uint16_t triggered{};
    std::uint16_t reserved04{};
    std::uint16_t reserved06{};
};


struct FightCommandMatcher {
    std::uint16_t commandId{};
    std::uint8_t matchedOrDisabled{};
    std::uint8_t bootstrapMaskHigh{};
    std::uint8_t initCountdown{};
    std::uint8_t retryCountdown{};
    std::uint8_t inputWindowLength{};
    std::int8_t inputWindowEnd{};
    std::uint8_t resultKind{};
    std::uint8_t matcherPhase{};
    std::uint8_t resultCode{};
    std::uint8_t classifyCode{};
    std::uint32_t classifyMask{};
    std::uint8_t classifyLength{};
    std::uint8_t classifyReferenceCount{};
    std::uint8_t programInputIndex{};
    std::uint8_t ended{};
    std::uint8_t inputOpcodeCount{};
    std::uint8_t pc{};
    std::uint8_t historyCursor{};
    std::uint8_t hasDirectionHistory{};
    std::uint32_t pendingInput{};
    std::uint32_t previousInput{};
    std::uint32_t savedInput{};
    std::uint16_t lastDirection{};
    std::uint16_t pad26{};
    std::uint32_t selectedAnimation{};
    std::uint8_t matchedInputCount{};
    std::uint8_t pad2d[3]{};
};

static_assert(sizeof(FightCommandMatcher) == 0x30);

static_assert(offsetof(FightCommandMatcher, classifyMask) == 0x0c);

static_assert(offsetof(FightCommandMatcher, pendingInput) == 0x18);

static_assert(offsetof(FightCommandMatcher, lastDirection) == 0x24);

static_assert(offsetof(FightCommandMatcher, selectedAnimation) == 0x28);

static_assert(offsetof(FightCommandMatcher, matchedInputCount) == 0x2c);

struct FightCommandClassification {
    std::uint8_t code{};
    std::uint32_t mask{};
    std::uint8_t length{};
    std::uint8_t referenceCount{};
};


struct VFAnimDecisionBits {
    std::uint8_t one;
    std::uint8_t two;
    std::uint8_t three;
    std::uint8_t four;
    std::uint8_t extra;
    std::uint8_t mirror;
};

static_assert(sizeof(VFAnimDecisionBits) == 6);

} // namespace shenmue::s1::vf_detail
