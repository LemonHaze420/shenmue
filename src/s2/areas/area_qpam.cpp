#include "s2/areas/area_qpam.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {
void QpamInitNullsub() {
}

void QpamIdentify(MapStandEventWork &work) {
    switch (work.stand.name) {
    case SHEN_NAME(PD50):
        EV_stepSetFlags(0, 0x1b1);
        work.item = 6019;
        work.map = SHEN_NAME(PQR0);
        break;
    case SHEN_NAME(PD51):
    case SHEN_NAME(PD55):
        EV_stepSetFlags(0, 0x1b1);
        work.item = 6020;
        work.map = SHEN_NAME(PQ10);
        break;
    case SHEN_NAME(PD64):
    case SHEN_NAME(PD65):
        EV_stepSetFlags(0, 0x1b1);
        work.item = 6021;
        work.map = SHEN_NAME(PQ30);
        break;
    case SHEN_NAME(PD60):
    case SHEN_NAME(PD59):
        EV_stepSetFlags(0, 0x1b1);
        work.item = 6022;
        work.map = SHEN_NAME(PQ20);
        break;
    case SHEN_NAME(PD61):
    case SHEN_NAME(PD62):
    case SHEN_NAME(PD63):
    case SHEN_NAME(PD56):
        EV_stepSetFlags(0, 0x1b2);
        work.item = 6001;
        work.map = SHEN_NAME(PQA0);
        break;
    case SHEN_NAME(PD52):
        EV_stepSetFlags(0, 0x1b3);
        work.item = 6002;
        work.map = SHEN_NAME(PQB0);
        break;
    case SHEN_NAME(PD53):
        EV_stepSetFlags(0, 0x1b5);
        work.item = 6004;
        work.map = SHEN_NAME(PQD0);
        break;
    case SHEN_NAME(PD54):
        EV_stepSetFlags(0, 0x1b4);
        work.item = 6003;
        work.map = SHEN_NAME(PQC0);
        break;
    case SHEN_NAME(PD57):
    case SHEN_NAME(PD58):
        EV_stepSetFlags(0, 0x1b6);
        work.item = 6005;
        work.map = SHEN_NAME(PQE0);
        break;
    default:
        break;
    }
}

