#include "s2/areas/pati_event.hpp"
#include "s2/areas/pati_event_tables.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/pati_tables.hpp"
#include "s2/ui.hpp"
#include <cmath>
namespace shenmue::s2 {
namespace {
inline constexpr shen_name PATI_BALL_TASK{SHEN_NAME(EVNT)};
}
// @verified - p
void InitializePatiBallCharacters(PatiBallWork *ball) {
    EV_DisableCharacterDisplay(PATI_PLAYER);
    if (DoesScenarioCharacterExist(PATI_BACKPACK))
        EV_DisableCharacterDisplay(PATI_BACKPACK);
    AT_SetLocalBaseChara(PATI_CAMERA, ball->game->board);
    EV_AT_SetPos(PATI_CAMERA, EventTransformFlags::AllAxes, Vector3{});
    EV_AT_SetAngY(PATI_CAMERA, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, ball->boardRotation.y, 0}, nullptr);
    AT_SetLocalBaseChara(ball->nails->character, ball->game->board);
    EV_AT_SetPos(ball->nails->character, EventTransformFlags::AllAxes, Vector3{});
    EV_AT_SetAngY(ball->nails->character, EventTransformFlags::AllAxes, Vector3i{}, nullptr);
    const float scale = (ball->scale * 0.73699999f) * ball->radiusScale;
    const Vector3 size{scale, scale, scale};
    for (std::uint32_t index = 0; index < ball->ballLimit; ++index) {
        shen_name &character = ball->game->ballCharacters[index].character;
        if (character.name) {
            SetScenarioCharacterKillMode(character, 1);
            character = {};
        }
        character = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>(ball->ballModel), {});
        EV_AT_SetAngY(character, EventTransformFlags::AllAxes, Vector3i{0, ball->boardRotation.y, 0x4000}, nullptr);
        mtwkSetMotionworkScale3(character, &size);
    }
    ball->current.position = {ball->right, ball->top, ball->radius + 0.0049999999f};
    ball->current.velocity = {0.0f, 0.0f, 0.0f};
    ball->previous = ball->current;
    ball->releaseJitter = 0;
}
// @verified - p
void InitializePatiBall(PatiBallWork *ball, HLib::Task *task) {
    ball->nails = &ball->game->nails;
    ball->task = task;
    ball->nailTask = nullptr;
    InitializePatiBallNails(ball);
    if (ball->game->mode) {
        ball->ballLimit = ball->game->ballLimit;
        ball->winLimit = ball->game->winLimit;
        ball->missLimit = ball->game->missLimit;
    }
    ConfigurePatiPrizes(ball);
    ball->phase = 0;
    ball->soundFrames = 0;
    ball->previousSound = 0;
    ball->settled = 0;
    EV_GetCharaPosition(ball->game->board, CHARACTER_TRANSLATION_ROOT, &ball->boardPosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ball->game->board, &ball->boardRotation, EventTransformFlags::World);
    if (ball->boardRotation.z >= 0x8000) {
        ball->boardRotation.x += 32768;
        ball->boardRotation.z -= 32768;
        ball->boardRotation.y =
            static_cast<std::uint16_t>(static_cast<std::int32_t>((540.0 - static_cast<float>(ball->boardRotation.y) / 182.04445f) * 8192.0 / 45.0));
    }
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(ball->boardRotation.x))));
    ball->gravity = (std::fabs(cosine) * -0.052266665f) / 30.0f;
    SetScenarioCharacterFlag40(ball->game->board, 1);
    SetScenarioCharacterRadius(ball->game->board, 100.0f);
    InitializePatiBallCharacters(ball);
    SetupPatiBoardSize(0);
}
// @verified - p
void QueuePatiBall(PatiGameWork *game) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(PatiBallCallback, 6, 0x0b, sizeof(PatiBallWork), PATI_BALL_TASK);
    PatiBallWork *ball = static_cast<PatiBallWork *>(HLib::GetTaskParameter(task));
    ball->game = game;
    InitializePatiBall(ball, task);
}
// @verified - p
void DestroyPatiBall(PatiBallWork *ball) {
    hndlSetFlags(FindCharacterByScenarioName(shen_name{static_cast<std::uint32_t>(patiGameplayPlacements[0].yaw)}));
    EV_EnableCharacterDisplay(PATI_PLAYER);
    ResetScenarioCharacterAttachment(PATI_CAMERA);
    if (DoesScenarioCharacterExist(PATI_BACKPACK))
        EV_EnableCharacterDisplay(PATI_BACKPACK);
    if (ball->nailTask)
        HLib::DestroyTask(ball->nailTask);
    HLib::DestroyCurrentTask();
    HLib::ResolveDeferredCallback(ball->game->parent);
}
// @verified - p
void PatiBallCallback(void *data) {
    PatiBallWork &b = *static_cast<PatiBallWork *>(data);
    PatiGameWork &game = *b.game;
    const shen_name hand{static_cast<std::uint32_t>(patiGameplayPlacements[0].yaw)};
    if (game.result == -3)
        b.phase = 13;
    switch (b.phase) {
    case 0: {
        HLib::SetupTaskDelay(10);
        shenSetClockAndCompassRenderTo(1);
        SetLowQualityHands(FindCharacterByScenarioName(hand));
        SetHighModelXYZ(FindCharacterByScenarioName(hand), 1, 0, 0);
        ApplyCharacterHandRotation(FindCharacterByScenarioName(hand), 0, 21, &PATI_HAND_ROTATION);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(hand), 0, reinterpret_cast<std::byte *>(patiHandPoseCommand), 1);
        EV_EnsureCharacterDraw(hand);
        EV_DisableCharacterDisplay(hand);
        modelSetCharaAlpha(FindCharacterByScenarioName(hand), 0.0f);
        if (hand.name == PATI_HAND_MODEL.name) {
            Vector3i rotation{};
            Matrix4x4_Push(nullptr);
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::RotateAroundZ_Wrapper(0);
            Matrix4x4::RotateAroundY_Wrapper(0);
            Matrix4x4::RotateAroundX_Wrapper(0);
            Matrix4x4::preTranslate(-0.093699999f, 0.16869999f, 0.47999999f);
            Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            Matrix4x4::Pop(1);
            EV_scenApplyPosOrRotToPartOf(hand, 0xffff, &position, &rotation, EventCharacterPartTransformMode::Absolute);
            rotation = {61895, b.boardRotation.y, 0};
            EV_AT_SetAngY(hand, EventTransformFlags::AllAxes, rotation, nullptr);
        }
        b.phase = 3;
        [[fallthrough]];
    }
    case 3:
        b.releaseX = shenGetRandomInt32(8) - 4;
        b.releaseJitter = b.releaseX;
        b.previous.position = b.current.position = {static_cast<float>(b.releaseX) / 1000.0f, b.top, b.radius + 0.0049999999f};
        b.current.velocity = {0.0f, 0.0f, 0.0f};
        b.previous.velocity = b.current.velocity;
        b.repeatFrames = 0;
        b.frames = 0;
        EV_EnableCharacterDisplay(hand);
        b.phase = 5;
        b.cameraHeight = b.top - 0.13f;
        g_patiCameraHeight = 9999.0f;
        CheckHelpIcon(33, 0, CheckHelpIconOp::Set);
        if (game.mode == 2)
            CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        UpdatePatiBallPosition(&b);
        UpdatePatiHandPosition(&b);
        g_patiModelAlpha = 0.0f;
        b.historyCursor = 0;
        for (std::int32_t index = 0; index < 90; ++index) {
            b.history[index] = {0.0f, 0.0f};
            b.movement[index] = 0.0f;
        }
        for (std::uint32_t index = 0; index < 8; ++index)
            b.contacts[index] = {index, -1, 0};
        if (game.mode == 2) {
            b.phase = 6;
            g_patiDealerInputClock = 0;
            g_patiDealerInputIndex = game.balls + shenGetRandomInt32(16);
        }
        break;
    case 4:
    case 5: {
        if (b.frames <= 5400) {
            HandlePatiBallInput(&b, static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_REPEAT)));
            GetBattleDataConfig(0, PATI_ACCEPT);
            if (b.option == 7)
                GetBattleDataConfig(0, PATI_CANCEL);
        } else
            HandlePatiBallInput(&b, 1024);
        const std::uint32_t oldFrames = b.frames++;
        if (oldFrames == 1800 && !g_patiVersus) {
            std::int32_t frame = 0;
            g_patiCameraTime = 99999;
            const std::int16_t random = static_cast<std::int16_t>(shenGetRandomInt32(2));
            QueuePatiVoice(&game, random == 1 ? "B090" : "B088", &frame);
        }
        if (shenGetMinutesUntilNextFrameMinute() <= 0 && b.frames >= 1800) {
            b.phase = 13;
            game.result = 4;
        }
        break;
    }
    case 6: {
        game.activeBall = game.balls;
        EV_EnableCharacterDisplay(game.ballCharacters[game.activeBall].character);
        std::uint16_t input = 0;
        const std::uint32_t tolerance = (b.frames + static_cast<std::uint32_t>(game.targetVariation)) / 10;
        if (b.frames <= 150 || (std::abs(b.releaseX - game.targetX) > static_cast<std::int32_t>(tolerance) && b.frames <= 210)) {
            const PatiDealerInput &step = PATI_DEALER_INPUTS[g_patiDealerInputIndex & 15];
            if (g_patiDealerInputClock >= step.duration) {
                if (g_patiDealerInputClock > step.duration + step.pause) {
                    g_patiDealerInputClock = 0;
                    ++g_patiDealerInputIndex;
                }
            } else
                input = static_cast<std::uint16_t>(step.input);
            ++g_patiDealerInputClock;
        } else
            input = 1024;
        ++b.frames;
        if (HandlePatiBallInput(&b, input))
            g_patiDealerInputClock = PATI_DEALER_INPUTS[g_patiDealerInputIndex & 15].duration;
        break;
    }
    case 7:
        g_patiModelAlpha = 0.0f;
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        EV_DisableCharacterDisplay(hand);
        game.activeBall = game.balls;
        EV_EnableCharacterDisplay(game.ballCharacters[game.activeBall].character);
        AdvancePatiBall(&b);
        UpdatePatiBallPosition(&b);
        break;
    case 13:
        shenSetClockAndCompassRenderTo(0);
        EV_DisableCharacterDisplay(hand);
        EV_ToggleCharacterFlag(hand);
        DestroyPatiBall(&b);
        break;
    default:
        break;
    }
    if (!b.settled || b.phase != 7)
        return;
    game.result = -2;
    if (b.radius <= std::fabs(b.current.position.y - b.bottom)) {
        if (b.option == 8) {
            for (std::int32_t index = 0; index < 6; ++index) {
                const PatiCenterPocket &pocket = b.centerPockets[index];
                if (pocket.type && b.current.position.x > pocket.x - pocket.halfWidth && pocket.x + pocket.halfWidth > b.current.position.x &&
                    b.current.position.y > pocket.y && (b.radius + b.radius) + pocket.y > b.current.position.y) {
                    game.pocket = index + 10;
                    game.prize = 0;
                    while (PATI_PRIZES[game.prize].code && PATI_PRIZES[game.prize].code != b.centerPrizes[index])
                        ++game.prize;
                    game.result = 0;
                }
            }
        }
        if (game.result == -2) {
            b.phase = 13;
            game.result = -1;
        }
    } else {
        std::int32_t index = 0;
        while (index + 1 < b.pocketCount && !(b.dividers[index].x > b.current.position.x))
            ++index;
        game.pocket = index;
        game.prize = 0;
        while (PATI_PRIZES[game.prize].code && PATI_PRIZES[game.prize].code != b.prizes[index])
            ++game.prize;
        game.result = 0;
    }
    if (game.result)
        return;
    ++game.balls;
    b.phase = 3;
    const std::uint16_t mask = static_cast<std::uint16_t>(1U << game.pocket);
    if (mask & b.deathMask) {
        b.phase = 13;
        game.result = 3;
    } else {
        if (mask & b.winMask)
            ++game.wins;
        if (game.balls >= b.ballLimit || game.wins >= b.winLimit || game.balls - game.wins > b.missLimit)
            b.phase = 13;
        if (b.phase == 13) {
            game.result = (game.wins >= b.winLimit) + 1;
            const std::int32_t random = shenGetRandomInt32(game.result == 2 ? 10 : 50);
            game.targetX = random + b.releaseX;
            game.targetVariation = shenGetRandomInt32(game.result == 2 ? 100 : 500);
        }
    }
    if (b.phase == 3) {
        EV_DisableCharacterDisplay(game.ballCharacters[game.balls - 1].character);
        EV_AT_SetPos(PATI_CAMERA, EventTransformFlags::AllAxes, Vector3{});
        SetupPatiBoardSize(1);
    }
}
} // namespace shenmue::s2
