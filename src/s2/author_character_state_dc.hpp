#pragma once

#include "s2/author_controller_dc.hpp"
#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const std::size_t AUTHOR_SAVED_CHARACTER_COUNT = 32;
typedef std::uint32_t AuthSavedCharacterFlag;
enum {
    AuthRestoreFigurePose = 1,
    AuthRestoreObjectXForms = 2,
    AuthRestoreLowQualityHands = 4,
    AuthRestoreRightHand = 8,
    AuthRestoreLeftHand = 0x10
};
struct AuthSavedCharacter {
    shen_name subject;
    std::uint32_t retained04;
    std::uint32_t flags;
    const std::int32_t *finalMotionArguments;
    std::int32_t finalMotionFrame;
    Vector4 reflectorValues;
    std::int8_t reflectorSector;
    std::int8_t unknown40;
    std::int8_t motionSnapshot;
    std::int8_t storedShadowY;
    std::int32_t drawControl;
    std::uint32_t controlFlags;
    std::int16_t pluralState;
    std::int8_t displayEnabled;
    std::byte retained33;
};
typedef char Layout_author_character_state_dc_0[(sizeof(AuthSavedCharacter) == 0x34) ? 1 : -1];
typedef char Layout_author_character_state_dc_1[(offsetof(AuthSavedCharacter, flags) == 8) ? 1 : -1];
typedef char Layout_author_character_state_dc_2[(offsetof(AuthSavedCharacter, reflectorValues) == 0x14) ? 1 : -1];
typedef char Layout_author_character_state_dc_3[(offsetof(AuthSavedCharacter, reflectorSector) == 0x24) ? 1 : -1];
typedef char Layout_author_character_state_dc_4[(offsetof(AuthSavedCharacter, unknown40) == 0x25) ? 1 : -1];
typedef char Layout_author_character_state_dc_5[(offsetof(AuthSavedCharacter, motionSnapshot) == 0x26) ? 1 : -1];
typedef char Layout_author_character_state_dc_6[(offsetof(AuthSavedCharacter, storedShadowY) == 0x27) ? 1 : -1];
typedef char Layout_author_character_state_dc_7[(offsetof(AuthSavedCharacter, drawControl) == 0x28) ? 1 : -1];
typedef char Layout_author_character_state_dc_8[(offsetof(AuthSavedCharacter, controlFlags) == 0x2c) ? 1 : -1];
typedef char Layout_author_character_state_dc_9[(offsetof(AuthSavedCharacter, pluralState) == 0x30) ? 1 : -1];
typedef char Layout_author_character_state_dc_10[(offsetof(AuthSavedCharacter, displayEnabled) == 0x32) ? 1 : -1];
typedef char Layout_author_character_state_dc_11[(offsetof(AuthSavedCharacter, finalMotionArguments) == 0x0c) ? 1 : -1];
typedef char Layout_author_character_state_dc_12[(offsetof(AuthSavedCharacter, finalMotionFrame) == 0x10) ? 1 : -1];
extern std::int32_t g_authorSavedCharacterCount;
extern AuthSavedCharacter g_authorSavedCharacters[AUTHOR_SAVED_CHARACTER_COUNT];
AuthSavedCharacter *RecordAuthCharacterState(shen_name subject, std::uint32_t value);
AuthSavedCharacter *FindAuthCharacterState(shen_name subject);
AuthSavedCharacter *SetAuthSavedCharacterFlags(shen_name subject, std::uint32_t flags);
AuthSavedCharacter *ClearAuthSavedCharacterFlags(shen_name subject, std::uint32_t flags);
void RestoreAuthCharacterStates();
void CaptureAuthCharacterState(Character *character, shen_name subject);
std::int32_t PrepareAuthorHands(Character *character, shen_name subject);

} } // namespace shenmue::s2

#endif
