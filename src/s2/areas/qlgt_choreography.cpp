#include "s2/areas/area_qlgt.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/osage.hpp"
#include "s2/step_event.hpp"
namespace shenmue::s2::areas::qlgt {
void Film(std::int32_t frames, std::int32_t from, std::int32_t to, std::int32_t delay) {
    EVT_AT_MenCtrl({0, frames, from, 0, 0, 0, to, 0, 0, 0, static_cast<float>(delay)});
}
void Camera(std::int32_t camera, shen_name anchor, std::int32_t delay, bool negativeZero) {
    EVT_AT_SetCam(camera, anchor, anchor, negativeZero ? -0.0f : 0.0f, 0, delay);
}
namespace {
void PoseNow(shen_name character, shen_name parent, float x, float z, float yaw) {
    EventSetPositionAngle pose{0, character, parent, {x, 0, z}, yaw, 0};
    EVT_AT_SetPosAng_Callback(&pose);
}
void ResetRenCloth() {
    toggleOsagFlags(FindCharacterByScenarioName(ren));
    SetClothFlag(FindCharacterByScenarioName(ren), 16, 1);
}
} // namespace
void QueueSearchPose(std::int32_t side, std::int32_t delay) {
    EVT_AT_SetPosAng(ryo, start, 0, 0, 0, delay);
    Camera(side ? 7008 : 7002, start, delay);
    EVT_AT_SetMot(ryo, 0x1d86, 0, .7f, side ? 0x40000000U : 0x60000000U, delay);
}
std::int32_t FirstIntroduction(std::int32_t side) {
    EVT_AT_SetVoice("0263A001", 1, 30); // @text EN: "Hm?" | JA: "んっ？"
    EVT_AT_SetVoice("0263B001", 1, 80); // @text EN: "What's wrong?" | JA: "どうした？"
    EVT_AT_SetVoice("0263A002", 1, 120); // @text EN: "The guards." | JA: "見張りがいる。"
    EVT_AT_SetVoice("0263B002", 1, 170); // @text EN: "Oh, brother." | JA: "ちっ、うざってえな。"
    EVT_AT_SetVoice("0263B003", 1, 290); // @text EN: "Hm?" | JA: "んっ…？"
    EVT_AT_SetVoice("0263B004", 1, 380); // @text EN: "Look, we don't have time\nto deal with them." | JA: "おい、ザコにかまってる\nヒマはねえぜ。"
    EVT_AT_SetVoice("0263B005", 1, 470); // @text EN: "Run when the lights go off." | JA: "明かりが消えたら走るぞ！"
    EVT_AT_SetVoice("0263A003", 1, 590); // @text EN: "Hey, Ren!" | JA: "おいっ、レン…！"
    ResetRenCloth();
    // The first-course mirrored arm is natively dormant: it schedules only the voices and cloth reset.
    if (!side) {
        Camera(7020, start);
        Camera(7021, start, 40);
        Camera(7034, renDummy, 80);
        Camera(7035, start, 110);
        Camera(7024, start, 290);
        Camera(7025, light, 330);
        Camera(7026, ryoDummy, 380);
        PoseNow(ren, start, -2.9f, 0, -90);
        PoseNow(ryo, start, -2.2f, -.7f, -90);
        EVT_AT_SetPosAng(ryo, ren, -.3f, -.5f, -90, 330);
        EVT_AT_SetAngY(ren, ren, 90, 290);
        EVT_AT_SetAngY(ren, ren, 40, 330);
        const std::int32_t frames = EVT_AT_SetMot(ryo, 7590, 20, 1, 0x40000000U, 0);
        EVT_AT_SetMot(ryo, 7592, 0, 1, 0, frames);
        EVT_AT_SetMot(ryo, 7595, 0, 1, 0x40000000U, 290);
        EVT_AT_SetMot(ren, 7588, 20, 1, 0x40000000U, 0);
        EVT_AT_SetMot(ren, 7584, 0, 1, 0x40000000U, 110);
        EVT_AT_SetMot(ren, 7587, 0, 1, 0x40000000U, 290);
        EVT_AT_SetPoint(ryo, ren, 5, 1, 1, 290, 240, 0x80000000U);
    }
    return 530;
}
void SkipFirstIntroduction(std::int32_t side) {
    if (side)
        return;
    PoseNow(ren, start, 0, 0, -90);
    PoseNow(ryo, start, -.5f, -.5f, -90);
    EVT_AT_SetAngY(ren, ren, 90, 0);
    EVT_AT_SetAngY(ren, ren, 40, 0);
    EVT_AT_SetMot(ryo, 7595, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ren, 7587, 0, 1, 0x40000000U, 0);
}
std::int32_t Introduction(std::int32_t side, std::int32_t course) {
    if (course == 1) {
        Camera(7032, start);
        Camera(7037, ryoDummy, 210);
        EVT_AT_SetVoice("0263A001", 1, 30); // @text EN: "Hm?" | JA: "んっ？"
        EVT_AT_SetVoice("0263A010", 1, 50); // @text EN: "There are guards here, too." | JA: "ここにも見張りが…"
        EVT_AT_SetVoice("0263B002", 1, 110); // @text EN: "Oh, brother." | JA: "ちっ、うざってえな。"
        EVT_AT_SetVoice("0263B014", 1, 210); // @text EN: "Same thing.\nRun when the lights go out." | JA: "さっきと同じ要領だ。\n明かりが消えたら走るぜ！"
        EVT_AT_SetVoice("0263A008", 1, 320); // @text EN: "Yeah." | JA: "ああ。"
    } else {
        Camera(7023, start, 0, true);
        Camera(7026, ryoDummy, 210, true);
        if (course == 2) {
            EVT_AT_SetVoice("0263A009", 1, 30); // @text EN: "Hm?" | JA: "ん！？"
            EVT_AT_SetVoice("0263A021", 1, 50); // @text EN: "Men on guard." | JA: "ここも見張りがいる。"
            EVT_AT_SetVoice("0263B013", 1, 110); // @text EN: "Not again." | JA: "ちっ、またかよ。"
            EVT_AT_SetVoice("0263B019", 1, 210); // @text EN: "Same thing.\nRun when the lights go out." | JA: "ここも同じ要領だ。\n明かりが消えたら行くぜ！"
            EVT_AT_SetVoice("0263A004", 1, 320); // @text EN: "All right." | JA: "わかった。"
        } else {
            EVT_AT_SetVoice("0263A020", 1, 30); // @text EN: " " | JA: "むっ…！"
            EVT_AT_SetVoice("0263A014", 1, 50); // @text EN: "There are men on the lookout." | JA: "見張りがいる…"
            EVT_AT_SetVoice("0263B018", 1, 110); // @text EN: "Can't we ever get rid of them?" | JA: "…ったく。キリがねぇ。"
            EVT_AT_SetVoice("0263B028", 1, 210); // @text EN: "Same thing.\nRun when the lights go out." | JA: "また同じ要領で行くぞ。\n明かりが消えたら走るぜ！"
            EVT_AT_SetVoice("0263A008", 1, 320); // @text EN: "Yeah." | JA: "ああ。"
        }
    }
    ResetRenCloth();
    EVT_AT_SetPoint(ryo, actors[2], 5, 1, 1, 0, 345, 0x80000000U);
    if (!side) {
        Camera(7023, start);
        Camera(7026, ryoDummy, 210);
    }
    const float sign = side ? 1.0f : -1.0f;
    PoseNow(ren, start, sign * 2, 0, sign * 90);
    PoseNow(ryo, start, sign, -.7f, sign * 90);
    EVT_AT_SetAngY(ren, ren, sign * -120, 210);
    EVT_AT_SetPosAng(ryo, ren, sign * .3f, -.5f, sign * 90, 210);
    const std::uint32_t flags = side ? 0x60000000U : 0x40000000U;
    EVT_AT_SetMot(ryo, 7595, 0, 1, flags, 0);
    EVT_AT_SetMot(ren, 7584, 20, 0, flags, 0);
    EVT_AT_SetMot(ren, 7584, 20, 1, flags, 60);
    EVT_AT_SetMot(ren, 7587, 0, 1, flags, 210);
    return 345;
}
void SkipIntroduction(std::int32_t side) {
    if (!side)
        Camera(7026, ryoDummy);
    EVT_AT_SetAngY(ren, ren, side ? -120.0f : 120.0f, 0);
    PoseNow(ryo, ren, side ? .3f : -.3f, -.5f, side ? 90.0f : -90.0f);
    EVT_AT_SetMot(ren, 7587, 0, 1, side ? 0x60000000U : 0x40000000U, 0);
}
std::int32_t LectureFinish(std::int32_t side) {
    PoseNow(ren, end, 0, -.5f, 0);
    Camera(7006, end, 0, side != 0);
    const std::int32_t frames = EVT_AT_SetMot(ren, 7583, 50, 1, side ? 0x60000000U : 0x40000000U, 0);
    EVT_AT_SetMot(ren, 0x1d87, 0, 1, side ? 0x20000000U : 0, frames);
    return frames - 20;
}
std::int32_t SuccessDialogue(std::int32_t side, std::int32_t course) {
    switch (course) {
    case 0:
        EVT_AT_SetVoice("0263B006", 1, 5); // @text EN: "Must be others on the lookout ahead." | JA: "この先も見張りが\nゴロゴロいるはずだ。"
        EVT_AT_SetVoice("0263B007", 1, 85); // @text EN: "Try to avoid them as much as possible." | JA: "ザコはできるだけ、かわして行くぜ。"
        EVT_AT_SetVoice("0263A004", 1, 175); // @text EN: "All right." | JA: "わかった。"
        break;
    case 1:
        EVT_AT_SetVoice("0263B015", 1, 5); // @text EN: "Good.\nWell done." | JA: "よぉし、上出来だ。"
        EVT_AT_SetVoice("0263B012", 1, 75); // @text EN: "Try to avoid them as much as possible." | JA: "この先も、ザコはできるだけ\nかわして行くぜ。"
        EVT_AT_SetVoice("0263A008", 1, 165); // @text EN: "Yeah." | JA: "ああ。"
        break;
    case 2:
        EVT_AT_SetVoice("0263B020", 1, 5); // @text EN: "Way to go." | JA: "よっしゃ。上出来だ。"
        EVT_AT_SetVoice("0263B021", 1, 75); // @text EN: "We'll run out of time if we have to deal\nwith all of them." | JA: "ザコを相手にしてたら時間がねぇ。\nこの調子で行くぞ。"
        EVT_AT_SetVoice("0263A022", 1, 205); // @text EN: "True.\nLet's go." | JA: "そうだな。行こう。"
        break;
    default:
        EVT_AT_SetVoice("0263B029", 1, 5); // @text EN: "Good enough." | JA: "まあまあってとこだ。"
        EVT_AT_SetVoice("0263B007", 1, 75); // @text EN: "Try to avoid them as much as possible." | JA: "ザコはできるだけ、かわして行くぜ。"
        EVT_AT_SetVoice("0263A016", 1, 165); // @text EN: "Yeah, I know." | JA: "ああ。わかってる。"
        break;
    }
    ResetRenCloth();
    EVT_AT_SetPoint(ryo, ren, 5, 1, 1, 0, 285, 0x80000000U);
    Camera(7022, renDummy, 0, side != 0);
    Camera(7026, ryoDummy, 80, !side);
    Camera(7033, ryoDummy, 150, side != 0);
    const float sign = side ? -1.0f : 1.0f;
    EVT_AT_SetPosAng(ren, end, -sign, 0, 0, 0);
    EVT_AT_SetPosAng(ryo, end, sign * 2, -.5f, sign * 90, 0);
    EVT_AT_SetPosAng(ryo, ren, sign * .5f, -.3f, sign * 90, 80);
    EVT_AT_SetAngY(ren, ren, -sign * 30, 80);
    EVT_AT_SetMot(ryo, 7595, 0, 1, side ? 0x60000000U : 0x40000000U, 0);
    EVT_AT_SetMot(ren, 7587, 0, 1, side ? 0x60000000U : 0x40000000U, 0);
    return 285;
}
std::int32_t FailDialogue(std::int32_t side, std::int32_t course) {
    switch (course) {
    case 0:
        EVT_AT_SetVoice("0263F001", 1, 60); // @text EN: "Who are you?" | JA: "なんだおめえ！？"
        EVT_AT_SetVoice("0263A006", 1, 130); // @text EN: "Damn." | JA: "くそっ…"
        EVT_AT_SetVoice("0263B010", 1, 160); // @text EN: "Idiot=@" | JA: "ちっ、あのバカ…"
        EVT_AT_SetVoice("0263F002", 1, 270); // @text EN: "Get him!" | JA: "やっちまえ！"
        break;
    case 1:
        EVT_AT_SetVoice("0263D001", 1, 60); // @text EN: "Who's there?" | JA: "誰だ！？"
        EVT_AT_SetVoice("0263A006", 1, 130); // @text EN: "Damn." | JA: "くそっ…"
        EVT_AT_SetVoice("0263B010", 1, 160); // @text EN: "Idiot=@" | JA: "ちっ、あのバカ…"
        EVT_AT_SetVoice("0263D002", 1, 270); // @text EN: "Get him!" | JA: "やっちまえ～！"
        break;
    case 2:
        EVT_AT_SetVoice("0263H001", 1, 60); // @text EN: "Who's there?" | JA: "誰だ！？"
        EVT_AT_SetVoice("0263A018", 1, 130); // @text EN: "Damn=@" | JA: "しまったっ…"
        EVT_AT_SetVoice("0263B024", 1, 160); // @text EN: "So much trouble." | JA: "…ったく。しょうがねえ。"
        EVT_AT_SetVoice("0263H002", 1, 270); // @text EN: "Catch him!" | JA: "とっ捕まえろ！"
        break;
    default:
        EVT_AT_SetVoice("0263J001", 1, 60); // @text EN: "Hold it right there!" | JA: "待て！　キサマ！"
        EVT_AT_SetVoice("0263A006", 1, 130); // @text EN: "Damn." | JA: "くそっ…"
        EVT_AT_SetVoice("0263B033", 1, 160); // @text EN: "He's such an idiot." | JA: "けっ、あのボケが…"
        EVT_AT_SetVoice("0263J002", 1, 270); // @text EN: "Kill him!" | JA: "ぶっ殺せ！"
        break;
    }
    ResetRenCloth();
    EV_DisableCharacterDisplay(ryo);
    EVT_AT_ObjectDisp_1(ryo, 230, 0);
    Camera(7025, light, 0, side != 0);
    Camera(7027, actors[2], 60, side != 0);
    Camera(7029, renDummy, 150, side != 0);
    Camera(7030, battle, 230);
    const float sign = side ? 1.0f : -1.0f;
    PoseNow(ren, end, -sign * .2f, 0, sign * 90);
    PoseNow(ryo, start, -sign, -.7f, sign * 90);
    EVT_AT_SetPosAng(ryo, battle, 0, 0, 0, 230);
    const std::uint32_t flags = side ? 0x60000000U : 0x40000000U;
    EVT_AT_SetMot(ryo, 13, 0, 1, flags, 0);
    EVT_AT_SetMot(ren, 7587, 0, 1, flags, 0);
    EVT_AT_SetMot(ren, 7585, 35, .1f, flags, 150);
    EVT_AT_SetMot(actors[2], 0x1dae, 0, 1, flags, 60);
    return 340;
}
std::int32_t WinDialogue(std::int32_t side, std::int32_t course) {
    std::int32_t frames;
    switch (course) {
    case 0:
        frames = 305;
        EVT_AT_SetVoice("0263B011", 1, 5); // @text EN: "Not that much trouble, eh?" | JA: "へっ…俺様の手をわずらわす\nほどじゃなかったな。"
        EVT_AT_SetVoice("0263A007", 1, 145); // @text EN: "Shut up." | JA: "…うるさい。"
        EVT_AT_SetVoice("0263B012", 1, 185); // @text EN: "Try to avoid them as much as possible." | JA: "この先も、ザコはできるだけ\nかわして行くぜ。"
        EVT_AT_SetVoice("0263A008", 1, 265); // @text EN: "Yeah." | JA: "ああ。"
        break;
    case 1:
        frames = 305;
        EVT_AT_SetVoice("0263B017", 1, 5); // @text EN: "That's right.\nYou should clean up your own mess." | JA: "そうそう。てめぇのドジは、\nてめぇで片づけなきゃな。"
        EVT_AT_SetVoice("0263A007", 1, 145); // @text EN: "Shut up." | JA: "…うるさい。"
        EVT_AT_SetVoice("0263B012", 1, 185); // @text EN: "Try to avoid them as much as possible." | JA: "この先も、ザコはできるだけ\nかわして行くぜ。"
        EVT_AT_SetVoice("0263A008", 1, 265); // @text EN: "Yeah." | JA: "ああ。"
        break;
    case 2:
        frames = 375;
        EVT_AT_SetVoice("0263B025", 1, 5); // @text EN: "Welcome back.\nI got so bored waiting." | JA: "ごくろうさん。へへっ、\nこっちは待ちくたびれたぜ。"
        EVT_AT_SetVoice("0263A019", 1, 145); // @text EN: "Oh, really." | JA: "…そうかよ。"
        EVT_AT_SetVoice("0263B026", 1, 185); // @text EN: "Don't mess up next time.\nWe don't have all day." | JA: "今度は失敗するな。こんなんじゃ\n時間がいくらあっても足りねえ。"
        EVT_AT_SetVoice("0263A008", 1, 345); // @text EN: "Yeah." | JA: "ああ。"
        break;
    default:
        frames = 335;
        EVT_AT_SetVoice("0263B034", 1, 5); // @text EN: "You're wasting so much time!" | JA: "…ったく。ムダな時間ばっか\n使いやがってよぉ！"
        EVT_AT_SetVoice("0263A024", 1, 145); // @text EN: "Sorry." | JA: "…悪かったな。"
        EVT_AT_SetVoice("0263B035", 1, 185); // @text EN: "Why don't you use a little bit\nof your brain? Muscle-head!" | JA: "ちったぁアタマ使ってやれよ。\nこの体力バカが！"
        EVT_AT_SetVoice("0263A025", 1, 325); // @text EN: " " | JA: "くっ…"
        break;
    }
    ResetRenCloth();
    EVT_AT_SetPoint(ryo, ren, 5, 1, 1, 0, frames, 0x80000000U);
    Camera(7022, renDummy, 0, side != 0);
    Camera(7036, ryoDummy, 140, !side);
    Camera(7026, ryoDummy, 180, !side);
    const float sign = side ? -1.0f : 1.0f;
    PoseNow(ren, end, -sign, 0, 0);
    PoseNow(ryo, end, sign * 2, -.5f, sign * 90);
    EVT_AT_SetPosAng(ryo, ren, sign * .5f, -.3f, sign * 90, 180);
    EVT_AT_SetAngY(ren, ren, -sign * 30, 180);
    EVT_AT_SetMot(ryo, 0xf005, 0, .7f, side ? 0x60000000U : 0x40000000U, 0);
    EVT_AT_SetMot(ryo, 7595, 0, 1, side ? 0x20000000U : 0, 70);
    EVT_AT_SetMot(ren, 7587, 0, 1, side ? 0x60000000U : 0x40000000U, 0);
    return frames;
}
std::int32_t ReturnDialogue() {
    EVT_AT_SetVoice(GetRandomNumberInRange(100) >= 50 ? "0263A027" : "0263A026", 1, 5); // @text 0263A027 EN: "I'd only be going back the way I came." | JA: "こっちに行っても戻るだけだ…" ; 0263A026 EN: "This isn't the right way." | JA: "こっちじゃなかったな…"
    EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0, 30);
    return 30;
}
} // namespace shenmue::s2::areas::qlgt
