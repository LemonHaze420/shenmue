#include "s2/areas/area_sfiz.hpp"
#include "s2/areas/area_qf00.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_voice.hpp"
#include "s2/extra_telop.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/game_runtime.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include <cstring>

namespace shenmue::s2 {
namespace {
std::int32_t streetMotion{}, demonstrationMotion{}, izumiMotion{}, admissionUnused{};
Vector3 demonstrationCameraTarget{};
Vector3i demonstrationCameraRotation{};
void IzumiMainCallback(void *);
void EndIzumiBattleCallback(void *);
void SfizAdmissionNullsub() {
}

void HideIzumiActors() {
    for (std::uint32_t index = 1; SFIZ_CHARACTERS[index].name != EVENT_CHARACTER_INVALID.name; ++index) {
        EV_ToggleCharacterFlag(SFIZ_CHARACTERS[index]);
        EV_DisableCharacterDisplay(SFIZ_CHARACTERS[index]);
    }
}

void SetupIzumiMap(float) {
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_SetModelDataDefaultVertexShader();
    SetDefaultVS();
    EV_AssignLightParams(1, 1);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015)) {
        SetMapFieldDepthSlotEnabled(0, 3);
        SetMapFieldDepthSlotEnabled(0, 5);
    }
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    for (std::uint32_t index = 2; index <= 6; ++index)
        AssignLightProgram(index);
    SetupLightConfig();
    for (shen_name character : SFIZ_LIT_MODELS)
        ApplyCharacterGeometryLighting(ctrlGetCharaByName(character), 0);
    Vector4 colour = SFIZ_REFBOARD_COLOUR;
    for (shen_name character : SFIZ_REFBOARD_CHARACTERS)
        SetScenarioReflectorBoardValues(character, &colour);
    SetPluralControlFor(ctrlGetCharaByName(SFIZ_RYO), 1);
}

void EileenAuthorLightCallback(void *) {
    Vector3 position;
    EV_GetCharaPosition(SFIZ_A015, 0xffffffffU, &position, EventTransformFlags::Local);
    SetLightRuntimePosition(64, position);
    EV_GetCharaPosition(SFIZ_A016, 0xffffffffU, &position, EventTransformFlags::Local);
    SetLightRuntimePosition(63, position);
}

void IzumiQteAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 200)
        ResumeParticleEffect(3, static_cast<std::uint32_t>(areas::qf00FloorFbx2Placement.particleHandle));
}

void UpdateDemonstrationCamera() {
    const std::int32_t x = static_cast<std::int32_t>(GetCurrentControlInput(5));
    const std::int32_t y = static_cast<std::int32_t>(GetCurrentControlInput(6));
    GetCurrentControlInput(4);
    const std::int32_t zoom = static_cast<std::int32_t>(GetCurrentControlInput(3));
    Vector3 delta{((x - 128) * 0.0078125f) + ((x - 128) * 0.0078125f), ((y - 128) * 0.0078125f) + ((y - 128) * 0.0078125f), 0};
    ApplyScenarioTranslationAndRotation(&delta, &demonstrationCameraRotation, &delta);
    Vector3 target;
    camCtrlGetTargetPosition(&target);
    target.x += ((demonstrationCameraTarget.x + delta.x) - target.x) * 0.039999999f;
    target.z += ((demonstrationCameraTarget.z + delta.z) - target.z) * 0.039999999f;
    target.y += ((demonstrationCameraTarget.y - delta.y) - target.y) * 0.039999999f;
    camCtrlSetTargetPosition(target);
    const std::int16_t perspective = static_cast<std::int16_t>(camCtrlGetPerspective());
    camCtrlSetFov(static_cast<std::int16_t>(perspective -
        ((((((((60.0f - (static_cast<float>(zoom) * 0.15686275f)) * 8192.0f) / 45.0f) - perspective) / 182.04445f) * 0.039999999f) * -8192.0f) / 45.0f)));
}

