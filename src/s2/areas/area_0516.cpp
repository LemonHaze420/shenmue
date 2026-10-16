#include "s2/areas/area_0516.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
// @verified - p
void Update0516Waves(void *) {
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(12);
    SetTextureAnimationUV(0.0f, 0.09000000357627869f);
    ApplyTextureAnimationResource(13);
}
} // namespace shenmue::s2::areas
