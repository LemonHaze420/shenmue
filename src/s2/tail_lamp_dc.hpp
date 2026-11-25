#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_components_dc.hpp"
#include "s2/model_dc.hpp"

namespace shenmue { namespace s2 {

#define MODEL_COMPONENT_LAMP SHEN_NAME4('L', 'A', 'M', 'P')
static const std::uint32_t tailLampLightSlot = 13, tailLampNoLightSlot = 17;
static const Vector4 tailLampColour= {SHEN_FIELD(x) 1.0f, SHEN_FIELD(y) 1.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 0.0f};

struct TailLampWork {
    MT7Model *model;
    std::uint32_t lightSlot, active;
    std::int32_t lifetime;
};
struct TailLampDrawWork {
    Matrix4x4 transform;
    MT7Node *root;
    float alpha, fadeDelta;
};
typedef char Layout_tail_lamp_dc_0[(sizeof(TailLampWork) == 0x10) ? 1 : -1];
typedef char Layout_tail_lamp_dc_1[(offsetof(TailLampWork, lightSlot) == 4) ? 1 : -1];
typedef char Layout_tail_lamp_dc_2[(offsetof(TailLampWork, active) == 8) ? 1 : -1];
typedef char Layout_tail_lamp_dc_3[(offsetof(TailLampWork, lifetime) == 0x0c) ? 1 : -1];
typedef char Layout_tail_lamp_dc_4[(sizeof(TailLampDrawWork) == 0x4c) ? 1 : -1];
typedef char Layout_tail_lamp_dc_5[(offsetof(TailLampDrawWork, root) == 0x40) ? 1 : -1];
typedef char Layout_tail_lamp_dc_6[(offsetof(TailLampDrawWork, alpha) == 0x44) ? 1 : -1];
typedef char Layout_tail_lamp_dc_7[(offsetof(TailLampDrawWork, fadeDelta) == 0x48) ? 1 : -1];

void TailLampTaskCallback(void *payload);
void UpdateTailLamp(Character *character, void *payload);
void CleanupTailLamp(Character *character, void *payload);

} } // namespace shenmue::s2

#endif