std::int32_t LoadIzumiResources(IzumiEventWork *work) {
    EV_SetNewSceneIndex();
    const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("SFIZ"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SFIZ"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("SFIZ"), 0);
    qteBattleUtil(BattleQteMode::SetWait, 15, 6, 20, 0);
    qteBattleUtil(BattleQteMode::SetFirstCommands, 1536, 2304, 0, 0);
    qteBattleUtil(BattleQteMode::SetLastCommands, 0, 0, 0, 0);
    qteBattleUtil(BattleQteMode::ResetStates, 0, 0, 0, 0);
    qteBattleUtil(BattleQteMode::Load, 0, 0, 0, 0);
    EV_FreeAuthorPak(1, "0187");
    EV_FreeAuthorPak(1, "0188");
    std::strcpy(skVoicePrefixes[0], "0187");
    std::strcpy(skVoicePrefixes[1], "0188");
    streetMotion = demonstrationMotion = izumiMotion = -1;
    EV_SetPlayerFlag(static_cast<PlayerFlagWriteMode>(0x0c), 1021, 1);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFIZ_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFIZ_A015);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFIZ_A016);
    SetLowQualityHands(ctrlGetCharaByName(work->opponent));
    SetPluralControlFor(ctrlGetCharaByName(SFIZ_RYO), 0);
    areas::qf00FloorFbx2Placement.particleHandle =
        static_cast<std::int32_t>(CreateFallingParticleEffect(SFIZ_LEAF_POSITION, SFIZ_LEAF_VOLUME, 600, 0.4f, shen_name{}, "misc", "SHENLEAF.pvr", 106.0f));
    SetParticleEffectSize(3, static_cast<std::uint32_t>(areas::qf00FloorFbx2Placement.particleHandle), 0.05f, 0.05f);
    SetParticleEffectVelocity(3, static_cast<std::uint32_t>(areas::qf00FloorFbx2Placement.particleHandle), SFIZ_LEAF_VELOCITY, SFIZ_LEAF_VARIANCE);
    work->state = IzumiEventState::Offer;
    return 0;
}

std::int32_t OfferIzumiBattle(IzumiEventWork *work) {
    if (work->frame) {
        const std::int16_t choice = static_cast<std::int16_t>(shenGetCtrlSpeakIconFlag());
        work->state = choice == 382 ? IzumiEventState::AcceptAuthoredFight : choice == 162 ? IzumiEventState::PayDemonstration : IzumiEventState::Decline;
        work->frame = 0;
        return 0;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_EnsureCharacterDraw(SFIZ_BOOKIE);
    EV_EnableCharacterDisplay(SFIZ_BOOKIE);
    EVT_AT_SetPos(SFIZ_RYO, EVENT_CHARACTER_INVALID, SFIZ_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_RYO, EVENT_CHARACTER_INVALID, 337.5f, 0);
    EVT_AT_SetPos(SFIZ_BOOKIE, EVENT_CHARACTER_INVALID, SFIZ_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_BOOKIE, EVENT_CHARACTER_INVALID, 135.0f, 0);
    EVT_AT_SetMot(SFIZ_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(SFIZ_BOOKIE, 11593, 0, 1, 0x40000000U, 0);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1000, .eye = EVENT_CHARACTER_INVALID, .target = EVENT_CHARACTER_INVALID, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetVoice("0188B001", 1, 0); // @text EN: "Oh? Watching?\nOr are you participating?" | JA: "お？　兄ちゃん、見てくのか？\nそれとも、やる方かい？"
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= 20) {
        QueueSkChoiceWait(work->task, 0, 382, 162, 34, 34, 0, 100);
        ++work->frame;
        return 0;
    }
    ++work->frame;
    work->state = IzumiEventState::InsufficientCash;
    return 140;
}

std::int32_t AcceptIzumiAuthoredFight(IzumiEventWork *work) {
    EVT_AT_SetVoice("0188B005", 1, 0); // @text EN: "Okay.\nIt's $20 to participate." | JA: "そうか。じゃあ、\n参加料は２０ドルだ。"
    AddThisToCashOrSegaCoins(-20, ECashOrCoins::Cash2);
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 120});
    work->state = IzumiEventState::StartAuthor;
    return 150;
}

