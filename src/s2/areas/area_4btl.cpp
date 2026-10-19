#include "s2/areas/area_4btl.hpp"
#include "s2/scene_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <array>
#include <cstddef>
#include <cmath>

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, sam{SHEN_NAME(SAM_)}, lly{SHEN_NAME(LLY_)}, ren{SHEN_NAME(REN_)}, clj{SHEN_NAME(CLJ_)};
constexpr shen_name cam{SHEN_NAME(CAM_)}, dummy{0x5f4d5544U}, dummyModel{SHEN_NAME(DUMY) | 0x6000000U};
constexpr shen_name ball{SHEN_NAME(BALL) | 0x13010001U}, invalid{0xffffffffU};
constexpr shen_name tables[]{SHEN_NAME(TBLa), SHEN_NAME(TBLb), SHEN_NAME(TBLc), SHEN_NAME(TBLd), SHEN_NAME(TBLe)};
constexpr shen_name brokenTables[]{SHEN_NAME(bTK0), SHEN_NAME(bTK1), SHEN_NAME(bTK2), SHEN_NAME(bTK3), SHEN_NAME(bTK4)};
constexpr shen_name chairs[]{
    SHEN_NAME(IS_5), SHEN_NAME(IS_7), SHEN_NAME(IS_8), SHEN_NAME(IS_9), SHEN_NAME(IS12), SHEN_NAME(IS17), SHEN_NAME(IS18), SHEN_NAME(IS20), SHEN_NAME(IS23)};
struct BattleAnchor {
    Vector3 position;
    float yaw;
};
constexpr BattleAnchor anchors[]{{{237.3f, 98.35f, 463}, 0}, {{234.6f, 98.35f, 463.3f}, 0}, {{234.8f, 98.35f, 467.6f}, 0}, {{230.3f, 98.35f, 467}, 0},
    {{241, 98.35f, 466}, 0}, {{233.2f, 101.6f, 458.6f}, 0}, {{233.1f, 101.6f, 454}, 0}, {{261.9f, 100.9f, 418.8f}, 270}, {{265.3f, 100.9f, 418.6f}, 270},
    {{270.2f, 100.9f, 414.5f}, 0}, {{267.1f, 100.9f, 405.7f}, 45}, {{259.3f, 101, 400}, 0}, {{259.2f, 100.9f, 390.9f}, 15}, {{258.1f, 100.9f, 384.8f}, 0},
    {{256.8f, 100.9f, 379.8f}, 0}, {{245.8f, 100.2f, 379.8f}, 90}, {{238.8f, 100.2f, 377.4f}, 60}, {{230.8f, 100.2f, 372.6f}, 70},
    {{221.9f, 100.2f, 367.6f}, 30}, {{220.2f, 100.2f, 354.9f}, 0}, {{220.8f, 100.2f, 341.8f}, 0}, {{224, 100.2f, 328}, 315}, {{237.6f, 100.2f, 324}, 270},
    {{246, 100.2f, 328.5f}, 225}, {{249.6f, 100.2f, 341.8f}, 180}, {{251.6f, 100.2f, 353}, 180}, {{251.5f, 100.2f, 362.6f}, 180}};
std::array<Vector3, 9> savedChairPositions;
std::array<Vector3i, 9> savedChairRotations;
std::int32_t entrySelection{}, anchorSelection{}, motionBank{};
shen_name characterNames[]{ryo, invalid, invalid};
std::int32_t handCommand[]{1999, 65354, 62444, 1638, 180, 63900, 546, 64719, 3366, 0, 64809, 60257, 0, 64809, 59528, 0, 0, 62623, 0, 0, 64628, 0, 0, 58983, 0,
    0, 58073, 0, 65172, 61897, 0, 0, 65354, 0, 729, 60074, 0, 65173, 57163, 0, 65172, 63171, 0, 0, 0, 0, 547, 56252, 0, 1639, 60074, 0, 64809, 61715, 64080,
    182, 65535};
