#include "s2/areas/area_titl.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include "s2/minigame_random.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
void InitializeTitleSeed(TitleWork *work) {
    SetMinigameSeed(8);
    work->advancedPatternDivisor = 3;
    work->simplePatterns = 3;
    work->doublePatterns = 2;
    work->triplePatterns = 2;
    work->successes = 0;
    work->frame = 0;
    work->extraTiming = 0;
    work->replayAudio = 0;
    work->pattern[0] = 0;
    work->step = 0;
    work->lives = 3;
    work->timingFrames = 31;
    work->stepScore = 1000;
    work->indicators[0] = 0;
    work->indicators[1] = 0;
    work->indicators[2] = 0;
}
void BuildTitlePattern(TitleWork *work) {
    const std::uint16_t mode = work->pattern[0];
    std::int64_t selection;
    if (mode == 0 || mode == 3) {
        selection = static_cast<std::int64_t>(IncrementMinigameRandFloat() * work->simplePatterns);
        switch (selection) {
        case 0:
            work->pattern[1] = 0;
            work->pattern[2] = 3;
            work->pattern[3] = 6;
            break;
        case 1:
            work->pattern[1] = 6;
            work->pattern[2] = 3;
            work->pattern[3] = 0;
            break;
        case 2:
            work->pattern[1] = 3;
            work->pattern[2] = 7;
            work->pattern[3] = 0;
            break;
        default:
            break;
        }
    } else if (mode == 1 || mode == 2) {
        selection = static_cast<std::int64_t>(IncrementMinigameRandFloat() * work->doublePatterns);
        switch (selection) {
        case 0:
            work->pattern[1] = 6;
            work->pattern[2] = 0;
            work->pattern[3] = 3;
            break;
        case 1:
            work->pattern[1] = 3;
            work->pattern[2] = 7;
            work->pattern[3] = 0;
            break;
        case 2:
            work->pattern[1] = 3;
            work->pattern[2] = 0;
            work->pattern[3] = 7;
            break;
        case 3:
            work->pattern[1] = 0;
            work->pattern[2] = 6;
            work->pattern[3] = 3;
            break;
        case 4:
            work->pattern[1] = 7;
            work->pattern[2] = 3;
            work->pattern[3] = 0;
            break;
        case 5:
            work->pattern[1] = 0;
            work->pattern[2] = 3;
            work->pattern[3] = 6;
            break;
        default:
            break;
        }
    } else if (mode == 4) {
        selection = static_cast<std::int64_t>(IncrementMinigameRandFloat() * work->triplePatterns);
        switch (selection) {
        case 0:
            work->pattern[1] = 3;
            work->pattern[2] = 6;
            work->pattern[3] = 0;
            break;
        case 1:
            work->pattern[1] = 0;
            work->pattern[2] = 6;
            work->pattern[3] = 3;
            break;
        case 2:
            work->pattern[1] = 3;
            work->pattern[2] = 0;
            work->pattern[3] = 6;
            break;
        case 3:
            work->pattern[1] = 0;
            work->pattern[2] = 3;
            work->pattern[3] = 7;
            break;
        case 4:
            work->pattern[1] = 7;
            work->pattern[2] = 0;
            work->pattern[3] = 0;
            break;
        case 5:
            work->pattern[1] = 6;
            work->pattern[2] = 3;
            work->pattern[3] = 3;
            break;
        default:
            break;
        }
    }
}
bool AdvanceTitlePatternStep(TitleWork *work) {
    bool result = false;
    if (work->step == 0) {
        if (work->pattern[0] == 0 || work->pattern[0] == 3)
            result = true;
        else if (work->pattern[0] == 1) {
            work->step = 2;
            return true;
        }
    }
    if (work->step == 1 && work->pattern[0] == 2)
        result = true;
    if (work->step == 2)
        result = work->pattern[0] == 4;
    ++work->step;
    return result;
}
void CalculateTitleTimingBonus(TitleWork *work) {
    float bonus = 0;
    if (work->inputFrames != static_cast<std::uint32_t>(work->timingFrames)) {
        bonus = static_cast<float>(work->stepScore) * 0.1f;
        if (work->inputFrames != 0) {
            bonus = (static_cast<float>(static_cast<std::uint32_t>(work->timingFrames) - work->inputFrames + 1) /
                        static_cast<float>(static_cast<std::uint32_t>(work->timingFrames))) *
                bonus;
        }
    }
    work->timingBonus[work->step] = bonus;
}
bool PollTitleButtons(TitleWork *work) {
    const std::uint16_t buttons = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_NEW_PRESSES));
    std::int16_t count = 0;
    if (buttons) {
        const bool freeInput = GetIsFreeInputType(-1);
        const std::uint16_t b = freeInput ? 0x200 : 0x400;
        const std::uint16_t a = freeInput ? 0x400 : 0x200;
        if (buttons & b) {
            work->buttons |= 0x100;
            ++count;
        }
        if (buttons & a) {
            work->buttons |= 0x200;
            ++count;
        }
        if (buttons & 4) {
            work->buttons |= 0x400;
            ++count;
        }
        if (work->pattern[0] == 1) {
            if (work->buttons) {
                if (work->firstComboFrame || count > 1)
                    return true;
                work->firstComboFrame = static_cast<std::int32_t>(work->inputFrames);
            }
        } else if (work->buttons)
            return true;
    }
    if ((work->firstComboFrame && work->inputFrames - static_cast<std::uint32_t>(work->firstComboFrame) >= 5) || work->inputFrames >= work->inputWindow) {
        work->buttons = 40960;
        return true;
    }
    const std::uint32_t phase = work->inputFrames % 5;
    if (phase <= 2) {
        if (phase == 2)
            Loop3DSoundEffect(0xab);
        if (work->requireB)
            work->indicators[0] = 1;
        if (work->requireA)
            work->indicators[1] = 1;
        if (work->requireX)
            work->indicators[2] = 1;
    } else {
        work->indicators[0] = 0;
        work->indicators[1] = 0;
        work->indicators[2] = 0;
    }
    ++work->inputFrames;
    return false;
}
bool ConsumeTitleButtons(TitleWork *work) {
    bool result = false;
    if (work) {
        work->indicators[0] = 0;
        work->indicators[1] = 0;
        work->indicators[2] = 0;
        work->skipEffect = 0;
        const std::uint32_t buttons = work->buttons;
        if (buttons == 40960 || !buttons)
            work->skipEffect = 1;
        else {
            std::int16_t count = 0;
            if (work->requireB && (buttons & 0x100))
                ++count;
            if (work->requireA && (buttons & 0x200))
                ++count;
            if (work->requireX && (buttons & 0x400))
                ++count;
            if ((work->requireB || !(buttons & 0x100)) && (work->requireA || !(buttons & 0x200)) && (work->requireX || !(buttons & 0x400))) {
                result = work->pattern[0] == 1 ? count == 2 : count == 1;
            }
        }
    }
    if (g_forcePlayerGodmode) {
        result = true;
        work->skipEffect = 0;
    } else if (!result) {
        Loop3DSoundEffect(0xa00ab);
        return false;
    }
    const char *voices[]{"0115A012", "0115A013", "0115A014", "0115A015", "0115A016"};
    EV_PlayDialogue(voices[shenGetRandomInt32(5)]);
    Loop3DSoundEffect(0x900ab);
    return result;
}
void TitleMittIdle(void *data) {
    static_cast<TitleMittWork *>(data)->phase = 0;
}
void TitleMittUp(void *data) {
    TitleMittWork *work = static_cast<TitleMittWork *>(data);
    if (!work->phase) {
        work->frame = 0;
        work->phase = 1;
        work->raised = 1;
        Loop3DSoundEffect(0x18b);
    }
    std::uint32_t angle;
    if (work->frame >= work->duration) {
        if (work->frame >= work->duration + 8) {
            angle = 0;
            HLib::AssignCallbackForTask(work->task, TitleMittIdle);
        } else
            angle = ((work->frame - work->duration) & 2) ? 0 : 65354;
    } else {
        const float degrees = static_cast<float>(45 * (static_cast<std::int32_t>(work->frame) - work->duration - 1)) / static_cast<float>(work->duration + 1);
        angle = static_cast<std::uint16_t>(static_cast<std::int32_t>((degrees * 8192.0f) / 45.0f));
    }
    EV_AT_SetAngY(work->character, EventTransformFlags::AllAxes, Vector3i{static_cast<std::int32_t>(angle), 0, 0});
    ++work->frame;
}
void TitleMittDown(void *data) {
    TitleMittWork *work = static_cast<TitleMittWork *>(data);
    work->duration = 60;
    if (!work->phase) {
        work->frame = 0;
        work->phase = 1;
        work->raised = 0;
    }
    std::uint32_t angle;
    if (work->frame > 61) {
        angle = 57344;
        HLib::AssignCallbackForTask(work->task, TitleMittIdle);
    } else
        angle = static_cast<std::uint16_t>(static_cast<std::int32_t>((((static_cast<float>(-45 * work->frame) / 61.0f) * 8192.0f) / 45.0f)));
    EV_AT_SetAngY(work->character, EventTransformFlags::AllAxes, Vector3i{static_cast<std::int32_t>(angle), 0, 0});
    ++work->frame;
}
void TitleMittFake(void *data) {
    TitleMittWork *work = static_cast<TitleMittWork *>(data);
    if (!work->phase) {
        work->frame = 0;
        work->phase = 1;
        work->raised = 0;
        Loop3DSoundEffect(0x18b);
    }
    const std::uint32_t length = work->duration + 1;
    std::uint32_t angle;
    if (work->frame > length) {
        angle = 57344;
        HLib::AssignCallbackForTask(work->task, TitleMittIdle);
    } else {
        const std::int32_t numerator = work->frame > (length >> 1) ? -45 * work->frame : 45 * (static_cast<std::int32_t>(work->frame) - work->duration - 1);
        angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(((static_cast<float>(numerator) / static_cast<float>(length)) * 8192.0f) / 45.0f));
    }
    EV_AT_SetAngY(work->character, EventTransformFlags::AllAxes, Vector3i{static_cast<std::int32_t>(angle), 0, 0});
    ++work->frame;
}
void TitleMittKnockout(void *data) {
    TitleMittWork *work = static_cast<TitleMittWork *>(data);
    if (!work->phase) {
        work->frame = 0;
        work->phase = 1;
        work->raised = 0;
        Loop3DSoundEffect(0x18c);
        Loop3DSoundEffect(0x189);
    }
    std::uint32_t angle;
    if (work->frame < 2)
        angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(((static_cast<float>(-45 * work->frame) * 0.5f) * 8192.0f) / 45.0f));
    else if (work->frame < 10)
        angle = ((work->frame & 2) != 0) ? 57527 : 57344;
    else {
        angle = 57344;
        HLib::AssignCallbackForTask(work->task, TitleMittIdle);
    }
    EV_AT_SetAngY(work->character, EventTransformFlags::AllAxes, Vector3i{static_cast<std::int32_t>(angle), 0, 0});
    ++work->frame;
}
void TitleMittSleep(void *data) {
    TitleMittWork *work = static_cast<TitleMittWork *>(data);
    if (!work->phase) {
        work->frame = 0;
        work->raised = 0;
        work->phase = 0;
    }
    EV_AT_SetAngY(work->character, EventTransformFlags::AllAxes, Vector3i{57344, 0, 0});
}
void ApplyTitleMittBehavior(TitleMittWork *mitt, TitleMittMode mode, std::uint16_t duration, std::int32_t delay) {
    mitt->duration = duration;
    mitt->phase = 0;
    switch (mode) {
    case TitleMittMode::Up:
        HLib::AssignCallbackForTask(mitt->task, TitleMittUp);
        break;
    case TitleMittMode::Down:
        HLib::AssignCallbackForTask(mitt->task, TitleMittDown);
        break;
    case TitleMittMode::Fake:
        HLib::AssignCallbackForTask(mitt->task, TitleMittFake);
        break;
    case TitleMittMode::Knockout:
        HLib::AssignCallbackForTask(mitt->task, TitleMittKnockout);
        break;
    case TitleMittMode::Sleep:
        HLib::AssignCallbackForTask(mitt->task, TitleMittSleep);
        break;
    default:
        break;
    }
    if (delay)
        HLib::SetupTimedTask(mitt->task, delay);
}
namespace {
TitleMittWork *MittForLane(TitleWork *work, std::uint16_t lane) {
    switch (lane) {
    case 0:
    case 1:
        return work->mitts[2];
    case 3:
    case 4:
        return work->mitts[0];
    case 6:
    case 7:
        return work->mitts[1];
    }
    return nullptr;
}
} // namespace
void ConfigureTitleMitts(TitleWork *work) {
    std::int32_t delay = 1;
    TitleMittWork *mitt;
    if (work->pattern[0] == 3) {
        mitt = MittForLane(work, work->pattern[3]);
        if (mitt)
            ApplyTitleMittBehavior(mitt, TitleMittMode::Fake, static_cast<std::uint16_t>(work->timingFrames), 0);
        delay = static_cast<std::uint32_t>(work->timingFrames) >> 1;
    }
    mitt = MittForLane(work, work->pattern[1]);
    if (mitt)
        ApplyTitleMittBehavior(mitt, TitleMittMode::Up, static_cast<std::uint16_t>(work->timingFrames), delay);
    const std::uint16_t mode = work->pattern[0];
    if (((mode - 1) & 0xfffc) == 0 && mode != 3) {
        if (mode != 1)
            delay = static_cast<std::uint32_t>(work->timingFrames) >> 1;
        mitt = MittForLane(work, work->pattern[2]);
        if (mitt)
            ApplyTitleMittBehavior(mitt, TitleMittMode::Up, static_cast<std::uint16_t>(work->timingFrames), delay);
    }
    if (mode == 4) {
        mitt = MittForLane(work, work->pattern[3]);
        if (mitt)
            ApplyTitleMittBehavior(mitt, TitleMittMode::Up, static_cast<std::uint16_t>(work->timingFrames), work->timingFrames);
    }
}
void UpdateTitleScoreDisplay(TitleWork *work) {
    scenCopyCharaToGame(QTE_GAME_TITLE);
    for (std::uint32_t i = 0; i < 3; ++i)
        if (work->indicators[i])
            scenGetCharaGameNode(QTE_GAME_TITLE, static_cast<std::uint16_t>(292 + i), &TITL_INDICATOR_POSITIONS[i], 1, &TITL_INDICATOR_SCALE);
    SetPosRotForGame(QTE_GAME_TITLE, 256, &QTE_TITLE_DIGIT_STEP, 99999999, 0);
    UpdateGameNodeFlags(QTE_GAME_TITLE, *work->scoreSources[0], &QTE_TITLE_SCORE_POSITIONS[0], 1, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, *work->scoreSources[1], &QTE_TITLE_SCORE_POSITIONS[1], 1, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, work->scoreSources[2] ? *work->scoreSources[2] : 0, &QTE_TITLE_SCORE_POSITIONS[2], 1, nullptr);
}
void TitleScoreFlashCallback(void *data) {
    TitleScoreFlashWork *work = static_cast<TitleScoreFlashWork *>(data);
    scenCopyCharaToGame(QTE_GAME_TITLE);
    SetPosRotForGame(QTE_GAME_TITLE, 256, &QTE_TITLE_DIGIT_STEP, 99999999, 0);
    if (work->draw) {
        if (work->lanes[0])
            UpdateGameNodeFlags(QTE_GAME_TITLE, work->title->score, &QTE_TITLE_SCORE_POSITIONS[0], 1, nullptr);
        if (work->lanes[1])
            UpdateGameNodeFlags(QTE_GAME_TITLE, work->title->bestScore, &QTE_TITLE_SCORE_POSITIONS[1], 1, nullptr);
        if (work->lanes[2])
            UpdateGameNodeFlags(QTE_GAME_TITLE, work->title->previousScore, &QTE_TITLE_SCORE_POSITIONS[2], 1, nullptr);
    }
    HLib::DestroyCurrentTask();
}
void QueueTitleScoreFlashes(TitleWork *work, std::uint8_t lanes) {
    for (std::int32_t i = 0; i < 8; ++i) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(TitleScoreFlashCallback, 6, 0x0b, sizeof(TitleScoreFlashWork), AREA_EVENT_TASK);
        if (i)
            HLib::SetupTimedTask(task, 20 * i);
        TitleScoreFlashWork *flash = static_cast<TitleScoreFlashWork *>(HLib::GetTaskParameter(task));
        flash->title = work;
        flash->draw = 1;
        if (i & 1) {
            flash->lanes[0] = 1;
            flash->lanes[1] = 1;
            flash->lanes[2] = 1;
        } else {
            flash->lanes[1] = 0;
            flash->lanes[2] = 0;
            flash->lanes[0] = (lanes & 1) != 0;
            flash->lanes[1] = (lanes & 2) != 0;
            if (lanes & 4)
                flash->lanes[2] = 1;
        }
    }
}
void ShowTitleLives(TitleWork *work) {
    const std::uint32_t index = work->lives >= 3 ? 2 : work->lives >= 2 ? 1 : 0;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepObjectDisplayWork *display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    *display = {.cleanup = 1, .character = TITL_LIVES[index], .mode = 0, .delay = -1};
    if (index) {
        for (std::uint32_t i = 0; i < 5; ++i) {
            task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), AREA_EVENT_TASK);
            RegisterEventTask(task);
            display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
            *display = {
                .cleanup = 1, .character = TITL_LIVES[index - 1], .mode = static_cast<std::int32_t>(i & 1), .delay = i ? static_cast<float>(20 * i) : -1.0f};
        }
        task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), AREA_EVENT_TASK);
        RegisterEventTask(task);
        display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
        *display = {.cleanup = 1, .character = TITL_LIVES[index - 1], .mode = 1, .delay = 100};
    }
}
void SetTitleLaneCamera(std::uint16_t lane) {
    const std::int32_t random = shenGetRandomInt32(6);
    std::int32_t camera;
    shen_name target = QTE_GAME_TITLE;
    switch (lane) {
    case 0:
    case 1:
        camera = 7350 + random;
        break;
    case 3:
    case 4:
        camera = 7380 + (random == 3 ? 4 : random >= 4 ? 3 : random);
        break;
    case 6:
        camera = 7360 + random;
        break;
    case 7:
        camera = 7370 + random;
        break;
    case 9:
        camera = 7390 + random % 3;
        target = TITL_RYO;
        break;
    default:
        return;
    }
    EVT_AT_SetCam(camera, target, target, 0, 0, -1);
}
void PlayTitleStepMotion(TitleWork *work, bool success) {
    const std::uint16_t lane = work->pattern[work->step + 1];
    switch (lane) {
    case 0:
    case 1:
        EVT_AT_SetPosAng(TITL_RYO, QTE_GAME_TITLE, 0.14f, 1.36f, 0, -1);
        break;
    case 3:
    case 4:
        EVT_AT_SetPosAng(TITL_RYO, QTE_GAME_TITLE, -0.30000001f, 1.3200001f, 0, -1);
        break;
    case 6:
        EVT_AT_SetPosAng(TITL_RYO, QTE_GAME_TITLE, -0.034000002f, 1.3200001f, 0, -1);
        break;
    case 7:
        EVT_AT_SetPosAng(TITL_RYO, QTE_GAME_TITLE, -0.1f, 1.36f, 0, -1);
        break;
    default:
        break;
    }
    switch (lane) {
    case 0:
    case 1:
    case 7:
        EVT_AT_SetMot(TITL_RYO, 5317, work->step ? 0.80000001f : 0, 1.5f, 0x40000000U, -1);
        break;
    case 3:
    case 4:
    case 6:
        EVT_AT_SetMot(TITL_RYO, 5316, work->step ? 0.80000001f : 0, 1.25f, 0x40000000U, -1);
        break;
    default:
        break;
    }
    Loop3DSoundEffect(0x188);
    if (success) {
        TitleMittWork *mitt = MittForLane(work, lane);
        if (mitt)
            ApplyTitleMittBehavior(mitt, TitleMittMode::Knockout, 0, 5);
        SetTitleLaneCamera(lane);
    }
}
} // namespace shenmue::s2::areas
