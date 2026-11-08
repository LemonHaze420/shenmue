#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define OMAKE_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define OMAKE_AREA SHEN_NAME4('O', 'M', 'A', 'K')
#define OMAKE_NO_AREA SHEN_NAME4(' ', ' ', ' ', ' ')
#define OMAKE_UNKNOWN SHEN_NAME4('?', '?', '?', '_')
#define OMAKE_SPACE SHEN_NAME4('S', 'P', 'C', '_')
#define OMAKE_MENU_ROW(row) SHEN_NAME4('M', 'N', 'U', (unsigned char)(row))

typedef struct OmakeWork OmakeWork;
typedef struct OmakeCommand {
    shen_name area;
    int entry;
} OmakeCommand;
typedef struct OmakeEntry {
    void (*callback)(OmakeWork *work, const OmakeCommand *command);
    OmakeCommand command;
    unsigned int requiredFlags;
    int photoTexture, colour;
    const char *name, *description, *descriptionEnd;
    int helpTopic;
} OmakeEntry;
typedef struct OmakeCategory {
    int icon;
    OmakeEntry *entries;
} OmakeCategory;

typedef struct OmakeSelection {
    int category, selected[3], scroll[3];
} OmakeSelection;
typedef struct OmakeSpriteTexCoords {
    unsigned int reserved;
    float uv[4];
    unsigned int reservedTail;
} OmakeSpriteTexCoords;
typedef struct OmakeSpritePacket {
    unsigned int flags;
    float corners[8];
    unsigned int colour, reservedColour[3], secondaryColour, reservedUv[2];
    OmakeSpriteTexCoords tex;
} OmakeSpritePacket;
typedef struct OmakeSprite {
    shen_name name;
    OmakeSpritePacket packet;
    int texture;
    float depth;
} OmakeSprite;
typedef struct OmakeTextTexture {
    shen_name name;
    int texture;
} OmakeTextTexture;
struct OmakeWork {
    int state, delay, animationFrames, animationRemaining, animationDirection, refreshDescription;
    OmakeSelection *selection;
    int textures[512];
    OmakeSprite sprites[512];
    int debugSprite;
    OmakeTextTexture textTextures[128];
    HLibTask *currentTask, *mainTask, *drawTask;
    int entry;
    void *spriteFile;
};
typedef struct OmakeDrawWork {
    OmakeWork *owner;
} OmakeDrawWork;
typedef struct OmakeDeleteTextureWork {
    OmakeWork *owner;
    int texture, frames, reserved0c;
} OmakeDeleteTextureWork;
typedef struct OmakeModuleCallWork {
    OmakeWork *owner;
    int frames;
    const OmakeCommand *command;
} OmakeModuleCallWork;
typedef struct OmakeFightLoadWork {
    OmakeWork *owner;
    int delay;
} OmakeFightLoadWork;

typedef struct OmakeDateTime {
    signed char year, month, day;
    unsigned char week;
    signed char hour, minute, second;
    short reserved08;
} OmakeDateTime;

extern int g_storyStep;
extern int g_hotLoadModule;
extern const OmakeEntry omakeArcadeEntries[13], omakeVideoEntries[13], omakeMinigameEntries[13], omakeFightEntries[13];
extern OmakeCommand omakeColdBootCommand;
extern OmakeCategory omakeCategories[4];

