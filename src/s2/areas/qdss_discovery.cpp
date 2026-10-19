#include "s2/areas/area_qdss.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/search.hpp"
#include "s2/step_event.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
constexpr float F(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}
constexpr shen_name frame{SHEN_NAME(_gak)}, jar{SHEN_NAME(_tub)}, uniform{SHEN_NAME(_doi)};
void Camera(std::int32_t id, std::int32_t delay, shen_name base = QDSS_BASE) {
    EVT_AT_SetCam(id, base, base, 0, 0, delay);
}
void Voice(const char *token, std::int32_t delay) {
    EVT_AT_SetVoice(token, delay);
}
void Position(shen_name character, shen_name base, float x, float y, float z, std::int32_t delay) {
    EVT_AT_SetPos(character, base, Vector3{x, y, z}, delay);
}
void Attach(shen_name prop, std::int32_t bone, Vector3 position, Vector3i rotation, std::int32_t delay) {
    QueueQdssCling(prop, QDSS_RYO, bone, &position, &rotation, 0, delay);
}
void Detach(shen_name prop, std::int32_t delay) {
    QueueQdssCling(prop, shen_name{0}, 0, nullptr, nullptr, 1, delay);
}
void DetachHide(shen_name prop, std::int32_t delay) {
    Detach(prop, delay);
    EVT_AT_ObjectDisp_1(prop, delay, 1);
}
void ShowDiscovery(shen_name prop) {
    EV_EnableCharacterDisplay(QDSS_RYO);
    EV_DisableCharacterDisplay(QDSS_REN);
    EV_EnableCharacterDisplay(prop);
}
std::int32_t DiagramDiscovery(QdssWork &work) {
    const QdssKey &key = work.keys[1];
    if (EV_GetShenmueFlag(0x325))
        EV_stepSetFlags(0, 0x325);
    if (EV_GetShenmueFlag(0x326)) {
        EV_stepSetFlags(0, 0x326);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1523, 1);
    }
    ShowDiscovery(key.character);
    EV_EnableCharacterDisplay(QDSS_DIAGRAM);
    Camera(2000, 0);
    Position(QDSS_RYO, QDSS_BASE, 2.5999999f, 0, 5.9499998f, 0);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 0, 0);
    EVT_AT_SetMot(QDSS_RYO, 10574, 220, 1, 0x40000000U, 0);
    Attach(key.character, 21, Vector3{F(0x3de56042), F(0x3c8b4396), -.0089999996f}, Vector3i{23665, 16384, 0}, 0);
    Attach(QDSS_DIAGRAM, 14, Vector3{F(0x3e408312), F(0x3d408312), -.017000001f}, Vector3i{49516, 55341, 18386}, 0);
    Voice("0247A040", 20); // @text EN: "Hm?" | JA: "んっ？"
    Voice("0247A046", 65); // @text EN: "This picture=@" | JA: "この絵は…"
    Voice("0247A047", 100); // @text EN: "The Four Heavenly Beasts Ren mentioned." | JA: "レンが言ってた四神獣か。"
    EVT_AT_SetMot(QDSS_RYO, 10574, 370, 0, 0x40000000U, 150);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(key.flag))) {
        QueueQdssAward(10, 150);
        QdssFade(30, true, 180);
        DetachHide(key.character, 210);
        DetachHide(QDSS_DIAGRAM, 210);
        Position(QDSS_RYO, QDSS_INVALID, 165.59f, 136, 67.449997f, 210);
        return 210;
    }
    Camera(2001, 180);
    Voice("0247A048", 180); // @text EN: "Hm? This is=@" | JA: "ん？　これは…"
    EVT_AT_SetMot(QDSS_RYO, 10574, 370, 1, 0x40000000U, 250);
    Camera(2002, 260);
    Voice("0247A176", 280); // @text EN: "Looks similar to the stick\nthat Zhang gave me." | JA: "張さんから渡された棒に\n似ているな。"
    Camera(2003, 390);
    EVT_AT_SetMot(QDSS_RYO, 10574, 510, 0, 0x40000000U, 390);
    Voice("0247A120", 390); // @text EN: "There must be some meaning to the\ncombination of these pictures." | JA: "この絵の組合わせに、\n何か意味がありそうだ。"
    Camera(2004, 500);
    EVT_AT_SetMot(QDSS_RYO, 10574, 510, 1, 0x40000000U, 500);
    Voice("0247A121", 500); // @text EN: "Somewhere in this room\nthere are the others=@?" | JA: "他の棒は、この部屋のどこかに…？"
    Camera(2005, 610);
    Voice("0247A123", 610); // @text EN: "Some connection with\nthe Four Heavenly Beasts?" | JA: "四神獣と何か関係があるのか？"
    QueueQdssAward(10, 610);
    QdssFade(60, true, 670);
    DetachHide(key.character, 730);
    DetachHide(QDSS_DIAGRAM, 670);
    return 730;
}
std::int32_t RenAdvice(std::int32_t delay) {
    QdssFade(30, false, delay);
    EVT_AT_ObjectDisp_1(QDSS_RYO, delay, 0);
    EVT_AT_ObjectDisp_1(QDSS_REN, delay, 0);
    EV_SetCharacterAlphaEnabled(QDSS_REN, false);
    EVT_AT_ObjectDisp_1(QDSS_DIAGRAM, delay, 0);
    Camera(2100, delay);
    Position(QDSS_RYO, QDSS_BASE, -.037f, 0, .54000002f, delay);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 270, delay);
    EVT_AT_SetMot(QDSS_RYO, 10570, 0, 1, 0x40000000U, delay);
    Position(QDSS_REN, QDSS_BASE, 4.993f, 0, .040000021f, delay);
    EVT_AT_SetAngY(QDSS_REN, QDSS_BASE, 270, delay);
    EVT_AT_SetMot(QDSS_REN, 10561, 0, 1, 0x40000000U, delay);
    Attach(QDSS_DIAGRAM, 14, Vector3{F(0x3e408312), F(0x3d408312), -.017000001f}, Vector3i{49516, 55341, 18386}, delay);
    Voice("0247B056", delay + 30); // @text EN: "How's it going?" | JA: "どうだ？"
    Voice("0247B057", delay + 80); // @text EN: "Did you find something?" | JA: "なんか見っかったのかよ。"
    Camera(2110, delay + 143);
    Voice("0247B058", delay + 200); // @text EN: "Hey, what is it?" | JA: "おい、なんだそりゃ？"
    Camera(2120, delay + 244);
    QueueQdssEyePart(delay + 244);
    Camera(2130, delay + 360);
    Voice("0247B059", delay + 360); // @text EN: " " | JA: "んー?"
    Voice("0247B060", delay + 430); // @text EN: "These are the Four Heavenly Beasts." | JA: "こいつは四神獣…"
    Camera(2140, delay + 530);
    Voice("0247B061", delay + 530); // @text EN: "Same picture that's on those sticks\nthat the ol' man gave you. Look." | JA: "張のおっさんがよこした\n棒と同じ絵だ…見ろ。"
    if (EV_GetShenmueFlag(0x37a)) {
        Camera(2150, delay + 630);
        Voice("0247B062", delay + 630); // @text EN: "The four sticks and the statue of\nBuddha must be connected." | JA: "４本の棒と、この仏像には\n関係がありそうだ。"
        Camera(2160, delay + 770);
        Voice("0247A162", delay + 770); // @text EN: "What connection does the statue\nhave with the four sticks?" | JA: "…棒と仏像…どんな関係が？"
        Voice("0247B063", delay + 870); // @text EN: "That's for you to figure out." | JA: "んなもん、てめぇで探しな。"
    } else {
        Camera(2150, delay + 660);
        Voice("0247B064", delay + 660); // @text EN: "Hey=@ that statue looks fishy." | JA: "おい…あの仏像がくせぇぞ。"
        Voice("0247A163", delay + 760); // @text EN: "Huh?" | JA: "えっ…"
        Camera(2160, delay + 800);
        Voice("0247A164", delay + 800); // @text EN: "Statue of Buddha=@" | JA: "仏像…"
        Voice("0247B065", delay + 870); // @text EN: "Well, check carefully." | JA: "ま、せいぜいしっかり調べんだな…"
    }
    QdssFade(30, true, delay + 970);
    DetachHide(QDSS_DIAGRAM, delay + 1000);
    QueueQdssEye(shen_name{0}, delay + 1000);
    return 1000;
}
std::int32_t RenReminder() {
    EV_EnableCharacterDisplay(QDSS_RYO);
    EV_EnableCharacterDisplay(QDSS_REN);
    EV_SetCharacterAlphaEnabled(QDSS_REN, false);
    Camera(2200, 0);
    Position(QDSS_RYO, QDSS_BASE, 5.0899997f, 0, 3.8800001f, 0);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 0, 0);
    EVT_AT_SetMot(QDSS_RYO, 10571, 0, 1, 0x40000000U, 0);
    Position(QDSS_REN, QDSS_BASE, 5.1999998f, 0, .23f, 0);
    EVT_AT_SetAngY(QDSS_REN, QDSS_BASE, 0, 0);
    EVT_AT_SetMot(QDSS_REN, 10562, 0, 1, 0x40000000U, 0);
    Voice("0247B056", 30); // @text EN: "How's it going?" | JA: "どうだ？"
    Voice("0247B057", 80); // @text EN: "Did you find something?" | JA: "なんか見っかったのかよ。"
    Camera(2210, 135);
    Voice("0247A165", 140); // @text EN: "I haven't found a clue yet." | JA: "…いや、手がかりは、何もないな。"
    Camera(2220, 210);
    Voice("0247B066", 240); // @text EN: "No way!\nWere you searching seriously?" | JA: "んなわけねぇだろ。\nマジメにやってんのか？"
    Voice("0247A166", 370); // @text EN: "Yeah." | JA: "ああ。"
    Voice("0247B067", 395); // @text EN: "Did you check everything,\nlike that desk drawer there?" | JA: "その辺の机の中とか、調べたのか？"
    Voice("0247A167", 485); // @text EN: "A desk drawer=@" | JA: "…そうか、机か。"
    Camera(2230, 575);
    Voice("0247B068", 575); // @text EN: "There you go=@" | JA: "ったく、言わんこっちゃねぇ。"
    Voice("0247B065", 675); // @text EN: "Well, check carefully." | JA: "ま、せいぜいしっかり調べんだな…"
    QdssFade(30, true, 745);
    return 805;
}
std::int32_t DragonDiscovery(QdssWork &work) {
    const QdssKey &key = work.keys[2];
    ShowDiscovery(key.character);
    EV_EnableCharacterDisplay(frame);
    QueueQdssProp(SHEN_NAME(MA56), 0, 0);
    Camera(2300, 0);
    Camera(2301, 150);
    Camera(2302, 365);
    Camera(2303, 570);
    Camera(2304, 670);
    Camera(2305, 890);
    Position(QDSS_RYO, QDSS_BASE, 2.4649999f, 0, 6.652f, 0);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 180, 0);
    EVT_AT_SetMot(QDSS_RYO, 10575, 30, 1, 0x40000000U, 0);
    Position(frame, QDSS_BASE, 2.6600001f, 2.125f, 7.0970001f, 0);
    EVT_AT_SetAngY(frame, QDSS_BASE, 180, 0);
    Position(key.character, QDSS_BASE, 2.26f, 1.766f, 7.2220001f, 0);
    EVT_AT_SetAngY(key.character, QDSS_BASE, 0, 0);
    EVT_AT_SetAng_Enqueue(key.character, QDSS_BASE, 270, 0, 100, 0);
    Attach(frame, 14, Vector3{F(0x3eb43958), F(0xbed78d50), F(0xbf028f5c)}, Vector3i{9466, 35862, 49698}, 240);
    Detach(frame, 355);
    Attach(key.character, 14, Vector3{F(0x3dac0831), F(0xbb03126f), -.0099999998f}, Vector3i{15291, 32768, 56251}, 570);
    DetachHide(key.character, 975);
    Voice("0247A134", 30); // @text EN: "Yin and Yang." | JA: "八陰八陽…"
    Voice("0247A135", 140); // @text EN: "Same as the banner in our dojo." | JA: "家の道場にあったものと同じだ…"
    Voice("0247A136", 410); // @text EN: "Hm?\nThere's something=@" | JA: "んっ？　…何かあるな。"
    Voice("0247A043", 530); // @text EN: "This is=@" | JA: "これは…"
    const Vector3 eye{164.300003f, 137.399994f, 67.199997f};
    SetEyeToChara(ctrlGetCharaByName(QDSS_RYO), &eye, 560, 2);
    Voice("0247A137", 670); // @text EN: "A picture of a dragon." | JA: "龍の絵がついている。"
    Voice("0247A138", 750); // @text EN: "Same shape as the other sticks.\nAre these keys?" | JA: "他の棒と同じ形だ\n…何かのカギなのか？"
    Voice("0247A139", 910); // @text EN: "I'll keep it." | JA: "取っておこう…"
    QueueQdssAward(key.item, 910);
    QdssFade(30, true, 1000);
    EVT_AT_ObjectDisp_1(frame, 1030, 1);
    QueueQdssProp(SHEN_NAME(MA56), 1, 1030);
    QueueQdssEye(shen_name{0}, 1030);
    Position(QDSS_RYO, QDSS_INVALID, 164.69f, 136, 67.349998f, 1030);
    return 1030;
}
std::int32_t JarDiscovery(QdssWork &work, bool phoenix) {
    const QdssKey &key = work.keys[phoenix ? 3 : 6];
    const shen_name prop = phoenix ? SHEN_NAME(MC28) : SHEN_NAME(MC30);
    ShowDiscovery(key.character);
    EV_EnableCharacterDisplay(jar);
    QueueQdssProp(prop, 0, 0);
    Position(QDSS_RYO, QDSS_BASE, phoenix ? -.736f : 3.059f, 0, phoenix ? .87599999f : .63800001f, 0);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, phoenix ? 90 : 180, 0);
    EVT_AT_SetMot(QDSS_RYO, 10576, 30, 1, 0x40000000U, 0);
    if (phoenix) {
        AT_SetLocalBaseChara(QDSS_CAMERA_BASE, QDSS_RYO);
        Position(QDSS_CAMERA_BASE, QDSS_RYO, 0, 0, 0, 0);
        EVT_AT_SetAngY(QDSS_CAMERA_BASE, QDSS_RYO, 0, 0);
        EVT_AT_SetAng_Enqueue(QDSS_CAMERA_BASE, QDSS_RYO, 0, 0, 0, 0);
        Camera(2400, 0, QDSS_CAMERA_BASE);
    }
    Position(jar, QDSS_RYO, -.032000002f, 1.526f, -.60799998f, 0);
    EVT_AT_SetAngY(jar, QDSS_RYO, 270, 0);
    if (!phoenix) {
        AT_SetLocalBaseChara(QDSS_CAMERA_BASE, QDSS_RYO);
        Position(QDSS_CAMERA_BASE, QDSS_RYO, 0, 0, 0, 0);
        EVT_AT_SetAngY(QDSS_CAMERA_BASE, QDSS_RYO, 0, 0);
        EVT_AT_SetAng_Enqueue(QDSS_CAMERA_BASE, QDSS_RYO, 0, 0, 0, 0);
        Camera(2400, 0, QDSS_CAMERA_BASE);
    }
    Camera(2401, 70, QDSS_CAMERA_BASE);
    Attach(jar, 21, Vector3{F(0x3e0c49ba), F(0xbd9fbe77), .22f}, Vector3i{4004, 2366, 64625}, 70);
    Position(QDSS_RYO, QDSS_BASE, phoenix ? -.336f : 3.059f, 0, phoenix ? .87599999f : .23800001f, 70);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, phoenix ? 90 : 180, 70);
    EVT_AT_SetMot(QDSS_RYO, 10576, 100, 1, 0x40000000U, 70);
    Voice("0247A141", 120); // @text EN: "Hm?\nThere's something=@" | JA: "んっ…中に何かあるな。"
    Camera(2402, 220, QDSS_CAMERA_BASE);
    Attach(jar, 14, Vector3{F(0x3e170a3d), F(0xbd6425af), -.22840001f}, Vector3i{10403, 45120, 56169}, 220);
    Detach(jar, 284);
    Attach(key.character, 14, Vector3{F(0x3dac0831), F(0xbb03126f), -.0099999998f}, Vector3i{15291, 32768, 56251}, 360);
    Camera(2404, 400, QDSS_CAMERA_BASE);
    Voice("0247A043", 400); // @text EN: "This is=@" | JA: "これは…"
    QueueQdssEye(key.character, 400);
    Voice(phoenix ? "0247A142" : "0247A175", 440); // @text 0247A142 EN: "A Phoenix=@" | JA: "朱雀の絵…" ; 0247A175 EN: "Similar to the Snaketortoise stick." | JA: "玄武の棒と似たような形だな。"
    Camera(2403, 502, QDSS_CAMERA_BASE);
    Voice(phoenix ? "0247A045" : "0247A071", 520); // @text 0247A045 EN: "I should keep this." | JA: "持っておこう…" ; 0247A071 EN: "I'll keep it." | JA: "取っておこう。"
    QueueQdssAward(key.item, 520);
    QdssFade(30, true, 610);
    DetachHide(key.character, 640);
    EVT_AT_ObjectDisp_1(jar, 640, 1);
    QueueQdssProp(prop, 1, 640);
    QueueQdssEye(shen_name{0}, 640);
    return 640;
}
std::int32_t rightUniformPose[19][3]{{0, 0, 0}, {0, 0, 0}, {0, 62260, 3276}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 2002}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    {0, 0, 2730}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 3276}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 3822}};
std::int32_t leftUniformPose[19][3]{{0, 493, 0}, {0, 0, 2184}, {0, 61714, 64990}, {0, 0, 0}, {0, 0, 0}, {0, 0, 62806}, {0, 0, 3640}, {0, 0, 0}, {0, 0, 62988},
    {0, 0, 0}, {0, 0, 3458}, {0, 0, 0}, {0, 0, 64810}, {0, 0, 0}, {0, 0, 3640}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 3822}};
