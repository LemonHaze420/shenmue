#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "s2/character_dc.hpp"
#include "s2/hlib.hpp"

#define SHENMUE_CHARACTER_TABLE_LAYOUT_NAME_(line) ShenmueCharacterTableLayoutLayoutCheck##line
#define SHENMUE_CHARACTER_TABLE_LAYOUT_NAME(line) SHENMUE_CHARACTER_TABLE_LAYOUT_NAME_(line)
#define SHENMUE_CHARACTER_TABLE_LAYOUT(condition) typedef char SHENMUE_CHARACTER_TABLE_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]

namespace shenmue { namespace s2 {

typedef std::uint32_t CharacterTableArgumentType;
enum { CharacterTableArgumentType_Float = 1, CharacterTableArgumentType_Integer = 2, CharacterTableArgumentType_String = 3, CharacterTableArgumentType_Records = 4  };
typedef std::int32_t CharacterTableCommand;
enum { CharacterTableCommand_Character = 0, CharacterTableCommand_DefImage = 1, CharacterTableCommand_LocalPos = 2, CharacterTableCommand_PreLoad = 3  };
typedef std::int32_t CharacterNodeCommand;
enum { CharacterNodeCommand_Position = 0, CharacterNodeCommand_Angle = 1, CharacterNodeCommand_Layer = 2, CharacterNodeCommand_Local = 3, CharacterNodeCommand_HasColi = 4  };
enum CharacterComponentCommand {
    CC_Player, CC_Image, CC_Entry, CC_Flags, CC_Coli, CC_Human, CC_Object, CC_Party,
    CC_Face, CC_Hand, CC_Item, CC_Range, CC_ColiOff, CC_LineWalk, CC_ForkLift, CC_MotClip,
    CC_Shadow, CC_ShadowOff, CC_Book, CC_KeysRing, CC_Fight, CC_Group, CC_MotOnly, CC_CasetteDeck,
    CC_CasetteTape, CC_Handcuffs, CC_Cloth, CC_TinyMot, CC_Tel, CC_Step, CC_Walk3D, CC_CommonMotionWork,
    CC_FootMark, CC_RefBoard, CC_Osage, CC_Knife, CC_TParty, CC_Trans, CC_Vehicle, CC_Bone,
    CC_DummyChara, CC_Bicycle, CC_Patrol, CC_Vender, CC_LowQuality, CC_DesignOK, CC_Clock, CC_ProjectionTrans,
    CC_TailLamp, CC_TinyGame, CC_Calendar, CC_CutMvShadow, CC_ObjBlur, CC_ForceTrans
};
enum CharacterNodeFields {
    CN_Position = 1, CN_Angle = 2, CN_Layer = 4, CN_Local = 8, CN_Collision = 0x10
};
union CharacterTableValue {
    std::uint32_t bits;
    std::int32_t integer;
    float floating;
    const char *string;
};
struct CharacterTableArgument {
    CharacterTableArgumentType type;
    const std::uint32_t *source;
    CharacterTableValue value;
};
struct CharacterTableRecord {
    const std::uint32_t *source;
    std::int32_t tokenIndex;
    std::int32_t count;
    CharacterTableArgument arguments[10];
};
struct CharacterEntryOverride {
    std::uint32_t fields;
    CharacterTableRecord flags, image, group;
    CharacterNode node;
};
SHENMUE_CHARACTER_TABLE_LAYOUT(sizeof(CharacterTableValue) == 4);
SHENMUE_CHARACTER_TABLE_LAYOUT(sizeof(CharacterTableArgument) == 12);
SHENMUE_CHARACTER_TABLE_LAYOUT(offsetof(CharacterTableArgument, source) == 4);
SHENMUE_CHARACTER_TABLE_LAYOUT(offsetof(CharacterTableArgument, value) == 8);
SHENMUE_CHARACTER_TABLE_LAYOUT(sizeof(CharacterTableRecord) == 0x84);
SHENMUE_CHARACTER_TABLE_LAYOUT(offsetof(CharacterTableRecord, arguments) == 12);
SHENMUE_CHARACTER_TABLE_LAYOUT(sizeof(CharacterEntryOverride) == 0x1b4);
SHENMUE_CHARACTER_TABLE_LAYOUT(offsetof(CharacterEntryOverride, flags) == 4);
SHENMUE_CHARACTER_TABLE_LAYOUT(offsetof(CharacterEntryOverride, image) == 0x88);
SHENMUE_CHARACTER_TABLE_LAYOUT(offsetof(CharacterEntryOverride, group) == 0x10c);
SHENMUE_CHARACTER_TABLE_LAYOUT(offsetof(CharacterEntryOverride, node) == 0x190);
#define SHENMUE_CHARACTER_TOKENS static const char *const
#define SHENMUE_CHARACTER_TOKEN_INIT =
#define SHENMUE_CHARACTER_TOKEN_NULL 0

SHENMUE_CHARACTER_TOKENS characterTableTokens[] SHENMUE_CHARACTER_TOKEN_INIT {"Character", "DefImage", "LocalPos", "PreLoad", SHENMUE_CHARACTER_TOKEN_NULL};
SHENMUE_CHARACTER_TOKENS characterNodeTokens[] SHENMUE_CHARACTER_TOKEN_INIT {"Position", "Angle", "Layer", "Local", "HasColi", SHENMUE_CHARACTER_TOKEN_NULL};
SHENMUE_CHARACTER_TOKENS characterEntryTokens[] SHENMUE_CHARACTER_TOKEN_INIT {"Flags", "Image", "Group", SHENMUE_CHARACTER_TOKEN_NULL};
SHENMUE_CHARACTER_TOKENS characterComponentTokens[] SHENMUE_CHARACTER_TOKEN_INIT {
    "Player", "Image", "Entry", "Flags", "Coli", "Human", "Object", "Party", "Face", "Hand", "Item",
    "Range", "ColiOff", "LineWalk", "ForkLift", "MotClip", "Shadow", "ShadowOff", "Book", "KeysRing", "Fight", "Group", "MotOnly", "CasetteDeck", "CasetteTape",
    "Handcuffs", "Cloth", "TinyMot", "Tel", "Step", "Walk3D", "CommonMotionWork", "FootMark", "RefBoard", "Osage", "Knife", "TParty", "Trans", "Vehicle",
    "Bone", "DummyChara", "Bicycle", "Patrol", "Vender", "LowQuality", "DesignOK", "Clock", "ProjectionTrans", "TailLamp", "TinyGame", "Calendar",
    "CutMvShadow", "ObjBlur", "ForceTrans", SHENMUE_CHARACTER_TOKEN_NULL};

#undef SHENMUE_CHARACTER_TOKENS
#undef SHENMUE_CHARACTER_TOKEN_INIT
#undef SHENMUE_CHARACTER_TOKEN_NULL

struct CharacterNode;
std::int32_t CompareCharacterTableTokens(const char *left, const char *right);
const std::uint32_t *ParseCharacterTableRecord(CharacterTableRecord *record, const char *const *tokens, const std::uint32_t *source);
const std::uint32_t *ApplyCharacterTableNodeRecord(CharacterNode *node, std::uint32_t *fields, const std::uint32_t *source);
void LoadCharacters(const void *data, std::int32_t entry, shen_name allocationName);
void BeginModelImageFileReads();
void FinishModelImageFileReads();
void AssignModelImageDefinition(HLib::Task *task, const CharacterTableArgument *arguments, std::int32_t count, std::int32_t entry);
void InitializeScenarioCharacter(const CharacterTableArgument *arguments, std::int32_t count, std::int32_t entry, shen_name allocationName);
struct Character;
void ApplyCharacterDefinition(Character *character, const std::uint32_t *source, std::int32_t size, std::int32_t entry, shen_name allocationName);
void ApplyCharacterTableControl(const CharacterTableArgument *arguments);
bool ApplyCharacterEntryOverride(CharacterEntryOverride *state, const CharacterTableArgument *arguments, std::int32_t entry);
void ApplyCharacterTableFlags(Character *character, const CharacterTableArgument *arguments, std::int32_t count);
void ApplyMapEventCharacterFlag(Character *character, const CharacterTableArgument *arguments, std::int32_t count);
void AllocateMapEventComponent(Character *character);
void ParseMapEv(Character *character, std::int32_t enabled);

} } // namespace shenmue::s2
#undef SHENMUE_CHARACTER_TABLE_LAYOUT_NAME
#undef SHENMUE_CHARACTER_TABLE_LAYOUT_NAME_
#undef SHENMUE_CHARACTER_TABLE_LAYOUT

#endif
