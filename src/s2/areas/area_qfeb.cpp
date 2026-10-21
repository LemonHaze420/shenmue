#include "s2/areas/area_qfeb.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/fog.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
void QfebMainCallback(void *);
void QfebAuthorCallback(void *);
void QfebAuthorLoopCallback(void *);
void QueueQfebAuthor(shen_name author) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QfebAuthorCallback, 4, 0x0b, sizeof(QfebAuthorWork), QFEB_TASK);
    QfebAuthorWork *const work = static_cast<QfebAuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    work->author = author;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0302");
}
void StartQfebAuthorLoop(QfebAuthorWork *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QfebAuthorLoopCallback, 4, 0x0b, sizeof(QfebAuthorLoopWork), QFEB_TASK);
    QfebAuthorLoopWork *const work = static_cast<QfebAuthorLoopWork *>(HLib::GetTaskParameter(task));
    work->frame = 0;
    work->parent = parent;
    mtwkSetExtraFlag(scenFindMotionwork(QFEB_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(QFEB_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(QFEB_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(QFEB_REN, false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author.name));
    parent->state = 2;
}
void FinishQfebAuthor(QfebAuthorWork *work) {
    using enum CameraState;
    HLib::DestroyTask(work->task);
    SetPlayerControlEnabled(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    const Vector3i &rotation = work->author.name == QFEB_AUTHORS[2].name ? qfebAuthorReturnRotation : QFEB_AUTHOR_DEFAULT_ROTATION;
    EV_AT_SetAngY(QFEB_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void QfebAuthorCallback(void *data) {
    QfebAuthorWork *const work = static_cast<QfebAuthorWork *>(data);
    if (work->state == 1)
        StartQfebAuthorLoop(work);
    else if (work->state == 3)
        FinishQfebAuthor(work);
}
void QfebAuthorLoopCallback(void *data) {
    QfebAuthorLoopWork *const work = static_cast<QfebAuthorLoopWork *>(data);
    work->frame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        LoadCutsceneAndSoundProgramForArea();
        mtwkSetExtraFlag(scenFindMotionwork(QFEB_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(QFEB_RYO, true);
        mtwkSetExtraFlag(scenFindMotionwork(QFEB_REN), 6, 0);
        EV_SetCharacterAlphaEnabled(QFEB_REN, true);
        SetCurrentGameFlag(22, 0);
        work->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}
void PrepareQfebBattleChoreography(QfebEventWork *work) {
    const std::int8_t entry = static_cast<std::int8_t>(work->entry - 1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QFEB_RYO), FacialExpressionId::Normal, 5, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QFEB_REN), FacialExpressionId::Normal, 5, 1);
    EVT_AT_SetFaceLooks(QFEB_RYO, FacialExpressionId::Angry, 10, 1, 190);
    EVT_AT_SetFaceLooks(QFEB_REN, FacialExpressionId::Angry, 10, 1, 150);
    for (std::int32_t index = 0; index < 2; ++index) {
        const shen_name name = QFEB_CHOREOGRAPHY_NPCS[entry][index];
        mtwkSetRequestedRootMode(scenFindMotionwork(name), 3);
        SetMotionFrameTime(scenFindMotionwork(name), 15, MotionRegion::LowerBody);
        EVT_AT_SetMot(name, index == 0 ? 0x7f03U : 0x7f06U, 0, 1, 0x80000000U, index == 0 ? 10 : 20);
        EventSetPosition position{
            .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_CHOREOGRAPHY_NPC_POSITIONS[entry][index], .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{
            .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_CHOREOGRAPHY_NPC_YAWS[entry][index], .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
    }
    for (std::int32_t index = 0; index < 2; ++index) {
        const shen_name name = index == 0 ? QFEB_RYO : QFEB_REN;
        EVT_AT_SetMot(name, index == 0 ? 0x7f05U : 0x7f04U, 0, 1, 0x80000000U, index == 0 ? 190 : 150);
        EventSetPosition position{
            .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_CHOREOGRAPHY_POSITIONS[entry][index], .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_CHOREOGRAPHY_YAWS[entry][index], .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
    }
    if (entry == 0)
        EVT_AT_SetVoice("0302C001", 1, 110); // @text EN: "Hey!\nWho are you!?" | JA: "おい！　てめぇ何モンだ！"
    else if (entry == 1)
        EVT_AT_SetVoice("0302D001", 1, 110); // @text EN: "You must be the intruders!" | JA: "侵入者ってのは、てめぇらか！？"
    else if (entry == 2)
        EVT_AT_SetVoice("0302G001", 1, 110); // @text EN: "Where did you rats come from?!" | JA: "てめぇら、どこのネズミだ！"
    EventSetCamera camera{
        .cleanup = 0, .camera = work->entry + 1999, .eye = QFEB_BASE, .target = QFEB_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(work->entry + 2049, QFEB_BASE, QFEB_BASE, 0, 0, 170);
}
void PrepareQfebBattleStances(QfebEventWork *work) {
    using enum CameraState;
    if (work->entry >= 1 && work->entry <= 3) {
        const std::int32_t entry = work->entry - 1;
        for (std::int32_t index = 0; index < 7; ++index) {
            const shen_name name = QFEB_FIGHT_CHARACTERS[index];
            EventSetMotion motion{
                .cleanup = 0, .character = name, .animation = QFEB_STANCE_MOTIONS[index], .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(QFEB_STANCE_MOTIONS[index]);
            EventSetPosition position{
                .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_BATTLE_POSITIONS[entry][index], .delay = 0};
            EVT_AT_SetPosRot(&position);
            EventSetAngle angle{.cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_BATTLE_YAWS[entry][index], .delay = 0};
            EVT_AT_SetAngY_Callback(&angle);
        }
        camCtrlSetState(Event);
        camCtrlSetNextPosition(QFEB_CAMERA_EYES[entry]);
        camCtrlSetTargetPosition(QFEB_CAMERA_TARGETS[entry]);
    }
    work->phase = 7;
}
void AddQfebGenerators(QfebEventWork *work) {
    if (work->entry >= 1 && work->entry <= 3) {
        for (std::int32_t index = 1; index < 7; ++index) {
            const Vector3 &point = QFEB_BATTLE_POSITIONS[work->entry - 1][index];
            vfAddGeneratorPoint(point.x, point.y, point.z);
        }
    }
}
void StartQfebBattle(QfebEventWork *work) {
    EV_DisableCharacterDisplay(QFEB_REN);
    EV_ToggleCharacterFlag(QFEB_REN);
    shenStartFGST(9, 20, 0, 6);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x20}, 1);
    AddQfebGenerators(work);
    shenInitFightData(ctrlGetCharaByName(QFEB_RYO), nullptr, 0, 0, 0, 0);
    vfUpdateFlagsFor(ctrlGetCharaByName(QFEB_RYO), 0, 0x20000, 31);
    if (work->entry >= 1 && work->entry <= 3) {
        for (std::int32_t index = 0; index < 3; ++index) {
            const shen_name name = QFEB_LIVE_ENEMIES[work->entry - 1][index];
            shenInitFightData(ctrlGetCharaByName(name), nullptr, 1, 1, 150, static_cast<std::uint8_t>(index + 2));
            vfUpdateFlagsFor(ctrlGetCharaByName(name), 0, 0x400000, 128);
            vfUpdateFlagsFor(ctrlGetCharaByName(name), 0, 0x200000, 2048);
        }
    }
    Character *const dead = reinterpret_cast<Character *>(static_cast<std::uintptr_t>(QFEB_DEAD.name));
    switch (work->entry) {
    case 1:
        PublishFightRespawn(150, 3, 3636, 234.89f, 128.8f, 138.92f, 185);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 229.64f, 128.8f, 147.21001f, 330);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 237.86f, 128.8f, 142.98f, 90);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 226.94f, 128.8f, 142.86f, 227);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 235.08f, 128.8f, 147.39f, 30);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 229.96001f, 128.8f, 138.52f, 175);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 234.89f, 128.8f, 138.92f, 185);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 229.64f, 128.8f, 147.21001f, 330);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 237.86f, 128.8f, 142.98f, 90);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 226.94f, 128.8f, 142.86f, 227);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 229.96001f, 128.8f, 138.52f, 175);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 234.89f, 128.8f, 138.92f, 185);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 229.64f, 128.8f, 147.21001f, 330);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 237.86f, 128.8f, 142.98f, 90);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 232.37f, 142.94f, 6.0f);
        break;
    case 2:
        PublishFightRespawn(150, 4, 3640, 216.17999f, 128.8f, 172.60001f, 149);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 218.13f, 128.8f, 173.88f, 84);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 212.92f, 128.8f, 177.89999f, 209);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 216.46001f, 128.8f, 177.59f, 21);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 212.14f, 128.8f, 175.77f, 285);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 214.62f, 128.8f, 171.67f, 169);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 216.17999f, 128.8f, 172.60001f, 149);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 218.13f, 128.8f, 177.88f, 84);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 212.92f, 128.8f, 177.89999f, 209);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 212.14f, 128.8f, 175.77f, 285);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 214.62f, 128.8f, 171.67f, 169);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 216.17999f, 128.8f, 172.60001f, 149);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 218.13f, 128.8f, 173.88f, 84);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 214.8f, 174.60001f, 4.0f);
        break;
    case 3:
        PublishFightRespawn(150, 3, 3636, 196.21001f, 128.8f, 147.84f, 62);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 193.72f, 128.8f, 153.48f, 190);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 198.14f, 128.8f, 151.24001f, 201);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 191.60001f, 128.8f, 150.71001f, 147);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 196.77f, 128.8f, 153.56f, 171);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 193.2f, 128.8f, 147.91f, 170);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 196.21001f, 128.8f, 147.84f, 204);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 193.72f, 128.8f, 153.48f, 165);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 198.14f, 128.8f, 151.24001f, 217);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 191.60001f, 128.8f, 150.71001f, 82);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 196.77f, 128.8f, 153.56f, 62);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 193.2f, 128.8f, 147.91f, 88);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 196.21001f, 128.8f, 147.91f, 99);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 193.72f, 128.8f, 153.56f, 181);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 194.96001f, 150.7f, 4.0f);
        break;
    default:
        break;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    for (const shen_name *name = QFEB_CHARACTERS; name->name != EVENT_CHARACTER_INVALID.name; ++name) {
        SetPluralControlFor(ctrlGetCharaByName(*name), 1);
        SetScenarioCharacterType(*name, CharacterTypeDefault);
    }
    SetPluralControlFor(ctrlGetCharaByName(QFEB_RYO), 1);
    SetScenarioCharacterType(QFEB_RYO, CharacterTypeDefault);
    SetHighModelXYZ(ctrlGetCharaByName(QFEB_RYO), 0, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(QFEB_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(ctrlGetCharaByName(QFEB_RYO), FacialNodeOriginal);
    if (work->entry >= 1 && work->entry <= 3)
        for (shen_name name : QFEB_RELEASE_ENEMIES[work->entry - 1])
            EV_npcFree(name);
    vfmQueueFightStart(6, 2, 1, 1, -1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
    vfmDispatchControl(FightControlOp::CameraParameter, shen_name{}, 10, 1);
    work->phase = 8;
}
void UpdateQfebBattleTaunts(QfebEventWork *work) {
    ++work->voiceFrames;
    if (cvsHasValidConverseData(nullptr) != 1 && work->battleUpdates >= 30 && work->voiceFrames >= 90) {
        std::uint32_t index = static_cast<std::uint32_t>(shenGetRandomInt32(4));
        if (index == work->previousVoice)
            index = (index + 1) % 4;
        switch (index) {
        case 0:
            EV_PlayDialogue("0302C002");
            break;
        case 1:
            EV_PlayDialogue("0302E001");
            break;
        case 2:
            EV_PlayDialogue("0302F001");
            break;
        case 3:
            EV_PlayDialogue("0302H001");
            break;
        }
        const std::uint16_t voice = static_cast<std::uint16_t>(index << 8);
        std::memcpy(&work->voiceFrames, &voice, sizeof(voice));
    }
}
void QueueQfebAftermath() {
    EVT_AT_SetSound(16, 10);
    EVT_AT_SetVoice("S2BVF007", 1, 11);
    EVT_AT_SetSound(38, 30);
    EVT_AT_SetVoice("S2BVA030", 1, 31);
    EVT_AT_SetVoice("S2BVE017", 1, 50); // @text EN: "Damn kid!" | JA: "このガキぃっ！"
    EVT_AT_SetVoice("S2BVG014", 1, 89); // @text EN: "You cocky kid!" | JA: "ふざけやがって！"
    EVT_AT_SetVoice("S2BVF012", 1, 124); // @text EN: "You're not going any further!" | JA: "これ以上、行かせるか！"
    EVT_AT_SetVoice("S2BVF005", 1, 171); // @text EN: "Die!" | JA: "死ねっ！"
    EVT_AT_SetSound(16, 193);
    EVT_AT_SetVoice("S2BVA007", 1, 194);
    EVT_AT_SetSound(14, 231);
    EVT_AT_SetSound(14, 236);
    EVT_AT_SetVoice("S2BVG019", 1, 238); // @text EN: "Drop dead!" | JA: "くたばりやがれっ！"
    EVT_AT_SetSound(34, 246);
    EVT_AT_SetSound(16, 273);
    EVT_AT_SetVoice("S2BVA012", 1, 274);
    EVT_AT_SetSound(38, 293);
}
void FinishQfebDefeat(QfebEventWork *work) {
    if (work->entry >= 1 && work->entry <= 3)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(1989 + work->entry), 0);
    SetPluralControlFor(ctrlGetCharaByName(QFEB_RYO), 0);
    EV_SetCharacterAlphaEnabled(QFEB_REN, true);
    EV_EnsureCharacterDraw(QFEB_REN);
    EV_EnableCharacterDisplay(QFEB_REN);
    SetCharaKill(QFEB_BASE, 1);
    ET_AT_KillWork(work->characters);
    GetNpcBatchCountOverride(work->npcBatchOverride);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_FreeAuthorPak(0, "S2BV");
    LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    camCtrlSetNpcRadius(25);
    SetAreaFogParams();
    SetPlayerControlEnabled(0);
    EV_stepSetFlags(0, 337);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QFEB_RYO), FacialExpressionId::Normal, 5, 1);
    scenFreeEvent("QFEB.EV2");
    EV_SetCanDoAreaJumpTo(1);
    std::int32_t index = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2062) != 0;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2063))
        index = 2;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2064))
        index = 0;
    LoadAreaFunc_Wrapper_0("QTCV.EV3", 0);
    ExecuteCurrAreaMainFunc("QTCV.EV3", reinterpret_cast<std::uintptr_t>(QFEB_RETURN_ARGUMENTS[index]));
}
void FinishQfebEscape(QfebEventWork *work) {
    SetCharaKill(QFEB_BASE, 1);
    if (work->entry >= 1 && work->entry <= 3)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(1989 + work->entry), 0);
    EV_SetCharaControllerFlags(QFEB_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(QFEB_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    ET_AT_KillWork(work->characters);
    GetNpcBatchCountOverride(work->npcBatchOverride);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_FreeAuthorPak(0, "0302");
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    camCtrlSetNpcRadius(25);
    SetAreaFogParams();
    RepositionPlayerForQronFloor(static_cast<std::int32_t>(GetQronNextStream()));
    EV_SetCanDoAreaJumpTo(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void QfebMainCallback(void *data) {
    QfebEventWork *const work = static_cast<QfebEventWork *>(data);
    std::int8_t entry = static_cast<std::int8_t>(work->entry - 1);
    Vector3 ryo;
    EV_GetCharaPosition(QFEB_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::Local);
    switch (work->phase) {
    case 0:
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        EV_LoadCutsceneFromCurrentScene("0302A000");
        EV_LoadAudio("0302A000", 0);
        SetMapControlStatusFlag(0, 0);
        SetCurrentGameFlag(22, 1);
        if (work->entry >= 1 && work->entry <= 3) {
            const std::int16_t readFlag = work->entry == 2 ? 1991 : 1990;
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, readFlag))
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(1989 + work->entry), 1);
            for (std::int32_t index = 1; index < 7; ++index) {
                const shen_name name = QFEB_FIGHT_CHARACTERS[index];
                EventSetPosition position{
                    .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_BATTLE_POSITIONS[entry][index], .delay = 0};
                EVT_AT_SetPosRot(&position);
                EventSetAngle angle{.cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_BATTLE_YAWS[entry][index], .delay = 0};
                EVT_AT_SetAngY_Callback(&angle);
            }
            QueueQfebAuthor(QFEB_AUTHORS[entry]);
        }
        work->phase = 1;
        break;
    case 1:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            EV_AT_SetPos(QFEB_REN, EventTransformFlags::World | EventTransformFlags::AllAxes, QFEB_AUTHOR_RETURN_POSES[entry].position);
            EV_AT_SetAngY(QFEB_REN, EventTransformFlags::World | EventTransformFlags::AllAxes, QFEB_AUTHOR_RETURN_POSES[entry].rotation);
            SetRenderClockAndCompassTo(0);
            ExecuteCleanupFuncForAreaWithCachedScene();
            work->phaseFrames = 0;
            for (std::int32_t flag = 1; flag <= 3; ++flag)
                EV_SetCharaControllerFlags(QFEB_RYO, flag, NpcCharacterControllerState::Enabled);
            work->phase = 2;
        }
        break;
    case 2:
        if (GetMotionAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf009 ||
            GetMotionAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf008) {
            SetPlayerControlEnabled(0);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            shenToggleAreaFlags(2);
            work->phase = 11;
        } else if (GetMotionAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf02b ||
            GetMotionAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf02a) {
            SetPlayerControlEnabled(0);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            shenToggleAreaFlags(2);
            work->phase = 12;
        } else if (ryoNotBusy() && ++work->idleFrames > 300 && GetQronNextStream() == 8 && static_cast<std::uint32_t>(work->entry - 2) <= 1) {
            SetPlayerControlEnabled(0);
            EVT_AT_SetMot(QFEB_RYO, 0xf003, 0, 1, 0x40000000U, 0);
            EV_SetNewSceneIndex();
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            SetRenderClockAndCompassTo(1);
            HLib::SetupTaskDelay(30);
            work->phase = 3;
        } else if (GetSquaredDistanceXZ(&QFEB_PROXIMITY_POINTS[0], &ryo) < 4 || GetSquaredDistanceXZ(&QFEB_PROXIMITY_POINTS[1], &ryo) < 4 ||
            GetSquaredDistanceXZ(&QFEB_PROXIMITY_POINTS[2], &ryo) < 4) {
            SetPlayerControlEnabled(0);
            EVT_AT_SetMot(QFEB_RYO, 0xf003, 0, 1, 0x40000000U, 0);
            EV_SetNewSceneIndex();
            spmFadeOut();
            SetRenderClockAndCompassTo(1);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            work->phase = 3;
        }
        break;
    case 3:
        if (!filmCheckFadeTrackColour()) {
            if (!DoesScenarioCharacterExist(QFEB_REN))
                EV_ForceLoadNpc(QFEB_REN);
            if (EV_ForceLoadNpc(QFEB_REN)) {
                for (std::int32_t index = 0; index < 2; ++index) {
                    const shen_name name = index == 0 ? QFEB_RYO : QFEB_REN;
                    if (index)
                        EV_SetCharacterAlphaEnabled(name, false);
                    mtwkSetRequestedRootMode(scenFindMotionwork(name), 3);
                    SetMotionFrameTime(scenFindMotionwork(name), 7, MotionRegion::LowerBody);
                    EVT_AT_SetMot(name, 0xf003, 0, 1, 0x80000000U, 0);
                    EventSetPosition position{
                        .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_CHOREOGRAPHY_POSITIONS[entry][index], .delay = 0};
                    EVT_AT_SetPosRot(&position);
                    EventSetAngle angle{
                        .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_CHOREOGRAPHY_YAWS[entry][index], .delay = 0};
                    EVT_AT_SetAngY_Callback(&angle);
                }
                EV_LoadCutsceneFromCurrentScene("0302A000");
                EV_LoadAudio("0302A000", 0);
                EV_spmBeginSampleExecute();
                adfsSetCameraIdxForChara(QFEB_RKN, QFEB_CAMERA_RKN);
                adfsSetCameraIdxForChara(QFEB_ANC, QFEB_CAMERA_ANC);
                adfsSetCameraIdxForChara(QFEB_MRC, QFEB_CAMERA_MRC);
                work->phase = 4;
                HLib::SetupTaskDelay(15);
            }
        }
        break;
    case 4:
        if (work->phaseFrames == 15)
            PrepareQfebBattleChoreography(work);
        if (++work->phaseFrames > 445)
            work->phase = 5;
        else if (work->phaseFrames == 415)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    case 5:
        PrepareQfebBattleStances(work);
        break;
    case 6:
        HLib::SetupTaskDelay(work->delay);
        work->phase = 7;
        break;
    case 7:
        work->battleFade = 0;
        work->exitFade = 0;
        work->battleFrames = 360;
        StartQfebBattle(work);
        break;
    case 8: {
        UpdateQfebBattleTaunts(work);
        const std::uint32_t updates = work->battleUpdates++;
        if (updates > 180) {
            if (!work->battleFade) {
                vfmDispatchControl(FightControlOp::SpriteDepth, shen_name{}, 0, 1);
                SetScreenFilmFade(360, 0, 0, 0, 0, 185, 35, 20, 0);
                work->battleFade = 1;
            }
            if (work->battleFrames-- == 1) {
                work->battleFrames = 210;
                work->phase = 9;
            }
        }
        break;
    }
    case 9:
        if (!work->exitFade) {
            SetScreenFilmFade(210, 185, 35, 20, 0, 255, 0, 0, 0);
            work->exitFade = 1;
        }
        if (work->battleFrames-- == 1) {
            check_dead_status(1);
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            EV_FreeAuthorPak(0, "0302");
            work->phase = 10;
        }
        break;
    case 10:
        if (!filmCheckFadeTrackColour()) {
            work->battleFrames = 60;
            work->phaseFrames = 0;
            EV_FreeAuthorPak(1, "S2BV");
            work->phase = 13;
        }
        break;
    case 11:
    case 12:
        if (!filmCheckFadeTrackColour()) {
            shenToggleAreaFlags(1);
            RepositionPlayerForQronFloor(work->phase == 11 ? 7 : 9);
            work->battleFrames = 60;
            HLib::SetupTaskDelay(30);
            work->phase = 16;
        }
        break;
    case 13:
        if (!work->phaseFrames)
            QueueQfebAftermath();
        if (++work->phaseFrames > 320)
            work->phase = 14;
        else if (work->phaseFrames == 290)
            spmFadeOut();
        break;
    case 14:
        RepositionPlayerForQronFloor(7);
        Queue_ST_LoadTitleFadeIn(QFEB_TITLE);
        work->phase = 15;
        break;
    case 15:
        if (work->battleFrames-- == 1) {
            SetAuthorStateFlag(11, 1);
            ST_LoadTitleFade();
            work->battleFrames = 60;
            work->phaseFrames = 0;
            work->phase = 18;
        }
        break;
    case 16: {
        Queue_ST_LoadTitleFadeIn(QFEB_EXIT_TITLE);
        if (GetQronNextStream() == 9)
            entry = 3;
        EVT_AT_SetMot(QFEB_RYO, 0xf002, 0, 1, 0x40000000U, 0);
        EventSetPosition position{.cleanup = 0, .character = QFEB_RYO, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_EXIT_POSITIONS[entry], .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = QFEB_RYO, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_EXIT_YAWS[entry], .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
        PlaceNpcAtAreaPoint(QFEB_REN, shen_name{}, &QFEB_REN_EXIT_POSITIONS[entry], QFEB_REN_EXIT_YAWS[entry]);
        work->phase = 17;
        break;
    }
    case 17:
        if (work->battleFrames-- == 1) {
            ST_LoadTitleFade();
            work->battleFrames = 60;
            work->phase = 19;
        }
        break;
    case 18:
        if (work->battleFrames-- == 1)
            FinishQfebDefeat(work);
        break;
    case 19:
        if (work->battleFrames-- == 1)
            FinishQfebEscape(work);
        break;
    default:
        break;
    }
}
} // namespace
void QfebInit() {
}
void QfebMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QfebMainCallback, 5, 0x0b, sizeof(QfebEventWork), QFEB_TASK);
    QfebEventWork *const work = static_cast<QfebEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->phase = 0;
    work->reserved38 = 0;
    work->npcBatchOverride = GetNpcBatchCountOverride(46);
    work->reserved38 = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, QFEB_CHARACTERS);
    work->entry = static_cast<std::uint8_t>(argument);
    work->delay = 0;
    work->battleUpdates = 0;
    work->battleFrames = 0;
    work->battleFade = 0;
    work->exitFade = 0;
    work->idleFrames = 0;
    work->phaseFrames = 0;
    work->voiceFrames = 0;
    work->previousVoice = 0;
    work->phase = 0;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QFEB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_SetNewSceneIndex();
    FinishLoading();
    SetRenderClockAndCompassTo(1);
    SetFogRed(9.1f);
    SetFogGreen(40);
    SetFogBlue(1);
    SetFogDistance(0x70000);
    ReinitFogTasks(1);
    camCtrlSetNpcRadius(40);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QFEB_BASE);
    EventSetPosition position{
        .cleanup = 0, .character = QFEB_BASE, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_BASE_POSITIONS[work->entry - 1], .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = QFEB_BASE, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_BASE_YAW, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EV_SetCanDoAreaJumpTo(0);
}
const ScriptAreaDefinition qfebScriptDefinition{.areaId = AREA_QFEB, .field04 = 0, .init = QfebInit, .main = QfebMain};
} // namespace shenmue::s2
