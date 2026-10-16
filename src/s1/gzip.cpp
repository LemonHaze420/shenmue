#include "s1/file_loader.hpp"
#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <zlib.h>

namespace shenmue::s1 {

void decompress(void *source, void *destination, std::int64_t, std::int64_t, std::uint32_t decompressedSize, std::int64_t, std::uint32_t compressedSize) {
    z_stream stream{};
    stream.next_in = static_cast<Bytef *>(source);
    stream.avail_in = compressedSize;
    stream.next_out = static_cast<Bytef *>(destination);
    stream.avail_out = decompressedSize;

    if (inflateInit2(&stream, MAX_WBITS + 16) != Z_OK) {
        return;
    }
    inflate(&stream, Z_FINISH);
    inflateEnd(&stream);
}

void *LoadGzippedSpriteIntoHeapMem(const char *filename, std::int32_t allocationToken, std::uint32_t *decompressedSize) {
    if (decompressedSize)
        *decompressedSize = 0;

    const std::uint32_t compressedSize = static_cast<std::uint32_t>(shenOpenFile("sprite", filename));
    void *const compressed = HLib_AllocMemBlock(compressedSize, allocationToken);
    if (!compressed)
        return nullptr;

    constexpr std::int32_t kScratchToken = SHEN_NAME(GZIP);
    void *const scratch = HLib_AllocMemBlock(0x4000, kScratchToken);
    if (!scratch) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(compressed), 0);
        return nullptr;
    }

    if (shenLoadFile("sprite", filename, compressed, static_cast<std::int32_t>(compressedSize))) {
        std::int32_t outputSize = 0;
        std::memcpy(&outputSize, static_cast<const std::byte *>(compressed) + compressedSize - sizeof(outputSize), sizeof(outputSize));
        if (outputSize != 0) {
            void *const output = HLib_AllocHeapMemBlock(outputSize, allocationToken);
            if (output) {
                decompress(compressed, output, 0, 0, static_cast<std::uint32_t>(outputSize), 0, compressedSize);
                if (decompressedSize)
                    *decompressedSize = static_cast<std::uint32_t>(outputSize);
                HLib_FreeTaskData(static_cast<HLib::TaskData *>(compressed), 0);
                HLib_FreeTaskData(static_cast<HLib::TaskData *>(scratch), 0);
                return output;
            }
        }
    }

    HLib_FreeTaskData(static_cast<HLib::TaskData *>(scratch), 0);
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(compressed), 0);
    return nullptr;
}

} // namespace shenmue::s1
