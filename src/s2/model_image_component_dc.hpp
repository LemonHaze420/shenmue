#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#include "s2/model_image_dc.hpp"

#define SHENMUE_IMAGE_COMPONENT_LAYOUT_NAME_(line) SHENMUE_IMAGE_COMPONENTLayoutCheck##line
#define SHENMUE_IMAGE_COMPONENT_LAYOUT_NAME(line) SHENMUE_IMAGE_COMPONENT_LAYOUT_NAME_(line)
#define SHENMUE_IMAGE_COMPONENT_LAYOUT(condition) typedef char SHENMUE_IMAGE_COMPONENT_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_IMAGE_COMPONENT_CONSTANT static const
#define SHENMUE_IMAGE_COMPONENT_STRINGS static const char *const
#define SHENMUE_IMAGE_COMPONENT_INITIALIZER =
#define SHENMUE_IMAGE_COMPONENT_NULL 0
namespace shenmue {
namespace s2 {

#define MEMORY_HIMG (::shenmue::shen_name(SHEN_NAME4('H', 'I', 'M', 'G')))
enum ModelImageEntryFlag { MIEF_InUse = 1, MIEF_SelectionDirty = 2 };
SHENMUE_IMAGE_COMPONENT_STRINGS modelImageAnimationTokens[] SHENMUE_IMAGE_COMPONENT_INITIALIZER{"Num", "Loop", "Wait", "Kind", SHENMUE_IMAGE_COMPONENT_NULL};
SHENMUE_IMAGE_COMPONENT_STRINGS modelRigNames[] SHENMUE_IMAGE_COMPONENT_INITIALIZER{
    "AKI", "BIG", "CHA", "DEB", "KKO", "GEN", "IWA", "JIJ", "KOD", "KOK", "OHS", "REN", "SYP", "YKI", "YUA", "SIN", "CAT", "YAG", "DOG", "PNW", "KCT"};
struct ImageModuleComponent {
    HLib::Task *manager;
    ModelManagerEntry **entries;
    std::uint8_t count, selected, previous, forceFirst;
    std::uint32_t retainedAnimationState;
    std::uint32_t remainingFrames;
    std::int16_t sourceType;
    std::uint16_t retainedSourceState;
    std::uint32_t animationGroup, animationFlags;
    float frameStep, frameAccumulator;
};
SHENMUE_IMAGE_COMPONENT_LAYOUT(sizeof(ImageModuleComponent) == 0x28);
SHENMUE_IMAGE_COMPONENT_LAYOUT(offsetof(ImageModuleComponent, retainedAnimationState) == 0x0c);
SHENMUE_IMAGE_COMPONENT_LAYOUT(offsetof(ImageModuleComponent, remainingFrames) == 0x10);
SHENMUE_IMAGE_COMPONENT_LAYOUT(offsetof(ImageModuleComponent, sourceType) == 0x14);
SHENMUE_IMAGE_COMPONENT_LAYOUT(offsetof(ImageModuleComponent, animationFlags) == 0x1c);
SHENMUE_IMAGE_COMPONENT_LAYOUT(offsetof(ImageModuleComponent, frameStep) == 0x20);

ModelManagerEntry *AllocateImageEntry(HLib::Task *manager, const char *name);
void ReleaseImageEntry(HLib::Task *manager, ModelManagerEntry *entry);
void SetImageDistance(HLib::Task *manager, ModelManagerEntry *entry, std::uint32_t priority);
void UpdateImageDistance(HLib::Task *manager, ModelManagerEntry *entry, Vector3 *position);
MT7Model *SelectImageEntrySource(HLib::Task *manager, const ModelManagerEntry *entry);
MT7Node *ResolveImageEntry(HLib::Task *manager, ModelManagerEntry *entry);
void SetImageEntrySelection(ModelManagerEntry *entry, std::uint32_t child, std::int32_t group, std::uint32_t variant);
void InitializeImageModule(ImageModuleComponent *image);
void ReleaseImageModule(ImageModuleComponent *image);
MT7Node *ResolveImageModule(ImageModuleComponent *image, Vector3 *position);
void ConfigureImageModuleVariants(ImageModuleComponent *image, HLib::Task *manager, const char *name, std::int32_t count);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ConfigureImageModuleAnimation(ImageModuleComponent *image, std::int32_t mode, std::int32_t remaining, float step);
#else
void ConfigureImageModuleAnimation(ImageModuleComponent *image, std::uint8_t mode, std::int32_t remaining, float step);
#endif
void ModelManagerNullsub0();
void ModelManagerNullsub1();
void SetImageModuleSelection(ImageModuleComponent *image, std::int32_t selected);
void SetImageModuleEntrySelection(ImageModuleComponent *image, std::int32_t entry, std::uint32_t child, std::int32_t group, std::uint32_t variant);
void SetupImageModule(Character *character, HLib::Task *manager, const char *name);
void SetCharacterImageSelection(Character *character, std::int32_t selected);
void SetCharacterImageEntrySelection(Character *character, std::int32_t entry, std::uint32_t child, std::int32_t group, std::uint32_t variant);
void ImageModuleUpdate(Character *character, void *data);
void ImageModuleCleanup(Character *character, void *data);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AssignImageModuleParameters(Character *character, std::int32_t mode, std::int32_t remaining, float step, std::uint32_t group);
#else
void AssignImageModuleParameters(Character *character, std::uint8_t mode, std::int32_t remaining, float step, std::uint32_t group);
#endif
void SetupImageModuleVariants(Character *character, HLib::Task *manager, const char *name, std::int32_t count);
void PublishImageModule(Character *character, ImageModuleComponent *image);
MT7Model *GetImageModuleSource(const Character *character);
const void *GetImageModuleSelectedTransform(const Character *character);
std::int32_t GetCharacterImageSourceType(const Character *character);
void ClearCharacterFacialModel(Character *character);
std::uint32_t CountImageModelFrames(const MT7Node *node);
std::uint32_t AdvanceImageModelFrame(MT7Node *root, std::uint32_t flags);
void DrawImageCharacterCore(Character *character);
void imgmDrawModelNode(const MT7Node *node, float alpha);
bool mtwkSetSequenceFlags(Character *character);
MT7Node *CloneImageSelection(MT7Model *source, std::uint32_t child, std::int32_t group, std::uint32_t variant);
void ReleaseMT7ModelNodeTreeClone(MT7Node *root);
void GraftFacialModelNode(void *face, MT7Node *root);
Vector3 *camCtrlGetNextCamPos(Vector3 *position);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_IMAGE_COMPONENT_LAYOUT_NAME_
#undef SHENMUE_IMAGE_COMPONENT_LAYOUT_NAME
#undef SHENMUE_IMAGE_COMPONENT_LAYOUT
#undef SHENMUE_IMAGE_COMPONENT_CONSTANT
#undef SHENMUE_IMAGE_COMPONENT_STRINGS
#undef SHENMUE_IMAGE_COMPONENT_INITIALIZER
#undef SHENMUE_IMAGE_COMPONENT_NULL

#endif
