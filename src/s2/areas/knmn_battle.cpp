#include "s2/areas/area_knmn.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/extra_telop.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/particle_effects.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
namespace {
template <std::size_t Count> std::int64_t KnmnCycleVoice(void *data, const std::int16_t (&keys)[Count]) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(data);
    const std::int16_t key = keys[index];
    index = static_cast<std::uint16_t>(index + 1);
    if (index >= Count)
        index = 0;
    return key;
}
std::int64_t KnmnCycleOpponentHitVoice(void *data) {
    const std::int16_t keys[]{122, 123, 124};
    return KnmnCycleVoice(data, keys);
}
std::int64_t KnmnCycleOpponentAttackVoice(void *data) {
    const std::int16_t keys[]{128, 129, 130, 131, 132};
    return KnmnCycleVoice(data, keys);
}
std::int64_t KnmnCyclePlayerAdviceVoice(void *data) {
    const std::int16_t keys[]{133, 134, 135};
    return KnmnCycleVoice(data, keys);
}
std::int64_t KnmnCyclePlayerMotionVoice(void *data) {
    const std::int16_t keys[]{125, 126, 127};
    return KnmnCycleVoice(data, keys);
}
std::int64_t KnmnCyclePlayerEvadeVoice(void *data) {
    const std::int16_t keys[]{119, 120, 121};
    return KnmnCycleVoice(data, keys);
}
std::int64_t KnmnCyclePlayerFirstReactionVoice(void *data) {
    const std::int16_t keys[]{113, 114, 115};
    return KnmnCycleVoice(data, keys);
}
std::int64_t KnmnCyclePlayerSecondReactionVoice(void *data) {
    const std::int16_t keys[]{116, 117, 118};
    return KnmnCycleVoice(data, keys);
}
std::int64_t KnmnResumeConditionalDialogues(void *) {
    ResumeBattleDialogue(knmnConditionalDialogues[0]);
    ResumeBattleDialogue(knmnConditionalDialogues[1]);
    return reinterpret_cast<std::int64_t>(ResumeBattleDialogue(knmnConditionalDialogues[2]));
}
std::int64_t KnmnInstallDialogueConditions(void *) {
    std::uint32_t id = AllocateBattleDialogue(8, -1, KNMN_KEM, shen_name{}, 1, 100, 1);
    AssignBattleDialogueCallback(id, KnmnCycleOpponentHitVoice);
    id = AllocateBattleDialogueCondition7(7, -1, KNMN_KEM, KNMN_RYO, 1, 100, 1);
    AssignBattleDialogueCallback(id, KnmnCycleOpponentAttackVoice);
    id = AllocateBattleDialogueCondition15(10, -1, KNMN_RYO, shen_name{}, 150, 100, 1);
    AssignBattleDialogueCallback(id, KnmnCyclePlayerAdviceVoice);
    id = AllocateMotionBattleDialogue(8, -1, KNMN_RYO, shen_name{}, 1, 1, 100, 1);
    AssignBattleDialogueCallback(id, KnmnCyclePlayerMotionVoice);
    id = AllocateBattleDialogueCondition17(10, -2, KNMN_KEM, KNMN_RYO, 1, 100, 1);
    AssignBattleDialogueCallback(id, KnmnResumeConditionalDialogues);
    knmnConditionalDialogues[0] = AllocateMotionBattleDialogue(8, -1, KNMN_RYO, shen_name{}, 283, 1, 100, 2);
    AssignBattleDialogueCallback(knmnConditionalDialogues[0], KnmnCyclePlayerEvadeVoice);
    SuspendBattleDialogue(knmnConditionalDialogues[0]);
    knmnConditionalDialogues[1] = AllocateReactionBattleDialogue(9, -1, KNMN_RYO, shen_name{}, 12, 5, 100, 2);
    AssignBattleDialogueCallback(knmnConditionalDialogues[1], KnmnCyclePlayerFirstReactionVoice);
    SuspendBattleDialogue(knmnConditionalDialogues[1]);
    knmnConditionalDialogues[2] = AllocateReactionBattleDialogue(9, -1, KNMN_RYO, shen_name{}, 48, 5, 100, 2);
    AssignBattleDialogueCallback(knmnConditionalDialogues[2], KnmnCyclePlayerSecondReactionVoice);
    return reinterpret_cast<std::int64_t>(SuspendBattleDialogue(knmnConditionalDialogues[2]));
}
std::int64_t KnmnRequestFirstBattleEnd(void *) {
    knmnEndRequested = 1;
    knmnEndTimer = 55;
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 1);
    return vfmDispatchControl(FightControlOp::QueuedAction, KNMN_KEM, 0x08000d1eU, 1);
}
std::int64_t KnmnRequestButokuBattleEnd(void *) {
    knmnEndRequested = 1;
    knmnEndTimer = 300;
    return 0;
}
void KnmnInstallEndConditions(std::int64_t (*callback)(void *)) {
    knmnEndRequested = 0;
    const std::uint32_t hit = AllocateBattleDialogue(0, -2, KNMN_RYO, shen_name{}, 20, 100, 1);
    AssignBattleDialogueCallback(hit, callback);
    const std::uint32_t timed = AllocatePeriodicBattleDialogue(0, -2, shen_name{}, shen_name{}, 1800, 100, 0);
    AssignBattleDialogueCallback(timed, callback);
}
void KnmnCreateLeafEmitter(float birthRate) {
    const Vector3 position{.x = 39.310001373291016f, .y = 105.80000305175781f, .z = 240.89999389648438f};
    const Vector3 volume{.x = 1.5f, .y = .02f, .z = 2}, velocity{.x = 0, .y = -.04f, .z = 0}, variance{.x = .012f, .y = -.01f, .z = .012f};
    knmnLeafEffect = static_cast<std::int32_t>(CreateFallingParticleEffect(position, volume, 80, birthRate, shen_name{0x400}, "MISC", "REDLEAF.PVR", -1));
    SetParticleEffectVelocity(3, knmnLeafEffect, velocity, variance);
    SetParticleEffectSize(3, knmnLeafEffect, .02f, .06f);
}
} // namespace
void KnmnInstallBattleDialogues() {
    knmnEndRequested = 0;
    const std::uint32_t id = AllocatePeriodicBattleDialogue(100, -2, shen_name{}, shen_name{}, 150, 100, 0);
    AssignBattleDialogueCallback(id, KnmnInstallDialogueConditions);
}
void KnmnInstallFirstEndConditions() {
    KnmnInstallEndConditions(KnmnRequestFirstBattleEnd);
}
void KnmnInstallButokuEndConditions() {
    KnmnInstallEndConditions(KnmnRequestButokuBattleEnd);
}
void KnmnWaitBattleRegularCallback(void *data) {
    KnmnGameWork &work = *static_cast<KnmnGameWork *>(data);
    if (!work.state) {
        const std::int32_t player = static_cast<std::int32_t>(GetFightCharacterFlag(1, FindCharacterByScenarioName(KNMN_RYO), 0));
        const std::int32_t opponent = static_cast<std::int32_t>(GetFightCharacterFlag(1, FindCharacterByScenarioName(KNMN_KEM), 0));
        if (!IsFightActive() || player <= 30 || opponent <= 30) {
            if (IsFightActive())
                check_dead_status(0);
            ++work.state;
        }
    } else if (work.state == 1 && !IsFightActive()) {
        work.state = 0;
        HLib::SetCurrentTaskTo(KnmnStopCallback);
    }
    if (work.battleFrames == 5400)
        check_dead_status(1);
    else
        ++work.battleFrames;
}
void KnmnWazaFrameEvents() {
    switch (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) {
    case 50:
        KnmnCreateLeafEmitter(.5f);
        SuspendParticleEffect(3, knmnLeafEffect);
        break;
    case 68:
        ProcessAicaCommand(0x1200a0U, 0);
        break;
    case 70:
        QueueExtraTelop("DDDDDD", 0);
        break;
    case 71:
        EV_LoadAudio("0026A000", 0);
        break;
    case 200:
        SetParticleEffectBirthRate(3, knmnLeafEffect, 0);
        break;
    case 420:
        ResumeParticleEffect(3, knmnLeafEffect);
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
