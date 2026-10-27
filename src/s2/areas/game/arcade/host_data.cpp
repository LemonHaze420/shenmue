#include "s1/d3t/settings.hpp"
#include "s1/input.hpp"
#include "s1/modules/arcade.hpp"

#include <array>
#include <cstdint>

// S2-private storage consumed by the unchanged CPU/audio and Hang-On DLL ABIs.
namespace shenmue::s1 {

InputStateStorage g_inputState{.slots = {
                                   {.enabled = 1,
                                       .unknownMask = 0x06fe,
                                       .held = 0,
                                       .previousHeld = 0,
                                       .pressedPriority = 0,
                                       .repeatedPresses = 0,
                                       .triggers = 0x8080,
                                       .rightStick = 0x8080,
                                       .leftStick = 1,
                                       .xinputReserved = 0},
                                   {},
                                   {},
                                   {},
                                   {},
                               }};
D3TApplicationSettings g_applicationSettings{.windowRect = {.left = 1, .top = 58, .right = 1025, .bottom = 826}};
HangOnLibrary g_hangOnLibrary{};
std::int32_t g_harrierExitedEmulator{};

} // namespace shenmue::s1
