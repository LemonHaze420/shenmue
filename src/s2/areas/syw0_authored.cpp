#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/input.hpp"
#include "s2/model_component.hpp"
#include "s2/training_seqp.hpp"
namespace shenmue::s2::areas {
namespace {
Vector3 syw0FirePositions[10];
void Syw0AuthorCharacter(shen_name character) {
    const Vector4 values{.x = 1, .y = .3f, .z = .3f, .w = .3f};
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
    SetScenarioReflectorBoardValues(character, &values);
    EV_SetRefBoardParamsFor(character, 1);
    SetFaceFlagsForChara(FindCharacterByScenarioName(character), 0);
}
void Syw0FinishAuthorCharacters() {
    EV_EnableCharacterDisplay(SYW0_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(SYW0_RYO), 6, 0);
    for (const shen_name *name = SYW0_AUTHOR_CHARACTERS; name->name != SYW0_INVALID.name; ++name) {
        EV_EnableCharacterDisplay(*name);
        mtwkSetExtraFlag(scenFindMotionwork(*name), 6, 0);
    }
}
void Syw0AuthorLoop(void *data) {
    Syw0AuthorLoopWork &loop = *static_cast<Syw0AuthorLoopWork *>(data);
    Syw0AuthorWork &work = *loop.parent;
    loop.frame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (loop.frame == 1) {
        SetPluralControlFor(FindCharacterByScenarioName(SYW0_RYO), 1);
        SetPluralControlFor(FindCharacterByScenarioName(SYW0_SYE), 1);
        SetPluralControlFor(FindCharacterByScenarioName(SYW0_KNK), 1);
        Syw0AuthorCharacter(SYW0_RYO);
        for (const shen_name *name = SYW0_AUTHOR_CHARACTERS; name->name != SYW0_INVALID.name; ++name)
            Syw0AuthorCharacter(*name);
    }
    if (work.author.name == SYW0_AUTHORS[1].name && loop.frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (work.author.name == SYW0_AUTHORS[2].name) {
        switch (loop.frame) {
        case 547:
            ProcessAicaCommand(0x1200a0, 0);
            break;
        case 549:
            QueueExtraTelop("AAAAAAAA", 0);
            break;
        case 551:
            EV_LoadAudio("0064A000", 0);
            break;
        default:
            break;
        }
    }
    if (work.author.name == SYW0_AUTHORS[5].name && loop.frame == 1000 && EV_GetShenmueFlag(255)) {
        SetScenarioCharacterDrawControl(SYW0_RYO, 0, 0);
        SetScenarioCharacterDrawControl(SYW0_SYE, 0, 0);
        SetScenarioCharacterDrawControl(SYW0_KNK, 0, 0);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SelectPostFxBank(3, work.postFxBank);
        if (work.author.name == SYW0_AUTHORS[5].name && EV_GetShenmueFlag(255)) {
            SetScenarioCharacterDrawControl(SYW0_RYO, 1, 0);
            SetScenarioCharacterDrawControl(SYW0_SYE, 1, 0);
            SetScenarioCharacterDrawControl(SYW0_KNK, 1, 0);
        }
        SetPluralControlFor(FindCharacterByScenarioName(SYW0_RYO), 1);
        SetPluralControlFor(FindCharacterByScenarioName(SYW0_SYE), 1);
        SetPluralControlFor(FindCharacterByScenarioName(SYW0_KNK), 1);
        Syw0FinishAuthorCharacters();
        work.phase = 3;
        HLib::DestroyCurrentTask();
    }
}
void Syw0StartAuthor(Syw0AuthorWork &work) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Syw0AuthorLoop, 4, 0x0b, sizeof(Syw0AuthorLoopWork), SYW0_TASK);
    Syw0AuthorLoopWork &loop = *static_cast<Syw0AuthorLoopWork *>(HLib::GetTaskParameter(task));
    loop.parent = &work;
    loop.frame = 0;
    if (work.author.name != SYW0_AUTHORS[1].name && work.author.name != SYW0_AUTHORS[6].name)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    work.postFxBank = static_cast<std::int32_t>(GetPostFxBankValue(3));
    SelectPostFxBank(3, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author.name));
    work.phase = 2;
}
void Syw0AuthorCallback(void *data) {
    Syw0AuthorWork &work = *static_cast<Syw0AuthorWork *>(data);
    if (work.phase == 1)
        Syw0StartAuthor(work);
    else if (work.phase == 3)
        HLib::DestroyTask(work.task);
}
} // namespace
void Syw0QueueAuthor(shen_name author) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Syw0AuthorCallback, 4, 0x0b, sizeof(Syw0AuthorWork), SYW0_TASK);
    Syw0AuthorWork &work = *static_cast<Syw0AuthorWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.phase = 1;
    work.author = author;
    SetPlayerControlEnabled(0);
}
void Syw0PrepareAuthorCharacters() {
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        AT_SetLocalBaseChara(name, SYW0_INVALID);
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        SetScenarioCharacterDrawControl(name, 1, 0);
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        SetScenarioCharacterDrawControl(name, 2, 1);
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        SetScenarioCharacterType(name, CharacterTypeStencilShadow);
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        ResetTrainingAuthorCharacter(name);
}
void Syw0RestoreTrainingCharacters() {
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        SetScenarioCharacterDrawControl(name, 1, 0);
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        SetScenarioCharacterDrawControl(name, 2, 1);
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
        SetScenarioCharacterType(name, CharacterTypeStencilShadow);
    for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK}) {
        const Vector4 values{.x = 1, .y = .3f, .z = .3f, .w = .3f};
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, false);
        mtwkSetRequestedRootMode(scenFindMotionwork(name), 1);
        SetScenarioReflectorBoardValues(name, &values);
        EV_SetRefBoardParamsFor(name, 1);
    }
    EV_DisableCharacterDisplay(SYW0_KNK);
}
void Syw0CreateFire() {
    syw0FirePositions[0] = {.x = 343.5f, .y = 129.1f, .z = 228.3f};
    syw0FirePositions[1] = {.x = 343.5f, .y = 129.1f, .z = 235.5f};
    syw0FirePositions[2] = {.x = 327.1f, .y = 128.4f, .z = 276};
    syw0FirePositions[3] = {.x = 343.7f, .y = 128.4f, .z = 267.9f};
    syw0FirePositions[4] = {.x = 343.7f, .y = 128.4f, .z = 276};
    syw0FirePositions[5] = {.x = 327.1f, .y = 128.4f, .z = 228.3f};
    syw0FirePositions[6] = {.x = 327.1f, .y = 128.4f, .z = 236.4f};
    syw0FirePositions[7] = {.x = 327.1f, .y = 128.4f, .z = 247.2f};
    syw0FirePositions[8] = {.x = 327.1f, .y = 128.4f, .z = 257.1f};
    syw0FirePositions[9] = {.x = 327.1f, .y = 128.4f, .z = 267.9f};
    syw0FireEffect = static_cast<std::uint32_t>(CreateFireEffect(syw0FirePositions[0], .3f, .4f, 0, 4));
    for (std::int32_t i = 1; i < 10; ++i)
        SetFirePosition(syw0FireEffect, i, &syw0FirePositions[i]);
}
void Syw0CreateSmoke() {
    for (std::int32_t i = 0; i < 2; ++i) {
        const Vector3 volume{.x = .1f, .y = 0, .z = .1f}, velocity{.x = 0, .y = .016f, .z = 0}, variance{.x = 0, .y = 0, .z = 0};
        const std::uint32_t effect = static_cast<std::uint32_t>(CreateSmokeEffect(SYW0_SMOKE_POSITIONS[i], volume, 40, .05f, shen_name{}, 0));
        syw0SmokeEffects[i * 4] = effect;
        SetSmokeEffectFadeFactor(effect, 21);
        SetSmokeEffectFalloff(effect, 110);
        SetParticleEffectColour(0, effect, 0, 144, 144, 144);
        SetParticleEffectInitialScale(0, effect, .329f);
        SetParticleEffectSize(0, effect, 1.56f, 1.41f);
        SetParticleEffectBirthRate(0, effect, .0078f);
        SetParticleEffectVelocity(0, effect, velocity, variance);
        ResumeParticleEffect(0, effect);
    }
}
void Syw0AwardSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    void *event = sequence.event;
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0Advance(sequence, 100);
            break;
        case 100:
            Syw0Wait(sequence, 2, 101, 137);
            return;
        case 101:
            EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
            QueueExtraTelop("AAAAAAAA", 0);
            Syw0Wait(sequence, 2, 102, 1);
            break;
        case 102: {
            const std::int8_t learned = Syw0Value<std::int8_t>(event, 0x74);
            if (learned == -1) {
                Syw0Advance(sequence, 104);
                break;
            }
            if (learned == 5 || !learned) {
                SetMoveProficiency(0, 1, 24, learned ? 12 : 1);
                SetNewEwzoTypeForMoveLearned(learned ? 0 : 2, 1, 24);
            }
            for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
                SetClothFlag(FindCharacterByScenarioName(name), 4, 1);
            Syw0Value(event, 0x76, std::int16_t{150});
            Syw0Value(&sequence, 0x248, std::int32_t{1});
            Syw0Advance(sequence, 103);
            break;
        }
        case 103: {
            const std::int16_t frames = Syw0Value<std::int16_t>(event, 0x76);
            if (GetCurrentControlInput(INPUT_PRESSED) == 512 || !frames) {
                EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
                for (shen_name name : {SYW0_RYO, SYW0_SYE, SYW0_KNK})
                    SetClothFlag(FindCharacterByScenarioName(name), 4, 0);
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                Syw0Advance(sequence, 104);
            } else
                Syw0Value(event, 0x76, static_cast<std::int16_t>(frames - 1));
            break;
        }
        case 104:
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0NightSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0: {
            SetPlayerControlEnabled(0);
            Syw0CreateFire();
            DateTime time;
            GetCurrentDateTime(time);
            time.hour = 20;
            time.minute = 0;
            SetCurrentDateTimeTo(time);
            EV_lightLoad(SYW0_LIGHT);
            PushGlobalLightingProgram();
            CheckIsArgEquZero(1);
            EV_SetObjectFlagByIndex(2, 1);
            EV_SetObjectFlagByIndex(4, 1);
            ResumeParticleEffect(4, syw0FireEffect);
            ApplyWeatherRainAndRippleClip(SYW0_RAIN_CLIP);
            ApplyWeatherParticleClip(SYW0_PARTICLE_CLIP);
            CreateEventCharacter(CharacterCreationHighModel, nullptr, SYW0_NIGHT);
            EV_DisableCharacterDisplay(SYW0_RYO);
            Syw0Wait(sequence, 2, 100, 2);
            break;
        }
        case 100:
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SYW0_NIGHT_AUTHOR.name));
            Syw0Wait(sequence, 13, 101, 5, 0);
            break;
        case 101:
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            Syw0Wait(sequence, 11, 102, 2);
            break;
        case 102:
            SuspendParticleEffect(4, syw0FireEffect);
            FreeParticleEffect(4, syw0FireEffect);
            syw0FireEffect = 0;
            SetCharaKill(SYW0_NIGHT, 1);
            EV_EnableCharacterDisplay(SYW0_RYO);
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
