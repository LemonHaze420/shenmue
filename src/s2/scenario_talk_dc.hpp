#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define SHENMUE_SCNT_CONSTANT static const
#define SHENMUE_SCNT_JOIN_(a, b) a##b
#define SHENMUE_SCNT_JOIN(a, b) SHENMUE_SCNT_JOIN_(a, b)
#define SHENMUE_SCNT_CHECK(condition) typedef char SHENMUE_SCNT_JOIN(LayoutCheckscenario_talk_dc, __LINE__)[(condition) ? 1 : -1]

namespace shenmue {
namespace s2 {

#define CHARACTER_FREE_TALK (::shenmue::shen_name(SHEN_NAME4('F', 'R', 'T', 'K')))
#define SCENARIO_TALK_AREA (::shenmue::shen_name(SHEN_NAME4('T', 'A', 'L', 'K')))
struct ScenarioFreeTalkCharacter;
struct MotionWork;
struct PadControlEntry;
struct FreeTalkRegistration {
    shen_name name;
    std::int16_t savedStateIndex;
    std::int8_t availability, searchState;
    std::uint32_t meshIndex;
    float distance;
    Character *character;
    MotionWork *motionWork;
    ScenarioFreeTalkCharacter *component;
    FreeTalkRegistration *next, *previous;
};
struct FreeTalkCharacterState {
    union {
        std::byte retainedState[8];
        std::uint16_t resumeOffsets[4];
    };
    std::uint16_t flags;
    std::uint8_t completedCount, activationCount;
};
struct FreeTalkLocalState {
    FreeTalkCharacterState saved;
    FreeTalkRegistration *registration;
    const std::byte *defaultRoot, *alternateRoot;
    std::uint32_t requestedRoot;
    std::uint32_t retainedRootState;
};
struct ScenarioFreeTalkCharacter {
    shen_name name;
    std::uint8_t retained04;
    std::int8_t executionState;
    std::uint8_t retained06, commandState;
    std::uint32_t retained08;
    std::int32_t selection;
    std::uint8_t participantCount, approachCount, locationCount, keyValueCount;
    std::int16_t savedStateIndex;
    union {
        std::byte retained16[2];
        std::int16_t dialogueFlagId;
    };
    std::uint32_t flags;
    std::int16_t promptTimeout;
    std::int16_t requestedSection;
    const std::byte *participants, *locations, *vectors;
    union {
        std::uint32_t retained2c;
        const Vector3 *selectedApproachPoint;
    };
    const char *voicePath;
    const std::byte *keyValues, *script, *lineTable;
    union {
        std::byte retained40[12];
        struct {
            const std::byte *firstLine, *dialoguePreparation;
            std::uint32_t playerSpeaking;
        } dialogue;
    };
    const std::byte *source;
    union {
        std::byte retained50[32];
        struct {
            const std::byte *cursor, *savedCursor, *returnCursor, *pendingCursor;
            std::uint8_t directionMode, retained61;
            std::int16_t minimumYaw, maximumYaw, startFrames, followFrames, endFrames, actionFrames, retained6e;
        } runtime;
    };
    FreeTalkCharacterState *state;
    FreeTalkLocalState localState;
};
SHENMUE_SCNT_CHECK(sizeof(FreeTalkRegistration) == 0x24);
SHENMUE_SCNT_CHECK(offsetof(FreeTalkRegistration, character) == 0x10);
SHENMUE_SCNT_CHECK(offsetof(FreeTalkRegistration, component) == 0x18);
SHENMUE_SCNT_CHECK(offsetof(FreeTalkRegistration, next) == 0x1c);
SHENMUE_SCNT_CHECK(sizeof(FreeTalkCharacterState) == 0x0c);
SHENMUE_SCNT_CHECK(offsetof(FreeTalkCharacterState, flags) == 8);
SHENMUE_SCNT_CHECK(offsetof(FreeTalkCharacterState, completedCount) == 10);
SHENMUE_SCNT_CHECK(offsetof(FreeTalkCharacterState, activationCount) == 11);
SHENMUE_SCNT_CHECK(sizeof(FreeTalkLocalState) == 0x20);
SHENMUE_SCNT_CHECK(offsetof(FreeTalkLocalState, registration) == 0x0c);
SHENMUE_SCNT_CHECK(sizeof(ScenarioFreeTalkCharacter) == 0x94);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, executionState) == 5);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, participantCount) == 0x10);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, savedStateIndex) == 0x14);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, flags) == 0x18);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, participants) == 0x20);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, voicePath) == 0x30);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, script) == 0x38);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, source) == 0x4c);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, state) == 0x70);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, localState) == 0x74);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, localState) + offsetof(FreeTalkLocalState, defaultRoot) == 0x84);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, localState) + offsetof(FreeTalkLocalState, alternateRoot) == 0x88);
SHENMUE_SCNT_CHECK(offsetof(ScenarioFreeTalkCharacter, localState) + offsetof(FreeTalkLocalState, requestedRoot) == 0x8c);

