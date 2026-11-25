#pragma once

#include "s2/character_control_dc.hpp"
#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

#define MEMORY_BLUR_WORK SHEN_NAME4('B', 'L', 'W', 'K')
#define MEMORY_BLUR_MODEL_FILE SHEN_NAME4('B', 'L', 'O', 'B')
#define MODEL_BLUR_PACKAGE_ENTRY SHEN_NAME4('C', 'H', 'R', 'M')
static const std::uint32_t blurWorkEntryCount = 20, blurBoneTransformCount = 15;
struct BlurWorkEntry {
    std::uint8_t flags, state;
    std::int8_t mode;
    std::uint8_t boneId;
    Matrix4x4 boneTransforms[blurBoneTransformCount];
    Matrix4x4 rootTransform;
    float time;
    Vector3 position;
};
struct FigurePose {
    std::int8_t mode, startTime, motionScale, directionFlag;
    std::uint8_t flags;
    std::int8_t entryIndex;
    std::int16_t counter;
    char filename[16];
    MT7Node *targetRootInstance;
    MT7Model *targetModelReader;
    union {
        CharacterUpdateCallback previousModelCallback;
        void (*previousCoreCallback)(Character *);
    };
    BlurWorkEntry *blurWork;
    float maximumTime, time;
    Character *character;
    void *modelFile;
};
typedef char Layout_blur_figure_pose_dc_0[(sizeof(BlurWorkEntry) == 0x414) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_1[(offsetof(BlurWorkEntry, boneTransforms) == 4) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_2[(offsetof(BlurWorkEntry, rootTransform) == 0x3c4) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_3[(offsetof(BlurWorkEntry, time) == 0x404) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_4[(sizeof(FigurePose) == 0x38) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_5[(offsetof(FigurePose, targetRootInstance) == 0x18) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_6[(offsetof(FigurePose, targetModelReader) == 0x1c) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_7[(offsetof(FigurePose, previousModelCallback) == 0x20) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_8[(offsetof(FigurePose, blurWork) == 0x24) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_9[(offsetof(FigurePose, maximumTime) == 0x28) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_10[(offsetof(FigurePose, time) == 0x2c) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_11[(offsetof(FigurePose, character) == 0x30) ? 1 : -1];
typedef char Layout_blur_figure_pose_dc_12[(offsetof(FigurePose, modelFile) == 0x34) ? 1 : -1];
struct CharacterTableRecord;
void AllocateBLWK(FigurePose *figure);
void figpUpdateBlurModelState(FigurePose *figure);
bool LerpBlurMatrix(Matrix4x4 *destination, float alpha, const Matrix4x4 *first, const Matrix4x4 *second);
void DrawBlurObjectEntries(FigurePose *figure);
void DrawBlurObjectInterpolations(FigurePose *figure);
void AttachObjectBlur(Character *character, const CharacterTableRecord *record);
void BlendMatrices(const MT7Node *model, float alpha);
void FreeBlurWork(Character *character, void *payload);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadBlurModel(FigurePose *figure, const char *filename);
void AssignBlurCallbacks(Character *character);
void BlurWork(Character *character);
void DrawBlurFigurePoseEntries(FigurePose *figure);
void DrawBlurFigurePoseInterpolations(FigurePose *figure);
void SetBlurModel(Character *character, const char *filename, float time);
void LoadBlurModelForCharacter(shen_name character, const char *filename, float time);
void figpLoadBlurModelFor(shen_name character, const char *filename);
#endif

} } // namespace shenmue::s2

#endif
