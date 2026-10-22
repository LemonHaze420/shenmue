#include "s2/street_fight_conversation.hpp"
#include "s2/map_control.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/fight.hpp"
#include "s2/input.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/ui.hpp"
#include "s2/street_fight_event.hpp"
#include "s2/event_queued.hpp"
#include "s2/sound_program.hpp"
#include "s2/area_event.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/math.hpp"
#include "s2/sk_event.hpp"
#include "s2/motion_work.hpp"
#include "s2/street_fight_tables.hpp"
namespace shenmue::s2 {
std::int32_t scmnConversationSkipEnabled{};
StreetFightConversationSkipCallback scmnConversationSkipCallback{};
// @verified - p
void QueueScmnStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(ScmnStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void ScmnStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (scmnConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (scmnConversationSkipCallback)
            scmnConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        scmnConversationSkipEnabled = 0;
        scmnConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                scmnConversationSkipEnabled = 0;
                scmnConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfbrConversationSkipEnabled{};
StreetFightConversationSkipCallback sfbrConversationSkipCallback{};
// @verified - p
void QueueSfbrStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SfbrStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SfbrStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfbrConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfbrConversationSkipCallback)
            sfbrConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfbrConversationSkipEnabled = 0;
        sfbrConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfbrConversationSkipEnabled = 0;
                sfbrConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfjfConversationSkipEnabled{};
StreetFightConversationSkipCallback sfjfConversationSkipCallback{};
// @verified - p
void QueueSfjfStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SfjfStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SfjfStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfjfConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfjfConversationSkipCallback)
            sfjfConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfjfConversationSkipEnabled = 0;
        sfjfConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfjfConversationSkipEnabled = 0;
                sfjfConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfkkConversationSkipEnabled{};
StreetFightConversationSkipCallback sfkkConversationSkipCallback{};
// @verified - p
void QueueSfkkStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SfkkStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SfkkStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfkkConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfkkConversationSkipCallback)
            sfkkConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfkkConversationSkipEnabled = 0;
        sfkkConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfkkConversationSkipEnabled = 0;
                sfkkConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfkmConversationSkipEnabled{};
StreetFightConversationSkipCallback sfkmConversationSkipCallback{};
// @verified - p
void QueueSfkmStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SfkmStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SfkmStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfkmConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfkmConversationSkipCallback)
            sfkmConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfkmConversationSkipEnabled = 0;
        sfkmConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfkmConversationSkipEnabled = 0;
                sfkmConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfluConversationSkipEnabled{};
StreetFightConversationSkipCallback sfluConversationSkipCallback{};
// @verified - p
void QueueSfluStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SfluStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SfluStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfluConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfluConversationSkipCallback)
            sfluConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfluConversationSkipEnabled = 0;
        sfluConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfluConversationSkipEnabled = 0;
                sfluConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfm2ConversationSkipEnabled{};
StreetFightConversationSkipCallback sfm2ConversationSkipCallback{};
// @verified - p
void QueueSfm2StreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(Sfm2StreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void Sfm2StreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfm2ConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfm2ConversationSkipCallback)
            sfm2ConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfm2ConversationSkipEnabled = 0;
        sfm2ConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfm2ConversationSkipEnabled = 0;
                sfm2ConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfs2ConversationSkipEnabled{};
StreetFightConversationSkipCallback sfs2ConversationSkipCallback{};
// @verified - p
void QueueSfs2StreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(Sfs2StreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void Sfs2StreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfs2ConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfs2ConversationSkipCallback)
            sfs2ConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfs2ConversationSkipEnabled = 0;
        sfs2ConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfs2ConversationSkipEnabled = 0;
                sfs2ConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfsrConversationSkipEnabled{};
StreetFightConversationSkipCallback sfsrConversationSkipCallback{};
// @verified - p
void QueueSfsrStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SfsrStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SfsrStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfsrConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfsrConversationSkipCallback)
            sfsrConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfsrConversationSkipEnabled = 0;
        sfsrConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfsrConversationSkipEnabled = 0;
                sfsrConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sftcConversationSkipEnabled{};
