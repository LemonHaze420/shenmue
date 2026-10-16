#include "s2/darts.hpp"
#include "s2/model_component.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"

#include <cmath>

namespace shenmue::s2 {

std::uint32_t dartsEventActive{};

// @verified - p
std::int64_t CalculateDartsPositionOrScore(std::int8_t score, Vector3 *position) {
    if (score == -1) {
        const float x = position->x;
        const float y = (position->y - 1.4174f) * DART_BOARD_Y_SCALE;
        const float radius = static_cast<float>(std::sqrt(static_cast<double>(x * x + y * y)));
        if (radius <= DART_INNER_BULL_RADIUS)
            return 500000;
        if (radius <= 0.024f)
            return 250000;
        const std::uint16_t fixedAngle = static_cast<std::uint16_t>(InverseFixedAtan2(-x, -y));
        float angle = static_cast<float>(static_cast<double>(static_cast<float>(fixedAngle) / 182.04445f) + 9.0);
        if (angle >= 360.0f)
            angle = static_cast<float>(static_cast<double>(angle) - 360.0);
        const std::int32_t sector = static_cast<std::int32_t>(angle / 18.0f);
        const std::int32_t points = DART_SECTOR_ORDER[sector < 20 ? sector : 0];
        if (radius > DART_TRIPLE_MINIMUM_RADIUS) {
            if (radius <= DART_TRIPLE_MAXIMUM_RADIUS)
                return 30000 * points;
            if (radius > 0.14920001f) {
                if (radius <= 0.16599999f)
                    return 20000 * points;
                return 0;
            }
        }
        return 10000 * points;
    }
    float radiusOffset = 0.0f;
    float randomRadius = 0.0f;
    float angle;
    if (score == 50 || score == 25) {
        if (score == 25)
            radiusOffset = DART_INNER_BULL_RADIUS;
        randomRadius = IncrementSharedPrngFloat() * DART_INNER_BULL_RADIUS;
        angle = IncrementSharedPrngFloat() * 360.0f;
    } else {
        bool found = false;
        for (std::int32_t multiplier = 1; multiplier < 4 && !found; ++multiplier) {
            float sectorAngle = -9.0f;
            for (std::int32_t sector = 0; DART_SECTOR_ORDER[sector] != -1; ++sector) {
                if (score == DART_SECTOR_ORDER[sector] * multiplier) {
                    angle = sectorAngle + IncrementSharedPrngFloat() * 18.0f;
                    if (multiplier == 1) {
                        randomRadius = IncrementSharedPrngFloat() * 0.063999996f;
                        radiusOffset = 0.024f;
                    } else if (multiplier == 2) {
                        randomRadius = IncrementSharedPrngFloat() * 0.016799986f;
                        radiusOffset = 0.14920001f;
                    } else {
                        randomRadius = IncrementSharedPrngFloat() * 0.016000003f;
                        radiusOffset = DART_TRIPLE_MINIMUM_RADIUS;
                    }
                    found = true;
                    break;
                }
                sectorAngle += 18.0f;
            }
        }
        if (!found)
            return 0;
    }
    const float radius = randomRadius + radiusOffset;
    const std::uint16_t fixedAngle = static_cast<std::uint16_t>(static_cast<std::int32_t>(angle * 8192.0f / 45.0f));
    position->x = sin_fixed16(static_cast<std::int16_t>(fixedAngle)) * radius;
    position->y = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(fixedAngle))) * radius / DART_BOARD_Y_SCALE + 1.4174f;
    position->z = -0.068200007f;
    return 0;
}

// @verified - p
void UpdateDartsScoreDisplay(shen_name character, std::int32_t score, std::int32_t mode) {
    Vector3 position{0.02239999920129776f, DART_LARGE_SCORE_Y[mode], -0.03999999910593033f};
    const Vector3 scale{0.5f, 0.5f, 0.5f};
    const std::uint32_t largeScore = static_cast<std::uint32_t>(score / 10000);
    std::int32_t smallScore = score % 10000;
    SetPosRotForGame(character, 256, &DART_LARGE_DIGIT_STEP, 9999, 0);
    UpdateGameNodeFlags(character, largeScore, &position, 1.0f, nullptr);
    Vector3 symbolPosition{DART_SMALL_DIGIT_STEP.x * 4.0f + 0.14740000665187836f, DART_SMALL_SCORE_Y[mode], -0.03999999910593033f};
    scenGetCharaGameNode(character, 336, &symbolPosition, 1.0f, &scale);
    position = {0.14740000665187836f, DART_SMALL_SCORE_Y[mode], -0.03999999910593033f};
    SetPosRotForGame(character, 256, &DART_SMALL_DIGIT_STEP, 9, 0);
    for (std::int32_t index = 0; index < 4; ++index) {
        UpdateGameNodeFlags(character, static_cast<std::uint32_t>(smallScore % 10), &position, 1.0f, &scale);
        smallScore /= 10;
        position.x += DART_SMALL_DIGIT_STEP.x;
        position.y += DART_SMALL_DIGIT_STEP.y;
        position.z += DART_SMALL_DIGIT_STEP.z;
    }
}

// @verified - p
void UpdateDartsCompactScoreDisplay(shen_name character, std::uint32_t score, std::int32_t mode) {
    const Vector3 step{DART_COMPACT_DIGIT_STEP[mode], 0.0f, 0.0f};
    SetPosRotForGame(character, 256, &step, 9999, 0);
    UpdateGameNodeFlags(character, score, &DART_COMPACT_SCORE_POSITION[mode], 1.0f, &DART_COMPACT_SCORE_SCALE[mode]);
}

// @verified - p
void RestoreOpponentDartsScores(shen_name character, std::int32_t *bestScore, std::int32_t *previousScore) {
    std::size_t index = 0;
    while (index < 5 && DART_SAVED_SCORE_WORDS[3 * index] != character.name)
        ++index;

    if (character.name < 5) {
        *bestScore = static_cast<std::int32_t>(GetSavedMinigameScore(DART_SAVED_SCORE_WORDS[3 * index + 1]));
        *previousScore = static_cast<std::int32_t>(GetSavedMinigameScore(DART_SAVED_SCORE_WORDS[3 * index + 2]));
    }
}

} // namespace shenmue::s2
