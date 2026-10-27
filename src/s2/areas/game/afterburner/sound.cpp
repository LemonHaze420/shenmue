#include "s2/areas/game/afterburner/sound.hpp"
#include "s2/areas/game/arcade/audio_host.hpp"
#include "s2/areas/game/arcade/sound.hpp"
#include "s1/archive_data.hpp"
#include "s1/audio_stream.hpp"
#include <cstdlib>

namespace shenmue::s2 {
namespace {
std::array<std::uint32_t, 4> engineVoices{};
std::array<std::uint8_t, 32> commands{};
std::array<std::uint8_t, 256> deferredCommands{};
std::uint32_t commandCount{}, engineCounters{};
std::int32_t engineInput{}, previousInput{}, engineMode{}, accumulator{}, engineFlags{};
float pitchElapsed{}, fadeElapsed{};
std::uint8_t musicCommand{}, enginePaused{};
std::byte *engineSamples{};

void SetMatrix(std::size_t voice, const float *matrix) {
    s1::SetRawSoundEffectOutputMatrix(s1::GetRawSoundEffectVoice(engineVoices[voice]), matrix, 1, 6);
}
void SetReverb(std::size_t voice, float gain) {
    s1::SetOutputMatrixVolumeAndChannels(s1::GetRawSoundEffectVoice(engineVoices[voice]), gain, 1);
}
void SetPitch(std::size_t voice, float pitch) {
    s1::SetRawSoundEffectFrequencyRatio(s1::GetRawSoundEffectVoice(engineVoices[voice]), pitch);
}
float Frequency(std::int32_t pitch, bool truncate) {
    double value = pitch * .0078125 * 15600.0;
    if (value < 100)
        value = 100;
    if (value > 100000)
        value = 100000;
    return truncate ? static_cast<float>(static_cast<std::uint32_t>(value)) / 8000.f : static_cast<float>(value / 8000.0);
}
float EnginePitch(std::int32_t input, std::int32_t offset) {
    if (input < 1)
        input = 1;
    if (input > 63)
        input = 63;
    return Frequency(afterBurnerEnginePitch[input - 1] + offset, false);
}
float Gain(std::uint8_t value) {
    return value / 255.f;
}
void ResetEngine() {
    const std::array<float, 6> matrix{};
    for (std::size_t voice = 0; voice < 4; ++voice) {
        SetMatrix(voice, matrix.data());
        SetReverb(voice, 0);
    }
    engineMode = 0;
}
void SetCounter(std::uint8_t value) {
    engineCounters = (engineCounters & 0xffff00ffU) | (static_cast<std::uint32_t>(value) << 8);
}
std::int8_t Counter() {
    return static_cast<std::int8_t>(engineCounters >> 8);
}
void ProcessCommand(std::uint8_t command) {
    if (command == 0xff) {
        StopArcadeMusic();
        StopArcadeSoundEffects();
        ResetEngine();
        return;
    }
    switch (afterBurnerSoundCommandTypes[command]) {
    case 1: {
        if (deferredCommands[command])
            return;
        const std::int16_t index = afterBurnerSoundIndices[command];
        if (index < 0)
            return;
        if (command == 0x8e) {
            StopArcadeSoundEffect(deferredCommands[0x8f]);
            deferredCommands[0x8f] = 0;
            PlayArcadeSoundEffect(index, false);
        } else {
            if (command == 0x8f) {
                StopArcadeSoundEffect(deferredCommands[0x8e]);
                deferredCommands[0x8e] = 0;
            }
            PlayArcadeSoundEffect(index, true);
        }
        deferredCommands[command] = 0;
        break;
    }
    case 2:
        musicCommand = command;
        if (command != 0xb9)
            QueueArcadeMusic(afterBurnerMusicFiles[command]);
        break;
    case 4:
        if (static_cast<std::uint32_t>(command) - 1U <= 62U)
            engineInput = command;
        if (!command && engineMode != 3)
            engineMode = 0;
        else if (!engineMode && static_cast<std::uint32_t>(command) - 1U <= 62U)
            engineMode = 1;
        else
            switch (command) {
            case 0xee:
                engineMode = 3;
                engineFlags = 136;
                break;
            case 0xef:
                engineMode = 3;
                engineFlags = 160;
                break;
            case 0xf0:
                if (engineMode >= 1) {
                    engineMode = 2;
                    engineFlags = 0;
                }
                break;
            case 0xf1:
                if (engineMode == 2)
                    engineFlags |= 2;
                break;
            case 0xf2:
                engineMode = 3;
                engineFlags = 128;
                break;
            }
        break;
    }
}
void UpdateSteadyEngine() {
    const std::uint8_t delta = static_cast<std::uint8_t>(engineInput >= previousInput ? engineInput - previousInput : previousInput - engineInput);
    std::int32_t next = accumulator + (static_cast<std::int32_t>(delta) << 8);
    if (next > 65535)
        next = 65535;
    next -= 64;
    accumulator = next >= 0 ? next : 0;
    std::int32_t level = (accumulator * 3) >> 8;
    if (level > 24)
        level = 24;
    level += 20;
    if (level >= 64)
        level = 63;
    const float gain = static_cast<float>(level) / 255.f;
    const std::array<float, 6> matrix{gain, gain, 0, 0, 0, 0};
    SetMatrix(0, matrix.data());
    SetMatrix(1, matrix.data());
    std::int32_t input = engineInput;
    if (input < 1)
        input = 1;
    if (input > 63)
        input = 63;
    SetPitch(0, Frequency(afterBurnerEnginePitch[input - 1], true));
    SetPitch(1, Frequency(afterBurnerEnginePitch[input - 1] + 4, true));
    previousInput = engineInput;
}
void UpdatePitchSweep() {
    previousInput = engineInput;
    if (!(engineFlags & 1)) {
        engineFlags |= 1;
        pitchElapsed = 0;
        engineCounters &= 0xff000000U;
    }
    pitchElapsed += 2;
    if (pitchElapsed < 8)
        return;
    const std::int8_t counter = static_cast<std::int8_t>(static_cast<std::uint8_t>(Counter() + 1));
    SetCounter(static_cast<std::uint8_t>(counter));
    pitchElapsed += -8;
    const std::array<float, 6> zero{};
    if (counter >= 63) {
        engineMode = 1;
        accumulator = 0;
        SetMatrix(2, zero.data());
        return;
    }
    SetPitch(0, EnginePitch(counter, 0));
    SetPitch(1, EnginePitch(counter, 4));
    SetPitch(2, EnginePitch(counter, 0));
    std::uint8_t level = afterBurnerEngineGain[counter];
    if (engineFlags & 2) {
        const std::uint8_t decay = static_cast<std::uint8_t>((engineCounters >> 16) + 3);
        level = static_cast<std::uint8_t>(level - decay);
        engineCounters = (engineCounters & 0xff00ffffU) | (static_cast<std::uint32_t>(decay) << 16);
        if (level < 16) {
            accumulator = 0;
            engineMode = 1;
            SetMatrix(2, zero.data());
            SetReverb(2, 0);
            return;
        }
    }
    const float gain = Gain(level);
    const std::array<float, 6> matrix{gain, gain, 0, 0, 0, 0};
    for (std::size_t voice = 0; voice < 3; ++voice)
        SetMatrix(voice, matrix.data());
}
void UpdateEngineFade() {
    previousInput = engineInput;
    if (!(engineFlags & 0x40)) {
        engineFlags |= 0x40;
        fadeElapsed = 0;
        SetCounter(engineFlags & 8 ? 99 : 0);
        const std::array<float, 6> zero{}, right{0, .0784313753247261f, 0, 0, 0, 0};
        SetMatrix(0, zero.data());
        SetMatrix(1, zero.data());
        SetMatrix(2, right.data());
        SetMatrix(3, right.data());
    }
    const std::int32_t interval = engineFlags & 0x20 ? 10 : 7;
    fadeElapsed += 2;
    if (fadeElapsed <= interval)
        return;
    fadeElapsed -= interval;
    std::array<std::array<float, 6>, 4> matrices;
    for (std::size_t voice = 0; voice < 4; ++voice)
        GetArcadePcmVoiceOutputMatrix(engineVoices[voice], matrices[voice].data());
    std::int32_t pitch;
    std::int8_t counter;
    std::uint8_t level;
    if (engineFlags & 8) {
        counter = static_cast<std::int8_t>(static_cast<std::uint8_t>(Counter() - 1));
        pitch = counter;
        level = afterBurnerEngineGain[pitch + 80];
    } else {
        counter = static_cast<std::int8_t>(static_cast<std::uint8_t>(Counter() + 1));
        pitch = counter - 1;
        level = afterBurnerEngineGain[pitch + 192];
    }
    SetCounter(static_cast<std::uint8_t>(counter));
    const float gain = Gain(level);
    matrices[0][0] = gain;
    matrices[1][0] = gain;
    matrices[2][1] = gain;
    matrices[3][1] = gain;
    if (static_cast<std::uint8_t>(counter - 1) > 97) {
        engineFlags &= 0x17;
        engineMode = 1;
        accumulator = 0;
        const std::array<float, 6> zero{};
        SetMatrix(2, zero.data());
        SetMatrix(3, zero.data());
        SetReverb(2, 0);
        SetReverb(3, 0);
        return;
    }
    if (pitch < 1)
        pitch = 1;
    if (pitch > 98)
        pitch = 98;
    SetPitch(0, Frequency(afterBurnerEngineFadePitch[pitch], true));
    SetPitch(1, Frequency(afterBurnerEngineFadePitch[pitch], true));
    SetPitch(2, Frequency(afterBurnerEngineFadePitch[pitch] - 4, true));
    SetPitch(3, Frequency(afterBurnerEngineFadePitch[pitch] - 4, true));
    for (std::size_t voice = 0; voice < 4; ++voice) {
        SetMatrix(voice, matrices[voice].data());
        SetReverb(voice, 0);
    }
}
} // namespace

void InitializeAfterBurnerSound() {
    commandCount = 0;
    deferredCommands.fill(0);
    InitializeArcadeSoundEffects(afterBurnerSoundFiles.data(), 164);
    ResetArcadeMusic();
    musicCommand = 0xff;
    const std::uint64_t size = s1::ArchiveData_GetFileSize("xab/sound/eng.dat");
    engineSamples = static_cast<std::byte *>(std::malloc(static_cast<std::size_t>(size)));
    s1::ArchiveData_Read("xab/sound/eng.dat", 0, engineSamples, size);
    engineMode = 0;
    engineCounters = 0;
    previousInput = 0;
    engineInput = 0;
    accumulator = 0;
    pitchElapsed = 0;
    engineFlags = 0;
    fadeElapsed = 0;
    const std::array<float, 6> matrix{};
    for (std::size_t voice = 0; voice < 4; ++voice) {
        engineVoices[voice] = s1::RawSoundEffectVoiceId(s1::AllocateRawSoundEffectVoice(8, 1, true));
        SubmitArcadePcmVoice(engineVoices[voice], 1, 8000, engineSamples, static_cast<std::uint32_t>(size), 0, static_cast<std::uint32_t>(size), true);
        SetMatrix(voice, matrix.data());
        SetReverb(voice, 0);
    }
    enginePaused = 0;
}
void QueueAfterBurnerSoundCommand(std::uint8_t command) {
    if (commandCount != 32)
        commands[commandCount++] = command;
}
void MuteAfterBurnerPeripheralAudio() {
    StopArcadeMusic();
    StopArcadeSoundEffects();
    ResetEngine();
}
void UpdateAfterBurnerSound(const GameArcadeRuntime &runtime) {
    PauseArcadeSoundEffects(runtime.paused != 0);
    if (musicCommand == 0xb9)
        FadeOutArcadeMusic(120);
    else
        AdvanceArcadeMusic(60);
    const std::uint8_t paused = runtime.paused != 0;
    if (enginePaused != paused) {
        for (std::uint32_t voice : engineVoices)
            s1::SetRawSoundEffectVolume(s1::GetRawSoundEffectVoice(voice), paused ? 0.f : 1.f);
        enginePaused = paused;
    }
    if (runtime.paused)
        return;
    for (std::uint32_t command = 0; command < commandCount; ++command)
        ProcessCommand(commands[command]);
    commandCount = 0;
    switch (engineMode) {
    case 0:
        ResetEngine();
        break;
    case 1:
        UpdateSteadyEngine();
        break;
    case 2:
        UpdatePitchSweep();
        break;
    case 3:
        UpdateEngineFade();
        break;
    }
}
void ReleaseAfterBurnerSound() {
    ReleaseArcadeSoundEffects();
    ReleaseArcadeMusic();
    for (std::uint32_t voice : engineVoices) {
        s1::StopSourceVoiceAndFlush(voice);
        s1::ReleaseRawSoundEffectReservation(s1::GetRawSoundEffectVoice(voice));
    }
    std::free(engineSamples);
}
} // namespace shenmue::s2
