#include "s2/sound_table_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_table_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/title_dc.hpp"
#include "platform/dc/aica.hpp"
#include "platform/dc/simm.hpp"
#include <cstdio>
#include <cstring>

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_EV_LoadAudio(const char *name, std::uint32_t sceneIndex);
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_BeginPrimarySampleExecution(std::uint32_t disabled);
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_BeginSecondarySampleExecution(std::uint32_t disabled);

namespace shenmue {
namespace s2 {

SampleProgramState *SelectSampleProgram(SampleProgramState *first);

// @verified - d
void sndSaveSceneState() {
    std::strcpy(g_savedSoundMapName, GetCurrentBgm());
    std::strcpy(g_savedSoundSequenceName, shenGetCurrentSbt());
    g_savedSoundSceneIndex = GetCurrentSoundSceneIndex();
}

// @verified - d
bool sndLoadSceneMap(const char *name) {
    return SelectSoundMap(name, g_actNumber);
}

// @verified - d
void sndRestoreSceneState() {
    if (g_savedSoundMapName[0]) {
        sndLoadSceneMap(g_savedSoundMapName);
        if (g_savedSoundSequenceName[0])
            shenmue_s2_event_EV_LoadAudio(g_savedSoundSequenceName, g_savedSoundSceneIndex);
    }
}

const SoundDataHeader *g_currentSoundMap = 0;

SoundEffectHandle g_materialSoundHandles[16];

std::int32_t g_soundPauseDepth;

std::uint32_t g_soundCallbackParity;

char g_activeSoundMapName[16];

char g_currentSoundSequenceName[16];

std::int32_t g_currentSoundSceneIndex;

std::int32_t g_soundListenerUsesCamera;

SoundBankFade g_soundBankFades[8];

char g_savedSoundMapName[16];

char g_savedSoundSequenceName[16];

std::uint32_t g_savedSoundSceneIndex;

std::uint32_t g_selectedSoundProgramFrame;

const SelectedSoundProgramRecord *g_selectedSoundProgram;

SoundMusicState g_soundMusic;

// @verified - d
void CopySoundProgramKey(const char *name, char *key) {
    std::int32_t length = 0;
    while (length < 8 && *name != '.' && *name != '\0') {
        *key++ = *name++;
        ++length;
    }
    for (; length < 8; ++length)
        *key++ = '\0';
}

// @verified - d
const SoundDataHeader *FindSoundRecordInTable(const SoundDataHeader *table, shen_name tag, std::uint32_t key0, std::uint32_t key1) {
    if (!table)
        return 0;
    for (; table->name; table = reinterpret_cast<const SoundDataHeader *>(reinterpret_cast<const std::byte *>(table) + table->nextOffset)) {
        if (table->name == tag && *reinterpret_cast<const std::uint32_t *>(table->key) == key0 &&
            *reinterpret_cast<const std::uint32_t *>(table->key + 4) == key1)
            return table;
    }
    return 0;
}

// @verified - d
const SoundDataHeader *FindSoundRecord(shen_name tag, const char *name) {
    std::uint32_t key[3];
    CopySoundProgramKey(name, reinterpret_cast<char *>(key));
    PakGlobalData *package = 0;
    while ((package = GetNextReadyModelPackage(package)) != 0) {
        const std::byte *const data = GetIdlePakSoundTable(package);
        if (data) {
            const SoundDataHeader *const record = FindSoundRecordInTable(reinterpret_cast<const SoundDataHeader *>(data + 8), tag, key[0], key[1]);
            if (record)
                return record;
        }
    }
    if (!g_currentSoundMap)
        return 0;
    key[2] = 0;
    return FindSoundRecordInTable(g_currentSoundMap, tag, key[0], key[1]);
}

// @verified - d
const SoundSequenceHeader *FindSbt(const char *name) {
    return reinterpret_cast<const SoundSequenceHeader *>(FindSoundRecord(SOUND_SEQUENCE_RECORD, name));
}

// @verified - d
void shenAssignSoundProgramByPtr(const SoundDataHeader *data) {
    g_currentSoundMap = data;
}

// @verified - d
void LoadSoundBankForAct(std::int32_t act, std::uint32_t bank, const char *filename) {
    char directory[40];
    if (act < 0)
        std::memcpy(directory, "MISC", 5);
    else
        std::sprintf(directory, "SCENE/%02d/SOUND", static_cast<int>(act));
    if (filename && CompareCharacterTableTokens("FREE", filename) != 0)
        LoadSoundBankData(directory, filename, bank);
}

void LoadResizedSoundBank(const char *directory, const char *filename, std::uint32_t bank) {
    if (!CompareCharacterTableTokens("FREE", filename))
        return;
    const std::uint32_t size = shenOpenFile(directory, filename);
    if (g_soundBankAllocations[bank] < size) {
        ReleaseSoundBank(bank);
        SetSoundBankAllocation(bank, size);
    }
    LoadSoundBankData(directory, filename, bank);
}

// @verified - d
void ReleaseSoundBank(std::uint32_t bank) {
    g_soundBankNames[bank][0] = 0;
    ReleaseMaterialSoundBank(bank);
    ReleaseSampleProgramBank(bank);
    ExecuteAicaCommand(0x1100a0, bank, 0);
    SOND_ReleaseBank(bank);
    if (static_cast<std::int32_t>(bank) == g_soundMusic.bank)
        g_soundMusic.activeProgramCommand = -1;
}

// @verified - d
bool ProcessAicaCommand(std::uint32_t command, std::int32_t value) {
    if (g_soundMusic.activeProgramCommand != -1) {
        ExecuteAicaCommand(command, g_soundMusic.bank, value);
        g_soundMusic.activeProgramCommand = -1;
        return true;
    }
    return false;
}

// @verified - d
bool LoadSoundProgram(const char *name, std::uint32_t sceneIndex, std::int32_t act) {
    if (sceneIndex == 0xffffffffU)
        return ProcessAicaCommand(0x1200a0, 0);
    const SoundSequenceHeader *const header = FindSbt(name);
    if (!header)
        return false;
    const SelectedSoundProgramRecord *record =
        reinterpret_cast<const SelectedSoundProgramRecord *>(reinterpret_cast<const std::byte *>(header) + header->dataOffset + sizeof(SoundDataHeader));
    for (std::int32_t index = 0; index < header->count; ++index, ++record) {
        if (record->sceneIndex != sceneIndex)
            continue;
        std::strcpy(g_currentSoundSequenceName, name);
        g_currentSoundSceneIndex = sceneIndex;
        char filename[16];
        std::strncpy(filename, record->bankName, 8);
        filename[8] = '\0';
        std::strcat(filename, ".snd");
        LoadSoundBankForAct(act, record->bank, filename);
        if (g_soundBankLoadStates[record->bank] < 0) {
            g_selectedSoundProgram = 0;
            return false;
        }
        g_selectedSoundProgramFrame = 0;
        g_selectedSoundProgram = record;
        return true;
    }
    return false;
}

// @verified - d
void ResetBankVolumeFades() {
    for (std::int32_t bank = 0; bank < 8; ++bank)
        g_soundBankFades[bank].step = 0.0f;
}

// @verified - d
std::int32_t StartSampleProgram(std::uint32_t disabled, SampleProgramState *program) {
    if (!program || !program->records)
        return 0;
    if (program->bank < 8 && g_soundBankLoadStates[program->bank] >= 0 && program->executing == 0) {
        program->executing = 1;
        program->duckVolume = 0;
        ExecuteAicaCommand(program->command, 0, 0);
        SampleProgramRecord *record = program->records;
        for (std::int32_t index = 0; index < program->count; ++index, ++record) {
            record->disabled = disabled & 1U;
            disabled >>= 1;
            record->volume = 0;
            record->pan = 0;
            record->attenuation = 0;
        }
        return 1;
    }
    return 0;
}

// @verified - d
void ConfigureSoundMap(const SoundMapRecord *record, std::int32_t act) {
    char filename[16];
    std::int32_t bank;
    std::int32_t index;
    for (bank = 0; bank < 8; ++bank) {
        if (record->banks[bank].allocation != 0xffffffffU && g_soundBankAllocations[bank] != record->banks[bank].allocation)
            break;
    }
    if (bank <= 7) {
        for (index = 7; bank <= index; --index)
            ReleaseSoundBank(index);
        for (index = bank; index <= 7; ++index)
            SetSoundBankAllocation(index, record->banks[index].allocation);
    }
    for (bank = 0; bank < 8; ++bank) {
        if (record->banks[bank].filename[0]) {
            if (static_cast<std::uint32_t>(record->banks[bank].filename[0]) != '@') {
                if (std::strstr(record->banks[bank].filename, ".sbt")) {
                    const SoundSequenceHeader *const sequence = FindSbt(record->banks[bank].filename);
                    if (sequence) {
                        std::strcpy(g_currentSoundSequenceName, record->banks[bank].filename);
                        for (index = 0; index < 8; ++index) {
                            if (sequence->bankNames[index][0]) {
                                std::strncpy(filename, sequence->bankNames[index], 8);
                                filename[8] = '\0';
                                std::strcat(filename, ".snd");
                                LoadSoundBankForAct(act, index, filename);
                            }
                        }
                        if (g_pendingSoundSceneIndex >= 0) {
                            shenmue_s2_event_EV_LoadAudio(record->banks[bank].filename, g_pendingSoundSceneIndex);
                            g_pendingSoundSceneIndex = -1;
                        }
                    }
                } else {
                    LoadSoundBankForAct(act, bank, record->banks[bank].filename);
                }
            }
        } else {
            ExecuteAicaCommand(0x1100a0, bank, 0);
            if (bank == g_soundMusic.bank)
                g_soundMusic.activeProgramCommand = -1;
            g_soundBankLoadStates[bank] = -1;
            if (bank == g_samplePrograms[0].bank)
                g_samplePrograms[0].executing = 0;
            if (bank == g_samplePrograms[1].bank)
                g_samplePrograms[1].executing = 0;
            if (bank == g_secondarySamplePrograms[0].bank)
                g_secondarySamplePrograms[0].executing = 0;
            if (bank == g_secondarySamplePrograms[1].bank)
                g_secondarySamplePrograms[1].executing = 0;
        }
    }
    ResetBankVolumeFades();
    shenmue_s2_event_BeginPrimarySampleExecution(0);
    shenmue_s2_event_BeginSecondarySampleExecution(0);
}

// @verified - d
bool SelectSoundMap(const char *name, std::int32_t act) {
    const SoundDataHeader *const record = FindSoundRecord(SOUND_MAP_RECORD, name);
    if (!record)
        return false;
    std::uint32_t key[3];
    CopySoundProgramKey(name, reinterpret_cast<char *>(key));
    key[2] = 0;
    std::strcpy(g_activeSoundMapName, reinterpret_cast<const char *>(key));
    ConfigureSoundMap(reinterpret_cast<const SoundMapRecord *>(record), act);
    return true;
}

// @verified - d
bool shenAssignActiveSmp(const char *name) {
    return SelectSoundMap(name, -1);
}

// @verified - d
bool LoadTitleSoundData() {
    const SoundDataHeader *const data =
        reinterpret_cast<const SoundDataHeader *>(shenLoadFile(Filepath_Gen(FilepathGenMode::MiscDirectory), "tit_snd.bin", TITLE_SOUND_DATA_TASK));
    if (!data)
        return false;
    shenAssignSoundProgramByPtr(data);
    if (!shenAssignActiveSmp("0002S000.smp"))
        return false;
    return true;
}

// @verified - d
void LoadTitleSoundProgram(std::int32_t sceneIndex) {
    std::uint32_t scene = 0xffffffffU;
    if (sceneIndex >= 0)
        scene = 0;
    LoadSoundProgramAnyScene("0002S000.sbt", scene);
}

} // namespace s2
} // namespace shenmue

#endif