std::int32_t PayIzumiDemonstration(IzumiEventWork *work) {
    EVT_AT_SetVoice("0188B002", 1, 0); // @text EN: "Really=@it's $20 to watch." | JA: "そうかい。\nじゃあ、観戦料２０ドルだ。"
    AddThisToCashOrSegaCoins(-20, ECashOrCoins::Cash2);
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 120});
    work->state = IzumiEventState::Demonstration;
    return 150;
}

std::int32_t InsufficientIzumiCash(IzumiEventWork *work) {
    EVT_AT_SetVoice("0188A001", 1, 0); // @text EN: "I don't have the money." | JA: "今は持ち合わせが…"
    EVT_AT_SetVoice("0188B004", 1, 60); // @text EN: "Well, forget it then." | JA: "じゃ、あきらめるんだな。"
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 120});
    work->state = IzumiEventState::End;
    return 150;
}

std::int32_t DeclineIzumiBattle(IzumiEventWork *work) {
    EVT_AT_SetVoice("0188B003", 1, 0); // @text EN: "Come again." | JA: "じゃあ、また来てくれよな。"
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 60});
    work->state = IzumiEventState::End;
    return 90;
}

std::int32_t IzumiDemonstration(IzumiEventWork *work) {
    const std::int16_t frame = work->frame;
    if (!frame) {
        const Vector3 eye{206.89999389648438f, 107.9000015258789f, 93.5999984741211f};
        const shen_name demonstrators[]{SFIZ_SF9, SFIZ_SFA};
        std::int32_t choice = shenGetRandomInt32(20);
        EV_LoadCutsceneFromCurrentScene("0187E010");
        EV_LoadAudio("0187E010", 1);
        shen_name opponent{static_cast<std::uint32_t>(choice)};
        if (choice == 19)
            izumiMotion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "SARAH.MOT");
        else {
            demonstrationMotion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
            opponent = demonstrators[choice % 2];
            streetMotion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "STREET.MOT");
            HideIzumiActors();
            EV_EnsureCharacterDraw(SFIZ_EILEEN);
            EV_EnableCharacterDisplay(SFIZ_EILEEN);
            EV_EnsureCharacterDraw(opponent);
            EV_EnableCharacterDisplay(opponent);
            EVT_AT_SetPos(SFIZ_EILEEN, EVENT_CHARACTER_INVALID, SFIZ_PLAYER_POSITION, 0);
            EVT_AT_SetAngY(SFIZ_EILEEN, EVENT_CHARACTER_INVALID, 90, 0);
            EVT_AT_SetPos(opponent, EVENT_CHARACTER_INVALID, SFIZ_OPPONENT_POSITION, 0);
            EVT_AT_SetAngY(opponent, EVENT_CHARACTER_INVALID, 270, 0);
            EVT_AT_SetMot(SFIZ_EILEEN, 0xf003, 0, 1, 0x40000000U, 0);
            EVT_AT_SetMot(opponent, 0xf003, 0, 1, 0x40000000U, 0);
            shenStartFGST(10, 10, 0, 0);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{2}, 1);
            DestroyElementByIndex(UIElement::StreetFighterHud);
            shenInitFightData(ctrlGetCharaByName(SFIZ_EILEEN), nullptr, 1, 0, 500, 19);
            if (opponent.name != SFIZ_IZUMI.name)
                shenInitFightData(ctrlGetCharaByName(opponent), nullptr, 1, 1, 500, 34);
        }
        if (choice == 19 || opponent.name == SFIZ_IZUMI.name)
            shenInitFightData(ctrlGetCharaByName(SFIZ_IZUMI), nullptr, 1, 1, 500, 48);
        vfUpdateFlagsFor(ctrlGetCharaByName(SFIZ_EILEEN), 0, 0x10000, 0);
        vfUpdateFlagsFor(ctrlGetCharaByName(opponent), 0, 0x10000, 0);
        vfmQueueFightStart(2, 99, 1.0f, 0, -1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        demonstrationCameraTarget = {.x = 206.80000305175781f, .y = 108.0999984741211f, .z = 89.69999694824219f};
        camCtrlSetState(CameraState::Event);
        camCtrlSetTargetPosition(demonstrationCameraTarget);
        camCtrlSetNextPosition(eye);
        camCtrlSetFov(0x2aaa);
        EulerRotation16 rotation;
        camCtrlGetRot(&rotation);
        demonstrationCameraRotation = {.x = rotation.pitch, .y = rotation.yaw, .z = rotation.roll};
        CheckHelpIcon(192, 0, CheckHelpIconOp::Set);
        ++work->frame;
        return 0;
    }
    work->frame = static_cast<std::int16_t>(frame + 1);
    if (frame > 10800) {
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        check_dead_status(1);
        work->state = IzumiEventState::End;
        work->frame = 0;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    UpdateDemonstrationCamera();
    GetBattleDataConfig(0, SFIZ_CAMERA_BUTTON);
    if (GetBattleDataConfig(1, SFIZ_CAMERA_BUTTON) && GetBattleDataConfig(2, SFIZ_CAMERA_BUTTON)) {
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        check_dead_status(1);
        work->state = IzumiEventState::End;
        work->frame = 0;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    return 0;
}

void IzumiBattleQte(IzumiEventWork *work) {
    if (work->frame == 1) {
        if (!(EV_vfBattleUtil(FightBattleUtilOp::ActorField, SFIZ_IZUMI, 13) & 1))
            return;
        std::int32_t chance;
        if (GetFightCharacterFlag(0, ctrlGetCharaByName(SFIZ_IZUMI), 0) < 70)
            chance = 10;
        else if (GetFightCharacterFlag(0, ctrlGetCharaByName(SFIZ_IZUMI), 0) < 140)
            chance = 25;
        else
            chance = GetFightCharacterFlag(0, ctrlGetCharaByName(SFIZ_IZUMI), 0) < 250 ? 40 : 100;
        const std::uint32_t opponentFlags = EV_vfBattleUtil(FightBattleUtilOp::ActorField, SFIZ_IZUMI, 0);
        if (((EV_vfBattleUtil(FightBattleUtilOp::ActorField, SFIZ_RYO, 0) | opponentFlags) & 0x7d010) || (GetFightTaskFlags() & 0x10) ||
            shenGetRandomInt32(chance))
            return;
        qteBattleUtil(BattleQteMode::Load);
        qteBattleUtil(BattleQteMode::Queue);
        qteBattleUtil(BattleQteMode::Start);
        ++work->frame;
    } else if (work->frame == 2) {
        const std::int32_t result = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetResult));
        if (!result)
            return;
        if (result == 1) {
            vfmDispatchControl(FightControlOp::QueuedAction, SFIZ_RYO, 0x80000df, 1);
            work->frame = 1;
        } else if (GetFightCharacterFlag(0, ctrlGetCharaByName(SFIZ_RYO), 0) <= 90) {
            check_dead_status(1);
            SetIsSkySetup(0);
            EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
            hmdlSetFlagsFor(ctrlGetCharaByName(SFIZ_IZUMI), 0, 0, 0);
            hmdlSetFlagsFor(ctrlGetCharaByName(SFIZ_IZUMI), 0, 0, 0);
            QueueExtraTelop("IIIIIIIIII", 0x030000c0);
            EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
            QueueSkAuthorStart(work->task, static_cast<std::int32_t>(SFIZ_IZUMI_QTE.name), IzumiQteAuthorCallback, nullptr, 0);
            work->state = IzumiEventState::Defeat;
        } else {
            vfmDispatchControl(FightControlOp::QueuedAction, SFIZ_IZUMI, 0x8000a53, 1);
            HLib::SetupTaskDelay(15);
            ++work->frame;
        }
        qteBattleUtil(BattleQteMode::RetireAndFree);
    } else if (work->frame == 3) {
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
        QueueExtraTelop("IIIIIIIIII", 0);
        ++work->frame;
    } else if (work->frame == 4) {
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 0);
        work->frame = 1;
    }
}

