#include "s2/areas/maki_tables.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/screen_film.hpp"

namespace shenmue::s2::areas {
namespace {
struct MakiDialogue {
    const char *voice;
    std::int32_t frames;
};
constexpr MakiDialogue makiDepositShenhua[]{{.voice = "0518B017", .frames = 60}, {.voice = "0518B018", .frames = 70}, {.voice = "0518B019", .frames = 90}, // @text 0518B017 EN: "Good. You found them." | JA: "やっぱり、奥にあったのね。" ; 0518B018 EN: "We need more.\nProbably twice as much." | JA: "これだけじゃ、朝までもたない。" ; 0518B019 EN: "We need more.\nMaybe twice as much." | JA: "もっといるわ…\nこの倍ぐらいあれば…"
    {.voice = "0518B020", .frames = 100}, {.voice = "0518B021", .frames = 40}, {.voice = "0518B022", .frames = 30}, {.voice = "0518B023", .frames = 40}, // @text 0518B020 EN: "A fair amount.\nBut a little more would be useful." | JA: "だいぶ集まったけど、\nもう少しあった方が…" ; 0518B021 EN: "Not enough yet." | JA: "まだ足りない…" ; 0518B022 EN: "We need more." | JA: "もっといるわ。" ; 0518B023 EN: "We need to gather more." | JA: "もっと集めないと…"
    {.voice = "0518B026", .frames = 50}, {.voice = "0518B027", .frames = 40}, {.voice = "0518B028", .frames = 40}}; // @text 0518B026 EN: "We need a bit more=@" | JA: "もう少しあった方が…" ; 0518B027 EN: "Let's gather a bit more." | JA: "もうちょっと集めましょ。" ; 0518B028 EN: "A bit more would help." | JA: "もっと集めておこう…"
constexpr MakiDialogue makiDepositRyo[]{{.voice = "0518A024", .frames = 100}, {.voice = "0518A025", .frames = 110}, {.voice = "0518A026", .frames = 140}, // @text 0518A024 EN: "Yeah. There seems to be more.\nI'll go gather them." | JA: "ああ、まだありそうだ。\nもっと集めてくるよ。" ; 0518A025 EN: "All right.\nI'll go look for more." | JA: "そうだな…もっと集めてくる。\n待っててくれ。" ; 0518A026 EN: "All right, I'll try to find them.\nI'm sure there's more." | JA: "そうか…よし、もっと探してみるよ。\nまだありそうだ。"
    {.voice = "0518A027", .frames = 110}, {.voice = "0518A028", .frames = 90}, {.voice = "0518A029", .frames = 50}, {.voice = "0518A030", .frames = 100}, // @text 0518A027 EN: "All right.\nI'll try looking for some more." | JA: "そうか…わかった。\nもうちょっと探してみるよ。" ; 0518A028 EN: "All right.\nI'll go look for more." | JA: "そうか…じゃあ、もっと探してくる。" ; 0518A029 EN: "All right.\nI'll look again." | JA: "わかった。もっと探してくる。" ; 0518A030 EN: "All right.\nLet's gather more." | JA: "そうか…よし、また探しに行こう。"
    {.voice = "0518A031", .frames = 60}, {.voice = "0518A032", .frames = 60}, {.voice = "0518A033", .frames = 50}}; // @text 0518A031 EN: "All right.\nI'll get some more." | JA: "そうか…わかった。" ; 0518A032 EN: "Yeah.\nLet's gather more." | JA: "そうだな…もっと探そう。" ; 0518A033 EN: "Yeah.\nI'll go again." | JA: "ああ。また行ってくるよ。"
constexpr MakiDialogue makiDepositReply[]{{.voice = "0518B013", .frames = 20}, {.voice = "0518B013", .frames = 20}, {.voice = "0518B013", .frames = 20}, // @text EN: "Yes." | JA: "うん。"
    {.voice = "0518B013", .frames = 20}, {.voice = "0518B013", .frames = 20}, {.voice = "0518B024", .frames = 20}, {.voice = "0518B025", .frames = 20}, // @text 0518B013 EN: "Yes." | JA: "うん。" ; 0518B024 EN: "Yes." | JA: "ええ。" ; 0518B025 EN: "Please." | JA: "おねがい。"
    {.voice = "0518B025", .frames = 20}, {.voice = "0518B024", .frames = 20}, {.voice = "0518B013", .frames = 20}}; // @text 0518B025 EN: "Please." | JA: "おねがい。" ; 0518B024 EN: "Yes." | JA: "ええ。" ; 0518B013 EN: "Yes." | JA: "うん。"
void MakiCampfireCamera(std::int32_t camera, std::int32_t delay) {
    EVT_AT_SetCam(camera, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, delay);
}
} // namespace
std::int32_t MakiDepositLogs() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    const std::int32_t previous = makiPreviousDeposit;
    std::int32_t selection = 0;
    if (previous != -1 && makiRuntime.depositedCount >= 5) {
        if (makiRuntime.depositedCount >= 10)
            selection = makiRuntime.depositedCount >= 15 ? 3 : 2;
        else
            selection = 1;
    }
    makiPreviousDeposit = selection;
    if (selection == previous)
        selection = makiRuntime.depositedCount >= 10 ? 7 : 4;
    std::int32_t firstIndex = selection, secondIndex = selection, thirdIndex = selection;
    if (selection == 4 || selection == 7)
        firstIndex += shenGetRandomInt32(3);
    EVT_AT_SetVoice(makiDepositShenhua[firstIndex].voice, 120);
    const std::int32_t firstFrames = makiDepositShenhua[firstIndex].frames;
    const std::int32_t secondDelay = 120 + firstFrames;
    if (selection == 4 || selection == 7)
        secondIndex += shenGetRandomInt32(3);
    EVT_AT_SetVoice(makiDepositRyo[secondIndex].voice, secondDelay);
    const std::int32_t secondFrames = makiDepositRyo[secondIndex].frames;
    const std::int32_t thirdDelay = secondDelay + secondFrames;
    if (selection == 4 || selection == 7)
        thirdIndex += shenGetRandomInt32(3);
    EVT_AT_SetVoice(makiDepositReply[thirdIndex].voice, thirdDelay);
    const std::int32_t thirdFrames = makiDepositReply[thirdIndex].frames;
    MakiCampfireCamera(2500, 0);
    camCtrlDisableFilmBars();
    MakiCampfireCamera(2501, 120);
    MakiCampfireCamera(2502, secondDelay);
    MakiCampfireCamera(2503, thirdDelay);
    EVT_AT_SetPos(MAKI_RYO, EVENT_CHARACTER_INVALID, {.x = 156.8f, .y = 100, .z = 390.7f}, 0);
    EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, 157.5f, 0);
    EVT_AT_SetPos(MAKI_RYO, EVENT_CHARACTER_INVALID, {.x = 156.8f, .y = 100, .z = 391}, secondDelay);
    EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, 135, secondDelay);
    EVT_AT_SetPos(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, {.x = 154.9f, .y = 100, .z = 393.5f}, 0);
    EVT_AT_SetAngY(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, 0, 0);
    if (makiRuntime.heldCount == 1)
        EVT_AT_SetMot(MAKI_RYO, 0x4002, 0, .7f, 0x40000000U, 0);
    else
        EVT_AT_SetMot(MAKI_RYO, 0x4006, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(MAKI_RYO, 0xf003, 0, 1, 0x40000000U, secondDelay);
    EVT_AT_SetMot(MAKI_SHENHUA, 0x4008, 0, 1, 0x40000000U, 0);
    const std::int32_t duration = 120 + firstFrames + secondFrames + thirdFrames;
    EVT_AT_SetPoint(MAKI_RYO, MAKI_SHENHUA, 21, 3, 1, firstFrames + 80, duration, 0x08000000U);
    EVT_AT_SetPoint(MAKI_SHENHUA, MAKI_RYO, 5, 3, 1, 20, duration, 0);
    EVT_AT_SetFaceLooks(MAKI_SHENHUA, FacialExpressionId::Normal, 8, 1, duration);
    return duration;
}
std::int32_t MakiDiscussEmptyHands() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    MakiCampfireCamera(2500, 0);
    camCtrlDisableFilmBars();
    MakiCampfireCamera(2504, 60);
    MakiCampfireCamera(2505, 120);
    MakiCampfireCamera(2506, 210);
    EVT_AT_SetPos(MAKI_RYO, EVENT_CHARACTER_INVALID, {.x = 156.8f, .y = 100, .z = 390.7f}, 0);
    EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, 130, 0);
    EVT_AT_SetPos(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, {.x = 154.9f, .y = 100, .z = 393.5f}, 0);
    EVT_AT_SetAngY(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, 0, 0);
    EVT_AT_SetMot(MAKI_RYO, 0xf005, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(MAKI_RYO, 0xf003, 0, 1, 0, 30);
    EVT_AT_SetMot(MAKI_SHENHUA, 0x4008, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice("0518B029", 60); // @text EN: "What happened?\nCouldn't you find any firewood?" | JA: "どうしたの？　リョウ…\nたき木、なかったの？"
    EVT_AT_SetVoice("0518A034", 180); // @text EN: "No=@" | JA: "いや…"
    EVT_AT_SetVoice("0518B030", 210); // @text EN: "Look carefully." | JA: "探せば、きっとあるはず。"
    EVT_AT_SetVoice("0518B031", 260); // @text EN: "Please look again, Ryo." | JA: "お願いね、リョウ。"
    EVT_AT_SetVoice("0518A035", 320); // @text EN: "Yeah, I know." | JA: "ああ、わかってる…"
    EVT_AT_SetPoint(MAKI_SHENHUA, MAKI_RYO, 5, 3, 1, 20, 360, 0);
    EVT_AT_SetFaceLooks(MAKI_SHENHUA, FacialExpressionId::Normal, 8, 1, 360);
    return 360;
}
} // namespace shenmue::s2::areas
