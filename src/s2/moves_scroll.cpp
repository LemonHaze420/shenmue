#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/moves_scroll.hpp"
#include "s2/event_matrix_dc.hpp"

#include "s2/hlib.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/pause.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/save_load_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/window.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_roll_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/lcdm_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "common/math.hpp"
#include <stdio.h>
#include <string.h>
#include "s2/ui_state_dc.hpp"
#define UI_ZERO
#define UI_ICON(name) name
#else
#include "s2/moves_scroll.hpp"

#include "s2/ui_scroll_list.hpp"

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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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
    {0x80000001U, 0x00000013U, 0, {9, 4, 6},
        "UIWAZAHH00"}, // @text EN: "A punch aimed at a point between the\nnose and mouth." | JA: "鼻と口の間にあるツボ、「人中」を狙う技である。"
    {0x40000040U, 0x0000006aU, 1, {14, 6, 12},
        "UIWAZAHH01"}, // @text EN: "A downward rotating blow aimed at\nthe opponent's collarbone." | JA: "腕を縦方向の回転運動で打ち下ろし、\n 相手の鎖骨を狙う技である。"
    {0x40000080U, 0x00000014U, 1, {12, 5, 12},
        "UIWAZAHH02"}, // @text EN: "A straight punch aimed at a pressure\npoint on the opponent's side neck." | JA: "ストレートパンチを相手の頸部に打ち出す技。"
    {0x44000000U, 0x00000017U, 2, {15, 4, 15},
        "UIWAZAHH03"}, // @text EN: "Unlike the Elbow Slam, it is thrown\nhorizontally in a sweeping hook." | JA: "小車打ちとは異なり、水平方向に振る、\n大振りのいわゆるフックである。"
    {0x48000000U, 0x00000015U, 2, {18, 10, 15},
        "UIWAZAHH04"}, // @text EN: "A strike on the jaw swung by\nmomentarily lowering and rising." | JA: "いったん体を下に沈め、伸び上がりながら\n下から相手の顎を振り打つ技。"
    {0x80000002U, 0x0000011bU, 3, {40, 5, 18},
        "UIWAZAHH05"}, // @text EN: "A move targeted at the center of\nthe opponent's body with a rush." | JA: "体当たりに近い感覚で行う肘打ち。\n相手の身体の正中線を狙う。"
    {0x40000200U, 0x0000004dU, 5, {23, 13, 17},
        "UIWAZAHH06"}, // @text EN: "A slashing blow with the side of the\npalm with an advancing step." | JA: "前足を一歩進め、手刀の部分で\n相手に斬りつける技である。"
    {0x40000100U, 0x0000001bU, 6, {27, 10, 18},
        "UIWAZAHH07"}, // @text EN: "A punch at a mid-level target while\ntaking a step forward." | JA: "後ろ足を前に一歩進めて、\n中段を正拳で突く技である。"
    {0x40000400U, 0x0000001aU, 7, {26, 10, 16},
        "UIWAZAHH08"}, // @text EN: "A diagonal blow on the opponent's\ncollarbone while closing in." | JA: "一気に間合いを詰め、鎖骨近辺を狙って、\n斜めに振り打たれる。"
    {0x40000800U, 0x00000148U, 8, {25, 7, 35},
        "UIWAZAHH09"}, // @text EN: "A strike on the jaw and chest by\nusing the palm and forearm." | JA: "手のひらや腕全体を使って\n相手の胸や顎を攻撃する技である。"
    {0x40002000U, 0x00000185U, 9, {12, 8, 15},
        "UIWAZAHH10"}, // @text EN: "A powerful surprise attack with\na spinning backfist." | JA: "回転して裏拳を放つ強力な奇襲技。"
    {0x40001000U, 0x0000006dU, 10, {17, 12, 13},
        "UIWAZAHH11"}, // @text EN: "A strike on the pit of the stomach\nby the elbow bent at an acute angle." | JA: "相手の攻撃をかわし、回転の勢いを利用して\n鋭角に曲げた肘で相手の水月を突く技である。"
    {0x42000000U, 0x00000037U, 14, {15, 8, 17},
        "UIWAZAHH12"}, // @text EN: "A diagonal blow to the area between\nthe shoulder and a nape of the neck." | JA: "走るスピードを利用し、敵の肩口から\n首筋にかけて、肘打ちを斜めに放つ技である。"
    {0x00000040U, 0x000000cdU, 4, {30, 10, 15},
        "UIWAZAHH13"}, // @text EN: "A blow to the opponent's chin thrust\nby lowering the body and springing." | JA: "いったん体勢を低くしたあと\n腰のバネを活かし、相手のアゴを突き上げる技。"
    {0x00000002U, 0x004c004bU, 5, {42, 6, 16},
        "UIWAZAHH14"}, // @text EN: "A slashing blow thrusted by\nswinging down the arm and up again." | JA: "腕を大きく振りおろしたのちに\n勢いを利用し振り上げる二連撃の技。"
    {0x00800000U, 0x00bc00bfU, 6, {45, 6, 12},
        "UIWAZAHH15"}, // @text EN: "A mid-level strike by a second\npunch on the same target." | JA: "中段突きで同じ場所を再度攻撃する技。"
    {0x00000020U, 0x00000019U, 10, {32, 11, 20},
        "UIWAZAHH16"}, // @text EN: "A penetrating blow with the palms\nusing power from the back twist." | JA: "裏回転で生じた力を手刀で相手に叩きこむ。"
    {0x04000000U, 0x01950193U, 11, {30, 9, 31},
        "UIWAZAHH17"}, // @text EN: "A blow by one hand on the opponent\nand slamming it with the other." | JA: "相手に密着させた掌をもう一方の掌で\n激しく叩く事で威力を浸透させる荒業。"
    {0x40400000U, 0x00d200d4U, 16, {25, 19, 30},
        "UIWAZAHH18"}, // @text EN: "Deflecting the opponent's attack and\nkicking his leg at the same time." | JA: "燕青拳の技で、攻撃を受け流すと同時に\n相手の足を蹴って倒す、高度な技である。"
    {0x00100000U, 0x00000275U, 1, {15, 6, 11},
        "UIWAZAHH19"}, // @text EN: "A thrust move delivered by stepping\nout the same side of foot and arm." | JA: "踏み込む足と同じ方の手を突き出す技。\n中国拳法では基本的な突き技。"
    {0x00000001U, 0x00000038U, 4, {50, 28, 15},
        "UIWAZAHH20"}, // @text EN: "A big step backward and then a\nforward create a powerful uppercut." | JA: "後ろに大きく下がって身を縮め、前進するとともに\n全力でアッパーを放つ豪快な技。"
    {0x00040000U, 0x00000001U, 7, {25, 9, 23},
        "UIWAZAHH21"}, // @text EN: "A Tai Chi hand move with force\ndelivered from the legs and waist." | JA: "太極拳の技の一つで、足腰の捻で発せられた力を\n掌に伝えて打ちこむ。"
    {0x00000400U, 0x00000149U, 8, {35, 14, 20},
        "UIWAZAHH22"}, // @text EN: "A basic Baji Style move. The\ntwisting of the waist exerts force." | JA: "八極拳の基本技で最重要の技。\n腰の回転を利用して突きを放つ。"
    {0x00200000U, 0x00000289U, 8, {25, 8, 16},
        "UIWAZAHH23"}, // @text EN: "While closing in, both palms are\nused to strike the opponent's chest." | JA: "一気に間合いを詰め、両手の掌を\n相手の胸に叩きつける技。"
    {0x00020000U, 0x00000119U, 9, {15, 14, 22},
        "UIWAZAHH24"}, // @text EN: "The Elbow Attack thrusted on the\nside while avoiding the attack." | JA: "八極拳六大開の一つ。相手の攻撃をさばき、\n体制を崩し、側面から頂肘を打つ大技。"
    {0x02000000U, 0x0000027bU, 11, {15, 13, 15},
        "UIWAZAHH25"}, // @text EN: "An upper blow on the chin and a\nconsecutive downward strike." | JA: "鈎手（こうしゅ）という手型で相手のアゴを跳ね上げ、\n隙をついて掌を落とす。"
    {0x00000800U, 0x00000276U, 17, {15, 10, 17},
        "UIWAZAHH26"}, // @text EN: "Striking the collarbone or the head\nwhile moving to the opponent's side." | JA: "相手の側面に回り込むと同時に、\n上から握った拳を鎖骨や頭に叩きつける技。"
    {0x00400000U, 0x0000028aU, 18, {5, 10, 20},
        "UIWAZAHH27"}, // @text EN: "A mid-level strike on the stomack\nwhile leaning toward the opponent." | JA: "その場から身を乗り出すようにして、\n相手の腹部へ中段突きを放つ技。"
}};

constexpr std::array<MoveDefinition, 23> moveDefinitions2{{
    {0x40010000U, 0x000000c8U, 0, {18, 6, 10},
        "UIWAZALH00"}, // @text EN: "An upward sword-stroke kick, aimed\nat the opponent's head." | JA: "下から太刀（たち）を摺り上げるように\n繰り出される蹴り技。相手の頭部を狙う。"
    {0x40020000U, 0x000000c4U, 0, {20, 8, 10},
        "UIWAZALH01"}, // @text EN: "A front kick aiming the front torso\nof the opponent with the toes." | JA: "いわゆる前蹴り。主に爪先で\n人体の正面にある急所を狙う。"
    {0x40040000U, 0x00000010U, 1, {18, 6, 14},
        "UIWAZALH02"}, // @text EN: "A low to mid-level kick using the\npower of the forward leg." | JA: "体を前方に大きく移動させながら\n足刀で相手の下段から中段を蹴る技。"
    {0x40080000U, 0x0000000fU, 2, {18, 5, 20},
        "UIWAZALH03"}, // @text EN: "A low, decisive kick aimed at the\noutside of the opponent's knee." | JA: "相手の膝の外側を狙い、小さく、かつ鋭く\n放たれるローキックの一種。"
    {0x80000800U, 0x0297029aU, 3, {40, 7, 25},
        "UIWAZALH04"}, // @text EN: "A heel kick followed by another\nusing the other leg." | JA: "カカトで相手を蹴り上げた後、すかさず\nもう一方の足で蹴りを放つ二段蹴り。"
    {0x40800000U, 0x00b200b3U, 5, {32, 6, 12},
        "UIWAZALH05"}, // @text EN: "A middle rotating kick followed by\na heel drop." | JA: "中段の回し蹴りから変化して、\nカカト落しを放つ技。軌道はきわめて小さい。"
    {0x41000000U, 0x000000abU, 5, {16, 11, 14},
        "UIWAZALH06"}, // @text EN: "A back roundhouse kick striking\nin a downward diagonal motion." | JA: "後ろ回し蹴りの一種で、相手の上段から\n斜め下方向に振り下ろされる技。"
    {0x40100000U, 0x000000afU, 6, {27, 9, 15},
        "UIWAZALH07"}, // @text EN: "A flying knee move made by\nleaping upward to kick." | JA: "一瞬体を縮めた後、伸び上がるように蹴る、\n一種の飛び膝蹴り。"
    {0x40008000U, 0x000000a1U, 12, {32, 7, 19},
        "UIWAZALH08"}, // @text EN: "A powerful high-speed reverse\nroundhouse kick." | JA: "高速の反転後ろ回し蹴り。体を大きく倒す事で\n遠心力と威力を増加させている。"
    {0x40004000U, 0x0000009eU, 13, {25, 14, 16},
        "UIWAZALH09"}, // @text EN: "A kick delivered while flipping\nforward in mid-air." | JA: "前方に空中回転をしながら蹴りを放つ技。\n芭月流の中でも特異なわざと言われている。"
    {0x60000000U, 0x000000a0U, 14, {36, 14, 13},
        "UIWAZALH10"}, // @text EN: "A flying back roundhouse kick with\nforce by jump and rotation." | JA: "跳び後ろ回し蹴り。\n回転と跳躍の力が加わるため、威力が高い。"
    {0x50000000U, 0x000000aeU, 15, {22, 13, 20},
        "UIWAZALH11"}, // @text EN: "A flying back kick, aiming\ndiagonally at the opponent's neck." | JA: "飛び後ろ蹴り。相手の首筋を\n袈裟（けさ）切りに蹴る大技である。"
    {0x40200000U, 0x000000aaU, 2, {28, 8, 11},
        "UIWAZALH12"}, // @text EN: "After luring the opponent in,\na reaping kick is delivered." | JA: "攻撃を受け流すと同時に\n相手の足を刈って倒す技。"
    {0x01000000U, 0x00360035U, 3, {40, 4, 17},
        "UIWAZALH13"}, // @text EN: "Using the spinning momentum of the\n1st kick, a 2nd kick is delivered." | JA: "空中で回し蹴りを出したあと、遠心力を活かし、\nもう１回転して蹴りを放つ荒技。"
    {0x00000008U, 0x000000b9U, 4, {23, 11, 26},
        "UIWAZALH14"}, // @text EN: "A kick on the chin by sliding toward\nthe opponent and making a handstand." | JA: "相手に向かって少しすべり込み、\n逆立ちしながら相手のアゴ先を蹴る奇襲的な技。"
    {0x00000010U, 0x000000bbU, 4, {22, 7, 19},
        "UIWAZALH15"}, // @text EN: "Sweeping out the opponent's leg\nwhile spinning on the pivot leg." | JA: "軸足を中心に急旋回して、もう一方の足で\n相手の足元を払う技である。"
    {0x00000004U, 0x00000133U, 6, {18, 11, 17},
        "UIWAZALH16"}, // @text EN: "A Hook Kick by the heel, aiming\nat the opponent's neck." | JA: "相手の首を根こそぎ刈り取るようにカカトで蹴る豪快な技。\n頭の急所を狙い放つ。"
    {0x00001000U, 0x00a700a6U, 13, {15, 13, 11},
        "UIWAZALH17"}, // @text EN: "A heel drop kick made\nafter a spinning kick." | JA: "回し蹴りを放った後に、もう一方の足で\nすかさず回転カカト落としを放つ迅速な技。  "
    {0x00000200U, 0x00000124U, 16, {20, 15, 17},
        "UIWAZALH18"}, // @text EN: "Sliding the ground forward to sweep\nthe feet out." | JA: "走っている最中にいきなり、身体を回転させて\n滑り込み、相手の足元を払う技。"
    {0x10000000U, 0x00000279U, 12, {16, 7, 13},
        "UIWAZALH19"}, // @text EN: "A kick delivered on the chin while\nturning the back on the opponent." | JA: "急に相手に背中を向けた後、真下から\nアゴに向かってカカトを蹴り上げる技。"
    {0x08000000U, 0x00000285U, 4, {20, 7, 15},
        "UIWAZALH20"}, // @text EN: "A heel kick by lowering the body\nsuddenly in front of the opponent." | JA: "相手の目の前で急激に身を低くして、\n相手の足元をカカトで蹴る奇襲技。"
    {0x40000001U, 0x00000286U, 14, {36, 8, 24},
        "UIWAZALH21"}, // @text EN: "A diagonal upward kick after\nrunning and jumping to the opponent." | JA: "相手に走り込み、目の前で跳躍し\n斜め下から急激に蹴りを突き上げる技。  "
    {0x40000020U, 0x0260027dU, 15, {20, 10, 10},
        "UIWAZALH22"}, // @text EN: "After a low, elbow strike, an upward\nkick is delivered with a handstand." | JA: "相手の前で身を低くし回転肘打ち、\nすかざず逆立ち蹴りを放つ高度な蹴り技。"
}};

constexpr std::array<MoveDefinition, 22> moveDefinitions3{{
    {0x80000004U, 0x000000f0U, 0, {30, 1, 16},
        "UIWAZATH00"}, // @text EN: "The opponent is lifted over the back\nand is thrown over." | JA: "柔術でも古典的な技の一つ。\n相手の腕を取り、懐に入り込んで投げる。"
    {0x80000008U, 0x00000103U, 1, {45, 1, 27},
        "UIWAZATH01"}, // @text EN: "The opponent's inner thigh is\nswept up to throw." | JA: "柔術でも古典的な技の一つ。足を使って\n相手の内股の部分を払い上げて投げる。"
    {0x80000010U, 0x000000dfU, 2, {39, 1, 40},
        "UIWAZATH02"}, // @text EN: "The practitioner falls backward to\nthrow the opponent overhead to rear." | JA: "柔術でも古典的な技の一つ。\n自分が後方に倒れながら、相手を後ろに投げる。"
    {0x80000020U, 0x000000deU, 3, {52, 1, 38},
        "UIWAZATH03"}, // @text EN: "Grabbing the opponent's neck and\nsweeping the opponent's pivot leg." | JA: "相手の首に腕を巻きつけ、\n体重のかかっている足を刈る投げ技。"
    {0x80000040U, 0x000000e5U, 4, {58, 1, 30},
        "UIWAZATH04"}, // @text EN: "While throwing the opponent, the\npractitioner flips and lands on him." | JA: "相手を投げると同時に、自らも回転し\n相手の上に身体を落とす大技。"
    {0x80000080U, 0x00000105U, 5, {52, 1, 1},
        "UIWAZATH05"}, // @text EN: "Grasping the arm and executing\nan armlock to pin." | JA: "相手の腕を取り逆関節を決めて押さえつける技。\n古流柔術では基本的な技である。"
    {0x80000100U, 0x000000d0U, 6, {45, 1, 13},
        "UIWAZATH06"}, // @text EN: "Lifting the opponent on the shoulder\nand dropping him headfirst." | JA: "身を低くしながら相手の足を取り、肩に担ぎあげ、\n地面に叩き落とす豪快な技。"
    {0x80000200U, 0x000000cbU, 7, {52, 1, 21},
        "UIWAZATH07"}, // @text EN: "Closing in on the opponent's side\nto throw down while spinning." | JA: "相手の側面から接近し、相手を半回転させながら\n投げ落とす技。"
    {0x80000400U, 0x000000c9U, 8, {71, 1, 16},
        "UIWAZATH08"}, // @text EN: "The throw is executed with\na twist toward the rear." | JA: "柔術でも古典的な技の一つ。後方にひねり落すように\n相手を投げる。"
    {0x00000100U, 0x000000ebU, 12, {0, 1, 26},
        "UIWAZATH09"}, // @text EN: "Evading the opponent and moving\nback into his blind spot to attack." | JA: "相手の攻撃をかわしつつ、背後に回り込み\n敵の死角をつく技。"
    {0x00002000U, 0x000000f1U, 9, {19, 1, 14},
        "UIWAZATH10"}, // @text EN: "A complex move damaging the\nopponent's arms in multiple areas." | JA: "相手の腕関節に数ヶ所同時にダメージを与える、\n複雑な高等技である。"
    {0x00000080U, 0x00da00dcU, 10, {70, 1, 34},
        "UIWAZATH11"}, // @text EN: "Crouching momentarily and lunging\nheadfirst into the opponent." | JA: "一瞬、身体を低くして力をため、瞬発力をもって、\n相手に強烈な頭突きをする技。"
    {0x00004000U, 0x00ee00edU, 13, {41, 1, 12},
        "UIWAZATH12"}, // @text EN: "Striking the back of the neck after\nmoving into opponent's blind spot." | JA: "影身の派生技。後方に回ったあと、\n無防備となった相手の首筋に手刀を打ち込む。"
    {0x00008000U, 0x000000d6U, 14, {61, 1, 39},
        "UIWAZATH13"}, // @text EN: "Evading the opponent's attack\ndiagonally to make body attack." | JA: "相手の攻撃をななめにかわすと同時に\n死角から急接近し、体当たりする技。"
    {0x00010000U, 0x00000120U, 11, {52, 1, 2},
        "UIWAZATH14"}, // @text EN: "A strike on the chin delivered while\nmoving to the rear of the opponent." | JA: "相手の後方に足を進めると同時に\n鈎手（こうしゅ）でアゴを打ち倒す太極拳の技。"
    {0x00080000U, 0x0000001eU, 0, {30, 1, 13},
        "UIWAZATH15"}, // @text EN: "An advanced throw move. The\ndirection of the throw is unlimited." | JA: "本来の意味は投げ技。色々な方向に\n好きなように投げ分ける高度な投げ技。"
    {0x40000002U, 0x0000027aU, 1, {58, 1, 24},
        "UIWAZATH16"}, // @text EN: "While reaping both legs of the\nopponent, the torso is thrusted." | JA: "蟷螂拳の技で相手の両足を刈ると同時に\n上半身を手で払う投げ技。"
    {0x40000004U, 0x00000273U, 3, {51, 1, 10},
        "UIWAZATH17"}, // @text EN: "A top and middle-level thrust\nand blow delivered consecutively." | JA: "連続で素早く直突きや裏拳を上段、中段に放つ\n高度な連続技。"
    {0x40000008U, 0x00000287U, 5, {40, 1, 12},
        "UIWAZATH18"}, // @text EN: "A strike on the side followed by a\nthrow after a quick move." | JA: "相手の側面から掌を打ち、すぐに転身して\n反対の側面に入り投げ倒す技。"
    {0x20000000U, 0x0000028eU, 9, {15, 1, 10},
        "UIWAZATH19"}, // @text EN: "A strike on the head by making the\nopponent fall as jumping on to him." | JA: "素早く相手の肩口にのぼり、\n相手を転ばせると同時に頭部を強打する投げ技。"
    {0x80001000U, 0x0000029dU, 15, {90, 1, 5},
        "UIWAZATH20"}, // @text EN: "Three rapid strikes on the eyes\nand the throat of the opponent." | JA: "八極拳八大招式の一つ。目や喉を\n素早く３回突く技で、恐ろしい殺傷力を持つ。"
    {0x40000010U, 0x00000132U, 16, {0, 0, 0},
        "UIWAZATH21"}, // @text EN: "A technique to feel the opponent's\nmove using all senses." | JA: "目だけでなく、全身を使って\n相手の気配を感じる高度な技術。"
}};