std::int32_t FightIzumi(IzumiEventWork *work) {
    if (!work->frame) {
        HideIzumiActors();
        SetupIzumiMap(0);
        EV_EnsureCharacterDraw(work->opponent);
        EV_EnableCharacterDisplay(work->opponent);
        if (work->opponent.name == SFIZ_IZUMI.name) {
            EV_LoadAudio("0188E000", 1);
            izumiMotion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "SARAH.MOT");
        } else {
            EV_LoadAudio("0187E010", 1);
            streetMotion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "STREET.MOT");
        }
        camCtrlSetState(CameraState::Event);
        camCtrlSetNextPosition(SFIZ_BATTLE_CAMERA);
        camCtrlSetTargetPosition(SFIZ_BATTLE_TARGET);
        shenStartFGST(10, 10, 0, 0);
        shenInitFightData(ctrlGetCharaByName(SFIZ_RYO), nullptr, 0, 0, 0, 0);
        shenInitFightData(ctrlGetCharaByName(work->opponent), nullptr, 1, 1, 500, work->opponent.name == SFIZ_IZUMI.name ? 48 : 19);
        EVT_AT_SetPos(work->opponent, EVENT_CHARACTER_INVALID, SFIZ_OPPONENT_POSITION, 0);
        EVT_AT_SetAngY(work->opponent, EVENT_CHARACTER_INVALID, 270, 0);
        EVT_AT_SetPos(SFIZ_RYO, EVENT_CHARACTER_INVALID, SFIZ_PLAYER_POSITION, 0);
        EVT_AT_SetAngY(SFIZ_RYO, EVENT_CHARACTER_INVALID, 90, 0);
        EVT_AT_SetMot(work->opponent, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(SFIZ_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        vfCopyPosFromEntity(0, nullptr, 207.14999f, 87.800003f, 4.0f);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        vfmQueueFightStart(2, 99, 1.0f, 0, -1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        ++work->frame;
        return 0;
    }
    if (work->opponent.name == SFIZ_IZUMI.name)
        IzumiBattleQte(work);
    if (IsFightActive())
        return 0;
    if (GetBtlStatus(ctrlGetCharaByName(SFIZ_RYO)) == 3) {
        EV_stepSetFlags(0, EV_GetShenmueFlag(995) ? 995 : 996);
        work->state = IzumiEventState::VictoryAuthor;
    } else
        work->state = IzumiEventState::Defeat;
    work->frame = 0;
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    return 30;
}

std::int32_t StartIzumiAuthor(IzumiEventWork *work) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (work->opponent.name == SFIZ_IZUMI.name) {
        EV_LoadCutsceneFromCurrentScene("0188E000");
        EV_LoadAudio("0188E000", 0);
        if (IsAuthorStateFlagSet(38))
            EV_SetAuthorActiveTo(1);
        SetAuthorStateFlag(38, 1);
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(SFIZ_IZUMI_START.name), nullptr, nullptr, 0);
    } else {
        EV_LoadCutsceneFromCurrentScene("0187E010");
        EV_LoadAudio("0187E010", 0);
        if (IsAuthorStateFlagSet(37))
            EV_SetAuthorActiveTo(1);
        SetAuthorStateFlag(37, 1);
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(SFIZ_EILEEN_START.name), EileenAuthorLightCallback, nullptr, 0);
    }
    EV_DisableCharacterDisplay(SFIZ_BOOKIE);
    EV_spmBeginSampleExecute();
    work->state = IzumiEventState::Fight;
    return 0;
}

