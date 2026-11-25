#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/model_dc.hpp"
#include "s2/date_time.hpp"

namespace shenmue {
namespace s2 {

struct LensFlareState {
    Vector3 direction;
    float diskScale, jitterScale;
    std::int32_t enabled, explicitDirection;
};
typedef char Layout_lens_flare_dc_0[(sizeof(LensFlareState) == 0x1c) ? 1 : -1];
typedef char Layout_lens_flare_dc_1[(offsetof(LensFlareState, diskScale) == 0x0c) ? 1 : -1];
typedef char Layout_lens_flare_dc_2[(offsetof(LensFlareState, enabled) == 0x14) ? 1 : -1];
#define LENS_FLARE_STORAGE SHEN_NAME4('L', 'E', 'N', 'S')
extern MT7Model *g_lensFlareModels[14];
extern LensFlareState g_lensFlareState;
void LoadLensflare();
void shenLoadSetLensflare();
void SetSunDirectionVector(const Vector3 *direction);
void UpdateSunLensFlare();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct Character;
extern const Vector3 *g_sunOcclusionOrigin, *g_sunOcclusionEnd;
void UpdateSunFilmFade(float facing, float falloff);
std::int32_t TestSunOcclusionCharacter(Character *character, void *context);
std::int32_t IsSunOccluded(const Vector3 *origin, const Vector3 *direction);
void DrawLensFlareBillboard(std::int32_t index, float depth, float scale, float alpha);
void RotateLensFlareAlignment(const Vector3 *direction);
#endif
void DrawSunLensFlare(void *parameter);
void UpdateSunForDateTime(const DateTime &date, float latitude, Vector3 &direction, float scale);

} // namespace s2
} // namespace shenmue

#endif