void QpamDescribe(MapStandEventWork &work, std::int32_t mode) {
    std::int32_t reply = 0;
    const char *voice = nullptr;
    if (mode == 0)
        work.conversation = EV_PlayDialogue(GetRandomNumberInRange(2) == 0 ? "0099A003" : "0099A004"); // @text 0099A003 EN: "I'll get it later." | JA: "金がないな…\nあとにするか。" ; 0099A004 EN: "Not enough to buy one." | JA: "今は持ち合わせがないか…"
    else if (mode == 1)
        EV_PlayDialogue(GetRandomNumberInRange(2) == 0 ? "0099A001" : "0099A002"); // @text 0099A001 EN: "Huh?\nA map=@" | JA: "ん？　地図か…" ; 0099A002 EN: "Maps." | JA: "地図を売ってるな…"
    else if (mode == 2)
        EV_PlayDialogue("0099A033"); // @text EN: "Maybe later." | JA: "あとにしよう…"
    else if (mode == 3) {
        switch (work.stand.name) {
        case SHEN_NAME(PD50):
            voice = "0099A013"; // @text EN: "Dragon St." | JA: "龍影街の地図か…"
            reply = 1;
            break;
        case SHEN_NAME(PD51):
            voice = "0099A014"; // @text EN: "Thousand White Qr." | JA: "白千街の地図か…"
            reply = 2;
            break;
        case SHEN_NAME(PD55):
            voice = "0099A014"; // @text EN: "Thousand White Qr." | JA: "白千街の地図か…"
            reply = 1;
            break;
        case SHEN_NAME(PD60):
        case SHEN_NAME(PD59):
            voice = "0099A015"; // @text EN: "Stand Qr." | JA: "屋台街の地図か…"
            reply = 1;
            break;
        case SHEN_NAME(PD64):
        case SHEN_NAME(PD65):
            voice = "0099A016"; // @text EN: "Dimsum Qr." | JA: "点心街の地図か…"
            reply = 1;
            break;
        case SHEN_NAME(PD52):
            voice = "0099A019"; // @text EN: "Great View Bldg." | JA: "安見楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD54):
            voice = "0099A020"; // @text EN: "Thousand White Bldg." | JA: "白千楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD56):
            voice = "0099A041"; // @text EN: "Dancing Dragon Bldg." | JA: "龍喜楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD62):
            voice = "0099A042"; // @text EN: "Dimsum Bldg." | JA: "点心楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD63):
            voice = "0099A043"; // @text EN: "Tea Break Bldg." | JA: "飲茶楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD61):
            voice = "0099A044"; // @text EN: "Three Birds Bldg." | JA: "三鳥楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD58):
            voice = "0099A045"; // @text EN: "Black Heaven Bldg." | JA: "玄天楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD53):
            voice = "0099A046"; // @text EN: "Ghost Hall Bldg." | JA: "不幽楼の地図か…"
            reply = 3;
            break;
        case SHEN_NAME(PD57):
            voice = "0099A048"; // @text EN: "Moon Child Bldg." | JA: "百子楼の地図か…"
            reply = 3;
            break;
        default:
            break;
        }
        if (voice)
            EVT_AT_SetVoice(reply == 3 && work.mode == 2 ? "0099A018" : voice, 0); // @text EN: "A building nearby." | JA: "このあたりのビルの地図か…"
    }
    if (work.mode == 2) {
        const float selection = GetRandomNumberInRange(4);
        voice = selection == 0 ? "0099A037" : selection == 1 ? "0099A038" : selection == 2 ? "0099A039" : "0099A040"; // @text 0099A037 EN: "I already have one." | JA: "もう持っていたな。" ; 0099A038 EN: "I have one." | JA: "これはもう持ってるな。" ; 0099A039 EN: "I have it.\nDon't need another." | JA: "これは持ってる。いらないな…" ; 0099A040 EN: "I have this one." | JA: "これは持っていたな。"
    } else if (reply == 1)
        voice = GetRandomNumberInRange(2) == 0 ? "0099A025" : "0099A026"; // @text 0099A025 EN: "Good." | JA: "役に立ちそうだ。" ; 0099A026 EN: "It'll be useful." | JA: "これで迷わなくてすむな。"
    else if (reply == 2)
        voice = GetRandomNumberInRange(2) == 0 ? "0099A028" : "0099A029"; // @text 0099A028 EN: "I'll get it." | JA: "持っておこう。" ; 0099A029 EN: "It'll come in handy." | JA: "あとで使えるな。"
    else if (reply == 3)
        voice = GetRandomNumberInRange(2) == 0 ? "0099A031" : "0099A032"; // @text 0099A031 EN: "I'll need it." | JA: "使えそうだな。" ; 0099A032 EN: "Good." | JA: "助かるな…"
    else
        return;
    EVT_AT_SetVoice(voice, 55);
}

