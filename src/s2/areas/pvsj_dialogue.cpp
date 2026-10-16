#include "s2/areas/area_pvsj.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"
#include "s2/motion_work.hpp"
namespace shenmue::s2 {
namespace {
void Face(shen_name name, std::int32_t expression = 0) {
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(name), expression, 10, 1);
}
void Faces(PvsjEventWork *work, shen_name other, std::int32_t expression = 0) {
    Face(PVSJ_PLAYER);
    Face(other, expression);
}
void Camera(PvsjEventWork *work, std::int32_t id, shen_name name) {
    PvsjSetCamera(work, id, name, name, work->delay);
}
void Voice(PvsjEventWork *work, const char *code) {
    PvsjQueueVoice(work, code, &work->delay);
}
void RandomVoice(PvsjEventWork *work, const char *first, const char *second) {
    Voice(work, static_cast<std::int16_t>(shenGetRandomInt32(2)) == 1 ? second : first);
}
void Motion(PvsjEventWork *work, shen_name name, std::int32_t index, float frame = 0, std::int32_t delay = -1) {
    PvsjSetMotion(name, work->boards->motions[index], frame, 1, 0, delay < 0 ? work->delay : delay);
}
void RestoreIdle(PvsjEventWork *work, std::int32_t delay) {
    Motion(work, work->owner, 5, 0, delay);
    Motion(work, PVSJ_PLAYER, 7, 0, delay);
}
std::int32_t EarlierEnd(PvsjEventWork *work, std::int32_t start, std::int32_t limit) {
    return work->delay - start < limit ? work->delay : start + limit;
}
} // namespace

void PvsjOfferJob(PvsjEventWork *work) {
    const auto start = work->delay;
    Faces(work, work->owner);
    Camera(work, pvsjCustomerCamera, work->owner);
    Motion(work, work->owner, 0);
    Motion(work, PVSJ_PLAYER, 6, 0, work->delay + 25);
    Voice(work, "B094");
    Voice(work, "B095");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A035");
    RestoreIdle(work, EarlierEnd(work, start, 250));
}

void PvsjBeginShift(PvsjEventWork *work) {
    Faces(work, work->owner);
    Camera(work, pvsjCustomerCamera, work->owner);
    RandomVoice(work, "B096", "B097");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A036");
    RestoreIdle(work, work->delay);
}

void PvsjCallCustomer(PvsjEventWork *work) {
    Face(PVSJ_PLAYER);
    const auto previous = work->callFrames++;
    if (previous & 1)
        Voice(work, "A042");
    else {
        const auto choice = static_cast<std::int16_t>(shenGetRandomInt32(2));
        Voice(work, choice == 1 ? "A044" : choice == 2 ? "A045" : "A043");
    }
}

void PvsjWelcomeCustomer(PvsjEventWork *work) {
    Faces(work, work->customer);
    Camera(work, pvsjPlayerCamera + 1, PVSJ_PLAYER);
    Motion(work, work->customer, 12);
    Motion(work, PVSJ_PLAYER, 7);
    RandomVoice(work, "A046", "A047");
    Camera(work, pvsjCustomerCamera, work->customer);
    RandomVoice(work, "F001", "F002");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A048");
}

void PvsjExplainPartTimeJob(PvsjEventWork *work) {
    Faces(work, work->customer);
    Camera(work, pvsjCustomerCamera + 1, work->customer);
    Motion(work, work->customer, 17);
    Motion(work, PVSJ_PLAYER, 7);
    RandomVoice(work, "F003", "F004");
    Camera(work, pvsjPlayerCamera + 1, PVSJ_PLAYER);
    Voice(work, "A049");
    Camera(work, pvsjCustomerCamera + 1, work->customer);
    RandomVoice(work, "F001", "F002");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A048");
}

void PvsjStartCustomerGame(PvsjEventWork *work) {
    Faces(work, work->customer);
    Camera(work, pvsjCustomerCamera, work->customer);
    RandomVoice(work, "F005", "F006");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A072");
}

void PvsjCustomerDeclines(PvsjEventWork *work) {
    Faces(work, work->customer);
    Camera(work, pvsjPlayerCamera + 1, PVSJ_PLAYER);
    Motion(work, work->customer, 12);
    Motion(work, PVSJ_PLAYER, 7);
    RandomVoice(work, "A046", "A047");
    Camera(work, pvsjCustomerCamera + 1, work->customer);
    RandomVoice(work, "F007", "F008");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A050");
    RandomVoice(work, "A051", "A052");
}