std::int32_t IzumiVictoryAuthor(IzumiEventWork *work) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    const bool izumi = work->opponent.name == SFIZ_IZUMI.name;
    EV_LoadAudio(izumi ? "0188E000" : "0187E010", 2);
    QueueSkAuthorStart(work->task, static_cast<std::int32_t>((izumi ? SFIZ_IZUMI_WIN : SFIZ_EILEEN_WIN).name), nullptr, nullptr, 0);
    work->state = IzumiEventState::VictoryReply;
    return 0;
}

std::int32_t IzumiVictoryReply(IzumiEventWork *work) {
    if (work->frame == 1) {
        PlaySkSecondPrefixVoice(0, "B008");
        EventSetCamera camera{
            .cleanup = 0, .camera = 3000, .eye = EVENT_CHARACTER_INVALID, .target = EVENT_CHARACTER_INVALID, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        ++work->frame;
        return 120;
    }
    if (work->frame == 2) {
        work->frame = 3;
        AddInventoryItem(26, 1, 1);
        return 120;
    }
    if (work->frame == 3) {
        if (work->opponent.name == SFIZ_EILEEN.name) {
            EventSetCamera camera{
                .cleanup = 0, .camera = 3000, .eye = EVENT_CHARACTER_INVALID, .target = EVENT_CHARACTER_INVALID, .context = 0, .angle = 0, .delay = 0};
            EVT_AT_SetCam_Callback(&camera);
            camCtrlDisableFilmBars();
        }
        PlaySkSecondPrefixVoice(1, "B003");
        work->state = IzumiEventState::End;
        work->frame = 0;
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 60});
        return 90;
    }
    if (work->frame)
        return 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupIzumiMap(0);
    HideIzumiActors();
    EV_EnsureCharacterDraw(SFIZ_RYO);
    EV_EnableCharacterDisplay(SFIZ_RYO);
    EV_EnsureCharacterDraw(SFIZ_BOOKIE);
    EV_EnableCharacterDisplay(SFIZ_BOOKIE);
    PlaySkSecondPrefixVoice(0, "B007");
    EVT_AT_SetPos(SFIZ_RYO, EVENT_CHARACTER_INVALID, SFIZ_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_RYO, EVENT_CHARACTER_INVALID, 337.5f, 0);
    EVT_AT_SetPos(SFIZ_BOOKIE, EVENT_CHARACTER_INVALID, SFIZ_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_BOOKIE, EVENT_CHARACTER_INVALID, 135, 0);
    EVT_AT_SetMot(SFIZ_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(SFIZ_BOOKIE, 11593, 0, 1, 0x40000000U, 0);
    EventSetCamera camera{
        .cleanup = 0, .camera = 3100, .eye = EVENT_CHARACTER_INVALID, .target = EVENT_CHARACTER_INVALID, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    work->frame = work->opponent.name == SFIZ_IZUMI.name ? 1 : 3;
    return 50;
}

void IzumiDefeatDialogue() {
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    if (areas::qf00FloorFbx2Placement.particleHandle != -1) {
        SuspendParticleEffect(3, static_cast<std::uint32_t>(areas::qf00FloorFbx2Placement.particleHandle));
        FreeParticleEffect(3, static_cast<std::uint32_t>(areas::qf00FloorFbx2Placement.particleHandle));
        areas::qf00FloorFbx2Placement.particleHandle = -1;
    }
    SetIsSkySetup(1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(SFIZ_IZUMI), Smile, 0, 1);
    EVT_AT_SetPos(SFIZ_IZUMI, EVENT_CHARACTER_INVALID, SFIZ_IZUMI_DEFEAT_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_IZUMI, EVENT_CHARACTER_INVALID, 180, 0);
    hmdlSetFlagsFor(ctrlGetCharaByName(SFIZ_IZUMI), 0, 8, 0);
    hmdlSetFlagsFor(ctrlGetCharaByName(SFIZ_IZUMI), 0, 8, 0);
    if (shenGetRandomInt32(2)) {
        const std::int32_t choice = shenGetRandomInt32(50);
        EventSetCamera camera{.cleanup = 0,
            .camera = choice ? 2500 : 2501,
            .eye = EVENT_CHARACTER_INVALID,
            .target = EVENT_CHARACTER_INVALID,
            .context = 0,
            .angle = 0,
            .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetMot(SFIZ_IZUMI, 11651, 0, 1, 0x40000000U, 0);
    } else {
        EventSetCamera camera{
            .cleanup = 0, .camera = 2502, .eye = EVENT_CHARACTER_INVALID, .target = EVENT_CHARACTER_INVALID, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetMot(SFIZ_IZUMI, 11652, 0, 1, 0x40000000U, 0);
    }
    AppendSkPendingVoice(1, SFIZ_IZUMI_DEFEAT_VOICES[shenGetRandomInt32(7)]);
}

void EileenDefeatDialogue() {
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(SFIZ_EILEEN), Normal, 0, 1);
    EVT_AT_SetMot(SFIZ_EILEEN, 11587, 0, 0.5f, 0x40000000U, 0);
    EVT_AT_SetPos(SFIZ_EILEEN, EVENT_CHARACTER_INVALID, SFIZ_EILEEN_DEFEAT_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_EILEEN, EVENT_CHARACTER_INVALID, 180, 0);
    EVT_AT_SetAngY(SFIZ_EILEEN, EVENT_CHARACTER_INVALID, 337.5f, 0);
    const std::int32_t choice = shenGetRandomInt32(2);
    EventSetCamera camera{
        .cleanup = 0, .camera = choice ? 2000 : 2010, .eye = EVENT_CHARACTER_INVALID, .target = EVENT_CHARACTER_INVALID, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    AppendSkPendingVoice(0, SFIZ_EILEEN_DEFEAT_VOICES[shenGetRandomInt32(8)]);
}

std::int32_t IzumiDefeat(IzumiEventWork *work) {
    if (!work->frame) {
        SetupIzumiMap(0);
        EV_DisableCharacterDisplay(SFIZ_RYO);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(work->opponent), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(work->opponent), 1, 1, 0);
        if (work->opponent.name == SFIZ_IZUMI.name)
            IzumiDefeatDialogue();
        else
            EileenDefeatDialogue();
        ++work->frame;
        QueueSkPendingVoiceWait(work->task);
        return 0;
    }
    std::int32_t delay = 0;
    if (work->frame == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), EVENT_CHARACTER_TASK);
        RegisterEventTask(task);
        StepObjectDisplayWork *const display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
        display->cleanup = 1;
        display->character = SFIZ_RYO;
        display->delay = 60.0f;
        EventFilmControl film{.cleanup = 0,
            .frames = 60,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&film);
        ++work->frame;
        delay = 60;
    } else if (work->frame != 2)
        return 0;
    work->state = IzumiEventState::DefeatReply;
    work->frame = 0;
    return delay;
}

