#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct Character;
struct MT7Model;
struct MT7Node;
struct MotionWork;

inline constexpr shen_name MODEL_FIGURE_POSE{SHEN_NAME(FIGP)};
inline constexpr shen_name MEMORY_BLUR_WORK{SHEN_NAME(BLWK)};
inline constexpr shen_name MEMORY_BLUR_MODEL_FILE{SHEN_NAME(BLOB)};
inline constexpr shen_name MODEL_BLUR_PACKAGE_ENTRY{SHEN_NAME(CHRM)};
inline constexpr shen_name CHARACTER_BLUR_RYO{SHEN_NAME(RYO_)};
inline constexpr std::size_t blurWorkEntryCount = 20;
inline constexpr std::size_t blurBoneTransformCount = 15;
inline constexpr std::int32_t blurBoneSelectors[blurBoneTransformCount] = {3, 5, 9, 12, 14, 16, 19, 21, 22, 23, 24, 26, 28, 29, 31};
inline constexpr std::uint16_t blurModelNodeIds[blurBoneTransformCount] = {1, 0xffbd, 5, 6, 0xffbf, 10, 11, 0xffbe, 14, 16, 17, 18, 21, 22, 23};
inline constexpr std::uint16_t blurBranchNodes[10][2] = {
    {0, 0}, {1, 0xffff}, {1, 14}, {14, 0xffff}, {5, 0xffff}, {10, 0xffff}, {16, 0xffff}, {21, 0xffff}, {1, 14}, {1, 14}};

struct BlurWorkEntry {
    std::uint8_t flags;
    std::uint8_t state;
    std::int8_t mode;
    std::uint8_t boneId;
    Matrix4x4 boneTransforms[blurBoneTransformCount];
    Matrix4x4 rootTransform;
    float time;
    Vector3 position;
};
static_assert(sizeof(BlurWorkEntry) == 0x414);
static_assert(offsetof(BlurWorkEntry, boneTransforms) == 4);
static_assert(offsetof(BlurWorkEntry, rootTransform) == 0x3c4);
static_assert(offsetof(BlurWorkEntry, time) == 0x404);
static_assert(offsetof(BlurWorkEntry, position) == 0x408);

struct FigurePose {
    std::int8_t mode;
    std::int8_t startTime;
    std::int8_t motionScale;
    std::int8_t directionFlag;
    std::uint8_t flags;
    std::int8_t entryIndex;
    std::int16_t counter;

    std::int8_t field08;
    std::byte reserved09[15];
    MT7Model *targetRootInstance;
    void *targetModelReader;
    union {
        void (*previousCoreCallback)(Character *);
        void (*previousModelCallback)(const MT7Model *, const MT7Node *, float);
    };
    BlurWorkEntry *blurWork;
    float maximumTime;
    float time;
    Character *character;
    // ObjBlur's separate BLOB owner; humanoid source bytes belong to the reader.
    void *modelFile;
};
static_assert(sizeof(FigurePose) == 0x50);
static_assert(alignof(FigurePose) == 8);
static_assert(offsetof(FigurePose, counter) == 6);
static_assert(offsetof(FigurePose, targetRootInstance) == 0x18);
static_assert(offsetof(FigurePose, targetModelReader) == 0x20);
static_assert(offsetof(FigurePose, previousCoreCallback) == 0x28);
static_assert(offsetof(FigurePose, blurWork) == 0x30);
static_assert(offsetof(FigurePose, maximumTime) == 0x38);
static_assert(offsetof(FigurePose, time) == 0x3c);
static_assert(offsetof(FigurePose, character) == 0x40);
static_assert(offsetof(FigurePose, modelFile) == 0x48);

FigurePose *mtSetupModelFigurePose(Character *character);
void AllocateBLWK(FigurePose *figure);
void figpUpdateBlurModelState(FigurePose *figure);
bool LerpBlurMatrix(Matrix4x4 *destination, float alpha, const Matrix4x4 *first, const Matrix4x4 *second);
void DrawBlurFigurePoseEntries(FigurePose *figure);
void DrawBlurFigurePoseInterpolations(FigurePose *figure);
void BlurWork(Character *character);
void BlendMatrices(const MT7Model *model, const MT7Node *node, float alpha);
void SetBlurModel(Character *character, const char *path, float time);
void mtFindBlurModel(Character *character, const char *path, const float *time);
void AttachObjectBlur(Character *character, std::uint32_t argumentCount, float time, const char *path);
void FreeBlurWork(Character *character, void *payload);
std::uintptr_t figpAssignMotScaleAndDirFor(Character *character, std::int8_t scale, std::int8_t direction);
std::uintptr_t figpToggleMotionworkFlag(Character *character, MotionWork *work);
FigurePose *figpAssignModeForCharacter(Character *character, std::int8_t mode);
std::uintptr_t EV_figpAssignFlagsFor(Character *character, std::uint8_t bitIndex, std::int8_t enabled);
void LoadBlurModelForCharacter(shen_name character, const char *path, float time);
std::uintptr_t figpLoadBlurModelFor(shen_name character, const char *path);
FigurePose *figpAssignModeFor(shen_name character, std::int8_t mode);
std::uintptr_t figpSetFlagModesFor(shen_name character, std::uint8_t bitIndex, std::int8_t enabled);
std::uintptr_t figpSetFlagsBlendForBlur(shen_name character, std::int8_t scale, std::int8_t direction);

} // namespace shenmue::s2