extern FreeTalkRegistration *g_freeTalkRegistrations, *g_freeTalkFreeRegistrations;
extern shen_name g_freeTalkVisibleCharacters[8];
extern std::int32_t g_freeTalkVisibleCharacterCount;
extern ScenarioFreeTalkCharacter g_residentFreeTalkCharacters[16];
extern FreeTalkCharacterState g_savedFreeTalkStates[512];
void ExpandFreeTalkRecord(const std::byte *source, ScenarioFreeTalkCharacter *record);
bool AttachScenarioFreeTalkCharacter(Character *character, const std::byte *source);
struct ScenarioEventContextPrefix {
    HLib::Task *scenarioTask;
    std::uint32_t flags;
    std::byte retained08[0x10];
    shen_name activeCharacter;
    union {
        std::byte retained1c[8];
        std::uint32_t previousCharacter;
    };
    Character *freeTalkCharacter;
    const std::byte *freeTalkData;
    PadControlEntry *padControlData, *inputState;
    std::int32_t residentCharacterCount;
};
SHENMUE_SCNT_CHECK(sizeof(ScenarioEventContextPrefix) == 0x38);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, scenarioTask) == 0);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, flags) == 4);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, freeTalkCharacter) == 0x24);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, freeTalkData) == 0x28);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, activeCharacter) == 0x18);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, padControlData) == 0x2c);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, inputState) == 0x30);
SHENMUE_SCNT_CHECK(offsetof(ScenarioEventContextPrefix, residentCharacterCount) == 0x34);
extern ScenarioEventContextPrefix *g_scenarioEventContext;
extern std::int32_t g_scenarioSubtitleHoldFrames;
ScenarioFreeTalkCharacter *FindScenarioFreeTalkCharacter(shen_name character);
void SetScenarioTalkCompletedCount(ScenarioFreeTalkCharacter *character, std::int32_t count);
void SetScenarioTalkActivationCount(ScenarioFreeTalkCharacter *character, std::int32_t count);
void FreeTalkCharacterUpdateNullsub(Character *character, void *payload);
void FreeTalkCharacterCleanup(Character *character, void *payload);
void AllocateScenarioFreeTalkCharacter(Character *character);
bool IsScenarioCharacterForcedVisible(shen_name character);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SHENMUE_SCNT_CONSTANT std::size_t kFreeTalkRegistrationCount = 64;
void InitializeFreeTalkRegistrations();
void PauseScenarioLineClock(std::int32_t paused);
void scenUpdateFreeTalkInteraction();
void scenUpdateCandidateAvailability();
void scenActivateCandidate(FreeTalkRegistration *candidate);
FreeTalkRegistration *srchEvaluateSearchTarget();
bool IsScenarioActiveRecordIdle();
bool CanActivateScenarioCandidate();
void EndScenarioConversation();
void scenExecuteRecord(ScenarioFreeTalkCharacter *record);
#endif

} // namespace s2
} // namespace shenmue

#undef SHENMUE_SCNT_CONSTANT
#undef SHENMUE_SCNT_JOIN_
#undef SHENMUE_SCNT_JOIN
#undef SHENMUE_SCNT_CHECK

#endif