std::int32_t IzumiDefeatReply(IzumiEventWork *work) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 150});
    HideIzumiActors();
    EV_EnsureCharacterDraw(SFIZ_RYO);
    EV_EnableCharacterDisplay(SFIZ_RYO);
    EV_EnsureCharacterDraw(SFIZ_BOOKIE);
    EV_EnableCharacterDisplay(SFIZ_BOOKIE);
    PlaySkSecondPrefixVoice(0, "B006");
    EventSetCamera camera{
        .cleanup = 0, .camera = 3000, .eye = EVENT_CHARACTER_INVALID, .target = EVENT_CHARACTER_INVALID, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetPos(SFIZ_RYO, EVENT_CHARACTER_INVALID, SFIZ_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_RYO, EVENT_CHARACTER_INVALID, 337.5f, 0);
    EVT_AT_SetPos(SFIZ_BOOKIE, EVENT_CHARACTER_INVALID, SFIZ_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_BOOKIE, EVENT_CHARACTER_INVALID, 135, 0);
    EVT_AT_SetMot(SFIZ_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(SFIZ_BOOKIE, 11593, 0, 1, 0x40000000U, 0);
    work->state = IzumiEventState::End;
    return 180;
}

void EndIzumiBattleCallback(void *parameter) {
    IzumiEventWork *const work = static_cast<IzumiEventWork *>(parameter);
    EVT_AT_SetPos(SFIZ_RYO, EVENT_CHARACTER_INVALID, SFIZ_EXIT_POSITION, 0);
    EVT_AT_SetAngY(SFIZ_RYO, EVENT_CHARACTER_INVALID, 0, 0);
    EVT_AT_SetMot(SFIZ_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EV_EnableCharacterDisplay(SFIZ_RYO);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_FreeAuthorPak(0, "0187");
    EV_FreeAuthorPak(0, "0188");
    if (areas::qf00FloorFbx2Placement.particleHandle != -1) {
        SuspendParticleEffect(3, static_cast<std::uint32_t>(areas::qf00FloorFbx2Placement.particleHandle));
        FreeParticleEffect(3, static_cast<std::uint32_t>(areas::qf00FloorFbx2Placement.particleHandle));
        areas::qf00FloorFbx2Placement.particleHandle = -1;
    }
    if (streetMotion != -1)
        DeleteMotion(streetMotion);
    if (demonstrationMotion != -1)
        DeleteMotion(demonstrationMotion);
    if (izumiMotion != -1)
        DeleteMotion(izumiMotion);
    ET_AT_KillWork(work->characters);
    EV_SetPlayerFlag(static_cast<PlayerFlagWriteMode>(0x0c), 1021, 0);
    SetCharaKill(SFIZ_BASE, 1);
    SetCharaKill(SFIZ_A015, 1);
    SetCharaKill(SFIZ_A016, 1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SFIZ"), 0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    scenFreeEvent("SFIZ.EV3");
    SetupIzumiMap(1);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_NPC_NewLoadSw(1);
    SetPluralControlFor(ctrlGetCharaByName(SFIZ_RYO), 1);
    FinishLoading();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}

void IzumiMainCallback(void *parameter) {
    IzumiEventWork *const work = static_cast<IzumiEventWork *>(parameter);
    std::int32_t delay;
    switch (work->state) {
    case IzumiEventState::Load:
        delay = LoadIzumiResources(work);
        break;
    case IzumiEventState::Offer:
        delay = OfferIzumiBattle(work);
        break;
    case IzumiEventState::InsufficientCash:
        delay = InsufficientIzumiCash(work);
        break;
    case IzumiEventState::Decline:
        delay = DeclineIzumiBattle(work);
        break;
    case IzumiEventState::AcceptAuthoredFight:
        delay = AcceptIzumiAuthoredFight(work);
        break;
    case IzumiEventState::PayDemonstration:
        delay = PayIzumiDemonstration(work);
        break;
    case IzumiEventState::Demonstration:
        delay = IzumiDemonstration(work);
        break;
    case IzumiEventState::Fight:
        delay = FightIzumi(work);
        break;
    case IzumiEventState::StartAuthor:
        delay = StartIzumiAuthor(work);
        break;
    case IzumiEventState::VictoryAuthor:
        delay = IzumiVictoryAuthor(work);
        break;
    case IzumiEventState::VictoryReply:
        delay = IzumiVictoryReply(work);
        break;
    case IzumiEventState::Defeat:
        delay = IzumiDefeat(work);
        break;
    case IzumiEventState::DefeatReply:
        delay = IzumiDefeatReply(work);
        break;
    case IzumiEventState::End:
        spmFadeOut();
        HLib::SetCurrentTaskTo(EndIzumiBattleCallback);
        HLib::SetupTaskDelay(30);
        return;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}

void QueueIzumiBattle(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(IzumiMainCallback, 5, 0x0b, sizeof(IzumiEventWork), EVENT_CHARACTER_TASK);
    IzumiEventWork *const work = static_cast<IzumiEventWork *>(HLib::GetTaskParameter(task));
    work->opponent = shen_name{};
    work->state = IzumiEventState::Load;
    work->frame = 0;
    work->characters = nullptr;
    work->task = task;
    work->state = IzumiEventState::Load;
    work->frame = 0;
    work->opponent = EV_GetShenmueFlag(995) ? SFIZ_EILEEN : SFIZ_IZUMI;
    work->characters = Queue_EVT_AT_ReadChr(task, SFIZ_CHARACTERS);
}

void InitializeIzumi() {
    areas::qf00FloorFbx2Placement.particleHandle = -1;
    streetMotion = demonstrationMotion = izumiMotion = admissionUnused = 0;
    demonstrationCameraTarget = {};
    demonstrationCameraRotation = {};
}

void StartIzumi(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), EVENT_CHARACTER_TASK);
    RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueIzumiBattle;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3.0f;
    SfizAdmissionNullsub();
}
} // namespace
const ScriptAreaDefinition areaSfizScriptDefinition{.areaId = SFIZ_AREA, .field04 = 0, .init = InitializeIzumi, .main = StartIzumi};
} // namespace shenmue::s2