StreetFightConversationSkipCallback sftcConversationSkipCallback{};
// @verified - p
void QueueSftcStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SftcStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SftcStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sftcConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sftcConversationSkipCallback)
            sftcConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sftcConversationSkipEnabled = 0;
        sftcConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sftcConversationSkipEnabled = 0;
                sftcConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sftsConversationSkipEnabled{};
StreetFightConversationSkipCallback sftsConversationSkipCallback{};
// @verified - p
void QueueSftsStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SftsStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SftsStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sftsConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sftsConversationSkipCallback)
            sftsConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sftsConversationSkipEnabled = 0;
        sftsConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sftsConversationSkipEnabled = 0;
                sftsConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t sfwfConversationSkipEnabled{};
StreetFightConversationSkipCallback sfwfConversationSkipCallback{};
// @verified - p
void QueueSfwfStreetFightConversation(StreetFightConversationProcedure procedure, void *arguments) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(SfwfStreetFightConversationCallback, 5, 0x0b, sizeof(StreetFightConversationWork), STREET_FIGHT_CONVERSATION_TASK);
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->procedure = procedure;
    work->arguments = arguments;
    work->skipped = 0;
    HLib::SetupTaskDelay(0);
}
// @verified - p
void SfwfStreetFightConversationCallback(void *parameter) {
    StreetFightConversationWork *const work = static_cast<StreetFightConversationWork *>(parameter);
    if (sfwfConversationSkipEnabled == 1 && EV_GetScreenFilmIsNotShowing() && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        if (sfwfConversationSkipCallback)
            sfwfConversationSkipCallback();
        scenCvsCtrlFree(nullptr);
        (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::MarkCurrentTaskForCleanup();
        sfwfConversationSkipEnabled = 0;
        sfwfConversationSkipCallback = nullptr;
        snapStopControl();
        return;
    }
    if (work->state) {
        if (work->state != 1) {
            if (work->state == 2) {
                (void)HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
                HLib::MarkCurrentTaskForCleanup();
                sfwfConversationSkipEnabled = 0;
                sfwfConversationSkipCallback = nullptr;
            }
            return;
        }
    } else {
        const std::int32_t frames = work->procedure(work->arguments);
        ++work->state;
        work->remainingFrames = static_cast<std::uint32_t>(frames);
        work->firstSkipPress = 0;
    }
    if (work->remainingFrames)
        --work->remainingFrames;
    if (work->remainingFrames) {
        if (GetFightModeFlags() & 0x10U) {
            if (work->firstSkipPress) {
                if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                    ++work->state;
                    work->skipped = 1;
                } else if (--work->skipPressFrames == 0) {
                    work->firstSkipPress = 0;
                }
            } else if (GetCurrentControlInput(INPUT_PRESSED) & 0x200U) {
                ++work->firstSkipPress;
                work->skipPressFrames = 20;
            }
        }
    } else {
        ++work->state;
    }
}
std::int32_t StartStreetFightBookieConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    (void)BeginSecondarySampleExecution(0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 200);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 225.0F, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 225.0F, 200);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf003, 0.0F, 1.0F, 0, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 199);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{0.0F, -0.050000001F, -1.299999952F}, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_CONVERSATION_BASE, 17.0F, 0);
    (void)EVT_AT_SetMot(fighter, 0x1adb, 0.0F, 0.80000001F, 0x40000000U, 0);
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 2.0F});
    EVT_AT_SetVoice("0100B038", 1, 50); // @text EN: "さあさあ、お立ち会い！" | JA: "さあさあ、お立ち会い！"
    EVT_AT_SetVoice("0100B067", 1, 132); // @text EN: "この強者に挑戦するヤツは\nいないか？" | JA: "この強者に挑戦するヤツは\nいないか？"
    EVT_AT_SetVoice("0100B068", 1, 240); // @text EN: "賭け金２０ドルの１本勝負だ！" | JA: "賭け金２０ドルの１本勝負だ！"
    EVT_AT_SetVoice("0100B069", 1, 330); // @text EN: "制限時間は３０秒。" | JA: "制限時間は３０秒。"
    EVT_AT_SetVoice("0100B070", 1, 412); // @text EN: "さあ、誰か勝負する\n腕自慢はいないか？" | JA: "さあ、誰か勝負する\n腕自慢はいないか？"
    EVT_AT_SetCam(g_currentMap.name == STREET_FIGHT_QSFB.name ? 8099 : 100, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 0);
    EVT_AT_SetCam(2004, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 120);
    EVT_AT_SetCam(2006, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 250);
    EVT_AT_SetCam(120, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 350);
    return 420;
}

