#include "s2/stream_audio_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/pak_dc.hpp"
#include <cri_adxf.h>
namespace shenmue {
namespace s2 {

// @verified - d
void ReleaseAudioPartition(AudioPartition *partition) {
    HLib::FreeTaskData(partition->handle);
    partition->id = -1;
}

// @verified - d
std::int32_t UnmountPakAfsPartition(PakAfsPartition *partition) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(partition);
    if (address >= reinterpret_cast<std::uintptr_t>(pakPartitions) && address <= reinterpret_cast<std::uintptr_t>(pakPartitions + 3)) {
        std::int32_t index = 0;
        while (index < 4 && partition != pakPartitions + index)
            ++index;
        AudioPartition *const afs = partition->afs;
        if (afs) {
            --partition->references;
            if (partition->references < 1)
                ReleaseAudioPartition(afs);
            ResetPakAfsPartition(partition);
            return partition->references;
        }
    }
    return 0;
}

} // namespace s2
} // namespace shenmue
#endif
