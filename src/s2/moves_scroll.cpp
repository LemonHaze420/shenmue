#include "s2/moves_scroll.hpp"

#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/archive_data.hpp"
#include "s2/camera.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/font.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/help.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/lighting.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <limits>
#include <span>
#include <string>
#include <vector>
#if defined(_M_X64) || defined(__x86_64__)
#include <emmintrin.h>
#endif

namespace shenmue::s2 {
HLib::Task *g_moveLearnedTask{};
std::array<std::int16_t, 3> g_moveLearnedTextSurfaces{};
std::uint16_t g_moveLearnedSavedHelpFlags{};
std::int16_t g_moveLearnedPendingCategory{}, g_moveLearnedPendingMove{};
std::int8_t g_moveLearnedSoundDelay{};
std::uint16_t g_moveUiClockAndNavigationHold{};
namespace {
std::uint16_t moveLearningResetWord{};

struct MoveDefinition {
    std::uint32_t skillFlag;
    std::uint32_t motionId;
    std::uint8_t group;
    std::array<std::uint8_t, 3> stats;
    const char *textToken;
};

constexpr std::array<MoveDefinition, 28> moveDefinitions1{{
    {0x80000001U, 0x00000013U, 0, {9, 4, 6}, "UIWAZAHH00"}, // @text EN: "A punch aimed at a point between the\nnose and mouth." | JA: "鼻と口の間にあるツボ、「人中」を狙う技である。"
    {0x40000040U, 0x0000006aU, 1, {14, 6, 12}, "UIWAZAHH01"}, // @text EN: "A downward rotating blow aimed at\nthe opponent's collarbone." | JA: "腕を縦方向の回転運動で打ち下ろし、\n 相手の鎖骨を狙う技である。"
    {0x40000080U, 0x00000014U, 1, {12, 5, 12}, "UIWAZAHH02"}, // @text EN: "A straight punch aimed at a pressure\npoint on the opponent's side neck." | JA: "ストレートパンチを相手の頸部に打ち出す技。"
    {0x44000000U, 0x00000017U, 2, {15, 4, 15}, "UIWAZAHH03"}, // @text EN: "Unlike the Elbow Slam, it is thrown\nhorizontally in a sweeping hook." | JA: "小車打ちとは異なり、水平方向に振る、\n大振りのいわゆるフックである。"
    {0x48000000U, 0x00000015U, 2, {18, 10, 15}, "UIWAZAHH04"}, // @text EN: "A strike on the jaw swung by\nmomentarily lowering and rising." | JA: "いったん体を下に沈め、伸び上がりながら\n下から相手の顎を振り打つ技。"
    {0x80000002U, 0x0000011bU, 3, {40, 5, 18}, "UIWAZAHH05"}, // @text EN: "A move targeted at the center of\nthe opponent's body with a rush." | JA: "体当たりに近い感覚で行う肘打ち。\n相手の身体の正中線を狙う。"
    {0x40000200U, 0x0000004dU, 5, {23, 13, 17}, "UIWAZAHH06"}, // @text EN: "A slashing blow with the side of the\npalm with an advancing step." | JA: "前足を一歩進め、手刀の部分で\n相手に斬りつける技である。"
    {0x40000100U, 0x0000001bU, 6, {27, 10, 18}, "UIWAZAHH07"}, // @text EN: "A punch at a mid-level target while\ntaking a step forward." | JA: "後ろ足を前に一歩進めて、\n中段を正拳で突く技である。"
    {0x40000400U, 0x0000001aU, 7, {26, 10, 16}, "UIWAZAHH08"}, // @text EN: "A diagonal blow on the opponent's\ncollarbone while closing in." | JA: "一気に間合いを詰め、鎖骨近辺を狙って、\n斜めに振り打たれる。"
    {0x40000800U, 0x00000148U, 8, {25, 7, 35}, "UIWAZAHH09"}, // @text EN: "A strike on the jaw and chest by\nusing the palm and forearm." | JA: "手のひらや腕全体を使って\n相手の胸や顎を攻撃する技である。"
    {0x40002000U, 0x00000185U, 9, {12, 8, 15}, "UIWAZAHH10"}, // @text EN: "A powerful surprise attack with\na spinning backfist." | JA: "回転して裏拳を放つ強力な奇襲技。"
    {0x40001000U, 0x0000006dU, 10, {17, 12, 13}, "UIWAZAHH11"}, // @text EN: "A strike on the pit of the stomach\nby the elbow bent at an acute angle." | JA: "相手の攻撃をかわし、回転の勢いを利用して\n鋭角に曲げた肘で相手の水月を突く技である。"
    {0x42000000U, 0x00000037U, 14, {15, 8, 17}, "UIWAZAHH12"}, // @text EN: "A diagonal blow to the area between\nthe shoulder and a nape of the neck." | JA: "走るスピードを利用し、敵の肩口から\n首筋にかけて、肘打ちを斜めに放つ技である。"
    {0x00000040U, 0x000000cdU, 4, {30, 10, 15}, "UIWAZAHH13"}, // @text EN: "A blow to the opponent's chin thrust\nby lowering the body and springing." | JA: "いったん体勢を低くしたあと\n腰のバネを活かし、相手のアゴを突き上げる技。"
    {0x00000002U, 0x004c004bU, 5, {42, 6, 16}, "UIWAZAHH14"}, // @text EN: "A slashing blow thrusted by\nswinging down the arm and up again." | JA: "腕を大きく振りおろしたのちに\n勢いを利用し振り上げる二連撃の技。"
    {0x00800000U, 0x00bc00bfU, 6, {45, 6, 12}, "UIWAZAHH15"}, // @text EN: "A mid-level strike by a second\npunch on the same target." | JA: "中段突きで同じ場所を再度攻撃する技。"
    {0x00000020U, 0x00000019U, 10, {32, 11, 20}, "UIWAZAHH16"}, // @text EN: "A penetrating blow with the palms\nusing power from the back twist." | JA: "裏回転で生じた力を手刀で相手に叩きこむ。"
    {0x04000000U, 0x01950193U, 11, {30, 9, 31}, "UIWAZAHH17"}, // @text EN: "A blow by one hand on the opponent\nand slamming it with the other." | JA: "相手に密着させた掌をもう一方の掌で\n激しく叩く事で威力を浸透させる荒業。"
    {0x40400000U, 0x00d200d4U, 16, {25, 19, 30}, "UIWAZAHH18"}, // @text EN: "Deflecting the opponent's attack and\nkicking his leg at the same time." | JA: "燕青拳の技で、攻撃を受け流すと同時に\n相手の足を蹴って倒す、高度な技である。"
    {0x00100000U, 0x00000275U, 1, {15, 6, 11}, "UIWAZAHH19"}, // @text EN: "A thrust move delivered by stepping\nout the same side of foot and arm." | JA: "踏み込む足と同じ方の手を突き出す技。\n中国拳法では基本的な突き技。"
    {0x00000001U, 0x00000038U, 4, {50, 28, 15}, "UIWAZAHH20"}, // @text EN: "A big step backward and then a\nforward create a powerful uppercut." | JA: "後ろに大きく下がって身を縮め、前進するとともに\n全力でアッパーを放つ豪快な技。"
    {0x00040000U, 0x00000001U, 7, {25, 9, 23}, "UIWAZAHH21"}, // @text EN: "A Tai Chi hand move with force\ndelivered from the legs and waist." | JA: "太極拳の技の一つで、足腰の捻で発せられた力を\n掌に伝えて打ちこむ。"
    {0x00000400U, 0x00000149U, 8, {35, 14, 20}, "UIWAZAHH22"}, // @text EN: "A basic Baji Style move. The\ntwisting of the waist exerts force." | JA: "八極拳の基本技で最重要の技。\n腰の回転を利用して突きを放つ。"
    {0x00200000U, 0x00000289U, 8, {25, 8, 16}, "UIWAZAHH23"}, // @text EN: "While closing in, both palms are\nused to strike the opponent's chest." | JA: "一気に間合いを詰め、両手の掌を\n相手の胸に叩きつける技。"
    {0x00020000U, 0x00000119U, 9, {15, 14, 22}, "UIWAZAHH24"}, // @text EN: "The Elbow Attack thrusted on the\nside while avoiding the attack." | JA: "八極拳六大開の一つ。相手の攻撃をさばき、\n体制を崩し、側面から頂肘を打つ大技。"
    {0x02000000U, 0x0000027bU, 11, {15, 13, 15}, "UIWAZAHH25"}, // @text EN: "An upper blow on the chin and a\nconsecutive downward strike." | JA: "鈎手（こうしゅ）という手型で相手のアゴを跳ね上げ、\n隙をついて掌を落とす。"
    {0x00000800U, 0x00000276U, 17, {15, 10, 17}, "UIWAZAHH26"}, // @text EN: "Striking the collarbone or the head\nwhile moving to the opponent's side." | JA: "相手の側面に回り込むと同時に、\n上から握った拳を鎖骨や頭に叩きつける技。"
    {0x00400000U, 0x0000028aU, 18, {5, 10, 20}, "UIWAZAHH27"}, // @text EN: "A mid-level strike on the stomack\nwhile leaning toward the opponent." | JA: "その場から身を乗り出すようにして、\n相手の腹部へ中段突きを放つ技。"
}};

constexpr std::array<MoveDefinition, 23> moveDefinitions2{{
    {0x40010000U, 0x000000c8U, 0, {18, 6, 10}, "UIWAZALH00"}, // @text EN: "An upward sword-stroke kick, aimed\nat the opponent's head." | JA: "下から太刀（たち）を摺り上げるように\n繰り出される蹴り技。相手の頭部を狙う。"
    {0x40020000U, 0x000000c4U, 0, {20, 8, 10}, "UIWAZALH01"}, // @text EN: "A front kick aiming the front torso\nof the opponent with the toes." | JA: "いわゆる前蹴り。主に爪先で\n人体の正面にある急所を狙う。"
    {0x40040000U, 0x00000010U, 1, {18, 6, 14}, "UIWAZALH02"}, // @text EN: "A low to mid-level kick using the\npower of the forward leg." | JA: "体を前方に大きく移動させながら\n足刀で相手の下段から中段を蹴る技。"
    {0x40080000U, 0x0000000fU, 2, {18, 5, 20}, "UIWAZALH03"}, // @text EN: "A low, decisive kick aimed at the\noutside of the opponent's knee." | JA: "相手の膝の外側を狙い、小さく、かつ鋭く\n放たれるローキックの一種。"
    {0x80000800U, 0x0297029aU, 3, {40, 7, 25}, "UIWAZALH04"}, // @text EN: "A heel kick followed by another\nusing the other leg." | JA: "カカトで相手を蹴り上げた後、すかさず\nもう一方の足で蹴りを放つ二段蹴り。"
    {0x40800000U, 0x00b200b3U, 5, {32, 6, 12}, "UIWAZALH05"}, // @text EN: "A middle rotating kick followed by\na heel drop." | JA: "中段の回し蹴りから変化して、\nカカト落しを放つ技。軌道はきわめて小さい。"
    {0x41000000U, 0x000000abU, 5, {16, 11, 14}, "UIWAZALH06"}, // @text EN: "A back roundhouse kick striking\nin a downward diagonal motion." | JA: "後ろ回し蹴りの一種で、相手の上段から\n斜め下方向に振り下ろされる技。"
    {0x40100000U, 0x000000afU, 6, {27, 9, 15}, "UIWAZALH07"}, // @text EN: "A flying knee move made by\nleaping upward to kick." | JA: "一瞬体を縮めた後、伸び上がるように蹴る、\n一種の飛び膝蹴り。"
    {0x40008000U, 0x000000a1U, 12, {32, 7, 19}, "UIWAZALH08"}, // @text EN: "A powerful high-speed reverse\nroundhouse kick." | JA: "高速の反転後ろ回し蹴り。体を大きく倒す事で\n遠心力と威力を増加させている。"
    {0x40004000U, 0x0000009eU, 13, {25, 14, 16}, "UIWAZALH09"}, // @text EN: "A kick delivered while flipping\nforward in mid-air." | JA: "前方に空中回転をしながら蹴りを放つ技。\n芭月流の中でも特異なわざと言われている。"
    {0x60000000U, 0x000000a0U, 14, {36, 14, 13}, "UIWAZALH10"}, // @text EN: "A flying back roundhouse kick with\nforce by jump and rotation." | JA: "跳び後ろ回し蹴り。\n回転と跳躍の力が加わるため、威力が高い。"
    {0x50000000U, 0x000000aeU, 15, {22, 13, 20}, "UIWAZALH11"}, // @text EN: "A flying back kick, aiming\ndiagonally at the opponent's neck." | JA: "飛び後ろ蹴り。相手の首筋を\n袈裟（けさ）切りに蹴る大技である。"
    {0x40200000U, 0x000000aaU, 2, {28, 8, 11}, "UIWAZALH12"}, // @text EN: "After luring the opponent in,\na reaping kick is delivered." | JA: "攻撃を受け流すと同時に\n相手の足を刈って倒す技。"
    {0x01000000U, 0x00360035U, 3, {40, 4, 17}, "UIWAZALH13"}, // @text EN: "Using the spinning momentum of the\n1st kick, a 2nd kick is delivered." | JA: "空中で回し蹴りを出したあと、遠心力を活かし、\nもう１回転して蹴りを放つ荒技。"
    {0x00000008U, 0x000000b9U, 4, {23, 11, 26}, "UIWAZALH14"}, // @text EN: "A kick on the chin by sliding toward\nthe opponent and making a handstand." | JA: "相手に向かって少しすべり込み、\n逆立ちしながら相手のアゴ先を蹴る奇襲的な技。"
    {0x00000010U, 0x000000bbU, 4, {22, 7, 19}, "UIWAZALH15"}, // @text EN: "Sweeping out the opponent's leg\nwhile spinning on the pivot leg." | JA: "軸足を中心に急旋回して、もう一方の足で\n相手の足元を払う技である。"
    {0x00000004U, 0x00000133U, 6, {18, 11, 17}, "UIWAZALH16"}, // @text EN: "A Hook Kick by the heel, aiming\nat the opponent's neck." | JA: "相手の首を根こそぎ刈り取るようにカカトで蹴る豪快な技。\n頭の急所を狙い放つ。"
    {0x00001000U, 0x00a700a6U, 13, {15, 13, 11}, "UIWAZALH17"}, // @text EN: "A heel drop kick made\nafter a spinning kick." | JA: "回し蹴りを放った後に、もう一方の足で\nすかさず回転カカト落としを放つ迅速な技。  "
    {0x00000200U, 0x00000124U, 16, {20, 15, 17}, "UIWAZALH18"}, // @text EN: "Sliding the ground forward to sweep\nthe feet out." | JA: "走っている最中にいきなり、身体を回転させて\n滑り込み、相手の足元を払う技。"
    {0x10000000U, 0x00000279U, 12, {16, 7, 13}, "UIWAZALH19"}, // @text EN: "A kick delivered on the chin while\nturning the back on the opponent." | JA: "急に相手に背中を向けた後、真下から\nアゴに向かってカカトを蹴り上げる技。"
    {0x08000000U, 0x00000285U, 4, {20, 7, 15}, "UIWAZALH20"}, // @text EN: "A heel kick by lowering the body\nsuddenly in front of the opponent." | JA: "相手の目の前で急激に身を低くして、\n相手の足元をカカトで蹴る奇襲技。"
    {0x40000001U, 0x00000286U, 14, {36, 8, 24}, "UIWAZALH21"}, // @text EN: "A diagonal upward kick after\nrunning and jumping to the opponent." | JA: "相手に走り込み、目の前で跳躍し\n斜め下から急激に蹴りを突き上げる技。  "
    {0x40000020U, 0x0260027dU, 15, {20, 10, 10}, "UIWAZALH22"}, // @text EN: "After a low, elbow strike, an upward\nkick is delivered with a handstand." | JA: "相手の前で身を低くし回転肘打ち、\nすかざず逆立ち蹴りを放つ高度な蹴り技。"
}};

constexpr std::array<MoveDefinition, 22> moveDefinitions3{{
    {0x80000004U, 0x000000f0U, 0, {30, 1, 16}, "UIWAZATH00"}, // @text EN: "The opponent is lifted over the back\nand is thrown over." | JA: "柔術でも古典的な技の一つ。\n相手の腕を取り、懐に入り込んで投げる。"
    {0x80000008U, 0x00000103U, 1, {45, 1, 27}, "UIWAZATH01"}, // @text EN: "The opponent's inner thigh is\nswept up to throw." | JA: "柔術でも古典的な技の一つ。足を使って\n相手の内股の部分を払い上げて投げる。"
    {0x80000010U, 0x000000dfU, 2, {39, 1, 40}, "UIWAZATH02"}, // @text EN: "The practitioner falls backward to\nthrow the opponent overhead to rear." | JA: "柔術でも古典的な技の一つ。\n自分が後方に倒れながら、相手を後ろに投げる。"
    {0x80000020U, 0x000000deU, 3, {52, 1, 38}, "UIWAZATH03"}, // @text EN: "Grabbing the opponent's neck and\nsweeping the opponent's pivot leg." | JA: "相手の首に腕を巻きつけ、\n体重のかかっている足を刈る投げ技。"
    {0x80000040U, 0x000000e5U, 4, {58, 1, 30}, "UIWAZATH04"}, // @text EN: "While throwing the opponent, the\npractitioner flips and lands on him." | JA: "相手を投げると同時に、自らも回転し\n相手の上に身体を落とす大技。"
    {0x80000080U, 0x00000105U, 5, {52, 1, 1}, "UIWAZATH05"}, // @text EN: "Grasping the arm and executing\nan armlock to pin." | JA: "相手の腕を取り逆関節を決めて押さえつける技。\n古流柔術では基本的な技である。"
    {0x80000100U, 0x000000d0U, 6, {45, 1, 13}, "UIWAZATH06"}, // @text EN: "Lifting the opponent on the shoulder\nand dropping him headfirst." | JA: "身を低くしながら相手の足を取り、肩に担ぎあげ、\n地面に叩き落とす豪快な技。"
    {0x80000200U, 0x000000cbU, 7, {52, 1, 21}, "UIWAZATH07"}, // @text EN: "Closing in on the opponent's side\nto throw down while spinning." | JA: "相手の側面から接近し、相手を半回転させながら\n投げ落とす技。"
    {0x80000400U, 0x000000c9U, 8, {71, 1, 16}, "UIWAZATH08"}, // @text EN: "The throw is executed with\na twist toward the rear." | JA: "柔術でも古典的な技の一つ。後方にひねり落すように\n相手を投げる。"
    {0x00000100U, 0x000000ebU, 12, {0, 1, 26}, "UIWAZATH09"}, // @text EN: "Evading the opponent and moving\nback into his blind spot to attack." | JA: "相手の攻撃をかわしつつ、背後に回り込み\n敵の死角をつく技。"
    {0x00002000U, 0x000000f1U, 9, {19, 1, 14}, "UIWAZATH10"}, // @text EN: "A complex move damaging the\nopponent's arms in multiple areas." | JA: "相手の腕関節に数ヶ所同時にダメージを与える、\n複雑な高等技である。"
    {0x00000080U, 0x00da00dcU, 10, {70, 1, 34}, "UIWAZATH11"}, // @text EN: "Crouching momentarily and lunging\nheadfirst into the opponent." | JA: "一瞬、身体を低くして力をため、瞬発力をもって、\n相手に強烈な頭突きをする技。"
    {0x00004000U, 0x00ee00edU, 13, {41, 1, 12}, "UIWAZATH12"}, // @text EN: "Striking the back of the neck after\nmoving into opponent's blind spot." | JA: "影身の派生技。後方に回ったあと、\n無防備となった相手の首筋に手刀を打ち込む。"
    {0x00008000U, 0x000000d6U, 14, {61, 1, 39}, "UIWAZATH13"}, // @text EN: "Evading the opponent's attack\ndiagonally to make body attack." | JA: "相手の攻撃をななめにかわすと同時に\n死角から急接近し、体当たりする技。"
    {0x00010000U, 0x00000120U, 11, {52, 1, 2}, "UIWAZATH14"}, // @text EN: "A strike on the chin delivered while\nmoving to the rear of the opponent." | JA: "相手の後方に足を進めると同時に\n鈎手（こうしゅ）でアゴを打ち倒す太極拳の技。"
    {0x00080000U, 0x0000001eU, 0, {30, 1, 13}, "UIWAZATH15"}, // @text EN: "An advanced throw move. The\ndirection of the throw is unlimited." | JA: "本来の意味は投げ技。色々な方向に\n好きなように投げ分ける高度な投げ技。"
    {0x40000002U, 0x0000027aU, 1, {58, 1, 24}, "UIWAZATH16"}, // @text EN: "While reaping both legs of the\nopponent, the torso is thrusted." | JA: "蟷螂拳の技で相手の両足を刈ると同時に\n上半身を手で払う投げ技。"
    {0x40000004U, 0x00000273U, 3, {51, 1, 10}, "UIWAZATH17"}, // @text EN: "A top and middle-level thrust\nand blow delivered consecutively." | JA: "連続で素早く直突きや裏拳を上段、中段に放つ\n高度な連続技。"
    {0x40000008U, 0x00000287U, 5, {40, 1, 12}, "UIWAZATH18"}, // @text EN: "A strike on the side followed by a\nthrow after a quick move." | JA: "相手の側面から掌を打ち、すぐに転身して\n反対の側面に入り投げ倒す技。"
    {0x20000000U, 0x0000028eU, 9, {15, 1, 10}, "UIWAZATH19"}, // @text EN: "A strike on the head by making the\nopponent fall as jumping on to him." | JA: "素早く相手の肩口にのぼり、\n相手を転ばせると同時に頭部を強打する投げ技。"
    {0x80001000U, 0x0000029dU, 15, {90, 1, 5}, "UIWAZATH20"}, // @text EN: "Three rapid strikes on the eyes\nand the throat of the opponent." | JA: "八極拳八大招式の一つ。目や喉を\n素早く３回突く技で、恐ろしい殺傷力を持つ。"
    {0x40000010U, 0x00000132U, 16, {0, 0, 0}, "UIWAZATH21"}, // @text EN: "A technique to feel the opponent's\nmove using all senses." | JA: "目だけでなく、全身を使って\n相手の気配を感じる高度な技術。"
}};

std::array<WazamakiMoveInfo, 28> moveInfo1{};
std::array<WazamakiMoveInfo, 23> moveInfo2{};
std::array<WazamakiMoveInfo, 22> moveInfo3{};

std::span<const MoveDefinition> MoveDefinitions(std::uint8_t category) {
    switch (category) {
    case 1:
        return moveDefinitions1;
    case 2:
        return moveDefinitions2;
    case 3:
        return moveDefinitions3;
    default:
        return {};
    }
}

std::span<WazamakiMoveInfo> MoveInfoCache(std::uint8_t category) {
    switch (category) {
    case 1:
        return moveInfo1;
    case 2:
        return moveInfo2;
    case 3:
        return moveInfo3;
    default:
        return {};
    }
}

std::array<std::uint32_t, 4> ReadSkillFlags() {
    std::array<std::uint32_t, 4> flags{};
    std::memcpy(flags.data(), S2Save_GetPersistentPlayerStats().statFlags.data(), sizeof(flags));
    return flags;
}

bool SkillFlagSet(const std::array<std::uint32_t, 4> &flags, std::uint32_t encoded) {
    return (flags[encoded >> 30U] & encoded & 0x3fffffffU) != 0;
}

std::array<WazamakiLearnedGroup, 73> wazamakiLearnedView{};
std::uint8_t wazamakiPresentationCategory{1}, wazamakiPresentationVariant{};
CameraState wazamakiSavedCameraState{};
Vector3 wazamakiSavedCameraEye{}, wazamakiSavedCameraTarget{};
std::uint16_t wazamakiSavedPerspective{};
std::uint32_t wazamakiSavedClockSuppressed{}, wazamakiSavedNpcLoadState{}, wazamakiSavedHelpTopic{};
std::uint16_t wazamakiSavedHelpFlags{};
std::int32_t wazamakiSavedPostFxFilter{}, wazamakiSavedPostFxBank{};
Character *wazamakiPreviewCharacter{};
std::int32_t wazamakiPreviewYaw{}, wazamakiPreviewMotionYaw{}, wazamakiPreviewPitch{}, wazamakiPreviewRoll{}, wazamakiPreviewBaseYaw{};
Vector3 wazamakiPreviewPosition{};
Matrix4x4 wazamakiPreviewRootTransform{};
std::uint16_t wazamakiPreviewPendingMotion{}, wazamakiPreviewPreviousAnalog{};
std::int8_t wazamakiPreviewFrameDelay{};
float wazamakiPreviewScale{};
void *wazamakiRyoFile{};
MT7Model wazamakiRyoModel;
void *wazamakiGroundFile{};
MT7Model wazamakiGroundModel;
std::array<std::int32_t, 8> wazamakiTextures{};
static_assert(sizeof(wazamakiTextures) == 0x20);
ConversationBuffer *wazamakiAudio{};
std::uint32_t wazamakiPulseBrightness{};
std::int8_t wazamakiPulseStep{};
HLib::Task *wazamakiTask{};
std::uint8_t wazamakiPhase{};
std::uint32_t wazamakiLegacyState{};
std::int32_t wazamakiSavedPlayerControl{};
std::uint16_t wazamakiTicks{};
std::uint32_t wazamakiFrameCount{};
float wazamakiAlpha{};
std::array<NativeWazamakiGroup, 28> wazamakiHandGroups{};
std::array<NativeWazamakiGroup, 23> wazamakiLegGroups{};
std::array<NativeWazamakiGroup, 22> wazamakiThrowGroups{};
std::array<std::uint8_t, 4> wazamakiGroupCounts{};
LegacyIconSprite wazamakiStatisticLabel{};
LegacyColourRect wazamakiStatisticBar{};
LegacyIconSprite wazamakiScrollTrackCap{}, wazamakiScrollTrackBody{}, wazamakiScrollThumbCap{}, wazamakiScrollThumbBody{};
LegacyIconSprite wazamakiNameSprite{}, wazamakiBackgroundSprite{}, wazamakiInputSprite{}, wazamakiCriticalSprite{};
LegacyColourRect wazamakiRowBar{};
std::array<std::uint16_t, 3> wazamakiCriticalFrames{};
std::int32_t wazamakiSelectedRow{}, wazamakiFirstVisible{};
std::uint8_t wazamakiInputPhase{};
std::int8_t wazamakiRepeatDelay{};
std::uint16_t wazamakiTransitionFrames{};
std::uint32_t wazamakiReverseVariant{};
std::array<std::array<std::int16_t, 40>, 3> wazamakiNameSurfaces{};
std::array<std::array<std::uint16_t, 40>, 3> wazamakiNameLengths{};
std::array<std::int16_t, 3> wazamakiStatSurfaces{};
WazamakiDescriptionWindow wazamakiDescription{};

std::span<NativeWazamakiGroup> NativeWazamakiGroups(std::uint8_t category) {
    switch (category) {
    case 1:
        return wazamakiHandGroups;
    case 2:
        return wazamakiLegGroups;
    case 3:
        return wazamakiThrowGroups;
    default:
        return {};
    }
}

NativeWazamakiGroup *FindWazamakiGroup(std::int32_t row, std::int32_t *category, std::int32_t *groupIndex = nullptr) {
    *category = -1;
    for (std::int32_t current = 1; current < 4; ++current) {
        if (row >= wazamakiGroupCounts[current]) {
            row -= wazamakiGroupCounts[current];
            continue;
        }
        *category = current;
        NativeWazamakiGroup *result = nullptr;
        std::int32_t scanned = 0;
        NativeWazamakiGroup *cursor = NativeWazamakiGroups(static_cast<std::uint8_t>(current)).data();
        for (std::int32_t found = 0; found <= row; ++found) {
            do {
                result = cursor++;
                ++scanned;
            } while (result->count == 0 && scanned < 40);
        }
        if (row >= 0 && groupIndex)
            *groupIndex = scanned - 1;
        return result;
    }
    return nullptr;
}

void CacheWazamakiGroups() {
    for (std::int32_t row = 0; row < wazamakiGroupCounts[0]; ++row) {
        std::int32_t category, groupIndex;
        const NativeWazamakiGroup *const native = FindWazamakiGroup(row, &category, &groupIndex);
        wazamakiLearnedView[row] = {.category = static_cast<std::uint8_t>(category),
            .group = static_cast<std::uint8_t>(groupIndex),
            .count = native->count,
            .selected = native->selected,
            .moves = native->moves};
    }
}

void MoveLearnedOwner_NullCallback(void *) {

}

void SetupModernMovesCamera();

bool LoadLegacyWazamakiTextures() {
    for (std::int32_t &texture : wazamakiTextures)
        texture = -1;
    void *allocation = nullptr;
    std::size_t size = 0;
    if (!LoadLegacySprite("sprite", "WAZAHYOU.SPR", WAZAMAKI_SPRITE_FILE, allocation, size))
        return false;
    TEXNReader(allocation, static_cast<std::int64_t>(size), wazamakiTextures.data());
    HLib::FreeTaskData(allocation);
    for (const std::int32_t texture : wazamakiTextures)
        if (texture == -1)
            return false;
    return true;
}

void UpdateLegacyPreviewTransform();
void PlayCurrentLegacyMove();

} // namespace

// @verified - p
std::int64_t ResetWazamakiState() noexcept {
    wazamakiTextures[0] = wazamakiTextures[1] = -1;
    wazamakiTask = nullptr;
    wazamakiLegacyState = 0;
    wazamakiPhase = 0;
    for (std::size_t index = 2; index < wazamakiTextures.size(); ++index)
        wazamakiTextures[index] = -1;
    return 0;
}

void LoadRyoSceneForMovesScreen() {
    wazamakiPreviewCharacter = nullptr;
    char directory[40];
    std::sprintf(directory, "scene/%02d/model/chara", g_actNumber);
    std::uint32_t size = 0;
    if (!(wazamakiRyoFile = LoadLegacySceneFile(directory, "RYO_M.MT7", MEMORY_MOVES_RYO, &size)))
        return;
    char path[272];
    std::snprintf(path, sizeof(path), "%s/RYO_M.MT7", directory);
    if (!ReadModelFormat7({static_cast<const std::byte *>(wazamakiRyoFile), size}, path, wazamakiRyoModel, 0x40) || wazamakiRyoModel.nodes.empty())
        return;
    if (!(wazamakiGroundFile = LoadLegacySceneFile("model/item", "WAZ0508G.MT7", MEMORY_MOVES_GROUND, &size))) {
        ReleaseWazamakiPreviewModels();
        return;
    }
    if (!ReadModelFormat7({static_cast<const std::byte *>(wazamakiGroundFile), size}, "model/item/WAZ0508G.MT7", wazamakiGroundModel, 0x40) ||
        wazamakiGroundModel.nodes.empty()) {
        ReleaseWazamakiPreviewModels();
        return;
    }
    HLib::Task *handler = nullptr;
    wazamakiPreviewCharacter = AddHighModelCharacter(MOVES_PREVIEW_CHARACTER, handler);
    if (!wazamakiPreviewCharacter) {
        ReleaseWazamakiPreviewModels();
        return;
    }
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    AssignScenarioCharacter(control->scenarioTask, MOVES_PREVIEW_CHARACTER, wazamakiPreviewCharacter);
    AllocateModelImageLevel(control->modelManagerTask, "WZMD", directory, "RYO_M.MT7");
    SetupImageModuleHandlers(wazamakiPreviewCharacter);
    SetupImageModule(wazamakiPreviewCharacter, control->modelManagerTask, "WZMD", 8);
    mtAssignClothFlags(wazamakiPreviewCharacter, static_cast<std::int8_t>(-1));
    AllocateMotionWork(wazamakiPreviewCharacter);
    AttachReflectorBoardComponent(wazamakiPreviewCharacter, 1);
    modelSetCharaAlpha(wazamakiPreviewCharacter, 1.0f);
    EV_SetCharDisplay(wazamakiPreviewCharacter, 3, 0);
    mtwkSetExtraFlag(FindMotionWork(wazamakiPreviewCharacter), 6, 2);
    SetMotionEnvironmentEnabled(0);
    charaToggleFlag(wazamakiPreviewCharacter, 0);
    wazamakiPreviewScale = wazamakiPreviewDefaultScale;
    UpdateLegacyPreviewTransform();
    wazamakiPreviewPitch = wazamakiPreviewInitialPitch;
    wazamakiPreviewRoll = wazamakiPreviewInitialRoll;
    wazamakiPreviewYaw = 0x8000 - wazamakiPreviewBaseYaw;
    wazamakiPreviewPreviousAnalog = 0x8080;
    PlayCurrentLegacyMove();
    wazamakiPreviewFrameDelay = 1;
}

namespace {

WazamakiLearnedGroup *CurrentLearnedGroup() {
    CacheWazamakiGroups();
    return wazamakiSelectedRow < wazamakiGroupCounts[0] ? &wazamakiLearnedView[wazamakiSelectedRow] : nullptr;
}

void RequestWazamakiPreviewMotion(Character *character, std::uint8_t category, std::uint8_t id) {
    const std::span<const MoveDefinition> definitions = MoveDefinitions(category);
    const std::span<const S2MoveSkillBank, 3> banks = S2Save_GetMoveSkillBanks();
    if (category == 0 || category > banks.size() || id >= definitions.size())
        return;

    const bool learned = banks[category - 1U].proficiency[id] >= 12;
    wazamakiPreviewPendingMotion = static_cast<std::uint16_t>(learned ? definitions[id].motionId : 306U);
    if (character) {
        MotionWork *const work = FindMotionWork(character);
        mtwkRequestMotion(work, wazamakiPreviewPendingMotion, MotionRegion::LowerBody);
        SetMotionRegionFlag(work, 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(work, 0, 2);
        SetMotionRegionFlag(work, 66, 0, MotionRegion::LowerBody);
    }
    wazamakiPreviewMotionYaw = 0;
    if (category == 3 && (id > 21 || !(0x00201900U & (1U << id))) && learned)
        wazamakiPreviewMotionYaw = 0x8000;
}

void PlayCurrentLegacyMove() {
    WazamakiLearnedGroup *const group = CurrentLearnedGroup();
    if (!group || !group->count)
        return;
    const std::uint8_t id = group->moves[group->selected];
    RequestWazamakiPreviewMotion(wazamakiPreviewCharacter, group->category, id);
    wazamakiPresentationCategory = group->category;
    wazamakiPresentationVariant = group->selected;
}

std::int32_t UpdatePreviewAnalogAngle(std::int32_t current, std::int32_t target, std::uint8_t analog, std::int32_t step) {
    if (analog == 128)
        return target;
    const std::int32_t offset = static_cast<std::int32_t>(analog) - 128;
    const std::uint16_t direction = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(offset) * 8192.0f / 45.0f));
    if (direction <= 0x8000U) {
        current += step;
        return current < target ? current : target;
    }
    current -= step;
    return current > target ? current : target;
}

void UpdateLegacyPreviewTransform() {

    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    const std::uint16_t authoredAngle = static_cast<std::uint16_t>(static_cast<std::int32_t>(wazamakiPreviewCameraAngle * 8192.0f / 45.0f));
    wazamakiPreviewBaseYaw = camCtrlGetNextRotY() - authoredAngle;
    const std::int16_t yaw = static_cast<std::int16_t>(wazamakiPreviewBaseYaw);
    wazamakiPreviewPosition = {
        .x = sin_fixed16(yaw) * wazamakiPreviewDistance + eye.x,
        .y = eye.y - wazamakiPreviewHeight + 0.05f,
        .z = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw)))) * wazamakiPreviewDistance + eye.z,
    };
    shenApplyTranslationToAll(&wazamakiPreviewCharacter->entity.node, wazamakiPreviewPosition);
    shenSetRotY(wazamakiPreviewCharacter, wazamakiPreviewCharacterYaw);
    const LegacyInputSlot &input = g_inputBuffer[0];
    wazamakiPreviewPreviousAnalog = static_cast<std::uint16_t>(input.analog0e | (static_cast<std::uint16_t>(input.analog0f) << 8U));
    const std::int32_t targetYaw = wazamakiPreviewBaseYaw + (static_cast<std::int32_t>(input.analog0e) - 128) * 256;
    const std::int32_t targetPitch = (static_cast<std::int32_t>(input.analog0f) - 128) * 128;
    wazamakiPreviewYaw = UpdatePreviewAnalogAngle(wazamakiPreviewYaw, targetYaw, input.analog0e, wazamakiPreviewYawStep);
    wazamakiPreviewPitch = UpdatePreviewAnalogAngle(wazamakiPreviewPitch, targetPitch, input.analog0f, wazamakiPreviewPitchStep / 2);
}

} // namespace

