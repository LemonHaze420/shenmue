#include "s2/areas/pati_event.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/inventory.hpp"
#include "s2/pati_tables.hpp"
namespace shenmue::s2 {
// @verified - p
void PlacePatiBet(PatiEventWork *work) {
    const std::int32_t amount = work->bet * work->remainingGames;
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-amount), work->delay);
    work->delay += 5;
    work->totalBet += amount;
}
// @verified - p
void PayPatiWinnings(PatiEventWork *work) {
    const std::int32_t amount = GetPatiPayout(work);
    SetPatiCamera(g_patiPlayerCamera + 1, PATI_PLAYER, PATI_PLAYER, work->delay);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(amount), work->delay);
    work->delay += 5;
    work->payout += amount;
}
// @verified - p
void RefundPatiBet(PatiEventWork *work) {
    const std::int32_t amount = work->bet * work->remainingGames;
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, work->delay);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(amount), work->delay);
    work->delay += 5;
    work->totalBet -= amount;
}
// @verified - p
void KeepPatiPrize(PatiEventWork *work) {
    const PatiPrize &prize = PATI_PRIZES[work->game.prize];
    if (!prize.quantity)
        return;
    std::uint32_t index = 0;
    while (index < work->prizeCount && work->heldPrizes[index].item != prize.item)
        ++index;
    if (index >= work->prizeCount) {
        work->heldPrizes[work->prizeCount].item = prize.item;
        work->heldPrizes[work->prizeCount++].count = 1;
    } else
        ++work->heldPrizes[index].count;
}
// @verified - p
bool AwardPatiPrize(PatiEventWork *work) {
    if (!work->prizeCount)
        return true;
    const PatiHeldPrize prize = work->heldPrizes[work->prizeIndex++];
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, work->delay);
    AddInventoryItem(prize.item, static_cast<std::uint8_t>(prize.count), 1);
    work->delay += 5;
    if (static_cast<std::uint32_t>(work->prizeIndex) < work->prizeCount)
        return false;
    work->prizeCount = 0;
    work->prizeIndex = 0;
    return true;
}
// @verified - p
void SetPatiBet(PatiEventWork *work) {
    switch (work->wagerMode) {
    case 1:
        work->bet = 1;
        break;
    case 2:
        work->bet = 5;
        break;
    case 3:
        work->bet = 10;
        break;
    case 4:
        work->bet = 20;
        break;
    case 5:
        work->bet = 50;
        break;
    case 6:
        work->bet = 100;
        break;
    default:
        work->bet = 1;
        break;
    }
}
// @verified - p
bool HasPatiWon(PatiEventWork *work) {
    switch (work->type) {
    case 1:
    case 2:
        return work->game.wins >= 1;
    case 3:
        return work->game.wins >= 3;
    case 4:
        return work->game.wins >= 4;
    case 5:
        return work->game.wins >= 2;
    case 6:
        return work->game.wins > 0 && GetPatiPayout(work) > 0;
    case 8:
        return work->game.wins >= work->game.winLimit;
    default:
        return false;
    }
}
} // namespace shenmue::s2
