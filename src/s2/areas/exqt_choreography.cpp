#include "s2/areas/area_exqt.hpp"
#include "s2/areas/area_dgct.hpp"
#include "s2/camera.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2::areas {
namespace {
void QueueCoinAttachment(bool release) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 11, sizeof(EventCling), EXQT_TASK);
    RegisterEventTask(task);
    EventCling &work = *static_cast<EventCling *>(HLib::GetTaskParameter(task));
    work.cleanup = 1;
    work.target = EXQT_COIN;
    if (release) {
        work.character = shen_name{0};
        work.mode = 1;
        work.delay = 120;
    } else {
        work.character = EXQT_RYO;
        work.bone = 14;
        work.position = exciteCoinPosition;
        work.rotation = exciteCoinRotation;
        work.mode = 0;
        work.delay = 60;
    }
}
} // namespace
void StartExciteIntro(bool continued) {
    const char *voices[]{"0116A024", "0116A029", "0116A016", "0116A017", "0116A019"}; // @text 0116A024 EN: "It's $5." | JA: "１回５ドルか…" ; 0116A029 EN: "Excite QTE.\n$5 for one play." | JA: "エキサイトＱＴＥ…\n１回５ドル…" ; 0116A016 EN: "Should I continue?" | JA: "どうしようか…" ; 0116A017 EN: "I'll play some more." | JA: "もう少し遊ぶか。" ; 0116A019 EN: "I guess I'll continue." | JA: "続けてみるか。"
    const std::int32_t choice = shenGetRandomInt32(continued ? 3 : 2) + (continued ? 2 : 0);
    if (choice >= 0 && choice < 5)
        EVT_AT_SetVoice(voices[choice], 1, 1);
    EVT_AT_SetCam(8000, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(8010, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 100);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_KsIcon_Callback, 6, 11, sizeof(EventChoiceIcon), EXQT_TASK);
    RegisterEventTask(task);
    EventChoiceIcon &work = *static_cast<EventChoiceIcon *>(HLib::GetTaskParameter(task));
    work.cleanup = 1;
    work.choices[0] = -1;
    work.choices[1] = continued ? 37 : 36;
    work.choices[2] = continued ? 34 : 23;
    work.choices[3] = -1;
    work.timeout = 300;
    work.defaultChoice = continued ? 34 : 23;
    work.mode = 2;
    work.delay = 100;
}
void StartExcitePaidIntro(bool continued) {
    EVT_AT_SetCam(8020, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(9030, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 117);
    const char *voices[]{"0116A003", "0116A004", "0116A005", "0116A017", "0116A018"}; // @text 0116A003 EN: "I'll try it." | JA: "やってみるか。" ; 0116A004 EN: "I guess I'll play." | JA: "遊んでいくか。" ; 0116A005 EN: "I'll have some fun." | JA: "遊んでみるか。" ; 0116A017 EN: "I'll play some more." | JA: "もう少し遊ぶか。" ; 0116A018 EN: "I'll try again." | JA: "もう１回やってみるか。"
    const std::int32_t choice = shenGetRandomInt32(continued ? 2 : 3) + (continued ? 3 : 0);
    if (choice >= 0 && choice < 5)
        EVT_AT_SetVoice(voices[choice], 1, 1);
    EVT_AT_SetMot(EXQT_RYO, 5249, 0, 1, 0x80000000U, -1);
    EVT_AT_SetMot(EXQT_RYO, 5251, 10, 1, 0x40000000U, 117);
    Queue_EVT_AT_CtrlHandUp_1(EXQT_RYO, 0, reinterpret_cast<std::byte *>(exciteCoinHandPose), 1, 1);
    QueueCoinAttachment(false);
    QueueCoinAttachment(true);
    EVT_AT_SetPos(EXQT_COIN, EXQT_INVALID, {1000, 1000, 1000}, 121);
    EVT_AT_SetAngY(EXQT_COIN, EXQT_INVALID, 0, 121);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-5), 120);
    EVT_AT_SetSound(381, 55);
    EVT_AT_SetSound(380, 125);
}
void StartExciteFreeIntro() {
    EventSetCamera camera{.cleanup = 0, .camera = 9030, .eye = EXQT_MACHINE, .target = EXQT_MACHINE, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EventSetMotion motion{.cleanup = 0, .character = EXQT_RYO, .animation = 5251, .frame = 10, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5251);
}
void ExciteLostLife(bool finalLife) {
    if (finalLife) {
        EVT_AT_SetCam(7055, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
        EVT_AT_SetMot(EXQT_RYO, 5252, 0, 1, 0x80000000U, -1);
    } else {
        EVT_AT_SetCam(8050, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
        EV_DisableCharacterDisplay(EXQT_MACHINE);
        EVT_AT_ObjectDisp_1(EXQT_MACHINE, 55, 0);
    }
    const char *voices[]{"0116A012", "0116A013", "0116A031"}; // @text 0116A012 EN: " " | JA: "あっ…" ; 0116A013 EN: "Damn!" | JA: "くそっ…" ; 0116A031 EN: " " | JA: "くっ…"
    const std::int32_t choice = shenGetRandomInt32(3);
    if (choice >= 0 && choice < 3)
        EVT_AT_SetVoice(voices[choice], 1, 0);
}
void ExciteHighScoreReaction() {
    EVT_AT_SetCam(7060, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(7065, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 30);
    EVT_AT_SetMot(EXQT_RYO, 5250, 0, 1, 0x80000000U, -1);
}
void ExciteFinishedReaction(std::int32_t score, std::int32_t best) {
    EVT_AT_SetCam(8070, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
        const std::int32_t choice = shenGetRandomInt32(2);
        if (choice == 0)
            EVT_AT_SetVoice("0116A014", 1, 0); // @text EN: "It's over." | JA: "終わったか…"
        else if (choice == 1)
            EVT_AT_SetVoice("0116A015", 1, 0); // @text EN: "It's over already." | JA: "もう終わりか…"
    } else if (score < best && score < 50000)
        EVT_AT_SetVoice("0116A015", 1, 1); // @text EN: "It's over already." | JA: "もう終わりか…"
    else if (score >= best && score >= 50000)
        EVT_AT_SetVoice("0116A035", 1, 1); // @text EN: "It felt good." | JA: "手ごたえあったな。"
    else
        EVT_AT_SetVoice("0116A014", 1, 1); // @text EN: "It's over." | JA: "終わったか…"
}
void StartGameCenterIntro() {
    EVT_AT_SetCam(7000, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(7010, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 100);
}
void StartGameCenterPaidIntro() {
    EVT_AT_SetCam(7020, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(7030, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 125);
    EVT_AT_SetCam(7040, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 155);
    EVT_AT_SetMot(EXQT_RYO, 5249, 0, 1, 0x80000000U, -1);
    EVT_AT_SetMot(EXQT_RYO, 5251, 0, 1, 0x80000000U, 125);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-100), 10);
}
void GameCenterLostLife(bool finalLife) {
    EVT_AT_SetCam(finalLife ? 7055 : 7050, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    if (finalLife)
        EVT_AT_SetMot(EXQT_RYO, 5252, 0, 1, 0x80000000U, -1);
}
void FinishGameCenterQte() {
    EVT_AT_SetCam(7070, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
}
} // namespace shenmue::s2::areas