void UpdateWazamakiPreviewFrame() {

    if (!wazamakiPreviewCharacter)
        return;
    if (wazamakiPreviewFrameDelay >= 0 && --wazamakiPreviewFrameDelay < 0)
        EV_SetCharDisplay(wazamakiPreviewCharacter, 3, 0);
    wazamakiPreviewScale = wazamakiPreviewDefaultScale;
    if (!(g_loadingFlag & 2U))
        UpdateLegacyPreviewTransform();
}

// @verified - p
std::uint32_t WazamakiMatrixUpdateNullsub() {
    return 0;
}

void DrawWazamakiPreview() {

    if (!wazamakiPreviewCharacter)
        return;
    const std::int32_t pitch = wazamakiPreviewMotionYaw ? wazamakiPreviewPitch : -wazamakiPreviewPitch;
    UpdateGlobalLighting({});
    ResetLightEntry(0);
    SetLightKind(0, 2);
    SetLegacySelectedLightEnabled(0, true);
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    Vector3 direction{wazamakiPreviewPosition.x - eye.x, wazamakiPreviewPosition.y - eye.y, wazamakiPreviewPosition.z - eye.z};
    Vector3::NormalizeInPlace(direction);
    SetSelectedLightDirection(0, direction);
    shenSetLightColour(0, {0.7f, 0.7f, 0.7f, 0.7f});
    SetCurrentActiveLight(1);
    UpdateLegacyLightTransforms();
    Matrix4x4_Push();
    WazamakiMatrixUpdateNullsub();
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(wazamakiPreviewPosition.x, wazamakiPreviewScale + wazamakiPreviewPosition.y, wazamakiPreviewPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(wazamakiPreviewYaw + wazamakiPreviewMotionYaw, pitch, wazamakiPreviewRoll);
    Matrix4x4::Scale(wazamakiPreviewScale, wazamakiPreviewScale, wazamakiPreviewScale);
    Matrix4x4::preTranslate(0.0f, 0.0f, 0.0f);
    Matrix4x4::GetTransformMatrix(&wazamakiPreviewRootTransform);
    Matrix4x4::Pop(1);
    WazamakiMatrixUpdateNullsub();
    SetMotionRootTransformOverride(FindMotionWork(wazamakiPreviewCharacter), &wazamakiPreviewRootTransform);
    ExecuteCharacterCoreDraw(wazamakiPreviewCharacter);
    Matrix4x4_Push();
    WazamakiMatrixUpdateNullsub();
    Matrix4x4::preTranslate(wazamakiPreviewPosition.x, wazamakiPreviewScale + wazamakiPreviewPosition.y, wazamakiPreviewPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(wazamakiPreviewYaw + wazamakiPreviewMotionYaw, pitch, wazamakiPreviewRoll);
    Matrix4x4::Scale(wazamakiPreviewScale, wazamakiPreviewScale, wazamakiPreviewScale);
    Matrix4x4::preTranslate(0.0f, -1.0f, 0.0f);
    SetLegacyRenderQueueEnabled(false);
    ConstructMatricesForObjectRecursive(wazamakiGroundModel, wazamakiGroundModel.nodes.empty() ? nullptr : wazamakiGroundModel.nodes.data());
    SetLegacyRenderQueueEnabled(true);
    Matrix4x4::Pop(1);
    WazamakiMatrixUpdateNullsub();
    ImmediateLightWork(nullptr);
}

void ReleaseWazamakiPreviewModels() {

    LcdmInit(0);
    g_worldDrawingEnabled = true;
    SetMotionEnvironmentEnabled(1);
    if (wazamakiPreviewCharacter) {
        EV_SetCharDisplay(wazamakiPreviewCharacter, 3, 0);
        ctrlSetKillAndIncrementCounter(wazamakiPreviewCharacter, 1);
    }
    wazamakiPreviewCharacter = nullptr;
    ReleaseModelFormat7(wazamakiRyoModel);
    ReleaseModelFormat7(wazamakiGroundModel);
    if (wazamakiRyoFile)
        HLib::FreeTaskData(wazamakiRyoFile);
    if (wazamakiGroundFile)
        HLib::FreeTaskData(wazamakiGroundFile);
    wazamakiRyoFile = wazamakiGroundFile = nullptr;
    wazamakiPreviewFrameDelay = -1;
}

namespace {

void UpdateWazamakiNameSurfaces(std::int32_t mode) {
    const std::span<const S2MoveSkillBank, 3> banks = S2Save_GetMoveSkillBanks();
    for (std::int32_t row = 0; row < wazamakiGroupCounts[0]; ++row) {
        std::int32_t category;
        const NativeWazamakiGroup *const group = FindWazamakiGroup(row, &category);
        const bool visible = row >= wazamakiFirstVisible && row < wazamakiFirstVisible + 8;
        for (std::uint8_t variant = 0; variant < group->count; ++variant) {
            const std::uint8_t id = group->moves[variant];
            const char *const name = MOVE_NAME_TOKENS[category][id].data();
            wazamakiNameLengths[category - 1][id] = static_cast<std::uint16_t>(std::strlen(name) >> 1U);
            std::int16_t &surface = wazamakiNameSurfaces[category - 1][id];
            if (mode == 1 && !visible) {
                if (surface != -1)
                    ReleaseNamedTexture(surface);
                surface = -1;
            } else if (mode == 0 && visible && surface == -1) {
                const std::int16_t colour = banks[category - 1].proficiency[id] == 199 ? -12 : -1;
                const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                surface = static_cast<std::int16_t>(CreateLeftAlignedWindowTextSurface(control->windowTask, name, 512, colour, -4096, 0));
            }
        }
    }
}

void InitializeWazamakiDescription(float left, float top, float right, float bottom) {
    wazamakiDescription.x = (left + right) * 0.5f;
    wazamakiDescription.y = (top + bottom) * 0.5f;
    wazamakiDescription.width = static_cast<std::int32_t>(std::fabs(right - left));
    wazamakiDescription.height = static_cast<std::int32_t>(std::fabs(bottom - top));
    wazamakiDescription.scale = 1.0f;
    wazamakiDescription.alpha = 0.5f;
    wazamakiDescription.textX = left + 28.0f;
    wazamakiDescription.textY = top + 16.0f;
    wazamakiDescription.columns = -2 - static_cast<std::int32_t>(static_cast<float>(wazamakiDescription.width) / -28.0f);
    wazamakiDescription.rows = -2 - static_cast<std::int32_t>(static_cast<float>(wazamakiDescription.height) * -0.0625f);
    if (wazamakiDescription.rows > 8)
        wazamakiDescription.rows = 8;
    for (std::array<std::int32_t, 2> &line : wazamakiDescription.textures)
        for (std::int32_t &surface : line)
            surface = -1;
}

void UpdateWazamakiDescriptionText(const char *text) {
    char buffer[256];
    std::strncpy(buffer, text, sizeof(buffer));
    for (std::array<std::int32_t, 2> &line : wazamakiDescription.textures) {
        for (std::int32_t &surface : line) {
            if (surface != -1)
                ReleaseNamedTexture(surface);
            surface = -1;
        }
    }
    char *token = std::strtok(buffer, "#");
    std::int32_t line = 0, half = 0;
    while (token && line < wazamakiDescription.rows) {
        char part[48];
        std::strncpy(part, token, 36);
        part[36] = 0;
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        wazamakiDescription.textures[line][half] = static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(control->windowTask, part, 512, -1, -4096, 0));
        if (std::strlen(token) > 36) {
            token += 36;
            half ^= 1;
            if (half == 0)
                ++line;
        } else {
            ++line;
            half = 0;
            token = std::strtok(nullptr, "#");
        }
    }
}

void RefreshWazamakiDescriptionAndMotion() {
    std::int32_t category;
    const NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &category);
    const std::uint8_t id = group->moves[group->selected];
    const char *const text = MoveDefinitions(static_cast<std::uint8_t>(category))[id].textToken;
    if (std::strlen(text) == 0)
        return;
    char buffer[256];
    std::sprintf(buffer, "%s.", text);
    UpdateWazamakiDescriptionText(buffer);
    RequestWazamakiPreviewMotion(wazamakiPreviewCharacter, static_cast<std::uint8_t>(category), id);
}

void SelectCurrentLegacyVariant() {
    std::int32_t category;
    NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &category);
    const std::int32_t step = wazamakiReverseVariant ? -1 : 1;
    group->selected = static_cast<std::uint8_t>((group->count + group->selected + step) % group->count);
    SetMoveEquipment(0, category, group->moves[group->selected]);
}