std::array<WazamakiMoveInfo, 28> moveInfo1{};

std::array<WazamakiMoveInfo, 23> moveInfo2{};

std::array<WazamakiMoveInfo, 22> moveInfo3{};

std::array<WazamakiLearnedGroup, 73> wazamakiLearnedView{};

std::uint8_t wazamakiPresentationCategory{1}, wazamakiPresentationVariant{};

CameraState wazamakiSavedCameraState;

Vector3 wazamakiSavedCameraEye{}, wazamakiSavedCameraTarget{};

std::uint16_t wazamakiSavedPerspective{};

std::uint32_t wazamakiSavedClockSuppressed{}, wazamakiSavedNpcLoadState{}, wazamakiSavedHelpTopic{};

std::uint16_t wazamakiSavedHelpFlags;

std::int32_t wazamakiSavedPostFxFilter{}, wazamakiSavedPostFxBank{};

Character *wazamakiPreviewCharacter;

std::int32_t wazamakiPreviewYaw, wazamakiPreviewMotionYaw{}, wazamakiPreviewPitch{}, wazamakiPreviewRoll{}, wazamakiPreviewBaseYaw{};

Vector3 wazamakiPreviewPosition{};

Matrix4x4 wazamakiPreviewRootTransform{};

std::uint16_t wazamakiPreviewPendingMotion, wazamakiPreviewPreviousAnalog{};

std::int8_t wazamakiPreviewFrameDelay;

float wazamakiPreviewScale;

void *wazamakiRyoFile{};

MT7Model wazamakiRyoModel;

void *wazamakiGroundFile{};

MT7Model wazamakiGroundModel;

std::array<std::int32_t, 8> wazamakiTextures;

static_assert(sizeof(wazamakiTextures) == 0x20);

ConversationBuffer *wazamakiAudio{};

std::uint32_t wazamakiPulseBrightness;

std::int8_t wazamakiPulseStep{};

HLib::Task *wazamakiTask;

std::uint8_t wazamakiPhase;

std::uint32_t wazamakiLegacyState;

std::int32_t wazamakiSavedPlayerControl{};

std::uint16_t wazamakiTicks{};

std::uint32_t wazamakiFrameCount;

float wazamakiAlpha;

std::array<NativeWazamakiGroup, 28> wazamakiHandGroups{};

std::array<NativeWazamakiGroup, 23> wazamakiLegGroups{};

std::array<NativeWazamakiGroup, 22> wazamakiThrowGroups{};

std::array<std::uint8_t, 4> wazamakiGroupCounts;

LegacyIconSprite wazamakiStatisticLabel{};

LegacyColourRect wazamakiStatisticBar;

LegacyIconSprite wazamakiScrollTrackCap{}, wazamakiScrollTrackBody{}, wazamakiScrollThumbCap{}, wazamakiScrollThumbBody{};

LegacyIconSprite wazamakiNameSprite, wazamakiBackgroundSprite{}, wazamakiInputSprite{}, wazamakiCriticalSprite{};

LegacyColourRect wazamakiRowBar;

std::array<std::uint16_t, 3> wazamakiCriticalFrames;

std::int32_t wazamakiSelectedRow, wazamakiFirstVisible{};

std::uint8_t wazamakiInputPhase{};

std::int8_t wazamakiRepeatDelay;

std::uint16_t wazamakiTransitionFrames;

std::uint32_t wazamakiReverseVariant;

std::array<std::array<std::int16_t, 40>, 3> wazamakiNameSurfaces;

std::array<std::array<std::uint16_t, 40>, 3> wazamakiNameLengths;

std::array<std::int16_t, 3> wazamakiStatSurfaces{};

WazamakiDescriptionWindow wazamakiDescription;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
NativeWazamakiGroup *FindWazamakiGroup(std::int32_t row, std::int32_t *category, std::int32_t *groupIndex);
void LoadRyoSceneForMovesScreen();
void RequestWazamakiPreviewMotion(std::uint8_t category, std::uint8_t id);
void UpdateWazamakiPreviewFrame();
void DrawWazamakiPreview();
void ReleaseWazamakiPreviewModels();
bool UpdateWazamakiMenu();
void RefreshWazamakiLearnedGroups();
std::int32_t SetNewEwzoTypeForMoveLearned(std::int8_t mode, std::int16_t category, std::int16_t moveIndex) noexcept;
void QueueWazamakiTask();
std::uint32_t IsWazamakiInactive() noexcept;
void SetupWZ2_CtrlWazamaki() noexcept;

namespace {

void CacheWazamakiGroups();
void MoveLearnedOwner_NullCallback(void *);
bool LoadLegacyWazamakiTextures();
WazamakiLearnedGroup *CurrentLearnedGroup();
void PlayCurrentLegacyMove();
void UpdateLegacyPreviewTransform();
void UpdateWazamakiNameSurfaces(std::int32_t mode);
void InitializeWazamakiDescription(float left, float top, float right, float bottom);
void UpdateWazamakiDescriptionText(const char *text);
void RefreshWazamakiDescriptionAndMotion();
void SelectCurrentLegacyVariant();
void UpdateLegacyWazamakiInput();
void DrawWazamakiStatistics();
void DrawWazamakiButtons();
void DrawWazamakiScrollbar(float x, float top, float bottom, float first, float last);
void DrawWazamakiDescription();
void DrawWazamakiRows();
void DrawLegacyWazamaki();
void WazamakiLoadingCompleteNullsub();
void RestoreWazamakiCamera();
void ReleaseLegacyWazamaki();
bool InitializeLegacyWazamaki();
bool ExecuteLegacyWazamaki();
void SetupModernMovesCamera();
bool ExecuteModernWazamaki();
void WazamakiTaskCallback(void *);

} // namespace
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetupModernMovesCamera();

void UpdateLegacyPreviewTransform();

void PlayCurrentLegacyMove();

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::array<std::uint32_t, 4> ReadSkillFlags() {
    std::array<std::uint32_t, 4> flags{};
    std::memcpy(flags.data(), S2Save_GetPersistentPlayerStats().statFlags.data(), sizeof(flags));
    return flags;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool SkillFlagSet(const std::array<std::uint32_t, 4> &flags, std::uint32_t encoded) {
    return (flags[encoded >> 30U] & encoded & 0x3fffffffU) != 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct NativeMoveName {
    const char *name, *ruby;
    std::int16_t group;
};

struct NativeMoveRecord {
    std::uint32_t skillFlag;
    std::uint16_t motion, alternateMotion;
    std::uint8_t parameters[4];
};

struct NativeWazamakiGroup {
    std::uint8_t count, selected, moves[3];
};

struct NativeWazamakiGroupPosition {
    std::int32_t category, index;
};

struct NativeWazamakiState {
    std::uint8_t phase, inputPhase;
    std::int16_t transitionFrames;
    std::uint32_t frameCount;
    std::uint8_t groupCounts[4];
    std::int32_t savedCameraState;
    Vector3 savedCameraEye, savedCameraTarget;
    std::uint32_t savedHelpTopic;
    std::uint16_t savedHelpFlags;
    std::int32_t savedPerspective, pulseBrightness;
    std::int8_t pulseStep;
    std::uint32_t reverseVariant;
    float alpha;
    std::int8_t ticks;
};
typedef char NativeWazamakiStateSizeCheck[(sizeof(NativeWazamakiState) == 0x48 && offsetof(NativeWazamakiState, savedHelpTopic) == 0x28 &&
                                              offsetof(NativeWazamakiState, reverseVariant) == 0x3c && offsetof(NativeWazamakiState, ticks) == 0x44) ?
        1 :
        -1];

struct NativeWazamakiPreview {
    MT7Model *ryoModel, *groundModel;
    Character *character;
    MT7Node *ryoRoot, *groundRoot;
    Vector3 position;
    Matrix4x4 rootTransform;
    float scale;
    std::int32_t unknown64, pitch, yaw, roll, baseYaw;
    std::uint32_t retained78;
    std::int16_t pendingMotion;
    std::uint8_t previousAnalogX, previousAnalogY;
    std::int8_t frameDelay;
};
typedef char NativeWazamakiPreviewSizeCheck[(sizeof(NativeWazamakiPreview) == 0x84 && offsetof(NativeWazamakiPreview, rootTransform) == 0x20 &&
                                                offsetof(NativeWazamakiPreview, pitch) == 0x68 && offsetof(NativeWazamakiPreview, frameDelay) == 0x80) ?
        1 :
        -1];

void UpdateLegacyWazamakiInput();

void UpdateWazamakiNameSurfaces();

void RefreshWazamakiDescriptionAndMotion(NativeWazamakiDescription *description);

void RequestWazamakiPreviewMotion(std::int32_t category, std::int32_t move);

void UpdateWazamakiPreviewFrame();

void DrawLegacyWazamaki();

bool InitializeLegacyWazamaki();

void ReleaseLegacyWazamaki();

} // namespace

std::int32_t GetMoveProficiency(std::uint8_t category, std::uint8_t move);

std::uint16_t g_moveUiClockAndNavigationHold = 0;

std::uint32_t wazamakiLegacyState;

LegacyIconSprite moveAndTelopSprite;

void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);

std::uint8_t helpGetCurrentIconIdx(std::int32_t slot);

void mtwkSetRootTransformOverride(MotionWork *work, const Matrix4x4 *transform);

