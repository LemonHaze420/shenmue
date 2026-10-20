#pragma once
#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct Character;
struct ImageModuleWork;
struct MdlmImageSlot;
struct PackModel;

enum ImageCycleMode : std::uint8_t { ICM_Stopped = 0, ICM_Forward = 1, ICM_PingPong = 2 };
enum ImageCycleFlag : std::int32_t { ICF_Reverse = 1, ICF_Finished = 0x80 };
#pragma pack(push, 1)


struct ImageModuleWork {
    HLib::Task *modelLoaderTask;
    MdlmImageSlot **images;
    std::uint8_t imageCount;
    std::uint8_t requestedImage;
    std::uint8_t displayedImage;
    std::uint8_t cycleMode;
    std::int8_t cycleFlags;
    std::byte reserved15[3];
    std::int32_t repeatCount;
    std::uint8_t frameDelay;
    std::uint8_t frameCounter;
    std::uint16_t basePoseId;
};
#pragma pack(pop)

static_assert(sizeof(ImageModuleWork) == 0x20);
static_assert(offsetof(ImageModuleWork, imageCount) == 0x10);
static_assert(offsetof(ImageModuleWork, basePoseId) == 0x1e);
void *IMGM_GetCurrentImageModel(Character *character);

ImageModuleWork *IMGM_GetWork(Character *character);
std::uint32_t IMGM_GetImageName(Character *character);
ImageModuleWork *IMGM_Alloc(Character *character);
bool IMGM_Create(Character *character, const char *name, std::uint32_t imageCount = 0);
void *imgmCreateHandlerForWithPos(Character *character, ImageModuleWork *image, Vector3 *position);
void IMGM_ReleaseImages(ImageModuleWork *image);
void IMGM_ReleaseForNullModelScript(Character *character);
void IMGM_SetupCharacterModel(Character *character, PackModel *model, std::uint32_t imageCount = 0);
void IMGM_SetupForScript(Character *character, const char *modelName, std::uint32_t imageCount);
void IMGM_UnloadCharacterModel(Character *character);
} // namespace shenmue::s1