void UpdateLegacyWazamakiInput() {
    std::uint16_t pressed = 0, directional = 0;
    if (!(g_loadingFlag & 2U)) {
        const LegacyInputSlot &input = g_inputBuffer[0];
        pressed = input.pressedPriority;
        const std::uint16_t repeated = input.repeatedPresses;
        if (pressed || repeated) {
            if (repeated & 0x3000U) {
                wazamakiRepeatDelay = static_cast<std::int8_t>(static_cast<std::uint8_t>(wazamakiRepeatDelay) - 1U);
                if (wazamakiRepeatDelay > 0)
                    directional = repeated & pressed;
                else {
                    wazamakiRepeatDelay = 1;
                    directional = repeated;
                }
            }
        } else
            wazamakiRepeatDelay = 16;
    }

    switch (wazamakiInputPhase) {
    case 0: {
        std::int32_t category;
        if (directional & 0x1000U) {
            if (wazamakiSelectedRow > 0 && !(wazamakiTicks & 0xffU)) {
                wazamakiTicks = static_cast<std::uint16_t>((wazamakiTicks & 0xff00U) | 3U);
                ExecuteAicaCommand(0x8abU, 0, 0);
            }
            for (std::int32_t row = wazamakiSelectedRow - 1; row >= 0; --row) {
                const NativeWazamakiGroup *const group = FindWazamakiGroup(row, &category);
                if (group && group->count) {
                    wazamakiSelectedRow = row;
                    wazamakiTicks |= 0x100U;
                    wazamakiAlpha = 0.0f;
                    break;
                }
            }
            if (wazamakiSelectedRow < wazamakiFirstVisible) {
                wazamakiFirstVisible = wazamakiSelectedRow;
                UpdateWazamakiNameSurfaces(0);
                UpdateWazamakiNameSurfaces(1);
            }
        } else if (directional & 0x2000U) {
            if (wazamakiSelectedRow < wazamakiGroupCounts[0] - 1 && !(wazamakiTicks & 0xffU)) {
                wazamakiTicks = static_cast<std::uint16_t>((wazamakiTicks & 0xff00U) | 3U);
                ExecuteAicaCommand(0x8abU, 0, 0);
            }
            for (std::int32_t row = wazamakiSelectedRow + 1; row < wazamakiGroupCounts[0]; ++row) {
                const NativeWazamakiGroup *const group = FindWazamakiGroup(row, &category);
                if (group && group->count) {
                    wazamakiSelectedRow = row;
                    wazamakiTicks |= 0x100U;
                    wazamakiAlpha = 0.0f;
                    break;
                }
            }
            if (wazamakiSelectedRow >= wazamakiFirstVisible + 8) {
                wazamakiFirstVisible = wazamakiSelectedRow - 7;
                UpdateWazamakiNameSurfaces(0);
                UpdateWazamakiNameSurfaces(1);
            }
        }
        const NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &category);
        if (group->count > 1) {
            if (pressed & 0x400U)
                wazamakiInputPhase = 1;
            else if (pressed & 0x8000U) {
                wazamakiReverseVariant = 0;
                wazamakiInputPhase = 5;
            } else if (pressed & 0x4000U) {
                wazamakiReverseVariant = 1;
                wazamakiInputPhase = 5;
            }
            if (pressed & 0xc400U) {
                wazamakiTransitionFrames = 8;
                ExecuteAicaCommand(0xabU, 0, 0);
            }
        }
        if (pressed & 0x200U)
            wazamakiPhase = 3;
        break;
    }
    case 1:
        if (--wazamakiTransitionFrames == 0)
            wazamakiInputPhase = 2;
        break;
    case 2:
        if (pressed & 0x0600U) {
            wazamakiInputPhase = 4;
            wazamakiTransitionFrames = 8;
            (void)ExecuteAicaCommand(0x00abU, 0, 0);
        } else if (pressed & 0xa000U) {
            wazamakiInputPhase = 3;
            wazamakiTransitionFrames = 8;
            wazamakiReverseVariant = 0;
            (void)ExecuteAicaCommand(0x108abU, 0, 0);
        } else if (pressed & 0x5000U) {
            wazamakiInputPhase = 3;
            wazamakiTransitionFrames = 8;
            wazamakiReverseVariant = 1;
            (void)ExecuteAicaCommand(0x108abU, 0, 0);
        }
        break;
    case 3:
        if (--wazamakiTransitionFrames == 0) {
            SelectCurrentLegacyVariant();
            wazamakiTicks |= 0x100U;
            wazamakiAlpha = 0.0f;
            wazamakiInputPhase = 2;
        }
        break;
    case 4:
        if (--wazamakiTransitionFrames == 0)
            wazamakiInputPhase = 0;
        break;
    case 5:
        if (--wazamakiTransitionFrames == 0) {
            SelectCurrentLegacyVariant();
            wazamakiTicks |= 0x100U;
            wazamakiAlpha = 0.0f;
            wazamakiInputPhase = 0;
        }
        break;
    default:
        break;
    }
}