void PvsjHandleCustomerInquiry(PvsjEventWork *work) {
    Faces(work, work->customer);
    Camera(work, pvsjCustomerCamera + 1, work->customer);
    Motion(work, work->customer, 17);
    Motion(work, PVSJ_PLAYER, 7);
    RandomVoice(work, "F009", "F004");
    Camera(work, pvsjPlayerCamera + 1, PVSJ_PLAYER);
    Voice(work, "A049");
    Camera(work, pvsjCustomerCamera + 1, work->customer);
    RandomVoice(work, "F010", "F011");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    RandomVoice(work, "A053", "A054");
    Camera(work, pvsjCustomerCamera, work->customer);
    RandomVoice(work, "F012", "F013");
}

void PvsjReportShift(PvsjEventWork *work) {
    const auto start = work->delay;
    Faces(work, work->owner, FacialExpressionId::Smile);
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    Motion(work, work->owner, 1, 100);
    Motion(work, PVSJ_PLAYER, 7);
    RandomVoice(work, "B098", "B099");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    RestoreIdle(work, EarlierEnd(work, start, 250));
}

void PvsjAskToContinue(PvsjEventWork *work) {
    Faces(work, work->owner);
    Camera(work, pvsjCustomerCamera, work->owner);
    RestoreIdle(work, work->delay);
    RandomVoice(work, "B100", "B101");
}

void PvsjClosingReport(PvsjEventWork *work) {
    Faces(work, work->owner);
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    RandomVoice(work, "B102", "B103");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A035");
}

void PvsjTimeExpired(PvsjEventWork *work) {
    Faces(work, work->owner);
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    const auto random = static_cast<std::int16_t>(shenGetRandomInt32(3));
    Voice(work, random == 1 ? "B119" : random == 2 ? "B120" : "B118");
}

void PvsjContinueShift(PvsjEventWork *work) {
    Faces(work, work->owner);
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    RandomVoice(work, "A036", "A037");
    Motion(work, work->owner, 0);
    Motion(work, PVSJ_PLAYER, 6);
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    Voice(work, "B104");
    RestoreIdle(work, work->delay);
}

void PvsjOfferPay(PvsjEventWork *work) {
    const auto start = work->delay;
    Faces(work, work->owner, FacialExpressionId::Smile);
    Camera(work, pvsjCustomerCamera, work->owner);
    Motion(work, work->owner, 3);
    Motion(work, PVSJ_PLAYER, 8);
    RandomVoice(work, "B105", "B106");
    RandomVoice(work, "B107", "B108");
    Voice(work, "B109");
    RestoreIdle(work, EarlierEnd(work, start, 250));
}

void PvsjDeclineContinue(PvsjEventWork *work) {
    Faces(work, work->owner, FacialExpressionId::Smile);
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    RestoreIdle(work, work->delay);
    RandomVoice(work, "B110", "B111");
}

void PvsjAskWithoutProfit(PvsjEventWork *work) {
    const auto start = work->delay;
    Faces(work, work->owner, FacialExpressionId::Angry);
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    Motion(work, work->owner, 2, 130);
    Motion(work, PVSJ_PLAYER, 7);
    Voice(work, "B112");
    Voice(work, "A038");
    Voice(work, "B113");
    RestoreIdle(work, EarlierEnd(work, start, 524));
}

void PvsjRestoreSpeech(PvsjEventWork *work) {
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    RandomVoice(work, "A039", "A040");
}

void PvsjFinishWithoutProfit(PvsjEventWork *work) {
    const auto start = work->delay;
    Faces(work, work->owner);
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    Motion(work, work->owner, 4);
    Motion(work, PVSJ_PLAYER, 9);
    const auto random = static_cast<std::int16_t>(shenGetRandomInt32(3));
    Voice(work, random == 1 ? "B115" : random == 2 ? "B116" : "B114");
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    Voice(work, "A041");
    Camera(work, pvsjCustomerCamera + 1, work->owner);
    Voice(work, "B117");
    RestoreIdle(work, EarlierEnd(work, start, 200));
}

void PvsjShowWallet(PvsjEventWork *work) {
    Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
    AnimateMoneyAmount(static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash)), 0, 0);
    SetPlayerStat(PlayerStatsId::Cash, 0);
}
} // namespace shenmue::s2
