#include "common/aica_adpcm.hpp"

#include <algorithm>
#include <array>

namespace shenmue {

// Yamaha ADPCM conversion adapted from Shenmue-Mods/DTPKUtil (MIT):
// https://github.com/Shenmue-Mods/DTPKUtil
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
void DecodeYamahaAdpcm(const std::uint8_t *input, std::size_t bytes, std::vector<std::uint8_t> &output) {
    constexpr std::array<int, 16> difference{1, 3, 5, 7, 9, 11, 13, 15, -1, -3, -5, -7, -9, -11, -13, -15};
    constexpr std::array<int, 8> scale{0xe6, 0xe6, 0xe6, 0xe6, 0x133, 0x199, 0x200, 0x266};
    output.resize(bytes * 4);
    int sample = 0;
    int quantizer = 0x7f;
    for (std::size_t index = 0; index < bytes * 2; ++index) {
        const unsigned nibble = (input[index / 2] >> ((index & 1U) * 4U)) & 15U;
        const int delta = quantizer * difference[nibble];
        sample = std::clamp(sample + ((delta + static_cast<int>(static_cast<std::uint32_t>(delta) >> 29)) >> 3), -32768, 32767);
        output[2 * index] = static_cast<std::uint8_t>(sample);
        output[2 * index + 1] = static_cast<std::uint8_t>(sample >> 8);
        quantizer = std::clamp((quantizer * scale[nibble & 7U]) >> 8, 0x7f, 0x6000);
        sample = sample * 254 / 256;
    }
}

} // namespace shenmue