std::int32_t QpamBuy(MapStandEventWork &work) {
    switch (work.phase) {
    case 10: {
        mtwkSetRequestedRootMode(scenFindMotionwork(QPAM_RYO), 0);
        EVT_AT_SetMot(QPAM_RYO, 0xf002, 0, 1, 0x40000000U, 0);
        EV_SetRefBoardParamsFor(QPAM_RYO, 1);
        EventSetPosition position{
            .cleanup = 0, .character = QPAM_RYO, .parent = work.stand, .position = {-.664900004863739f, 0, -.06469999998807907f}, .delay = 0};
        EVT_AT_SetPosRot(&position);
        EVT_AT_SetAng_Enqueue(QPAM_RYO, work.stand, 0, 270, 0, 0);
        EventSetCamera camera{.cleanup = 0, .camera = 1220, .eye = work.stand, .target = work.stand, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        if (SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(~std::uintptr_t{}))) {
            SetHighModelXYZ(ctrlGetCharaByName(QPAM_RYO), 1, 1, 0);
            std::array<std::int32_t, 57> hand = qpamRestingHand;
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(QPAM_RYO), 1, reinterpret_cast<std::byte *>(hand.data()), 0);
        }
        HLib::SetupTaskDelay(16);
        work.phase = 0;
        return 0;
    }
    case 0:
        if (GetPlayerStat(PlayerStatsId::Cash) < static_cast<std::uint32_t>(work.cost)) {
            AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash2);
            QpamDescribe(work, 0);
            work.mode = 99;
            return 120;
        }
        QpamDescribe(work, 1);
        ++work.phase;
        return 40;
    case 1:
        if (work.initialChoice) {
            EventFilmControl film{.cleanup = 0,
                .frames = 30,
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
            work.phase = 2;
            return 0;
        } else {
            std::int16_t choices[]{-1, 40, 41, -1};
            SetupControlSpeakAndUI(choices, 600, -1, 2);
            ++work.phase;
            return 0;
        }
    case 2:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return 0;
        if (shenGetCtrlSpeakIconFlag() != 40) {
            work.mode = 99;
            QpamDescribe(work, 2);
            EVT_AT_MenCtrl({1, 16, 0, 0, 0, 0, 255, 0, 0, 0, 14});
            return 30;
        }
        if (GetPlayerStat(PlayerStatsId::Cash) < static_cast<std::uint32_t>(work.cost)) {
            AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash2);
            QpamDescribe(work, 0);
            EVT_AT_MenCtrl({1, 16, 0, 0, 0, 0, 255, 0, 0, 0, 104});
            work.mode = 99;
            return 120;
        }
        ++work.phase;
        return 0;
    case 3: {
        Character *const character = ctrlGetCharaByName(QPAM_RYO);
        SetHighModelXYZ(character, 1, 0, 0);
        const Vector3i rotation{61529, 2501, 364};
        ApplyCharacterHandRotation(character, 0, 21, &rotation);
        std::array<std::int32_t, 57> hand = qpamBuyingHand;
        ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(hand.data()), 120);
        Vector3 standPosition;
        EV_GetCharaPosition(work.stand, CHARACTER_TRANSLATION_ROOT, &standPosition, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(QPAM_RYO), &standPosition, 0);
        EV_SetRefBoardParamsFor(work.map, 1);
        AddThisToCashOrSegaCoins(-work.cost, ECashOrCoins::Cash2);
        QueueEventRepeatedFunction(map_stand::GiveMap, &work.item, 120, 1);
        QpamDescribe(work, 3);
        EnableScenarioCharacterDisplay(QPAM_RYO);
        map_stand::QueueDisplay(work.map, 0, 86);
        map_stand::QueueDisplay(work.map, 1, 257);
        map_stand::QueueCling({1, work.map, QPAM_RYO, 14, {.10199999809265137f, -.04100000113248825f, -.14499999582767487f}, {348, 22935, 13482}, 0, 86});
        map_stand::QueueCling({1, work.map, QPAM_RYO, 14, {.07100000232458115f, .007000000216066837f, -.16200000047683716f}, {62608, 23298, 9113}, 0, 110});
        map_stand::QueueCling({1, work.map, {}, 0, {}, {}, 1, 257});
        EventSetPosition position{
            .cleanup = 0, .character = QPAM_RYO, .parent = work.stand, .position = {-.664900004863739f, 0, -.06469999998807907f}, .delay = 0};
        EVT_AT_SetPosRot(&position);
        EVT_AT_SetAng_Enqueue(QPAM_RYO, work.stand, 0, 270, 0, 0);
        EVT_AT_SetMot(QPAM_RYO, 0x1541, 0, 1, 0x40000000U, 1);
        EVT_AT_SetMot(QPAM_RYO, 0x1541, 112, 0, 0x40000000U, 110);
        EVT_AT_SetMot(QPAM_RYO, 0x1541, 150, 1, 0x40000000U, 220);
        EventSetCamera camera{.cleanup = 0, .camera = 2010, .eye = work.stand, .target = work.stand, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetCam(2020, work.stand, work.stand, 0, 0, 110);
        EVT_AT_SetCam(2030, work.stand, work.stand, 0, 0, 220);
        work.mode = 99;
        EVT_AT_MenCtrl({1, 16, 0, 0, 0, 0, 255, 0, 0, 0, 270});
        return 286;
    }
    default:
        return 0;
    }
}