void DrawWazamakiStatistics() {
    wazamakiStatisticLabel.y = 304.0f;
    for (std::int32_t index = 0; index < 3; ++index) {
        wazamakiStatisticLabel.icon = static_cast<std::uint16_t>(wazamakiStatSurfaces[index]);
        PrepareDrawTextureWithMode(wazamakiStatisticLabel, ScreenMode::Centered);
        if (index != 2)
            wazamakiStatisticLabel.y += 20.0f;
    }
    std::int32_t category;
    NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &category);
    if (!group || !group->count)
        return;
    const std::uint8_t move = group->moves[group->selected];
    if (category == 3 && move == 21)
        return;
    const MoveDefinition &definition = MoveDefinitions(static_cast<std::uint8_t>(category))[move];
    const float power = std::fmax(0.05f, std::fmin(1.0f, static_cast<float>(definition.stats[0]) / 90.0f));
    const float speed = std::fmax(0.05f, std::fmin(1.0f, 1.0f - static_cast<float>(definition.stats[1]) / 28.0f));
    const float rigid = std::fmax(0.05f, std::fmin(1.0f, static_cast<float>(definition.stats[2]) / 40.0f));
    wazamakiStatisticBar.width = static_cast<std::int16_t>(power * 72.0f);
    wazamakiStatisticBar.y = 307.0f;
    DrawLegacyColourRect(wazamakiStatisticBar);
    wazamakiStatisticBar.y += 20.0f;
    wazamakiStatisticBar.width = static_cast<std::int16_t>(speed * 72.0f);
    DrawLegacyColourRect(wazamakiStatisticBar);
    wazamakiStatisticBar.y += 20.0f;
    wazamakiStatisticBar.width = static_cast<std::int16_t>(rigid * 72.0f);
    DrawLegacyColourRect(wazamakiStatisticBar);
}

