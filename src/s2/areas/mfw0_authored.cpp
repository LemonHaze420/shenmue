#include "s2/areas/area_mfw0.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2::areas {
extern std::uint32_t mfw0SplashEffect;
extern std::uint16_t mfw0SplashFrames;
namespace {
inline constexpr shen_name MFW0_SPLASH_AUTHOR{SHEN_NAME(AU_A)}, MFW0_FIRST_AUTHOR{SHEN_NAME(AU_1)}, MFW0_REPEAT_AUTHOR{SHEN_NAME(AU_B)};
void Mfw0AuthorCharacter(shen_name character) {
    EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
    SetScenarioReflectorBoardValues(character, &MFW0_REFLECTOR);
    EV_SetRefBoardParamsFor(character, 1);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 0, 8, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 1, 8, 16);
}
void Mfw0PrepareAuthorCharacters() {
    Mfw0AuthorCharacter(MFW0_RYO);
    for (shen_name prop : {MFW0_AB10, MFW0_AB11}) {
        EV_EnableCharacterDisplay(prop);
        EV_SetCharaControllerFlags(prop, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(prop, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(prop, false);
    }
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_RYO), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_TKR), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_TKR), 1, 1, 0);
    Mfw0AuthorCharacter(MFW0_TKR);
    const Vector3 zero{.x = 0, .y = 0, .z = 0};
    EV_AT_SetPos(MFW0_AB10, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_AT_SetPos(MFW0_AB10, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_AT_SetPos(MFW0_AB11, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
}
void Mfw0RestoreAuthorCharacters() {
    EV_EnableCharacterDisplay(MFW0_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(MFW0_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(MFW0_RYO, false);
    EV_DisableCharacterDisplay(MFW0_AB10);
    EV_DisableCharacterDisplay(MFW0_AB11);
    EV_EnableCharacterDisplay(MFW0_TKR);
    mtwkSetExtraFlag(scenFindMotionwork(MFW0_TKR), 6, 0);
    EV_SetCharacterAlphaEnabled(MFW0_TKR, false);
}
void Mfw0SplashCallback(void *) {
    if (mfw0SplashFrames == 20)
        SetParticleEffectBirthRate(1, mfw0SplashEffect, 0);
    else if (mfw0SplashFrames == 45) {
        mfw0SplashFrames = 0;
        SuspendParticleEffect(1, mfw0SplashEffect);
        FreeParticleEffect(1, mfw0SplashEffect);
        HLib::DestroyCurrentTask();
        return;
    } else if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        HLib::DestroyCurrentTask();
        ++mfw0SplashFrames;
        return;
    }
    ++mfw0SplashFrames;
}
void Mfw0CreateSplash() {
    const Vector3 position{.x = 160.66f, .y = 99.800003f, .z = 231.60001f}, volume{.x = .2f, .y = 0, .z = .2f};
    const Vector3 velocity{.x = .0074509755f, .y = .059999995f, .z = .016078435f}, variance{.x = .015294112f, .y = .041176476f, .z = .039607845f};
    HLib::EnqueueTaskWithoutParameter(Mfw0SplashCallback, 4, 0x0b, static_cast<std::int32_t>(MFW0_TASK.name));
    mfw0SplashEffect =
        static_cast<std::uint32_t>(CreateSplashEffect(position, volume, 1000, .02f, shen_name{0}, std::bit_cast<std::int32_t>(98.0f), nullptr, nullptr));
    SetParticleEffectColour(1, mfw0SplashEffect, 255, 175, 175, 255);
    SetParticleEffectPosition(1, mfw0SplashEffect, &position, &volume);
    SetParticleEffectVelocity(1, mfw0SplashEffect, velocity, variance);
    SetParticleEffectBirthRate(1, mfw0SplashEffect, .1f);
    SetParticleEffectSize(1, mfw0SplashEffect, .012549019418656826f, .012549019418656826f);
    ResumeParticleEffect(1, mfw0SplashEffect);
}
void Mfw0AuthorLoop(void *data) {
    Mfw0AuthorWork &work = *static_cast<Mfw0AuthorLoopWork *>(data)->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1) {
        ResetTrainingAuthorCharacter(MFW0_RYO);
        ResetTrainingAuthorCharacter(MFW0_TKR);
    }
    if (work.author.name == MFW0_SPLASH_AUTHOR.name) {
        if (frame == 358)
            Mfw0CreateSplash();
        SetScenarioCharacterDrawControl(MFW0_TKR, 0, 0);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SelectPostFxBank(3, work.postFxBank);
        if (work.author.name == MFW0_FIRST_AUTHOR.name || work.author.name == MFW0_REPEAT_AUTHOR.name) {
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_RYO), 1, reinterpret_cast<std::byte *>(MFW0_HAND_POSE), 0);
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_RYO), 0, reinterpret_cast<std::byte *>(MFW0_HAND_POSE), 0);
        }
        Mfw0RestoreAuthorCharacters();
        work.phase = 3;
        HLib::DestroyCurrentTask();
    }
}
void Mfw0QueueAuthorLoop(Mfw0AuthorWork &work) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Mfw0AuthorLoop, 4, 0x0b, sizeof(Mfw0AuthorLoopWork), MFW0_TASK);
    Mfw0AuthorLoopWork &loop = *static_cast<Mfw0AuthorLoopWork *>(HLib::GetTaskParameter(task));
    loop.parent = &work;
    work.postFxBank = static_cast<std::int32_t>(GetPostFxBankValue(3));
    SelectPostFxBank(3, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author.name));
    Mfw0PrepareAuthorCharacters();
    work.phase = 2;
    Mfw0AuthorLoop(&loop);
}
void Mfw0AuthorCallback(void *data) {
    Mfw0AuthorWork &work = *static_cast<Mfw0AuthorWork *>(data);
    if (work.phase == 1)
        Mfw0QueueAuthorLoop(work);
    else if (work.phase == 3) {
        HLib::DestroyTask(work.task);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
}
} // namespace
void Mfw0QueueAuthor(shen_name author) {
    Mfw0Nullsub();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Mfw0AuthorCallback, 4, 0x0b, sizeof(Mfw0AuthorWork), MFW0_TASK);
    Mfw0AuthorWork &work = *static_cast<Mfw0AuthorWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.phase = 1;
    work.author = author;
    SetPlayerControlEnabled(0);
    switch (author.name) {
    case SHEN_NAME(AU_8):
    case SHEN_NAME(AU_0):
    case SHEN_NAME(AU_6):
    case SHEN_NAME(AU_7):
    case SHEN_NAME(AU_9):
    case SHEN_NAME(AU_A):
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        break;
    default:
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        break;
    }
    if (work.phase == 1)
        Mfw0QueueAuthorLoop(work);
    else if (work.phase == 3) {
        HLib::DestroyTask(work.task);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
}
void Mfw0ResetReflectors() {
    const Vector4 values{.x = 1, .y = .3f, .z = .3f, .w = .3f};
    for (shen_name name : {MFW0_RYO, MFW0_TKR}) {
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, false);
        mtwkSetRequestedRootMode(scenFindMotionwork(name), 1);
        SetScenarioReflectorBoardValues(name, &values);
        EV_SetRefBoardParamsFor(name, 1);
    }
}
} // namespace shenmue::s2::areas
