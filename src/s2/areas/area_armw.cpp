#include "s2/areas/area_armw.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cstring>
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
ArmwState armw{};
void ArmwNullsub() {
}
std::int32_t GetArmwGameplayActive() {
    return armw.active;
}
namespace {
constexpr const char *refereePackages[]{"ARMWSAMM", "ARMWLLYM"};
constexpr const char *opponentPackages[]{"ARMWSAMR", "ARMWLLYR", "ARMWLLYR", "ARMWHGNR"};
constexpr shen_name referees[]{SHEN_NAME(GAU_), SHEN_NAME(WD3_), SHEN_NAME(AD2_), SHEN_NAME(SD1_), SHEN_NAME(SD1_), SHEN_NAME(SD1_), SHEN_NAME(WD1_),
    SHEN_NAME(WD1_), SHEN_NAME(WD1_), SHEN_NAME(WD1_), SHEN_NAME(WD1_), SHEN_NAME(WD1_)};
constexpr std::int32_t nextEntries[]{20, 21, 22, 0, 0, 0, 20, 21, 22, 0, 0, 0};
std::int32_t leftHandPose[19][3]{{0, 0, 58253}, {0, 2182, 61167}, {0, 0, 5824}, {0, 65172, 53886}, {0, 1635, 52429}, {0, 0, 55705}, {0, 0, 0}, {0, 1456, 56434},
    {0, 0, 50425}, {0, 726, 54796}, {0, 0, 0}, {0, 2183, 57891}, {0, 0, 51518}, {0, 727, 54795}, {0, 182, 0}, {2912, 2001, 53156}, {725, 65353, 52613},
    {0, 910, 54431}, {65518, 255, 0}};
void MainCallback(void *);
void EndCallback(void *);
void Phase(ArmwWork &work, std::int16_t phase, std::int16_t step = 0) {
    work.step = step;
    work.phase = phase;
}
float Tick() {
    return g_stabilizeFps ? GetFrameTime() * 30.0f : 1.0f;
}
void Fade(std::int32_t frames, bool toBlack) {
    SetScreenFilmFade(frames, toBlack ? 0 : 255, 0, 0, 0, toBlack ? 255 : 0, 0, 0, 0);
}
void QueueFade(std::int32_t frames, bool toBlack, std::int32_t delay) {
    EVT_AT_MenCtrl({.cleanup = 0,
        .frames = frames,
        .startAlpha = toBlack ? 0 : 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = toBlack ? 255 : 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay)});
}
void FreePackage(const char *name) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
}
void LoadPackage(const char *name) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(name));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
// @verified - p
void FreeRefereePackages() {
    for (const char *name : refereePackages)
        FreePackage(name);
}
// @verified - p
void FreeOpponentPackages() {
    for (const char *name : opponentPackages)
        FreePackage(name);
}
void LoadOpponentPackage() {
    FreeOpponentPackages();
    LoadPackage(opponentPackages[armw.opponentMotion]);
}
void LoadMatchPackages() {
    LoadPackage("ARMWCOMN");
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ARMWCOMN"), 0);
    armw.handMotion = LoadHandPoseBank(g_actNumber, g_currentMap, "HMOT_0008E");
    LoadPackage(refereePackages[armw.refereeMotion]);
    LoadOpponentPackage();
}
void ReleaseHandMotion() {
    if (armw.handMotion != reinterpret_cast<void *>(~std::uintptr_t{}))
        ReleaseHandPoseBank(armw.handMotion);
}
void PrepareHands(shen_name name) {
    SetLowQualityHands(ctrlGetCharaByName(name));
    SetHighModelXYZ(ctrlGetCharaByName(name), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(name), 1, 1, 0);
}
// @verified - p
void PinBase() {
    const float *base = ARMW_BASES[armw.location];
    ArmwPosition(ARMW_BASE, EVENT_CHARACTER_INVALID, {base[0], base[1], base[2]}, static_cast<std::int32_t>(base[3] * 8192.0f / 45.0f));
}
std::int32_t OpponentMotion(shen_name opponent) {
    if (opponent.name == SHEN_NAME(SFC_) || opponent.name == SHEN_NAME(DGY_) || opponent.name == SHEN_NAME(MHK_))
        return 2;
    const std::int32_t bone = GetMotionRigBoneId(scenFindMotionwork(opponent));
    return bone == 7 ? 0 : bone == 9 ? 1 : 3;
}
std::int32_t Ladder(const ArmwWork &work) {
    return work.flags & 0x40000000U ? armw.location : ARMW_ROTATIONS[work.day][armw.location][work.rotation];
}
// @verified - p
void SelectOpponent(ArmwWork &work) {
    armw.bet = work.flags & 0x10000000U ? 20 : work.flags & 0x20000000U ? 100 : ARMW_BETS[armw.location][work.opponentIndex];
    const ArmwOpponent &opponent = armwOpponents[ARMW_OPPONENT_ORDER[Ladder(work)][work.opponentIndex]];
    work.opponent = opponent.character;
    work.strength = opponent.strength;
    work.minimumStrength = opponent.minimumStrength;
    work.maximumStrength = opponent.maximumStrength;
    work.stamina = opponent.stamina;
    work.initialStamina = opponent.stamina;
    work.strategy = static_cast<std::uint16_t>(opponent.strategy);
    work.warmupFrames = opponent.warmupFrames;
    work.buttonA = opponent.buttonA;
    work.buttonLeft = opponent.buttonLeft;
    work.buttonDown = opponent.buttonDown;
    work.buttonUp = opponent.buttonUp;
    work.finishQte = opponent.finishQte;
    armw.opponentMotion = OpponentMotion(work.opponent);
}
// @verified - p
void ResetAuthFlags() {
    for (std::int32_t flag = 0x23c; flag <= 0x23e; ++flag)
        if (EV_GetShenmueFlag(flag)) {
            EV_stepSetFlags(0, flag);
            break;
        }
}
// @verified - p
void FinishLadderFlags() {
    if (armw.location != 0 || EV_GetShenmueFlag(0x23e))
        for (std::int32_t flag = 0x233; flag <= 0x235; ++flag)
            if (EV_GetShenmueFlag(flag)) {
                EV_stepSetFlags(0, flag);
                break;
            }
}
void SelectCharacters(ArmwWork &work) {
    for (std::int32_t day = 0; day < 7; ++day)
        if (EV_GetShenmueFlag(0x22c + day)) {
            work.day = day;
            break;
        }
    if (armw.location == 0 || armw.location == 1)
        for (std::int32_t rotation = 0; rotation < 3; ++rotation)
            if (EV_GetShenmueFlag(0x233 + 3 * armw.location + rotation)) {
                work.rotation = rotation;
                break;
            }
    work.opponentIndex = static_cast<std::uint8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, ARMW_FLAGS[armw.location]));
    work.flags |= 0x40000000U;
    work.nextScene = shen_name{};
    work.nextEntry = nextEntries[armw.location];
    work.player = ARMW_RYO;
    work.referee = referees[armw.location];
    armw.refereeMotion = GetMotionRigBoneId(scenFindMotionwork(work.referee)) == 7 ? 0 : 1;
    SelectOpponent(work);
    armw.characters[0] = work.player;
    armw.characters[1] = work.referee;
    if (work.flags & 0x20000000U) {
        const std::int32_t ladder = Ladder(work);
        armw.characters[2] = armwOpponents[ARMW_OPPONENT_ORDER[ladder][0]].character;
        armw.characters[3] = armwOpponents[ARMW_OPPONENT_ORDER[ladder][1]].character;
        armw.characters[4] = armwOpponents[ARMW_OPPONENT_ORDER[ladder][2]].character;
        armw.characters[5] = EVENT_CHARACTER_INVALID;
        work.secondOpponent = armw.characters[3];
        work.thirdOpponent = armw.characters[4];
    } else {
        armw.characters[2] = work.opponent;
        armw.characters[3] = EVENT_CHARACTER_INVALID;
    }
    if ((EV_GetShenmueFlag(3) || EV_GetShenmueFlag(2)) && armw.location == 0)
        work.flags |= 8;
}
// @verified - p
void InitializeResources(ArmwWork &work) {
    armw.handMotion = reinterpret_cast<void *>(~std::uintptr_t{});
    EV_SetNewSceneIndex();
    PrepareHands(ARMW_RYO);
    PrepareHands(work.referee);
    PrepareHands(work.opponent);
    if (g_currentMap.name == SHEN_NAME(AR02))
        EV_LoadNPCRoute("NPC_ARAW.BIN");
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(ARMW_RYO));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(ARMW_RYO), FacialNodeReplacement);
    qteBattleUtil(BattleQteMode::Queue);
    qteBattleUtil(BattleQteMode::SetWait, 45, 6, 18);
    qteBattleUtil(BattleQteMode::SetFirstCommands);
    qteBattleUtil(BattleQteMode::SetLastCommands);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    EV_FreeAuthorPak(1, g_currentMap == AREA_QTB4 ? "0368" : "0008");
    ArmwSetVoicePrefixes(work.referee, work.opponent);
    LoadPackage("ARMW");
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ARMW"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ARMW"), 0);
    EV_LoadCutsceneFromCurrentScene("0008A000");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, ARMW_BASE);
    armw.opponentMotion = OpponentMotion(work.opponent);
    if (!EV_GetShenmueFlag(2) || armw.location != 0)
        LoadMatchPackages();
    else {
        for (std::int32_t i = 0; i < 6; ++i)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, ARMW_FLAGS[i], 0);
        LoadPackage("ARMWAUTH");
        armw.handMotion = LoadHandPoseBank(g_actNumber, g_currentMap, "HMOT_0008E");
        Queue_EVT_UTL_LoadNPCChara_3(armw.tutorialCharacters, work.self);
    }
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.player), 0);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.referee), 0);
    if (DoesScenarioCharacterExist(ARMW_ROCKO))
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(ARMW_ROCKO), 0);
    work.phase = 1;
}
std::int32_t Ready(ArmwWork &work) {
    work.falseStarts = 0;
    PinBase();
    ArmwPosition(ARMW_MONEY, ARMW_BASE, {-0.2f, 0.9f, -0.16f}, 0);
    EV_EnableCharacterDisplay(work.player);
    EV_EnableCharacterDisplay(work.opponent);
    EV_EnableCharacterDisplay(work.referee);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    if (work.flags & 0x20000000U) {
        EV_ToggleCharacterFlag(work.secondOpponent);
        EV_DisableCharacterDisplay(work.secondOpponent);
        EV_ToggleCharacterFlag(work.thirdOpponent);
        EV_DisableCharacterDisplay(work.thirdOpponent);
    }
    if (!EV_GetShenmueFlag(2) || armw.location != 0) {
        FinishLoading();
        EV_LoadAudio("0008A000", 1);
        EV_spmBeginSampleExecute();
        Phase(work, 5, 1);
        Fade(30, false);
    } else
        work.phase = 2;
    EV_NPC_NewLoadSw(1);
    return 0;
}
std::int32_t Tutorial(ArmwWork &work) {
    if (work.step) {
        hndlSetFlags(ctrlGetCharaByName(ARMW_SHIYO));
        for (shen_name name : armw.tutorialCharacters) {
            if (name.name == EVENT_CHARACTER_INVALID.name)
                break;
            if (DoesScenarioCharacterExist(name)) {
                EV_npcLockUnlock(name);
                EV_npcFree(name);
            }
        }
        ReleaseHandMotion();
        FreePackage("ARMWAUTH");
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        LoadMatchPackages();
        EV_NPC_NewLoadSw(1);
        Fade(30, false);
        Phase(work, 3);
    } else {
        EV_LoadAudio("0008A000", 0);
        EV_spmBeginSampleExecute();
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
        Fade(30, false);
        QueueSkAuthorStart(work.self, SHEN_NAME(YASA), nullptr, nullptr, 0);
        EV_stepSetFlags(0, 2);
        PrepareHands(ARMW_SHIYO);
        ConfigureCharacterHandPoseSequence(ctrlGetCharaByName(ARMW_HONG), 0, 1025, 0, 1, 0, 4, 1, 0);
        ConfigureCharacterHandPoseSequence(ctrlGetCharaByName(ARMW_SHIYO), 0, 1033, 0, 1, 0, 4, 1, 0);
        ++work.step;
    }
    return 0;
}
std::int32_t WalkAway(ArmwWork &work) {
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    EVT_AT_SetMot(work.player, 9165, 0, 1, 0x40000000U, 40);
    EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[0][armw.opponentMotion], 0, 1, 0x40000000U, 25);
    ArmwVoice(0, "027");
    ArmwVoice(2, "029", 90);
    ArmwCamera(1803, 40);
    EVT_AT_SetPoint(work.referee, work.player, 5, 3, 0, 40, 180, 0);
    work.flags |= 0x20;
    return 180;
}
std::int32_t AskAfterTutorial(ArmwWork &work) {
    if (work.step) {
        if (work.step != 1)
            return 0;
        if (shenGetCtrlSpeakIconFlag() == 22) {
            ArmwVoice(0, "028");
            Phase(work, 5);
            return 30;
        }
    } else {
        EV_LoadAudio("0008A000", 1);
        FinishLoading();
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(work.player), 1, reinterpret_cast<std::byte *>(leftHandPose), 0);
        EVT_AT_SetMot(work.player, 61442, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[0][armw.opponentMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[0][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        ArmwPosition(work.player, ARMW_BASE, {-4.3f, 0, -4.1f}, 44600);
        ArmwPosition(work.opponent, ARMW_BASE, {-1.2f, 0, 0.9f}, 0);
        ArmwPosition(work.referee, ARMW_BASE, {-2.9f, 0, -3.5f}, 11832);
        ArmwCamera(2101);
        if (GetPlayerStat(PlayerStatsId::Cash) > static_cast<std::uint32_t>(armw.bet)) {
            QueueSkChoiceWait(work.self, 0, 22, 153, -1, 153, 0, 10);
            ++work.step;
            return 0;
        }
    }
    const std::int32_t frames = WalkAway(work);
    Phase(work, 20);
    return frames;
}
std::int32_t Explain(ArmwWork &work) {
    std::int32_t frames = 0;
    switch (work.step) {
    case 0:
        EVT_AT_SetVoice("0008E065", 0); // @text EN: "All right!\nThat's the spirit!" | JA: "よっしゃ！\nそうこなくっちゃ！"
        EVT_AT_SetVoice("0008E066", 80); // @text EN: "You're Japanese, aren't you?\nWhat's your name?" | JA: "兄ちゃん日本人だろ。…名前は？"
        EVT_AT_SetVoice("0008A001", 180); // @text EN: "Ryo.\nRyo Hazuki." | JA: "涼…芭月涼だ。"
        EVT_AT_SetVoice("0008E067", 260); // @text EN: "Ryo Hazuki, eh?" | JA: "ハヅキリョウか…"
        EVT_AT_SetVoice("0008E068", 320); // @text EN: "Okay!\nLet's go!" | JA: "よーし！\nさあ、入った入った！"
        ArmwCamera(2130);
        ArmwCamera(2131, 180);
        ArmwCamera(2132, 260);
        EVT_AT_SetMot(work.referee, 9095, 60, 1, 0x40000000U, 260);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.referee), Smile, 0, 1);
        EVT_AT_SetFaceLooks(work.referee, Normal, 0, 1, 460);
        QueueFade(30, true, 400);
        frames = 460;
        ++work.step;
        break;
    case 1:
        Fade(30, false);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        ArmwPosition(work.player, ARMW_BASE, {0.5f, 0, -0.8f}, 28672);
        ArmwPosition(work.referee, ARMW_BASE, {1.2f, 0, 0.6f}, 61440);
        ArmwPosition(work.opponent, ARMW_BASE, {0, 0, 0.8f}, 0);
        EVT_AT_SetMot(work.player, 61443, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[13][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[1][armw.opponentMotion], 0, 1, 0x40000000U, 0);
        ArmwCamera(2150);
        EVT_AT_SetSound(188, 10);
        EVT_AT_SetSound(187, 30);
        EVT_AT_SetSound(271, 40);
        ArmwAppendVoice(work.referee.name == SHEN_NAME(SD1_) ? 1 : 2, "009");
        ArmwAppendVoice(2, "010");
        ArmwWaitVoices(work.self, 0x80000000U);
        ++work.step;
        break;
    case 2:
        if (armw.voiceSkipped) {
            Phase(work, 4, 1);
            Fade(0, true);
            break;
        }
        EVT_AT_SetMot(work.player, 9157, 20, 0.7f, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[13][armw.refereeMotion], 200, 1, 0x40000000U, 0);
        ArmwAppendVoice(1, "011");
        ArmwCamera(2151);
        EVT_AT_SetSound(187, 30);
        EVT_AT_SetSound(188, 40);
        EVT_AT_SetSound(271, 50);
        EVT_AT_SetSound(348, 60);
        ArmwWaitVoices(work.self, 0x80000000U);
        ++work.step;
        break;
    case 3:
        if (armw.voiceSkipped) {
            Phase(work, 4, 1);
            Fade(0, true);
            break;
        }
        EV_DisableCharacterDisplay(work.opponent);
        EVT_AT_SetMot(work.player, 61443, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[13][armw.refereeMotion], 320, 1, 0x40000000U, 0);
        ArmwPosition(work.player, ARMW_BASE, {0.5f, 0, -0.8f}, 28672);
        ArmwAppendVoice(2, "012");
        ArmwAppendVoice(2, "001");
        ArmwCamera(2152);
        ArmwWaitVoices(work.self, 0x80000000U);
        ++work.step;
        break;
    case 4:
        if (armw.voiceSkipped) {
            Phase(work, 4, 1);
            Fade(0, true);
            break;
        }
        EVT_AT_SetSound(187, 20);
        EVT_AT_SetSound(349, 60);
        EVT_AT_SetSound(187, 120);
        frames = ArmwIntroduceOpponent(work);
        ArmwVoice(2, "002");
        ArmwVoice(3, "007", frames - 60);
        Phase(work, 4, 1);
        EV_DisableCharacterDisplay(work.referee);
        EV_EnableCharacterDisplay(work.opponent);
        frames += 30;
        EVT_AT_ObjectDisp_1(work.referee, frames, 0);
        QueueFade(60, true, frames - 60);
        break;
    }
    return frames;
}
std::int32_t PrepareMatch(ArmwWork &work) {
    std::int32_t frames = 0;
    switch (work.step) {
    case 0:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        EV_LoadAudio("0008A000", 1);
        EV_spmBeginSampleExecute();
        Fade(30, false);
        frames = ArmwIntroduceOpponent(work);
        EV_DisableCharacterDisplay(work.referee);
        EVT_AT_ObjectDisp_1(work.referee, frames, 0);
        QueueFade(30, true, frames - 30);
        QueueFade(30, false, frames);
        ++work.step;
        break;
    case 1:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        work.position = 0.5f;
        if (!EV_GetScreenFilmIsNotShowing())
            Fade(30, false);
        EV_EnableCharacterDisplay(work.opponent);
        EV_EnableCharacterDisplay(work.referee);
        ArmwMatchHands(work);
        ArmwUpdatePose(work);
        ArmwPosition(work.referee, ARMW_BASE, {0.7f, 0, 0.05f}, 16384);
        ArmwPosition(ARMW_MONEY, ARMW_BASE, {-0.2f, 0.9f, -0.16f}, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[3][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetPoint(work.player, work.opponent, 5, 3, 0, 0, 0, 0xa0000000U);
        EVT_AT_SetPoint(work.opponent, work.player, 5, 3, 0, 0, 0, 0xa0000000U);
        ArmwCamera(3000);
        armw.introductionVariant = shenGetRandomInt32(2);
        ArmwAppendVoice(3, armw.started ? (armw.introductionVariant ? "003" : "004") : "001");
        ArmwWaitVoices(work.self);
        ++work.step;
        break;
    case 2: {
        constexpr const char *taunts[11][3]{{"002", "003", "004"}, {"029", "030", "031"}, {"034", "035", "036"}, {"039", "040", "041"}, {"043", "044", "045"},
            {"002", "028", "045"}, {"067", "068", "069"}, {"050", "051", "052"}, {"063", "064", "030"}, {"061", "031", "062"}, {"055", "056", "057"}};
        std::int32_t index = 0;
        switch (work.opponent.name) {
        case SHEN_NAME(SF8_):
            index = 1;
            break;
        case SHEN_NAME(SFC_):
            index = 2;
            break;
        case SHEN_NAME(KZN_):
            index = 3;
            break;
        case SHEN_NAME(SSY_):
            index = 4;
            break;
        case SHEN_NAME(SF2_):
            index = 5;
            break;
        case SHEN_NAME(FRS_):
            index = 6;
            break;
        case SHEN_NAME(MHK_):
            index = 7;
            break;
        case SHEN_NAME(DML_):
            index = 8;
            break;
        case SHEN_NAME(MSH_):
            index = 9;
            break;
        case SHEN_NAME(DGY_):
            index = 10;
            break;
        }
        ArmwCamera(3001);
        ArmwAppendVoice(0, taunts[index][armw.started ? (armw.introductionVariant ? 1 : 2) : 0]);
        ArmwWaitVoices(work.self);
        ++work.step;
        break;
    }
    case 3:
        ArmwCamera(3002);
        ArmwAppendVoice(3, armw.started ? (armw.introductionVariant ? "005" : "006") : "002");
        ArmwWaitVoices(work.self);
        ++work.step;
        break;
    case 4:
        if (!armw.started) {
            EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[10][armw.refereeMotion], 0, 1, 0x40000000U, 0);
            ArmwCamera(3003);
            ArmwAppendVoice(1, "013");
            ArmwWaitVoices(work.self);
        }
        ++work.step;
        break;
    case 5: {
        if (!armw.startFrames) {
            armw.started = 1;
            EVT_AT_SetPoint(work.player, shen_name{}, 0, 3, 0, 0, 0, 0x10000000U);
            EVT_AT_SetPoint(work.opponent, shen_name{}, 0, 3, 0, 0, 0, 0x10000000U);
            ArmwMatchHands(work);
            ArmwUpdatePose(work);
            ArmwPosition(work.referee, ARMW_BASE, {0.7f, 0, 0.05f}, 16384);
            EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[11][armw.refereeMotion], 0, 1, 0x40000000U, 0);
            ArmwCamera(3004);
            ArmwVoice(1, "014");
        }
        if (armw.startFrames == 40)
            ArmwCamera(3005);
        const std::int32_t frame = armw.startFrames++;
        if (frame <= 50) {
            if (!(GetCurrentControlInput(INPUT_NEW_PRESSES) & 0x400))
                break;
            CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
            Phase(work, 9);
        } else {
            GetBattleDataConfig(0, ARMW_BATTLE_INPUT);
            EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[12][armw.refereeMotion], 120, 1, 0, 0);
            ArmwVoice(1, "015");
            ArmwResetMatch(work);
            Phase(work, 10);
        }
        armw.startFrames = 0;
        break;
    }
    }
    return frames;
}
std::int32_t Refuse(ArmwWork &work) {
    ArmwAppendVoice(0, "027");
    ArmwAppendVoice(1, "029");
    ArmwAppendVoice(1, "030");
    work.phase = 20;
    ArmwWaitVoices(work.self);
    return 220;
}
std::int32_t Rematch(ArmwWork &work) {
    if (!work.step) {
        const std::int32_t frames = ArmwOfferRematch(work);
        if (shenGetMinutesUntilNextFrameMinute()) {
            ArmwCamera(3205, frames);
            QueueSkChoiceWait(work.self, 0, 22, 153, -1, 153, 0, frames);
            ++work.step;
            return 0;
        }
        EVT_AT_ObjectDisp_1(work.opponent, frames, 1);
        Phase(work, 7);
        return frames;
    }
    if (work.step == 1) {
        if (shenGetCtrlSpeakIconFlag() == 22) {
            ArmwVoice(0, "028");
            Fade(30, true);
            Phase(work, 4, 1);
            return 30;
        }
        Phase(work, 7);
    }
    return 0;
}
std::int32_t FalseStart(ArmwWork &work) {
    std::int32_t frames = ArmwWarnFalseStart(work);
    ++work.falseStarts;
    if (work.falseStarts >= 3) {
        Phase(work, 17, 1);
        frames += 30;
    } else
        Phase(work, 4, 4);
    QueueFade(30, false, frames);
    return frames;
}
// @verified - p
void AdvanceTournament(ArmwWork &work) {
    HasCharacterMotionWork(ctrlGetCharaByName(work.opponent));
    hndlSetFlags(ctrlGetCharaByName(work.opponent));
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    EV_ToggleCharacterFlag(work.opponent);
    EV_DisableCharacterDisplay(work.opponent);
    ++work.opponentIndex;
    work.phase = 4;
    work.falseStarts = 0;
    SelectOpponent(work);
    LoadOpponentPackage();
    EV_EnsureCharacterDraw(work.opponent);
    EV_EnableCharacterDisplay(work.opponent);
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(work.opponent));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(work.opponent), FacialNodeReplacement);
    PrepareHands(work.opponent);
    ArmwSetVoicePrefixes(work.referee, work.opponent);
}
void OpponentLoseMotion(ArmwWork &work, float speed) {
    std::uint32_t motion;
    float frame;
    switch (work.opponent.name) {
    case SHEN_NAME(SF2_):
    case SHEN_NAME(SF8_):
    case SHEN_NAME(DML_):
        EVT_AT_SetAngY(work.opponent, work.opponent, -90, 0);
        motion = 9232;
        frame = 20;
        break;
    case SHEN_NAME(SFC_):
    case SHEN_NAME(MHK_):
        motion = 9297;
        frame = 80;
        break;
    case SHEN_NAME(HNG_):
    case SHEN_NAME(MSH_):
    case SHEN_NAME(SSY_):
        motion = 9299;
        frame = 30;
        break;
    case SHEN_NAME(KZN_):
    case SHEN_NAME(FRS_):
        motion = 9360;
        frame = 70;
        break;
    case SHEN_NAME(DGY_):
        motion = 9295;
        frame = 30;
        break;
    default:
        return;
    }
    EVT_AT_SetMot(work.opponent, motion, frame, speed, 0x40000000U, 0);
}
void QueueCling(shen_name character, std::int32_t bone, const Vector3 &position, const Vector3i &rotation, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventCling &request = *static_cast<EventCling *>(HLib::GetTaskParameter(task));
    request.cleanup = 1;
    request.target = ARMW_MONEY;
    request.character = character;
    request.bone = bone;
    request.position = position;
    request.rotation = rotation;
    request.mode = 0;
    request.delay = static_cast<float>(delay);
}
std::int32_t Win(ArmwWork &work) {
    if (!work.step) {
        ArmwPosition(work.player, ARMW_BASE, {-0.02f, 0, -0.76f}, 32768);
        ArmwPosition(work.opponent, ARMW_BASE, {0.02f, 0, 0.76f}, 0);
        ArmwPosition(work.referee, ARMW_BASE, {1.5f, 0, 0}, 57344);
        EVT_AT_SetAngY(work.referee, ARMW_BASE, 45, 120);
        EVT_AT_SetMot(work.player, 9167, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[3][armw.opponentMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[0][armw.refereeMotion], 0, 1, 0x40000000U, 90);
        ArmwVoice(0, "022", 60);
        Loop3DSoundEffect(349);
        EVT_AT_SetSound(269, 20);
        EVT_AT_SetSound(347, 30);
        EVT_AT_SetSound(349, 10);
        ArmwCamera(shenGetRandomInt32(3) + 3100);
        if (!(work.flags & 0x20000000U) || work.opponentIndex == 2) {
            IncrementSteamStat(20);
            ++work.step;
        } else
            AdvanceTournament(work);
        if (work.flags & 8)
            Phase(work, 18);
        if (work.phase != 14 || work.step != 1)
            return 120;
        work.authorFrames = 120;
        work.step = 10;
        return 0;
    }
    if (work.step == 10) {
        if (HasPressedCancel()) {
            EV_CleanupMapControlTask();
            work.authorFrames = 0;
        }
        work.authorFrames -= Tick();
        if (work.authorFrames <= 0)
            work.step = 1;
        return 0;
    }
    if (work.step != 1)
        return 0;
    ArmwPosition(work.player, ARMW_BASE, {-0.4f, 0, -0.7f}, 32768);
    ArmwPosition(work.referee, work.player, {-0.75f, 0, -0.1f}, 47331);
    ArmwPosition(work.opponent, ARMW_BASE, {0.2f, 0, 0.76f}, 0);
    EVT_AT_SetMot(work.player, 9160, 70, 1, 0x40000000U, 0);
    OpponentLoseMotion(work, 1);
    EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[7][armw.refereeMotion], 70, 1, 0x40000000U, 0);
    QueueCling(work.referee, 21, {0.075f, 0, 0.032f}, {19114, 34133, 41870}, 70);
    QueueCling(work.player, 14, {0.086f, -0.002f, -0.011f}, {49152, 0, 49152}, 120);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventCling &detach = *static_cast<EventCling *>(HLib::GetTaskParameter(task));
    detach.cleanup = 1;
    detach.target = ARMW_MONEY;
    detach.character = shen_name{};
    detach.mode = 1;
    detach.delay = 220;
    ArmwVoice(1, "005", 30);
    ArmwVoice(1, "006", 100);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.referee), Smile, 0, 1);
    EVT_AT_SetFaceLooks(work.referee, Normal, 8, 1, 220);
    ArmwCamera(shenGetRandomInt32(3) + 3103);
    PATI_StartMoneyCount(armw.bet, 90);
    Phase(work, 19);
    return 220;
}
std::int32_t EndLadder(ArmwWork &work) {
    switch (work.step) {
    case 0:
        ArmwPosition(work.player, ARMW_BASE, {-0.02f, 0, -0.76f}, 32768);
        ArmwPosition(work.referee, ARMW_BASE, {0.5f, 0, 0.4f}, 4096);
        EVT_AT_SetMot(work.player, 61443, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[0][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        ArmwAppendVoice(1, "007");
        ArmwCamera(3150);
        ++work.step;
        break;
    case 1:
        ArmwAppendVoice(0, "024");
        ArmwCamera(3151);
        ++work.step;
        break;
    case 2:
        ArmwPosition(work.opponent, ARMW_BASE, {-0.2f, 0, 1.1f}, 61440);
        OpponentLoseMotion(work, 0.6f);
        ArmwAppendVoice(1, "008");
        ArmwAppendVoice(1, "022");
        ArmwCamera(3152);
        EV_DisableCharacterDisplay(ARMW_MONEY);
        ++work.step;
        break;
    case 3:
        ArmwAppendVoice(0, "025");
        ArmwCamera(3151);
        Phase(work, 20);
        break;
    }
    ArmwWaitVoices(work.self);
    return 0;
}
std::int32_t Lose(ArmwWork &work) {
    switch (work.step) {
    case 0:
        ArmwVoice(3, shenGetRandomInt32(2) ? "024" : "025");
        Loop3DSoundEffect(345);
        EVT_AT_SetSound(349, 20);
        EVT_AT_SetSound(3135, 40);
        work.authorFrames = static_cast<float>(ArmwOpponentCollects(work) + 30);
        work.step = 10;
        break;
    case 1: {
        ArmwRefereeCollects(work);
        EVT_AT_SetSound(346, 5);
        ArmwWaitVoices(work.self);
        if (work.falseStarts == 3)
            AddThisToCashOrSegaCoins(-armw.bet, ECashOrCoins::Cash2);
        EventCling detach{.cleanup = 0, .target = ARMW_MONEY, .character = {}, .bone = 0, .position = {}, .rotation = {}, .mode = 1, .delay = 0};
        EVT_AT_Cling(&detach);
        EV_DisableCharacterDisplay(ARMW_MONEY);
        work.authorFrames = 0;
        work.step = 11;
        break;
    }
    case 2:
        ArmwCamera(3391);
        ArmwVoice(3, "026");
        Phase(work, 20);
        return 100;
    case 10:
        if (HasPressedCancel()) {
            EV_CleanupMapControlTask();
            work.authorFrames = 0;
        }
        work.authorFrames -= Tick();
        if (work.authorFrames <= 0) {
            AddThisToCashOrSegaCoins(-armw.bet, ECashOrCoins::Cash2);
            work.step = 1;
        }
        break;
    case 11:
        if (HasPressedCancel())
            work.authorFrames = 0;
        work.authorFrames -= Tick();
        if (work.authorFrames <= 0) {
            EV_CleanupMapControlTask();
            work.step = 2;
        }
        break;
    }
    return 0;
}
std::int32_t StoryWin(ArmwWork &work) {
    switch (work.step) {
    case 0:
        ArmwPosition(work.opponent, ARMW_BASE, {0.2f, 0, 0.76f}, 0);
        EVT_AT_SetMot(work.opponent, 9299, 0, 1, 0x40000000U, 0);
        ArmwVoice(3, "020");
        ArmwCamera(6000);
        EV_DisableCharacterDisplay(work.referee);
        QueueFade(30, true, 60);
        ++work.step;
        return 90;
    case 1:
        EV_LoadAudio("0008A000", 0);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        ResetAuthFlags();
        FreeRefereePackages();
        FreeOpponentPackages();
        LoadPackage("ARMWAUTH");
        EV_NPC_NewLoadSw(1);
        EV_DisableCharacterDisplay(work.opponent);
        EV_EnableCharacterDisplay(work.referee);
        QueueSkNpcWait(work.self);
        ++work.step;
        break;
    case 2:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        QueueSkAuthorStart(work.self, SHEN_NAME(WON_), nullptr, nullptr, 0);
        PATI_StartMoneyCount(armw.bet, 30);
        ++work.step;
        break;
    default:
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        EVT_AT_ObjectDisp_1(work.opponent, 90, 0);
        work.opponentIndex = 1;
        Phase(work, 20);
        break;
    }
    return 0;
}
// @verified - p
bool LoadNextOpponent(ArmwWork &work) {
    EV_CheckWarmStartNpcRoute(work.opponent);
    if (!EV_ForceLoadNpc(work.opponent))
        return false;
    EV_SetCharacterAlphaEnabled(work.opponent, false);
    EV_SetCharaControllerFlags(work.opponent, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(work.opponent, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(work.opponent, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(work.opponent, 1);
    AT_SetLocalBaseChara(work.opponent, EVENT_CHARACTER_INVALID);
    mtwkSetRequestedRootMode(scenFindMotionwork(work.opponent), 3);
    SetMotionFrameTime(scenFindMotionwork(work.opponent), 7, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(work.opponent);
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(work.opponent));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(work.opponent), FacialNodeReplacement);
    PrepareHands(work.opponent);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    return true;
}
std::int32_t NextOpponent(ArmwWork &work) {
    if (!work.step) {
        ++work.opponentIndex;
        if (DoesScenarioCharacterExist(work.opponent) && (work.flags & 0x10000000U)) {
            HasCharacterMotionWork(ctrlGetCharaByName(work.opponent));
            hndlSetFlags(ctrlGetCharaByName(work.opponent));
            EV_npcFree(work.opponent);
            EV_npcLockUnlock(work.opponent);
        }
        ++work.step;
        work.falseStarts = 0;
        if (ARMW_OPPONENT_ORDER[Ladder(work)][work.opponentIndex] == -1) {
            FinishLadderFlags();
            Phase(work, 15);
        } else {
            ResetAuthFlags();
            SelectOpponent(work);
            if (work.flags & 0x10000000U) {
                EV_NPC_NewLoadSw(0);
                EV_ResetFreeTalkFlags();
                LoadOpponentPackage();
                EV_NPC_NewLoadSw(1);
            }
        }
    } else if (work.step == 1 && LoadNextOpponent(work)) {
        if (!EV_GetScreenFilmIsNotShowing())
            Fade(30, false);
        Phase(work, 8);
    }
    return 0;
}
// @verified - p
void EndCallback(void *data) {
    ArmwWork &work = *static_cast<ArmwWork *>(data);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, ARMW_FLAGS[armw.location], work.opponentIndex);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1293, 0);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(ARMW_RYO), 1);
    if (DoesScenarioCharacterExist(ARMW_ROCKO))
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(ARMW_ROCKO), 1);
    SetHighModelXYZ(ctrlGetCharaByName(ARMW_RYO), 0, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARMW_RYO), 0, 1, 0);
    hndlSetFlags(ctrlGetCharaByName(work.opponent));
    hndlSetFlags(ctrlGetCharaByName(work.referee));
    SetCharaKill(ARMW_BASE, 1);
    ET_AT_KillWork(work.characters);
    qteBattleUtil(BattleQteMode::RetireAndFree);
    ReleaseHandMotion();
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    if (DoesScenarioCharacterExist(work.opponent)) {
        EV_npcFree(work.opponent);
        EV_npcLockUnlock(work.opponent);
    }
    EV_FreeAuthorPak(0, g_currentMap == AREA_QTB4 ? "0368" : "0008");
    for (shen_name name : armw.tutorialCharacters) {
        if (name.name == EVENT_CHARACTER_INVALID.name)
            break;
        if (DoesScenarioCharacterExist(name)) {
            EV_npcLockUnlock(name);
            EV_npcFree(name);
        }
    }
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    FreePackage("ARMW");
    FreePackage("ARMWCOMN");
    FreePackage("ARMWAUTH");
    FreeRefereePackages();
    FreeOpponentPackages();
    if (g_currentMap.name == SHEN_NAME(AR02))
        EV_LoadNPCRoute("NPC.BIN");
    EV_stepSetFlags(work.flags & 1 ? 0 : work.flags & 0x20 ? 2 : 1, 4 - ((work.flags & 8) != 0));
    EV_NPC_NewLoadSw(1);
    if (!work.nextScene.name) {
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
        EVT_AT_SetMot(ARMW_RYO, 61442, 0, 1, 0x40000000U, 0);
        // The exit facing goes through EVT_AT_SetAngY, which keeps Ryo's current X and Z rotation.
        const auto place = [](const Vector3 &position, float degrees) {
            EventSetPosition pos{.cleanup = 0, .character = ARMW_RYO, .parent = EVENT_CHARACTER_INVALID, .position = position, .delay = 0};
            EVT_AT_SetPosRot(&pos);
            EventSetAngle angle{.cleanup = 0, .character = ARMW_RYO, .parent = EVENT_CHARACTER_INVALID, .degrees = degrees, .delay = 0};
            EVT_AT_SetAngY_Callback(&angle);
        };
        switch (armw.argument) {
        case 0:
            place({207.9f, 100.22f, 323}, 240);
            break;
        case 1:
            place({227.8f, 100.22f, 351.5f}, 90);
            break;
        case 2:
            place({252, 100.22f, 354.5f}, 0);
            break;
        }
        Fade(30, false);
    } else {
        EV_SetScenarioTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, work.nextScene, work.nextEntry);
    }
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    scenFreeEvent("ARMW.EV3");
}
// @verified - p
void MainCallback(void *data) {
    ArmwWork &work = *static_cast<ArmwWork *>(data);
    if (DoesScenarioCharacterExist(ARMW_BASE))
        PinBase();
    std::int32_t delay = 0;
    switch (work.phase) {
    case 0:
        InitializeResources(work);
        armw.active = 0;
        return;
    case 1:
        delay = Ready(work);
        break;
    case 2:
        delay = Tutorial(work);
        break;
    case 3:
        delay = AskAfterTutorial(work);
        break;
    case 4:
        delay = PrepareMatch(work);
        break;
    case 5:
        delay = Explain(work);
        break;
    case 7:
        delay = Refuse(work);
        break;
    case 8:
        delay = Rematch(work);
        break;
    case 9:
        delay = FalseStart(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 10:
        delay = ArmwPlayMatch(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 11:
        delay = ArmwDirectionQte(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 12:
        delay = ArmwFinishQte(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 13:
        delay = ArmwWinAnimation(work);
        break;
    case 14:
        delay = Win(work);
        break;
    case 15:
        delay = EndLadder(work);
        break;
    case 16:
        delay = ArmwLoseAnimation(work);
        break;
    case 17:
        delay = Lose(work);
        break;
    case 18:
        delay = StoryWin(work);
        break;
    case 19:
        delay = NextOpponent(work);
        break;
    case 20:
        if (EV_GetScreenFilmIsNotShowing()) {
            delay = 90;
            Fade(90, true);
        }
        spmFadeOut();
        HLib::SetCurrentTaskTo(EndCallback);
        break;
    default:
        return;
    }
    armw.active = 0;
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
void QueueArmwMain(void *) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, sizeof(ArmwWork), ARMW_TASK);
    ArmwWork &work = *static_cast<ArmwWork *>(HLib::GetTaskParameter(task));
    std::memset(&work, 0, sizeof(work));
    work.self = task;
    armw.location = g_currentMap.name == ARMW_AREA.name ? static_cast<std::int16_t>((g_entryPoint < 90 ? g_entryPoint : g_entryPoint - 90) % 12) :
                                                          static_cast<std::int16_t>(armw.argument);
    work.step = 0;
    SelectCharacters(work);
    SetWeatherUpdateEnabledTo(true);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    work.characters = Queue_EVT_AT_ReadChr(task, armw.characters);
    ArmwNullsub();
    EV_EventSetUpHelpIcon_0(65, 0);
}
} // namespace
// @verified - p
void InitializeArmwEvent() {
    armw.characters[0] = ARMW_RYO;
    armw.handMotion = nullptr;
    armw.argument = 0;
    armw.qtePenalty = 0;
    armw.location = 0;
    armw.started = 0;
    armw.startFrames = 0;
    armw.introductionVariant = 0;
    armw.qteButton = 0;
    armw.qteSpeaker = 0;
    armw.refereeMotion = 0;
    armw.opponentMotion = 0;
    armw.bet = 0;
    armw.voiceCount = 0;
    std::memset(armw.voicePrefixes, 0, sizeof(armw.voicePrefixes));
    armw.characters[1] = EVENT_CHARACTER_INVALID;
    armw.tutorialCharacters[0] = ARMW_SHIYO;
    armw.tutorialCharacters[1] = EVENT_CHARACTER_INVALID;
    std::memset(armw.voices, 0, sizeof(armw.voices));
    armw.voiceSkipped = 0;
}
// @verified - p
void RunArmwEvent(std::uintptr_t argument) {
    Fade(1, true);
    SetPlayerControlEnabled(0);
    ArmwNullsub();
    armw.argument = static_cast<std::int32_t>(argument);
    SetWeatherUpdateEnabledTo(true);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepNpcClearWork &work = *static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work.callback = QueueArmwMain;
    work.callbackData = nullptr;
    work.reserved10 = 0;
    work.state = 0;
    work.delay = 3;
}
const ScriptAreaDefinition armwScriptDefinition{.areaId = ARMW_AREA, .field04 = 0, .init = InitializeArmwEvent, .main = RunArmwEvent};
} // namespace shenmue::s2::areas