void DrawWazamakiButtons() {
    wazamakiPulseBrightness += static_cast<std::uint32_t>(3 * wazamakiPulseStep);
    const std::int32_t brightness = std::bit_cast<std::int32_t>(wazamakiPulseBrightness);
    if (brightness >= 63) {
        wazamakiPulseBrightness = 63;
        wazamakiPulseStep = -1;
    } else if (brightness <= 0) {
        wazamakiPulseBrightness = 0;
        wazamakiPulseStep = 1;
    }
    LegacyIconSprite sprite{.x = 44.0f,
        .y = 52.0f,
        .scaleX = 0.85f,
        .scaleY = 0.85f,
        .depth = 9.0f,
        .flags = 0,
        .rotation = 0,
        .icon = 776,
        .width = 38,
        .height = 38,
        .alignment = 1};
    PrepareDrawSimple(sprite, (wazamakiPulseBrightness << 8) | 0xff000000U, ScreenMode::Centered);
    sprite.icon = 777;
    sprite.x += 40.800003f;
    PrepareDrawSimple(sprite, (wazamakiPulseBrightness << 16) | 0xff000000U, ScreenMode::Centered);
    sprite.icon = 2049;
    sprite.x += 40.800003f;
    sprite.y += -8.0f;
    sprite.width = sprite.height = 64;
    const std::uint32_t colour = wazamakiPulseBrightness | ((wazamakiPulseBrightness | (wazamakiPulseBrightness << 8)) << 8) | 0xff000000U;
    PrepareDrawSimple(sprite, colour, ScreenMode::Centered);
}

void DrawWazamakiScrollbar(float x, float top, float bottom, float first, float last) {
    wazamakiScrollTrackCap.x = wazamakiScrollTrackBody.x = wazamakiScrollThumbCap.x = wazamakiScrollThumbBody.x = x;
    wazamakiScrollTrackCap.y = wazamakiScrollThumbCap.y = wazamakiScrollThumbBody.y = top;
    wazamakiScrollTrackBody.y = top + 16.0f;
    wazamakiScrollTrackCap.flags = wazamakiScrollTrackBody.flags = wazamakiScrollThumbCap.flags = wazamakiScrollThumbBody.flags = 0;
    wazamakiScrollTrackCap.rotation = wazamakiScrollTrackBody.rotation = wazamakiScrollThumbCap.rotation = wazamakiScrollThumbBody.rotation = 0;
    wazamakiScrollTrackCap.scaleX = wazamakiScrollTrackCap.scaleY = wazamakiScrollTrackBody.scaleX = 1.0f;
    wazamakiScrollTrackBody.scaleY = (bottom - (top + 16.0f)) * 0.0625f;
    wazamakiScrollThumbCap.scaleX = wazamakiScrollThumbCap.scaleY = wazamakiScrollThumbBody.scaleX = wazamakiScrollThumbBody.scaleY = 1.0f;
    PrepareDrawTextureWithMode(wazamakiScrollTrackBody, ScreenMode::Centered);
    PrepareDrawTextureWithMode(wazamakiScrollTrackCap, ScreenMode::Centered);
    wazamakiScrollTrackCap.y = bottom;
    wazamakiScrollTrackCap.flags = 0x200;
    PrepareDrawTextureWithMode(wazamakiScrollTrackCap, ScreenMode::Centered);
    first = first < 0.0f ? 0.0f : (first > 1.0f ? 1.0f : first);
    last = last < 0.0f ? 0.0f : (last > 1.0f ? 1.0f : last);
    if (first < last) {
        const float start = top + 8.0f;
        const float length = (bottom - 8.0f) - start;
        const float thumbTop = length * first + start, thumbBottom = length * last + start;
        wazamakiScrollThumbBody.y = thumbTop + 8.0f;
        wazamakiScrollThumbBody.scaleY = (thumbBottom - thumbTop) * 0.0625f;
        PrepareDrawTextureWithMode(wazamakiScrollThumbBody, ScreenMode::Centered);
        wazamakiScrollThumbCap.y = thumbTop;
        PrepareDrawTextureWithMode(wazamakiScrollThumbCap, ScreenMode::Centered);
        wazamakiScrollThumbCap.y = thumbBottom;
        wazamakiScrollThumbCap.flags = 0x200;
        PrepareDrawTextureWithMode(wazamakiScrollThumbCap, ScreenMode::Centered);
    }
}

void DrawWazamakiDescription() {
    shenSubmitQuadsUI(wazamakiDescription.x, wazamakiDescription.y, 0x7f000000U, 0x7fffffffU, wazamakiDescription.width, wazamakiDescription.height, 10.0f);
    LegacyIconSprite left{.x = wazamakiDescription.textX,
        .y = wazamakiDescription.textY,
        .scaleX = wazamakiDescription.scale,
        .scaleY = wazamakiDescription.alpha,
        .depth = 11.0f,
        .flags = 0,
        .rotation = 0,
        .width = 512,
        .height = 32,
        .alignment = 0};
    LegacyIconSprite right{.x = left.x + 420.0f,
        .y = left.y,
        .scaleX = left.scaleX,
        .scaleY = left.scaleY,
        .depth = 11.0f,
        .flags = 0,
        .rotation = 0,
        .width = 128,
        .height = 32,
        .alignment = 0};
    if (wazamakiAlpha <= 0.0f)
        return;
    const double alpha = static_cast<double>(wazamakiAlpha) * 255.0;
#if defined(_M_X64) || defined(__x86_64__)
    const std::int64_t integerAlpha = _mm_cvttsd_si64(_mm_set_sd(alpha));
#else
    const std::int64_t integerAlpha = alpha >= -0x1p63 && alpha < 0x1p63 ? static_cast<std::int64_t>(alpha) : std::numeric_limits<std::int64_t>::min();
#endif
    const std::uint32_t colour = (static_cast<std::uint32_t>(integerAlpha) << 24U) | 0xffffffU;
    for (std::int32_t row = 0; row < wazamakiDescription.rows; ++row) {
        if (wazamakiDescription.textures[row][0] != -1) {
            left.icon = static_cast<std::uint16_t>(wazamakiDescription.textures[row][0]);
            shenTransformScreenCoors(left, colour, ScreenMode::Centered);
        }
        left.y += 16.0f;
        if (wazamakiDescription.textures[row][1] != -1) {
            right.icon = static_cast<std::uint16_t>(wazamakiDescription.textures[row][1]);
            shenTransformScreenCoors(right, colour, ScreenMode::Centered);
        }
        right.y += 16.0f;
    }
}

