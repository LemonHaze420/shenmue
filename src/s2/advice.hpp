#pragma once

#include "s2/hlib.hpp"

#include <cstdint>
#include <array>
#include <string_view>

namespace shenmue::s2 {

enum class AdviceControlMode : std::int32_t {
    None = 0,
    IronThrustMaster = 1,
    AcrobatMaster = 2,
    IronHeadMaster = 3,
    MongolianWrestling = 4,
    PassiveRaidMaster = 5,
    RodStunt = 6,
    GregMore = 7,
    OneEyedMan = 8,
    Chunyan = 9,
    Baihu = 10,
    TheBeast = 11,
    JianminInstruction = 12,
    DelinInstruction = 13,
    GuixiangInstruction = 14,
    ZhoushanInstruction = 15,
    HanhuiInstruction = 16,
    XiuyingInstruction1 = 17,
    XiuyingInstruction2 = 18,
    KaiInstruction = 19,
    PassiveRaidMaster2 = 20,
};

inline constexpr shen_name TASK_FIGHT_ADVICE = SHEN_NAME(FBAV);
inline constexpr shen_name TASK_ADVICE_CONTROL = SHEN_NAME(ADVC);
inline constexpr std::array<const char*, 20> ADVICE_TEXT_TOKENS{
    "UIADVIM001", "UIADVIM002", "UIADVIM003", "UIHELPM097", "UIHELPM099", // @text UIADVIM001 EN: "&[Iron Thrust Style Master Battle]&&Evade the opponent's blow.&QTE will occur during battle.&The opponent will not attack Ryo&during the QTE.&Carefully enter commands.&" | JA: "&【鉄砂掌の達人】&&相手の必殺技を、うまくかわしてください。&バトル中に、ＱＴＥが発生します。&コマンドＱＴＥ中は、&相手は攻撃しません。&おちついて、入力してください。&" ; UIADVIM002 EN: "&&[Acrobat Style Master Battle]&&Ryo wins if he catches the opponent.&Close in on the opponent and press^H&to catch. Go near the opponent and& press ^H as much as possible.&" | JA: "&&【軽功の達人】&&逃げ回る相手をつかまえたら勝ちです。&相手に接近して、^Hでつかまえます。&接近して、なるべく多く^Hを押してください。" ; UIADVIM003 EN: "&&[Iron Head Style Master Battle]&&Watch the opponent's attack and&press^Yat the right moment to evade.& The trick is to evade attacks near&the edge of the ring.&" | JA: "&&【鉄頭功の達人】&&相手の攻撃にあわせてタイミング良く、&^Yを押すとよけられます。&リングぎわで、よけを成功させるのがコツです。&&" ; UIHELPM097 EN: "&&[Mongolian Wrestling]&&Ryo loses when he is caught by the opponent.& Press^Yto evade or attack the opponent to prevent Ryo from&getting caught. &&If Ryo gets too close, the opponent will make a Mongolian throw move.&" | JA: "&&【モンゴル相撲】&&相手につかまれると負けです。&^Yでよけるか、攻撃をくわえ、相手を近づけないことが、コツです。&&近づくと、モンゴル相撲で投げてきます。&" ; UIHELPM099 EN: "&&&[Passive Raid Style Master Battle]&&Command QTE will occur during battle. If successful,& a critical move will be executed. The opponent will not attack&during the Command QTE, so enter the commands carefully.&" | JA: "&&&【化勁の達人】&&バトル中に、コマンドＱＴＥが発生します。&成功すると、必殺技がくりだされます。&コマンドＱＴＥ中は、相手は攻撃しません。&おちついて、入力してください。"
    "UIADVIM004", "UIADVIM005", "UIADVIM006", "UIHELPM105", "UIHELPM108", // @text UIADVIM004 EN: "&[Rod Stunt]&&A Free Battle three-set match.&The opponent is a Pancratium Master. &&Make use of Ryo's past&battle experience to fight against this&opponent. Check Ryo's moves in the&\"Moves Scroll.\"&" | JA: "&【ロッド・スタント】&&３セットマッチのフリーバトル形式です。&相手はパンクラチオンの達人です。&今までの経験を生かして、がんばってください。&全技巻物で技の確認ができます。&" ; UIADVIM005 EN: "&[Greg More]&&A Free Battle three-set match.&The opponent is the strongest of all& wrestlers. &&Make use of Ryo's past&battle experience to fight.&Check Ryo's moves in the \"Moves Scroll.\"&" | JA: "&【グレッグ・モア】&&３セットマッチのフリーバトル形式です。&相手は最強のレスラーです。&&今までの経験を生かして、&がんばってください。&全技巻物で技の確認ができます。&&" ; UIADVIM006 EN: "&[One-eyed Man]&&Find the opponent's weak spot&and fight. If Ryo can evade the opponent,&there will be a chance for attack.&& Press ^C and ^Y simultaneously to evade&in the desired position.&" | JA: "&【片目の男】&&相手の弱点を見抜き戦ってください。&よけをうまく使うと、チャンスが生まれます。&&^Cと^Yを同時に押すと、目的の方向によけます。&" ; UIHELPM105 EN: "&[Chunyan]&&Command QTE will occur during battle. If successful,& a critical move will be executed. The opponent will not&attack during the Command QTE.&&Carefully enter the commands.&&Check Ryo's moves in the \"Moves Scroll\".&" | JA: "&【春燕】&&バトル中に、コマンドＱＴＥが発生します。&成功すると、必殺技がくりだされます。&コマンドＱＴＥ中は、相手は攻撃しません。&&おちついて、入力してください。&&全技巻物で技の確認ができます。&" ; UIHELPM108 EN: "&&[Baihu]&&QTE and Command QTE will occur during battle.&If successful, a critical move will be executed.&The opponent will not attack during the Command QTE.&&Carefully enter the commands.&" | JA: "&&【白虎】&&バトル中にＱＴＥや&コマンドＱＴＥが発生します。&成功すると、必殺技がくりだされます。&コマンドＱＴＥ中は、相手は攻撃しません。&おちついて、入力してください。&"
    "UIADVIM007", "UIADVIM008", "UIADVIM009", "UIADVIM010", "UIADVIM011", // @text UIADVIM007 EN: "&&[The Beast]&&Press ^Y to check the Martial Arts Moves&Scroll. Look for moves that&do large amounts of damage.&" | JA: "&&【熊男】&&^Yで全技巻物を確認できます。&威力の高い技をチェックしてみてください。&" ; UIADVIM008 EN: "&[Jianmin's Move Instruction]&&Exert force with the arms and legs.&The trick is to make all the moves&simultaneously.&&\\t^G Guard&\\t^C Move \\t^P Punch&\\t^H Throw\\t^KKick &" | JA: "&【建民の技伝授】&&「腕」と「足」に力を入れます。&全ての行動を同時に&行うのがコツです。\n&&\\t^G防御&\\t^Cコマンド\\t^Pパンチ&\\t^H投げ\\t^Kキック&" ; UIADVIM009 EN: "&[Delin's Move Instruction]&&The trick is to step back twice and&\"Punch\" simultaneously.&&\\t^G Guard\\t ^C Command &\\t^PPunch\\t^H Throw&\\t^K Kick &" | JA: "&【徳林の技伝授】&&２歩下がったと同時に&「パンチ」を打つのがコツです。\n&&\\t^G防御\\t ^Cコマンド&\\t^Pパンチ\\t^H投げ&\\t^Kキック&" ; UIADVIM010 EN: "&[Guixiang's Move Instruction]&&\"Guard\" and \"Throw\" simultaneously&to execute Hind Blow.& Watch for the right timing to execute&the move.&\\t^L^R Run\\t^GGuard&\\t^PPunch\\t^H Throw&\\t^C Move\\t^KKick &" | JA: "&【桂香の技伝授】&&相手に近づいて「防御」と「投げ」を押すと、&斜行でたおす事が出来ます。&相手のスキをついて技をかけてみましょう。&&\\t^L^R走る\\t^G防御&\n\\t^Pパンチ\\t^H投げ&\\t^C移動\\t^Kキック&" ; UIADVIM011 EN: "&[Zhoushan's Move Instruction]&&Press^Bto grab hold of the opponent.&After grasping, press^Cto throw the& opponent in the desired direction.&Approach the opponent aggressively.&&\\t^CMove or\\t^BGrab&\\t throwing direction&" | JA: "&【周善の技伝授】&&つかむ動作は^Bで行えます。&相手をつかんだ後に^Cで色々な方向に投げてみましょう。&相手に接近して、積極的にチャレンジしてください。&&\\t^C移動\\t^Bつかむ&\\t投げる方向&"
    "UIADVIM012", "UIADVIM013", "UIADVIM014", "UIADVIM015", "UIADVIM016"}; // @text UIADVIM012 EN: "&[Hanhui's Move Instruction]&&Exert force with the arms and legs while&moving forward. The trick is to make& all the moves simultaneously.&&\\t^G Guard&\\t^C Move \\t^P Punch&\\t^H Throw\\t^KKick &" | JA: "&【漢輝の技伝授】&&前に進むと同時に、&「腕」と「足」に力を入れます。&全ての行動を同時に行うのがコツです。&&\\t^G防御&\\t^Cコマンド\\t^Pパンチ&\\t^H投げ\\t^Kキック&\n" ; UIADVIM013 EN: "&[Xiuying's Move Instruction]&&Follow the instructions, and input the&commands for the move. Moves that& Ryo masters can be used during&Free Battles.&&\\t^G Guard&\\t^C Move \\t^P Punch&\\t^H Throw\\t^KKick &" | JA: "&【秀瑛の技伝授】&&教えを頼りに、&技コマンドを入力してください。&覚えた技は、&フリーバトルで使えます。&&\n\\t^G防御&\\t^C移動\\t^Pパンチ&\\t^H投げ\\t^Kキック&" ; UIADVIM014 EN: "[Xiuying's Move Instruction]&&Guard Hanhui's attack with the&Counter Elbow Assault. Watch for the& timing of Hanhui's attack and enter&the commands as quickly as possible.&&\\t^G Guard&\\t^C Move \\t^P Punch&\\t^H Throw\\t^KKick &" | JA: "【秀瑛の技伝授】&&漢輝の攻撃を外門頂肘で受けましょう。&攻撃してくるタイミングを見極め、&素早くコマンドを入力しましょう。\n&&&\\t^G防御&\\t^Cコマンド\\t^Pパンチ&\\t^H投げ\\t^Kキック&" ; UIADVIM015 EN: "&[Kai's Move Instruction]&&Press^Cto adjust the direction and&attack. If the strike hits the opponent&within the allotted time, Ryo is&successful. Try to sense the opponent's&moves swiftly.&&\\t^CDirection\\t^PPunch&" | JA: "&【凱の技伝授】&&^Cで方向を合わせ&攻撃して下さい。&時間内に攻撃を当てられれば成功です。&相手の気配を素早く読み取って下さい。\n&&\\t^C移動\\t^Pパンチ&" ; UIADVIM016 EN: "&&[Passive Raid Style Master Battle]&&Thoughtless strikes will not be effective&in attacking the opponent. Stay calm&and watch for an opportunity to attack.&" | JA: "&&【化勁の達人】&&むやみに攻撃しても、当たりません。&おちつき、隙を見て攻撃してください。&"

HLib::Task* QueueAdviceControl();
void AssignAdviceControlMode(AdviceControlMode mode) noexcept;
AdviceControlMode GetAdviceControlMode() noexcept;
void FormatAdviceHelpText(std::string_view text);
void QueueFightAdviceControl(std::uint8_t mode);
std::int64_t FightAdviceControlCallback();
void ReleaseAdviceControlOwners() noexcept;

} // namespace shenmue::s2