namespace {

static std::int16_t *moveProficiencyTables[4] = {0, g_moveSkillBank1.proficiency, g_moveSkillBank2.proficiency, g_moveSkillBank3.proficiency};

static std::int16_t *moveTrainingGainTables[4] = {0, g_fightMoveTrainingGains[0], g_fightMoveTrainingGains[1], g_fightMoveTrainingGains[2]};

static std::int8_t initialMoveProficiency1[13] = {28, 22, 23, 36, 20, 20, 20, 21, 20, 18, 19, 20, 19};
static std::int8_t initialMoveProficiency2[12] = {19, 21, 23, 18, 0, 17, 20, 20, 20, 20, 18, 15};
static std::int8_t initialMoveProficiency3[9] = {26, 27, 27, 22, 17, 20, 20, 24, 17};
static std::int8_t *initialMoveProficiencies[4] = {0, initialMoveProficiency1, initialMoveProficiency2, initialMoveProficiency3};

static const NativeMoveName moveNames1[28] = {
    {"\353\247\303\255", "\245\310\245\351\245\323\245\267", 0},
    {"\323\367\353\265\341\376\361\243", "\245\263\245\260\245\353\245\336\245\246\245\301", 1},
    {"\363\276\363\275\341\375\363\300", "\245\340\245\351\245\265\245\341\245\305\245\255", 1},
    {"\362\341\362\351\303\255", "\245\315\245\270\245\352\245\323\245\267", 2},
    {"\335\263\362\351\303\255", "\245\316\245\334\245\352\245\323\245\267", 2},
    {"\261\267\347\307\321\277\362\315", "\245\352\245\342\245\363\245\301\245\347\245\246\245\301\245\345\245\246", 3},
    {"\344\335\361\334\357\311", "\245\277\245\301\245\254\245\271\245\337", 5},
    {"\332\273\341\375\363\300", "\245\275\245\307\245\305\245\255", 6},
    {"\360\315\353\265", "\245\252\245\252\245\260\245\353\245\336", 7},
    {"\361\344\352\306\360\375", "\245\342\245\355\245\306\245\312\245\337", 8},
    {"\360\314\357\315", "\245\246\245\351\245\344\245\312\245\256", 9},
    {"\335\260\273\276\361\342", "\245\333\245\246\245\266\245\363\245\275\245\246", 10},
    {"\361\303\346\264", "\245\312\245\256\245\253\245\274", 14},
    {"\260\374\331\370\346\244", "\245\354\245\303\245\263\245\246\245\273\245\363", 4},
    {"\361\344\326\336", "\245\275\245\246\245\270\245\363", 5},
    {"\346\345\355\266\341\375\363\300", "\245\271\245\244\245\262\245\304\245\305\245\255", 6},
    {"\356\266\355\342", "\245\350\245\355\245\244\245\311\245\252\245\267", 10},
    {"\360\351\332\260\360\324\337\324", "\245\253\245\265\245\315\245\242\245\306", 11},
    {"\362\245\361\364\357\324\357\315", "\245\250\245\363\245\273\245\363\245\317\245\244\245\352\245\345\245\246", 16},
    {"\341\372\323\362\326\244", "\245\270\245\345\245\363\245\333\245\271\245\244", 1},
    {"\335\323\340\375\363\370\363\341\363\374\321\265", "\245\261\245\363\245\253\245\242\245\303\245\321\241\274", 4},
    {"\323\367\321\266\341\376", "\245\267\245\347\245\246\245\255\245\363\245\300", 7},
    {"\341\373\326\244", "\245\301\245\345\245\246\245\271\245\244", 8},
    {"\361\344\361\343\341\376", "\245\275\245\246\245\267\245\347\245\246\245\300", 8},
    {"\362\343\347\307\321\277\362\315", "\245\254\245\244\245\342\245\363\245\301\245\347\245\246\245\301\245\345\245\246", 9},
    {"\354\253\327\253\323\363", "\245\354\245\363\245\253\245\363\245\330\245\255", 11},
    {"\362\344\323\362\323\363\323\364", "\245\267\245\343\245\333\245\330\245\255\245\266\245\363", 17},
    {"\362\344\323\357\326\244", "\245\267\245\343\245\263\245\271\245\244", 18},
};

static const NativeMoveRecord moveRecords1[28] = {
    {0x80000001U, 0x13U, 0x0U, {0, 0, 0, 0}},
    {0x40000040U, 0x6aU, 0x0U, {1, 0, 0, 0}},
    {0x40000080U, 0x14U, 0x0U, {1, 0, 0, 0}},
    {0x44000000U, 0x17U, 0x0U, {1, 0, 0, 0}},
    {0x48000000U, 0x15U, 0x0U, {1, 0, 0, 0}},
    {0x80000002U, 0x11bU, 0x0U, {1, 0, 0, 0}},
    {0x40000200U, 0x4dU, 0x0U, {1, 0, 0, 0}},
    {0x40000100U, 0x1bU, 0x0U, {1, 0, 0, 0}},
    {0x40000400U, 0x1aU, 0x0U, {2, 0, 0, 0}},
    {0x40000800U, 0x148U, 0x0U, {1, 0, 0, 0}},
    {0x40002000U, 0x185U, 0x0U, {1, 0, 0, 0}},
    {0x40001000U, 0x6dU, 0x0U, {2, 0, 0, 0}},
    {0x42000000U, 0x37U, 0x0U, {1, 0, 0, 0}},
    {0x40U, 0xcdU, 0x0U, {2, 0, 0, 0}},
    {0x2U, 0x4bU, 0x4cU, {2, 0, 0, 0}},
    {0x800000U, 0xbfU, 0x0U, {2, 0, 0, 0}},
    {0x20U, 0x19U, 0x0U, {2, 0, 0, 0}},
    {0x4000000U, 0x193U, 0x195U, {2, 0, 0, 0}},
    {0x40400000U, 0xd4U, 0xd5U, {2, 0, 0, 0}},
    {0x100000U, 0x275U, 0x0U, {2, 0, 0, 0}},
    {0x1U, 0x38U, 0x0U, {2, 0, 0, 0}},
    {0x40000U, 0x1U, 0x0U, {2, 0, 0, 0}},
    {0x400U, 0x149U, 0x0U, {2, 0, 0, 0}},
    {0x200000U, 0x289U, 0x0U, {2, 0, 0, 0}},
    {0x20000U, 0x119U, 0x0U, {2, 0, 0, 0}},
    {0x2000000U, 0x27bU, 0x0U, {2, 0, 0, 0}},
    {0x800U, 0x276U, 0x0U, {2, 0, 0, 0}},
    {0x400000U, 0x28aU, 0x0U, {2, 0, 0, 0}},
};

static const std::uint8_t wazamakiGroupCommands1[19][7] = {{1, 0, 0, 0, 0, 0, 0}, {2, 2, 0, 0, 0, 0, 0}, {2, 3, 0, 0, 0, 0, 0}, {3, 2, 2, 0, 0, 0, 0},
    {3, 3, 3, 0, 0, 0, 0}, {3, 2, 3, 0, 0, 0, 0}, {3, 3, 2, 0, 0, 0, 0}, {3, 0, 7, 1, 0, 0, 0}, {4, 2, 0, 7, 1, 0, 0}, {4, 3, 0, 7, 1, 0, 0},
    {5, 2, 2, 0, 7, 1, 0}, {5, 3, 3, 0, 7, 1, 0}, {5, 2, 3, 0, 7, 1, 0}, {5, 3, 2, 0, 7, 1, 0}, {2, 4, 0, 0, 0, 0, 0}, {4, 4, 0, 7, 1, 0, 0},
    {3, 3, 0, 1, 0, 0, 0}, {4, 2, 0, 7, 6, 0, 0}, {4, 3, 0, 7, 6, 0, 0}};

static const NativeMoveName moveNames2[23] = {
    {"\363\373\363\372\355\266\351\307\362\351", "\245\337\245\253\245\305\245\255\245\262\245\352", 0},
    {"\363\371\362\247\351\307\362\351", "\245\325\245\337\245\262\245\352", 0},
    {"\362\347\344\373\363\273\357\325", "\245\307\245\242\245\267\245\311\245\341", 1},
    {"\363\347\357\330\330\370", "\245\317\245\351\245\244\245\312\245\256", 2},
    {"\354\253\327\253\362\246", "\245\354\245\363\245\253\245\363\245\277\245\244", 3},
    {"\263\357\351\307\362\351", "\245\244\245\253\245\305\245\301\245\262\245\352", 5},
    {"\362\345\316\332\313\360\362\351", "\245\261\245\265\245\256\245\352", 5},
    {"\363\277\335\263\362\351", "\245\277\245\255\245\316\245\334\245\352", 6},
    {"\353\247\360\313", "\245\263\245\351\245\363", 12},
    {"\357\316\355\266", "\245\267\245\363\245\262\245\304", 13},
    {"\353\266\346\264", "\245\317\245\344\245\306", 14},
    {"\346\264\353\265", "\245\325\245\246\245\267\245\343", 15},
    {"\362\245\361\364\360\316\360\374", "\245\250\245\363\245\273\245\363\245\263\245\246\245\255\245\343\245\257", 2},
    {"\361\362\361\361\362\350\363\341\327\254", "\245\310\245\353\245\315\241\274\245\311\245\255\245\303\245\257", 3},
    {"\361\345\360\376\360\350", "\245\304\245\301\245\260\245\342", 4},
    {"\361\364\346\264", "\245\304\245\340\245\270\245\253\245\274", 4},
    {"\314\372\362\351\351\307\362\351", "\245\253\245\352\245\262\245\352", 6},
    {"\354\254\362\245\354\253\314\372", "\245\322\245\250\245\363\245\354\245\363\245\262", 13},
    {"\357\312\355\301\362\351", "\245\253\245\262\245\254\245\352", 16},
    {"\330\366\330\367\360\374", "\245\320\245\306\245\244\245\255\245\343\245\257", 12},
    {"\341\374\346\263\360\374", "\245\325\245\257\245\267\245\363\245\255\245\343\245\257", 4},
    {"\361\364\331\360\360\374", "\245\273\245\363\245\257\245\246\245\255\245\343\245\257", 14},
    {"\360\316\327\276\360\374", "\245\263\245\246\245\352\245\345\245\246\245\255\245\343\245\257", 15},
};

static const NativeMoveRecord moveRecords2[23] = {
    {0x40010000U, 0xc8U, 0x0U, {1, 0, 0, 0}},
    {0x40020000U, 0xc4U, 0x0U, {1, 0, 0, 0}},
    {0x40040000U, 0x10U, 0x0U, {1, 0, 0, 0}},
    {0x40080000U, 0xfU, 0x0U, {2, 0, 0, 0}},
    {0x80000800U, 0x29aU, 0x297U, {2, 0, 0, 0}},
    {0x40800000U, 0xb3U, 0xb2U, {0, 0, 0, 0}},
    {0x41000000U, 0xabU, 0x0U, {0, 0, 0, 0}},
    {0x40100000U, 0xafU, 0x0U, {2, 0, 0, 0}},
    {0x40008000U, 0xa1U, 0x0U, {2, 0, 0, 0}},
    {0x40004000U, 0x9eU, 0x0U, {2, 0, 0, 0}},
    {0x60000000U, 0xa0U, 0x0U, {2, 0, 0, 0}},
    {0x50000000U, 0xaeU, 0x0U, {1, 0, 0, 0}},
    {0x40200000U, 0xaaU, 0x0U, {0, 0, 0, 0}},
    {0x1000000U, 0x35U, 0x36U, {2, 0, 0, 0}},
    {0x8U, 0xb9U, 0x0U, {2, 0, 0, 0}},
    {0x10U, 0xbbU, 0x0U, {2, 0, 0, 0}},
    {0x4U, 0x133U, 0x0U, {2, 0, 0, 0}},
    {0x1000U, 0xa6U, 0xa7U, {0, 0, 0, 0}},
    {0x200U, 0x124U, 0x0U, {2, 0, 0, 0}},
    {0x10000000U, 0x279U, 0x0U, {2, 0, 0, 0}},
    {0x8000000U, 0x285U, 0x0U, {2, 0, 0, 0}},
    {0x40000001U, 0x286U, 0x0U, {2, 0, 0, 0}},
    {0x40000020U, 0x260U, 0x0U, {2, 0, 0, 0}},
};

static const std::uint8_t wazamakiGroupCommands2[17][7] = {{1, 1, 0, 0, 0, 0, 0}, {2, 2, 1, 0, 0, 0, 0}, {2, 3, 1, 0, 0, 0, 0}, {4, 2, 2, 1, 1, 0, 0},
    {3, 3, 3, 1, 0, 0, 0}, {3, 2, 3, 1, 0, 0, 0}, {3, 3, 2, 1, 0, 0, 0}, {3, 0, 7, 1, 0, 0, 0}, {4, 2, 0, 7, 1, 0, 0}, {4, 3, 0, 7, 1, 0, 0},
    {5, 2, 2, 0, 7, 1, 0}, {5, 3, 3, 0, 7, 1, 0}, {5, 2, 3, 0, 7, 1, 0}, {5, 3, 2, 0, 7, 1, 0}, {2, 4, 1, 0, 0, 0, 0}, {4, 4, 0, 7, 1, 0, 0},
    {4, 4, 6, 7, 1, 0, 0}};

static const NativeMoveName moveNames3[22] = {
    {"\341\311\341\310\357\330\343\325\363\301", "\245\273\245\252\245\244\245\312\245\262", 0},
    {"\357\310\357\330\354\252", "\245\317\245\351\245\244\245\264\245\267", 1},
    {"\360\311\343\325\363\301", "\245\310\245\342\245\250\245\312\245\262", 2},
    {"\357\311\314\372\362\351", "\245\253\245\271\245\337\245\254\245\352", 3},
    {"\337\323\326\334\332\272\357\323", "\245\351\245\273\245\304\245\252\245\310\245\267", 4},
    {"\361\361\363\300\354\251\357\325", "\245\336\245\255\245\254\245\277\245\341", 5},
    {"\353\267\352\315\352\305", "\245\306\245\363\245\260\245\267\245\347\245\246", 6},
    {"\360\314\347\346\355\266", "\245\246\245\351\245\317\245\305\245\255", 7},
    {"\360\314\332\272\357\323", "\245\246\245\351\245\252\245\310\245\267", 8},
    {"\357\312\346\263", "\245\253\245\262\245\337", 12},
    {"\345\373\342\340\361\344", "\245\252\245\334\245\355\245\340\245\275\245\246", 9},
    {"\353\247\346\263\335\260\357\323", "\245\310\245\351\245\337\245\257\245\272\245\267", 10},
    {"\357\312\361\334", "\245\253\245\262\245\254\245\277\245\312", 13},
    {"\356\326\355\266", "\245\266\245\363\245\262\245\304", 14},
    {"\362\344\363\271", "\245\267\245\343\245\263\245\246", 11},
    {"\343\324\343\336", "\245\267\245\345\245\304\245\333\245\246", 0},
    {"\360\315\306\347\333\250", "\245\300\245\244\245\306\245\363\245\317\245\257", 1},
    {"\354\253\352\306\335\262", "\245\354\245\363\245\267\245\345\245\333\245\246", 3},
    {"\335\304\346\263\340\320", "\245\270\245\343\245\267\245\363\245\306\245\363", 5},
    {"\353\245\344\330\332\272", "\245\317\245\257\245\250\245\363\245\351\245\257", 9},
    {"\344\333\344\337\363\373\305\273\352\306", "\245\265\245\363\245\306\245\363\245\267\245\345", 15},
    {"\323\360\323\361", "\245\301\245\347\245\246\245\261\245\244", 16},
};

static const NativeMoveRecord moveRecords3[22] = {
    {0x80000004U, 0xf0U, 0x0U, {0, 0, 0, 0}},
    {0x80000008U, 0x103U, 0x0U, {0, 0, 0, 0}},
    {0x80000010U, 0xdfU, 0x0U, {0, 0, 0, 0}},
    {0x80000020U, 0xdeU, 0x0U, {0, 0, 0, 0}},
    {0x80000040U, 0xe5U, 0x0U, {0, 0, 0, 0}},
    {0x80000080U, 0x105U, 0x0U, {0, 0, 0, 0}},
    {0x80000100U, 0xd0U, 0x0U, {0, 0, 0, 0}},
    {0x80000200U, 0xcbU, 0x0U, {0, 0, 0, 0}},
    {0x80000400U, 0xc9U, 0x0U, {0, 0, 0, 0}},
    {0x100U, 0xebU, 0x0U, {0, 0, 0, 0}},
    {0x2000U, 0xf1U, 0x0U, {0, 0, 0, 0}},
    {0x80U, 0xdcU, 0xdaU, {0, 0, 0, 0}},
    {0x4000U, 0xedU, 0xeeU, {0, 0, 0, 0}},
    {0x8000U, 0xd6U, 0x0U, {0, 0, 0, 0}},
    {0x10000U, 0x120U, 0x0U, {0, 0, 0, 0}},
    {0x80000U, 0x1eU, 0x0U, {0, 0, 0, 0}},
    {0x40000002U, 0x27aU, 0x0U, {0, 0, 0, 0}},
    {0x40000004U, 0x273U, 0x0U, {0, 0, 0, 0}},
    {0x40000008U, 0x287U, 0x0U, {0, 0, 0, 0}},
    {0x20000000U, 0x28eU, 0x0U, {0, 0, 0, 0}},
    {0x80001000U, 0x29dU, 0x0U, {0, 0, 0, 0}},
    {0x40000010U, 0x132U, 0x0U, {0, 0, 0, 0}},
};

static const std::uint8_t wazamakiGroupCommands3[17][7] = {{1, 5, 0, 0, 0, 0, 0}, {2, 2, 5, 0, 0, 0, 0}, {2, 3, 5, 0, 0, 0, 0}, {3, 2, 2, 5, 0, 0, 0},
    {3, 3, 3, 5, 0, 0, 0}, {3, 2, 3, 5, 0, 0, 0}, {3, 3, 2, 5, 0, 0, 0}, {1, 5, 0, 0, 0, 0, 0}, {1, 5, 0, 0, 0, 0, 0}, {4, 2, 3, 3, 5, 0, 0},
    {4, 3, 2, 2, 5, 0, 0}, {3, 6, 7, 5, 0, 0, 0}, {4, 2, 6, 7, 5, 0, 0}, {5, 2, 6, 7, 5, 0, 0}, {5, 2, 2, 6, 7, 5, 0}, {6, 3, 2, 2, 6, 7, 5},
    {1, 7, 0, 0, 0, 0, 0}};

static const char *moveDescriptionPhrases[32] = {"\265\273\244\307\244\242\244\353", "\301\352\274\352\244\316", "\301\352\274\352\244\362",
    "\301\352\274\352\244\313", "\275\300\275\321\244\307\244\342\270\305\305\265\305\252\244\312\265\273\244\316\244\322\244\310\244\304\241\243",
    "\244\350\244\246\244\313", "\244\244\244\357\244\346\244\353", "\262\363\244\267\275\263\244\352", "\262\363\305\276", "\275\263\244\352",
    "\244\312\244\254\244\351", "\306\261\273\376\244\313", "\271\266\267\342", "\277\310\302\316\244\362", "\312\374\244\304", "\244\362\301\300\244\246",
    "\275\263\244\353", "\305\352\244\262", "\306\315\244\255", "\270\345\244\355", "\312\247\244\246", "\302\347\244\255\244\257",
    "\241\312\244\263\244\246\244\267\244\345\241\313", "\244\271\244\253\244\265\244\272", "\306\247\244\337\271\376", "\276\345\244\262\244\353",
    "\276\345\244\262", "\300\332\244\352", "\311\252\302\307\244\301", "\302\255\244\362", "\310\254\266\313\267\375",
    "\260\354\265\244\244\313\264\326\271\347\244\244\244\362\265\315\244\341\241\242"};

static const char *const moveDescriptions1[28] = {
    "\270\327\311\251\244\317\241\242\311\241\244\310\270\375\244\316\264\326\244\313\244\242\244\353\245\304\245\334\241\24206#\241\326\277\315\303\346"
    "\241\312\244\270\244\363\244\301\244\345\244\246\241\313\241\3271500",
    "\317\323\244\362\275\304\312\375\270\376\244\31608\261\277\306\260\244\307\302\307\244\301\262\274\244\355\244\267\241\24201\272\277\271\3741500",
    "\261\324\244\244\244\336\244\303\244\271\244\260\244\31218\244\362\241\326\302\274\261\253\241\327\244\310\244\244\244\246\277\315\302\316\244\316\265"
    "\336\275\352\244\31314\265\273",
    "\276\256\274\326\302\307\244\301\244\310\244\317\260\333\244\312\244\352\241\242\277\345\312\277\312\375\270\376\244\313\277\266\244\353\241\242#\302"
    "\347\277\266\244\352\244\31606\245\325\245\303\245\257\244\307\244\242\244\353",
    "\244\244\244\303\244\277\244\363\302\316\244\362\262\274\244\313\304\300\244\341\241\242\277\255\244\323\276\345\244\254\244\35210#\262\274\244\253"
    "\244\35101\245\242\245\264\244\362\277\266\244\352\302\307\244\304\265\273",
    "\302\316\305\366\244\277\244\352\244\313\266\341\244\244\264\266\263\320\244\307\271\324\244\312\244\24628\241\24301\277\310\302\316\244\316\300\265"
    "\303\346\300\37615",
    "\260\354\312\34224\244\340\244\31011\241\242\274\352\305\341\244\316\311\364\312\254\244\30701\317\306\312\242\244\362\306\345\244\26005\273\302\244"
    "\352\244\304\244\261\244\353\265\273",
    "1929\301\260\244\313\260\354\312\342\277\312\244\341\244\306\241\242\303\346\303\312\244\362\300\265\267\375\244\307\306\315\244\25700",
    "31\272\277\271\374\266\341\312\325\244\362\301\300\244\303\244\306\241\242#\274\320\244\341\244\313\277\266\244\352\302\307\244\277\244\354\244\353"
    "\302\347\265\273",
    "\261\324\244\25724\244\337\241\242\274\352\244\316\244\322\244\351\244\344\317\323\301\264\302\316\244\362\273\310\244\303\244\306#01\266\273\244\344"
    "\263\334\244\36212\244\271\244\353\274\352\265\273",
    "08\244\267\244\306\316\242\267\375\244\36214\264\361\275\261\265\273\241\24308\244\316\316\317\244\254#\262\303\244\357\244\353\244\277\244\341\241"
    "\242\260\322\316\317\244\317\271\342\244\244",
    "1308\244\265\244\273\241\242\261\324\263\321\244\313\266\312\244\262\244\277\311\252\244\30701#\277\345\267\356\244\3621825\265\273",
    "\301\366\244\353\245\271\245\324\241\274\245\311\244\362\315\370\315\321\244\267\241\242\305\250\244\316\270\252\270\375\244\253\244\351\274\363\266"
    "\332\244\313\244\253\244\261\244\306\241\24228\244\362\274\320\244\341\244\31314\265\273",
    "\244\244\244\303\244\277\244\363\302\316\300\252\244\362\304\343\244\257\244\267\244\277\244\242\244\310\311\250\244\316\245\320\245\315\244\362#\263"
    "\350\244\253\244\267\241\24201\245\242\245\264\244\3621825\265\273",
    "21\302\316\244\362\273\310\244\303\244\30627\244\252\244\355\244\267\241\24227\244\252\244\355\244\267\244\307\275\314\244\341\244\277\277\310\244\362"
    "\263\253\244\257\244\310112725\265\273",
    "\301\260\277\312\244\271\244\353\244\310\244\310\244\342\244\313\303\346\303\31218\244\362\312\374\244\301\241\242\244\265\244\351\244\313#24\244\363"
    "\244\307\306\261\244\270\276\354\275\352\244\362\265\325\244\316\274\352\244\307\306\315\244\257\265\273",
    "\303\346\271\361\244\253\244\351\307\316\267\356\264\340\244\254\273\375\244\301\265\242\244\303\244\277\265\273\241\243\302\255\271\370\244\31608\244"
    "\307\300\270\244\270\244\277\316\317\244\36203\303\241\244\255\271\376\244\340",
    "03\314\251\303\345\244\265\244\273\244\277\276\270\244\362\244\342\244\246\260\354\312\375\244\316\276\270\244\307\267\343\244\267\244\257\303\241\244"
    "\257\273\366\244\307\260\322\316\317\244\362\277\273\306\251\244\265\244\273\244\353\271\323\265\273",
    "\261\355\300\304\267\375\244\316\265\273\244\307\241\24212\244\362\274\365\244\261\316\256\244\271\244\31011\241\242#0129\275\263\244\303\244\306\305"
    "\335\244\271\241\242\271\342\305\331\244\31200",
    "24\244\340\302\255\244\310\306\261\244\270\312\375\244\316\274\352\244\36218\275\320\244\271\265\273\241\243#\303\346\271\361\267\375\313\241\244\307"
    "\244\317\264\360\313\334\305\252\244\31218\265\273",
    "19\244\31321\262\274\244\254\244\303\244\306\277\310\244\362\275\314\244\341\241\242\301\260\277\312\244\271\244\353\244\310\244\310\244\342\244\313"
    "\301\264\316\317\244\307\245\242\245\303\245\321\241\274\244\36214\271\353\262\367\244\312\265\273",
    "\276\345\303\312\274\365\244\261\244\31011\276\270\244\36218\275\320\244\271\241\242\271\266\313\311\260\354\302\316\244\310\244\312\244\303\244\277"
    "\271\342\305\331\244\312\265\273\241\243\315\315\241\271\244\312\312\321\262\275\244\254\244\242\244\353",
    "30\244\316\264\360\313\334\265\273\244\307\272\307\275\305\315\327\244\316\265\273\241\243\271\370\244\31608\244\362#\315\370\315\321\244\267\244\3061"
    "8\244\36214",
    "31\316\276\274\352\244\316\276\270\244\36201\266\273\244\313\303\241\244\255\244\304\244\261\244\353\265\273",
    "30\317\273\302\347\263\253\244\316\260\354\244\304\241\2430112\244\362\244\265\244\320\244\255\241\242\302\316\300\252\244\362\312\370\244\267\302\246"
    "\314\314\244\253\244\351\304\272\311\252\244\362\302\307\244\304\302\347\265\273",
    "\263\303\274\35222\244\310\244\244\244\246\274\352\267\277\244\30701\245\242\245\264\244\362\304\267\244\31526\241\242\267\344\244\362\244\304\244\244"
    "\244\306\276\270\244\362\315\356\244\310\244\271",
    "\301\35201\302\246\314\314\244\313\262\363\244\352\271\376\244\340\244\31011\241\242\276\345\244\253\244\351#\260\256\244\303\244\277\267\375\244\362"
    "\272\277\271\374\244\344\306\254\244\313\303\241\244\255\244\304\244\261\244\353\265\273",
    "\303\346\303\312\244\31318\244\362\312\374\244\301\241\242\260\325\274\261\244\254\262\274\244\330\244\244\244\303\244\27701\306\254\244\362\270\314"
    "\244\362\311\301\244\25705\316\242\267\375\244\307\305\335\244\271\265\273"};

static const char *const moveDescriptions2[23] = {
    "\262\274\244\253\244\351\302\300\305\341\241\312\244\277\244\301\241\313\244\362\244\271\244\3522505#\267\253\244\352\275\320\244\265\244\354\244\3530"
    "9\265\273\241\24301\306\254\311\36415",
    "06\301\26009\241\243\274\347\244\313\304\336\300\350\244\307\277\315\302\316\244\316\300\265\314\314\244\313#\244\242\244\353\265\336\275\35215",
    "\302\316\244\362\301\260\312\375\244\31321\260\334\306\260\244\265\244\27310\241\242\301\260\302\255\244\316#\302\255\305\341\244\30701\262\274\303"
    "\312\244\253\244\351\303\346\303\312\244\36216\265\273",
    "01\311\250\244\316\263\260\302\246\244\362\301\300\244\244\241\242\276\256\244\265\244\257\241\242\244\253\244\304\261\324\244\257#\312\374\244\277"
    "\244\354\244\353\245\355\241\274\245\255\245\303\245\257\244\316\260\354\274\357",
    "\245\253\245\253\245\310\244\307020926\244\277\270\345\241\24223#\244\342\244\246\260\354\312\375\244\316\302\255\244\30709\244\36214\306\363\303\3120"
    "9",
    "\303\346\303\312\244\31607\244\253\244\351\312\321\262\275\244\267\244\306\241\242\354\373\315\356\244\267\244\36214\265\273\241\243\265\260\306\273"
    "\244\317\244\255\244\357\244\341\244\306\276\256\244\265\244\244",
    "1907\244\316\260\354\274\357\241\243\312\270\273\372\304\314\244\352\241\242\267\266\272\30027\244\313\273\302\244\352\244\304\244\261\244\35305\277"
    "\266\244\352\262\274\244\355\244\271",
    "\260\354\275\326\302\316\244\362\275\314\244\341\244\277\270\345\241\242\277\255\244\323\276\345\244\254\244\3530516\241\242\260\354\274\357\244\316"
    "\310\364\244\323\311\25009",
    "\271\342\302\256\244\3161907\241\243\302\316\244\36221\305\335\244\271\273\366\244\307#\261\363\277\264\316\317\244\310\260\322\316\317\244\362\301"
    "\375\262\303\244\265\244\273\244\306\244\244\244\353",
    "\301\260\312\375\244\313\266\365\303\34608\244\362\244\2671009\244\36214\241\242#06\315\341\244\323\244\27309",
    "\304\267\244\3231907\241\24308\244\310\304\267\314\366\244\316\316\317\244\254\262\303\244\357\244\353\244\277\244\341\241\242\260\322\316\317\244\254"
    "\271\342\244\244",
    "\310\364\244\3231909\241\24301\274\363\266\332\244\362\267\266\272\300\241\312\244\261\244\265\241\31327\244\31316\302\34700",
    "\261\355\300\304\267\375\244\316\302\255\265\273\244\316\244\322\244\310\244\304\241\243\277\310\244\362\260\372\244\257\273\366\244\30702\315\266\244"
    "\244\271\376\244\337\241\242200516\265\273",
    "\266\365\303\346\244\30707\244\362\275\320\244\267\244\277\244\242\244\310\241\242\261\363\277\264\316\317\244\362#\263\350\244\253\244\267\241\242"
    "\244\342\244\246\243\26108\244\267\244\30609\244\36214\271\323\265\273",
    "03\270\376\244\303\244\306\276\257\244\267\244\271\244\331\244\352\271\376\244\337\241\242\265\325\316\251\244\301\244\2671001\245\242\245\264\300\350"
    "\244\36216\264\361\275\261\305\252\244\312\265\273",
    "13\304\300\244\341\241\242\274\26429\303\346\277\264\244\313\265\336\300\373\262\363\244\267\241\242\244\342\244\246#\260\354\312\375\244\316\302\255"
    "\244\30701\302\255\270\265\244\3622000",
    "01\274\363\244\362\272\254\244\263\244\275\244\256\264\242\244\352\274\350\244\35305\245\253\245\253\245\310\244\30716\271\353\262\367\244\312\265\273"
    "\241\243\306\254\244\316\265\336\275\352\244\362\301\300\244\24414",
    "07\244\362\312\374\244\303\244\277\270\345\244\313\241\242\244\342\244\246\260\354\312\375\244\316\302\255\244\307#2319\262\363\244\267\354\373\315"
    "\356\244\267\244\36214\277\327\302\256\244\312\265\273",
    "\301\366\244\303\244\306\244\244\244\353\272\307\303\346\244\313\244\244\244\255\244\312\244\352\241\2421308\244\265\244\273\244\306\263\352\244\352"
    "\271\376\244\337\241\24201\302\255\270\265\244\36220\265\273",
    "\265\336\244\31303\307\330\303\346\244\362\270\376\244\261\244\277\270\345\241\242\277\277\262\274\244\253\244\351\245\253\245\253\245\310\244\3620926"
    "\241\242\245\242\245\264\244\36216\265\273",
    "\301\352\274\352\244\316\314\334\244\316\301\260\244\307\265\336\267\343\244\313\277\310\244\362\304\343\244\257\244\267\244\306\241\242\301\352\274"
    "\352\244\316\302\255\270\265\244\362\245\253\245\253\245\310\244\307\275\263\244\353\264\361\275\261\265\273",
    "\310\36403\301\366\244\352\271\376\244\337\241\242\314\334\244\316\301\260\244\307\304\267\314\366\244\267\274\320\244\341\262\274\244\253\244\351\265"
    "\336\267\343\244\31309\244\3621825\265\273",
    "01\301\260\244\307\277\310\244\362\304\343\244\257\244\2670828\241\24223\265\325\316\251\244\30109\244\36214\271\342\305\331\244\31209\265\273"};

static const char *const moveDescriptions3[22] = {
    "0401\317\323\244\362#\274\350\244\352\241\242\262\373\244\313\306\376\244\352\271\376\244\363\244\30717\244\353",
    "0429\273\310\244\303\244\306#01\306\342\270\324\244\316\311\364\312\254\244\362\312\247\244\24426\244\30617\244\353",
    "04\274\253\312\254\244\254\270\345\312\375\244\313\305\335\244\35410\241\2420219\244\31317\244\353\265\273",
    "01\274\363\244\313\317\323\244\362\264\254\244\255\244\304\244\261\241\242\302\316\275\305\244\316\244\253\244\253\244\303\244\306\244\244\244\35329"
    "\264\242\244\35317\265\273",
    "02\260\354\313\334\307\330\311\351\244\244\244\30717\244\353\244\31011\241\242\274\253\244\351\244\34208\244\26701\276\345\244\313\302\316\244\362\315"
    "\356\244\271\302\347\265\273",
    "\317\323\244\362\274\350\244\352\265\325\264\330\300\341\244\362\267\350\244\341\244\306\262\241\244\265\244\250\244\304\244\261\244\353\265\273\241"
    "\243#\270\305\316\256\275\300\275\321\244\307\244\317\264\360\313\334\305\252\244\31200",
    "\277\310\244\362\304\343\244\257\244\267100129\274\350\244\352\241\242\270\252\244\313\303\264\244\25626\241\242\303\317\314\314\244\313\303\241\244"
    "\255\315\356\244\271\271\353\262\367\244\312\265\273",
    "01\302\246\314\314\244\253\244\351\300\334\266\341\244\267\241\24202\310\27608\244\265\244\2731017\315\356\244\310\244\271\265\273",
    "04\270\345\312\375\244\313\244\322\244\315\244\352\315\356\244\310\244\271050217\244\353",
    "0112\244\362\244\253\244\357\244\267\244\304\244\304\241\242\307\330\270\345\244\313\262\363\244\352\271\376\244\337#\305\250\244\316\273\340\263\321"
    "\244\313\244\304\244\257\265\273",
    "01\317\323\264\330\300\341\244\313\277\364\245\366\275\35211\245\300\245\341\241\274\245\270\244\362\315\277\244\250\244\353\241\242\312\243\273\250"
    "\244\312\271\342\305\37100",
    "\260\354\275\326\241\24213\304\343\244\257\244\267\244\306\316\317\244\362\244\277\244\341\241\242\275\326\310\257\316\317\244\362#\244\342\244\303"
    "\244\306\241\24203\266\257\316\365\244\312\306\25418\244\362\244\271\244\353\265\273",
    "\261\306\277\310\244\316\307\311\300\270\265\273\241\243\270\345\312\375\244\313\262\363\244\303\244\277\244\242\244\310\241\242\314\265\313\311\310"
    "\367\244\310\244\312\244\303\244\27701\274\363\266\332\244\313\274\352\305\341\244\362\302\307\244\301\271\376\244\340",
    "0112\244\362\244\312\244\312\244\341\244\313\244\253\244\357\244\271\244\31011\241\242#\273\340\263\321\244\253\244\351\265\336\300\334\266\341\244"
    "\267\241\242\302\316\305\366\244\277\244\352\244\271\244\353\265\273",
    "01\270\345\312\375\244\31329\277\312\244\341\244\353\244\31011\263\303\274\352#22\244\307\245\242\245\264\244\362\302\307\244\301\305\335\244\271\302"
    "\300\266\313\267\375\244\316\265\273",
    "\313\334\315\350\244\316\260\325\314\243\244\31717\265\273\241\243\277\247\241\271\244\312\312\375\270\376\244\313\271\245\244\255\244\3120517\312\254"
    "\244\261\244\353\271\342\305\331\244\312\265\273\275\321",
    "\352\273\352\247\267\375\244\316\265\273\244\30701\316\27629\264\242\244\353\244\31011#\276\345\310\276\277\310\244\362\274\352\244\3072017\265\273",
    "\317\242\302\263\244\307\301\307\301\341\244\257\304\276\306\315\244\255\244\344\316\242\267\375\244\362\276\345\303\312\241\242\303\346\303\312\244"
    "\313#\306\315\244\255\244\362\312\374\244\304\271\342\305\331\244\312\317\242\302\263\265\273",
    "01\302\246\314\314\244\253\244\351\276\270\244\362\302\307\244\301\241\242\244\271\244\260\244\313\305\276\277\310\244\267\244\306#\310\277\302\320"
    "\244\316\302\246\314\314\244\313\306\376\244\35217\305\335\244\271\265\273",
    "\301\307\301\341\244\25701\270\252\270\375\244\313\244\316\244\334\244\352\241\24202\305\276\244\320\244\273\244\353\244\31011\306\254\311\364\244\362"
    "\266\257\302\307\244\271\244\35317\265\273",
    "\310\254\266\313\267\375\310\254\302\347\276\267\274\260\244\316\260\354\244\304\241\243\314\334\244\344\271\242\244\362\301\307\301\341\244\257\243"
    "\263\262\363\306\315\244\257\265\273\244\307\241\242\266\262\244\355\244\267\244\244\273\246\275\375\316\317\244\362\273\375\244\304",
    "\314\334\244\300\244\261\244\307\244\312\244\257\241\242\301\264\277\310\244\362\273\310\244\303\244\30601\265\244\307\333\244\362#\264\266\244\270"
    "\244\353\271\342\305\331\244\312\265\273\275\321"};

} // namespace