void DrawWazamakiRows() {
    std::int32_t selectedCategory;
    const NativeWazamakiGroup *const selectedGroup = FindWazamakiGroup(wazamakiSelectedRow, &selectedCategory);
    const std::int32_t extraRows = selectedGroup->count - 1;
    const std::int32_t lastCollapsedRow = wazamakiFirstVisible + 7 - extraRows;
    float y = 48.0f, selectedY = 68.0f;
    const std::span<const S2MoveSkillBank, 3> banks = S2Save_GetMoveSkillBanks();
    for (std::int32_t ordinal = 0; ordinal < 8; ++ordinal) {
        const std::int32_t row = wazamakiFirstVisible + ordinal;
        if (row == wazamakiSelectedRow)
            selectedY = y + 20.0f;
        const float remaining = static_cast<float>(wazamakiTransitionFrames) * 0.125f;
        float expansion = 0.0f, fade = 1.0f, scale = 1.0f, advance = 40.0f;
        switch (wazamakiInputPhase) {
        case 1:
            expansion = fade = 1.0f - remaining;
            break;
        case 2:
        case 3:
            expansion = fade = 1.0f;
            break;
        case 4:
            expansion = fade = remaining;
            break;
        case 5:
            fade = remaining;
            break;
        }
        const std::uint32_t fadeColour = (static_cast<std::uint32_t>(TruncateSpriteCoordinate(fade * 255.0f)) << 24U) | 0xffffffU;
        if (wazamakiSelectedRow <= lastCollapsedRow - 1) {
            if (row == wazamakiSelectedRow + 1)
                y += (expansion * 40.0f) * static_cast<float>(extraRows);
            if (row > lastCollapsedRow) {
                scale = 1.0f - expansion;
                advance = scale * 40.0f;
            }
        } else {
            if (row < wazamakiFirstVisible + extraRows) {
                scale = 1.0f - expansion;
                advance = scale * 40.0f;
            }
            if (row == wazamakiSelectedRow)
                advance = (expansion * 40.0f) * static_cast<float>(extraRows) + 40.0f;
        }
        if (scale < 0.01f)
            continue;
        std::int32_t category, groupIndex;
        const NativeWazamakiGroup *const group = FindWazamakiGroup(row, &category, &groupIndex);
        const std::uint8_t move = group->moves[group->selected];
        const std::int32_t nextVariant = (group->count + group->selected + (wazamakiReverseVariant ? -1 : 1)) % group->count;
        const std::uint8_t nextMove = group->moves[nextVariant];
        if (group->count) {
            float proficiency = static_cast<float>(banks[category - 1].proficiency[move]);
            if ((wazamakiInputPhase == 3 || wazamakiInputPhase == 5) && row == wazamakiSelectedRow)
                proficiency += (1.0f - remaining) * (static_cast<float>(banks[category - 1].proficiency[nextMove]) - proficiency);
            wazamakiRowBar.colour = proficiency < 12.0f ? 0x7f507daeU : proficiency == 200.0f ? 0x4ff03122U : 0x7fffaa54U;
            wazamakiRowBar.y = y + 8.0f;
            wazamakiRowBar.width = static_cast<std::int16_t>(TruncateSpriteCoordinate((proficiency / 200.0f) * 356.0f));
            if (wazamakiRowBar.width == 0)
                wazamakiRowBar.width = 2;
            wazamakiRowBar.scaleY = scale;
            DrawLegacyColourRect(wazamakiRowBar);
            const float nameY = y + 4.0f;
            wazamakiNameSprite.scaleX = 1.0f;
            wazamakiNameSprite.scaleY = scale;
            wazamakiNameSprite.y = nameY;
            wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category - 1][move]);
            bool drawName = true;
            if (wazamakiInputPhase == 3 && row == wazamakiSelectedRow && group->count > 1) {
                float xs[3], ys[3];
                xs[0] = wazamakiNameSprite.x;
                ys[0] = nameY;
                for (std::int32_t index = 1; index < group->count; ++index) {
                    xs[index] = xs[0] + 24.92f;
                    ys[index] = static_cast<float>(index) * 40.0f + ys[0];
                }
                const std::int32_t step = wazamakiReverseVariant ? 1 : -1;
                std::int32_t variant = group->selected;
                for (std::int32_t index = 0; index < group->count; ++index) {
                    const std::int32_t other = (index + group->count + step) % group->count;
                    wazamakiNameSprite.x = remaining * xs[index] + (1.0f - remaining) * xs[other];
                    wazamakiNameSprite.y = remaining * ys[index] + (1.0f - remaining) * ys[other];
                    const std::uint32_t colour = (step == -1 && index == 0) || (step == 1 && other == 0) ? 0x80ffffffU : fadeColour;
                    wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category - 1][group->moves[variant]]);
                    shenTransformScreenCoors(wazamakiNameSprite, colour, ScreenMode::Centered);
                    variant = (group->count + variant + 1) % group->count;
                }
                wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category - 1][move]);
                wazamakiNameSprite.y = nameY;
                drawName = false;
            } else if ((wazamakiInputPhase == 1 || wazamakiInputPhase == 2 || wazamakiInputPhase == 4) && row == wazamakiSelectedRow && group->count > 1) {
                wazamakiNameSprite.x += 24.92f;
                wazamakiNameSprite.y = nameY - (1.0f - expansion) * 32.0f;
                std::int32_t variant = (group->count + group->selected + 1) % group->count;
                for (std::int32_t index = 1; index < group->count; ++index) {
                    wazamakiNameSprite.y += 40.0f;
                    wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category - 1][group->moves[variant]]);
                    shenTransformScreenCoors(wazamakiNameSprite, fadeColour, ScreenMode::Centered);
                    variant = (group->count + variant + 1) % group->count;
                }
                wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category - 1][move]);
                wazamakiNameSprite.y = nameY;
            } else if (wazamakiInputPhase == 5 && row == wazamakiSelectedRow) {
                wazamakiNameSprite.scaleX = fade;
                wazamakiNameSprite.x = wazamakiReverseVariant ?
                    220.0f :
                    static_cast<float>(28 * static_cast<std::int16_t>(wazamakiNameLengths[category - 1][nextMove])) * (1.0f - fade) + 220.0f;
                shenTransformScreenCoors(wazamakiNameSprite, fadeColour, ScreenMode::Centered);
                wazamakiNameSprite.x = wazamakiReverseVariant ?
                    static_cast<float>(28 * static_cast<std::int16_t>(wazamakiNameLengths[category - 1][move])) * fade + 220.0f :
                    220.0f;
                wazamakiNameSprite.scaleX = 1.0f - fade;
                wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category - 1][nextMove]);
                const std::uint32_t colour = (static_cast<std::uint32_t>(TruncateSpriteCoordinate((1.0f - fade) * 255.0f)) << 24U) | 0xffffffU;
                shenTransformScreenCoors(wazamakiNameSprite, colour, ScreenMode::Centered);
                drawName = false;
            }
            if (drawName) {
                wazamakiNameSprite.x = 220.0f;
                PrepareDrawTextureWithMode(wazamakiNameSprite, ScreenMode::Centered);
            }
            if (category != 3 || move != 21) {
                wazamakiInputSprite.x = 608.0f;
                wazamakiInputSprite.y = ((scale * 32.0f) * 0.5f) + nameY;
                if (MOVE_CRITICAL_MASKS[category - 1] & (1U << move)) {
                    wazamakiCriticalSprite.x = 608.0f;
                    wazamakiCriticalSprite.y = wazamakiInputSprite.y;
                    wazamakiCriticalSprite.scaleY = scale;
                    const std::int32_t frame = std::bit_cast<std::int32_t>(wazamakiFrameCount) / 3 % 3;
                    wazamakiCriticalSprite.icon = wazamakiCriticalFrames[static_cast<std::uint8_t>(frame)];
                    PrepareDrawIndexedTextureWithSecondary(wazamakiCriticalSprite, 0xff000000U, ScreenMode::Centered);
                }
                wazamakiInputSprite.x += -38.400002f;
                const std::array<std::uint8_t, 7> &commands = WAZAMAKI_GROUP_COMMANDS[category][groupIndex];
                for (std::int32_t index = commands[0] - 1; index >= 0; --index) {
                    const std::uint8_t command = commands[index + 1];
                    wazamakiInputSprite.scaleX = wazamakiInputSprite.scaleY = 0.7f;
                    if (command == 2 || command == 3) {
                        wazamakiInputSprite.scaleX = scale * 0.7f;
                        wazamakiInputSprite.rotation = command == 2 ? 0x4000 : 0xc000;
                    } else {
                        wazamakiInputSprite.scaleY = scale * 0.7f;
                        wazamakiInputSprite.rotation = 0;
                    }
                    wazamakiInputSprite.icon = WAZAMAKI_INPUT_ICONS[command];
                    PrepareDrawSimpleOpaque(wazamakiInputSprite, WAZAMAKI_INPUT_COLOURS[command], ScreenMode::Centered);
                    wazamakiInputSprite.x += -22.4f;
                }
            }
            if (group->count > 1) {
                wazamakiInputSprite.scaleY = wazamakiInputSprite.scaleX;
                wazamakiInputSprite.icon = 776;
                wazamakiInputSprite.x = 204.0f;
                wazamakiInputSprite.rotation = 0;
                wazamakiInputSprite.scaleX = 0.7f;
                PrepareDrawSimpleOpaque(wazamakiInputSprite, (wazamakiPulseBrightness << 8U) | 0xff000000U, ScreenMode::Centered);
                wazamakiInputSprite.scaleX = wazamakiInputSprite.scaleY = 0.7f;
            }
        }
        y += advance;
    }
    shenSubmitQuadsUI(406.0f, selectedY, 0x1f0000ffU, 0xbf0000ffU, 372, 40, -48.0f);
}

void DrawLegacyWazamaki() {
    const std::int32_t columns = TruncateSpriteCoordinate(static_cast<float>(std::ceil(((1.0f - GetS2AspectRatioAdjustment()) + 1.0f) * 5.0f)));
    for (std::int32_t column = 0; column < columns; ++column) {
        wazamakiBackgroundSprite.x = static_cast<float>(column) * 128.0f + 64.0f;
        for (std::int32_t row = 0; row < 4; ++row) {
            wazamakiBackgroundSprite.y = static_cast<float>(row) * 128.0f + 64.0f;
            PrepareDrawIndexedTexture(wazamakiBackgroundSprite, static_cast<ScreenMode>(0));
        }
    }
    DrawWazamakiStatistics();
    DrawWazamakiScrollbar(172.0f, 40.0f, 352.0f, static_cast<float>(wazamakiFirstVisible) / wazamakiGroupCounts[0],
        static_cast<float>(wazamakiFirstVisible + 8) / wazamakiGroupCounts[0]);
    DrawWazamakiDescription();
    DrawWazamakiPreview();
    DrawWazamakiRows();
    DrawWazamakiButtons();
}

// @verified - p
void WazamakiLoadingCompleteNullsub() {
}

void RestoreWazamakiCamera() {

    camCtrlSetState(wazamakiSavedCameraState);
    camCtrlSetNextPosition(wazamakiSavedCameraEye);
    camCtrlSetTargetPosition(wazamakiSavedCameraTarget);
    camCtrlSetPerspective(wazamakiSavedPerspective);
}

void ReleaseLegacyWazamaki() {

    ReleaseWazamakiPreviewModels();
    SetHelpIconMenuFlags(wazamakiSavedHelpFlags);
    EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(wazamakiSavedHelpTopic), 0);
    SetRenderClockAndCompassTo(wazamakiSavedClockSuppressed ? 1 : 0);
    for (std::array<std::int16_t, 40> &category : wazamakiNameSurfaces) {
        for (std::int16_t &surface : category) {
            if (surface != -1)
                ReleaseNamedTexture(surface);
            surface = -1;
        }
    }
    for (std::int16_t &surface : wazamakiStatSurfaces) {
        if (surface != -1)
            ReleaseNamedTexture(surface);
        surface = -1;
    }
    for (std::int32_t &texture : wazamakiTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    for (std::array<std::int32_t, 2> &line : wazamakiDescription.textures) {
        for (std::int32_t &surface : line) {
            if (surface != -1)
                ReleaseNamedTexture(surface);
            surface = -1;
        }
    }
    EV_NPC_NewLoadSw(wazamakiSavedNpcLoadState);
    wazamakiLegacyState = 0;
    DispatchPostFxFilterRender(1);
    SetPostFxFilterMode(wazamakiSavedPostFxFilter);
    SelectPostFxBank(1, wazamakiSavedPostFxBank);
}

