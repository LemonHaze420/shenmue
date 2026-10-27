#include "s2/areas/area_arbt.hpp"
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
void ArbtTrackLiftHand(void *) {
    Vector3 position;
    EV_GetCharaPosition(ARBT_EG0L, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 17, 0.01f);
    LatchMotionBoneTarget(scenFindMotionwork(ARBT_RYO), 17, &position);
}
std::int32_t ArbtIntroduceJob() {
    Position(ARBT_TKR, ARBT_BASE, -2.9f, 0, 3.1f, 10);
    Position(ARBT_QLN, ARBT_BASE, -4.1f, 0, -0.1f, -40);
    Position(ARBT_RYO, ARBT_QLN, 0, 0, -1.5f, 180);
    EVT_AT_SetMot(ARBT_RYO, 4841, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_QLN, 4802, 2, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1101, ARBT_BASE, ARBT_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0018B001", 1, 30); // @text EN: "Well, you can start right away\nand carry these crates." | JA: "じゃ、さっそく\nそこの荷物運んでくれ。"
    return 120;
}
std::int32_t ArbtExplainFirstCarry() {
    Position(ARBT_TKR, ARBT_DUMB, 0, 0, 0, 0);
    Position(ARBT_RYO, ARBT_TKR, 0, 0, -1.8f, 180);
    EVT_AT_SetMot(ARBT_RYO, 4851, 2, 1.2f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_TKR, 4824, 2, 1.2f, 0x40000000U, 0);
    EVT_AT_SetCam(1127, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1104, ARBT_DUMT, ARBT_DUMT, 0, 0, 150);
    EVT_AT_SetCam(1114, ARBT_DUMT, ARBT_DUMT, 0, 0, 230);
    EVT_AT_SetCam(1115, ARBT_DUMT, ARBT_DUMT, 0, 0, 400);
    EVT_AT_SetCam(1128, ARBT_DUMT, ARBT_DUMT, 0, 0, 430);
    EVT_AT_SetCam(1103, ARBT_DUMT, ARBT_DUMT, 0, 0, 530);
    EVT_AT_SetVoice("0018C001", 1, 50); // @text EN: "I'm Delin.\nGood to see you." | JA: "俺は徳林ってんだ。よろしくな。"
    EVT_AT_SetVoice("0018A002", 1, 150); // @text EN: "Hi, I'm Ryo." | JA: "芭月です。こちらこそ。"
    EVT_AT_SetVoice("0018C002", 1, 230); // @text EN: "Let's get started then." | JA: "じゃあ、始めるとすっか。"
    EVT_AT_SetVoice("0018C003", 1, 310); // @text EN: "Move as I tell you to." | JA: "俺の指示に従って動いてくれ。"
    EVT_AT_SetVoice("0018A001", 1, 400); // @text EN: "Sure." | JA: "はい。"
    EVT_AT_SetVoice("0018C004", 1, 430); // @text EN: "Okay.\nWe're going to carry it over there." | JA: "よーし、あそこに運ぶぞ。"
    EVT_AT_SetVoice("0018C005", 1, 530); // @text EN: "It's heavy.\nDon't drop it." | JA: "重いからな。\n落とすんじゃないぞ。"
    return 630;
}
std::int32_t ArbtExplainCarry() {
    Position(ARBT_TKR, ARBT_DUMB, 0, 0, 0, 0);
    Position(ARBT_RYO, ARBT_TKR, 0, 0, -1.8f, 180);
    EVT_AT_SetMot(ARBT_RYO, 4831, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_TKR, 4825, 2, 1.2f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_TKR, 4822, 2, 1.2f, 0, 140);
    EVT_AT_SetCam(1122, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1121, ARBT_DUMT, ARBT_DUMT, 0, 0, 160);
    EVT_AT_SetCam(1104, ARBT_DUMT, ARBT_DUMT, 0, 0, 250);
    EVT_AT_SetVoice("0018C006", 1, 0); // @text EN: "Yo, good to see you again." | JA: "よう、またよろしく頼むぜ。"
    EVT_AT_SetVoice("0018C007", 1, 100); // @text EN: "Let's get started." | JA: "始めるとすっか。"
    EVT_AT_SetVoice("0018C004", 1, 160); // @text EN: "Okay.\nWe're going to carry it over there." | JA: "よーし、あそこに運ぶぞ。"
    EVT_AT_SetVoice("0018A001", 1, 250); // @text EN: "Sure." | JA: "はい。"
    return 284;
}
std::int32_t ArbtIntroduceNextCrate() {
    Position(ARBT_TKR, ARBT_DUMB, 0, 0, 0, 0);
    Position(ARBT_RYO, ARBT_TKR, 0, 0, -1.8f, 180);
    EVT_AT_SetMot(ARBT_RYO, 4831, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_TKR, 4807, 2, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1103, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice("0018C008", 1, 0); // @text EN: "Next,\nwe'll carry this one." | JA: "よし、今度はこいつだ。"
    EVT_AT_SetVoice("0018A001", 1, 80); // @text EN: "Sure." | JA: "はい。"
    return 110;
}
std::int32_t ArbtStartWork(std::int32_t delay) {
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, delay);
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, delay + 160);
    EVT_AT_SetPosAng(ARBT_TKR, ARBT_RYO, 0, -1.95f, 0, delay + 160);
    EVT_AT_SetCam(1123, ARBT_DUMT, ARBT_DUMT, 0, 0, delay);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, delay + 160);
    EVT_AT_SetVoice("0018C010", 1, delay); // @text EN: "Okay. Unless I tell you otherwise,\nkeep pushing forward." | JA: "よし、俺が指示する時以外は、\n前に押し続けてくれ。"
    EVT_AT_SetVoice("0018A001", 1, delay + 130); // @text EN: "Sure." | JA: "はい。"
    return delay + 160;
}
std::int32_t ArbtLiftHighCrate(ArbtCrateWork &work) {
    Position(ARBT_TKR, ARBT_DUMB, 0, 0, 0.15f, 180);
    Position(ARBT_RYO, ARBT_TKR, 0, 0, 1.95f, 0);
    EVT_AT_SetMot(ARBT_RYO, 4837, 2, 1.5f, 0x40000000U, 0);
    const std::int32_t frames = EVT_AT_SetMot(ARBT_TKR, 4809, 2, 1.5f, 0x40000000U, 0);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 17, 1);
    QueueEventRepeatedFunction(ArbtTrackLiftHand, nullptr, 20, 30);
    BeginMovingCrate(work, 50);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetVoice("0018C009", 1, 20); // @text EN: "Here." | JA: "よっ…と。"
    EVT_AT_SetSound(255, 20);
    return frames;
}
std::int32_t ArbtLiftLowCrate(ArbtCrateWork &work) {
    Position(ARBT_TKR, ARBT_DUMB, 0, 0, 0.15f, 180);
    Position(ARBT_RYO, ARBT_TKR, 0, 0, 1.95f, 0);
    EVT_AT_SetMot(ARBT_RYO, 4836, 5, 1.5f, 0x40000000U, 0);
    const std::int32_t frames = EVT_AT_SetMot(ARBT_TKR, 4808, 2, 1.5f, 0x40000000U, 0);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 17, 1);
    QueueEventRepeatedFunction(ArbtTrackLiftHand, nullptr, 59, 30);
    BeginMovingCrate(work, 89);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetVoice("0018C009", 1, 60); // @text EN: "Here." | JA: "よっ…と。"
    EVT_AT_SetSound(255, 60);
    return frames;
}
std::int32_t ArbtPutHighCrate(ArbtCrateWork &work, std::int32_t streak, shen_name destination) {
    Position(ARBT_TKR, destination, -0.9f, 0, 0.1f, 90);
    Position(ARBT_RYO, ARBT_TKR, 0, 0, 1.8f, 0);
    EVT_AT_SetAngY(ARBT_TKR, ARBT_TKR, 180, 120);
    EVT_AT_SetMot(ARBT_RYO, 4847, 2, 1.6f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 4831, 2, 1, 0x40000000U, 120);
    EVT_AT_SetMot(ARBT_TKR, 4819, 2, 1.6f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_TKR, 4825, 2, 1, 0x40000000U, 120);
    if (work.moveTask)
        work.moveFrames = 65;
    const std::int32_t frames = ArbtPraiseCarry(streak, 120);
    EVT_AT_SetCam(1111, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetCam(1130, ARBT_DUMT, ARBT_DUMT, 0, 0, 120);
    EVT_AT_SetCam(1131, ARBT_DUMA, ARBT_DUMA, 0, 0, frames + 55);
    EVT_AT_SetVoice("0018C009", 1, 50); // @text EN: "Here." | JA: "よっ…と。"
    EVT_AT_SetVoice("0018C031", 1, frames); // @text EN: "Okay, next!" | JA: "よし、次だ。"
    EVT_AT_SetVoice("0018A001", 1, frames + 60); // @text EN: "Sure." | JA: "はい。"
    EVT_AT_SetSound(256, 35);
    return frames + 90;
}
std::int32_t ArbtPutLowCrate(ArbtCrateWork &work, std::int32_t streak, shen_name destination) {
    Position(ARBT_TKR, destination, -0.9f, 0, 0.2f, 90);
    Position(ARBT_RYO, ARBT_TKR, 0, 0, 1.85f, 0);
    EVT_AT_SetAngY(ARBT_TKR, ARBT_TKR, 180, 225);
    EVT_AT_SetMot(ARBT_RYO, 4846, 2, 1.5f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 4831, 2, 1, 0x40000000U, 225);
    EVT_AT_SetMot(ARBT_TKR, 4818, 2, 1.5f, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_TKR, 4825, 2, 1, 0x40000000U, 225);
    if (work.moveTask)
        work.moveFrames = 61;
    const std::int32_t frames = ArbtPraiseCarry(streak, 225);
    EVT_AT_SetCam(1132, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    EVT_AT_SetCam(1130, ARBT_DUMT, ARBT_DUMT, 0, 0, 225);
    EVT_AT_SetCam(1131, ARBT_DUMA, ARBT_DUMA, 0, 0, frames + 55);
    EVT_AT_SetVoice("0018C009", 1, 50); // @text EN: "Here." | JA: "よっ…と。"
    EVT_AT_SetVoice("0018C031", 1, frames); // @text EN: "Okay, next!" | JA: "よし、次だ。"
    EVT_AT_SetVoice("0018A001", 1, frames + 60); // @text EN: "Sure." | JA: "はい。"
    EVT_AT_SetSound(256, 65);
    return frames + 90;
}
std::int32_t ArbtPraiseCarry(std::int32_t streak, std::int32_t delay) {
    const float random = GetRandomNumberInRange(100);
    std::int32_t end;
    const char *voice;
    if (streak == 1) {
        end = delay + 110;
        voice = random < 50 ? "0018C063" : "0018C064"; // @text 0018C063 EN: "We make the perfect pair." | JA: "だんだん息が合ってきたようだな。" ; 0018C064 EN: "We're looking pretty good." | JA: "だいぶ、いい感じに\nなってきたぞ。"
    } else if (streak == 2) {
        end = delay + 105;
        voice = random < 50 ? "0018C066" : "0018C067"; // @text 0018C066 EN: "We're a good pair, aren't we?" | JA: "俺たちゃ、\nいいコンビじゃねえか。" ; 0018C067 EN: "I think\nwe're getting along fine." | JA: "おめぇとは気が合うみてえだな。"
    } else if (streak == 4) {
        end = delay + 90;
        voice = "0018C070"; // @text EN: "We're working\nin perfect harmony." | JA: "気持ちも呼吸もぴったりだ。"
    } else
        return delay;
    EVT_AT_SetVoice(voice, 1, delay);
    EVT_AT_SetFaceLooks(ARBT_TKR, FacialExpressionId::Smile, 30, 1, delay);
    EVT_AT_SetFaceLooks(ARBT_TKR, FacialExpressionId::Normal, 30, 1, end);
    return end;
}
namespace {
void BeginCarryWarning(std::uint32_t motion) {
    EVT_AT_SetMot(ARBT_RYO, 61442, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_TKR, motion, 2, 1, 0x40000000U, 0);
}
void FinishCarryWarning(std::int32_t frames) {
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(ARBT_TKR), FacialExpressionId::Angry, 30, 1);
    EVT_AT_SetFaceLooks(ARBT_TKR, FacialExpressionId::Normal, 30, 1, frames);
}
} // namespace
std::int32_t ArbtWarnCarryOnce() {
    BeginCarryWarning(4826);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 60 : random < 60 ? 110 : 50;
    EVT_AT_SetCam(random < 60 ? 1125 : 1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0018C040" : random < 60 ? "0018C032" : "0018C033", 1, 0); // @text 0018C040 EN: "What are you doing?" | JA: "おい、何やってんだ！" ; 0018C032 EN: "I hope nothing's broken." | JA: "あちゃー\nこわれてなきゃいいがな。" ; 0018C033 EN: "Don't drop it!" | JA: "落とすなよ、おめぇー"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t ArbtWarnCarryTwice() {
    BeginCarryWarning(4827);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 50 ? 160 : random < 60 ? 150 : 185;
    EVT_AT_SetCam(random < 60 ? 1125 : 1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1112, ARBT_DUMT, ARBT_DUMT, 0, 0, 100);
    EVT_AT_SetVoice(random < 50 ? "0018C034" : random < 60 ? "0018C038" : "0018C040", 1, 0); // @text 0018C034 EN: "You dropped it." | JA: "あーあ、やっちまった…" ; 0018C038 EN: "Hey, hey.\nNot again=@" | JA: "おいおい、またか…" ; 0018C040 EN: "What are you doing?" | JA: "おい、何やってんだ！"
    EVT_AT_SetVoice(random < 50 ? "0018C035" : random < 60 ? "0018C036" : "0018C043", 1, random < 50 ? 90 : random < 60 ? 70 : 75); // @text 0018C035 EN: "Steady, boy!" | JA: "しっかり頼むぜ！" ; 0018C036 EN: "Be a man,\nshow some strength!" | JA: "男なら、リキ入れろよ！" ; 0018C043 EN: "You won't get paid if you keep\ndoing this! Wake up!" | JA: "そんなんじゃバイト代出ねえぞ。\nしっかりしろ！"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t ArbtWarnCarryThird() {
    BeginCarryWarning(4828);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 150 : random < 60 ? 200 : 230;
    EVT_AT_SetCam(random < 30 ? 1109 : 1124, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0018C040" : random < 60 ? "0018C039" : "0018C044", 1, 0); // @text 0018C040 EN: "What are you doing?" | JA: "おい、何やってんだ！" ; 0018C039 EN: "Oh man!\nAgain?" | JA: "なんてこった、またかよ…" ; 0018C044 EN: "How many times do you have\nto drop this?" | JA: "何度しくじったらわかるんだ！"
    EVT_AT_SetVoice(random < 30 ? "0018C037" : random < 60 ? "0018C047" : "0018C042", 1, random < 30 ? 70 : random < 60 ? 100 : 90); // @text 0018C037 EN: "Be careful!" | JA: "もっと慎重に持ってくれ！" ; 0018C047 EN: "You don't call this working!" | JA: "これじゃ仕事になんねえだろ！" ; 0018C042 EN: "This is valuable cargo!" | JA: "大事な荷物なんだ。\n親方にドヤされるぞ！"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t ArbtWarnCarryFourth() {
    BeginCarryWarning(4829);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 220 : random < 60 ? 210 : 220;
    EVT_AT_SetCam(random < 30 ? 1109 : 1124, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0018C046" : random < 60 ? "0018C048" : "0018C044", 1, 0); // @text 0018C046 EN: "I can't even keep count anymore!" | JA: "おいおい、これで何度目だ！" ; 0018C048 EN: "This is not getting us anywhere!" | JA: "ぜんぜん進まねえじゃねえか！" ; 0018C044 EN: "How many times do you have\nto drop this?" | JA: "何度しくじったらわかるんだ！"
    EVT_AT_SetVoice(random < 30 ? "0018C050" : random < 60 ? "0018C043" : "0018C042", 1, random < 30 ? 90 : random < 60 ? 100 : 90); // @text 0018C050 EN: "If you can't get serious,\nwe won't get this done!" | JA: "マジにやんねえと\nマジで終わんねえぞ！" ; 0018C043 EN: "You won't get paid if you keep\ndoing this! Wake up!" | JA: "そんなんじゃバイト代出ねえぞ。\nしっかりしろ！" ; 0018C042 EN: "This is valuable cargo!" | JA: "大事な荷物なんだ。\n親方にドヤされるぞ！"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t ArbtWarnCarryRepeatedly() {
    BeginCarryWarning(4830);
    const float random = GetRandomNumberInRange(100);
    const std::int32_t frames = random < 30 ? 220 : 250;
    EVT_AT_SetCam(random < 30 ? 1109 : 1124, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(random < 30 ? "0018C045" : random < 60 ? "0018C048" : "0018C039", 1, 0); // @text 0018C045 EN: "Why do you keep\ndropping the crate?" | JA: "なんでこんなに落としまくるんだ！" ; 0018C048 EN: "This is not getting us anywhere!" | JA: "ぜんぜん進まねえじゃねえか！" ; 0018C039 EN: "Oh man!\nAgain?" | JA: "なんてこった、またかよ…"
    EVT_AT_SetVoice(random < 30 ? "0018C051" : random < 60 ? "0018C052" : "0018C041", 1, random < 30 ? 90 : 100); // @text 0018C051 EN: "If you are about to drop it,\nget your act together!" | JA: "落としそうになったら\nド根性でなんとかしろ！" ; 0018C052 EN: "You're so unsteady!\nYou've got no spirit!" | JA: "気合が入ってねえから\n腰がフラつくんだよ！" ; 0018C041 EN: "Move along with me.\nShow some guff!" | JA: "ちゃんと動きを合わせてくれ。\n気合いが足んねえぞ！"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t ArbtWarnWrongForward() {
    BeginCarryWarning(4826);
    const float cameraRandom = GetRandomNumberInRange(100);
    EVT_AT_SetCam(cameraRandom < 60 ? 1125 : 1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    const float voiceRandom = GetRandomNumberInRange(100);
    EVT_AT_SetVoice(voiceRandom < 50 ? "0018C029" : "0018C030", 1, 0); // @text 0018C029 EN: "Which way are you pushing?\nForward!" | JA: "どこに押してんだ！\n前だ、前！" ; 0018C030 EN: "Push forward!\nNot any other way!" | JA: "前に押すんだ！\n他に押すんじゃない！"
    FinishCarryWarning(100);
    return 100;
}
std::int32_t ArbtWarnWrongDouble(std::uint32_t direction) {
    BeginCarryWarning(4826);
    GetRandomNumberInRange(100);
    const std::int32_t frames = direction == 0x8000 ? 140 : 155;
    EVT_AT_SetCam(1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice(direction == 0x8000 ? "0018C086" : "0018C087", 1, 0); // @text 0018C086 EN: "When I say, “right, right,”\npush to the right two times." | JA: "俺が右右って言ったら、\n右に２回押してくれ。" ; 0018C087 EN: "When I say, “left, left,”\npush to the left two times." | JA: "俺が左左って言ったら、\n左に２回押してくれ。"
    FinishCarryWarning(frames);
    return frames;
}
std::int32_t ArbtWarnWrongMultiple() {
    BeginCarryWarning(4826);
    GetRandomNumberInRange(100);
    EVT_AT_SetCam(1126, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetVoice("0018C089", 1, 0); // @text EN: "Push the number of times\nthat I tell you." | JA: "俺が指示した回数だけ、\nそこに押してくれ。"
    FinishCarryWarning(110);
    return 110;
}
std::int32_t ArbtWarnIdleForward(std::int32_t count) {
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 0);
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 90);
    EVT_AT_SetCam(1123, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 130);
    EVT_AT_SetVoice(count <= 3 ? "0018C028" : "0018C026", 1, 30); // @text 0018C028 EN: "Don't forget!\nPush forward!" | JA: "忘れるな！\n前に押すんだよ！" ; 0018C026 EN: "I told you already!\nPush forward!" | JA: "何度言わせんだ！\n前に押すんだよ！"
    FinishCarryWarning(150);
    return 150;
}
std::int32_t ArbtWarnIdlePut(std::int32_t count) {
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 0);
    EVT_AT_SetAngY(ARBT_DUMT, ARBT_DUMT, 180, 90);
    EVT_AT_SetCam(1123, ARBT_DUMT, ARBT_DUMT, 0, 0, 0);
    EVT_AT_SetCam(1106, ARBT_DUMA, ARBT_DUMA, 0, 0, 130);
    EVT_AT_SetVoice(count <= 3 ? "0018C092" : "0018C091", 1, 30); // @text 0018C092 EN: "Put it down." | JA: "下に降ろすんだ。" ; 0018C091 EN: "Hey!\nI said, “Down.”" | JA: "おいおい、下だよ、下。"
    FinishCarryWarning(150);
    return 150;
}
std::int32_t ArbtWalkForward(float speed, std::int32_t delay) {
    EVT_AT_SetMot(ARBT_RYO, 0x12e2, 2, speed, 0, delay);
    EVT_AT_SetMot(ARBT_TKR, 0x12c5, 2, speed, 0, delay);
    EVT_AT_SetSound(254, delay + 20);
    return static_cast<std::int32_t>(115.0 / static_cast<double>(speed));
}
std::int32_t ArbtQueueDirection(std::uint32_t direction, std::int32_t intensity, std::int32_t delay) {
    switch (intensity) {
    case 0:
    case 1:
    case 2:
        EVT_AT_SetVoice(direction == 0x4000 ? "0018C014" : direction == 0x8000 ? "0018C015" : "0018C013", 1, delay); // @text 0018C014 EN: "Left." | JA: "左だ。" ; 0018C015 EN: "Right." | JA: "右だ。" ; 0018C013 EN: "Forward." | JA: "前だ。"
        break;
    case 3:
        EVT_AT_SetVoice(direction == 0x8000 ? "0018C080" : "0018C083", 1, delay); // @text 0018C080 EN: "Right, right!" | JA: "右右！" ; 0018C083 EN: "Left, left!" | JA: "左左！"
        break;
    case 4:
        EVT_AT_SetVoice(direction == 0x8000 ? "0018C081" : "0018C084", 1, delay); // @text 0018C081 EN: "Right, right, right!" | JA: "右右右！" ; 0018C084 EN: "Left, left, left!" | JA: "左左左！"
        break;
    case 5:
        EVT_AT_SetVoice(direction == 0x8000 ? "0018C082" : "0018C085", 1, delay); // @text 0018C082 EN: "Right, right, right, right!" | JA: "右右右右！" ; 0018C085 EN: "Left, left, left, left!" | JA: "左左左左！"
        break;
    case 6:
    case 7:
        EVT_AT_SetVoice(GetRandomNumberInRange(100) < 50 ? "0018C017" : "0018C018", 0, delay); // @text 0018C017 EN: " " | JA: "ぃよしっ！" ; 0018C018 EN: " " | JA: "ぃよいしょー！"
        return 52;
    default:
        break;
    }
    return delay;
}
std::int32_t ArbtWalkDirection(float speed, std::int32_t intensity, std::uint32_t direction) {
    EVT_AT_SetPosAng(ARBT_TKR, ARBT_RYO, 0, -1.95f, 0, 0);
    std::uint32_t ryo, worker;
    std::int32_t frames;
    if (direction == 0x4000) {
        ryo = intensity ? 0x12ff : 0x12e6;
        worker = intensity ? 0x12fd : 0x12cb;
        frames = static_cast<std::int32_t>((intensity ? 90.0f : 100.0f) / speed);
    } else if (direction == 0x8000) {
        ryo = intensity ? 0x1303 : 0x12f0;
        worker = intensity ? 0x1301 : 0x12d4;
        frames = intensity ? static_cast<std::int32_t>(90.0 / static_cast<double>(speed)) : static_cast<std::int32_t>(90.0f / speed);
    } else {
        ryo = 0x12e2;
        worker = 0x12c5;
        frames = static_cast<std::int32_t>(115.0f / speed);
    }
    EVT_AT_SetMot(ARBT_RYO, ryo, 2, speed, 0, 0);
    EVT_AT_SetMot(ARBT_TKR, worker, 2, speed, 0x02000000U, 0);
    return frames;
}
std::int32_t ArbtFailCarry(ArbtCrateWork &work, shen_name crate, std::int32_t intensity, std::uint32_t direction) {
    Vector3 player, position;
    Vector3i rotation;
    EV_GetCharaPosition(ARBT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::Local);
    EV_GetCharaPosition(crate, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(crate, &rotation, EventTransformFlags::Local);
    if (direction == 0x4000) {
        position.x += intensity ? -0.38026428f : 0.11395264f;
        position.z += intensity ? 0.644867f : 0.65039098f;
        EVT_AT_SetMot(ARBT_RYO, 4844, 2, 1.4f, 0, 0);
        EVT_AT_SetMot(ARBT_TKR, 4816, 2, 1.4f, 0, 12);
    } else if (direction == 0x8000) {
        position.x += intensity ? -0.3815918f : 0.11235046f;
        position.z += intensity ? -0.42904603f : -0.40728712f;
        EVT_AT_SetMot(ARBT_RYO, 4845, 2, 1.4f, 0, 0);
        EVT_AT_SetMot(ARBT_TKR, 4817, 2, 1.4f, 0, 12);
    } else {
        if (direction == 0x1000) {
            position.x = (player.x - 0.58590698f) - 0.9f;
            position.z = player.z + 0.16f;
        } else {
            position.x += 0.11660004f;
            position.z += 0.11210001f;
        }
        EVT_AT_SetMot(ARBT_RYO, 4842, 25, 1.4f, 0, 0);
        EVT_AT_SetMot(ARBT_TKR, 4814, 25, 1.4f, 0, 0);
    }
    if (work.moveTask)
        work.moveFrames = 37;
    QueueArbtDropBox(work, 37);
    EVT_AT_SetPosAng(crate, ARBT_BASE, position.x, position.z, static_cast<float>(rotation.y), 61);
    Position(ARBT_TKR, ARBT_TKR, -0.1f, 0, 0, 0.35f);
    EVT_AT_SetPosAng(ARBT_TKR, ARBT_DUMB, 0, 0, 0, 61);
    EVT_AT_SetPosAng(ARBT_RYO, ARBT_TKR, 0, -1.95f, 180, 61);
    EVT_AT_SetCam(1107, ARBT_DUMA, ARBT_DUMA, 0, 0, 0);
    return 60;
}
std::int32_t ArbtAnnounceTimeOver() {
    Position(ARBT_QLN, ARBT_BASE, -4.1f, 0, -0.1f, 140);
    EVT_AT_SetMot(ARBT_QLN, 4801, 2, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1001, ARBT_DUMS, ARBT_DUMS, 0, 0, 0);
    Loop3DSoundEffect(1577);
    EVT_AT_SetVoice("0018B005", 1, 0); // @text EN: "All right. That's it for today!\nEveryone can go home!" | JA: "よーし…んっ、今日はここまでだ！\nみんな上がってくれ。"
    EVT_AT_SetVoice("0018C061", 1, 215); // @text EN: "Yessir!" | JA: "うぃーす！"
    return 260;
}
namespace {
std::int32_t PayRyo(std::int32_t cash, std::int32_t crates, const char *opening, const char *assessment, const char *amount, const char *farewell) {
    PATI_StartMoneyCount(static_cast<std::uint32_t>(cash), 219);
    EVT_AT_ObjectDisp_1(ARBT_PAY, 120, 0);
    EVT_AT_ObjectDisp_1(ARBT_PAY, 240, 1);
    Position(ARBT_QLN, ARBT_BASE, 27.1f, 0, -28.25f, 140);
    Position(ARBT_TKR, ARBT_QLN, -2, 0, -0.6f, -90);
    Position(ARBT_RYO, ARBT_QLN, 0, 0, -1.5f, 180);
    EVT_AT_SetPos(ARBT_KNT, ARBT_QLN, {5.4f, 0, -0.4f}, 0);
    EVT_AT_SetAngY(ARBT_KNT, ARBT_QLN, 90, 0);
    EVT_AT_SetPos(ARBT_AA1, ARBT_QLN, {4.2f, 0, 1.14f}, 0);
    EVT_AT_SetAngY(ARBT_AA1, ARBT_QLN, 60, 0);
    EVT_AT_SetPos(ARBT_LYO, ARBT_QLN, {1.4f, 0, 1.16f}, 0);
    EVT_AT_SetAngY(ARBT_LYO, ARBT_QLN, -30, 0);
    EVT_AT_SetPos(ARBT_HAK, ARBT_QLN, {1.58f, 0, 0.13f}, 0);
    EVT_AT_SetAngY(ARBT_HAK, ARBT_QLN, -100, 0);
    EVT_AT_SetPos(ARBT_EIN, ARBT_QLN, {3.6f, 0, -2.3f}, 0);
    EVT_AT_SetAngY(ARBT_EIN, ARBT_QLN, 1, 0);
    EVT_AT_SetAngY(ARBT_QLN, ARBT_QLN, 180, 110);
    EVT_AT_SetAngY(ARBT_QLN, ARBT_QLN, 180, 320);
    EVT_AT_SetAngY(ARBT_DUMS, ARBT_DUMS, 180, 0);
    EVT_AT_SetAngY(ARBT_DUMS, ARBT_DUMS, 180, 110);
    EVT_AT_SetAngY(ARBT_DUMS, ARBT_DUMS, 180, 320);
    EVT_AT_SetMot(ARBT_TKR, 4802, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 61442, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_RYO, 4840, 2, 1, 0, 110);
    EVT_AT_SetMot(ARBT_QLN, 4802, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ARBT_QLN, 4813, 2, 1, 0, 110);
    EVT_AT_SetMot(ARBT_QLN, 4825, 2, 1, 0, 320);
    for (std::int32_t index = 0; index < 5; ++index)
        EVT_AT_SetMot(ARBT_PAY_CROWD[index], 4853 + index, 2, 1, 0x40000000U, 0);
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
        static constexpr const char *voices[]{"0018B027", "0018B028", "0018B029", "0018B030", "0018B031", "0018B032"}; // @text 0018B027 EN: "One crate,\nso I'll pay you $10." | JA: "１個運んだから１０ドルだな。" ; 0018B028 EN: "Two crates,\nso I'll pay you $20." | JA: "２個運んだから２０ドルだな。" ; 0018B029 EN: "Three crates,\nso I'll pay you $30." | JA: "３個運んだから３０ドルだな。" ; 0018B030 EN: "Four crates,\nso I'll pay you $40." | JA: "４個運んだから４０ドルだな。" ; 0018B031 EN: "Five crates,\nso I'll pay you $50." | JA: "５個運んだから５０ドルだな。" ; 0018B032 EN: "Six crates,\nso I'll pay you $60." | JA: "６個運んだから６０ドルだな。"
        EVT_AT_SetVoice(voices[crates - 1], 1, 280);
        frames = 100;
    }
    EVT_AT_SetVoice("0018A009", 1, frames + 280); // @text EN: "Thanks." | JA: "どうも…"
    EVT_AT_SetVoice(farewell, 1, frames + 320);
    EVT_AT_SetVoice("0018A001", 1, frames + 390); // @text EN: "Sure." | JA: "はい。"
    return frames + 440;
}
} // namespace
std::int32_t ArbtPayFirstJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0018B006", "0018B007", "0018B008", "0018B009"); // @text 0018B006 EN: "Good work!" | JA: "おう、おつかれさん！" ; 0018B007 EN: "You must be tired." | JA: "初めてでだいぶ疲れたろ。" ; 0018B008 EN: "Here's what you earned." | JA: "ほら、今日のバイト代だ。" ; 0018B009 EN: "Come and help us again." | JA: "また手伝ってくれよ。"
}
std::int32_t ArbtPayGoodJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0018B010", "0018B011", "0018B008", "0018B009"); // @text 0018B010 EN: "You did a good job." | JA: "兄ちゃん、よくがんばったな。" ; 0018B011 EN: "Come again." | JA: "その調子でまた頼むぜ。" ; 0018B008 EN: "Here's what you earned." | JA: "ほら、今日のバイト代だ。" ; 0018B009 EN: "Come and help us again." | JA: "また手伝ってくれよ。"
}
std::int32_t ArbtPayAverageJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0018B012", "0018B013", "0018B008", "0018B014"); // @text 0018B012 EN: "Tough day?" | JA: "ごくろうさん。" ; 0018B013 EN: "Try a little harder next time." | JA: "次はもう少しがんばってくれよ。" ; 0018B008 EN: "Here's what you earned." | JA: "ほら、今日のバイト代だ。" ; 0018B014 EN: "You can help us again." | JA: "また手伝ってな。"
}
std::int32_t ArbtPayPoorJob(std::int32_t cash, std::int32_t crates) {
    return PayRyo(cash, crates, "0018B015", "0018B016", "0018B017", "0018B018"); // @text 0018B015 EN: "You're not really helping,\nyou know=@" | JA: "困るんだよな、あれじゃ…" ; 0018B016 EN: "You've got to show little more effort." | JA: "ちゃんとやってくれねぇとよ…" ; 0018B017 EN: "I will pay you, though." | JA: "まあバイト代は出すけど…" ; 0018B018 EN: "Work harder next time." | JA: "次はしっかりな。"
}
} // namespace shenmue::s2::areas