void UniformHands(void *) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(QDSS_RYO), 1, reinterpret_cast<std::byte *>(rightUniformPose), 10);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(QDSS_RYO), 0, reinterpret_cast<std::byte *>(leftUniformPose), 10);
    HLib::DestroyCurrentTask();
}
void KeyHands(void *) {
    ApplyQdssKeyGrip(QDSS_RYO, 1);
    ApplyQdssKeyGrip(QDSS_RYO, 0);
    Character *owner = ctrlGetCharaByName(QDSS_RYO);
    const Vector3i left{8736, 4004, 0}, right{0, 0, 0};
    ApplyCharacterHandRotation(owner, 0, 21, &left);
    ApplyCharacterHandRotation(owner, 1, 21, &right);
    HLib::DestroyCurrentTask();
}
void QueueHands(HLib::TaskCallback callback, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(callback, 4, 0x0b, static_cast<std::int32_t>(QDSS_EVENT.name));
    HLib::SetupTimedTask(task, delay);
}
std::int32_t TigerDiscovery(QdssWork &work) {
    const QdssKey &key = work.keys[0];
    Character *ryo = ctrlGetCharaByName(QDSS_RYO);
    const Vector3i left{0, 4915, 0}, right{0, 61896, 0};
    ApplyCharacterHandRotation(ryo, 0, 21, &left);
    ApplyCharacterHandRotation(ryo, 1, 21, &right);
    EV_EnableCharacterDisplay(QDSS_RYO);
    EV_DisableCharacterDisplay(QDSS_REN);
    EV_EnableCharacterDisplay(uniform);
    QueueQdssProp(SHEN_NAME(M675), 0, 0);
    QueueQdssProp(SHEN_NAME(M217), 0, 0);
    Camera(2500, 0);
    Position(QDSS_RYO, QDSS_BASE, F(1074702319U), F(3197536043U), F(3205092502U), 0);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 0, 0);
    EVT_AT_SetMot(QDSS_RYO, 10577, 100, 1, 0x40000000U, 0);
    GetMotionFrameCount(10577);
    Position(uniform, QDSS_BASE, F(1075058835U), F(1061192466U), F(3215638659U), 0);
    EVT_AT_SetAngY(uniform, QDSS_BASE, 0, 0);
    QueueHands(UniformHands, 150);
    Attach(uniform, 21, Vector3{F(0x3eab020c), F(0xbcf5c28f), F(1044750795U)}, Vector3i{46954, 45511, 21277}, 185);
    Camera(2501, 200);
    Position(QDSS_RYO, QDSS_BASE, 2.2989998f, -.294f, -.33840007f, 200);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 0, 200);
    EVT_AT_SetMot(QDSS_RYO, 10577, 300, 1, 0x40000000U, 200);
    Camera(2502, 300);
    QueueHands(KeyHands, 300);
    Position(QDSS_RYO, QDSS_BASE, 2.23f, 0, -.46000004f, 300);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 0, 300);
    EVT_AT_SetMot(QDSS_RYO, 10577, 440, 1, 0x40000000U, 300);
    EVT_AT_ObjectDisp_1(key.character, 300, 0);
    Position(key.character, QDSS_INVALID, 171.58f, 136, 66.5f, 300);
    EVT_AT_SetAngY(key.character, QDSS_INVALID, 90, 300);
    Detach(uniform, 300);
    Position(uniform, QDSS_BASE, 2.2839999f, .75199997f, -1.334f, 300);
    EVT_AT_SetAngY(uniform, QDSS_BASE, 0, 300);
    EVT_AT_SetAng_Enqueue(uniform, QDSS_BASE, 0, 0, 0, 300);
    Camera(2503, 390);
    Attach(key.character, 14, Vector3{F(0x3dac0831), F(0xbb03126f), -.0099999998f}, Vector3i{15291, 32768, 56251}, 390);
    DetachHide(key.character, 700);
    Voice("0247A112", 10); // @text EN: "Huh? This uniform is=@" | JA: "んっ？　この胴着は…"
    Voice("0247A113", 110); // @text EN: "The Hazuki name's on it." | JA: "芭月の字がぬい込まれている…"
    Voice("0247A114", 190); // @text EN: "Perhaps this was my father's." | JA: "もしかして、親父の…"
    Voice("0247A144", 260); // @text EN: "What?" | JA: "なんだ？"
    Voice("0247A145", 420); // @text EN: "Same shape as the stick Zhang gave me." | JA: "張さんに渡された棒と、同じ形だ。"
    QueueQdssEye(key.character, 420);
    Voice("0247A146", 540); // @text EN: "This is a picture of a tiger." | JA: "これは、虎の絵だ…"
    Camera(2504, 540);
    Voice("0247A079", 670); // @text EN: "I'll keep it." | JA: "取っておくか…"
    Camera(2505, 670);
    QueueQdssAward(key.item, 670);
    QdssFade(60, true, 730);
    EVT_AT_ObjectDisp_1(uniform, 790, 1);
    QueueQdssProp(SHEN_NAME(M675), 1, 790);
    QueueQdssProp(SHEN_NAME(M217), 1, 790);
    QueueQdssEye(shen_name{0}, 790);
    return 790;
}
std::int32_t DecoyDiscovery(QdssWork &work, std::int32_t index) {
    const QdssKey &key = work.keys[index];
    ShowDiscovery(key.character);
    Attach(key.character, 14, Vector3{F(0x3de56042), F(0x3c8b4396), -.0089999996f}, Vector3i{23665, 16384, 0}, 0);
    if (index == 4) {
        Position(QDSS_RYO, QDSS_BASE, 2.21f, 0, 4.29f, 0);
        EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 180, 0);
    } else if (index == 5) {
        Position(QDSS_RYO, QDSS_BASE, .0099999998f, 0, 6.3899999f, 0);
        EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 90, 0);
    } else {
        Position(QDSS_RYO, QDSS_BASE, 4.8299999f, 0, 5.5500002f, 0);
        EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 180, 0);
    }
    EVT_AT_SetMot(QDSS_RYO, 10578, 80, 1, 0x40000000U, 0);
    Camera(2600, 1, QDSS_RYO);
    Voice(index == 4 ? "0247A175" : index == 5 ? "0247A081" : "0247A073", 0); // @text 0247A175 EN: "Similar to the Snaketortoise stick." | JA: "玄武の棒と似たような形だな。" ; 0247A081 EN: "A picture of a rooster=@" | JA: "ニワトリの絵がついてるな。" ; 0247A073 EN: "A picture of a fish=@" | JA: "魚の絵がついてる…"
    QueueQdssEye(key.character, 100);
    Voice(index == 5 ? "0247A045" : "0247A071", 140); // @text 0247A045 EN: "I should keep this." | JA: "持っておこう…" ; 0247A071 EN: "I'll keep it." | JA: "取っておこう。"
    QueueQdssAward(key.item, 140);
    DetachHide(key.character, 160);
    QdssFade(30, true, 230);
    QueueQdssEye(shen_name{0}, 260);
    if (index == 4)
        Position(QDSS_RYO, QDSS_INVALID, 167.42f, 136, 67.010002f, 260);
    return 260;
}
} // namespace
std::int32_t RunQdssDiscovery(QdssWork &work, std::int32_t selector, std::int32_t) {
    srchSetLookMode();
    PrepareQdssHands(Vector3i{0, 0, 0}, Vector3i{0, 0, 0});
    EV_SetRefBoardParamsFor(QDSS_RYO, 0);
    EV_SetRefBoardParamsFor(QDSS_REN, 0);
    std::int32_t duration = 0;
    switch (selector) {
    case 10:
        duration = DiagramDiscovery(work) + 30;
        Position(QDSS_BASE, QDSS_INVALID, 171.5f, 136, 63.800003f, duration);
        if (EV_GetShenmueFlag(0x3c9)) {
            EV_stepSetFlags(0, 0x3c9);
            duration += RenAdvice(duration);
        }
        Position(QDSS_BASE, QDSS_INVALID, 171.5f, 136, 64.800003f, duration);
        break;
    case 11:
        duration = RenAdvice(0);
        break;
    case 12:
        duration = RenReminder();
        break;
    case 13:
        duration = DragonDiscovery(work);
        break;
    case 14:
        duration = JarDiscovery(work, true);
        break;
    case 15:
        duration = TigerDiscovery(work);
        break;
    case 16:
        duration = DecoyDiscovery(work, 4);
        break;
    case 17:
        duration = DecoyDiscovery(work, 5);
        break;
    case 18:
        duration = JarDiscovery(work, false);
        break;
    case 19:
        duration = DecoyDiscovery(work, 7);
        break;
    default:
        break;
    }
    if (selector != 11 && selector != 12)
        HLib::SetupTaskDelay(duration);
    return duration;
}
} // namespace shenmue::s2::areas