struct BattleTask {
    HLib::Task *self;
    shen_name opponent;
    std::int32_t state, phase, reserved14, wait, resume;
    EventReadCharactersWork *reader;
};
static_assert(sizeof(BattleTask) == 0x28);
static_assert(offsetof(BattleTask, reader) == 0x20);
struct ClearNpcTask {
    void (*callback)(void *);
    void *parameter;
    std::int32_t reserved10, state;
    float delay;
    std::uint32_t reserved1c;
};
static_assert(sizeof(ClearNpcTask) == 0x20);
Character *CharacterFor(shen_name name) {
    return FindCharacterByScenarioName(name);
}
void Camera(std::int32_t index, shen_name eye = cam, shen_name target = cam, std::int32_t delay = 0) {
    EVT_AT_SetCam(index, eye, target, 0, 0, delay);
}
void Film(std::int32_t start, std::int32_t target, std::int32_t frames, std::int32_t delay = 0) {
    EVT_AT_MenCtrl(EventFilmControl{0, frames, start, 0, 0, 0, target, 0, 0, 0, static_cast<float>(delay)});
}
void Place(shen_name name, const Vector3 &position, float yaw, shen_name parent = invalid) {
    EVT_AT_SetPos(name, parent, position, 0);
    EVT_AT_SetAngY(name, parent, yaw, 0);
}
void AttachCameraToDummy() {
    Place(cam, {}, 0, dummy);
}
void ApplyAnchor(shen_name name) {
    const BattleAnchor &anchor = anchors[anchorSelection];
    Place(name, anchor.position, anchor.yaw);
    Place(dummyModel, anchor.position, anchor.yaw);
    AttachCameraToDummy();
}
void SelectAnchor(shen_name name) {
    Vector3 position{};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    float nearest = 500;
    anchorSelection = 0;
    for (std::size_t i = 0; i < std::size(anchors); ++i) {
        const Vector3 difference{position.x - anchors[i].position.x, position.y - anchors[i].position.y, position.z - anchors[i].position.z};
        const float distance = std::sqrt(difference.x * difference.x + difference.z * difference.z);
        if (distance < nearest) {
            nearest = distance;
            anchorSelection = static_cast<std::int32_t>(i);
        }
    }
    ApplyAnchor(name);
}
void ChairState(bool restore) {
    if (restore) {
        for (shen_name name : tables)
            if (DoesScenarioCharacterExist(name))
                EV_EnableCharacterDisplay(name);
        for (shen_name name : brokenTables)
            if (DoesScenarioCharacterExist(name))
                EV_DisableCharacterDisplay(name);
    }
    for (std::size_t i = 0; i < std::size(chairs); ++i)
        if (DoesScenarioCharacterExist(chairs[i])) {
            if (restore) {
                EV_AT_SetPos(chairs[i], EventTransformFlags::World | EventTransformFlags::AllAxes, savedChairPositions[i]);
                EV_AT_SetAngY(chairs[i], EventTransformFlags::World | EventTransformFlags::AllAxes, savedChairRotations[i]);
            } else {
                EV_GetCharaPosition(chairs[i], CHARACTER_TRANSLATION_ROOT, &savedChairPositions[i], EventTransformFlags::World);
                GetScenarioCharacterEulerAngles(chairs[i], &savedChairRotations[i], EventTransformFlags::World);
            }
        }
}
void AuthorModel(shen_name name, std::int32_t enabled) {
    Character *const character = CharacterFor(name);
    SetFaceFlagsForChara_0(character, enabled);
    SetHighModelXYZ(character, static_cast<std::int8_t>(enabled), 0, 0);
    SetHighModelXYZ(character, static_cast<std::int8_t>(enabled), 1, 0);
}
void CreateStageObjects(bool samFight) {
    if (samFight) {
        AllocateVOBJ(0, 4);
        vfStageDefinitionAddHitbox(0, 2, 1, 0, 0, 0, 0.3f);
        vfStageDefinitionAddHitbox(0, 2, 1, -0.3f, 0, 0, 0.3f);
        vfStageDefinitionAddHitbox(0, 2, 1, -0.6f, 0, 0, 0.3f);
        ToggleVOBJArrayIndex(0, 15, -1, 0, 0);
        vfStageDefinitionSetHitResponse(0, 0, 0, 2560, 0, 0, 1.0f, 0.01f);
        vfStageDefinitionSetAttachment(0, 14, 0.05f, 0, -0.05f, 0, 0, 0x47700000U);
        vfAllocateVOBJ(0, CharacterFor(SHEN_NAME(KEIB)), 0);
        vfStageObjectAssignMotFlags(0, 4214784);
        vfStageObjectReset(0, CharacterFor(sam));
        vfInitializeStageObjectHitboxes();
        EV_DisableCharacterDisplay(SHEN_NAME(KEIB));
        vfUpdateFlagsFor(CharacterFor(sam), 0, 0x8000, 1604);
        return;
    }
    AllocateVOBJ(0, 1);
    vfStageDefinitionAddHitbox(0, 2, 1, 0, 0.3f, 0, 0.6f);
    vfStageDefinitionAddHitbox(0, 2, 0x8001, 0.9f, 0.5f, 0, 0.5f);
    vfStageDefinitionSetMotion(0, SHEN_NAME(TUKU), 17, nullptr, 2, 2, 299, 117);
    for (std::size_t i = 0; i < std::size(tables); ++i) {
        vfAllocateVOBJ(static_cast<std::int32_t>(i), CharacterFor(tables[i]), 0);
        vfStageObjectSetBrokenModel(static_cast<std::int32_t>(i), CharacterFor(brokenTables[i]), 720);
    }
    AllocateVOBJ(1, 7);
    vfStageDefinitionAddHitbox(1, 2, 1, 0, 0.2f, 0, 0.4f);
    vfStageDefinitionSetHitResponse(1, 2916, 2914, 393216, 0x2000, 0, 17.0f, 0);
    ToggleVOBJArrayIndex(1, 10, -1, 14, 2913);
    for (std::size_t i = 0; i < std::size(chairs); ++i) {
        vfAllocateVOBJ(static_cast<std::int32_t>(i), CharacterFor(chairs[i]), 1);
        vfStageObjectAssignMotFlags(static_cast<std::int32_t>(i), 3);
        vfStageObjectAssignMotFlags(static_cast<std::int32_t>(i), 4096);
    }
    vfInitializeStageObjectHitboxes();
}
std::int32_t Intro(BattleTask &task) {
    const shen_name opponent = task.opponent;
    if (entrySelection == 0 || entrySelection == 2) {
        SelectAnchor(opponent);
        Place(opponent, {}, 180, opponent);
        Place(ryo, {-0.1f, 0, -2.6f}, 180, opponent);
        EVT_AT_SetMot(ryo, opponent.name == sam.name ? 6660 : 6985, 0, 1, 0, 0);
        EVT_AT_SetMot(opponent, opponent.name == sam.name ? 6664 : 6981, 0, 1, 0, 0);
        Camera(1500, opponent, opponent);
        EVT_AT_SetVoice(opponent.name == sam.name ? "0082A001" : "0083A007", 1, 0); // @text 0082A001 EN: "Talk!\nWhere is Yuanda Zhu?" | JA: "教えろ。朱元達はどこにいる！？" ; 0083A007 EN: "Where is Yuanda Zhu?" | JA: "朱元達はどこにいる！？"
        EVT_AT_SetVoice(opponent.name == sam.name ? "0082B001" : "0083B001", 1, opponent.name == sam.name ? 60 : 90); // @text EN: "That was fast=@" | JA: "…来やがったか。"
        EVT_AT_SetVoice(opponent.name == sam.name ? "0082B002" : "0083B004", 1, opponent.name == sam.name ? 140 : 150); // @text 0082B002 EN: "You are so damn persistent!" | JA: "この野郎…\nしつけえんだよ！" ; 0083B004 EN: "Come on.\nIf you can beat me, then I'll tell ya!" | JA: "かかってきな…\n俺に勝てたら教えてやる！"
        return 220;
    }
    if (opponent.name == sam.name) {
        Place(ryo, {236, 98.5f, 465.3f}, 337.5f);
        Place(cam, {}, 0, ryo);
        Place(opponent, {237.5f, 98.3f, 461.9f}, 180);
        EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0, 0);
        EVT_AT_SetMot(opponent, 6662, 0, 1, 0, 0);
        Camera(1300);
        EVT_AT_SetVoice("0082B004", 1, 0); // @text EN: "Huh?" | JA: "ん！？"
        EVT_AT_SetVoice("0082B002", 1, 110); // @text EN: "You are so damn persistent!" | JA: "この野郎…\nしつけえんだよ！"
        return 200;
    }
    anchorSelection = 26;
    Place(ryo, {252.1f, 100.3f, 363.9f}, 180);
    Place(opponent, {252.6f, 100.22f, 366.73f}, 270);
    Place(cam, {}, 0, ryo);
    EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(opponent, 6979, 0, 0, 0x40000000U, 0);
    EVT_AT_SetMot(opponent, 6977, 0, 1, 0, 280);
    Camera(1400);
    Camera(1401, cam, cam, 60);
    Camera(1402, cam, cam, 135);
    SetHighModelAndFaceFlagsForChara(CharacterFor(ryo), 0, 10, 1);
    EVT_AT_SetFaceLooks(ryo, 1, 7, 1, 70);
    SetHighModelAndFaceFlagsForChara(CharacterFor(opponent), 1, 10, 1);
    EVT_AT_SetFaceLooks(opponent, 13, 10, 1, 135);
    EVT_AT_SetFaceLooks(opponent, 1, 10, 1, 370);
    EVT_AT_SetPoint(opponent, ryo, 5, 3, 1, 0, 280, 0x80000000U);
    EVT_AT_SetVoice("0083B001", 1, 0); // @text EN: "That was fast=@" | JA: "…来やがったか。"
    EVT_AT_SetVoice("0083A001", 1, 60); // @text EN: "Tell me about Yuanda Zhu." | JA: "朱元達のことを教えろ。"
    EVT_AT_SetVoice("0083B002", 1, 150); // @text EN: "Hold it right there." | JA: "まあ、待てよ…"
    EVT_AT_SetVoice("0083B003", 1, 210); // @text EN: "What will I tell the boss\nif he found out that I just told you?" | JA: "素直に教えてやったんじゃ、\nレンさんに合わせる顔がねえ。"
    EVT_AT_SetVoice("0083B004", 1, 370); // @text EN: "Come on.\nIf you can beat me, then I'll tell ya!" | JA: "かかってきな…\n俺に勝てたら教えてやる！"
    EVT_AT_SetVoice("0083B020", 1, 480);
    return 490;
}
struct VoiceCue {
    const char *voice;
    std::int32_t delay;
};
void Voices(const VoiceCue *cues, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i)
        EVT_AT_SetVoice(cues[i].voice, 1, cues[i].delay);
}
std::int32_t Won(BattleTask &task) {
    const shen_name opponent = task.opponent;
    Film(255, 0, 30);
    if (opponent.name == sam.name) {
        Place(opponent, {237.6f, 98.3f, 466.53f}, 0);
        EVT_AT_SetPos(opponent, invalid, {237.9f, 98.3f, 465.7f}, 1060);
        EVT_AT_SetAngY(opponent, invalid, 180, 1060);
        Place(dummyModel, {237.6f, 98.3f, 466.53f}, 0);
        AttachCameraToDummy();
        Place(ryo, {0.16f, 0, 2.7f}, 0, opponent);
        Film(0, 255, 30, 1170);
        EVT_AT_SetMot(ryo, 6666, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(opponent, 6667, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(opponent, 6668, 330, 1, 0x40000000U, 1060);
        Camera(1200);
        Camera(1201, cam, cam, 560);
        Camera(1202, cam, cam, 800);
        Camera(1203, cam, cam, 940);
        Camera(1204, cam, cam, 1060);
        constexpr VoiceCue cues[]{{"0082B016", 0}, {"0082A001", 80}, {"0082B006", 200}, {"0082B017", 260}, {"0082A002", 380}, {"0082B007", 420}, // @text 0082A001 EN: "Talk!\nWhere is Yuanda Zhu?" | JA: "教えろ。朱元達はどこにいる！？" ; 0082B006 EN: "Ow, ouch!" | JA: "イ、イテテッ…" ; 0082B017 EN: "Lemme go!\nOnly Ren knows about that!" | JA: "放せ！\nレンアニキしか知らねえんだよっ！" ; 0082A002 EN: "What?" | JA: "なんだと！？" ; 0082B007 EN: "It's true!\nRen is probably over at Lucky Plaza=@" | JA: "本当だ！　アニキならたぶん、\nラッキープラザに行ったはずだ…"
            {"0082A003", 560}, {"0082B008", 600}, {"0082B009", 740}, {"0082A004", 940}, {"0082A011", 1000}, {"0082B018", 1120}}; // @text 0082A003 EN: "Lucky Plaza?" | JA: "ラッキープラザ？" ; 0082B008 EN: "Yeah, you go through the alley\nby the General Store on Queen's St." | JA: "そうだ、クイーンズストリートの…\n雑貨屋の脇の路地を入るんだ…" ; 0082B009 EN: "The building under construction\nis Lucky Plaza." | JA: "その奥の…工事中のビルが、\nラッキープラザだ。" ; 0082A004 EN: "Lucky Plaza=@" | JA: "ラッキープラザか…" ; 0082A011 EN: "Take me there." | JA: "そこまで案内しろ。" ; 0082B018 EN: "All right,\nfollow me." | JA: "わかったよ…ついてきな。"
        Voices(cues, std::size(cues));
        EVT_AT_SetPoint(ryo, opponent, 0, 3, 0, 940, 260, 0xc0000000U);
        return 1230;
    }
    ApplyAnchor(opponent);
    Film(0, 255, 30, 840);
    Place(ryo, {-1.4f, 0, -0.7f}, 270, opponent);
    EVT_AT_SetPos(opponent, ryo, {1.2f, 0, -1.2f}, 700);
    EVT_AT_SetAngY(opponent, ryo, 180, 700);
    EVT_AT_SetMot(ryo, 6986, 0, 1, 0, 0);
    EVT_AT_SetMot(opponent, 6980, 0, 1, 0, 0);
    EVT_AT_SetMot(opponent, 6987, 280, 1, 0x40000000U, 700);
    Camera(1000);
    Camera(1001, cam, cam, 40);
    Camera(1002, cam, cam, 145);
    Camera(1003, cam, cam, 250);
    Camera(1004, cam, cam, 440);
    Camera(1005, cam, cam, 485);
    Camera(1006, cam, cam, 660);
    Camera(1007, cam, cam, 700);
    constexpr VoiceCue cues[]{{"0083B005", 55}, {"0083A003", 150}, {"0083B006", 255}, {"0083B007", 320}, {"0083A004", 445}, {"0083B008", 490}, // @text 0083B005 EN: "You are strong=@" | JA: "……やっぱ強ぇな、おめえ…" ; 0083A003 EN: "Where's Yuanda Zhu?" | JA: "約束だ。朱元達はどこにいる？" ; 0083B006 EN: "I don't know anything about him." | JA: "俺はそいつのことは知らねえ。" ; 0083B007 EN: "Ask the boss.\nHe should be at Lucky Plaza." | JA: "レンさんに聞け。\nきっとラッキープラザにいるはずだ。" ; 0083A004 EN: "Lucky Plaza?" | JA: "ラッキープラザ？" ; 0083B008 EN: "A building under construction." | JA: "工事中のビルさ…"
        {"0083B021", 550}, {"0083A015", 660}, {"0083B023", 750}}; // @text 0083B021 EN: "Go through the alley\nby the General Store on Queen's St." | JA: "クイーンズストリートの、\n雑貨屋の脇の路地を入りな…" ; 0083A015 EN: "Take me there." | JA: "そこまで案内しろ。" ; 0083B023 EN: "All right.\nFollow me." | JA: "しょうがねえな…ついてきな。"
    Voices(cues, std::size(cues));
    SetHighModelAndFaceFlagsForChara(CharacterFor(ryo), 0, 10, 1);
    EVT_AT_SetFaceLooks(ryo, 1, 7, 1, 160);
    SetHighModelAndFaceFlagsForChara(CharacterFor(opponent), 6, 10, 1);
    return 870;
}
std::int32_t Lost(BattleTask &task) {
    const shen_name opponent = task.opponent;
    Film(255, 0, 30);
    Film(0, 255, 30, 390);
    Film(255, 0, 30, 420);
    if (opponent.name == sam.name) {
        Place(ryo, {236.8f, 98.4f, 466.4f}, 0);
        Place(opponent, {236.7f, 98.3f, 467.2f}, 0);
        Place(dummyModel, {236.8f, 98.4f, 466.4f}, 0);
        AttachCameraToDummy();
        EVT_AT_SetAngY(opponent, opponent, 180, 90);
    } else {
        ApplyAnchor(ryo);
        EVT_AT_SetAngY(ryo, ryo, -90, 0);
        EVT_AT_SetAngY(cam, cam, -90, 0);
        Place(opponent, {0, 0, -1}, 180, ryo);
        EVT_AT_SetAngY(opponent, opponent, -70, 90);
    }
    ApplyCharacterHandPoseCommand(CharacterFor(ryo), 0, reinterpret_cast<std::byte *>(handCommand), 8);
    ApplyCharacterHandPoseCommand(CharacterFor(ryo), 1, reinterpret_cast<std::byte *>(handCommand), 8);
    EVT_AT_SetMot(ryo, opponent.name == sam.name ? 6658 : 6983, 0, 1, 0, 0);
    EVT_AT_SetMot(ryo, opponent.name == sam.name ? 6659 : 6984, 0, 1, 0, 420);
    EVT_AT_SetMot(opponent, opponent.name == sam.name ? 6659 : 6984, 0, 1, 0, 0);
    EVT_AT_SetMot(opponent, 0xf005, 0, 1, 0, 90);
    EVT_AT_SetPoint(opponent, ryo, 0, 3, 0, 0, 90, 0);
    Camera(1100);
    Camera(1101, cam, cam, 90);
    SetHighModelAndFaceFlagsForChara(CharacterFor(ryo), 6, 0, 1);
    EVT_AT_SetFaceLooks(ryo, 0, 7, 1, 420);
    EVT_AT_SetVoice(opponent.name == sam.name ? "0082A005" : "0083B010", 1, 0); // @text EN: "So long=@" | JA: "じゃあな…"
    EVT_AT_SetVoice(opponent.name == sam.name ? "0082B010" : "0083A014", 1, opponent.name == sam.name ? 120 : 300); // @text 0082B010 EN: "Take a nap!" | JA: "そこでずっと、おねんねしてな！" ; 0083A014 EN: "Damn.\nI've got to look for the others." | JA: "…くそっ…\nほかのヤツらを探すか…"
    return 420;
}
void InitializeBattle(BattleTask &task) {
    EV_SetNewSceneIndex();
    const char *const package = task.opponent.name == sam.name ? "4NIN_SAM" : "4NIN_LLY";
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_FreeAuthorPak(1, task.opponent.name == sam.name ? "0082" : "0083");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, dummyModel);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, ball);
    SetLowQualityHands(CharacterFor(ryo));
    SetLowQualityHands(CharacterFor(task.opponent));
    AuthorModel(ryo, 1);
    AuthorModel(task.opponent, 1);
    ChairState(false);
    if (EV_GetShenmueFlag(0x376) || EV_GetShenmueFlag(0x378)) {
        anchorSelection = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Story, 39));
        task.state = 3;
    } else
        task.state = 1;
    Film(255, 0, 15);
}
std::int32_t RunFight(BattleTask &task) {
    if (task.phase == 0) {
        Film(255, 0, 30);
        motionBank = static_cast<std::int32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "REN.MOT"));
        const bool samFight = task.opponent.name == sam.name;
        const char *const resource = samFight ? "0082E000" : "0083E000";
        EV_LoadCutsceneFromCurrentScene(resource);
        EV_LoadAudio(resource, 0);
        Camera(samFight ? 1301 : 1403);
        shenStartFGST(10, 10, 0, 0);
        shenInitFightData(CharacterFor(ryo), nullptr, 0, 0, 0, 0);
        shenInitFightData(CharacterFor(task.opponent), nullptr, 1, 1, 300, samFight ? 11 : 12);
        vfUpdateFlagsFor(CharacterFor(task.opponent), 0, 0x8000000, 1);
        CreateStageObjects(samFight);
        if (samFight)
            EV_vfBattleUtil(FightBattleUtilOp::MotionOrSlotFlag, sam, 1);
        EVT_AT_SetAngY(task.opponent, ryo, 180, 0);
        if (!samFight && anchorSelection == 12)
            vfCopyPosFromEntity(0, nullptr, 259.74f, 388.47f, 4);
        else if (!samFight && anchorSelection == 11) {
            vfCopyPosFromEntity(0, nullptr, 260.4f, 399.1f, 5);
            flddSetPointFlagOnGrid(SHEN_NAME(BTC1), 1);
            flddSetPointFlagOnGrid(SHEN_NAME(PROP), 0);
            flddSetPointFlagOnGrid(SHEN_NAME(0000), 0);
        } else
            vfCopyPosFromEntity(1, CharacterFor(ryo), 0, 0, !samFight && anchorSelection == 14 ? 3.8f : 5.0f);
        EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
        for (shen_name name : {ryo, task.opponent}) {
            SetPluralControlFor(CharacterFor(name), 1);
            SetScenarioCharacterType(name, CharacterTypeDefault);
            AuthorModel(name, 0);
        }
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        for (shen_name name : {ren, clj, samFight ? lly : sam})
            if (DoesScenarioCharacterExist(name)) {
                EV_DisableCharacterDisplay(name);
                EV_ToggleCharacterFlag(name);
            }
        vfmQueueFightStart(2, 99, 1, 0, -1);
        ++task.phase;
    } else if (task.phase == 1) {
        if (!IsFightActive()) {
            Film(0, 255, 30);
            ++task.phase;
            return 30;
        }
    } else if (task.phase == 2) {
        DeleteMotion(motionBank);
        task.state = GetBtlStatus(CharacterFor(ryo)) == 3 ? 3 : 4;
        task.phase = 0;
        if (task.opponent.name == sam.name) {
            EV_DisableCharacterDisplay(SHEN_NAME(KEIB));
            EV_vfBattleUtil(FightBattleUtilOp::MotionOrSlotFlag, sam, 0);
        }
        EV_SetCharacterAlphaEnabled(task.opponent, false);
        EV_SetCharacterAlphaEnabled(ryo, false);
        AuthorModel(ryo, 1);
        AuthorModel(task.opponent, 1);
        if (anchorSelection == 11) {
            flddSetPointFlagOnGrid(SHEN_NAME(BTC1), 0);
            flddSetPointFlagOnGrid(SHEN_NAME(PROP), 1);
            flddSetPointFlagOnGrid(SHEN_NAME(0000), 1);
        }
    }
    return 0;
}
void FinishBattle(BattleTask &task) {
    const shen_name opponent = task.opponent;
    const std::int32_t outcome = task.state;
    for (shen_name name : {ren, clj, opponent.name == sam.name ? lly : sam})
        if (DoesScenarioCharacterExist(name)) {
            EV_EnableCharacterDisplay(name);
            EV_EnsureCharacterDraw(name);
        }
    SetPluralControlFor(CharacterFor(ryo), 0);
    hndlSetFlags(CharacterFor(ryo));
    hndlSetFlags(CharacterFor(opponent));
    PlaceNpcAtAreaPoint(opponent, shen_name{}, nullptr, 0);
    SetScenarioFollowCharacter(opponent);
    ET_AT_KillWork(task.reader);
    if (!EV_GetShenmueFlag(0x376) && !EV_GetShenmueFlag(0x378))
        ChairState(true);
    if (EV_GetShenmueFlag(0x376))
        EV_stepSetFlags(0, 0x376);
    if (EV_GetShenmueFlag(0x378))
        EV_stepSetFlags(0, 0x378);
    DateTime time{};
    GetCurrentDateTime(time);
    if (outcome == 7 && ((time.hour == 2 && time.minute >= 30) || (time.hour >= 3 && time.hour <= 8))) {
        EV_stepSetFlags(2, opponent.name == sam.name ? 0x81 : 0x82);
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(AR02) | 0x1000000U, 1);
        return;
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 39, opponent.name == sam.name ? 0 : anchorSelection);
    EV_stepSetFlags(outcome != 7, opponent.name == sam.name ? 0x81 : 0x82);
    if (opponent.name == lly.name) {
        EV_debug_log("4BTL\\e4BTL_et_battle_four.c", 399, "======================================\n");
        EV_debug_log("4BTL\\e4BTL_et_battle_four.c", 400, "    SetFreeTalkCounterNo = %d\n", anchorSelection);
        EV_debug_log("4BTL\\e4BTL_et_battle_four.c", 401, "======================================\n");
    }
    EV_FreeAuthorPak(0, opponent.name == sam.name ? "0082" : "0083");
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(opponent.name == sam.name ? "4NIN_SAM" : "4NIN_LLY"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(task.self);
    scenFreeEvent("BTL4.EV3");
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
    if (outcome == 7) {
        SelectAnchor(ryo);
        if (opponent.name == lly.name)
            EVT_AT_SetAngY(ryo, ryo, 180, 0);
        EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0x40000000U, 0);
        LoadCutsceneAndSoundProgramForArea();
        EV_LoadNPCRoute("NPC_AR02.BIN");
        Film(255, 0, 30);
    } else {
        EVT_AT_SetAngY(ryo, ryo, 90, 0);
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
    }
    SetCharaKill(dummyModel, 1);
    SetCharaKill(ball, 1);
}
void UpdateBattle(void *data) {
    auto &task = *static_cast<BattleTask *>(data);
    const std::int32_t previous = task.state;
    std::int32_t delay = 0;
    switch (previous) {
    case 0:
        InitializeBattle(task);
        break;
    case 1:
        delay = Intro(task);
        task.state = 2;
        Film(0, 255, 30, delay - 30);
        break;
    case 2:
        delay = RunFight(task);
        break;
    case 3:
        delay = Won(task);
        EVT_AT_FadeBGM(30, 1, delay - 30);
        task.state = 7;
        break;
    case 4:
        delay = Lost(task);
        EVT_AT_FadeBGM(30, 1, delay - 30);
        task.state = 6;
        break;
    case 5:
        if (authGetFinished()) {
            EV_CleanupMapControlTask();
            task.wait = 0;
        }
        if (task.wait-- <= 0)
            task.state = task.resume;
        break;
    case 6:
    case 7:
        FinishBattle(task);
        break;
    default:
        break;
    }
    if (previous == 3 || previous == 4) {
        task.resume = task.state;
        task.state = 5;
        task.wait = delay;
    } else if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void QueueBattle(void *) {
    HLib::Task *const owner = HLib::EnqueueTaskWithParameter(UpdateBattle, 6, 11, sizeof(BattleTask), SHEN_NAME(EVNT));
    auto *const task = static_cast<BattleTask *>(HLib::GetTaskParameter(owner));
    task->self = owner;
    task->opponent = entrySelection < 2 ? sam : lly;
    task->state = 0;
    task->phase = 0;
    characterNames[1] = task->opponent;
    task->reader = Queue_EVT_AT_ReadChr(owner, characterNames);
}
void ClearNpcs(void *data) {
    auto &work = *static_cast<ClearNpcTask *>(data);
    if (work.delay > 0) {
        work.delay -= HLib::GetScaledDeltaTick();
        return;
    }
    if (work.state) {
        work.callback(work.parameter);
        HLib::MarkCurrentTaskForCleanup();
        HLib::DestroyCurrentTask();
    } else {
        EV_NPC_NewLoadSw(0);
        work.state = static_cast<std::int32_t>(EV_ResetFreeTalkFlags());
    }
}
void Initialize4Btl() {
    entrySelection = 0;
    anchorSelection = 0;
}
void Start4Btl(std::uintptr_t argument) {
    entrySelection = static_cast<std::int32_t>(argument);
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *const owner = HLib::EnqueueTaskWithParameter(ClearNpcs, 6, 11, sizeof(ClearNpcTask), SHEN_NAME(EVNT));
    auto *const work = static_cast<ClearNpcTask *>(HLib::GetTaskParameter(owner));
    work->callback = QueueBattle;
    work->parameter = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
} // namespace
const ScriptAreaDefinition area4BtlScriptDefinition{.areaId = SHEN_NAME(4BTL), .field04 = 0, .init = Initialize4Btl, .main = Start4Btl};
} // namespace shenmue::s2::areas
