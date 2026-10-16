#include "s2/areas/bkw0_sequence.hpp"
#include "s2/areas/bkw0_tables.hpp"
#include "s2/areas/area_arbt.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/npc.hpp"
namespace shenmue::s2::areas {
void Bkw0PlaceCrowd() {
    for (std::int32_t i = 0; i < 18; ++i) {
        const shen_name character = bkw0Characters[bkw0CrowdOffset + i];
        const Vector3i rotation{0, i < 9 ? 32768 : 0, 0};
        EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_CROWD_POSITIONS[i]);
        EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    }
}
void Bkw0TrainCrowd(std::int32_t includeSisters, std::int32_t fixed) {
    for (std::int32_t i = 0; i < 18; ++i) {
        const shen_name character = bkw0Characters[bkw0CrowdOffset + i];
        if (!includeSisters && ((character.name - BKW0_S01.name) & 0xfffeffffU) == 0)
            continue;
        EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 1);
        SetMotionFrameTime(scenFindMotionwork(character), 30, MotionRegion::LowerBody);
        const float frame = GetRandomNumberInRange(6);
        mtwkRequestMotion(scenFindMotionwork(character), character.name == BKW0_K04.name ? 7951 : 7956, MotionRegion::LowerBody);
        SetMotionLayerFrameRange(scenFindMotionwork(character), -1, frame, -1, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(character), 1, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(character), 71, fixed ? 0 : 2, MotionRegion::LowerBody);
    }
}
void Bkw0RandomizeCrowdPractice() {
    Vector3 position;
    EV_GetCharaPosition(BKW0_RYO, 5, &position, EventTransformFlags::World);
    for (std::int32_t i = 0; i < 18; ++i) {
        const shen_name character = bkw0Characters[bkw0CrowdOffset + i];
        if (((character.name - BKW0_S01.name) & 0xfffeffffU) == 0)
            continue;
        EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
        const std::int32_t motion = shenGetRandomInt32(6);
        if (motion >= 0 && motion <= 5) {
            mtwkRequestMotion(scenFindMotionwork(character), static_cast<std::uint16_t>(7944 + motion), MotionRegion::LowerBody);
            SetMotionLayerFrameRange(scenFindMotionwork(character), -1, -1, -1, MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(character), 1, MotionRegion::LowerBody);
        }
        UpdateMotionWorkWithPosition(scenFindMotionwork(character), &position, 0);
    }
}
void Bkw0IdleCrowd() {
    for (std::int32_t i = 0; i < 18; ++i) {
        const shen_name character = bkw0Characters[bkw0CrowdOffset + i];
        if (((character.name - BKW0_S01.name) & 0xfffeffffU) == 0)
            continue;
        EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
        SetMotionFrameTime(scenFindMotionwork(character), 30, MotionRegion::LowerBody);
        const float frame = GetRandomNumberInRange(6);
        mtwkRequestMotion(scenFindMotionwork(character), 7950, MotionRegion::LowerBody);
        SetMotionLayerFrameRange(scenFindMotionwork(character), -1, frame, -1, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(character), 1, MotionRegion::LowerBody);
    }
}
void Bkw0LookSequence(void *data) {
    CompiledSequenceWork &work = *static_cast<CompiledSequenceWork *>(data);
    const shen_name character = Bkw0SequenceValue<shen_name>(work, 0x138), target = Bkw0SequenceValue<shen_name>(work, 0x140);
    work.callbackFlag = 0;
    do {
        work.continueLoop = 0;
        if (!work.mode) {
            Bkw0SequenceValue(work, 0x1d0, std::int32_t{1});
            Bkw0Advance(work, 100);
        } else if (work.mode == 100) {
            const std::int32_t frames = Bkw0SequenceValue<std::int32_t>(work, 0x1d0);
            Bkw0SequenceValue(work, 0x1d0, frames - 1);
            if (frames == 1) {
                Vector3 position;
                EV_GetCharaPosition(target, 5, &position, EventTransformFlags::World);
                UpdateMotionWorkWithPosition(scenFindMotionwork(character), &position, 0);
                Bkw0SequenceValue(work, 0x1d0, std::int32_t{10});
            }
        } else
            UpdateCompiledSequenceWait(&work);
    } while (work.continueLoop);
}
void Bkw0ResetReflectors() {
    for (shen_name character : {BKW0_RYO, BKW0_LKN, BKW0_BKN}) {
        const Vector4 values{1, .3f, .3f, .3f};
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(character, false);
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 1);
        SetScenarioReflectorBoardValues(character, &values);
        EV_SetRefBoardParamsFor(character, 1);
    }
}
void Bkw0CreateSmoke() {
    for (std::int32_t i = 0; i < 2; ++i) {
        const Vector3 volume{.1f, 0, .1f}, velocity{0, .016f, 0}, variance{};
        const std::uint32_t effect = static_cast<std::uint32_t>(CreateSmokeEffect(BKW0_SMOKE_POSITIONS[i], volume, 40, .05f, shen_name{}, 0));
        bkw0Effects[i] = effect;
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
} // namespace shenmue::s2::areas
