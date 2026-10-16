#include "s2/areas/wbbk_dialogue.hpp"
#include "s2/areas/wbbk_motion.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/face.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
namespace shenmue::s2::areas {
const char *const bookMinorDropVoices[3]{
    "0035A036", // @text EN: "Oops=@" | JA: "しまった…"
    "0035A0370035A038",

    reinterpret_cast<const char *>(std::uintptr_t{0x000000aa000000afULL}),
};
const char *const bookDropPlayerVoices[4]{"0035A036", "0035A039", "0035A040", "0035A042"}; // @text 0035A036 EN: "Oops=@" | JA: "しまった…" ; 0035A039 EN: " " | JA: "うわっ！" ; 0035A040 EN: "Oh." | JA: "おっと！" ; 0035A042 EN: " " | JA: "あっ…"
const char *const bookDropCompanionVoices[4]{"0035E011", "0035E012", "0035E013", "0035E014"}; // @text 0035E011 EN: "Steady there=@" | JA: "あの…しっかりしてください。" ; 0035E012 EN: "Please be more careful." | JA: "もっと慎重に運んでくださいよ。" ; 0035E013 EN: "They can't take much abuse." | JA: "本がいたんでしまいますよ。" ; 0035E014 EN: "Be careful.\nThe books here are precious." | JA: "気をつけてください。\n貴重な本なんですから。"
const char *const bookDropReplyVoices[4]{nullptr, nullptr, nullptr, "0035A041"}; // @text EN: "I'm sorry." | JA: "すまない…"
namespace {
void Voice(const char *name, std::int32_t delay) {
    EVT_AT_SetVoice(name, delay);
}
void Camera(std::int32_t camera, std::int32_t delay) {
    const shen_name invalid{0xffffffffU};
    EVT_AT_SetCam(camera, invalid, invalid, 0, 6, delay);
}
} // namespace
std::int32_t QueueBookDialogue(std::int32_t sequence, std::int32_t books) {
    switch (sequence) {
    case 0:
        Camera(2000, 0);
        Voice("0035E001", 0); // @text EN: "Please get started." | JA: "じゃあ、さっそくお願いします。"
        Camera(2001, 73);
        Voice("0035A030", 73); // @text EN: "Yes." | JA: "ああ…"
        Camera(2002, 111);
        Voice("0035E002", 111); // @text EN: "Carry each pile and place it\non the stand outside." | JA: "ここに積んである本をひと山ずつ、\n外の台まで運んでください。"
        Voice("0035E003", 245); // @text EN: "I'll spread out the books.\nYou just carry them out." | JA: "並べるのは私がやりますから、\nあなたは運ぶだけでけっこうです。"
        Voice("0035E004", 388); // @text EN: "Your service to the Temple\nwill last until noon." | JA: "ご奉仕は昼の１２時までですが、"
        Voice("0035E005", 487); // @text EN: "If you carry them all out,\nyou'll be done." | JA: "全部運んだら、そこでおしまい。\nがんばれば早く終われますよ。"
        Camera(2001, 621);
        Voice("0035A031", 621); // @text EN: "I got it." | JA: "わかった。"
        Camera(2009, 668);
        Voice("0035E006", 668); // @text EN: "They're precious books,\nso don't drop them." | JA: "どれも貴重な本ですから\n落とさないように。"
        Voice("0035E007", 776); // @text EN: "Let's get to work." | JA: "じゃあ、頼みましたよ。"
        Camera(2003, 831);
        Voice("0035A032", 831); // @text EN: "Might as well." | JA: "やるか…"
        return 878;
    case 1:
        Camera(2000, 0);
        Voice("0035E008", 0); // @text EN: "Good morning." | JA: "おはようございます。"
        Camera(2001, 80);
        Voice("0035A033", 80); // @text EN: "Good morning." | JA: "おはよう。"
        Camera(2002, 130);
        Voice("0035E009", 130); // @text EN: "Same task as yesterday." | JA: "じゃあ、今日もお願いします。"
        Voice("0035A031", 220); // @text EN: "I got it." | JA: "わかった。"
        Voice("0035E010", 260); // @text EN: "Make sure you don't drop them.\nBe careful." | JA: "くれぐれも、落とさないように、\n気をつけてくださいね。"
        Voice("0035A034", 380); // @text EN: "All right." | JA: "ああ。"
        Camera(2003, 450);
        Voice("0035A035", 450); // @text EN: "Gotta start." | JA: "始めるか…"
        return 490;
    case 2:
        Voice(bookMinorDropVoices[shenGetRandomInt32(3)], 0);
        return 90;
    case 3: {
        const std::int32_t index = shenGetRandomInt32(4);
        Voice(bookDropPlayerVoices[index], 0);
        Voice(bookDropCompanionVoices[index], 45);
        if (!bookDropReplyVoices[index])
            return 155;
        Voice(bookDropReplyVoices[index], 155);
        return 245;
    }
    case 4:
        Camera(2000, 0);
        Voice("0035E015", 0); // @text EN: "Thank you." | JA: "おつかれさまでした。"
        Voice("0035E016", 47); // @text EN: "I'll do the cleanup later.\nI'll see you tomorrow." | JA: "片づけは後で私がやりますから、\n明日もこの調子でお願いします。"
        Camera(2001, 208);
        Voice("0035A034", 208); // @text EN: "All right." | JA: "ああ。"
        Camera(2002, 246);
        Voice("0035E017", 246); // @text EN: "See you." | JA: "それじゃあ…"
        return 284;
    case 5:
        Camera(2000, 0);
        Voice("0035E018", 0); // @text EN: "You must be tired." | JA: "おつかれさまです。"
        Voice("0035A030", 55); // @text EN: "Yes." | JA: "ああ…"
        Voice("0035E019", 93); // @text EN: "You couldn't finish." | JA: "全部運べませんでしたね。"
        Camera(2001, 175);
        Voice("0035A041", 175); // @text EN: "I'm sorry." | JA: "すまない…"
        Camera(2002, 222);
        Voice("0035E020", 222); // @text EN: "Well, we'll continue tomorrow." | JA: "まあ、後は明日にしましょう。"
        Voice("0035E021", 321); // @text EN: "See you=@" | JA: "それじゃ…"
        return 359;
    case 6: {
        Voice("0035A016", 0);
        constexpr const char *first[]{"0035E022", "0035E023"}, *second[]{"0035E024", "0035E025", "0035E026"}; // @text 0035E022 EN: "Bring the next pile." | JA: "その調子で次を頼みます。" ; 0035E023 EN: "That's the way to go!" | JA: "その調子、その調子！" ; 0035E024 EN: "The next pile please." | JA: "さあ、次をお願いしますよ。" ; 0035E025 EN: "Let's keep up the pace." | JA: "さあ、どんどんいきましょう！" ; 0035E026 EN: "Bring the piles out." | JA: "どんどん持ってきてください。"
        constexpr const char *third[]{"0035E027", "0035E028", "0035E029"}, *fourth[]{"0035E030", "0035E031"}; // @text 0035E027 EN: "A few more.\nKeep it up." | JA: "もうちょっとですね。\nがんばりましょう。" ; 0035E028 EN: "Just a few left!" | JA: "あと少しです！" ; 0035E029 EN: "We're almost done." | JA: "もうひとがんばりですよ。" ; 0035E030 EN: "The next pile is the last one." | JA: "次でおしまいですね。" ; 0035E031 EN: "One more pile and we're done." | JA: "次が最後ですね。"
        switch (books) {
        case 1:
        case 2:
        case 3:
        case 4:
            Voice(first[shenGetRandomInt32(2)], 20);
            return 110;
        case 5:
        case 6:
        case 7:
        case 8:
            Voice(second[shenGetRandomInt32(3)], 20);
            return 110;
        case 9:
        case 10:
        case 11:
        case 12:
            Voice(third[shenGetRandomInt32(3)], 20);
            return 110;
        case 13:
            Voice(fourth[shenGetRandomInt32(2)], 20);
            return 110;
        default:
            return 20;
        }
    }
    case 7: {
        constexpr const char *voices[]{"0035E032", "0035E033", "0035E034", "0035E035", "0035E036"}; // @text 0035E032 EN: "Where are you going?" | JA: "ちょっと！　どこ行くんですか？" ; 0035E033 EN: "We're not done yet!" | JA: "まだ終わってませんよ！" ; 0035E034 EN: "Master Tao will not be pleased\nif you leave now." | JA: "ちゃんと本を運ばないと、\n老師にしかられますよ。" ; 0035E035 EN: "Wait!" | JA: "待ってください！" ; 0035E036 EN: "If you don't do the task,\nI'll get into trouble too." | JA: "ちゃんとご奉仕しないと、\n私まで老師にしかられますから。"
        Voice(voices[shenGetRandomInt32(5)], 0);
        return 90;
    }
    case 8: {
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(BOOK_SAI), FacialNodeReplacement);
        constexpr const char *voices[]{"0035E037", "0035E038", "0035E039", "0035E040", "0035E041"}; // @text 0035E037 EN: "Hurry and bring the books out." | JA: "早く本を運んできてください。" ; 0035E038 EN: "What are you doing?\nPlease bring them." | JA: "何してるんですか？\nお願いしますよ。" ; 0035E039 EN: "If you're wasting time,\nyou'll get into trouble." | JA: "サボってると、\n老師にしかられますよ。" ; 0035E040 EN: "What are you doing?\nBring the books." | JA: "何してるんですか？\n本を運んでください。" ; 0035E041 EN: "What's wrong?\nYou should work or else=@" | JA: "どうしたんですか？\n本を運ばないと…"
        Voice(voices[shenGetRandomInt32(5)], 0);
        return 90;
    }
    case 9: {
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(BOOK_SAI), FacialNodeReplacement);
        constexpr const char *voices[]{"0035E042", "0035E043", "0035E044", "0035E045", "0035E046"}; // @text 0035E042 EN: "What's wrong?\nBring the pile please." | JA: "どうしたんですか？\n早く運んでください。" ; 0035E043 EN: "Are you tired?" | JA: "あれ？　疲れちゃったんですか？" ; 0035E044 EN: "Come on.\nBring them." | JA: "さあ、運んでください。" ; 0035E045 EN: "Place the piles on the stand." | JA: "本を台まで運ぶんですよ。" ; 0035E046 EN: "Keep up the work." | JA: "がんばって運んでください！"
        Voice(voices[shenGetRandomInt32(5)], 0);
        return 90;
    }
    case 10: {
        constexpr const char *voices[]{
            "0035A007", "0035A008", "0035A009", "0035A016", "0035A017", "0035A022", "0035A024", "0035A029", "0035A040", "0035A042", "0035A044", "0035A045"}; // @text 0035A007 EN: "N-no=@" | JA: "い、いえ…" ; 0035A008 EN: "Yes." | JA: "はい…" ; 0035A009 EN: "Huh?" | JA: "…えっ？" ; 0035A017 EN: "Huh?" | JA: "え？" ; 0035A022 EN: " " | JA: "へえ…" ; 0035A024 EN: " " | JA: "ん?" ; 0035A029 EN: " " | JA: "はあ…" ; 0035A040 EN: "Oh." | JA: "おっと！" ; 0035A042 EN: " " | JA: "あっ…" ; 0035A044 EN: "Sorry." | JA: "すまない。" ; 0035A045 EN: "Huh?" | JA: "ん…？"
        Voice(voices[shenGetRandomInt32(12)], 0);
        return 30;
    }
    default:
        return 0;
    }
}
} // namespace shenmue::s2::areas
