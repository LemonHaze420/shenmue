#include "s2/areas/area_qabt.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
namespace shenmue::s2::areas {
namespace {
void Position(shen_name character, shen_name parent, float x, float y, float z, float yaw) {
    EventSetPositionAngle request{.cleanup = 0, .character = character, .parent = parent, .position = {x, y, z}, .degrees = yaw, .delay = 0};
    EVT_AT_SetPosAng_Callback(&request);
}
void BeginMovingCrate(ArbtCrateWork &work, std::int32_t delay) {
    if (!work.moveTask) {
        work.moveFrames = 100000000;
        work.moveTask = HLib::EnqueueTaskWithParameter(ArbtMoveBox, 3, 0x0b, sizeof(ArbtCrateWork *), AREA_EVENT_TASK);
        *static_cast<ArbtCrateWork **>(HLib::GetTaskParameter(work.moveTask)) = &work;
        HLib::SetupTimedTask(work.moveTask, delay);
    }
}
} // namespace
std::int32_t QabtIntroduceJob() {
    Position(QABT_AW2, ARBT_BASE, -2.9f, 0, 3.1f, 10);
    Position(QABT_RNP, ARBT_BASE, -4.1f, 0, -0.1f, -40);
    Position(ARBT_RYO, QABT_RNP, 0, 0, -1.5f, 180);
    EVT_AT_SetMot(ARBT_RYO, 11124, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_RNP, 11075, 2, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1101, ARBT_BASE, ARBT_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0360B001", 1, 30); // @text EN: "Well then, get started\nand carry the crates." | JA: "じゃあ、さっそく\nそこの荷物運んで。"
    return 120;
}
std::int32_t QabtExplainFirstCarry() {
    Position(QABT_AW2, ARBT_DUMB, 0, 0, 0, 0);
    Position(ARBT_RYO, QABT_AW2, 0, 0, -1.8f, 180);
    EVT_AT_SetMot(ARBT_RYO, 11137, 2, 1.2f, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_AW2, 11102, 2, 1.2f, 0x40000000U, 0);
    EVT_AT_SetCam(1127, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1104, ARBT_DUMT, ARBT_DUMT, 0, 0, 150);
    EVT_AT_SetCam(1114, ARBT_DUMT, ARBT_DUMT, 0, 0, 230);
    EVT_AT_SetCam(1115, ARBT_DUMT, ARBT_DUMT, 0, 0, 400);
    EVT_AT_SetCam(1128, ARBT_DUMT, ARBT_DUMT, 0, 0, 430);
    EVT_AT_SetCam(1103, ARBT_DUMT, ARBT_DUMT, 0, 0, 530);
    EVT_AT_SetVoice("0360C001", 1, 50); // @text EN: "I'm Congo.\nLet's get to it." | JA: "オレ、名前、コンゴ。\nよろしく。"
    EVT_AT_SetVoice("0360A002", 1, 150); // @text EN: "I'm Ryo Hazuki." | JA: "芭月です。こちらこそ。"
    EVT_AT_SetVoice("0360C002", 1, 230); // @text EN: "Let's start, okay?" | JA: "レッツ、スタート、ＯＫ？"
    EVT_AT_SetVoice("0360C003", 1, 310); // @text EN: "You listen and move as Congo say." | JA: "コンゴの指示聞く。\nその通りに動く。"
    EVT_AT_SetVoice("0360A001", 1, 400); // @text EN: "Okay." | JA: "はい。"
    EVT_AT_SetVoice("0360C004", 1, 430); // @text EN: "We carry the crates over there." | JA: "あそこに、がんばる、運ぶ。"
    EVT_AT_SetVoice("0360C005", 1, 530); // @text EN: "These things are heavy.\nDon't drop them." | JA: "これ、とてもヘビー。\n落とす、ダメ。"
    return 630;
}
std::int32_t QabtExplainCarry() {
    Position(QABT_AW2, ARBT_DUMB, 0, 0, 0, 0);
    Position(ARBT_RYO, QABT_AW2, 0, 0, -1.8f, 180);
    EVT_AT_SetMot(ARBT_RYO, 11112, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_AW2, 11103, 2, 1.2f, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_AW2, 11099, 2, 1.2f, 0, 140);
    EVT_AT_SetCam(1122, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1121, ARBT_DUMT, ARBT_DUMT, 0, 0, 160);
    EVT_AT_SetCam(1104, ARBT_DUMT, ARBT_DUMT, 0, 0, 250);
    EVT_AT_SetVoice("0360C006", 1, 0); // @text EN: "We'll try hard today, okay?" | JA: "コンゴ、今日も一緒にがんばる。"
    EVT_AT_SetVoice("0360C007", 1, 100); // @text EN: "We'll start!" | JA: "スタート、ＯＫ？"
    EVT_AT_SetVoice("0360C004", 1, 160); // @text EN: "We carry the crates over there." | JA: "あそこに、がんばる、運ぶ。"
    EVT_AT_SetVoice("0360A001", 1, 250); // @text EN: "Okay." | JA: "はい。"
    return 284;
}
std::int32_t QabtIntroduceNextCrate() {
    Position(QABT_AW2, ARBT_DUMB, 0, 0, 0, 0);
    Position(ARBT_RYO, QABT_AW2, 0, 0, -1.8f, 180);
    EVT_AT_SetMot(ARBT_RYO, 11112, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_AW2, 11080, 2, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1103, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice("0360C008", 1, 0); // @text EN: "Okay, we carry this next one." | JA: "ＯＫ、次、コレ運ぶ。"
    EVT_AT_SetVoice("0360A001", 1, 80); // @text EN: "Okay." | JA: "はい。"
    return 110;
}
std::int32_t QabtStartWork(std::int32_t delay) {
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, delay);
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, delay + 160);
    EVT_AT_SetPosAng(QABT_AW2, ARBT_RYO, 0, -1.95f, 0, delay + 160);
    EVT_AT_SetCam(1123, ARBT_DUMT, ARBT_DUMT, 0, 0, delay);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, delay + 160);
    EVT_AT_SetVoice("0360C010", 1, delay); // @text EN: "When Congo say nothing,\npush forward. Push." | JA: "コンゴ、指示しないとき、\n前にプッシュ、プッシュね。"
    EVT_AT_SetVoice("0360A001", 1, delay + 130); // @text EN: "Okay." | JA: "はい。"
    return delay + 160;
}
std::int32_t QabtLiftHighCrate(ArbtCrateWork &work) {
    Position(QABT_AW2, ARBT_DUMB, 0, 0, 0.15f, 180);
    Position(ARBT_RYO, QABT_AW2, 0, 0, 1.95f, 0);
    EVT_AT_SetMot(ARBT_RYO, 11118, 2, 1.5f, 0x40000000U, 0);
    const std::int32_t frames = EVT_AT_SetMot(QABT_AW2, 11082, 2, 1.5f, 0x40000000U, 0);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 17, 1);
    QueueEventRepeatedFunction(ArbtTrackLiftHand, nullptr, 20, 30);
    BeginMovingCrate(work, 50);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetVoice("0360C009", 1, 20); // @text EN: " " | JA: "フンッ！"
    EVT_AT_SetSound(255, 20);
    return frames;
}
std::int32_t QabtLiftLowCrate(ArbtCrateWork &work) {
    Position(QABT_AW2, ARBT_DUMB, 0, 0, 0.15f, 180);
    Position(ARBT_RYO, QABT_AW2, 0, 0, 1.95f, 0);
    EVT_AT_SetMot(ARBT_RYO, 11117, 5, 1.5f, 0x40000000U, 0);
    const std::int32_t frames = EVT_AT_SetMot(QABT_AW2, 11081, 2, 1.5f, 0x40000000U, 0);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 17, 1);
    QueueEventRepeatedFunction(ArbtTrackLiftHand, nullptr, 59, 30);
    BeginMovingCrate(work, 89);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetVoice("0360C009", 1, 60); // @text EN: " " | JA: "フンッ！"
    EVT_AT_SetSound(255, 60);
    return frames;
}
std::int32_t QabtPutHighCrate(ArbtCrateWork &work, std::int32_t streak, shen_name destination) {
    Position(QABT_AW2, destination, -0.9f, 0, 0.1f, 90);
    Position(ARBT_RYO, QABT_AW2, 0, 0, 1.8f, 0);
    EVT_AT_SetAngY(QABT_AW2, QABT_AW2, 180, 120);
    EVT_AT_SetMot(ARBT_RYO, 11130, 2, 1.6f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 11112, 2, 1, 0x40000000U, 120);
    EVT_AT_SetMot(QABT_AW2, 11094, 2, 1.6f, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_AW2, 11103, 2, 1, 0x40000000U, 120);
    if (work.moveTask)
        work.moveFrames = 65;
    const std::int32_t frames = QabtPraiseCarry(streak, 120);
    EVT_AT_SetCam(1111, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetCam(1130, ARBT_DUMT, ARBT_DUMT, 0, 0, 120);
    EVT_AT_SetCam(1131, ARBT_DUMA, ARBT_DUMA, 0, 0, frames + 55);
    EVT_AT_SetVoice("0360C009", 1, 50); // @text EN: " " | JA: "フンッ！"
    EVT_AT_SetVoice("0360C031", 1, frames); // @text EN: "Okay.\nGet the next one." | JA: "ＯＫ、次いくよ。"
    EVT_AT_SetVoice("0360A001", 1, frames + 60); // @text EN: "Okay." | JA: "はい。"
    EVT_AT_SetSound(256, 35);
    return frames + 90;
}
std::int32_t QabtPutLowCrate(ArbtCrateWork &work, std::int32_t streak, shen_name destination) {
    Position(QABT_AW2, destination, -0.9f, 0, 0.2f, 90);
    Position(ARBT_RYO, QABT_AW2, 0, 0, 1.85f, 0);
    EVT_AT_SetAngY(QABT_AW2, QABT_AW2, 180, 225);
    EVT_AT_SetMot(ARBT_RYO, 11129, 2, 1.5f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 11112, 2, 1, 0x40000000U, 225);
    EVT_AT_SetMot(QABT_AW2, 11093, 2, 1.5f, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_AW2, 11103, 2, 1, 0x40000000U, 225);
    if (work.moveTask)
        work.moveFrames = 61;
    const std::int32_t frames = QabtPraiseCarry(streak, 225);
    EVT_AT_SetCam(1132, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetCam(1130, ARBT_DUMT, ARBT_DUMT, 0, 0, 225);
    EVT_AT_SetCam(1131, ARBT_DUMA, ARBT_DUMA, 0, 0, frames + 55);
    EVT_AT_SetVoice("0360C009", 1, 50); // @text EN: " " | JA: "フンッ！"
    EVT_AT_SetVoice("0360C031", 1, frames); // @text EN: "Okay.\nGet the next one." | JA: "ＯＫ、次いくよ。"
    EVT_AT_SetVoice("0360A001", 1, frames + 60); // @text EN: "Okay." | JA: "はい。"
    EVT_AT_SetSound(256, 65);
    return frames + 90;
}
std::int32_t QabtPraiseCarry(std::int32_t streak, std::int32_t delay) {
    const float random = GetRandomNumberInRange(100);
    std::int32_t end;
    const char *voice;
    if (streak == 1) {
        end = delay + 110;
        voice = random < 50 ? "0360C063" : "0360C064"; // @text 0360C063 EN: "Perfect timing.\nNice work." | JA: "タイミング、ピッタリ。\nナイスね、ボーイ。" ; 0360C064 EN: "Okay.\nYou're getting it, boy." | JA: "ＯＫ、感じつかめてきたね。\nボーイ。"
    } else if (streak == 2) {
        end = delay + 105;
        voice = random < 50 ? "0360C066" : "0360C067"; // @text 0360C066 EN: "Congo and boy make a good pair." | JA: "コンゴとボーイ、\nナイス・コンビね。" ; 0360C067 EN: "Good!\nThat's the timing, boy!" | JA: "グッド！\nそのタイミングよ、ボーイ。"
    } else if (streak == 4) {
        end = delay + 90;
        voice = "0360C070"; // @text EN: "Boy, you're a good partner." | JA: "ボーイ、\nユー・グッド・パートナー！"
    } else
        return delay;
    EVT_AT_SetVoice(voice, 1, delay);
    EVT_AT_SetFaceLooks(QABT_AW2, 3, 30, 1, delay);
    EVT_AT_SetFaceLooks(QABT_AW2, 0, 30, 1, end);
    return end;
}
namespace {
void BeginCarryWarning(std::uint32_t motion) {
    EVT_AT_SetMot(ARBT_RYO, 61442, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_AW2, motion, 2, 1, 0x40000000U, 0);
}
void FinishCarryWarning(std::int32_t frames) {
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QABT_AW2), FacialExpressionId::Angry, 30, 1);
    EVT_AT_SetFaceLooks(QABT_AW2, 0, 30, 1, frames);
}
} // namespace
std::int32_t QabtWarnCarryOnce() {
    BeginCarryWarning(11104);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 60 : random < 60 ? 110 : 50;
    EVT_AT_SetCam(random < 60 ? 1125 : 1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0360C040" : random < 60 ? "0360C032" : "0360C033", 1, 0); // @text 0360C040 EN: "What are you doing?" | JA: "ヘイ、何やってるね！" ; 0360C032 EN: "Oh no! It's going to break!\nCongo'll get in trouble!" | JA: "オー・ノー！　こわれる、\nコンゴ、怒られる。" ; 0360C033 EN: "Don't drop it!" | JA: "落とす、ダメよ！"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t QabtWarnCarryTwice() {
    BeginCarryWarning(11105);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 50 ? 160 : random < 60 ? 150 : 185;
    EVT_AT_SetCam(random < 60 ? 1125 : 1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1112, ARBT_DUMT, ARBT_DUMT, 0, 0, 100);
    EVT_AT_SetVoice(random < 50 ? "0360C034" : random < 60 ? "0360C038" : "0360C040", 1, 0); // @text 0360C034 EN: "Oh no!\nVery bad!" | JA: "オー・ノー！\nベリー・バッド！" ; 0360C038 EN: "Hey, hey boy=@" | JA: "ヘイヘイ、ボーイ…" ; 0360C040 EN: "What are you doing?" | JA: "ヘイ、何やってるね！"
    EVT_AT_SetVoice(random < 50 ? "0360C035" : random < 60 ? "0360C036" : "0360C043", 1, random < 50 ? 90 : random < 60 ? 70 : 75); // @text 0360C035 EN: "Hold it steady, okay?" | JA: "しっかり持つ、ＯＫ？" ; 0360C036 EN: "Use your strength!\nStrength!" | JA: "もっと、パワー！　パワー！" ; 0360C043 EN: "We won't get paid this way." | JA: "それじゃ、マネー、出せない。\nユー・ＯＫ！"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t QabtWarnCarryThird() {
    BeginCarryWarning(11105);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 150 : random < 60 ? 200 : 230;
    EVT_AT_SetCam(random < 30 ? 1109 : 1124, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0360C040" : random < 60 ? "0360C039" : "0360C044", 1, 0); // @text 0360C040 EN: "What are you doing?" | JA: "ヘイ、何やってるね！" ; 0360C039 EN: "Oh no!\nYou dropped it again." | JA: "オー・ノー！\nまた落としたね。" ; 0360C044 EN: "You're making too many mistakes!" | JA: "ユー、ミステイク、多すぎね！"
    EVT_AT_SetVoice(random < 30 ? "0360C037" : random < 60 ? "0360C047" : "0360C042", 1, random < 30 ? 70 : random < 60 ? 100 : 90); // @text 0360C037 EN: "Carry it with care!" | JA: "気をつけて持つ、基本よ！" ; 0360C047 EN: "We're not getting our job done.\nCongo gonna get in trouble." | JA: "仕事、進まない。\nコンゴ、怒られる。" ; 0360C042 EN: "These are precious. We'll\nget in trouble with boss." | JA: "大事なコンテナ。\n姐さん、怒る、恐い"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t QabtWarnCarryFourth() {
    BeginCarryWarning(11106);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 220 : random < 60 ? 210 : 220;
    EVT_AT_SetCam(random < 30 ? 1109 : 1124, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0360C046" : random < 60 ? "0360C048" : "0360C044", 1, 0); // @text 0360C046 EN: "Oh no!\nYou're always dropping it!" | JA: "オー・ノー！\n落としすぎよ！" ; 0360C048 EN: "Don't drop it!\nWe won't get done." | JA: "落とす、ダメ！\n仕事、終わらない。" ; 0360C044 EN: "You're making too many mistakes!" | JA: "ユー、ミステイク、多すぎね！"
    EVT_AT_SetVoice(random < 30 ? "0360C050" : random < 60 ? "0360C043" : "0360C042", 1, random < 30 ? 90 : random < 60 ? 100 : 90); // @text 0360C050 EN: "If you drop it, we don't get our\njob done. Get serious and push!" | JA: "落とす、仕事、終わらない。\nマジメにプッシュ！" ; 0360C043 EN: "We won't get paid this way." | JA: "それじゃ、マネー、出せない。\nユー・ＯＫ！" ; 0360C042 EN: "These are precious. We'll\nget in trouble with boss." | JA: "大事なコンテナ。\n姐さん、怒る、恐い"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t QabtWarnCarryRepeatedly() {
    BeginCarryWarning(11107);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 220 : 250;
    EVT_AT_SetCam(random < 30 ? 1109 : 1124, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0360C045" : random < 60 ? "0360C048" : "0360C039", 1, 0); // @text 0360C045 EN: "Never drop them!\nNever!" | JA: "落とす、ダメ！\n絶対にダメ！" ; 0360C048 EN: "Don't drop it!\nWe won't get done." | JA: "落とす、ダメ！\n仕事、終わらない。" ; 0360C039 EN: "Oh no!\nYou dropped it again." | JA: "オー・ノー！\nまた落としたね。"
    EVT_AT_SetVoice(random < 30 ? "0360C051" : random < 60 ? "0360C052" : "0360C041", 1, random < 30 ? 90 : 100); // @text 0360C051 EN: "The crate's heavy.\nYou gotta show Congo your guts." | JA: "コンテナ、とてもヘビー。\nユーのガッツでカバーね！" ; 0360C052 EN: "Use your strength.\nSteady your steps." | JA: "もっと、パワー、出す。\n体、フラつく、ダメダメ！" ; 0360C041 EN: "You move along with Congo, okay?\nShow me your guts!" | JA: "コンゴに、動き合わせる。ＯＫ？\nもっとガッツ出す！"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t QabtWarnWrongForward() {
    BeginCarryWarning(11104);
    const float cameraRandom = GetRandomNumberInRange(100);
    EVT_AT_SetCam(cameraRandom < 60 ? 1125 : 1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    const float voiceRandom = GetRandomNumberInRange(100);
    EVT_AT_SetVoice(voiceRandom < 50 ? "0360C029" : "0360C030", 1, 0); // @text 0360C029 EN: "Don't push in other directions!\nForward!" | JA: "他にプッシュ、ダメ！\n前、前！" ; 0360C030 EN: "Push forward!\nNot anywhere else!" | JA: "前にプッシュね！\n他はノー・プッシュ！"
    FinishCarryWarning(100);
    return 100;
}
std::int32_t QabtWarnWrongDouble(std::uint32_t direction) {
    BeginCarryWarning(11104);
    GetRandomNumberInRange(100);
    const std::int32_t frames = direction == 0x8000 ? 140 : 155;
    EVT_AT_SetCam(1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(direction == 0x8000 ? "0360C086" : "0360C087", 1, 0); // @text 0360C086 EN: "When Congo says, “right, right,”\nyou push right twice." | JA: "コンゴ、右右言う、\nボーイは右に２回プッシュね。" ; 0360C087 EN: "When Congo says, “left, left,”\nyou push left twice." | JA: "コンゴ、左左言う、\nボーイは左に２回プッシュね。"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t QabtWarnWrongMultiple() {
    BeginCarryWarning(11104);
    GetRandomNumberInRange(100);
    EVT_AT_SetCam(1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice("0360C089", 1, 0); // @text EN: "Push in the direction Congo say\nfor the same number of times." | JA: "コンゴ、指示するカウントだけ、\nそこにプッシュよ。"
    FinishCarryWarning(110);
    return 110;
}
std::int32_t QabtWarnIdleForward(std::int32_t) {
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 0);
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 90);
    EVT_AT_SetCam(1123, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 130);
    EVT_AT_SetVoice("0360C028", 1, 30); // @text EN: "Don't forget!\nPush forward!" | JA: "忘れる、ダメ！\n前にプッシュよ！"
    FinishCarryWarning(150);
    return 150;
}
std::int32_t QabtWarnIdlePut(std::int32_t count) {
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 0);
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 90);
    EVT_AT_SetCam(1123, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 130);
    EVT_AT_SetVoice(count <= 3 ? "0360C092" : "0360C091", 1, 30); // @text 0360C092 EN: "Put it down." | JA: "下にダウンよ。" ; 0360C091 EN: "Hey, down.\nPut it down." | JA: "ヘイ！　ダウンよ、ダウン。"
    FinishCarryWarning(150);
    return 150;
}
std::int32_t QabtWalkForward(float speed, std::int32_t delay) {
    EVT_AT_SetMot(ARBT_RYO, 11115, 2, speed, 0, delay);
    EVT_AT_SetMot(QABT_AW2, 11078, 2, speed, 0, delay);
    EVT_AT_SetSound(254, delay + 20);
    return static_cast<std::int32_t>(115.0 / static_cast<double>(speed));
}
std::int32_t QabtQueueDirection(std::uint32_t direction, std::int32_t intensity, std::int32_t delay) {
    switch (intensity) {
    case 0:
    case 1:
    case 2:
        EVT_AT_SetVoice(direction == 0x4000 ? "0360C014" : direction == 0x8000 ? "0360C015" : "0360C013", 1, delay); // @text 0360C014 EN: "Left!" | JA: "左よ！" ; 0360C015 EN: "Right!" | JA: "右よ！" ; 0360C013 EN: "Forward!" | JA: "前ね！"
        break;
    case 3:
        EVT_AT_SetVoice(direction == 0x8000 ? "0360C080" : "0360C083", 1, delay); // @text 0360C080 EN: "Right, right!" | JA: "右右！" ; 0360C083 EN: "Left, left!" | JA: "左左！"
        break;
    case 4:
        EVT_AT_SetVoice(direction == 0x8000 ? "0360C081" : "0360C084", 1, delay); // @text 0360C081 EN: "Right, right, right!" | JA: "右右右！" ; 0360C084 EN: "Left, left, left!" | JA: "左左左！"
        break;
    case 5:
        EVT_AT_SetVoice(direction == 0x8000 ? "0360C082" : "0360C085", 1, delay); // @text 0360C082 EN: "Right, right, right, right!" | JA: "右右右右！" ; 0360C085 EN: "Left, left, left, left!" | JA: "左左左左！"
        break;
    case 6:
    case 7:
        GetRandomNumberInRange(100);
        EVT_AT_SetVoice("0360C017", 0, delay); // @text EN: " " | JA: "フンッ！！"
        return 52;
    default:
        break;
    }
    return delay;
}
std::int32_t QabtWalkDirection(float speed, std::int32_t intensity, std::uint32_t direction) {
    EVT_AT_SetPosAng(QABT_AW2, ARBT_RYO, 0, -1.95f, 0, 0);
    std::uint32_t ryo, worker;
    std::int32_t frames;
    if (direction == 0x4000) {
        ryo = intensity ? 11119 : 11121;
        worker = intensity ? 11084 : 11086;
        frames = static_cast<std::int32_t>((intensity ? 90.0f : 100.0f) / speed);
    } else if (direction == 0x8000) {
        ryo = intensity ? 11120 : 11133;
        worker = intensity ? 11085 : 11097;
        frames = static_cast<std::int32_t>(90.0f / speed);
    } else {
        ryo = 11115;
        worker = 11078;
        frames = static_cast<std::int32_t>(115.0f / speed);
    }
    EVT_AT_SetMot(ARBT_RYO, ryo, 2, speed, 0, 0);
    EVT_AT_SetMot(QABT_AW2, worker, 2, speed, 0x02000000U, 0);
    return frames;
}
std::int32_t QabtFailCarry(ArbtCrateWork &work, shen_name crate, std::int32_t intensity, std::uint32_t direction) {
    Vector3 player, position;
    Vector3i rotation;
    EV_GetCharaPosition(ARBT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::Local);
    EV_GetCharaPosition(crate, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(crate, &rotation, EventTransformFlags::Local);
    if (direction == 0x4000) {
        position.x += intensity ? -0.38026428f : 0.11395264f;
        position.z += intensity ? 0.644867f : 0.65039098f;
        EVT_AT_SetMot(ARBT_RYO, 11127, 2, 1.4f, 0, 0);
        EVT_AT_SetMot(QABT_AW2, 11091, 2, 1.4f, 0, 12);
    } else if (direction == 0x8000) {
        position.x += intensity ? -0.3815918f : 0.11235046f;
        position.z += intensity ? -0.42904603f : -0.40728712f;
        EVT_AT_SetMot(ARBT_RYO, 11128, 2, 1.4f, 0, 0);
        EVT_AT_SetMot(QABT_AW2, 11092, 2, 1.4f, 0, 12);
    } else {
        if (direction == 0x1000) {
            position.x = (player.x - 0.58590698f) - 0.9f;
            position.z = player.z + 0.16f;
        } else {
            position.x += 0.11660004f;
            position.z += 0.11210001f;
        }
        EVT_AT_SetMot(ARBT_RYO, 11125, 25, 1.4f, 0, 0);
        EVT_AT_SetMot(QABT_AW2, 11089, 25, 1.4f, 0, 0);
    }
    if (work.moveTask)
        work.moveFrames = 37;
    QueueArbtDropBox(work, 37);
    EVT_AT_SetPosAng(crate, ARBT_BASE, position.x, position.z, static_cast<float>(rotation.y), 61);
    Position(QABT_AW2, QABT_AW2, -0.1f, 0, 0, 0.35f);
    EVT_AT_SetPosAng(QABT_AW2, ARBT_DUMB, 0, 0, 0, 61);
    EVT_AT_SetPosAng(ARBT_RYO, QABT_AW2, 0, -1.95f, 180, 61);
    EVT_AT_SetCam(1107, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    return 60;
}
std::int32_t QabtAnnounceTimeOver() {
    Position(QABT_RNP, ARBT_BASE, -4.1f, 0, -0.1f, 140);
    EVT_AT_SetMot(QABT_RNP, 11073, 2, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1001, ARBT_DUMS, ARBT_DUMS, 0, 0, 0);
    Loop3DSoundEffect(1577);
    EVT_AT_SetVoice("0360B005", 1, 0); // @text EN: "Okay, that's it for today.\nEveryone can stop now." | JA: "はいはい、今日はここまでよ！\nみんな上がってちょうだい。"
    EVT_AT_SetVoice("0360C061", 1, 215); // @text EN: "Yes!" | JA: "うぃーす！"
    return 260;
}
namespace {
std::int32_t PayRyo(std::int32_t cash, std::int32_t crates, const char *opening, const char *assessment, const char *amount, const char *farewell) {
    PATI_StartMoneyCount(static_cast<std::uint32_t>(cash), 220);
    EVT_AT_ObjectDisp_1(ARBT_PAY, 120, 0);
    EVT_AT_ObjectDisp_1(ARBT_PAY, 240, 1);
    Position(QABT_RNP, ARBT_BASE, 27.1f, 0, -28.25f, 140);
    Position(QABT_AW2, QABT_RNP, -2, 0, -0.6f, -90);
    Position(ARBT_RYO, QABT_RNP, 0, 0, -1.5f, 180);
    EVT_AT_SetPos(ARBT_KNT, QABT_RNP, {5.4f, 0, -0.4f}, 0);
    EVT_AT_SetAngY(ARBT_KNT, QABT_RNP, 90, 0);
    EVT_AT_SetPos(ARBT_AA1, QABT_RNP, {4.2f, 0, 1.14f}, 0);
    EVT_AT_SetAngY(ARBT_AA1, QABT_RNP, 60, 0);
    EVT_AT_SetPos(ARBT_LYO, QABT_RNP, {1.4f, 0, 1.16f}, 0);
    EVT_AT_SetAngY(ARBT_LYO, QABT_RNP, -30, 0);
    EVT_AT_SetPos(ARBT_HAK, QABT_RNP, {1.58f, 0, 0.13f}, 0);
    EVT_AT_SetAngY(ARBT_HAK, QABT_RNP, -100, 0);
    EVT_AT_SetPos(ARBT_EIN, QABT_RNP, {3.6f, 0, -2.3f}, 0);
    EVT_AT_SetAngY(ARBT_EIN, QABT_RNP, 1, 0);
    EVT_AT_SetAngY(QABT_RNP, QABT_RNP, 180, 110);
    EVT_AT_SetAngY(QABT_RNP, QABT_RNP, 180, 320);
    EVT_AT_SetAngY(ARBT_DUMS, ARBT_DUMS, 180, 0);
    EVT_AT_SetAngY(ARBT_DUMS, ARBT_DUMS, 180, 110);
    EVT_AT_SetAngY(ARBT_DUMS, ARBT_DUMS, 180, 320);
    EVT_AT_SetMot(QABT_AW2, 11075, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 61442, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 11123, 2, 1, 0, 110);
    EVT_AT_SetMot(QABT_RNP, 11075, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(QABT_RNP, 11088, 2, 1, 0, 110);
    EVT_AT_SetMot(QABT_RNP, 11103, 2, 1, 0, 320);
    for (std::int32_t index = 0; index < 5; ++index)
        EVT_AT_SetMot(ARBT_PAY_CROWD[index], 11139 + index, 2, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1116, ARBT_DUMS, ARBT_DUMS, 0, 0, 0);
    EVT_AT_SetCam(1117, ARBT_DUMS, ARBT_DUMS, 0, 0, 180);
    EVT_AT_SetCam(1118, ARBT_DUMA, ARBT_DUMA, 0, 0, 240);
    EVT_AT_SetCam(1119, ARBT_DUMA, ARBT_DUMA, 0, 0, 360);
    EVT_AT_SetCam(1120, ARBT_DUMS, ARBT_DUMS, 0, 0, 415);
    EVT_AT_SetVoice(opening, 1, 0);
    EVT_AT_SetVoice(assessment, 1, 100);
    EVT_AT_SetVoice(amount, 1, 190);
    std::int32_t frames = 1;
    if (crates) {
        static constexpr const char *voices[]{"0360B027", "0360B028", "0360B029", "0360B030", "0360B031", "0360B032"}; // @text 0360B027 EN: "Carried one,\nso here's $10." | JA: "１個運んだから１０ドルだね。" ; 0360B028 EN: "You carried two.\nHere's $20." | JA: "２個運んだから２０ドルだね。" ; 0360B029 EN: "You carried three.\nHere's $30." | JA: "３個運んだから３０ドルだね。" ; 0360B030 EN: "You carried four.\nHere's $40." | JA: "４個運んだから４０ドルだね。" ; 0360B031 EN: "You carried five.\nHere's $50." | JA: "５個運んだから５０ドルだね。" ; 0360B032 EN: "You carried six.\nHere's $60." | JA: "６個運んだから６０ドルだね。"
        EVT_AT_SetVoice(voices[crates - 1], 1, 280);
        frames = 100;
    }
    EVT_AT_SetVoice("0360A009", 1, frames + 280); // @text EN: "Thanks." | JA: "どうも…"
    EVT_AT_SetVoice(farewell, 1, frames + 320);
    EVT_AT_SetVoice("0360A001", 1, frames + 390); // @text EN: "Okay." | JA: "はい。"
    return frames + 440;
}
} // namespace
std::int32_t QabtPayFirstJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0360B006", "0360B007", "0360B008", "0360B009"); // @text 0360B006 EN: "Good work." | JA: "はい、おつかれさん！" ; 0360B007 EN: "Your first day.\nYou look pretty worn out." | JA: "初めてでだいぶ疲れたみたいね。" ; 0360B008 EN: "Here's your pay for the day." | JA: "ほら、今日のバイト代よ。" ; 0360B009 EN: "Come and help us again." | JA: "また手伝ってちょうだい。"
}
std::int32_t QabtPayGoodJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0360B010", "0360B011", "0360B008", "0360B009"); // @text 0360B010 EN: "You worked pretty hard today." | JA: "あんた、けっこうがんばったね。" ; 0360B011 EN: "Keep up the good work." | JA: "その調子でまた頼むわ。" ; 0360B008 EN: "Here's your pay for the day." | JA: "ほら、今日のバイト代よ。" ; 0360B009 EN: "Come and help us again." | JA: "また手伝ってちょうだい。"
}
std::int32_t QabtPayAverageJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0360B012", "0360B013", "0360B008", "0360B014"); // @text 0360B012 EN: "A tough day." | JA: "ご苦労さん。" ; 0360B013 EN: "But you've got to work harder next time." | JA: "次はもう少し気合い入れてくれない？" ; 0360B008 EN: "Here's your pay for the day." | JA: "ほら、今日のバイト代よ。" ; 0360B014 EN: "Come help us again." | JA: "また手伝ってね。"
}
std::int32_t QabtPayPoorJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0360B015", "0360B016", "0360B017", "0360B018"); // @text 0360B015 EN: "I'm not too impressed, you know=@" | JA: "困るのよね、あんなんじゃ…" ; 0360B016 EN: "You've got to be more serious.\nGet your act together!" | JA: "ちゃんとやってくれないと…" ; 0360B017 EN: "I will pay you, though." | JA: "まあバイト代は出すけど…" ; 0360B018 EN: "Try harder next time." | JA: "次はしっかりね。"
}
} // namespace shenmue::s2::areas
