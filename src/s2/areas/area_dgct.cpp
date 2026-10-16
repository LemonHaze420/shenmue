#include "s2/areas/area_dgct.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {

void InitializeDgct() {
}
void RunDgct(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(GameCenterMain, 4, 11, sizeof(GameCenterWork), EXQT_TASK);
    GameCenterWork &work = *static_cast<GameCenterWork *>(HLib::GetTaskParameter(task));
    SetPlayerControlEnabled(1);
    work.task = task;
    work.state = 1;
    work.game.game.state = 0;
    work.game.game.best = 30000;
    work.game.game.continued = 0;
}
void GameCenterMain(void *data) {
    GameCenterWork &work = *static_cast<GameCenterWork *>(data);
    switch (work.state) {
    case 1: {
        if (GetCurrentControlInput(7) == 1024)
            work.state = 2;
        Vector3 position;
        EV_GetCharaPosition(EXQT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        if (position.z > 117.5f)
            work.state = 11;
        SetSharedPrngSeed(7);
        break;
    }
    case 2:
        if (UpdateGameCenterQte(work.game))
            work.state = 1;
        break;
    case 11:
        SetPlayerControlEnabled(0);
        HLib::SetupTaskDelay(60);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        work.state = 12;
        break;
    case 12:
        EV_scenQueueLoad(1, DGCT_RETURN, 50);
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
void InitializeGameCenterQte(ExciteState &state) {
    Vector3 position;
    EV_GetCharaPosition(EXQT_MACHINE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x += 0;
    position.y += 0;
    position.z += .718f;
    SetMotionRegionFlag(scenFindMotionwork(EXQT_RYO), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetAngY(EXQT_RYO, EventTransformFlags::AllAxes, {0, 0, 0}, nullptr);
    EV_AT_SetPos(EXQT_RYO, EventTransformFlags::AllAxes, position);
    EV_SetCharaControllerFlags(EXQT_RYO, 2, static_cast<NpcCharacterControllerState>(0));
    EV_SetCharaControllerFlags(EXQT_RYO, 3, static_cast<NpcCharacterControllerState>(0));
    state.game.score = 0;
    SetPlayerControlEnabled(0);
    state.game.motion = ReadMotiFromDirectory(1, DGCT_AREA, "0116E.MOT");
}
bool UpdateGameCenterQte(ExciteState &state) {
    ExciteGameState &game = state.game;
    ExciteInput &input = state.controls.arcade.input;
    UpdateGameCenterDisplay(state);
    switch (game.state) {
    case 0:
        InitializeGameCenterQte(state);
        state.lives = 3;
        game.score = 0;
        game.successes = 0;
        game.cumulativeBonus = 0;
        game.timingBonus = 0;
        game.waitElapsed = 0;
        state.cycle = 0;
        state.patternByte = 0;
        state.patternStage = 0;
        state.immediateAdvance = 0;
        game.waitTarget = 40;
        state.patternScale = 3;
        state.tier = 0;
        state.timeScale = 8;
        input.required = 0;
        input.pressed = 0;
        input.window = 0;
        input.elapsed = 0;
        input.attempts = 0;
        InitializeExciteMachineParts();
        HideExciteMachineParts();
        ResetExciteMachineDisplay();
        game.state = 1;
        return false;
    case 1:
        StartGameCenterIntro();
        game.waitTarget = 200;
        game.waitElapsed = 0;
        game.state = 2;
        return false;
    case 2: {
        if (game.waitElapsed < game.waitTarget)
            break;
        std::int16_t choices[]{-1, 74, 75, -1};
        game.waitElapsed = 0;
        SetupControlSpeakAndUI(choices, 300, -1, 0);
        game.waitElapsed = 0;
        game.waitTarget = 40;
        game.state = 3;
        return false;
    }
    case 3:
        if (shenGetCtrlSpeakIconFlag() == 74)
            game.state = 4;
        else if (shenGetCtrlSpeakIconFlag() == 75 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            game.state = 18;
        return false;
    case 4:
        StartGameCenterPaidIntro();
        game.waitTarget = 205;
        game.waitElapsed = 0;
        game.state = 5;
        return false;
    case 5:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitTarget = 40;
        game.waitElapsed = 0;
        game.state = 6;
        return false;
    case 6:
        HideExciteMachineParts();
        for (std::int32_t i = 0; i < 6; ++i)
            QueueExcitePart(170, (i & 1) == 0, 14 * i);
        EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        game.waitTarget = 84;
        game.waitElapsed = 0;
        game.state = 7;
        return false;
    case 7:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        SetExciteMachinePart(169, 1);
        SetExciteMachinePart(170, 1);
        SetExciteMachinePart(176, 1);
        UpdateExciteLives(state);
        game.waitElapsed = 0;
        game.waitTarget = 40;
        game.state = 8;
        return false;
    case 8:
        if (state.immediateAdvance == 1)
            game.state = 10;
        else {
            if (game.waitElapsed < game.waitTarget)
                break;
            game.state = 10;
            game.waitElapsed = 0;
        }
        return false;
    case 10:
        BuildGameCenterInput(state);
        game.state = 11;
        return false;
    case 11:
        if (PollGameCenterInput(input))
            game.state = 12;
        return false;
    case 12:
        if (static_cast<std::int32_t>(input.window - input.elapsed) <= 0)
            game.timingBonus = 0;
        else {
            const float bonus = static_cast<float>(100 * (input.window - input.elapsed + 2) / input.window);
            game.timingBonus = bonus < 100 ? bonus : 100;
            game.cumulativeBonus += game.timingBonus;
        }
        if (ConsumeGameCenterInput(input)) {
            ++game.successes;
            game.state = 14;
        } else {
            EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
            game.state = 15;
        }
        return false;
    case 14: {
        const std::int32_t base = 1000 * game.successes;
        game.cumulativeBonus += game.timingBonus;
        const std::int32_t score = static_cast<std::int32_t>(game.cumulativeBonus + base);
        game.score = score > 99999000 ? 99999000 : score;
        game.state = 8;
        return false;
    }
    case 15:
        state.immediateAdvance = 0;
        if (state.lives <= 1) {
            GameCenterLostLife(true);
            state.retryFrames = 0;
        } else {
            --state.lives;
            UpdateExciteLives(state);
            GameCenterLostLife(false);
            state.retryFrames = game.waitTarget ? game.waitTarget : 1;
        }
        game.waitElapsed = 0;
        game.waitTarget = 55;
        game.state = 16;
        return false;
    case 16:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        if (state.retryFrames) {
            EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
            game.waitTarget = 60;
            game.state = 8;
        } else
            game.state = game.score >= game.best ? 19 : 21;
        return false;
    case 17:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 11;
        game.waitElapsed = 0;
        return false;
    case 18:
        DeleteMotion(game.motion);
        if (game.score >= game.best)
            game.best = game.score;
        game.continued = game.score;
        SetPlayerControlEnabled(1);
        EVS_LoopCamera(0, shen_name{0}, shen_name{0});
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        game.state = 0;
        return true;
    case 19:
        ExciteHighScoreReaction();
        game.waitTarget = 85;
        game.waitElapsed = 0;
        game.state = 20;
        return false;
    case 20:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 21;
        game.waitElapsed = 0;
        return false;
    case 21:
        HideExciteMachineParts();
        SetExciteMachinePart(166, 1);
        QueueExciteResultDisplay(state);
        EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        game.waitTarget = 84;
        game.waitElapsed = 0;
        game.state = 22;
        return false;
    case 22:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 23;
        game.waitElapsed = 0;
        return false;
    case 23:
        FinishGameCenterQte();
        game.waitTarget = 100;
        game.waitElapsed = 0;
        game.state = 24;
        return false;
    case 24:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 18;
        game.waitElapsed = 0;
        return false;
    default:
        return false;
    }
    ++game.waitElapsed;
    return false;
}
const ScriptAreaDefinition dgctScriptDefinition{.areaId = DGCT_AREA, .field04 = 0, .init = InitializeDgct, .main = RunDgct};
} // namespace shenmue::s2::areas
