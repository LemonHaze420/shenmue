#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/texture_dc.hpp"
#else
#include "s2/pak.hpp"
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC
std::int32_t textureMotionFlag = 0;
#else
namespace {
std::int32_t textureMotionFlag = 0;
}
#endif

// @verified - pd
void TXMO_Reader(const void *txmo, std::int32_t entryPoint) {
    if (!txmo) {
        textureMotionFlag = 0;
        return;
    }
    const std::int32_t *entry = reinterpret_cast<const std::int32_t *>(static_cast<const std::byte *>(txmo) + 8);
    if (*entry != -1 && *entry != entryPoint) {
        do {
            if (*++entry == -1) {
                textureMotionFlag = 0;
                return;
            }
        } while (*entry != entryPoint);
    }
    textureMotionFlag = 1;
}

// @verified - pd
std::int32_t texGetMotionFlag() {
    return textureMotionFlag;
}

// @verified - pd
void texSetMotionFlag(std::int32_t enabled) {
    textureMotionFlag = enabled;
}

} // namespace s2
} // namespace shenmue
