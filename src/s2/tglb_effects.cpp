#include "s2/tglb_effects.hpp"

#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/motion_work.hpp"
#include "s2/particle_effects.hpp"
#include "s2/tglb_event.hpp"

namespace shenmue::s2 {

HLib::Task *togyuSmokeTask{};
HLib::Task *togyuBattleVoiceTask{};
std::int32_t togyuVictorySmokeId{};
std::int32_t togyuMotionVoiceIndex{};

// @verified - p
std::int32_t CreateTogyuSmoke() {
    const std::uint32_t effect = static_cast<std::uint32_t>(
        CreateSmokeEffect(togyuSmokePosition, togyuSmokeVolume, togyuSmokeCount, togyuSmokeInitialBirthRate, togyuSmokeFlags, togyuSmokeProfile));
    SetSmokeEffectFadeFactor(effect, togyuSmokeFadeFactor);
    SetSmokeEffectFalloff(effect, togyuSmokeFalloff);
    SetParticleEffectColour(0, effect, 0, 223, 234, 202);
    SetParticleEffectInitialScale(0, effect, togyuSmokeInitialScale);
    SetParticleEffectSize(0, effect, togyuSmokeSize, togyuSmokeSize);
    SetParticleEffectBirthRate(0, effect, togyuSmokeBirthRate);
    SetParticleEffectVelocity(0, effect, togyuSmokeVelocity, togyuSmokeVelocityVariance);
    SetSmokeEffectSpread(effect, togyuSmokeSpreadFalloff, togyuSmokeSpreadOpacity);
    SetSmokeEffectRadialMotion(effect, togyuSmokeRadialVelocity, togyuSmokeRadialDecay);
    return static_cast<std::int32_t>(effect);
}

// @verified - p
void TogyuSmokeControlCallback(void *taskData) {
    TogyuSmokeControlWork &work = *static_cast<TogyuSmokeControlWork *>(taskData);
    TogyuEventWork &event = *work.event;
    if (work.mode == 0) {
        if (event.smokeId == -1)
            event.smokeId = CreateTogyuSmoke();
    } else if (work.mode == 1) {
        SetStationarySmokeAlpha(static_cast<std::uint32_t>(event.smokeId), 16);
        ResumeParticleEffect(0, static_cast<std::uint32_t>(event.smokeId));
    }
    ++work.mode;
}

HLib::Task *QueueTogyuSmokeControl(TogyuEventWork *event) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TogyuSmokeControlCallback, 6, 0x0b, sizeof(TogyuSmokeControlWork), togyuSmokeTaskToken);
    togyuSmokeTask = task;
    TogyuSmokeControlWork &work = *static_cast<TogyuSmokeControlWork *>(HLib::GetTaskParameter(task));
    work.event = event;
    work.mode = 0;
    return task;
}

// @verified - p
void SuspendTogyuSmoke(TogyuEventWork *event, float) {
    if (event->smokeId != -1) {
        SuspendParticleEffect(0, static_cast<std::uint32_t>(event->smokeId));
        EV_debug_log("TGLB\\eTGLB_et_tgy_battle.c", 1851, "Stop! SMOKE\n");
    }
}

// @verified - p
void UpdateTogyuSmoke(TogyuEventWork *event) {
    const std::uint32_t effect = static_cast<std::uint32_t>(event->smokeId);
    if (effect != invalidEffect) {
        SetParticleEffectPosition(0, effect, &togyuSmokePosition, &togyuSmokeVolume);
        SetSmokeEffectFadeFactor(effect, togyuSmokeFadeFactor);
        SetSmokeEffectFalloff(effect, togyuSmokeFalloff);
        SetParticleEffectColour(0, effect, 0, 223, 234, 202);
        SetParticleEffectInitialScale(0, effect, togyuSmokeInitialScale);
        SetParticleEffectSize(0, effect, togyuSmokeSize, togyuSmokeSize);
        SetParticleEffectBirthRate(0, effect, togyuSmokeBirthRate);
        SetParticleEffectVelocity(0, effect, togyuSmokeVelocity, togyuSmokeVelocityVariance);
        SetSmokeEffectSpread(effect, togyuSmokeSpreadFalloff, togyuSmokeSpreadOpacity);
        SetSmokeEffectRadialMotion(effect, togyuSmokeRadialVelocity, togyuSmokeRadialDecay);
        ResumeParticleEffect(0, static_cast<std::uint32_t>(event->smokeId));
    }
}

// @verified - p
std::int32_t CreateTogyuVictorySmoke() {
    return static_cast<std::int32_t>(
        CreateSmokeEffect(victorySmokePosition, victorySmokeVolume, togyuSmokeCount, togyuSmokeInitialBirthRate, togyuSmokeFlags, togyuSmokeProfile));
}

