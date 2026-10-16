#include "s2/areas/area_dart.hpp"
#include "s2/sound_program.hpp"
#include <cstring>

namespace shenmue::s2::areas {


void DartsLoadAudioBank(DartsEventWork *work, std::int32_t bankMode) {
    std::int32_t banks[6];
    banks[0] = 3;
    banks[1] = 4;
    banks[2] = 1;
    banks[3] = 2;
    banks[4] = 5;
    if (work->audioBank == static_cast<std::uint32_t>(bankMode))
        return;
    if (bankMode == 1) {
        EV_LoadCutsceneFromCurrentScene("01130000.smp");
        EV_LoadAudio("01130000.sbt", 0);
    } else if (bankMode == 2) {
        EV_LoadCutsceneFromCurrentScene("01130000.smp");
        if (work->difficulty >= 2 && work->entry->enemy->name.name == DART_JON.name) {
            EV_LoadAudio("01130000.sbt", 6);
        } else {
            std::size_t index = 0;
            while (index < 5 && DART_OPPONENT_WORDS[index][0] != work->entry->enemy->name.name)
                ++index;
            EV_LoadAudio("01130000.sbt", banks[index]);
        }
    }
    work->audioBank = static_cast<std::uint32_t>(bankMode);
}


std::uint32_t DartsResetScoring(DartsEventWork *work) {
    work->fieldF0 = 100000;
    work->fieldFc = 0;
    work->field104 = 0;
    work->bullseyes = 0;
    work->playerScore.score = 0;
    work->playerScore.accumulated = 0;
    work->playerBullseyes = 0;
    work->scoreStep = 0;
    work->playerWon = 0;
    work->field114 = 100000;
    const bool enemyTurn = work->entry->opponentMode == 2 ? work->field10 == 0 : work->entry->opponentMode == 1;
    if (enemyTurn) {
        work->enemyTurn = 1;
        DartsLoadAudioBank(work, 2);
        std::memcpy(&work->bestScore, &work->alternateBestScore, 16);
        work->opponentScore = work->playerScore;
        work->opponentBullseyes = work->playerBullseyes;
    } else {
        work->enemyTurn = 0;
        std::memcpy(&work->bestScore, &work->fieldE8, 16);
        std::memcpy(&work->opponentScore, &work->fieldF8, 16);
        work->opponentBullseyes = work->bullseyes;
    }
    return static_cast<std::uint32_t>(work->opponentBullseyes);
}


std::uint8_t DartsSwitchScoringTurn(DartsEventWork *work) {
    const std::uint8_t previousTurn = static_cast<std::uint8_t>(work->enemyTurn);
    work->enemyTurn = previousTurn == 0;
    if (previousTurn) {
        std::memcpy(&work->alternateBestScore, &work->bestScore, 16);
        work->playerScore = work->opponentScore;
        work->playerBullseyes = work->opponentBullseyes;
        std::memcpy(&work->bestScore, &work->fieldE8, 16);
        std::memcpy(&work->opponentScore, &work->fieldF8, 16);
        work->opponentBullseyes = work->bullseyes;
    } else {
        DartsLoadAudioBank(work, 2);
        std::memcpy(&work->fieldE8, &work->bestScore, 16);
        std::memcpy(&work->fieldF8, &work->opponentScore, 16);
        work->bullseyes = work->opponentBullseyes;
        std::memcpy(&work->bestScore, &work->alternateBestScore, 16);
        work->opponentScore = work->playerScore;
        work->opponentBullseyes = work->playerBullseyes;
    }
    return previousTurn;
}


bool DartsAdvanceScoreTransfer(DartsEventWork *work, std::int32_t *score, float soundContext) {
    const std::int32_t pending = work->pendingScore;
    if (!pending) {
        Loop3DSoundEffect(0x184, soundContext);
        return true;
    }
    if (work->scoreTick[0]) {
        work->scoreTick[0] = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->scoreTick[0]) - 1U);
        return false;
    }
    if (pending < work->scoreTick[1])
        work->scoreTick[1] = pending;
    const std::uint32_t amount = static_cast<std::uint32_t>(work->scoreTick[1]);
    work->opponentScore.accumulated = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->opponentScore.accumulated) + amount);
    work->pendingScore = static_cast<std::int32_t>(static_cast<std::uint32_t>(pending) - amount);
    *score = static_cast<std::int32_t>(static_cast<std::uint32_t>(*score) - amount);
    work->scoreTick[0] = work->scoreTickInterval;
    return false;
}
} // namespace shenmue::s2::areas