bool InitializeLegacyWazamaki() {
    wazamakiAlpha = 1.0f;
    wazamakiFrameCount = 0;
    wazamakiTicks = 0;
    wazamakiFirstVisible = 0;
    wazamakiInputPhase = 0;
    wazamakiPulseBrightness = 0;
    wazamakiPulseStep = 1;
    wazamakiSavedNpcLoadState = static_cast<std::uint32_t>(GetNpcNewLoadState());
    wazamakiSelectedRow = 0;
    wazamakiPresentationCategory = 1;
    wazamakiPresentationVariant = 0;
    wazamakiLegacyState = 1;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    RefreshWazamakiLearnedGroups();
    if (!LoadLegacyWazamakiTextures())
        return false;
    std::uint64_t freeBytes, largestBlock;
    HLib::TryGetFreeHeapStats(&freeBytes, &largestBlock);
    if (freeBytes < 475136U)
        return false;
    for (std::array<std::int16_t, 40> &category : wazamakiNameSurfaces)
        for (std::int16_t &surface : category)
            surface = -1;
    UpdateWazamakiNameSurfaces(0);
    UpdateWazamakiNameSurfaces(1);
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    wazamakiNameSprite.x = 220.0f;
    wazamakiNameSprite.flags = 0;
    wazamakiNameSprite.rotation = 0;
    wazamakiNameSprite.scaleX = wazamakiNameSprite.scaleY = 1.0f;
    wazamakiNameSprite.depth = 11.0f;
    wazamakiNameSprite.width = 512;
    wazamakiNameSprite.height = 32;
    wazamakiNameSprite.alignment = 0;
    const char *labels[3]{"Power", "Speed", "Rigid"};
    for (std::int32_t index = 0; index < 3; ++index) {
        wazamakiStatSurfaces[index] = static_cast<std::int16_t>(CreateLeftAlignedWindowTextSurface(control->windowTask, labels[index], 128, -1, -4096, 0));
    }

    wazamakiStatisticLabel.x = 22.0f;
    wazamakiStatisticLabel.flags = 0;
    wazamakiStatisticLabel.rotation = 0;
    wazamakiStatisticLabel.alignment = 0;
    wazamakiStatisticLabel.depth = -44.0f;
    wazamakiStatisticLabel.width = 128;
    wazamakiStatisticLabel.height = 16;
    wazamakiStatisticLabel.scaleX = wazamakiStatisticLabel.scaleY = 1.0f;
    for (std::int32_t index = 0; index < 3; ++index)
        wazamakiCriticalFrames[index] = static_cast<std::uint16_t>(wazamakiTextures[index + 5]);
    wazamakiCriticalSprite.flags = 0;
    wazamakiCriticalSprite.rotation = 0;
    wazamakiCriticalSprite.width = wazamakiCriticalSprite.height = 32;
    wazamakiCriticalSprite.scaleX = wazamakiCriticalSprite.scaleY = 1.0f;
    wazamakiCriticalSprite.depth = 11.0f;
    wazamakiCriticalSprite.icon = static_cast<std::uint16_t>(wazamakiTextures[5]);
    wazamakiCriticalSprite.alignment = 5;
    wazamakiSavedClockSuppressed = IsClockAndCompassSuppressed() ? 1U : 0U;
    SetRenderClockAndCompassTo(1);
    wazamakiSavedHelpTopic = helpGetCurrentIconIdx(1);
    EV_EventSetUpHelpIcon_0(0x33, 0);
    wazamakiSavedHelpFlags = static_cast<std::uint16_t>(GetUiFlag(0));
    SetHelpIconMenuFlags(0);
    wazamakiBackgroundSprite.x = wazamakiBackgroundSprite.y = 63.0f;
    wazamakiBackgroundSprite.flags = 0;
    wazamakiBackgroundSprite.rotation = 0;
    wazamakiBackgroundSprite.width = wazamakiBackgroundSprite.height = 128;
    wazamakiBackgroundSprite.scaleX = wazamakiBackgroundSprite.scaleY = 1.0f;
    wazamakiBackgroundSprite.depth = -50.0f;
    wazamakiBackgroundSprite.icon = static_cast<std::uint16_t>(wazamakiTextures[0]);
    wazamakiBackgroundSprite.alignment = 5;
    wazamakiRowBar.x = 228.0f;
    wazamakiRowBar.scaleX = wazamakiRowBar.scaleY = 1.0f;
    wazamakiRowBar.depth = -47.0f;
    wazamakiRowBar.height = 24;
    wazamakiRowBar.rotation = 0;
    wazamakiRowBar.alignment = 0;
    wazamakiInputSprite.flags = 0;
    wazamakiInputSprite.rotation = 0;
    wazamakiInputSprite.width = wazamakiInputSprite.height = 32;
    wazamakiInputSprite.scaleX = wazamakiInputSprite.scaleY = 0.7f;
    wazamakiInputSprite.depth = 11.0f;
    wazamakiInputSprite.alignment = 5;
    wazamakiStatisticBar.x = 95.0f;
    wazamakiStatisticBar.depth = -45.0f;
    wazamakiStatisticBar.colour = 0x5000ff05U;
    wazamakiStatisticBar.height = 12;
    wazamakiStatisticBar.scaleX = wazamakiStatisticBar.scaleY = 1.0f;
    wazamakiStatisticBar.rotation = 0;
    wazamakiStatisticBar.alignment = 0;
    LegacyIconSprite *const scrollSprites[]{&wazamakiScrollTrackCap, &wazamakiScrollTrackBody, &wazamakiScrollThumbCap, &wazamakiScrollThumbBody};
    for (std::int32_t index = 0; index < 4; ++index) {
        scrollSprites[index]->depth = -46.0f;
        scrollSprites[index]->width = scrollSprites[index]->height = 16;
        scrollSprites[index]->alignment = 0;
        scrollSprites[index]->icon = static_cast<std::uint16_t>(wazamakiTextures[index + 1]);
    }
    HLib::TryGetFreeHeapStats(&freeBytes, &largestBlock);
    if (freeBytes < 6144U)
        return false;
    LoadRyoSceneForMovesScreen();
    InitializeWazamakiDescription(16.0f, 378.0f, 624.0f, 446.0f);
    RefreshWazamakiDescriptionAndMotion();
    wazamakiSavedPostFxFilter = GetPostFxFilterMode();
    DispatchPostFxFilterRender(0);
    wazamakiSavedPostFxBank = static_cast<std::int32_t>(GetPostFxBankValue(1));
    SelectPostFxBank(1, 0);
    return true;
}

bool ExecuteLegacyWazamaki() {

    switch (wazamakiPhase) {
    case 0:
        if (InitializeLegacyWazamaki()) {
            SetupModernMovesCamera();
            wazamakiPhase = 1;
            ExecuteAicaCommand(0xabU, 0, 0);
            PrimeAicaEmulationForPause(1);
            wazamakiAudio = shenPlayAudioWithFlags("WAZAMAKIMONO", 17);
            const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            Character *const ryo = static_cast<Character *>(control->ryo);
            if (ryo) {
                MotionWork *const motion = FindMotionWork(ryo);
                if (motion)
                    mtwkRequestMotion(motion, 0xf002, MotionRegion::LowerBody);
            }
            if (g_loadingFlag & 0x20U) {
                wazamakiTicks |= 0x200U;
                FinishLoading();
            }
        } else {
            wazamakiPhase = 2;
        }
        break;
    case 1:
        if (wazamakiTicks & 0x100U) {
            RefreshWazamakiDescriptionAndMotion();
            wazamakiTicks &= static_cast<std::uint16_t>(~0x100U);
        }
        UpdateLegacyWazamakiInput();
        UpdateWazamakiPreviewFrame();
        DrawLegacyWazamaki();
        if (!cvsHasValidConverseData(wazamakiAudio) && !(g_loadingFlag & 2U))
            wazamakiAudio = shenPlayAudioWithFlags("WAZAMAKIMONO", 17);
        break;
    case 2:
        ReleaseLegacyWazamaki();
        wazamakiPhase = 0;
        return true;
    case 3:
        DrawLegacyWazamaki();
        FlushQueuedLegacyDrawPasses();
        FlushQueuedStripType3Batches();
        RestoreWazamakiCamera();
        ReleaseLegacyWazamaki();
        scenCvsCtrlFree(nullptr);
        PrimeAicaEmulationForPause(0);
        wazamakiPhase = 0;
        if (wazamakiTicks & 0x200U)
            WazamakiLoadingCompleteNullsub();
        return true;
    default:
        break;
    }
    ++wazamakiFrameCount;
    if (wazamakiTicks & 0xffU)
        --wazamakiTicks;
    wazamakiAlpha = static_cast<float>(static_cast<double>(wazamakiAlpha) + 0.25);
    if (wazamakiAlpha > 1.0f)
        wazamakiAlpha = 1.0f;
    return false;
}

void SetupModernMovesCamera() {

    wazamakiSavedCameraState = camCtrlGetState();
    camCtrlGetNextCamPos(&wazamakiSavedCameraEye);
    camCtrlGetTargetPosition(&wazamakiSavedCameraTarget);
    camCtrlSetState(CameraState::Event);
    wazamakiSavedPerspective = camCtrlGetPerspective();
    camCtrlSetPerspective(10922);
    camCtrlSetNextRoll(0);
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const Character *const ryo = static_cast<const Character *>(control->ryo);
    const std::int16_t yaw = static_cast<std::int16_t>(-shenGetRotYFromNode(ryo));
    Vector3 position = GetCharacterWorldPosition(ryo);
    position.y += 1.6f;
    camCtrlSetNextPosition(position);
    position.x += sin_fixed16(yaw) * 0.17f;
    position.z += sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw)))) * -0.17f;
    camCtrlSetTargetPosition(position);
}




bool ExecuteModernWazamaki() {
    switch (wazamakiPhase) {
    case 0: {
        SetupModernMovesCamera();
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Character *const ryo = static_cast<Character *>(control->ryo);
        EV_SetCharDisplay(ryo, 3, 0);
        charaSetVisibilityFlagTo(ryo, 0);
        if (ryo) {
            MotionWork *const motion = FindMotionWork(ryo);
            if (motion)
                mtwkRequestMotion(motion, 0xf002, MotionRegion::LowerBody);
        }
        ShowElementByIndex(UIElement::SubMenuMoves);
        ShowElementByIndex(UIElement::GameBackground);
        wazamakiPhase = 1;
        return false;
    }
    case 1:

        if ((g_inputBuffer[0].pressedPriority & 0x0200U) != 0)
            wazamakiPhase = 3;
        return false;
    case 3:
        DestroyElementByIndex(UIElement::SubMenuMoves);
        DestroyElementByIndex(UIElement::GameBackground);
        wazamakiPhase = 0;
        return true;
    default:
        return false;
    }
}

void WazamakiTaskCallback(void *) {
    if (!UpdateWazamakiMenu() || !wazamakiTask)
        return;
    HLib::DestroyTaskInternal(wazamakiTask);
    wazamakiTask = nullptr;
    if (wazamakiSavedPlayerControl)
        SetPlayerControlEnabled(1);
}

} // namespace
bool UpdateWazamakiMenu() {
    return GetDebugForceUIEnabled(UIElement::SubMenuMoves) ? ExecuteModernWazamaki() : ExecuteLegacyWazamaki();
}

void CancelWazamakiMenu(std::int32_t) {
    if (wazamakiPhase != 1)
        return;
    DrawLegacyWazamaki();
    ReleaseLegacyWazamaki();
    scenCvsCtrlFree(nullptr);
    PrimeAicaEmulationForPause(0);
    wazamakiPhase = 0;
    if (wazamakiTicks & 0x200U)
        WazamakiLoadingCompleteNullsub();
}

