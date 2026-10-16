#include "s2/areas/area_dart.hpp"
#include "s2/debug.hpp"
#include "s2/event_voice.hpp"
#include "s2/motion_work.hpp"

namespace shenmue::s2::areas {

void DartsQueueOpponentVoice(shen_name opponent) {
    const std::uint32_t index = shenGetRandomInt32(3);
    switch (opponent.name) {
    case SHEN_NAME(FCJ_):
        QueueEventVoiceList(nullptr, &DART_FCJ_VOICES[4 * index], 200, 0, 0);
        break;
    case SHEN_NAME(HLN_):
        QueueEventVoiceList(nullptr, &DART_HLN_VOICES[2 * index], 200, 0, 0);
        break;
    case SHEN_NAME(JON_):
        QueueEventVoiceList(nullptr, &DART_JON_VOICES[2 * index], 200, 0, 0);
        break;
    case SHEN_NAME(HCY_):
        QueueEventVoiceList(nullptr, &DART_HCY_VOICES[2 * index], 200, 0, 0);
        break;
    default:
        QueueEventVoiceList(nullptr, &DART_DEFAULT_VOICES[4 * index], 200, 0, 0);
        break;
    }
}


void DartsStartChildTask(DartsEventWork *work, DartsChildTask kind) {
    constexpr std::array<HLib::TaskCallback, 4> callbacks{
        DartsDisplayCallback, DartsIncrementFramesCallback, DartsMonitorOpponentVoiceCallback, DartsOpponentVoiceCallback};
    const std::uint32_t index = static_cast<std::uint32_t>(kind);
    HLib::Task *&slot = index == 3 ? work->voiceTask : work->childTasks[index];
    if (slot) {
        EV_debug_log("DART\\eDART_et_darts.c", 3727, "Task Already Created %d\n", index);
        return;
    }
    slot = HLib::EnqueueTaskWithParameter(callbacks[index], 4, 0x0b, sizeof(DartsChildWork), DART_TASK);
    DartsChildWork *const child = static_cast<DartsChildWork *>(HLib::GetTaskParameter(slot));
    if (child) {
        child->rootTask = work->task;
        child->root = work;
        child->state = 0;
    }
}


void DartsMonitorOpponentVoiceCallback(void *taskData) {
    DartsChildWork *const child = static_cast<DartsChildWork *>(taskData);
    DartsEventWork *const work = child->root;
    if (work->scoreUiFlags) {
        if (work->standardBoard) {
            if (work->scoreTicker <= 0) {
                work->scoreTicker = 0;
            } else {
                work->scoreTicker = static_cast<std::int32_t>(100000U - 166U * static_cast<std::uint32_t>(work->opponentScore.impactIterations));
                child->root->opponentScore.impactIterations =
                    static_cast<std::int32_t>(static_cast<std::uint32_t>(child->root->opponentScore.impactIterations) + 1U);
            }
            DartsEventWork *const current = child->root;
            if (current->entry->opponentMode == 2 && !current->enemyTurn && static_cast<std::uint32_t>(current->scoreTicker) - 40000U < 10000U &&
                !current->voiceTask)
                DartsStartChildTask(current, DartsChildTask::OpponentVoice);
        }
    } else if (work->voiceTask) {
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(child->root->voiceTask);
        child->root->voiceTask = nullptr;
    }
}


void DartsOpponentVoiceCallback(void *taskData) {
    DartsQueueOpponentVoice(static_cast<DartsChildWork *>(taskData)->root->entry->enemy->name);
    HLib::SetupTaskDelay(800);
}
} // namespace shenmue::s2::areas
