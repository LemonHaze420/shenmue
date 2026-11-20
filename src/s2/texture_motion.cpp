#include "shenmue.h"

#include "s2/pak.hpp"

namespace shenmue { namespace s2 {

namespace {
std::int32_t textureMotionFlag = 0;
}

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

} } // namespace shenmue::s2