// @verified - p
std::int32_t AcceptStreetFightConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0x1aef, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetVoice("0100A022", 1, 30); // @text EN: "I'll do it." | JA: "俺がやろう。"
    EVT_AT_SetVoice("0100B055", 1, 60); // @text EN: "よし。決まりだ。" | JA: "よし。決まりだ。"
    EVT_AT_SetPoint(STREET_FIGHT_BOOKIE, STREET_FIGHT_RYO, 5, 3, 0, 50, 156, 0);
    EVT_AT_SetPoint(fighter, STREET_FIGHT_RYO, 5, 1, 0, 70, 156, 0);
    EVT_AT_SetCam(130, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 0);
    EVT_AT_SetCam(141, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 28);
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 126.0F});
    return 156;
}

// @verified - p
std::int32_t DeclineStreetFightConversation(void *) {
    switch (shenGetRandomInt32(3)) {
    case 0: EVT_AT_SetVoice("0100A038", 1, 0); break; // @text EN: "No, I don't think so." | JA: "やめておこう…"
    case 1: EVT_AT_SetVoice("0100A039", 1, 0); break; // @text EN: "Maybe next time." | JA: "またにするか…"
    case 2: EVT_AT_SetVoice("0100A040", 1, 0); break; // @text EN: "I might try next time." | JA: "また今度にするか…"
    default:
        break;
    }
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 120.0F});
    return 150;
}

// @verified - p
std::int32_t InsufficientStreetFightFundsConversation(void *) {
    (void)PATI_StartMoneyCount(0, 60);
    EVT_AT_SetVoice("0100A037", 1, 0); // @text EN: "I'd like to participate\nbut I don't have enough money." | JA: "参加したいけど、\n金がないか…"
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 120.0F});
    return 150;
}

// @verified - p
std::int32_t StreetFightVictoryConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 180.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_VICTORY_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_CONVERSATION_BASE, 17.0F, 0);
    (void)EVT_AT_SetMot(fighter, 0x1ac3, 0.0F, 1.0F, 0x50000000U, 0);
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5.0F});
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(task, 6);
    SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
    message->message = STREET_FIGHT_DEBUG_VICTORY;
    message->frames = 60;
    EVT_AT_SetCam(2007, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 5);
    EVT_AT_SetCam(210, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 118);
    return 120;
}

// @verified - p
std::int32_t StreetFightNextOpponentConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 180.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_VICTORY_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_CONVERSATION_BASE, 17.0F, 0);
    (void)EVT_AT_SetMot(fighter, 0x1adb, 0.0F, 0.80000001F, 0x50000000U, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(firstTask, 1);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_NEXT_OPPONENT;
    first->frames = 59;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_REMATCH;
    second->frames = 60;
    const char *rewardMessage = nullptr;
    switch (work->completedOpponents) {
    case 1:
        rewardMessage = STREET_FIGHT_DEBUG_PRIZE_40;
        break;
    case 2:
        rewardMessage = STREET_FIGHT_DEBUG_PRIZE_60;
        break;
    case 3:
        rewardMessage = STREET_FIGHT_DEBUG_PRIZE_80;
        break;
    case 4:
        rewardMessage = STREET_FIGHT_DEBUG_PRIZE_100;
        break;
    case 5:
        rewardMessage = STREET_FIGHT_DEBUG_PRIZE_120;
        break;
    default:
        break;
    }
    if (rewardMessage) {
        HLib::Task *const rewardTask =
            HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
        HLib::SetupTimedTask(rewardTask, 120);
        SkEventMessageWork *const reward = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(rewardTask));
        reward->message = rewardMessage;
        reward->frames = 60;
    }
    return 180;
}

