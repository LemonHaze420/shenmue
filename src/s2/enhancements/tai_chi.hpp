#pragma once
#include "shenmue.h"

#if SHENMUE_IS_PC && SHENMUE_IS_S2 && SHENMUE_ENHANCEMENT_S2_TAI_CHI
namespace shenmue::s2 {
struct Character;
void UpdateTaiChi(Character *player);
bool TaiChiHintActive();
bool IsPractisingTaiChi();
void ResetTaiChi();
}
#endif
