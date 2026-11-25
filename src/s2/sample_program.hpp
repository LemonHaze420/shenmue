#pragma once
#include "shenmue.h"
#include "common/math.hpp"
#if SHENMUE_S2_GCC295
#define SHENMUE_SAMPLE_ARRAY(type, name, count) type name[count]
#define SHENMUE_SAMPLE_DEFAULT
#else
#include <array>
#include <cstddef>
#include <cstdint>
#define SHENMUE_SAMPLE_ARRAY(type, name, count) std::array<type, count> name
#define SHENMUE_SAMPLE_DEFAULT {}
#endif
namespace shenmue { namespace s2 {
struct MaterialSoundPair {
    std::uint32_t material;
    std::uint32_t command;
};
struct MaterialSoundBank {
    MaterialSoundPair *pairs;
    std::uint32_t capacity;
    std::int32_t bank;
    std::uint32_t count;
};
#if SHENMUE_S2_GCC295
typedef char sizeofMaterialSoundPairCheck[sizeof(MaterialSoundPair) == 8 ? 1 : -1];
typedef char sizeofMaterialSoundBankCheck[sizeof(MaterialSoundBank) == (SHENMUE_IS_DC ? 0x10 : 0x18) ? 1 : -1];
typedef char offsetofMaterialSoundBankpairsCheck[offsetof(MaterialSoundBank, pairs) == 0 ? 1 : -1];
typedef char offsetofMaterialSoundBankcapacityCheck[offsetof(MaterialSoundBank, capacity) == (SHENMUE_IS_DC ? 4 : 8) ? 1 : -1];
typedef char offsetofMaterialSoundBankbankCheck[offsetof(MaterialSoundBank, bank) == (SHENMUE_IS_DC ? 8 : 12) ? 1 : -1];
typedef char offsetofMaterialSoundBankcountCheck[offsetof(MaterialSoundBank, count) == (SHENMUE_IS_DC ? 12 : 16) ? 1 : -1];
#else
static_assert(sizeof(MaterialSoundPair) == 8);
static_assert(sizeof(MaterialSoundBank) == (SHENMUE_IS_DC ? 0x10 : 0x18));
static_assert(offsetof(MaterialSoundBank, pairs) == 0);
static_assert(offsetof(MaterialSoundBank, capacity) == (SHENMUE_IS_DC ? 4 : 8));
static_assert(offsetof(MaterialSoundBank, bank) == (SHENMUE_IS_DC ? 8 : 12));
static_assert(offsetof(MaterialSoundBank, count) == (SHENMUE_IS_DC ? 12 : 16));
#endif

struct SoundEffectHandle {
    std::int32_t active;
    std::int32_t command;
    std::uint32_t slot;
    std::int32_t gain;
    std::int32_t volume;
    std::int32_t control;
};
#if SHENMUE_S2_GCC295
typedef char sizeofSoundEffectHandleCheck[sizeof(SoundEffectHandle) == 0x18 ? 1 : -1];
typedef char offsetofSoundEffectHandleactiveCheck[offsetof(SoundEffectHandle, active) == 0 ? 1 : -1];
typedef char offsetofSoundEffectHandlecommandCheck[offsetof(SoundEffectHandle, command) == 4 ? 1 : -1];
typedef char offsetofSoundEffectHandleslotCheck[offsetof(SoundEffectHandle, slot) == 8 ? 1 : -1];
typedef char offsetofSoundEffectHandlegainCheck[offsetof(SoundEffectHandle, gain) == 12 ? 1 : -1];
typedef char offsetofSoundEffectHandlevolumeCheck[offsetof(SoundEffectHandle, volume) == 16 ? 1 : -1];
typedef char offsetofSoundEffectHandlecontrolCheck[offsetof(SoundEffectHandle, control) == 20 ? 1 : -1];
#else
static_assert(sizeof(SoundEffectHandle) == 0x18);
static_assert(offsetof(SoundEffectHandle, active) == 0);
static_assert(offsetof(SoundEffectHandle, command) == 4);
static_assert(offsetof(SoundEffectHandle, slot) == 8);
static_assert(offsetof(SoundEffectHandle, gain) == 12);
static_assert(offsetof(SoundEffectHandle, volume) == 16);
static_assert(offsetof(SoundEffectHandle, control) == 20);
#endif

struct SoundBankFade {
    float volume;
    float step;
};
#if SHENMUE_S2_GCC295
typedef char sizeofSoundBankFadeCheck[sizeof(SoundBankFade) == 8 ? 1 : -1];
typedef char alignofSoundBankFadeCheck[__alignof__(SoundBankFade) == 4 ? 1 : -1];
typedef char offsetofSoundBankFadestepCheck[offsetof(SoundBankFade, step) == 4 ? 1 : -1];
#else
static_assert(sizeof(SoundBankFade) == 8);
static_assert(alignof(SoundBankFade) == 4);
static_assert(offsetof(SoundBankFade, step) == 4);
#endif

struct SoundProgramFadeRecord {
    std::uint16_t frame;
    std::uint8_t volume;
    std::uint8_t duration;
};
struct SelectedSoundProgramRecord {
    std::uint32_t reserved00;
    char bankName[8];
    std::uint16_t sequence;
    std::uint8_t bank;
    std::uint8_t volume;
    std::uint8_t restart;
    std::uint8_t sceneIndex;
    std::uint16_t reserved12;
    std::uint16_t startFrame;
    std::uint16_t reserved16;
    std::uint16_t panFrame;
    std::uint8_t reserved1a;
    std::uint8_t pan;
    std::uint32_t reserved1c;
    SoundProgramFadeRecord fades[16];
};
#if SHENMUE_S2_GCC295
typedef char sizeofSelectedSoundProgramRecordCheck[sizeof(SelectedSoundProgramRecord) == 96 ? 1 : -1];
typedef char offsetofSelectedSoundProgramRecordstartFrameCheck[offsetof(SelectedSoundProgramRecord, startFrame) == 20 ? 1 : -1];
typedef char offsetofSelectedSoundProgramRecordfadesCheck[offsetof(SelectedSoundProgramRecord, fades) == 32 ? 1 : -1];
#else
static_assert(sizeof(SelectedSoundProgramRecord) == 96);
static_assert(offsetof(SelectedSoundProgramRecord, startFrame) == 20);
static_assert(offsetof(SelectedSoundProgramRecord, fades) == 32);
#endif

struct SampleProgramRecord {
    std::uint16_t reserved00;
    SHENMUE_SAMPLE_ARRAY(std::uint8_t, channels, 8);
    std::uint8_t spatialMode;
    std::uint8_t panMode;
    std::uint8_t timeMode;
    std::uint8_t frontGainPercent;
    std::uint8_t weatherMode;
    std::uint8_t weekdays;
    Vector3 position;
    std::uint32_t characterName;
    float minimumDistance;
    float fadeDistance;
    std::uint8_t minimumMusicVolume;
    SHENMUE_SAMPLE_ARRAY(std::byte, reserved29, 3);
    std::uint8_t disabled;
    std::uint8_t volume;
    std::int8_t pan;
    std::uint8_t attenuation;
    SHENMUE_SAMPLE_ARRAY(std::uint8_t, quarterHourGains, 96);
};

struct SampleProgramPool {
    SampleProgramRecord *records;
    std::int32_t capacity;
    std::int32_t bank;
};

#if SHENMUE_S2_GCC295
typedef char sizeofSampleProgramRecordCheck[sizeof(SampleProgramRecord) == 0x90 ? 1 : -1];
typedef char offsetofSampleProgramRecorddisabledCheck[offsetof(SampleProgramRecord, disabled) == 0x2c ? 1 : -1];
typedef char offsetofSampleProgramRecordpositionCheck[offsetof(SampleProgramRecord, position) == 0x10 ? 1 : -1];
typedef char offsetofSampleProgramRecordquarterHourGainsCheck[offsetof(SampleProgramRecord, quarterHourGains) == 0x30 ? 1 : -1];
typedef char sizeofSampleProgramPoolCheck[sizeof(SampleProgramPool) == (SHENMUE_IS_DC ? 0x0c : 0x10) ? 1 : -1];
#else
static_assert(sizeof(SampleProgramRecord) == 0x90);
static_assert(offsetof(SampleProgramRecord, disabled) == 0x2c);
static_assert(offsetof(SampleProgramRecord, position) == 0x10);
static_assert(offsetof(SampleProgramRecord, quarterHourGains) == 0x30);
static_assert(sizeof(SampleProgramPool) == (SHENMUE_IS_DC ? 0x0c : 0x10));
#endif

struct SampleProgramState {
    std::int32_t executing SHENMUE_SAMPLE_DEFAULT;
    std::uint32_t command SHENMUE_SAMPLE_DEFAULT;
    std::int32_t count SHENMUE_SAMPLE_DEFAULT;
    std::int32_t bank SHENMUE_SAMPLE_DEFAULT;
    std::int32_t duckVolume SHENMUE_SAMPLE_DEFAULT;
    std::int32_t duckDelay SHENMUE_SAMPLE_DEFAULT;
    SampleProgramRecord *records SHENMUE_SAMPLE_DEFAULT;
#if SHENMUE_S2_GCC295
    SampleProgramState() : executing(0), command(0), count(0), bank(0), duckVolume(0), duckDelay(0), records(0) {}
#endif
};

#if SHENMUE_S2_GCC295
typedef char sizeofSampleProgramStateCheck[sizeof(SampleProgramState) == (SHENMUE_IS_DC ? 0x1c : 0x20) ? 1 : -1];
typedef char offsetofSampleProgramStaterecordsCheck[offsetof(SampleProgramState, records) == 0x18 ? 1 : -1];
#else
static_assert(sizeof(SampleProgramState) == (SHENMUE_IS_DC ? 0x1c : 0x20));
static_assert(offsetof(SampleProgramState, records) == 0x18);
#endif
} } // namespace shenmue::s2

#undef SHENMUE_SAMPLE_ARRAY
#undef SHENMUE_SAMPLE_DEFAULT