// @verified - p
std::int32_t StreetFightDefeatConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 180.0F, 1);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 1);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{-0.98799998F, 0.0F, -1.2F}, 1);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_CONVERSATION_BASE, 17.0F, 1);
    (void)EVT_AT_SetMot(fighter, 0x1af2, 0.0F, 1.0F, 0x40000000U, 0);
    SetMotionLayerEndFrame(scenFindMotionwork(fighter), 1.0F, MotionRegion::LowerBody);
    (void)EVT_AT_SetMot(fighter, 0x1af2, 1.0F, 1.0F, 0x40000000U, 60);
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5.0F});
    EVT_AT_SetVoice("0100B077", 1, 35); // @text EN: "いやあ、惜しかったなあ、\n次はがんばんな。" | JA: "いやあ、惜しかったなあ、\n次はがんばんな。"
    EVT_AT_SetCam(2008, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 0);
    EVT_AT_SetCam(310, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 300);
    (void)PATI_StartMoneyCount(static_cast<std::uint32_t>(-20), 180);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 250.0F});
    return 290;
}

// @verified - p
std::int32_t StreetFightWithdrawalConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 180.0F, 1);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 1);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{-0.98799998F, 0.0F, -1.2F}, 1);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_CONVERSATION_BASE, 17.0F, 1);
    (void)EVT_AT_SetMot(fighter, 0x1af2, 0.0F, 1.0F, 0x40000000U, 0);
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5.0F});
    EVT_AT_SetVoice("0100B078", 1, 35); // @text EN: "引き分けじゃ金は取れねえな。" | JA: "引き分けじゃ金は取れねえな。"
    EVT_AT_SetVoice("0100B079", 1, 140); // @text EN: "また今度挑戦してくれ。" | JA: "また今度挑戦してくれ。"
    EVT_AT_SetCam(107, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 0);
    EVT_AT_SetCam(310, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 300);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 250.0F});
    return 280;
}

// @verified - p
std::int32_t StreetFightContinueConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 180.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, Vector3{0.0F, -0.050000001F, -1.3F}, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 17.0F, 60);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{0.0F, -10000.0F, -1.299999952F}, 0);
    if (work->completedOpponents < streetFightMaximumOpponents) {
        EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{-3.3F, -0.5F, -5.5F}, 60);
        EVT_AT_SetAngY(fighter, STREET_FIGHT_CONVERSATION_BASE, 225.0F, 60);
    }
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_ACCEPT;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 30);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_WAIT;
    second->frames = 30;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 60);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_INVITATION;
    third->frames = 90;
    HLib::Task *const fourthTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(fourthTask, 150);
    SkEventMessageWork *const fourth = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(fourthTask));
    fourth->message = STREET_FIGHT_DEBUG_CHALLENGE;
    fourth->frames = 60;
    EVT_AT_SetCam(2006, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 60);
    EVT_AT_SetCam(120, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 150);
    return 210;
}

// @verified - p
std::int32_t StreetFightCompletedConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, Vector3{0.0F, -0.0500000007F, -1.299999952F}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 17.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{0.0F, -10000.0F, -1.299999952F}, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_CALL;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_NO_OPPONENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 5);
    (void)PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work->completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 180.0F});
    return 240;
}

// @verified - p
std::int32_t StreetFightNextMatchConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{-3.299999952F, -0.5F, -5.5F}, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_CONVERSATION_BASE, 225.0F, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
    message->message = STREET_FIGHT_DEBUG_OPPONENT_ACCEPT;
    message->frames = 60;
    EVT_AT_SetVoice("0100B071", 1, 60); // @text EN: "よし、決まりだ。" | JA: "よし、決まりだ。"
    EVT_AT_SetCam(130, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 0);
    return 120;
}

// @verified - p
std::int32_t StreetFightStopConversation(void *arguments) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(arguments);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, work->playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, STREET_FIGHT_CONVERSATION_BASE, 180.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0.0F, 1.0F, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, work->bookiePosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BOOKIE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0);
    (void)EVT_AT_SetMot(STREET_FIGHT_BOOKIE, 0x1aed, 0.0F, 1.1F, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_CONVERSATION_BASE, Vector3{0.0F, -10000.0F, -1.299999952F}, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_STOP;
    first->frames = 60;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_AGREEMENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_STOP_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, STREET_FIGHT_CONVERSATION_BASE, STREET_FIGHT_CONVERSATION_BASE, 0.0F, 0, 5);
    (void)PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work->completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{1, 40, 0, 0, 0, 0, 255, 0, 0, 0, 180.0F});
    return 240;
}

} // namespace shenmue::s2