std::int32_t QpamOwned(MapStandEventWork &work) {
    mtwkSetRequestedRootMode(scenFindMotionwork(QPAM_RYO), 0);
    EventSetMotion motion{.cleanup = 0, .character = QPAM_RYO, .animation = 0xf002, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    EventSetCamera camera{.cleanup = 0, .camera = 2010, .eye = work.stand, .target = work.stand, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EventSetPosition position{.cleanup = 0, .character = QPAM_RYO, .parent = work.stand, .position = {-.664900004863739f, 0, -.06469999998807907f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{.cleanup = 0, .character = QPAM_RYO, .parent = work.stand, .rotation = {0, 49152, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    QpamDescribe(work, 3);
    work.mode = 99;
    EVT_AT_MenCtrl({1, 16, 0, 0, 0, 0, 255, 0, 0, 0, 83});
    return 100;
}

void QpamMain(std::uintptr_t request) {
    const std::uint32_t *const input = reinterpret_cast<const std::uint32_t *>(request);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QpamCallback, 5, 0x0b, sizeof(MapStandEventWork), AREA_EVENT_TASK);
    MapStandEventWork *const work = static_cast<MapStandEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->mode = static_cast<std::int32_t>(input[1]);
    work->phase = 10;
    work->map = {};
    work->initialChoice = 0;
    if (static_cast<std::uint32_t>(request) >= 99)
        work->stand = shen_name{input[0]};
    else {
        const std::uint32_t index = static_cast<std::uint32_t>(request) % 100;
        work->stand = shen_name{SHEN_NAME(PD00) + (index % 10) * 0x01000000U + (index / 10) * 0x00010000U};
    }
    QpamIdentify(*work);
    if (static_cast<std::uint8_t>(GetItemState(static_cast<std::uint32_t>(work->item))))
        work->mode = 2;
    work->cost = 10;
    const Vector4 reflector{1, 1, 1, 1};
    SetScenarioReflectorBoardValues(QPAM_RYO, &reflector);
    EV_SetRefBoardParamsFor(QPAM_RYO, 1);
    EV_SetNewSceneIndex();
    EV_EventSetUpHelpIcon_0(0x75, 0);
    SetPlayerControlEnabled(0);
    EventFilmControl film{.cleanup = 0,
        .frames = 16,
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
    HLib::SetupTaskDelay(-1);
    EV_SetCharaControllerFlags(QPAM_RYO, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(QPAM_RYO, 3, NpcCharacterControllerState::Disabled);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QPAM"), 0);
    EV_FreeAuthorPak(1, "0099");
    EV_LoadCutsceneFromCurrentScene("0099E000.smp");
}

void QpamCallback(void *data) {
    MapStandEventWork &work = *static_cast<MapStandEventWork *>(data);
    std::int32_t delay = 0;
    switch (work.mode) {
    case 0:
        delay = map_stand::Intro(work);
        break;
    case 1:
        delay = QpamBuy(work);
        break;
    case 2:
        delay = QpamOwned(work);
        break;
    case 99:
        HLib::SetCurrentTaskTo(QpamExitCallback);
        return;
    default:
        return;
    }
    if (delay)
        HLib::SetupTaskDelay(delay);
}

void QpamExitCallback(void *data) {
    MapStandEventWork &work = *static_cast<MapStandEventWork *>(data);
    LoadCutsceneAndSoundProgramForArea();
    EventFilmControl film{.cleanup = 0,
        .frames = 8,
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
    EnableScenarioCharacterDisplay(QPAM_RYO);
    EV_SetRefBoardParamsFor(QPAM_RYO, 0);
    hndlSetFlags(ctrlGetCharaByName(QPAM_RYO));
    UpdateMotionWorkWithPosition(scenFindMotionwork(QPAM_RYO), nullptr, 0);
    if (work.map.name)
        EV_SetRefBoardParamsFor(work.map, 0);
    EV_SetCharaControllerFlags(QPAM_RYO, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(QPAM_RYO, 3, NpcCharacterControllerState::Enabled);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0099");
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    SetPlayerControlEnabled(1);
    HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QPAM"), 0);
}

const ScriptAreaDefinition qpamScriptDefinition{.areaId = AREA_QPAM, .field04 = 0, .init = QpamInitNullsub, .main = QpamMain};
} // namespace shenmue::s2
