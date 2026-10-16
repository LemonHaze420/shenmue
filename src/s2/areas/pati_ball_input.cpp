#include "s2/areas/pati_event.hpp"
#include "s2/event_runtime.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
// @verified - p
bool HandlePatiBallInput(PatiBallWork *ball, std::uint16_t input) {
    PatiBallWork &b = *ball;
    bool atEdge = false;
    if (b.phase == 4) {
        if (input & 0x400) {
            CheckHelpIcon(33, 0, CheckHelpIconOp::Set);
            b.phase = 5;
        } else if (input & 0x200) {
            CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
            b.phase = 13;
            b.game->result = -4;
        }
    } else if (b.phase == 5 || b.phase == 6) {
        if (input & 0x400) {
            CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
            b.phase = 7;
            b.frames = 0;
            b.repeatFrames = 0;
            b.soundFrames = 0;
            b.previous.position.y = b.current.position.y = b.top;
            b.previous.position.z = b.current.position.z = b.radius + 0.0049999999f;
            b.current.velocity = {0.0f, 0.0f, 0.0f};
            b.previous.velocity = b.current.velocity;
            b.settled = 0;
            b.cameraHeight = b.top - 0.13f;
        } else if ((input & 0x200) && b.option == 7) {
            CheckHelpIcon(33, 0, CheckHelpIconOp::Set);
            CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
            b.phase = 13;
            b.game->result = -4;
        } else {
            if (input & 0xf000)
                ++b.repeatFrames;
            else
                b.repeatFrames = 0;
            const std::int32_t repeats = b.repeatFrames >= 45 ? 9 : b.repeatFrames / 5;
            for (std::int32_t repeat = 0; repeat <= repeats; ++repeat) {
                std::int32_t candidate;
                bool outside;
                if (input & 0x4000) {
                    candidate = b.releaseX - 1;
                    outside = static_cast<double>(candidate) / 1000.0 < static_cast<double>(b.releaseJitter + 4) / 1000.0 + b.left;
                } else if (input & 0x8000) {
                    candidate = b.releaseX + 1;
                    outside = b.right - static_cast<double>(b.releaseJitter + 4) / 1000.0 < static_cast<double>(candidate) / 1000.0;
                } else
                    continue;
                if (outside)
                    atEdge = true;
                else
                    b.releaseX = candidate;
            }
        }
    }
    const shen_name hand{static_cast<std::uint32_t>(patiGameplayPlacements[0].yaw)};
    if (b.phase == 4) {
        EV_DisableCharacterDisplay(hand);
        b.game->activeBall = b.game->balls;
        EV_DisableCharacterDisplay(b.game->ballCharacters[b.game->activeBall].character);
        SetupPatiBoardSize(2);
    } else if (b.phase == 5 || b.phase == 6) {
        EV_EnableCharacterDisplay(hand);
        b.game->activeBall = b.game->balls;
        EV_EnableCharacterDisplay(b.game->ballCharacters[b.game->activeBall].character);
        SetupPatiBoardSize(1);
    }
    if (b.phase != 7) {
        b.previous.position.x = b.current.position.x = static_cast<float>(b.releaseX) / 1000.0f;
        if (input & 0xc000) {
            UpdatePatiBallPosition(ball);
            UpdatePatiHandPosition(ball);
        }
    }
    return atEdge;
}
} // namespace shenmue::s2