void RefreshWazamakiLearnedGroups() {
    for (std::uint8_t category = 1; category <= 3; ++category) {
        for (NativeWazamakiGroup &group : NativeWazamakiGroups(category)) {
            group.count = 0;
            group.selected = 0;
        }
    }
    wazamakiGroupCounts = {};
    const std::array<std::uint32_t, 4> flags = ReadSkillFlags();
    const std::span<const S2MoveSkillBank, 3> banks = S2Save_GetMoveSkillBanks();
    for (std::uint8_t category = 1; category <= 3; ++category) {
        const std::span<const MoveDefinition> definitions = MoveDefinitions(category);
        const std::span<NativeWazamakiGroup> groups = NativeWazamakiGroups(category);
        for (std::uint8_t id = 0; id < definitions.size(); ++id) {
            NativeWazamakiGroup &group = groups[definitions[id].group];
            if (banks[category - 1U].proficiency[id] <= 0)
                continue;
            group.moves[group.count] = id;
            if (SkillFlagSet(flags, definitions[id].skillFlag))
                group.selected = group.count;
            if (group.count == 0)
                ++wazamakiGroupCounts[category];
            ++group.count;
        }
    }
    wazamakiGroupCounts[0] = static_cast<std::uint8_t>(wazamakiGroupCounts[1] + wazamakiGroupCounts[2] + wazamakiGroupCounts[3]);
    CacheWazamakiGroups();
}

std::span<const WazamakiLearnedGroup> GetWazamakiLearnedGroups() noexcept {
    CacheWazamakiGroups();
    return {wazamakiLearnedView.data(), wazamakiGroupCounts[0]};
}

void PlayWazamakiGroupMove(std::uint32_t row) {
    if (row >= wazamakiGroupCounts[0])
        return;
    CacheWazamakiGroups();
    const WazamakiLearnedGroup &group = wazamakiLearnedView[row];
    RequestWazamakiPreviewMotion(wazamakiPreviewCharacter, group.category, group.moves[group.selected]);
}

void ResetMoveLearningPresentationState() noexcept {
    g_moveLearnedTask = nullptr;
    g_moveLearnedPendingMove = 0;
    g_moveLearnedPendingCategory = 0;
    moveLearningResetWord = 0;
    g_moveUiClockAndNavigationHold = 0;
    g_extraTelopTask = nullptr;
    g_extraTelopDisplayedText = nullptr;
}

std::int32_t SetNewEwzoTypeForMoveLearned(std::int8_t mode, std::int16_t category, std::int16_t moveIndex) noexcept {
    std::uint8_t operation = static_cast<std::uint8_t>(mode);
    if (mode < 0) {
        category = g_moveLearnedPendingCategory;
        moveIndex = g_moveLearnedPendingMove;
        operation &= 0x7fU;
        g_moveLearnedPendingCategory = 0;
        g_moveLearnedPendingMove = 0;
    }
    if (operation & 1U) {
        if (g_moveLearnedTask) {
            for (std::int16_t surface : g_moveLearnedTextSurfaces)
                if (surface != -1)
                    ReleaseNamedTexture(surface);
            DestroyElementByIndex(UIElement::MoveLearned);
            HLib::DestroyTaskInternal(g_moveLearnedTask);
            g_moveLearnedTask = nullptr;
            SetHelpIconMenuFlags(g_moveLearnedSavedHelpFlags);
        }
        return 1;
    }
    IncrementSteamStat(19);
    if (g_moveLearnedTask || category == 0)
        return 1;
    g_moveLearnedTask = HLib::EnqueueTaskWithoutParameter(MoveLearnedOwner_NullCallback, 15, 0, static_cast<std::int32_t>(MOVE_LEARNED_TASK.name));
    ShowElementByIndex(UIElement::MoveLearned);
    const std::string_view categoryToken = category == 3 && moveIndex == 21 ? "UIMOVESPEC" : MOVE_LEARNED_CATEGORY_TOKENS[category]; // @text EN: "Special move" | JA: "特殊技"
    const char *text = GetLocalizedSceneText(categoryToken).data();
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_moveLearnedTextSurfaces[0] = static_cast<std::int16_t>(SetupUIElement(control->windowTask, text, 256, -1, -20480, 0));
    SetMoveLearnedUiLine(0, categoryToken);
    const std::string_view moveToken = MOVE_NAME_TOKENS[category][moveIndex];
    char moveName[24];
    std::sprintf(moveName, "%s", moveToken.data());
    SetMoveLearnedUiLine(1, moveToken);
    text = GetLocalizedSceneText(moveName).data();
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_moveLearnedTextSurfaces[1] = static_cast<std::int16_t>(SetupUIElement(control->windowTask, text, 512, -1, -20480, 0));
    const std::string_view resultToken = operation == 2 ? "UIMOVETAUG" : "UIMOVEMAST"; // @text UIMOVETAUG EN: "has been taught." | JA: "を修得しました。" ; UIMOVEMAST EN: "has been mastered." | JA: "を修得しました。"
    SetMoveLearnedUiLine(2, resultToken);
    text = GetLocalizedSceneText(resultToken).data();
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_moveLearnedTextSurfaces[2] = static_cast<std::int16_t>(SetupUIElement(control->windowTask, text, 512, -1, -20480, 0));
    if (g_moveLearnedTextSurfaces[0] != -1 && g_moveLearnedTextSurfaces[1] != -1 && g_moveLearnedTextSurfaces[2] != -1) {
        g_moveLearnedSavedHelpFlags = static_cast<std::uint16_t>(GetUiFlag(0));
        if (operation == 2) {
            ExecuteAicaCommand(0xaabU, 0, 0);
            g_moveLearnedSoundDelay = -1;
        } else {
            ExecuteAicaCommand(0x208abU, 0, 0);
            g_moveLearnedSoundDelay = 18;
        }
        return 1;
    }
    for (std::int16_t surface : g_moveLearnedTextSurfaces)
        if (surface != -1)
            ReleaseNamedTexture(surface);
    HLib::DestroyTaskInternal(g_moveLearnedTask);
    g_moveLearnedTask = nullptr;
    return 0;
}

void ReleaseMoveLearnedOwner() noexcept {
    (void)SetNewEwzoTypeForMoveLearned(1, 0, 0);
}

bool IsMoveLearnedActive() noexcept {
    return g_moveLearnedTask != nullptr;
}
std::span<const WazamakiMoveInfo> GetWazamakiMoveCategory(std::uint8_t category) noexcept {
    const std::span<const MoveDefinition> definitions = MoveDefinitions(category);
    std::span<WazamakiMoveInfo> cache = MoveInfoCache(category);
    if (definitions.empty() || cache.size() != definitions.size())
        return {};
    const auto banks = S2Save_GetMoveSkillBanks();
    const S2MoveSkillBank &bank = banks[category - 1U];
    const auto flags = ReadSkillFlags();
    for (std::size_t index = 0; index < definitions.size(); ++index) {
        const MoveDefinition &definition = definitions[index];
        const std::array<std::uint8_t, 7> &commands = category == 1 ? MOVE_INPUT_COMMANDS_HAND[index] :
            category == 2                                           ? MOVE_INPUT_COMMANDS_LEG[index] :
                                                                      MOVE_INPUT_COMMANDS_THROW[index];
        cache[index] = {.category = category,
            .id = static_cast<std::uint8_t>(index),
            .group = definition.group,
            .stats = definition.stats,
            .proficiency = bank.proficiency[index],
            .usage = bank.usage[index],
            .motionId = definition.motionId,
            .textToken = definition.textToken,
            .equipped = SkillFlagSet(flags, definition.skillFlag),
            .inputCommands = commands,
            .critical = (MOVE_CRITICAL_MASKS[category - 1U] & (1U << index)) != 0};
    }
    return cache;
}

WazamakiSelection GetWazamakiSelection() noexcept {
    return {.category = wazamakiPresentationCategory, .row = static_cast<std::uint8_t>(wazamakiSelectedRow), .variant = wazamakiPresentationVariant};
}

bool SetWazamakiSelection(std::uint8_t category, std::uint8_t row) noexcept {
    if (row >= MoveDefinitions(category).size())
        return false;
    wazamakiPresentationCategory = category;
    wazamakiSelectedRow = row;
    wazamakiPresentationVariant = 0;
    if (wazamakiTask && !wazamakiLegacyState)
        RequestWazamakiPreviewMotion(wazamakiPreviewCharacter, category, row);
    return true;
}

std::int64_t SetMoveEquipment(std::int8_t operation, std::int32_t category, std::uint8_t move) noexcept {
    std::int64_t result = static_cast<std::uint32_t>(category - 1);
    if (static_cast<std::uint8_t>(category - 1) > 2U || move >= 40U)
        return result;
    const std::span<const MoveDefinition> definitions = MoveDefinitions(static_cast<std::uint8_t>(category));
    const MoveDefinition &selected = definitions[move];
    std::array<std::uint32_t, 4> flags = ReadSkillFlags();
    const std::uint32_t bank = selected.skillFlag >> 30U;
    const std::uint32_t mask = selected.skillFlag & 0x3fffffffU;
    bool replace = false;
    result = 3LL * move;
    if (operation == 0) {
        flags[bank] |= mask;
        result = bank;
    } else if (operation == 1) {
        flags[bank] &= ~mask;
        replace = true;
        result = bank;
    }
    for (std::uint8_t index = 0; index < definitions.size(); ++index) {
        const MoveDefinition &candidate = definitions[index];
        if (candidate.group == selected.group && index != move) {
            const std::uint32_t otherBank = candidate.skillFlag >> 30U;
            const std::uint32_t otherMask = candidate.skillFlag & 0x3fffffffU;
            if (replace) {
                flags[otherBank] |= otherMask;
                replace = false;
            } else
                flags[otherBank] &= ~otherMask;
        }
        result = index + 1U;
    }
    std::array<std::byte, 16> encoded{};
    std::memcpy(encoded.data(), flags.data(), sizeof(flags));
    S2Save_SetSkillFlags(encoded);
    return result;
}

bool SelectWazamakiMove(std::uint8_t category, std::uint8_t id) noexcept {
    const std::span<const MoveDefinition> definitions = MoveDefinitions(category);
    if (id >= definitions.size())
        return false;
    const MoveDefinition &selected = definitions[id];
    SetMoveEquipment(0, category, id);
    NativeWazamakiGroup &group = NativeWazamakiGroups(category)[selected.group];
    for (std::uint8_t variant = 0; variant < group.count; ++variant) {
        if (group.moves[variant] == id) {
            group.selected = variant;
            wazamakiPresentationVariant = variant;
            break;
        }
    }
    for (std::int32_t row = 0; row < wazamakiGroupCounts[0]; ++row) {
        std::int32_t groupCategory;
        if (FindWazamakiGroup(row, &groupCategory) == &group) {
            wazamakiSelectedRow = row;
            break;
        }
    }
    CacheWazamakiGroups();
    wazamakiPresentationCategory = category;
    return true;
}

void QueueWazamakiTask() {
    if (wazamakiTask)
        return;
    wazamakiTask = HLib::EnqueueTaskWithoutParameter(WazamakiTaskCallback, 15, 0, static_cast<std::int32_t>(WAZAMAKI_TASK.name));
    wazamakiSavedPlayerControl = plymIsSearchStartRequested();
    if (wazamakiSavedPlayerControl)
        SetPlayerControlEnabled(0);
    (void)UpdateWazamakiMenu();
}

std::uint32_t IsWazamakiInactive() noexcept {


    return wazamakiLegacyState;
}

// @verified - p
void SetupWZ2_CtrlWazamaki() noexcept {
    if (wazamakiTask && wazamakiPhase == 1)
        wazamakiPhase = 3;
}

} // namespace shenmue::s2
