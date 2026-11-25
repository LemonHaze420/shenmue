#pragma once

#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct Character;
#define FLAG_WIND_COMPONENT SHEN_NAME4('F', 'L', 'A', 'G')

struct FlagWindVertex {
    Vector3 *position;
    Vector3 originalPosition;
};
typedef char FlagWindDcLayout275[(sizeof(FlagWindVertex) == 0x10) ? 1 : -1];

struct FlagWindModel {
    std::int32_t type;
    void *mesh;
    std::int32_t vertexCount;
    FlagWindVertex *vertices;
    std::int32_t phase;
    std::int32_t speed;
    float maximumX, minimumX, maximumY, minimumY, maximumZ, minimumZ;
};
typedef char FlagWindDcLayout566[(sizeof(FlagWindModel) == 0x30) ? 1 : -1];
typedef char FlagWindDcLayout612[(offsetof(FlagWindModel, vertices) == 0x0c) ? 1 : -1];
typedef char FlagWindDcLayout670[(offsetof(FlagWindModel, maximumX) == 0x18) ? 1 : -1];

struct FlagWindObject {
    MT7Node *root;
    std::uint32_t modelCount;
    FlagWindModel *models;
    FlagWindVertex *vertices;
};
typedef char FlagWindDcLayout862[(sizeof(FlagWindObject) == 0x10) ? 1 : -1];

struct FlagAnimationPlacement {
    Vector3 translation;
    std::int32_t rotationZ, rotationY;
    std::uint32_t retained14;
    std::int32_t phase, speed;
};
struct FlagAnimationObject {
    MT7Node *root;
    FlagWindVertex *vertices;
    std::int32_t vertexCount;
    FlagAnimationPlacement *placements;
    std::int32_t placementCount;
};
typedef char FlagAnimationPlacementSizeCheck[sizeof(FlagAnimationPlacement) == 0x20 ? 1 : -1];
typedef char FlagAnimationObjectSizeCheck[sizeof(FlagAnimationObject) == 0x14 ? 1 : -1];
void AssignAnimatedFlag(shen_name name, FlagAnimationPlacement *placements, std::int32_t count);

extern Vector3 flagWindMaximum, flagWindMinimum;
std::uint32_t CountFlagWindModels(MT7Node *node);
std::int32_t CollectFlagWindModels(MT7Node *node, FlagWindModel *models, std::int32_t &index);
std::int32_t PopulateFlagWindObject(MT7Node *root, FlagWindObject &object, std::uint32_t modelCount);
FlagWindObject *AllocateFlagObject(MT7Node *root);
float EvaluateWindWave(std::int32_t angle);
void ApplyWaveformTransformSinewave(FlagWindVertex *vertices, std::uint32_t count, std::int32_t phase, float amplitude);
void ApplyCurvedWindOscillation(FlagWindModel *model, float amplitude);
void VerticalWaveOffsetZBend(FlagWindModel *model, float amplitude);
void SineStackYCurve(FlagWindModel *model, float scale);
void DualAxisZone(FlagWindModel *model, float scale);
void UpdateWindForModel(FlagWindObject *object);
void UpdateWindForChara(Character *character, void *payload);
void FreeWindObject(FlagWindObject *object);
void FreeWindModel(Character *character, void *payload);
void AttachFlagWindComponent(Character *character);

} // namespace s2
} // namespace shenmue

#endif
