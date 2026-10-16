#include "s2/areas/bkw0_sequence.hpp"
#include "s2/areas/bkw0_tables.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/training_seqp.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name BKW0_AUTHOR_PROP_FIRST{SHEN_NAME(aa48)}, BKW0_AUTHOR_PROP_SECOND{SHEN_NAME(aa49)};
void Bkw0PrepareAuthorCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(BKW0_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(BKW0_RYO, false);
    EV_SetCharacterAlphaEnabled(BKW0_AUTHOR_PROP_FIRST, false);
    EV_SetCharacterAlphaEnabled(BKW0_AUTHOR_PROP_SECOND, false);
    for (const shen_name *character = BKW0_AUTHOR_CHARACTERS; character->name != EVENT_CHARACTER_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
void Bkw0AuthorLoop(void *data) {
    Bkw0AuthorWork &work = **static_cast<Bkw0AuthorWork **>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work.author.name == BKW0_AUTHOR_SUCCESS.name) {
        if (frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
            spmFadeOut();
        switch (frame) {
        case 48:
            ProcessAicaCommand(0x1200a0, 0);
            break;
        case 50:
            QueueExtraTelop("FFFF", 0);
            break;
        case 51:
            SetNewEwzoTypeForMoveLearned(0, 3, 15);
            break;
        case 121:
            SetNewEwzoTypeForMoveLearned(1, 0, 0);
            break;
        case 141:
            EV_LoadAudio("0068A000", 0);
            break;
        default:
            break;
        }
    } else if (work.author.name == BKW0_AUTHOR_FAILURE.name) {
        if (frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
            spmFadeOut();
    }
    if (work.author.name == BKW0_AUTHOR_INTRO.name) {
        if (frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        switch (frame) {
        case 290:
            Bkw0RandomizeCrowdPractice();
            break;
        case 340:
            Bkw0IdleCrowd();
            break;
        case 350:
            for (std::int32_t i = 0; i < 18; ++i) {
                const shen_name character = bkw0Characters[bkw0CrowdOffset + i];
                if (((character.name - BKW0_S01.name) & 0xfffeffffU) != 0)
                    UpdateMotionWorkWithPosition(scenFindMotionwork(character), nullptr, 0);
            }
            break;
        case 560:
            Bkw0TrainCrowd(0, 0);
            break;
        default:
            break;
        }
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SelectPostFxBank(3, work.postFxBank);
        mtwkSetExtraFlag(scenFindMotionwork(BKW0_RYO), 6, 0);
        for (const shen_name *character = BKW0_AUTHOR_CHARACTERS; character->name != EVENT_CHARACTER_INVALID.name; ++character)
            mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        work.phase = 3;
        HLib::SetupTimedTask(work.task, 30);
        HLib::DestroyCurrentTask();
    }
}
void Bkw0StartAuthorLoop(Bkw0AuthorWork &work) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Bkw0AuthorLoop, 4, 0x0b, sizeof(Bkw0AuthorWork *), BKW0_TASK);
    *static_cast<Bkw0AuthorWork **>(HLib::GetTaskParameter(task)) = &work;
    EV_LoadAudio("0068A000", 0);
    work.postFxBank = static_cast<std::int32_t>(GetPostFxBankValue(3));
    SelectPostFxBank(3, 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Bkw0PrepareAuthorCharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author.name));
    work.phase = 2;
}
} // namespace
void Bkw0AuthorCallback(void *data) {
    Bkw0AuthorWork &work = *static_cast<Bkw0AuthorWork *>(data);
    if (work.phase == 1)
        Bkw0StartAuthorLoop(work);
    else if (work.phase == 3)
        HLib::DestroyTask(work.task);
}
void Bkw0QueueAuthor(shen_name author) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Bkw0AuthorCallback, 4, 0x0b, sizeof(Bkw0AuthorWork), BKW0_TASK);
    Bkw0AuthorWork &work = *static_cast<Bkw0AuthorWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.phase = 1;
    work.author = author;
    SetPlayerControlEnabled(0);
}
void Bkw0FinalThrowSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    const std::int32_t finish = Bkw0SequenceValue<std::int32_t>(sequence, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0Advance(sequence, 103);
            break;
        case 101:
        case 102:
            return;
        case 103: {
            const bool success = (std::to_integer<std::uint8_t>(work.fightResults[8]) & 0x20) != 0;
            if (success) {
                work.throwDirection = 0;
                work.learnedMove = 4;
            }
            ResetTrainingAuthorCharacter(BKW0_RYO);
            ResetTrainingAuthorCharacter(BKW0_LKN);
            ResetTrainingAuthorCharacter(BKW0_BKN);
            figpAssignModeFor(BKW0_RYO, 0);
            figpAssignModeFor(BKW0_LKN, 0);
            if (success)
                EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
            Bkw0QueueAuthor(success ? BKW0_AUTHOR_SUCCESS : BKW0_AUTHOR_FAILURE);
            Bkw0Wait(sequence, 13, 134, 5, 0);
            break;
        }
        case 134:
            Bkw0SequenceValue(sequence, 0x88, finish);
            sequence.reserved1a0 = 1;
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            Bkw0ResetReflectors();
            Bkw0Wait(sequence, 2, 142, 2);
            break;
        case 142:
            Bkw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
