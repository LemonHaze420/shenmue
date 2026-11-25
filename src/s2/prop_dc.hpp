#pragma once

#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct PropReaderSlot {
    std::int16_t previous;
    std::int16_t next;
    MT7Model *reader;
};
typedef char Layout_prop_dc_0[(sizeof(PropReaderSlot) == 8) ? 1 : -1];
typedef char Layout_prop_dc_1[(offsetof(PropReaderSlot, reader) == 4) ? 1 : -1];

struct MT7MeshPrimitive {
    std::uint32_t flags;
    std::byte reserved04[0x48];
    std::uint32_t dataBytes;
};
typedef char Layout_prop_dc_2[(sizeof(MT7MeshPrimitive) == 0x50) ? 1 : -1];

extern PropReaderSlot g_propReaderSlots[17];
struct FlagWindObject;
struct PropState {
    MT7Node *root;
    std::uint32_t lightingTraversalResult;
    Vector3 position;
    FlagWindObject *flagWind;
};
extern PropState g_propState;
extern Matrix4x4 g_propDrawBase;
extern void (*g_propDrawMesh)(void *mesh);
static const float kPropObjectRadius = 3.0f, kPropDepthRadius = 2.0f;

void ResetPropReaderSlots();
MT7Node *GetPropRoot();
void GetPropVector4(Vector3 *position);
std::uint32_t GetPropLightingTraversalResult();
void DrawPropObjectHierarchy(const MT7Node *node);
void DrawPropObjectCallback(void *data);
std::int32_t RegisterPropReader(MT7Model *reader);
void RemovePropReader(std::uint32_t index);
std::int32_t RegisterPackagePropReader(MT7Model *reader);
void RemovePackagePropReader(std::uint32_t index);
void UpdatePropRoot();
void SetupModelForImageModule(MT7Node *root);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetPropVector3(const Vector3 *position);
void SetDisplayFlagForProp(shen_name name, std::uint32_t enabled);
std::uint32_t HasPropCharacterZeroUpperSignature(shen_name name);
#endif

} // namespace s2
} // namespace shenmue

#endif
