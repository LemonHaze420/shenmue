#pragma once

#include "s2/sound_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {
void SetSoundListenerUsesCamera(std::int32_t enabled);

struct SoundDataHeader {
    shen_name name;
    char key[8];
    std::int32_t nextOffset;
};

struct SoundMapBank {
    std::uint32_t allocation;
    char filename[16];
};

struct SoundMapRecord {
    SoundDataHeader header;
    SoundMapBank banks[8];
};

struct SoundMapExtendedRecord {
    SoundMapRecord map;
    shen_name name;
    std::byte retainedB4[12];
    std::uint8_t fadeIn[8];
    std::uint8_t fadeOut[8];
};

struct MaterialSoundCommand {
    std::uint32_t command;
    std::uint32_t extendedCommand;
    std::int32_t bank;
    std::uint32_t variantCount;
};

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef std::uint32_t SoundCommandMode;
enum { SoundCommandMode_Raw = 0, SoundCommandMode_Channel = 1, SoundCommandMode_Bank = 2, SoundCommandMode_Environment = 3 };
struct SoundDriverCommand {
    MaterialSoundCommand material;
    std::int32_t value;
};
typedef char Layout_sound_table_dc_0[(sizeof(SoundDriverCommand) == 0x14) ? 1 : -1];
typedef char Layout_sound_table_dc_1[(offsetof(SoundDriverCommand, value) == 0x10) ? 1 : -1];
static const std::uint16_t soundEnvironmentKeys[74] = {1467, 1468, 2466, 2469, 2141, 2142, 2467, 2470, 2153, 2154, 2468, 2471, 2661, 2662, 2663, 2664, 2850,
    2851, 2852, 2853, 1469, 1470, 2472, 2475, 2143, 2144, 2473, 2476, 2155, 2156, 2474, 2477, 2863, 2862, 2858, 2859, 2856, 2857, 2861, 2860, 1471, 1472, 2478,
    2481, 2145, 2146, 2479, 2482, 2157, 2158, 2480, 2483, 1473, 1474, 2147, 2148, 2159, 2160, 1475, 1476, 2149, 2150, 2161, 2162, 1477, 1478, 2854, 2855, 2151,
    2152, 2163, 2164, 2848, 2849};
extern const std::uint16_t *soundEnvironmentKeyRows[8][6];
std::int32_t DispatchSoundCommand(SoundCommandMode mode, const SoundDriverCommand *record);
SoundDriverCommand *SetSoundDriverCommand(SoundDriverCommand *record, std::uint32_t command, std::int32_t value);
std::uint32_t GetSoundMaterialKey(std::uint32_t flags);
std::int32_t GetSoundEnvironment();
std::int32_t GetSoundBankFadeVolume(std::int32_t bank);
std::int32_t Publish3DSoundCommand(
    SoundDriverCommand *record, const Vector3 *position, float minimumDistance, float fadeDistance, std::int32_t volume, std::uint32_t persistent);
#endif

#define SOUND_MAP_EXTENSION SHEN_NAME4('S', 'm', 'p', 'X')
static const std::uint8_t kDefaultSoundBankFadeIn[8] = {0, 30, 30, 30, 30, 0, 30, 30};
static const std::uint8_t kDefaultSoundBankFadeOut[8] = {0, 30, 30, 30, 30, 30, 30, 30};
typedef char Layout_sound_table_dc_2[(sizeof(SoundMapExtendedRecord) == 0xd0) ? 1 : -1];
typedef char Layout_sound_table_dc_3[(offsetof(SoundMapExtendedRecord, name) == 0xb0) ? 1 : -1];
typedef char Layout_sound_table_dc_4[(offsetof(SoundMapExtendedRecord, fadeIn) == 0xc0) ? 1 : -1];
typedef char Layout_sound_table_dc_5[(offsetof(SoundMapExtendedRecord, fadeOut) == 0xc8) ? 1 : -1];
typedef char Layout_sound_table_dc_6[(sizeof(MaterialSoundCommand) == 0x10) ? 1 : -1];
typedef char Layout_sound_table_dc_7[(offsetof(MaterialSoundCommand, extendedCommand) == 4) ? 1 : -1];
typedef char Layout_sound_table_dc_8[(offsetof(MaterialSoundCommand, bank) == 8) ? 1 : -1];
typedef char Layout_sound_table_dc_9[(offsetof(MaterialSoundCommand, variantCount) == 0x0c) ? 1 : -1];
bool FindMaterialSoundCommand(std::uint32_t material, MaterialSoundCommand *output);
std::int32_t GetSoundBankPlaybackState(std::int32_t bank);

struct SoundSequenceHeader {
    SoundDataHeader header;
    std::byte retained10[4];
    std::int8_t dataOffset;
    std::uint8_t retained15;
    std::int8_t count;
    std::uint8_t retained17;
    std::byte retained18[8];
    char bankNames[8][8];
};

