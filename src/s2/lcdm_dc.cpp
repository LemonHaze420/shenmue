#include "s2/lcdm_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "s2/frame_dc.hpp"
#include "s2/pause.hpp"
#include "s2/scene_assets_dc.hpp"
#include <sg_lcd.h>
extern "C" const std::uint8_t *kdGetDevice(std::uint32_t port);

namespace shenmue {
namespace s2 {

std::uint32_t g_lcdPeripheralPort = 1;

// @verified - d
std::uint32_t GetLcdPeripheralPort() {
    return g_lcdPeripheralPort;
}

} // namespace s2
} // namespace shenmue

#endif