// @verified - p
std::int32_t ConfigureTogyuVictorySmoke() {
    SetSmokeEffectFadeFactor(static_cast<std::uint32_t>(togyuVictorySmokeId), victorySmokeFadeFactor);
    SetSmokeEffectFalloff(static_cast<std::uint32_t>(togyuVictorySmokeId), victorySmokeFalloff);
    SetParticleEffectColour(0, static_cast<std::uint32_t>(togyuVictorySmokeId), 0, 192, 192, 192);
    SetParticleEffectInitialScale(0, static_cast<std::uint32_t>(togyuVictorySmokeId), togyuSmokeInitialScale);
    SetParticleEffectSize(0, static_cast<std::uint32_t>(togyuVictorySmokeId), victorySmokeSize, victorySmokeSize);
    SetParticleEffectBirthRate(0, static_cast<std::uint32_t>(togyuVictorySmokeId), victorySmokeBirthRate);
    SetParticleEffectVelocity(0, static_cast<std::uint32_t>(togyuVictorySmokeId), victorySmokeRiseVelocity, victorySmokeVelocityVariance);
    SetSmokeEffectSpread(static_cast<std::uint32_t>(togyuVictorySmokeId), victorySmokeSpreadFalloff, victorySmokeSpreadOpacity);
    return togyuVictorySmokeId;
}

// @verified - p
void TogyuBattleVoiceControlCallback(void *taskData) {
    TogyuBattleVoiceWork &work = *static_cast<TogyuBattleVoiceWork *>(taskData);
    if (work.phase == 0) {
        if (cvsHasValidConverseData())
            return;
        Character *const character = FindCharacterByScenarioName(TOGYU_NAME);
        const std::int32_t healthPercent = static_cast<std::int32_t>(GetFightCharacterFlag(1, character, 0.0f));
        std::uint16_t cue = 0;
        if (work.cueGroup >= 0 && work.cueGroup <= 2) {
            const std::int32_t choice = shenGetRandomInt32(healthPercent <= 30 ? 3 : 4);
            cue = healthPercent <= 30 ? togyuLowHealthVoiceCues[work.cueGroup][choice] : togyuBattleVoiceCues[work.cueGroup][choice];
            if (work.cueGroup == 2)
                work.cueGroup = 0;
            else
                ++work.cueGroup;
        }
        QueueBattleDialogue(10, static_cast<std::int16_t>(cue));
        HLib::SetupTaskDelay(togyuBattleVoiceAdmissionDelay);
        work.phase = 1;
    } else if (work.phase == 1) {
        if (!cvsHasValidConverseData()) {
            work.waitFrames = togyuBattleVoiceDelay;
            work.phase = 2;
        }
    } else if (work.phase == 2 && work.waitFrames-- == 1) {
        work.phase = 0;
    }
}

// @verified - p
std::int64_t SelectTogyuHitVoice(void *callbackArgument) {
    std::int16_t &index = *static_cast<std::int16_t *>(callbackArgument);
    if (index >= 0 && index <= 7)
        return togyuHitVoiceCues[index++];
    if (index == 8) {
        index = 0;
        return togyuHitVoiceCues[8];
    }
    return 0;
}

// @verified - p
std::int64_t SelectTogyuMotionVoice(void *) {
    if (togyuMotionVoiceIndex == 0) {
        togyuMotionVoiceIndex = 1;
        return togyuMotionVoiceCues[0];
    }
    if (togyuMotionVoiceIndex == 1) {
        togyuMotionVoiceIndex = 2;
        return togyuMotionVoiceCues[1];
    }
    if (togyuMotionVoiceIndex == 2) {
        togyuMotionVoiceIndex = 0;
        return togyuMotionVoiceCues[2];
    }
    return 0;
}

// @verified - p
HLib::Task *QueueTogyuBattleVoiceControl() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TogyuBattleVoiceControlCallback, 6, 0x0b, sizeof(TogyuBattleVoiceWork), togyuBattleVoiceTaskToken);
    TogyuBattleVoiceWork &work = *static_cast<TogyuBattleVoiceWork *>(HLib::GetTaskParameter(task));
    work.phase = 0;
    work.cueGroup = 0;
    HLib::SetupTimedTask(task, togyuBattleVoiceDelay);
    std::uint32_t id = AllocateBattleDialogue(10, -1, TOGYU_NAME, togyuCharacterNames[0], 1, 100.0f, 1);
    AssignBattleDialogueCallback(id, SelectTogyuHitVoice);
    togyuMotionVoiceIndex = 0;
    id = AllocateMotionBattleDialogue(9, -1, TOGYU_NAME, shen_name{}, 2071, 1, 100.0f, 1);
    AssignBattleDialogueCallback(id, SelectTogyuMotionVoice);
    id = AllocateMotionBattleDialogue(9, -1, TOGYU_NAME, shen_name{}, 2072, 1, 100.0f, 1);
    AssignBattleDialogueCallback(id, SelectTogyuMotionVoice);
    return task;
}

} // namespace shenmue::s2
