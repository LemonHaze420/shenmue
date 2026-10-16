#include "s2/areas/knw0_authored.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/hand.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
// @verified - p
void Knw0PrepareAuthorCharacter(shen_name character) {
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 0, 0, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(character), 1, 0, 16);
}
// @verified - p
void Knw0PrepareAuthorCharacters() {
    Knw0PrepareAuthorCharacter(KNW0_AUTHOR_RYO);
    Knw0PrepareAuthorCharacter(KNW0_AUTHOR_KNK);
    for (shen_name prop : KNW0_AUTHOR_PROPS)
        EV_SetCharacterAlphaEnabled(prop, false);
}
// @verified - p
void Knw0StartAuthorLoop(Knw0AuthorWork *work) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Knw0AuthorLoopCallback, 4, 0x0b, 8, KNW0_AUTHOR_TASK);
    *static_cast<Knw0AuthorWork **>(HLib::GetTaskParameter(task)) = work;
    work->savedPostFx = static_cast<std::int32_t>(GetPostFxBankValue(3));
    SelectPostFxBank(3, 0);
    Knw0PrepareAuthorCharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->author.name));
    work->state = 2;
}
// @verified - p
void Knw0AuthorMainCallback(void *data) {
    Knw0AuthorWork *work = static_cast<Knw0AuthorWork *>(data);
    if (work->state == 1)
        Knw0StartAuthorLoop(work);
    else if (work->state == 3)
        HLib::DestroyTask(work->task);
}
// @verified - p
void Knw0AuthorLoopCallback(void *data) {
    Knw0AuthorWork &work = **static_cast<Knw0AuthorWork **>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work.author.name == KNW0_AUTHOR_FOURTH.name) {
        if (frame == 78)
            ProcessAicaCommand(0x1200a0, 0);
        else if (frame == 80)
            QueueExtraTelop("GGGG", 0);
        else if (frame == 81)
            EV_LoadAudio("0069A000", 0);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SelectPostFxBank(3, work.savedPostFx);
        if (work.author.name == KNW0_AUTHOR_THIRD.name) {
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(KNW0_AUTHOR_RYO), 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(KNW0_AUTHOR_RYO), 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 0);
        }
        mtwkSetExtraFlag(scenFindMotionwork(KNW0_AUTHOR_RYO), 6, 0);
        mtwkSetExtraFlag(scenFindMotionwork(KNW0_AUTHOR_KNK), 6, 0);
        work.state = 3;
        HLib::DestroyCurrentTask();
    }
}
} // namespace shenmue::s2::areas