std::int8_t moveDefinitionCounts[4] = {0, 28, 23, 22};

namespace {

static const NativeMoveName *moveNames[4] = {0, moveNames1, moveNames2, moveNames3};

static const std::uint8_t (*wazamakiGroupCommands[4])[7] = {0, wazamakiGroupCommands1, wazamakiGroupCommands2, wazamakiGroupCommands3};

static const char *const *moveDescriptions[4] = {0, moveDescriptions1, moveDescriptions2, moveDescriptions3};

static const NativeMoveRecord *moveRecords[4] = {0, moveRecords1, moveRecords2, moveRecords3};

static const std::uint16_t wazamakiInputIcons[8] = {778, 776, 2048, 2048, 780, 777, 779, 770};

static const std::uint32_t wazamakiInputColours[8] = {0xff808000U, 0xff802020U, 0xff9f9f9fU, 0xff9f9f9fU, 0xff802080U, 0xff202080U, 0xff006000U, 0xff333333U};

HLib::Task *moveLearnedTask;

std::int16_t moveLearnedTextures[3] = {0, 0, 0};

std::uint16_t moveLearnedReserved, moveLearnedSavedHelp;

std::int16_t pendingLearnedCategory, pendingLearnedMove;

std::int8_t moveLearnedSoundDelay;

NativeWazamakiState wazamaki;

HLib::Task *wazamakiTask;

std::uint32_t wazamakiSavedControlEnabled;

NativeWazamakiGroup wazamakiHandGroups[28], wazamakiLegGroups[23], wazamakiThrowGroups[22];

NativeWazamakiGroup *wazamakiGroups[4] = {0, wazamakiHandGroups, wazamakiLegGroups, wazamakiThrowGroups};

std::int32_t wazamakiSelectedRow, wazamakiFirstVisible = 0;

std::int8_t wazamakiRepeatDelay;

std::uint32_t wazamakiSavedNpcLoadState;

std::int32_t wazamakiTextures[8];

std::int16_t wazamakiNameSurfaces[4][40], wazamakiNameLengths[4][40], wazamakiRubySurfaces[4][40];

std::int16_t wazamakiSpecialSurface;

std::int16_t wazamakiStatisticSurfaces[3];

std::uint16_t wazamakiCriticalFrames[3];

LegacyIconSprite wazamakiNameSprite, wazamakiRubySprite, wazamakiSpecialSprite, wazamakiInputSprite = {};

LegacyIconSprite wazamakiStatisticLabel = {}, wazamakiCriticalSprite = {}, wazamakiBackgroundSprite = {}, wazamakiScrollSprites[4] = {};

LegacyColourRect wazamakiRowBar, wazamakiStatisticBar, wazamakiDividerBar = {};

NativeWazamakiDescription wazamakiDescription;

NativeWazamakiPreview wazamakiPreview;

std::int32_t wazamakiPreviewMotionYaw = 0;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void vfResetMoveTrainingGains();
void ResetWazamakiState();

void InitializeEventWindowState() {
    std::int32_t counts[4] = {0, 13, 12, 9};
    moveLearnedTask = 0;
    pendingLearnedMove = 0;
    pendingLearnedCategory = 0;
    moveLearnedReserved = 0;
    g_moveUiClockAndNavigationHold = 0;
    g_moveSkillBank1.count = 28;
    g_moveSkillBank2.count = 23;
    g_moveSkillBank3.count = 22;
    for (std::int32_t category = 1; category < 4; ++category) {
        std::int16_t *const proficiency = moveProficiencyTables[category];
        const std::int8_t *const initial = initialMoveProficiencies[category];
        const std::int32_t count = counts[category];
        for (std::int32_t move = 0; move < 40; ++move) {
            if (move < count)
                proficiency[move] = initial[move];
            else
                proficiency[move] = 0;
        }
    }
    vfResetMoveTrainingGains();
    ResetWazamakiState();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace {

void SetMoveSkillFlag(std::uint8_t mode, std::uint8_t category, std::uint8_t move);
void MoveLearned_Callback(void *);
void WazamakiTaskCallback(void *);
void RefreshWazamakiLearnedGroups();
NativeWazamakiGroup *FindWazamakiGroup(std::int32_t row, NativeWazamakiGroupPosition *position);
void SelectCurrentLegacyVariant(NativeWazamakiGroup *group, std::int32_t category);
std::uint16_t RepeatWazamakiInput(std::uint16_t pressed, std::uint16_t repeated);
void UpdateLegacyWazamakiInput();
inline HLib::Task *GetWazamakiWindowTask() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD;
}
std::uint16_t GetMoveMotion(std::uint8_t category, std::uint8_t move);
std::uint16_t GetMoveBaseMotion(std::uint8_t category, std::uint8_t move);
void SetupWazamakiCamera();
void RestoreWazamakiCamera();
bool LoadLegacyWazamakiTextures();
void ReleaseWazamakiDescription(NativeWazamakiDescription *description);
void ReleaseLegacyWazamakiTextures();
void UpdateWazamakiNameSurfaces();
void ReleaseWazamakiNameSurfaces();
void InitializeWazamakiNameSprites();
void InitializeWazamakiSprites();
void InitializeWazamakiDescription(NativeWazamakiDescription *description, float left, float top, float right, float bottom);
void UpdateWazamakiDescriptionText(NativeWazamakiDescription *description, const char *text);
void RefreshWazamakiDescriptionAndMotion(NativeWazamakiDescription *description);
void RequestWazamakiPreviewMotion(std::int32_t category, std::int32_t move);
void UpdateWazamakiPreviewTransform();
void ReleaseWazamakiPreviewModels();
void LoadRyoSceneForMovesScreen();
void UpdateWazamakiPreviewFrame();
void DrawWazamakiPreview();
void DrawWazamakiStatistics();
void DrawWazamakiScrollbar(float x, float top, float, float bottom, float first, float last);
void DrawWazamakiRowScrollbar();
void DrawWazamakiDescription(const NativeWazamakiDescription *description);
void DrawWazamakiRows();
void DrawWazamakiButtons();
void DrawLegacyWazamaki();
bool InitializeLegacyWazamaki();
void ReleaseLegacyWazamaki();

} // namespace

std::int16_t SetMoveLearningEntry(std::uint8_t mode, std::uint8_t category, std::int16_t move, std::int16_t value);
void SetNewEwzoTypeForMoveLearned(std::int8_t mode, std::int16_t category, std::int16_t move);
std::uint32_t IsWazamakiInactive();
void SetupWZ2_CtrlWazamaki();
bool UpdateWazamakiMenu();
void QueueWazamakiTask();
void CloseWazamakiMenu();
std::int32_t GetMoveProficiency(std::uint8_t category, std::uint8_t move);
void AddMoveProficiencyUsage(std::uint8_t category, std::int16_t move, std::int16_t amount);
void ApplyMoveProficiencyUsage();
void AwardStepMoveTraining();
#else
namespace {

std::span<const MoveDefinition> MoveDefinitions(std::uint8_t category);
std::span<WazamakiMoveInfo> MoveInfoCache(std::uint8_t category);
std::array<std::uint32_t, 4> ReadSkillFlags();
bool SkillFlagSet(const std::array<std::uint32_t, 4> &flags, std::uint32_t encoded);
std::span<NativeWazamakiGroup> NativeWazamakiGroups(std::uint8_t category);
void WazamakiTaskCallback(void *);
void CacheWazamakiGroups();
void MoveLearnedOwner_NullCallback(void *);
void SelectCurrentLegacyVariant();
void UpdateLegacyWazamakiInput();
void RestoreWazamakiCamera();
bool LoadLegacyWazamakiTextures();
void UpdateWazamakiNameSurfaces(std::int32_t mode);
void InitializeWazamakiDescription(float left, float top, float right, float bottom);
void UpdateWazamakiDescriptionText(const char *text);
void RefreshWazamakiDescriptionAndMotion();
void PlayCurrentLegacyMove();
std::int32_t UpdatePreviewAnalogAngle(std::int32_t current, std::int32_t target, std::uint8_t analog, std::int32_t step);
void UpdateLegacyPreviewTransform();
WazamakiLearnedGroup *CurrentLearnedGroup();
void DrawWazamakiStatistics();
void DrawWazamakiScrollbar(float x, float top, float bottom, float first, float last);
void DrawWazamakiDescription();
void DrawWazamakiRows();
void DrawWazamakiButtons();
void DrawLegacyWazamaki();
void WazamakiLoadingCompleteNullsub();
bool InitializeLegacyWazamaki();
bool ExecuteLegacyWazamaki();
void SetupModernMovesCamera();
bool ExecuteModernWazamaki();
void ReleaseLegacyWazamaki();

} // namespace

std::int32_t SetNewEwzoTypeForMoveLearned(std::int8_t mode, std::int16_t category, std::int16_t moveIndex) noexcept;
void ReleaseMoveLearnedOwner() noexcept;
bool IsMoveLearnedActive() noexcept;
std::span<const WazamakiMoveInfo> GetWazamakiMoveCategory(std::uint8_t category) noexcept;
WazamakiSelection GetWazamakiSelection() noexcept;
bool SetWazamakiSelection(std::uint8_t category, std::uint8_t row) noexcept;
std::int64_t SetMoveEquipment(std::int8_t operation, std::int32_t category, std::uint8_t move) noexcept;
bool SelectWazamakiMove(std::uint8_t category, std::uint8_t id) noexcept;
std::uint32_t IsWazamakiInactive() noexcept;
void SetupWZ2_CtrlWazamaki() noexcept;
bool UpdateWazamakiMenu();
void CancelWazamakiMenu(std::int32_t);
void QueueWazamakiTask();
void RefreshWazamakiLearnedGroups();
std::int32_t GetWazamakiGroupCount(std::int32_t category) noexcept;
std::span<const WazamakiLearnedGroup> GetWazamakiLearnedGroups() noexcept;
void PlayWazamakiGroupMove(std::uint32_t row);
void ResetMoveLearningPresentationState() noexcept;
NativeWazamakiGroup *FindWazamakiGroup(std::int32_t row, std::int32_t *category, std::int32_t *groupIndex);
std::int64_t ResetWazamakiState() noexcept;
void FillScrollListMoveData(std::int32_t ordinal, ScrollListMoveData &data);
void RequestWazamakiPreviewMotion(std::uint8_t category, std::uint8_t id);
void ReleaseWazamakiPreviewModels();
void LoadRyoSceneForMovesScreen();
void UpdateWazamakiPreviewFrame();
std::uint32_t WazamakiMatrixUpdateNullsub();
void DrawWazamakiPreview();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint16_t GetMoveBaseMotion(std::uint8_t category, std::uint8_t move) {
    return moveRecords[category][move].motion;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint16_t GetMoveMotion(std::uint8_t category, std::uint8_t move) {
    std::uint16_t motion = moveRecords[category][move].motion;
    const std::uint16_t alternate = moveRecords[category][move].alternateMotion;
    if (alternate)
        motion = alternate;
    return motion;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void SetMoveSkillFlag(std::uint8_t mode, std::uint8_t category, std::uint8_t move) {
    if (static_cast<std::uint8_t>(category - 1) >= 3 || move >= 40)
        return;
    const std::uint32_t encoded = moveRecords[category][move].skillFlag;
    const std::uint8_t bank = static_cast<std::uint8_t>(encoded >> 30);
    const std::uint32_t mask = encoded & 0x3fffffffU;
    switch (mode) {
    case 0:
        g_nativePlayerStats.statFlags[bank] |= mask;
        break;
    case 1:
        g_nativePlayerStats.statFlags[bank] &= ~mask;
        break;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void MoveLearned_Callback(void *) {
    if (moveLearnedSoundDelay >= 0) {
        --moveLearnedSoundDelay;
        if (moveLearnedSoundDelay == 13)
            ExecuteAicaCommand(0x208ab, 0, 0);
        if (moveLearnedSoundDelay < 0)
            ExecuteAicaCommand(0x208ab, 0, 0);
    }
    DrawMainIconOverlay(0x7f002d15U, 0.0f);
    moveAndTelopSprite.width = 32;
    moveAndTelopSprite.height = 512;
    moveAndTelopSprite.scaleX = 1.2f;
    moveAndTelopSprite.scaleY = 1.2f;
    moveAndTelopSprite.alignment = 5;
    moveAndTelopSprite.depth = 0.5f;
    moveAndTelopSprite.rotation = 0;
    moveAndTelopSprite.icon = moveLearnedTextures[0];
    moveAndTelopSprite.x = 384.0f;
    moveAndTelopSprite.y = 112.0f;
    if (moveAndTelopSprite.icon != -1)
        PrepareDrawTextureWithMode(moveAndTelopSprite);
    moveAndTelopSprite.icon = moveLearnedTextures[1];
    moveAndTelopSprite.x = 320.0f;
    moveAndTelopSprite.y = 208.0f;
    if (moveAndTelopSprite.icon != -1)
        PrepareDrawTextureWithMode(moveAndTelopSprite);
    moveAndTelopSprite.icon = moveLearnedTextures[2];
    moveAndTelopSprite.x = 256.0f;
    moveAndTelopSprite.y = 256.0f;
    if (moveAndTelopSprite.icon != -1)
        PrepareDrawTextureWithMode(moveAndTelopSprite);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int16_t SetMoveLearningEntry(std::uint8_t mode, std::uint8_t category, std::int16_t move, std::int16_t value) {
    std::int32_t result = 0;
    if (move >= moveDefinitionCounts[category])
        return 0;
    std::int16_t *const proficiency = moveProficiencyTables[category];
    if (!mode)
        SetMoveSkillFlag(0, category, static_cast<std::uint8_t>(move));
    else {
        value = 0;
        SetMoveSkillFlag(1, category, static_cast<std::uint8_t>(move));
    }
    proficiency[move] = value;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AddMoveProficiencyUsage(std::uint8_t category, std::int16_t move, std::int16_t amount) {
    if (static_cast<std::uint8_t>(category - 1) < 3 && GetMoveProficiency(category, static_cast<std::uint8_t>(move)) > 0)
        moveTrainingGainTables[category][move] = static_cast<std::int16_t>(moveTrainingGainTables[category][move] + amount);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AwardStepMoveTraining() {
    for (std::int32_t category = 1; category < 4; ++category)
        for (std::int32_t move = 0; move < 40; ++move) {
            const std::int32_t value = GetMoveProficiency(static_cast<std::uint8_t>(category), static_cast<std::uint8_t>(move));
            if (value > 0) {
                if (value < 12)
                    AddMoveProficiencyUsage(static_cast<std::uint8_t>(category), static_cast<std::int16_t>(move), 1);
                else
                    AddMoveProficiencyUsage(static_cast<std::uint8_t>(category), static_cast<std::int16_t>(move), 30);
            }
        }
    ApplyMoveProficiencyUsage();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ApplyMoveProficiencyUsage() {
    for (std::int16_t category = 1; category < 4; ++category) {
        std::int16_t *const proficiency = moveProficiencyTables[category];
        std::int16_t *const gains = moveTrainingGainTables[category];
        for (std::int16_t move = 0; move < 40; ++move) {
            std::int16_t value;
            const std::int16_t previous = proficiency[move];
            if (category == 3)
                value = static_cast<std::int16_t>(previous + gains[move] * category);
            else
                value = static_cast<std::int16_t>(previous + gains[move] / 2);
            if (value > 199)
                value = 199;
            proficiency[move] = value;
            gains[move] = 0;
            if (previous < 12 && value > 11)
                SetMoveLearningEntry(0, static_cast<std::uint8_t>(category), move, value);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetMoveProficiency(std::uint8_t category, std::uint8_t move) {
    const std::int16_t *proficiency;
    if (static_cast<std::uint8_t>(category - 1) > 2)
        return 0;
    proficiency = moveProficiencyTables[category];
    if (move < 40)
        return proficiency[move];
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ReleaseNamedTexture_Wrapper(std::int32_t index);
std::int32_t ReadTitleSpriteTextures(const void *source, std::uint32_t size, std::int32_t *textures);
void AppendCentringSpaces(char *text, std::int16_t width);

// @verified - d
void SetNewEwzoTypeForMoveLearned(std::int8_t mode, std::int16_t requestedCategory, std::int16_t requestedMove) {
    std::uint8_t operation = static_cast<std::uint8_t>(mode);
    std::int16_t category = requestedCategory;
    std::int16_t move = requestedMove;
    if (operation & 0x80U) {
        category = pendingLearnedCategory;
        move = pendingLearnedMove;
        pendingLearnedCategory = 0;
        pendingLearnedMove = 0;
        operation &= 0x7fU;
    }
    if ((operation & 1U) == 0) {
        if (!moveLearnedTask && category) {
            moveLearnedTask = HLib::EnqueueTaskWithoutParameter(MoveLearned_Callback, 15, 0, SHEN_NAME4('E', 'W', 'Z', 'O'));
            static const char *const categories[4] = {"\363\340\355\267", "\352\306\355\267", "\344\373\355\267", "\335\264\355\267"};
            MapControlData *map;
            if (category == 3 && move == 21) {
                map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                moveLearnedTextures[0] =
                    static_cast<std::int16_t>(CreateVerticalWindowTextSurface(map->retainedTaskD, "\321\267\360\355\355\267", 256, 0xffff, 0xb000));
            } else {
                map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                moveLearnedTextures[0] =
                    static_cast<std::int16_t>(CreateVerticalWindowTextSurface(map->retainedTaskD, categories[category], 256, 0xffff, 0xb000));
            }
            char name[16];
            const NativeMoveName *const record = moveNames[category] + move;
            if (category == 1 && move == 20)
                strcpy(name, "\335\323\340\375\363\370\363\341\363\374\332\270");
            else
                sprintf(name, "%s", record->name);
            AppendCentringSpaces(name, 6);
            map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            moveLearnedTextures[1] = static_cast<std::int16_t>(CreateVerticalWindowTextSurface(map->retainedTaskD, name, 256, 0xffff, 0xb000));
            moveLearnedSavedHelp = static_cast<std::uint16_t>(GetUiFlag(0));
            const char *const result =
                operation == 2 ? "\363\342\323\366\363\305\362\351\361\351\357\323\362\255" : "\363\342\363\302\340\246\357\323\361\351\357\323\362\255";
            map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            moveLearnedTextures[2] = static_cast<std::int16_t>(CreateVerticalWindowTextSurface(map->retainedTaskD, result, 256, 0xffff, 0xb000));
            if (operation == 2) {
                ExecuteAicaCommand(0xaab, 0, 0);
                moveLearnedSoundDelay = -1;
            } else {
                ExecuteAicaCommand(0x208ab, 0, 0);
                moveLearnedSoundDelay = 18;
            }
        }
    } else if (moveLearnedTask) {
        for (std::int32_t index = 0; index < 3; ++index)
            if (moveLearnedTextures[index] != -1)
                ReleaseNamedTexture_Wrapper(moveLearnedTextures[index]);
        HLib::DestroyTaskInternal(moveLearnedTask);
        moveLearnedTask = 0;
        SetHelpIconMenuFlags(moveLearnedSavedHelp);
    }
}

#else

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
    const std::string_view categoryToken =
        category == 3 && moveIndex == 21 ? "UIMOVESPEC" : MOVE_LEARNED_CATEGORY_TOKENS[category]; // @text EN: "Special move" | JA: "特殊技"
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
    const std::string_view resultToken = operation == 2 ?
        "UIMOVETAUG" :
        "UIMOVEMAST"; // @text UIMOVETAUG EN: "has been taught." | JA: "を修得しました。" ; UIMOVEMAST EN: "has been mastered." | JA: "を修得しました。"
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ReleaseMoveLearnedOwner() noexcept {
    (void)SetNewEwzoTypeForMoveLearned(1, 0, 0);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool IsMoveLearnedActive() noexcept {
    return g_moveLearnedTask != nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

WazamakiSelection GetWazamakiSelection() noexcept {
    return {.category = wazamakiPresentationCategory, .row = static_cast<std::uint8_t>(wazamakiSelectedRow), .variant = wazamakiPresentationVariant};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool SetWazamakiSelection(std::uint8_t category, std::uint8_t row) noexcept {
    if (row >= MoveDefinitions(category).size())
        return false;
    wazamakiPresentationCategory = category;
    wazamakiSelectedRow = row;
    wazamakiPresentationVariant = 0;
    if (wazamakiTask && !wazamakiLegacyState)
        RequestWazamakiPreviewMotion(category, row);
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ResetWazamakiState() {
    wazamakiTask = 0;
    wazamakiLegacyState = 0;
    for (std::int32_t index = 0; index < 8; ++index)
        wazamakiTextures[index] = -1;
    wazamaki.phase = 0;
}

// @verified - d
void QueueWazamakiTask() {
    if (!wazamakiTask) {
        wazamakiTask = HLib::EnqueueTaskWithoutParameter(WazamakiTaskCallback, 15, 0, SHEN_NAME4('W', 'Z', 'O', 'G'));
        wazamakiSavedControlEnabled = IsPlayerControlEnabled();
        if (wazamakiSavedControlEnabled)
            SetPlayerControlEnabled(0);
        UpdateWazamakiMenu();
    }
}

#else

void QueueWazamakiTask() {
    if (wazamakiTask)
        return;
    wazamakiTask = HLib::EnqueueTaskWithoutParameter(WazamakiTaskCallback, 15, 0, static_cast<std::int32_t>(WAZAMAKI_TASK.name));
    wazamakiSavedPlayerControl = plymIsSearchStartRequested();
    if (wazamakiSavedPlayerControl)
        SetPlayerControlEnabled(0);
    (void)UpdateWazamakiMenu();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void WazamakiTaskCallback(void *) {
    if (UpdateWazamakiMenu() && wazamakiTask) {
        HLib::DestroyTaskInternal(wazamakiTask);
        wazamakiTask = 0;
        if (wazamakiSavedControlEnabled)
            SetPlayerControlEnabled(1);
    }
}

} // namespace

#else

namespace {

void WazamakiTaskCallback(void *) {
    if (!UpdateWazamakiMenu() || !wazamakiTask)
        return;
    HLib::DestroyTaskInternal(wazamakiTask);
    wazamakiTask = nullptr;
    if (wazamakiSavedPlayerControl)
        SetPlayerControlEnabled(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetupWZ2_CtrlWazamaki() {
    if (wazamakiTask && wazamaki.phase == 1)
        wazamaki.phase = 3;
}

#else

// @verified - p
void SetupWZ2_CtrlWazamaki() noexcept {
    if (wazamakiTask && wazamakiPhase == 1)
        wazamakiPhase = 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool UpdateWazamakiMenu() {
    switch (wazamaki.phase) {
    case 0:
        if (InitializeLegacyWazamaki()) {
            wazamaki.phase = 1;
            ExecuteAicaCommand(0xab, 0, 0);
        } else {
            wazamaki.phase = 2;
        }
        break;
    case 1:
        UpdateLegacyWazamakiInput();
        UpdateWazamakiPreviewFrame();
        DrawLegacyWazamaki();
        break;
    case 3:
        DrawLegacyWazamaki();
    case 2:
        ReleaseLegacyWazamaki();
        wazamaki.phase = 0;
        return true;
    }
    ++wazamaki.frameCount;
    const std::int8_t ticks = wazamaki.ticks;
    if (ticks)
        wazamaki.ticks = ticks - 1;
    if (wazamaki.inputPhase == 3 || wazamaki.inputPhase == 5)
        wazamaki.alpha = 0.0f;
    else
        wazamaki.alpha += 0.25f;
    if (wazamaki.alpha > 1.0f)
        wazamaki.alpha = 1.0f;
    return false;
}

#else

bool UpdateWazamakiMenu() {
    return GetDebugForceUIEnabled(UIElement::SubMenuMoves) ? ExecuteModernWazamaki() : ExecuteLegacyWazamaki();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CloseWazamakiMenu() {
    if (wazamaki.phase) {
        DrawLegacyWazamaki();
        ReleaseLegacyWazamaki();
    }
    if (wazamakiTask) {
        HLib::DestroyTaskInternal(wazamakiTask);
        wazamakiTask = 0;
    }
    wazamaki.phase = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t IsWazamakiInactive() {
    return wazamakiLegacyState;
}

#else

std::uint32_t IsWazamakiInactive() noexcept {
    // Native IsWazamakiInactive returns the legacy branch state. The UI45 branch does
    // not write it.
    return wazamakiLegacyState;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool InitializeLegacyWazamaki() {
    wazamakiLegacyState = 1;
    wazamakiFirstVisible = wazamakiSelectedRow = 0;
    wazamaki.inputPhase = 0;
    wazamaki.pulseBrightness = 0;
    wazamaki.pulseStep = 1;
    wazamaki.frameCount = 0;
    wazamaki.alpha = 1.0f;
    wazamaki.ticks = 0;
    wazamakiSavedNpcLoadState = GetNpcNewLoadState();
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    RefreshWazamakiLearnedGroups();
    if (!LoadLegacyWazamakiTextures())
        return false;
    InitializeWazamakiNameSprites();
    shenSetClockAndCompassRenderTo(1);
    wazamaki.savedHelpTopic = helpGetCurrentIconIdx(1);
    EV_EventSetUpHelpIcon_0(0x33, 0);
    wazamaki.savedHelpFlags = static_cast<std::uint16_t>(GetUiFlag(0));
    SetHelpIconMenuFlags(0, 0);
    SetupWazamakiCamera();
    InitializeWazamakiSprites();
    LoadRyoSceneForMovesScreen();
    InitializeWazamakiDescription(&wazamakiDescription, 16.0f, 378.0f, 624.0f, 446.0f);
    RefreshWazamakiDescriptionAndMotion(&wazamakiDescription);
    return true;
}

} // namespace

#else

namespace {

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
    wazamakiSavedClockSuppressed = shenIsClockAndCompassSuppressed() ? 1U : 0U;
    shenSetClockAndCompassRenderTo(1);
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetupModernMovesCamera() {

    using enum CameraState;

    wazamakiSavedCameraState = camCtrlGetState();
    camCtrlGetNextCamPos(&wazamakiSavedCameraEye);
    camCtrlGetTargetPosition(&wazamakiSavedCameraTarget);
    camCtrlSetState(Event);
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native ExecuteModernWazamaki, m_bDebugForceUIEnabled(UI_SubMenu_Moves) branch.
// Selection and preview are owned by the UI45 replacement itself. Retail does
// not enter the WAZAHYOU/RYO_M legacy owner from this branch.
bool ExecuteModernWazamaki() {
    switch (wazamakiPhase) {
    case 0: {
        SetupModernMovesCamera();
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Character *const ryo = static_cast<Character *>(control->ryo);
        EV_SetCharDisplay(ryo, CVM_DisplayEnabled, 0);
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
        // controllerInput is pressedPriority.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ReleaseLegacyWazamaki() {
    ReleaseWazamakiPreviewModels();
    RestoreWazamakiCamera();
    SetHelpIconMenuFlags(wazamaki.savedHelpFlags, 0);
    EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(wazamaki.savedHelpTopic), 0);
    shenSetClockAndCompassRenderTo(0);
    ReleaseWazamakiNameSurfaces();
    ReleaseLegacyWazamakiTextures();
    EV_NPC_NewLoadSw(wazamakiSavedNpcLoadState);
    wazamakiLegacyState = 0;
}

} // namespace

#else

namespace {

void ReleaseLegacyWazamaki() {

    ReleaseWazamakiPreviewModels();
    SetHelpIconMenuFlags(wazamakiSavedHelpFlags);
    EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(wazamakiSavedHelpTopic), 0);
    shenSetClockAndCompassRenderTo(wazamakiSavedClockSuppressed ? 1 : 0);
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RefreshWazamakiLearnedGroups() {
    std::int32_t category, index;
    for (index = 0; index < 28; ++index) {
        wazamakiGroups[1][index].count = 0;
        wazamakiGroups[1][index].selected = 0;
    }
    for (index = 0; index < 23; ++index) {
        wazamakiGroups[2][index].count = 0;
        wazamakiGroups[2][index].selected = 0;
    }
    for (index = 0; index < 22; ++index) {
        wazamakiGroups[3][index].count = 0;
        wazamakiGroups[3][index].selected = 0;
    }
    wazamaki.groupCounts[0] = 0;
    wazamaki.groupCounts[1] = 0;
    wazamaki.groupCounts[2] = 0;
    wazamaki.groupCounts[3] = 0;
    for (category = 1; category < 4; ++category) {
        const std::int32_t count = moveDefinitionCounts[category];
        for (index = 0; index < count; ++index) {
            const NativeMoveName *const names = moveNames[category];
            NativeWazamakiGroup *const group = &wazamakiGroups[category][names[index].group];
            if (GetMoveProficiency(static_cast<std::uint8_t>(category), static_cast<std::uint8_t>(index)) > 0) {
                group->moves[group->count] = static_cast<std::uint8_t>(index);
                if (vfCheckSkillFlag(shen_name(moveRecords[category][index].skillFlag)))
                    group->selected = group->count;
                if (!group->count)
                    ++wazamaki.groupCounts[category];
                ++group->count;
            }
        }
    }
    wazamaki.groupCounts[0] = wazamaki.groupCounts[1] + wazamaki.groupCounts[2] + wazamaki.groupCounts[3];
}

} // namespace

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int32_t GetWazamakiGroupCount(std::int32_t category) noexcept {
    return wazamakiGroupCounts[static_cast<std::size_t>(category)];
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::span<const WazamakiLearnedGroup> GetWazamakiLearnedGroups() noexcept {
    CacheWazamakiGroups();
    return {wazamakiLearnedView.data(), wazamakiGroupCounts[0]};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void PlayWazamakiGroupMove(std::uint32_t row) {
    if (row >= wazamakiGroupCounts[0])
        return;
    CacheWazamakiGroups();
    const WazamakiLearnedGroup &group = wazamakiLearnedView[row];
    RequestWazamakiPreviewMotion(group.category, group.moves[group.selected]);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetMoveLearningPresentationState() noexcept {
    g_moveLearnedTask = nullptr;
    g_moveLearnedPendingMove = 0;
    g_moveLearnedPendingCategory = 0;
    moveLearningResetWord = 0;
    g_moveUiClockAndNavigationHold = 0;
    g_extraTelopTask = nullptr;
    g_extraTelopDisplayedText = nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void SetupWazamakiCamera() {
    wazamaki.savedCameraState = camCtrlGetState();
    camCtrlGetNextCamPos(&wazamaki.savedCameraEye);
    camCtrlGetTargetPosition(&wazamaki.savedCameraTarget);
    camCtrlSetState(9);
    wazamaki.savedPerspective = camCtrlGetPerspective();
    camCtrlSetPerspective(0x2aaa);
    camCtrlSetNextRoll(0);
    MapControlData *map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::int32_t yaw = -shenGetRotYFromNode(map->ryo);
    map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Vector3 position;
    GetCharacterWorldPosition(map->ryo, &position);
    position.y += 1.6f;
    camCtrlSetNextPosition(position);
    position.x += CalculateFixedSine(static_cast<std::uint32_t>(yaw)) * 0.17f;
    position.z += CalculateFixedCosine(static_cast<std::uint32_t>(yaw)) * -0.17f;
    camCtrlSetTargetPosition(position);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RestoreWazamakiCamera() {
    camCtrlSetState(wazamaki.savedCameraState);
    camCtrlSetNextPosition(wazamaki.savedCameraEye);
    camCtrlSetTargetPosition(wazamaki.savedCameraTarget);
    camCtrlSetPerspective(static_cast<std::int16_t>(wazamaki.savedPerspective));
}

} // namespace

#else

namespace {

void RestoreWazamakiCamera() {

    camCtrlSetState(wazamakiSavedCameraState);
    camCtrlSetNextPosition(wazamakiSavedCameraEye);
    camCtrlSetTargetPosition(wazamakiSavedCameraTarget);
    camCtrlSetPerspective(wazamakiSavedPerspective);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool LoadLegacyWazamakiTextures() {
    for (std::int16_t index = 0; index < 8; ++index)
        wazamakiTextures[index] = -1;
    const std::uint32_t bytes = shenOpenFile("sprite", "WAZAHYOU.SPR");
    void *const file = SafeTryLoadFile("sprite", "WAZAHYOU.SPR", shen_name(SHEN_NAME4('W', 'A', 'Z', '1')), 0x20000);
    if (!file)
        return false;
    ReadTitleSpriteTextures(file, bytes, wazamakiTextures);
    HLib::FreeTaskData(file);
    for (std::int16_t index = 0; index < 8; ++index)
        if (wazamakiTextures[index] == -1)
            return false;
    return true;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void FillScrollListMoveData(std::int32_t ordinal, ScrollListMoveData &data) {
    std::int32_t category;
    NativeWazamakiGroup *const group = FindWazamakiGroup(ordinal, &category);
    if (!group)
        return;
    const std::uint8_t move = group->moves[group->selected];
    data.text = GetStringForUILabel(MOVE_NAME_TOKENS[category][move].data());
    data.category = category;
    data.commands = WAZAMAKI_GROUP_COMMANDS[category][move].data();
    data.groupCount = group->count;
    const std::span<const S2MoveSkillBank, 3> banks = S2Save_GetMoveSkillBanks();
    data.proficiency = banks[category - 1].proficiency[move];
    data.critical = static_cast<std::int32_t>((MOVE_CRITICAL_MASKS[category - 1] >> move) & 1U);
    data.known = 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ReleaseLegacyWazamakiTextures() {
    for (std::int32_t index = 0; index < 8; ++index) {
        if (wazamakiTextures[index] != -1)
            ReleaseNamedTexture_Wrapper(wazamakiTextures[index]);
        wazamakiTextures[index] = -1;
    }
    ReleaseWazamakiDescription(&wazamakiDescription);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void InitializeWazamakiNameSprites() {
    std::int32_t category, index;
    for (category = 1; category <= 3; ++category) {
        for (index = 0; index < 40; ++index) {
            wazamakiNameSurfaces[category][index] = -1;
            wazamakiRubySurfaces[category][index] = -1;
        }
        UpdateWazamakiNameSurfaces();
    }
    wazamakiNameSprite.flags = 0;
    wazamakiNameSprite.x = 220.0f;
    wazamakiNameSprite.width = 256;
    wazamakiNameSprite.height = 32;
    wazamakiNameSprite.scaleX = 1.0f;
    wazamakiNameSprite.scaleY = 1.0f;
    wazamakiNameSprite.rotation = 0;
    wazamakiNameSprite.alignment = 0;
    wazamakiNameSprite.depth = 11.0f;
    wazamakiRubySprite.flags = 0;
    wazamakiRubySprite.width = 256;
    wazamakiRubySprite.height = 32;
    wazamakiRubySprite.scaleX = 2.0f;
    wazamakiRubySprite.scaleY = 0.5f;
    wazamakiRubySprite.rotation = 0;
    wazamakiRubySprite.alignment = 0;
    wazamakiRubySprite.depth = 11.0f;
    const char *special = "\245\250\245\363\245\252\245\246";
    wazamakiSpecialSurface = static_cast<std::int16_t>(
        SetupUIElement(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, special, 128, 0xffff, 0xf000, 0));
    wazamakiSpecialSprite.icon = static_cast<std::uint16_t>(wazamakiSpecialSurface);
    wazamakiSpecialSprite.flags = 0;
    wazamakiSpecialSprite.width = 128;
    wazamakiSpecialSprite.height = 32;
    wazamakiSpecialSprite.scaleX = 1.0f;
    wazamakiSpecialSprite.scaleY = 0.5f;
    wazamakiSpecialSprite.rotation = 0;
    wazamakiSpecialSprite.alignment = 0;
    wazamakiSpecialSprite.depth = 11.0f;
    const char *const labels[3] = {"\260\322\316\317", "\302\256\305\331", "\267\344"};
    for (index = 0; index < 3; ++index)
        wazamakiStatisticSurfaces[index] = static_cast<std::int16_t>(
            SetupUIElement(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, labels[index], 64, 0xffff, 0xf000, 1));
    wazamakiStatisticLabel.flags = 0;
    wazamakiStatisticLabel.x = 15.0f;
    wazamakiStatisticLabel.width = 64;
    wazamakiStatisticLabel.height = 32;
    wazamakiStatisticLabel.scaleX = 1.0f;
    wazamakiStatisticLabel.scaleY = 0.5f;
    wazamakiStatisticLabel.rotation = 0;
    wazamakiStatisticLabel.alignment = 0;
    wazamakiStatisticLabel.depth = -44.0f;
    for (index = 0; index < 3; ++index)
        wazamakiCriticalFrames[index] = static_cast<std::uint16_t>(wazamakiTextures[index + 5]);
    wazamakiCriticalSprite.icon = wazamakiCriticalFrames[0];
    wazamakiCriticalSprite.flags = 0;
    wazamakiCriticalSprite.width = 32;
    wazamakiCriticalSprite.height = 32;
    wazamakiCriticalSprite.scaleX = 1.0f;
    wazamakiCriticalSprite.scaleY = 1.0f;
    wazamakiCriticalSprite.rotation = 0;
    wazamakiCriticalSprite.alignment = 5;
    wazamakiCriticalSprite.depth = 11.0f;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ReleaseWazamakiNameSurfaces() {
    for (std::int32_t category = 1; category <= 3; ++category)
        for (std::int32_t move = 0; move < 40; ++move) {
            if (wazamakiNameSurfaces[category][move] != -1)
                ReleaseNamedTexture_Wrapper(wazamakiNameSurfaces[category][move]);
            wazamakiNameSurfaces[category][move] = -1;
            if (wazamakiRubySurfaces[category][move] != -1)
                ReleaseNamedTexture_Wrapper(wazamakiRubySurfaces[category][move]);
            wazamakiRubySurfaces[category][move] = -1;
        }
    if (wazamakiSpecialSurface != -1)
        ReleaseNamedTexture_Wrapper(wazamakiSpecialSurface);
    wazamakiSpecialSurface = -1;
    for (std::int32_t index = 0; index < 3; ++index) {
        if (wazamakiStatisticSurfaces[index] != -1)
            ReleaseNamedTexture_Wrapper(wazamakiStatisticSurfaces[index]);
        wazamakiStatisticSurfaces[index] = -1;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void UpdateWazamakiNameSurfaces() {
    NativeWazamakiGroupPosition position;
    std::int32_t row;
    const NativeWazamakiGroup *group;
    const NativeMoveName *definitions;
    std::int32_t visible, variant, category;
    for (row = 0; row < wazamaki.groupCounts[0]; ++row) {
        group = FindWazamakiGroup(row, &position);
        category = position.category;
        definitions = moveNames[category];
        if (row < wazamakiFirstVisible || row >= wazamakiFirstVisible + 8)
            visible = 0;
        else
            visible = 1;
        for (variant = 0; variant < group->count; ++variant) {
            std::int16_t *const name = &wazamakiNameSurfaces[category][group->moves[variant]];
            wazamakiNameLengths[category][group->moves[variant]] =
                static_cast<std::int16_t>(static_cast<float>(strlen(definitions[group->moves[variant]].name)) * 0.5f);
            std::int16_t *const ruby = &wazamakiRubySurfaces[category][group->moves[variant]];
            if (!visible) {
                if (*name != -1) {
                    ReleaseNamedTexture(*name);
                    *name = -1;
                }
                if (*ruby != -1) {
                    ReleaseNamedTexture(*ruby);
                    *ruby = -1;
                }
            } else {
                if (*name == -1)
                    *name = static_cast<std::int16_t>(SetupUIElement(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD,
                        definitions[group->moves[variant]].name, 256, 0xffff, 0xf000, 1));
                if (*ruby == -1)
                    *ruby = static_cast<std::int16_t>(SetupUIElement(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD,
                        definitions[group->moves[variant]].ruby, 512, 0xffff, 0xf000, 0));
            }
        }
    }
}

} // namespace

#else

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void InitializeWazamakiSprites() {
    wazamakiBackgroundSprite.icon = static_cast<std::uint16_t>(wazamakiTextures[0]);
    wazamakiBackgroundSprite.flags = 0;
    wazamakiBackgroundSprite.x = 63.0f;
    wazamakiBackgroundSprite.y = 63.0f;
    wazamakiBackgroundSprite.width = 128;
    wazamakiBackgroundSprite.height = 128;
    wazamakiBackgroundSprite.scaleX = 1.0f;
    wazamakiBackgroundSprite.scaleY = 1.0f;
    wazamakiBackgroundSprite.rotation = 0;
    wazamakiBackgroundSprite.alignment = 5;
    wazamakiBackgroundSprite.depth = -50.0f;
    wazamakiDividerBar.y = 208.0f;
    wazamakiDividerBar.width = 2;
    wazamakiDividerBar.height = 320;
    wazamakiDividerBar.scaleX = 1.0f;
    wazamakiDividerBar.scaleY = 1.0f;
    wazamakiDividerBar.colour = 0x4f0000ffU;
    wazamakiDividerBar.rotation = 0;
    wazamakiDividerBar.depth = -49.0f;
    wazamakiDividerBar.alignment = 0;
    wazamakiStatisticBar.x = 75.0f;
    wazamakiStatisticBar.scaleX = 1.0f;
    wazamakiStatisticBar.scaleY = 1.0f;
    wazamakiStatisticBar.depth = -45.0f;
    wazamakiStatisticBar.colour = 0x5000ff05U;
    wazamakiStatisticBar.height = 12;
    wazamakiStatisticBar.rotation = 0;
    wazamakiStatisticBar.alignment = 0;
    wazamakiRowBar.x = 228.0f;
    wazamakiRowBar.scaleX = 1.0f;
    wazamakiRowBar.scaleY = 1.0f;
    wazamakiRowBar.depth = -47.0f;
    wazamakiRowBar.height = 24;
    wazamakiRowBar.rotation = 0;
    wazamakiRowBar.alignment = 0;
    wazamakiInputSprite.flags = 0;
    wazamakiInputSprite.width = 32;
    wazamakiInputSprite.height = 32;
    wazamakiInputSprite.scaleX = 0.7f;
    wazamakiInputSprite.scaleY = 0.7f;
    wazamakiInputSprite.rotation = 0;
    wazamakiInputSprite.alignment = 5;
    wazamakiInputSprite.depth = 11.0f;
    const std::int32_t textures[4] = {1, 2, 3, 4};
    for (std::int32_t index = 0; index < 4; ++index) {
        LegacyIconSprite &sprite = wazamakiScrollSprites[index];
        sprite.icon = static_cast<std::uint16_t>(wazamakiTextures[textures[index]]);
        sprite.alignment = 0;
        sprite.depth = -46.0f;
        sprite.width = 16;
        sprite.height = 16;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void DrawLegacyWazamaki() {
    for (std::int32_t column = 0; column < 5; ++column) {
        wazamakiBackgroundSprite.x = static_cast<float>(column * 128 + 64);
        for (std::int32_t row = 0; row < 4; ++row) {
            wazamakiBackgroundSprite.y = static_cast<float>(row * 128 + 64);
            PrepareDrawTextureWithMode(wazamakiBackgroundSprite);
        }
    }
    DrawWazamakiStatistics();
    DrawWazamakiRowScrollbar();
    DrawWazamakiDescription(&wazamakiDescription);
    DrawWazamakiPreview();
    DrawWazamakiRows();
    DrawWazamakiButtons();
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void WazamakiLoadingCompleteNullsub() {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void DrawWazamakiStatistics() {
    NativeWazamakiGroupPosition position;
    const NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &position);
    if (!group || !group->count)
        return;
    const std::uint16_t motion = static_cast<std::uint16_t>(GetMoveMotion(static_cast<std::uint8_t>(position.category), group->moves[group->selected]));
    float power, speed, rigid;
    power = speed = rigid = 0.0f;
    if (MOTN_GetMarkerData(motion)) {
        const std::byte *timing, *record = MOTN_FindAnimationExtraRecord(motion, 4);
        if (record) {
            power = static_cast<float>(static_cast<std::int8_t>(record[3]));
            timing = MOTN_FindAnimationExtraRecord(motion, 2);
            if (timing) {
                const std::int16_t *const frames = reinterpret_cast<const std::int16_t *>(timing);
                speed = static_cast<float>(frames[1]);
                rigid = static_cast<float>(frames[3]) - static_cast<float>(frames[2]);
            }
        } else {
            record = MOTN_FindAnimationExtraRecord(motion, 5);
            if (record) {
                power = static_cast<float>(static_cast<std::int8_t>(record[1]));
                timing = MOTN_FindAnimationExtraRecord(motion, 2);
                if (timing) {
                    speed = static_cast<float>(reinterpret_cast<const std::int16_t *>(timing)[1]);
                    const std::uint16_t follow = *reinterpret_cast<const std::uint16_t *>(record + 2);
                    if (MOTN_GetMarkerData(follow)) {
                        const std::byte *const followTiming = MOTN_FindAnimationExtraRecord(follow, 2);
                        if (followTiming)
                            rigid = 30.0f -
                                (static_cast<float>(reinterpret_cast<const std::int16_t *>(followTiming)[3]) -
                                    static_cast<float>(reinterpret_cast<const std::int16_t *>(timing)[3]));
                        if (1.0f > rigid)
                            rigid = 1.0f;
                    }
                }
            }
        }
    }
    if (power > 100.0f)
        power = 100.0f;
    if (speed > 20.0f)
        speed = 20.0f;
    if (rigid > 30.0f)
        rigid = 30.0f;
    wazamakiStatisticBar.y = 304.0f;
    wazamakiStatisticBar.width = static_cast<std::int16_t>(power / 100.0f * 80.0f);
    DrawLegacyColourRect(wazamakiStatisticBar);
    wazamakiStatisticBar.y += 20.0f;
    wazamakiStatisticBar.width = static_cast<std::int16_t>((1.0f - speed / 20.0f) * 80.0f);
    DrawLegacyColourRect(wazamakiStatisticBar);
    wazamakiStatisticBar.y += 20.0f;
    wazamakiStatisticBar.width = static_cast<std::int16_t>(rigid / 30.0f * 80.0f);
    DrawLegacyColourRect(wazamakiStatisticBar);
    wazamakiStatisticLabel.y = 304.0f;
    wazamakiStatisticLabel.icon = static_cast<std::uint16_t>(wazamakiStatisticSurfaces[0]);
    PrepareDrawTextureWithMode(wazamakiStatisticLabel);
    wazamakiStatisticLabel.y += 20.0f;
    wazamakiStatisticLabel.icon = static_cast<std::uint16_t>(wazamakiStatisticSurfaces[1]);
    PrepareDrawTextureWithMode(wazamakiStatisticLabel);
    wazamakiStatisticLabel.y += 20.0f;
    wazamakiStatisticLabel.icon = static_cast<std::uint16_t>(wazamakiStatisticSurfaces[2]);
    PrepareDrawTextureWithMode(wazamakiStatisticLabel);
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void DrawWazamakiRowScrollbar() {
    const std::int32_t total = wazamaki.groupCounts[0];
    const float start = static_cast<float>(wazamakiFirstVisible) / static_cast<float>(total);
    const float end = static_cast<float>(wazamakiFirstVisible + 8) / static_cast<float>(total);
    DrawWazamakiScrollbar(168.0f, 40.0f, 168.0f, 352.0f, start, end);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void DrawWazamakiScrollbar(float x, float top, float, float bottom, float first, float last) {
    LegacyIconSprite *sprite;
    std::int32_t index;
    for (index = 0; index < 4; ++index) {
        sprite = &wazamakiScrollSprites[index];
        sprite->flags = 0;
        sprite->x = x;
        sprite->y = top;
        sprite->scaleX = 1.0f;
        sprite->scaleY = 1.0f;
        sprite->rotation = 0;
    }
    sprite = &wazamakiScrollSprites[1];
    sprite->y = top + 16.0f;
    sprite->scaleY = (bottom - sprite->y) * 0.0625f;
    PrepareDrawTextureWithMode(*sprite);
    sprite = &wazamakiScrollSprites[0];
    PrepareDrawTextureWithMode(*sprite);
    sprite->flags = 0x200;
    sprite->y = bottom;
    PrepareDrawTextureWithMode(*sprite);
    first = 0.0f > first ? 0.0f : first > 1.0f ? 1.0f : first;
    last = 0.0f > last ? 0.0f : last > 1.0f ? 1.0f : last;
    if (!(last > first))
        return;
    const float start = top + 8.0f;
    bottom -= 8.0f;
    top = (bottom - start) * first + start;
    bottom = (bottom - start) * last + start;
    sprite = &wazamakiScrollSprites[3];
    sprite->y = top + 8.0f;
    sprite->scaleY = (bottom - top) * 0.0625f;
    PrepareDrawTextureWithMode(*sprite);
    sprite = &wazamakiScrollSprites[2];
    sprite->y = top;
    PrepareDrawTextureWithMode(*sprite);
    sprite->flags = 0x200;
    sprite->y = bottom;
    PrepareDrawTextureWithMode(*sprite);
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void InitializeWazamakiDescription(NativeWazamakiDescription *description, float left, float top, float right, float bottom) {
    const float x = (left + right) * 0.5f, y = (top + bottom) * 0.5f;
    description->x = x;
    description->y = y;
    description->width = static_cast<std::int32_t>(fabs(right - left));
    description->height = static_cast<std::int32_t>(fabs(bottom - top));
    description->scaleX = 1.0f;
    description->scaleY = 0.5f;
    description->textX = left + 28.0f;
    description->textY = top + 16.0f;
    description->columns = static_cast<std::int32_t>(static_cast<float>(description->width) / 28.0f - 2.0f);
    description->rows = static_cast<std::int32_t>(static_cast<float>(description->height) * 0.0625f - 2.0f);
    if (description->rows > 8)
        description->rows = 8;
    for (std::int32_t row = 0; row < description->rows; ++row)
        for (std::int32_t half = 0; half < 2; ++half)
            description->textures[row][half] = -1;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateWazamakiDescriptionText(NativeWazamakiDescription *description, const char *text) {
    std::int32_t line = 0, half = 0;
    const std::int32_t lengths[2] = {32, 8}, widths[2] = {512, 128};
    char part[36], buffer[256];
    strncpy(buffer, text, 256);
    ReleaseWazamakiDescription(description);
    char *token = strtok(buffer, "#");
    if (token)
        while (line < description->rows) {
            const std::int32_t length = lengths[half];
            strncpy(part, token, length);
            part[length] = 0;
            description->textures[line][half] =
                SetupUIElement(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, part, widths[half], 0xffff, 0xf000, 1);
            if (static_cast<std::int32_t>(strlen(token)) <= length) {
                ++line;
                half = 0;
                token = strtok(0, "#");
                if (!token)
                    break;
            } else {
                half ^= 1;
                token += length;
                if (!half)
                    ++line;
            }
        }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void DrawWazamakiDescription(const NativeWazamakiDescription *description) {
    shenSubmitQuadsUI(description->x, description->y, 0x7f000000U, 0x7fffffffU, description->width, description->height, 10.0f);
    LegacyIconSprite sprites[2];
    std::int32_t half, row;
    for (half = 0; half < 2; ++half) {
        LegacyIconSprite &sprite = sprites[half];
        sprite.x = half ? description->textX + 420.0f : description->textX;
        sprite.y = description->textY;
        sprite.scaleX = description->scaleX;
        sprite.scaleY = description->scaleY;
        sprite.width = half ? 128 : 512;
        sprite.height = 32;
        sprite.flags = 0;
        sprite.alignment = 0;
        sprite.rotation = 0;
        sprite.depth = 11.0f;
    }
    for (row = 0; row < description->rows; ++row)
        for (half = 0; half < 2; ++half) {
            if (description->textures[row][half] != -1) {
                sprites[half].icon = static_cast<std::uint16_t>(description->textures[row][half]);
                PrepareDrawTextureWithMode(sprites[half]);
            }
            sprites[half].y += 16.0f;
        }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ReleaseWazamakiDescription(NativeWazamakiDescription *description) {
    for (std::int32_t row = 0; row < 8; ++row)
        for (std::int32_t half = 0; half < 2; ++half) {
            if (description->textures[row][half] != -1)
                ReleaseNamedTexture_Wrapper(description->textures[row][half]);
            description->textures[row][half] = -1;
        }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RefreshWazamakiDescriptionAndMotion(NativeWazamakiDescription *description) {
    NativeWazamakiGroupPosition position;
    const NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &position);
    const std::int32_t category = position.category;
    const std::int32_t move = group->moves[group->selected];
    const char *const text = moveDescriptions[category][move];
    char buffer[256];
    char *cursor = buffer;
    if (static_cast<std::int32_t>(strlen(text)) <= 0)
        return;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(strlen(text)); ++index) {
        if (static_cast<std::uint8_t>(text[index] - '0') <= 9) {
            const std::int32_t phrase = text[index + 1] - '0' + (text[index] - '0') * 10;
            strcpy(cursor, moveDescriptionPhrases[phrase]);
            cursor += strlen(moveDescriptionPhrases[phrase]);
            ++index;
        } else {
            strncpy(cursor, text + index, 1);
            ++cursor;
        }
    }
    memcpy(cursor, "\241\243", 3);
    cursor[2] = 0;
    UpdateWazamakiDescriptionText(description, buffer);
    RequestWazamakiPreviewMotion(category, move);
}

} // namespace

#else

namespace {

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
    RequestWazamakiPreviewMotion(static_cast<std::uint8_t>(category), id);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateLegacyWazamakiInput() {
    NativeWazamakiGroupPosition position;
    const NativeWazamakiGroup *group;
    std::int32_t row;
    std::uint16_t pressed;
    std::uint16_t directional;
    if (g_loadingFlag & 2U) {
        pressed = 0;
        directional = 0;
    } else {
        pressed = g_inputBuffer[0].pressedPriority;
        directional = RepeatWazamakiInput(pressed, g_inputBuffer[0].held);
    }
    switch (wazamaki.inputPhase) {
    case 0:
        if (directional & 0x1000U) {
            if (wazamakiSelectedRow > 0 && !wazamaki.ticks) {
                wazamaki.ticks = 3;
                ExecuteAicaCommand(0x8ab, 0, 0);
            }
            for (row = wazamakiSelectedRow; row > 0; --row) {
                group = FindWazamakiGroup(row - 1, &position);
                if (group && group->count) {
                    wazamakiSelectedRow = row - 1;
                    RefreshWazamakiDescriptionAndMotion(&wazamakiDescription);
                    break;
                }
            }
            if (wazamakiSelectedRow < wazamakiFirstVisible) {
                wazamakiFirstVisible = wazamakiSelectedRow;
                UpdateWazamakiNameSurfaces();
            }
        } else if (directional & 0x2000U) {
            if (wazamakiSelectedRow < wazamaki.groupCounts[0] - 1 && !wazamaki.ticks) {
                wazamaki.ticks = 3;
                ExecuteAicaCommand(0x8ab, 0, 0);
            }
            for (row = wazamakiSelectedRow + 1; row < wazamaki.groupCounts[0]; ++row) {
                group = FindWazamakiGroup(row, &position);
                if (group && group->count) {
                    wazamakiSelectedRow = row;
                    RefreshWazamakiDescriptionAndMotion(&wazamakiDescription);
                    break;
                }
            }
            if (wazamakiSelectedRow >= wazamakiFirstVisible + 8) {
                wazamakiFirstVisible = wazamakiSelectedRow - 7;
                UpdateWazamakiNameSurfaces();
            }
        }
        if (FindWazamakiGroup(wazamakiSelectedRow, &position)->count > 1) {
            if (pressed & 0x400U) {
                wazamaki.inputPhase = 1;
                wazamaki.transitionFrames = 8;
                ExecuteAicaCommand(0xab, 0, 0);
            } else if (pressed & 0x8000U) {
                wazamaki.inputPhase = 5;
                wazamaki.transitionFrames = 8;
                wazamaki.reverseVariant = 0;
                ExecuteAicaCommand(0xab, 0, 0);
            } else if (pressed & 0x4000U) {
                wazamaki.inputPhase = 5;
                wazamaki.transitionFrames = 8;
                wazamaki.reverseVariant = 1;
                ExecuteAicaCommand(0xab, 0, 0);
            }
        }
        if (pressed & 0x200U)
            wazamaki.phase = 3;
        break;
    case 1:
        if (!--wazamaki.transitionFrames)
            wazamaki.inputPhase = 2;
        break;
    case 4:
        if (!--wazamaki.transitionFrames)
            wazamaki.inputPhase = 0;
        break;
    case 2:
        if (pressed & 0x600U) {
            wazamaki.inputPhase = 4;
            wazamaki.transitionFrames = 8;
            ExecuteAicaCommand(0xab, 0, 0);
        } else if (pressed & 0xa000U) {
            wazamaki.inputPhase = 3;
            wazamaki.transitionFrames = 8;
            wazamaki.reverseVariant = 0;
            ExecuteAicaCommand(0x108ab, 0, 0);
        } else if (pressed & 0x5000U) {
            wazamaki.inputPhase = 3;
            wazamaki.transitionFrames = 8;
            wazamaki.reverseVariant = 1;
            ExecuteAicaCommand(0x108ab, 0, 0);
        }
        break;
    case 3:
        if (!--wazamaki.transitionFrames) {
            SelectCurrentLegacyVariant(FindWazamakiGroup(wazamakiSelectedRow, &position), position.category);
            RefreshWazamakiDescriptionAndMotion(&wazamakiDescription);
            wazamaki.inputPhase = 2;
        }
        break;
    case 5:
        if (!--wazamaki.transitionFrames) {
            SelectCurrentLegacyVariant(FindWazamakiGroup(wazamakiSelectedRow, &position), position.category);
            RefreshWazamakiDescriptionAndMotion(&wazamakiDescription);
            wazamaki.inputPhase = 0;
        }
        break;
    }
    wazamakiPreview.previousAnalogX = g_inputBuffer[0].leftStickX;
    wazamakiPreview.previousAnalogY = g_inputBuffer[0].leftStickY;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void LoadRyoSceneForMovesScreen() {
    wazamakiPreview.ryoModel = 0;
    wazamakiPreview.ryoRoot = 0;
    wazamakiPreview.character = 0;
    wazamakiPreview.groundModel = 0;
    wazamakiPreview.groundRoot = 0;
    char directory[32];
    sprintf(directory, "scene/%02d/model/chara", static_cast<int>(g_actNumber));
    void *file = shenLoadFile(directory, "RYO_M.MT7", shen_name(SHEN_NAME4('W', 'Z', 'M', 'D')));
    if (!file)
        return;
    MT7Model *model = InitializeMT7Model(file);
    if (!model || !model->root)
        return;
    wazamakiPreview.ryoModel = model;
    wazamakiPreview.ryoRoot = model->root->child->child;
    file = shenLoadFile("model/item", "WAZ0508G.MT7", shen_name(SHEN_NAME4('W', 'Z', 'M', 'Z')));
    if (!file) {
        ReleaseWazamakiPreviewModels();
        return;
    }
    model = InitializeMT7Model(file);
    if (!model || !model->root) {
        ReleaseWazamakiPreviewModels();
        return;
    }
    wazamakiPreview.groundModel = model;
    wazamakiPreview.groundRoot = model->root;
    Character *const character = AddCharacterHandler(3);
    if (!character) {
        ReleaseWazamakiPreviewModels();
        return;
    }
    wazamakiPreview.character = character;
    MapControlData *map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CreateScenarioCharacterId(map->scenarioTask, shen_name(SHEN_NAME4('M', 'M', 'J', 'L')), character);
    map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    AllocateModelImageLevel(map->modelManagerTask, "WZMD", directory, "RYO_M.MT7");
    SetupImageModuleHandlers(character);
    map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetupImageModule(character, map->modelManagerTask, "WZMD");
    mtAssignClothFlags(character, 0xff);
    mtwkCreateForChara(character, 0);
    AttachReflectorBoardComponent(character, 1);
    SetCharacterAlpha(character, 1.0f);
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
    mtwkSetExtraFlag(charFindMotionwork(character), 6, 2);
    mtwkSetUseEnvFlag(0);
    charaToggleFlag(character, 0);
    wazamakiPreview.scale = 1.0f;
    UpdateWazamakiPreviewTransform();
    wazamakiPreview.pitch = 0;
    wazamakiPreview.yaw = 0x8000 - wazamakiPreview.baseYaw;
    wazamakiPreview.roll = 0;
    wazamakiPreview.retained78 = 0;
    wazamakiPreview.previousAnalogX = 0x80;
    wazamakiPreview.previousAnalogY = 0x80;
    NativeWazamakiGroupPosition position;
    const NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &position);
    RequestWazamakiPreviewMotion(position.category, group->moves[group->selected]);
    g_worldDrawingEnabled = 0;
    wazamakiPreview.frameDelay = 1;
}

} // namespace

#else

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
    EV_SetCharDisplay(wazamakiPreviewCharacter, CVM_DisplayEnabled, 0);
    mtwkSetExtraFlag(FindMotionWork(wazamakiPreviewCharacter), 6, 2);
    mtwkSetEnvironmentEnabled(0);
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

WazamakiLearnedGroup *CurrentLearnedGroup() {
    CacheWazamakiGroups();
    return wazamakiSelectedRow < wazamakiGroupCounts[0] ? &wazamakiLearnedView[wazamakiSelectedRow] : nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ReleaseWazamakiPreviewModels() {
    LcdmInit(0);
    g_worldDrawingEnabled = 1;
    mtwkSetUseEnvFlag(1);
    if (wazamakiPreview.character) {
        EV_SetCharDisplay(wazamakiPreview.character, CVM_DisplayEnabled, 0);
        MarkCharacterForCleanup(wazamakiPreview.character, 1);
    }
    wazamakiPreview.character = 0;
    if (wazamakiPreview.ryoModel)
        ReleaseMT7Model(wazamakiPreview.ryoModel);
    if (wazamakiPreview.groundModel)
        ReleaseMT7Model(wazamakiPreview.groundModel);
    wazamakiPreview.ryoModel = 0;
    wazamakiPreview.ryoRoot = 0;
    wazamakiPreview.groundModel = 0;
    wazamakiPreview.groundRoot = 0;
    wazamakiPreview.frameDelay = -1;
}

} // namespace

#else

void ReleaseWazamakiPreviewModels() {

    LcdmInit(0);
    g_worldDrawingEnabled = true;
    mtwkSetEnvironmentEnabled(1);
    if (wazamakiPreviewCharacter) {
        EV_SetCharDisplay(wazamakiPreviewCharacter, CVM_DisplayEnabled, 0);
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateWazamakiPreviewFrame() {
    if (!wazamakiPreview.character)
        return;
    if (wazamakiPreview.frameDelay >= 0 && --wazamakiPreview.frameDelay < 0)
        EV_SetCharDisplay(wazamakiPreview.character, CVM_DisplayEnabled, 1);
    wazamakiPreview.scale = 1.0f;
    UpdateWazamakiPreviewTransform();
}

} // namespace

#else

void UpdateWazamakiPreviewFrame() {

    if (!wazamakiPreviewCharacter)
        return;
    if (wazamakiPreviewFrameDelay >= 0 && --wazamakiPreviewFrameDelay < 0)
        EV_SetCharDisplay(wazamakiPreviewCharacter, CVM_DisplayEnabled, 0);
    wazamakiPreviewScale = wazamakiPreviewDefaultScale;
    if (!(g_loadingFlag & 2U))
        UpdateLegacyPreviewTransform();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint32_t WazamakiMatrixUpdateNullsub() {
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateWazamakiPreviewTransform() {
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    wazamakiPreview.baseYaw = camCtrlGetNextRotY() - 28413;
    wazamakiPreview.position.x = CalculateFixedSine(static_cast<std::uint32_t>(wazamakiPreview.baseYaw)) * 7.5f + eye.x;
    wazamakiPreview.position.z = CalculateFixedCosine(static_cast<std::uint32_t>(wazamakiPreview.baseYaw)) * 7.5f + eye.z;
    wazamakiPreview.position.y = eye.y - 0.425f + 0.05f;
    SetCharacterWorldPosition(wazamakiPreview.character, &wazamakiPreview.position);
    shenSetRotY(wazamakiPreview.character, 0x4000);
    const std::int32_t analogX = wazamakiPreview.previousAnalogX - 128, targetYaw = (analogX << 8) + wazamakiPreview.baseYaw;
    if (wazamakiPreview.previousAnalogX != 128) {
        if ((static_cast<std::int32_t>(static_cast<float>(analogX * 8192) / 45.0f) & 0xffff) > 0x8000) {
            wazamakiPreview.yaw -= 364;
            if (wazamakiPreview.yaw <= targetYaw)
                wazamakiPreview.yaw = targetYaw;
        } else {
            wazamakiPreview.yaw += 364;
            if (wazamakiPreview.yaw >= targetYaw)
                wazamakiPreview.yaw = targetYaw;
        }
    } else
        wazamakiPreview.yaw = targetYaw;
    const std::int32_t analogY = wazamakiPreview.previousAnalogY - 128, targetPitch = analogY << 7;
    if (wazamakiPreview.previousAnalogY != 128) {
        if ((static_cast<std::int32_t>(static_cast<float>(analogY * 8192) / 45.0f) & 0xffff) > 0x8000) {
            wazamakiPreview.pitch -= 364;
            if (wazamakiPreview.pitch <= targetPitch)
                wazamakiPreview.pitch = targetPitch;
        } else {
            wazamakiPreview.pitch += 364;
            if (wazamakiPreview.pitch >= targetPitch)
                wazamakiPreview.pitch = targetPitch;
        }
    } else
        wazamakiPreview.pitch = targetPitch;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void DrawWazamakiPreview() {
    const std::int32_t pitch = wazamakiPreviewMotionYaw ? wazamakiPreview.pitch : -wazamakiPreview.pitch;
    Vector4 colour;
    colour.x = 1.0f;
    colour.y = 0.4f;
    colour.z = 0.4f;
    colour.w = 0.4f;
    ResetReflectorBoardMode(wazamakiPreview.character, 1);
    SetReflectorBoardValues(wazamakiPreview.character, &colour);
    matrix_stack::Matrix4x4_Push(0);
    dc::sdk::PushGraphicsWorkspace();
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(wazamakiPreview.position.x, wazamakiPreview.position.y + wazamakiPreview.scale, wazamakiPreview.position.z);
    Matrix4x4::SetViewMatrixFromEuler(pitch, wazamakiPreview.yaw + wazamakiPreviewMotionYaw, wazamakiPreview.roll);
    Matrix4x4::Scale(wazamakiPreview.scale, wazamakiPreview.scale, wazamakiPreview.scale);
    Matrix4x4::preTranslate(0.0f, 0.0f, 0.0f);
    matrix_stack::GetMatrix(&wazamakiPreview.rootTransform);
    PopCurrentMatrix(1);
    dc::sdk::PopGraphicsWorkspace();
    mtwkSetRootTransformOverride(charFindMotionwork(wazamakiPreview.character), &wazamakiPreview.rootTransform);
    ExecuteCharacterCoreDraw(wazamakiPreview.character);
    matrix_stack::Matrix4x4_Push(0);
    dc::sdk::PushGraphicsWorkspace();
    Matrix4x4::preTranslate(wazamakiPreview.position.x, wazamakiPreview.position.y + wazamakiPreview.scale, wazamakiPreview.position.z);
    Matrix4x4::SetViewMatrixFromEuler(pitch, wazamakiPreview.yaw + wazamakiPreviewMotionYaw, wazamakiPreview.roll);
    Matrix4x4::Scale(wazamakiPreview.scale, wazamakiPreview.scale, wazamakiPreview.scale);
    Matrix4x4::preTranslate(0.0f, -1.0f, 0.0f);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    DrawCharacterModelBranch(wazamakiPreview.groundRoot);
#else
    reinterpret_cast<void (*)(MT7Node *)>(DrawCharacterModelBranch)(wazamakiPreview.groundRoot);
#endif
    PopCurrentMatrix(1);
    dc::sdk::PopGraphicsWorkspace();
}

} // namespace

#else

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
    mtwkSetRootTransformOverride(FindMotionWork(wazamakiPreviewCharacter), &wazamakiPreviewRootTransform);
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void PrepareDrawTextureWithSecondaryColour(const LegacyIconSprite &sprite, std::uint32_t secondaryColour);
void PrepareDrawTextureWithColour(const LegacyIconSprite &sprite, std::uint32_t colour);

namespace {

void DrawWazamakiRows() {
    float xs[4], ys[4];
    NativeWazamakiGroupPosition position;
    std::int32_t ordinal, row, lastRow, extraRows, category, move;
    std::uint32_t fadeColour, inverseColour;
    float y, advance, selectedY, fade;
    std::int32_t special, drawName;
    const NativeWazamakiGroup *group;
    std::int32_t nextMove, variant, index;
    float remaining, expansion, scale, proficiency;
    y = 48.0f;
    selectedY = 68.0f;
    lastRow = wazamakiFirstVisible + 7;
    group = FindWazamakiGroup(wazamakiSelectedRow, &position);
    extraRows = group->count - 1;
    for (ordinal = 0; ordinal <= 7; ++ordinal) {
        advance = 40.0f;
        row = wazamakiFirstVisible + ordinal;
        if (row == wazamakiSelectedRow)
            selectedY = y + 20.0f;
        remaining = static_cast<float>(wazamaki.transitionFrames) * 0.125f;
        switch (wazamaki.inputPhase) {
        case 2:
        case 3:
            expansion = 1.0f;
            fade = 1.0f;
            break;
        case 1:
            expansion = 1.0f - remaining;
            fade = 1.0f - remaining;
            break;
        case 4:
            expansion = remaining;
            fade = remaining;
            break;
        case 5:
            expansion = 0.0f;
            fade = remaining;
            break;
        default:
            expansion = 0.0f;
            fade = 1.0f;
            break;
        }
        const std::uint32_t fadeValue = static_cast<std::uint32_t>(static_cast<std::int32_t>(fade * 255.0f)) << 24 | 0xffffffU;
        fadeColour = fadeValue;
        const std::uint32_t inverseValue = static_cast<std::uint32_t>(static_cast<std::int32_t>((1.0f - fade) * 255.0f)) << 24 | 0xffffffU;
        inverseColour = inverseValue;
        scale = 1.0f;
        if (wazamakiSelectedRow > lastRow - (extraRows + 1)) {
            if (row < wazamakiFirstVisible + extraRows) {
                advance = (scale - expansion) * 40.0f;
                scale -= expansion;
            }
            if (row == wazamakiSelectedRow) {
                advance = expansion * 40.0f;
                advance = advance * static_cast<float>(extraRows) + 40.0f;
            }
        } else {
            if (row == wazamakiSelectedRow + 1)
                y += expansion * 40.0f * static_cast<float>(extraRows);
            if (row > lastRow - extraRows) {
                advance = (1.0f - expansion) * 40.0f;
                scale = 1.0f - expansion;
            }
        }
        if (0.01f > scale)
            continue;
        group = FindWazamakiGroup(row, &position);
        category = position.category;
        move = group->moves[group->selected];
        special = category == 3 && move == 20 ? 1 : 0;
        if (!wazamaki.reverseVariant)
            nextMove = group->moves[(group->selected + 1 + group->count) % group->count];
        else
            nextMove = group->moves[(group->selected - 1 + group->count) % group->count];
        if (!group->count) {
            y += advance;
            continue;
        }
        proficiency = static_cast<float>(GetMoveProficiency(static_cast<std::uint8_t>(category), static_cast<std::uint8_t>(move)));
        if ((wazamaki.inputPhase == 3 || wazamaki.inputPhase == 5) && row == wazamakiSelectedRow)
            proficiency += (static_cast<float>(GetMoveProficiency(static_cast<std::uint8_t>(category), static_cast<std::uint8_t>(nextMove))) - proficiency) *
                (1.0f - static_cast<float>(wazamaki.transitionFrames) * 0.125f);
        if (12.0f > proficiency)
            wazamakiRowBar.colour = 0x7f507daeU;
        else if (proficiency == 200.0f)
            wazamakiRowBar.colour = 0x4ff03122U;
        else
            wazamakiRowBar.colour = 0x7fffaa54U;
        wazamakiRowBar.width = static_cast<std::int16_t>(proficiency / 200.0f * 356.0f);
        wazamakiRowBar.y = y + 8.0f;
        if (!wazamakiRowBar.width)
            wazamakiRowBar.width = 2;
        wazamakiRowBar.scaleY = scale;
        DrawLegacyColourRect(wazamakiRowBar);
        drawName = 0;
        wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category][move]);
        wazamakiNameSprite.y = y + 4.0f;
        wazamakiNameSprite.scaleX = 1.0f;
        wazamakiNameSprite.scaleY = scale;
        wazamakiRubySprite.icon = static_cast<std::uint16_t>(wazamakiRubySurfaces[category][move]);
        wazamakiRubySprite.x = static_cast<float>(wazamakiNameLengths[category][move] * 28 + 224);
        wazamakiRubySprite.y = y + 4.0f + 32.0f * scale * 0.5f;
        wazamakiRubySprite.scaleX = 2.0f;
        wazamakiRubySprite.scaleY = 0.5f * scale;
        if (special) {
            wazamakiSpecialSprite.x = wazamakiRubySprite.x;
            wazamakiSpecialSprite.y = y + 6.0f;
            wazamakiSpecialSprite.scaleX = 1.0f;
            wazamakiSpecialSprite.scaleY = 0.5f * scale;
        }
        switch (wazamaki.inputPhase) {
        case 5:
            if (row == wazamakiSelectedRow) {
                const float outgoing = fade, incoming = 1.0f - outgoing;
                if (!wazamaki.reverseVariant) {
                    const float width = static_cast<float>(wazamakiNameLengths[category][nextMove] * 28);
                    wazamakiNameSprite.x = width * incoming + 220.0f;
                    wazamakiNameSprite.scaleX = outgoing;
                    PrepareDrawTextureWithColour(wazamakiNameSprite, fadeColour);
                    wazamakiNameSprite.x = 220.0f;
                    wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category][nextMove]);
                    wazamakiNameSprite.scaleX = incoming;
                    PrepareDrawTextureWithColour(wazamakiNameSprite, inverseColour);
                } else {
                    wazamakiNameSprite.x = 220.0f;
                    wazamakiNameSprite.scaleX = outgoing;
                    PrepareDrawTextureWithColour(wazamakiNameSprite, fadeColour);
                    wazamakiNameSprite.x = static_cast<float>(wazamakiNameLengths[category][move] * 28) * outgoing + 220.0f;
                    wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category][nextMove]);
                    wazamakiNameSprite.scaleX = incoming;
                    PrepareDrawTextureWithColour(wazamakiNameSprite, inverseColour);
                }
            } else
                drawName = 1;
            break;
        case 1:
        case 2:
        case 4:
            if (row == wazamakiSelectedRow && group->count > 1) {
                wazamakiNameSprite.x += 24.92f;
                variant = (group->selected + 1 + group->count) % group->count;
                wazamakiNameSprite.y -= (1.0f - expansion) * 32.0f;
                for (index = 1; index < group->count; ++index) {
                    wazamakiNameSprite.y += 40.0f;
                    wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category][group->moves[variant]]);
                    PrepareDrawTextureWithColour(wazamakiNameSprite, fadeColour);
                    variant = (variant + 1 + group->count) % group->count;
                }
            }
            wazamakiNameSprite.y = y + 4.0f;
            wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category][move]);
            drawName = 1;
            break;
        case 3:
            if (row == wazamakiSelectedRow && group->count > 1) {
                xs[0] = wazamakiNameSprite.x;
                ys[0] = wazamakiNameSprite.y;
                for (index = 1; index < group->count; ++index) {
                    xs[index] = xs[0] + 24.92f;
                    ys[index] = static_cast<float>(index * 40) + ys[0];
                }
                variant = group->selected;
                std::int32_t direction = 1;
                if (!wazamaki.reverseVariant)
                    direction = -1;
                for (index = 0; index < group->count; ++index) {
                    const std::int32_t other = (index + direction + group->count) % group->count;
                    wazamakiNameSprite.x = remaining * xs[index] + (1.0f - remaining) * xs[other];
                    wazamakiNameSprite.y = remaining * ys[index] + (1.0f - remaining) * ys[other];
                    std::uint32_t colour = fadeColour;
                    if ((direction == -1 && !index) || (direction == 1 && !other))
                        colour = 0x80ffffffU;
                    wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category][group->moves[variant]]);
                    PrepareDrawTextureWithColour(wazamakiNameSprite, colour);
                    variant = (variant + 1 + group->count) % group->count;
                }
                wazamakiNameSprite.y = y + 4.0f;
                wazamakiNameSprite.icon = static_cast<std::uint16_t>(wazamakiNameSurfaces[category][move]);
            } else
                drawName = 1;
            break;
        default:
            drawName = 1;
            break;
        }
        if (drawName) {
            std::uint32_t colour = 0xffffffffU;
            if (row == wazamakiSelectedRow) {
                const std::int32_t alpha = static_cast<std::int32_t>(wazamaki.alpha * 255.0f);
                colour = static_cast<std::uint32_t>(alpha) << 24 | 0xffffffU;
            }
            wazamakiNameSprite.x = 220.0f;
            PrepareDrawTextureWithMode(wazamakiNameSprite);
            PrepareDrawTextureWithColour(wazamakiRubySprite, colour);
            if (special)
                PrepareDrawTextureWithColour(wazamakiSpecialSprite, colour);
        }
        if (category != 3 || move != 21) {
            wazamakiInputSprite.x = 608.0f;
            wazamakiInputSprite.y = y + 4.0f + scale * 32.0f * 0.5f;
            const std::uint8_t (*const commands)[7] = wazamakiGroupCommands[category];
            const std::byte *const marker = MOTN_GetMarkerData(GetMoveMotion(static_cast<std::uint8_t>(category), static_cast<std::uint8_t>(move)));
            if (marker && (reinterpret_cast<const std::uint32_t *>(marker)[2] & 8U)) {
                const std::int32_t frame = static_cast<std::int32_t>(wazamaki.frameCount) / 3 % 3;
                wazamakiCriticalSprite.icon = wazamakiCriticalFrames[frame];
                wazamakiCriticalSprite.x = wazamakiInputSprite.x;
                wazamakiCriticalSprite.y = wazamakiInputSprite.y;
                wazamakiCriticalSprite.scaleY = scale;
                PrepareDrawTextureWithSecondaryColour(wazamakiCriticalSprite, 0xff000000U);
            }
            wazamakiInputSprite.x -= 38.4f;
            for (std::int32_t input = static_cast<std::int8_t>(commands[position.index][0]) - 1; input >= 0; --input) {
                const std::uint8_t *inputs = commands[position.index];
                inputs += 1;
                const std::uint8_t command = inputs[input];
                wazamakiInputSprite.scaleX = wazamakiInputSprite.scaleY = 0.7f;
                if (command == 2) {
                    wazamakiInputSprite.rotation = 0x4000;
                    wazamakiInputSprite.scaleX = scale * 0.7f;
                } else if (command == 3) {
                    wazamakiInputSprite.rotation = 0xc000;
                    wazamakiInputSprite.scaleX = scale * 0.7f;
                } else {
                    wazamakiInputSprite.rotation = 0;
                    wazamakiInputSprite.scaleY = scale * 0.7f;
                }
                wazamakiInputSprite.icon = wazamakiInputIcons[command];
                PrepareDrawSimpleOpaque(wazamakiInputSprite, wazamakiInputColours[command]);
                wazamakiInputSprite.x -= 22.4f;
            }
        }
        if (group->count > 1) {
            wazamakiInputSprite.icon = 776;
            wazamakiInputSprite.x = 204.0f;
            wazamakiInputSprite.y = wazamakiInputSprite.y;
            wazamakiInputSprite.rotation = 0;
            wazamakiInputSprite.scaleY = wazamakiInputSprite.scaleX;
            wazamakiInputSprite.scaleX = 0.7f;
            PrepareDrawSimpleOpaque(wazamakiInputSprite, static_cast<std::uint32_t>(wazamaki.pulseBrightness) << 16 | 0xff000000U);
            wazamakiInputSprite.scaleX = 0.7f;
            wazamakiInputSprite.scaleY = 0.7f;
        }
        y += advance;
    }
    shenSubmitQuadsUI(406.0f, selectedY, 0x1f0000ffU, 0xbf0000ffU, 372, 40, -48.0f);
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint16_t RepeatWazamakiInput(std::uint16_t pressed, std::uint16_t repeated) {
    std::uint16_t result = 0;
    if (pressed || repeated) {
        if (repeated & 0x3000U) {
            if (--wazamakiRepeatDelay < 1) {
                wazamakiRepeatDelay = 1;
                result = repeated;
            } else {
                result = repeated & pressed;
            }
        }
    } else {
        wazamakiRepeatDelay = 16;
    }
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void SelectCurrentLegacyVariant(NativeWazamakiGroup *group, std::int32_t category) {
    if (!wazamaki.reverseVariant)
        group->selected = static_cast<std::uint8_t>((group->selected + 1 + group->count) % group->count);
    else
        group->selected = static_cast<std::uint8_t>((group->selected - 1 + group->count) % group->count);
    for (std::int32_t index = 0; index < group->count; ++index) {
        if (index == group->selected)
            SetMoveSkillFlag(0, static_cast<std::uint8_t>(category), group->moves[index]);
        else
            SetMoveSkillFlag(1, static_cast<std::uint8_t>(category), group->moves[index]);
    }
}

} // namespace

#else

namespace {

void SelectCurrentLegacyVariant() {
    std::int32_t category;
    NativeWazamakiGroup *const group = FindWazamakiGroup(wazamakiSelectedRow, &category);
    const std::int32_t step = wazamakiReverseVariant ? -1 : 1;
    group->selected = static_cast<std::uint8_t>((group->count + group->selected + step) % group->count);
    SetMoveEquipment(0, category, group->moves[group->selected]);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
NativeWazamakiGroup *FindWazamakiGroup(std::int32_t row, NativeWazamakiGroupPosition *position) {
    position->index = -1;
    position->category = -1;
    for (std::int32_t kind = 1; kind <= 3; ++kind) {
        if (row < wazamaki.groupCounts[kind]) {
            position->category = kind;
            NativeWazamakiGroup *result = 0;
            std::int32_t found = 0, index = 0;
            for (; found <= row; ++found) {
                do
                    result = &wazamakiGroups[kind][index++];
                while (!result->count && index < 40);
            }
            if (result)
                position->index = index - 1;
            return result;
        }
        row -= wazamaki.groupCounts[kind];
    }
    return 0;
}

} // namespace

#else

NativeWazamakiGroup *FindWazamakiGroup(std::int32_t row, std::int32_t *category, std::int32_t *groupIndex) {
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void MoveLearnedOwner_NullCallback(void *) {
    // Native EWZO is an explicit nullsub lifetime token.
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RequestWazamakiPreviewMotion(std::int32_t category, std::int32_t move) {
    if (GetMoveProficiency(category, move) <= 11)
        wazamakiPreview.pendingMotion = 306;
    else
        wazamakiPreview.pendingMotion = GetMoveBaseMotion(category, move);
    if (wazamakiPreview.character) {
        mtwkRequestMotion(charFindMotionwork(wazamakiPreview.character), wazamakiPreview.pendingMotion, static_cast<MotionRegion>(0));
        mtwkSetRegionFlag(charFindMotionwork(wazamakiPreview.character), 71, 0, static_cast<MotionRegion>(0));
        mtwkSetExtraFlag(charFindMotionwork(wazamakiPreview.character), 0, 2);
        mtwkSetRegionFlag(charFindMotionwork(wazamakiPreview.character), 66, 0, static_cast<MotionRegion>(0));
    }
    wazamakiPreviewMotionYaw = 0;
    if (category == 3) {
        switch (move) {
        case 8:
        case 11:
        case 12:
        case 21:
            break;
        default:
            wazamakiPreviewMotionYaw = 0x8000;
        }
    }
}

} // namespace

#else

void RequestWazamakiPreviewMotion(std::uint8_t category, std::uint8_t id) {
    const std::span<const MoveDefinition> definitions = MoveDefinitions(category);
    const std::span<const S2MoveSkillBank, 3> banks = S2Save_GetMoveSkillBanks();
    if (category == 0 || category > banks.size() || id >= definitions.size())
        return;

    const bool learned = banks[category - 1U].proficiency[id] >= 12;
    wazamakiPreviewPendingMotion = static_cast<std::uint16_t>(learned ? definitions[id].motionId : 306U);
    if (wazamakiPreviewCharacter) {
        MotionWork *const work = FindMotionWork(wazamakiPreviewCharacter);
        mtwkRequestMotion(work, wazamakiPreviewPendingMotion, MotionRegion::LowerBody);
        mtwkSetRegionFlag(work, 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(work, 0, 2);
        mtwkSetRegionFlag(work, 66, 0, MotionRegion::LowerBody);
    }
    wazamakiPreviewMotionYaw = 0;
    if (category == 3 && (id > 21 || !(0x00201900U & (1U << id))) && learned)
        wazamakiPreviewMotionYaw = 0x8000;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void PlayCurrentLegacyMove() {
    WazamakiLearnedGroup *const group = CurrentLearnedGroup();
    if (!group || !group->count)
        return;
    const std::uint8_t id = group->moves[group->selected];
    RequestWazamakiPreviewMotion(group->category, id);
    wazamakiPresentationCategory = group->category;
    wazamakiPresentationVariant = group->selected;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void DrawWazamakiButtons() {
    wazamaki.pulseBrightness += wazamaki.pulseStep * 3;
    if (wazamaki.pulseBrightness > 62) {
        wazamaki.pulseBrightness = 63;
        wazamaki.pulseStep = -1;
    } else if (wazamaki.pulseBrightness <= 0) {
        wazamaki.pulseBrightness = 0;
        wazamaki.pulseStep = 1;
    }
    LegacyIconSprite sprite;
    sprite.width = 0;
    sprite.rotation = 0;
    sprite.scaleX = 0.85f;
    sprite.scaleY = 0.85f;
    sprite.flags = 0;
    sprite.depth = 9.0f;
    sprite.alignment = 1;
    sprite.x = 44.0f;
    sprite.y = 52.0f;
    sprite.width = 38;
    sprite.height = 38;
    sprite.icon = 776;
    std::uint32_t pulse = wazamaki.pulseBrightness;
    PrepareDrawSimpleOpaque(sprite, pulse << 16 | 0xff000000U);
    sprite.x += 40.800003f;
    sprite.icon = 777;
    pulse = wazamaki.pulseBrightness;
    PrepareDrawSimpleOpaque(sprite, pulse | 0xff000000U);
    sprite.width = 64;
    sprite.height = 64;
    sprite.x += 40.800003f;
    sprite.y -= 8.0f;
    sprite.icon = 2049;
    pulse = wazamaki.pulseBrightness;
    PrepareDrawSimpleOpaque(sprite, 0xff000000U | pulse << 16 | pulse << 8 | pulse);
}

} // namespace

#else

namespace {

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

} // namespace

#endif

} // namespace s2
} // namespace shenmue
