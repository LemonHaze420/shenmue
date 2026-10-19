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
