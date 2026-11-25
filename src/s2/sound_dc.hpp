#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/sample_program.hpp"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue {
namespace s2 {
#define AICA_DRIVER_STORAGE SHEN_NAME4('S', 'D', 'R', 'V')
#define SOUND_FILE_STORAGE SHEN_NAME4('S', 'N', 'D', '1')
#define SOUND_STREAM_STORAGE SHEN_NAME4('S', 'N', 'D', 'D')
#define SOUND_SYSTEM_CONFIG_STORAGE SHEN_NAME4('B', 'R', 'O', 'M')
#define SOUND_MATERIAL_TABLE SHEN_NAME4('S', 'G', 'T', 'S')
#define SOUND_AMBIENT_TABLE SHEN_NAME4('A', 'M', 'B', 'S')
#define SOUND_SECONDARY_SAMPLE_TABLE SHEN_NAME4('A', 'M', 'S', 'S')
struct SoundBankMetadataHeader {
    shen_name name;
    std::uint32_t size;
    std::uint32_t unknown08;
    std::uint8_t dataOffset;
    std::uint8_t recordSize;
    std::uint16_t count;
    std::byte retained10[12];
    std::uint32_t command;
};
typedef char Layout_sound_dc_0[(sizeof(SoundBankMetadataHeader) == 0x20) ? 1 : -1];
typedef char Layout_sound_dc_1[(offsetof(SoundBankMetadataHeader, dataOffset) == 0x0c) ? 1 : -1];
typedef char Layout_sound_dc_2[(offsetof(SoundBankMetadataHeader, command) == 0x1c) ? 1 : -1];
static const std::uint32_t kInitialAicaBankAllocations[8] = {0x30000, 0x400, 0x400, 0x400, 0x400, 0x9b000, 0x400, 0x400};
static const std::uint32_t kEmptyAicaBank[173] = {
    SHEN_NAME4('D', 'T', 'P', 'K'),
    0x11111111U,
    0x2b4U,
    0x0U,
    0x3ffff09U,
    0x15500U,
    0x200000U,
    0x0U,
    0x60U,
    0x198U,
    0x230U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x80001U,
    0x30U,
    0x7f00U,
    0xffU,
    0xf83U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0xff007f07U,
    0xffU,
    0x80U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x180U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x280U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x380U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x480U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x580U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x680U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x780U,
    0x7f007f00U,
    0x7f0c00U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x40000U,
    0x40000U,
    0x0U,
    0x0U,
    0x10U,
    0x0U,
    0x7f000000U,
    0x0U,
    0x7f02U,
    0x0U,
    0x7f0fU,
    0x0U,
    0x0U,
    0x0U,
    0x20200U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x1007fU,
    0xc800000U,
    0xf7ffffU,
    0x1f001fU,
    0x7fd0000U,
    0x7fd07fdU,
    0x27fdU,
    0x800000U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0xd600U,
    0x0U,
    0x0U,
    0x0U,
    0x0U,
    0x150e0700U,
    0x322b241cU,
    0x4b474039U,
    0x5a56524fU,
    0x6965615eU,
    0x7874706dU,
    0x827f7d7aU,
    0x8c898784U,
    0x9693918eU,
    0x9f9d9b98U,
    0xa9a7a4a2U,
    0xb1b0aeadU,
    0xb7b5b4b3U,
    0xbdbbbab8U,
    0xc2c1bfbeU,
    0xc8c6c5c4U,
    0xcdcccbc9U,
    0xd3d2d1cfU,
    0xd8d6d5d4U,
    0xdcdbdad9U,
    0xe0dfdeddU,
    0xe5e4e3e1U,
    0xe9e8e7e6U,
    0xeeedebeaU,
    0xf0efefeeU,
    0xf2f1f1f0U,
    0xf4f4f3f3U,
    0xf7f6f5f5U,
    0xf9f8f8f7U,
    0xfbfbfaf9U,
    0xfdfdfcfcU,
    0xfefefefeU,
};
extern char g_soundBankNames[8][16];
extern std::uint32_t g_soundBankAllocations[8];
extern std::int8_t g_soundBankLoadStates[8];
extern SampleProgramState g_samplePrograms[2], g_secondarySamplePrograms[2];
extern MaterialSoundBank g_materialSoundBanks[8];
extern SampleProgramPool g_sampleProgramStorage[4];
extern std::int32_t g_soundPauseDepth;
extern std::int32_t g_soundListenerUsesCamera;
extern std::int32_t g_pendingSoundSceneIndex;
std::int32_t LoadAicaDriver();
void InitAica();
void FinishAica();
void ResetSampleProgram(SampleProgramState *program);
void sndClearSceneState();
void SetSoundBankAllocation(std::int32_t bank, std::uint32_t bytes);
std::int32_t GetSystemAudioOutputMode();
void SetSystemAudioOutputMode(std::int32_t mode);
void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
void LoadSoundBankData(const char *directory, const char *filename, std::uint32_t bank);
bool LoadWholeSoundBank(const char *directory, const char *filename, std::int32_t bank);
bool LoadStreamedSoundBank(const char *directory, const char *filename, std::int32_t bank, std::uint32_t bufferBytes);
const std::uint32_t *LoadSoundBankMetadata(const std::uint32_t *data, std::int32_t bank);
void ReleaseMaterialSoundBank(std::int32_t bank);
void ReleaseSampleProgramBank(std::int32_t bank);
} // namespace s2
} // namespace shenmue
#endif