void RunOmake(void);
void BootOmake(void);
void InitializeOmakeList(OmakeEntry *destination, const OmakeEntry *source);
void InitializeOmakeSprites(OmakeWork *work);
void DeleteOmakeSprites(OmakeWork *work);
void CreateOmakeSprite(OmakeWork *work, shen_name name);
void DeleteOmakeSprite(OmakeWork *work, shen_name name);
OmakeSprite *FindOmakeSprite(OmakeWork *work, shen_name name);
void OmakeSpriteDrawCallback(void *data);
void DrawOmakeSprites(OmakeWork *work);
void InitializeOmakeTexts(OmakeWork *work);
void DeleteOmakeTexts(OmakeWork *work);
void OmakeTextDeleteCallback(void *data);
void CreateOmakeText(OmakeWork *work, shen_name name, const char *text, unsigned short colour);
void QueueOmakeTextDelete(OmakeWork *work, shen_name name);
int HasOmakeText(OmakeWork *work, shen_name name);
int FindOmakeText(OmakeWork *work, shen_name name);
void OmakeModuleCallCallback(void *data);
void QueueOmakeModuleCall(OmakeWork *work, const OmakeCommand *command);
void QueueOmakeVideo(OmakeWork *work, const OmakeCommand *command);
void QueueOmakeSceneChange(OmakeWork *work, const OmakeCommand *command);
void GiveDefaultPlayerFlags(OmakeWork *work);
void OmakeFightLoadCallback(void *data);
void QueueOmakeFight(OmakeWork *work, const OmakeCommand *command);
void CreateOmakeMenuSprites(OmakeWork *work);
void OmakeMainCallback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_sprite.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name OMAKE_AREA{SHEN_NAME(OMAK)}, OMAKE_EVENT{SHEN_NAME(EVNT)}, OMAKE_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name OMAKE_UNKNOWN{0x5f3f3f3fU}, OMAKE_SPACE{SHEN_NAME(SPC_)};
inline constexpr shen_name OMAKE_CATEGORY_ICONS[]{SHEN_NAME(IC13), SHEN_NAME(IC12), SHEN_NAME(IC14)};
inline constexpr shen_name OMAKE_MENU_BACKGROUNDS[]{SHEN_NAME(BGM0), SHEN_NAME(BGM1), SHEN_NAME(BGM2), SHEN_NAME(BGM3)};
inline constexpr shen_name OMAKE_MENU_ROWS[]{SHEN_NAME(MNU0), SHEN_NAME(MNU1), SHEN_NAME(MNU2), SHEN_NAME(MNU3)};
inline constexpr shen_name OMAKE_INSTRUCTIONS[]{SHEN_NAME(INS1), SHEN_NAME(INS2)};
inline constexpr shen_name OMAKE_PHOTO{SHEN_NAME(PHOT)}, OMAKE_LEFT_ARROW{SHEN_NAME(BG10)}, OMAKE_RIGHT_ARROW{SHEN_NAME(BG16)}, OMAKE_SCROLL{SHEN_NAME(SCBA)};
struct OmakeWork;
struct OmakeCommand {
    shen_name area;
    std::int32_t entry;
};
struct OmakeEntry {
    void (*callback)(OmakeWork *, const OmakeCommand *);
    OmakeCommand command;
    std::uint32_t requiredFlags;
    std::int32_t photoTexture, colour, reserved1c;
    const char *name;
    const char *description;
    const char *descriptionEnd;
    std::uint64_t helpTopic;
};
struct OmakeSelection {
    std::int32_t category, reserved04;
    std::int64_t selected[3], scroll[3];
};
#pragma pack(push, 4)
struct OmakeSprite {
    shen_name name;
    LegacySpritePacket packet;
    std::int32_t texture;
    float depth;
};
struct OmakeTextTexture {
    shen_name name;
    std::int32_t texture;
};
#pragma pack(pop)
struct OmakeWork {
    std::int32_t state, delay, animationFrames, animationRemaining, animationDirection, refreshDescription;
    OmakeSelection *selection;
    std::int32_t textures[512];
    OmakeSprite sprites[512];
    std::int32_t debugSprite;
    OmakeTextTexture textTextures[128];
    std::uint32_t reservedd424;
    HLib::Task *currentTask;
    HLib::Task *mainTask;
    HLib::Task *drawTask;
    std::int32_t entry, reservedd444;
    void *loadedSprite;
};
struct OmakeModuleCallWork {
    OmakeWork *owner;
    std::int32_t frames, reserved0c;
    const OmakeCommand *command;
};
struct OmakeDeleteTextureWork {
    OmakeWork *owner;
    std::int32_t texture, frames;
    std::uint64_t reserved10;
};
struct OmakeCategory {
    std::int32_t icon, reserved04;
    OmakeEntry *entries;
};
static_assert(sizeof(OmakeCommand) == 8 && sizeof(OmakeEntry) == 0x40 && offsetof(OmakeEntry, name) == 0x20);
static_assert(sizeof(OmakeSelection) == 0x38 && offsetof(OmakeSelection, selected) == 8 && offsetof(OmakeSelection, scroll) == 0x20);
static_assert(sizeof(OmakeSprite) == 0x64 && offsetof(OmakeSprite, texture) == 0x5c && offsetof(OmakeSprite, depth) == 0x60);
static_assert(sizeof(OmakeWork) == 0xd450 && offsetof(OmakeWork, textures) == 0x20 && offsetof(OmakeWork, sprites) == 0x820);
static_assert(offsetof(OmakeWork, debugSprite) == 0xd020 && offsetof(OmakeWork, textTextures) == 0xd024 && offsetof(OmakeWork, currentTask) == 0xd428);
static_assert(sizeof(OmakeModuleCallWork) == 0x18 && sizeof(OmakeDeleteTextureWork) == 0x18 && sizeof(OmakeCategory) == 0x10);
extern OmakeEntry omakeGameList[13], omakeMinigameList[13], omakeFightList[13];
extern OmakeSelection omakeSelection;
extern OmakeCategory omakeCategories[4];
extern std::int32_t omakeSelectedCategory, omakeSelectedEntry, omakeBackRequested, omakeUnlockOverride;
extern std::int32_t omakeDrawEnabled, omakeVideoCount;
extern float omakeSpriteAnimation;
void InitializeOmake();
void RunOmake(std::uintptr_t argument);
void BootOmake();
void OmakeMainCallback(void *data);
void OmakeModuleCallCallback(void *data);
void QueueOmakeModuleCall(OmakeWork *work, const OmakeCommand *command);
void QueueOmakeSceneChange(OmakeWork *work, const OmakeCommand *command);
void QueueOmakeVideo(OmakeWork *work, const OmakeCommand *command);
void OmakeShowMainMenu(OmakeWork *work, const OmakeCommand *command);
void InitializeOmakeList(OmakeEntry *destination, const OmakeEntry *source);
void DeleteOmakeSprites(OmakeWork *work);
OmakeSprite *FindOmakeSprite(OmakeWork *work, shen_name name);
std::int32_t CreateOmakeSprite(OmakeWork *work, shen_name name);
void OmakeSpriteDrawCallback(void *data);
void DrawOmakeSprites(OmakeWork *work);
void OmakeSpriteDrawNullsub(const LegacySpritePacket *packet, float depth);
bool HasOmakeText(OmakeWork *work, shen_name name);
std::int32_t FindOmakeText(OmakeWork *work, shen_name name);
std::int32_t CreateOmakeText(OmakeWork *work, shen_name name, const char *text, std::uint16_t colour);
void QueueOmakeTextDelete(OmakeWork *work, shen_name name);
void OmakeTextDeleteCallback(void *data);
void CreateOmakeMenuSprites(OmakeWork *work);
void RestoreOmakeArcadeSelection(OmakeWork *work);
void SelectOmakeEntry(std::int32_t category, std::uint32_t entry);
bool IsOmakeSelectionPending();
void RequestOmakeBack();
extern const ScriptAreaDefinition omakScriptDefinition;
} // namespace shenmue::s2::areas
#endif