typedef char Layout_sound_table_dc_10[(sizeof(SoundDataHeader) == 0x10) ? 1 : -1];
typedef char Layout_sound_table_dc_11[(offsetof(SoundDataHeader, nextOffset) == 0x0c) ? 1 : -1];
typedef char Layout_sound_table_dc_12[(sizeof(SoundMapBank) == 0x14 && sizeof(SoundMapRecord) == 0xb0) ? 1 : -1];
typedef char Layout_sound_table_dc_13[(offsetof(SoundMapRecord, banks) == 0x10) ? 1 : -1];
typedef char Layout_sound_table_dc_14[(sizeof(SoundSequenceHeader) == 0x60) ? 1 : -1];
typedef char Layout_sound_table_dc_15[(offsetof(SoundSequenceHeader, dataOffset) == 0x14) ? 1 : -1];
typedef char Layout_sound_table_dc_16[(offsetof(SoundSequenceHeader, count) == 0x16) ? 1 : -1];
typedef char Layout_sound_table_dc_17[(offsetof(SoundSequenceHeader, bankNames) == 0x20) ? 1 : -1];
extern SoundEffectHandle g_materialSoundHandles[16];
extern std::uint32_t g_soundCallbackParity;
extern char g_activeSoundMapName[16], g_currentSoundSequenceName[16];
extern std::int32_t g_currentSoundSceneIndex;
extern SoundBankFade g_soundBankFades[8];
extern char g_savedSoundMapName[16], g_savedSoundSequenceName[16];
extern std::uint32_t g_savedSoundSceneIndex;
void sndSaveSceneState();
bool sndLoadSceneMap(const char *name);
void sndRestoreSceneState();

#define SOUND_MAP_RECORD SHEN_NAME4('S', 'M', 'A', 'P')
#define SOUND_SEQUENCE_RECORD SHEN_NAME4('S', 'B', 'T', 'S')
extern const SoundDataHeader *g_currentSoundMap;
extern std::uint32_t g_selectedSoundProgramFrame;
extern const SelectedSoundProgramRecord *g_selectedSoundProgram;
struct SoundMusicState {
    char bankName[16];
    std::int32_t activeProgramCommand;
    std::int32_t bank;
    float volume;
    float volumeStep;
    std::int32_t publishedVolume;
    std::int32_t fadeFrames;
    std::int32_t duckVolume;
    std::int32_t duckDelay;
};
typedef char Layout_sound_table_dc_18[(sizeof(SoundMusicState) == 0x30) ? 1 : -1];
typedef char Layout_sound_table_dc_19[(offsetof(SoundMusicState, activeProgramCommand) == 0x10) ? 1 : -1];
typedef char Layout_sound_table_dc_20[(offsetof(SoundMusicState, volume) == 0x18) ? 1 : -1];
typedef char Layout_sound_table_dc_21[(offsetof(SoundMusicState, duckDelay) == 0x2c) ? 1 : -1];
extern SoundMusicState g_soundMusic;
extern const char *g_soundDefinitionProgramName;
extern std::int32_t g_soundDefinitionSceneIndex;
const char *FindSoundMapSequenceName(const char *name);
std::int32_t GetSoundSequenceSceneCount(const char *name);
void SNDD_Reader(const struct MapInfoChunk *chunk, std::int32_t act, std::int32_t entry);
bool SNDP_SoundProgramReader(const struct MapInfoChunk *chunk, std::int32_t act, shen_name area);
std::int32_t CompareSoundProgramKeys(const char *first, const char *second);
SampleProgramRecord *GetLoudestSampleProgramRecord(const SampleProgramState *program);
void CopySoundProgramKey(const char *name, char *key);
const SoundDataHeader *FindSoundRecordInTable(const SoundDataHeader *table, shen_name tag, std::uint32_t key0, std::uint32_t key1);
const SoundDataHeader *FindSoundRecord(shen_name tag, const char *name);
const SoundSequenceHeader *FindSbt(const char *name);
void shenAssignSoundProgramByPtr(const SoundDataHeader *data);
bool shenAssignActiveSmp(const char *name);
bool SelectSoundMap(const char *name, std::int32_t act);
void ConfigureSoundMap(const SoundMapRecord *record, std::int32_t act);
void LoadSoundBankForAct(std::int32_t act, std::uint32_t bank, const char *filename);
void ReleaseSoundBank(std::uint32_t bank);
bool LoadSoundProgram(const char *name, std::uint32_t sceneIndex, std::int32_t act);
bool ProcessAicaCommand(std::uint32_t command, std::int32_t value);
void ResetBankVolumeFades();
bool UpdateVolumeFades();
void InitializeMaterialSoundEffects();
void RestoreMaterialSoundHandles(std::int32_t mode);
void ResetSelectedSoundProgram();
void InitializeAmbientSoundProgram();
void SoundProgramControl_Callback(void *data);
void UpdateSampleProgram(SampleProgramState *program);
void AdvanceSelectedSoundProgram(bool fadeCommandIssued);
void SampleChannelCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value, const std::uint8_t *channels);
void SampleChannelBankCommand(std::uint32_t command, std::uint32_t bank, const std::uint8_t *channels);
std::int32_t SoundPan(std::int32_t *pan, std::int32_t yaw, const Vector3 *source, const Vector3 *camera);
float SoundDistanceGain(const Vector3 *listener, const Vector3 *source, float minimumDistance, float fadeDistance);
void UpdateSelectedSoundProgram(bool fadeCommandIssued);
void UpdateSampleProgramDucking(SampleProgramState *program, bool fadeCommandIssued);
void SetSoundPaused(std::int32_t enabled);
std::int32_t GetConversationStreamPauseState(const struct ConversationStream *stream);
void SetConversationStreamPauseState(struct ConversationStream *stream, std::int32_t paused);
void SetConversationsPaused(std::int32_t paused);
void SoundDebugDisplayNullsub(SampleProgramState *program, const char *caption, std::int32_t *selection);
SampleProgramState *SelectSampleProgram(SampleProgramState *first);
std::int32_t StartSampleProgram(std::uint32_t disabled, SampleProgramState *program);
std::int32_t SetSampleProgramEnabled(std::int32_t sample, std::int32_t enabled, SampleProgramState *program);
std::int32_t spmSetSampleEnabled(std::int32_t sample, std::int32_t enabled);
std::int32_t spmSetSecondarySampleEnabled(std::int32_t sample, std::int32_t enabled);

} // namespace s2
} // namespace shenmue

#endif
