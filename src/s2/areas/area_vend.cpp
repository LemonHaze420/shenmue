#include "s2/areas/area_vend.hpp"
#include "s2/areas/vending_hand_commands.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>

namespace shenmue::s2 {
namespace {
std::int32_t vendingState, vendingElapsed, vendingValidSelection;
std::uint16_t vendingPrizeVoice = 1;
std::int64_t vendingArchive = -1;
shen_name vendingMachine;
VendingLampBank vendingLamps;
const Vector3 vendingInitialBasePosition{
    .x = std::bit_cast<float>(0xffffffffU), .y = std::bit_cast<float>(0xffffffffU), .z = std::bit_cast<float>(0xffffffffU)};
const Vector3i vendingInitialBaseRotation{.x = -1, .y = -1, .z = -1};

void VendingInitCallback(void *);
void VendingCheckCallback(void *);
void VendingInitSelectionCallback(void *);
void VendingSelectionVoiceCallback(void *);
void VendingSelectCallback(void *);
void VendingDrinkCallback(void *);
void VendingPrizeCallback(void *);
void VendingEndCallback(void *);
void VendingKillCallback(void *);

// @verified - p
void VendingProcCallback(void *) {
}

void SelectVendingCanRecords(VendingMachineWork &work) {
    const VendingCanRecord &record = (work.colour == 1 ? vendRedCans : vendOtherCans)[work.previousSelected];
    work.archiveRecord = record.record;
    work.alternateRecord = record.alternate;
    work.sound = record.sound;
    work.canKind = record.kind;
}

void PlayVendingVoice(VendingMachineWork &work, std::int32_t delay) {
    const float random = GetRandomNumberInRange(1.0f);
    const char *voice = nullptr;
    switch (work.voiceState) {
    case 1:
        if (!(random >= .2))
            voice = "0109A001"; // @text EN: "A drink." | JA: "ジュースか。"
        else if (random < .4)
            voice = "0109A002"; // @text EN: "A vending machine." | JA: "自動販売機があるな。"
        else if (random < .6)
            voice = "0109A003"; // @text EN: "$5 a can." | JA: "１本５ドルか…"
        else if (random < .8)
            voice = "0109A004"; // @text EN: "I think I'll get a drink." | JA: "１本買ってみるか。"
        else
            voice = "0109A005"; // @text EN: "I'll buy a can." | JA: "ちょっと買ってみるか。"
        break;
    case 2:
        voice = random >= .5 ? "0109A009" : "0109A008"; // @text 0109A009 EN: "This isn't enough to buy me a can." | JA: "これじゃ、買えないな…" ; 0109A008 EN: "I don't have enough money." | JA: "足りないな…"
        break;
    case 3:
        voice = "0109A010"; // @text EN: "Which one should I get?" | JA: "どれにするか…"
        break;
    case 4:
        voice = !(random >= .3) ? "0109A036" : (random < .1 || random >= .6 ? "0109A038" : "0109A037"); // @text 0109A036 EN: "I'll get another can." | JA: "もう１本か…" ; 0109A038 EN: "I'll get an extra can." | JA: "もう１本だな。" ; 0109A037 EN: "Another can." | JA: "もう１本…"
        break;
    case 5:
        voice = "0109A013"; // @text EN: "This one." | JA: "これか。"
        break;
    case 7:
        voice = "0109A029"; // @text EN: "Maybe next time." | JA: "またにするか…"
        break;
    case 8:
        switch (vendingPrizeVoice) {
        case 1:
            voice = random < .3 ? "0109A026" : (random >= .6 ? "0109A028" : "0109A027"); // @text 0109A026 EN: "Oh, a winning can." | JA: "んっ…当たり缶だ。" ; 0109A028 EN: "Oh, it's a winner." | JA: "おっ…当たったな。" ; 0109A027 EN: "Oh, I got a winning can." | JA: "おっ…当たった。"
            break;
        case 2:
            voice = random < .3 ? "0109A030" : (random >= .6 ? "0109A032" : "0109A031"); // @text 0109A030 EN: "Inside=@" | JA: "中には…" ; 0109A032 EN: "Let's see what's inside." | JA: "中にあるのは…" ; 0109A031 EN: "What's inside=@?" | JA: "入ってるのは…"
            break;
        case 3:
            voice = random < .3 ? "0109A033" : (random >= .6 ? "0109A035" : "0109A034"); // @text 0109A033 EN: "This." | JA: "…これか。" ; 0109A035 EN: "This is what I got." | JA: "こんなものが…" ; 0109A034 EN: "This was inside." | JA: "これが入ってたのか。"
            break;
        default:
            break;
        }
        if (voice)
            EVT_AT_SetVoice(voice, 1, delay);
        ++vendingPrizeVoice;
        return;
    case 9:
        if (!(random >= .1))
            voice = "0109A016";
        else if (random < .2)
            voice = "0109A017";
        else if (random < .3)
            voice = "0109A018";
        else if (random < .4)
            voice = "0109A019";
        else if (random < .5)
            voice = "0109A020";
        else if (random < .6)
            voice = "0109A021";
        else if (random < .7)
            voice = "0109A022";
        else if (random < .8)
            voice = "0109A023";
        else if (random < .9)
            voice = "0109A024";
        else if (random < 1.0)
            voice = "0109A017";
        break;
    case 10:
        voice = random >= .5 ? "0109A007" : "0109A006"; // @text 0109A007 EN: "I guess not." | JA: "やっぱりやめよう。" ; 0109A006 EN: "No, I won't." | JA: "やめておこう…"
        break;
    default:
        break;
    }
    if (voice)
        EVT_AT_SetVoice(voice, 1, delay);
}

void InitializeVendingButtons(VendingMachineWork &work) {
    work.buttonCount = 8;
    work.upperButtons = 6;
    work.selected = 3;
    for (std::int32_t index = 0; index < work.buttonCount; ++index) {
        const std::int32_t column = index < work.upperButtons ? index : index + work.upperButtons - work.buttonCount;
        work.buttons[index] = {.x = static_cast<float>(column) * .086f - .187f, .y = index < work.upperButtons ? 1.25f : .95f, .z = .42f};
    }
}

void InitializeVendingResources(VendingMachineWork &work) {
    work.characters = Queue_EVT_AT_ReadChr(work.task, vendCharacterNames);
    InitializeVendingButtons(work);
    const char *directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("VEND"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("VEND"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("VEND"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0109E000");
    work.prizeState = 0;
}

void VendingInitEnqueue(void *argument) {
    HLib::Task *proc = HLib::EnqueueTaskWithParameter(VendingProcCallback, 5, 0x0b, 8, VEND_PROC_TASK);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(VendingInitCallback, 4, 0x0c, sizeof(VendingMachineWork), VEND_MAIN_TASK);
    VendingMachineWork *work = static_cast<VendingMachineWork *>(HLib::GetTaskParameter(task));
    *static_cast<VendingMachineWork **>(HLib::GetTaskParameter(proc)) = work;
    work->procTask = proc;
    work->task = task;
    work->originalMachine = argument ? *static_cast<shen_name *>(argument) : EVENT_CHARACTER_INVALID;
    InitializeVendingResources(*work);
    work->voiceState = 1;
    work->contentState = 0;
    work->firstSelection = 1;
    EV_FreeAuthorPak(1, "0109");
    EV_EventSetUpHelpIcon_0(25, 0);
    work->colour = 0;
    for (const VendingMapColour &row : vendMapColours) {
        if (row.map.name == g_currentMap.name) {
            work->colour = row.colour;
            break;
        }
    }
    if (work->colour == 1) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 600, " VM_COLOR ==> RED \n");
        vendingMachine = VEND_RED;
    } else if (work->colour == 2) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 605, " VM_COLOR ==> WHITE \n");
        vendingMachine = VEND_WHITE;
    } else if (work->colour == 3) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 610, " VM_COLOR ==> GREEN \n");
        vendingMachine = VEND_GREEN;
    } else {
        EV_debug_log("VEND\\eVEND_et_vend.c", 614, " ****** WARNING ********* \n");
        EV_debug_log("VEND\\eVEND_et_vend.c", 615, " VEND.EV3 can't get <AREA-ID>\n");
    }
    if (work->colour >= 1 && work->colour <= 3) {
        const char *directory = Filepath_Gen(FilepathGenMode::PackDirectory);
        vendingArchive = EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Mount, reinterpret_cast<std::uintptr_t>(directory),
            reinterpret_cast<std::uintptr_t>(work->colour == 1 ? "VEND.afs" : "SUNT.afs"));
    }
    work->wait = 0;
}

void VendingInitCallback(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    switch (vendingState) {
    case 0:
        vendingState = 1;
        if (std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash))) > 5)
            PlayVendingVoice(work, 25);
        [[fallthrough]];
    case 1: {
        vendingState = 2;
        EV_debug_log("VEND\\eVEND_et_vend.c", 660, " Now state -->VEND_FADE \n");
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 40,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 15.0f});
        HLib::SetupTaskDelay(6);
        return;
    }
    case 2: {
        vendingState = 1;
        EV_debug_log("VEND\\eVEND_et_vend.c", 671, " Now state -->VM_VEND_TRANS_CHARA \n");
        if (DoesScenarioCharacterExist(work.originalMachine)) {
            EV_EnableCharacterDisplay(vendingMachine);
            EV_DisableCharacterDisplay(work.originalMachine);
            Vector3 position;
            Vector3i rotation;
            EV_GetCharaPosition(work.originalMachine, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(work.originalMachine, &rotation, EventTransformFlags::World);
            EV_AT_SetPos(vendingMachine, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(vendingMachine, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        }
        SetLowQualityHands(ctrlGetCharaByName(VEND_HAND));
        SetLowQualityHands(ctrlGetCharaByName(VEND_RYO));
        SetHighModelXYZ(ctrlGetCharaByName(VEND_RYO), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(VEND_RYO), 1, 1, 0);
        AT_SetLocalBaseChara(VEND_RYO, vendingMachine);
        AT_SetLocalBaseChara(VEND_BASE, vendingMachine);
        AT_SetLocalBaseChara(VEND_HAND, VEND_BASE);
        EV_AT_SetAngY(VEND_HAND, EventTransformFlags::AllAxes, vendHandAngle);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 0, reinterpret_cast<std::byte *>(vendIdleHandCommand), 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 1);
        SetPlayerControlEnabled(0);
        EV_AT_SetPos(VEND_BASE, EventTransformFlags::AllAxes, vendingInitialBasePosition);
        EV_AT_SetAngY(VEND_BASE, EventTransformFlags::AllAxes, vendingInitialBaseRotation);
        EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 0xf003, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf003);
        HLib::SetCurrentTaskTo(VendingCheckCallback);
        break;
    }
    default:
        break;
    }
}

std::int32_t VendingNoMoney() {
    PATI_StartMoneyCount(0, 10);
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1111, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 70;
}

std::int32_t VendingInsertCoin() {
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 5060, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5060);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1103, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetSound(1481, 30);
    EVT_AT_SetSound(191, 125);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-5), 70);
    return 170;
}

void VendingCheckCallback(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 736, "@@@    eVEND_EVT_CheckVend ( ) \n");
    if (GetCurrentControlInput(1) == 1024 || work.originalMachine.name != 0xffffffffU) {
        std::int32_t delay;
        if (std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash))) < 5) {
            work.voiceState = 2;
            delay = VendingNoMoney();
            PlayVendingVoice(work, 45);
            HLib::SetCurrentTaskTo(VendingEndCallback);
            HLib::SetupTaskDelay(delay + 50);
        } else {
            delay = VendingInsertCoin();
            HLib::SetCurrentTaskTo(VendingInitSelectionCallback);
            HLib::SetupTaskDelay(delay);
        }
    }
}

void HideVendingLamps() {
    for (std::int32_t index = 0; index < 8; ++index) {
        Vector3 position;
        Vector3i rotation;
        EV_scenApplyPosOrRotToPartOf(
            vendingMachine, static_cast<std::uint16_t>(vendingLamps.parts[index]), &position, &rotation, EventCharacterPartTransformMode::Get);
        vendingLamps.positions[index] = position;
        position.z += .1f;
        EV_scenApplyPosOrRotToPartOf(
            vendingMachine, static_cast<std::uint16_t>(vendingLamps.parts[index]), &position, nullptr, EventCharacterPartTransformMode::Absolute);
    }
}

void RestoreVendingLamps(void *data) {
    VendingLampBank &lamps = *static_cast<VendingLampBank *>(data);
    for (std::int32_t index = 0; index < 8; ++index) {
        if (lamps.selected == lamps.parts[index])
            EV_debug_log("VEND\\eVEND_et_vend_auto.c", 468, "@@@   part == lamp[%d]\n", index);
        else
            EV_scenApplyPosOrRotToPartOf(
                vendingMachine, static_cast<std::uint16_t>(lamps.parts[index]), &lamps.positions[index], nullptr, EventCharacterPartTransformMode::Absolute);
    }
}

void VendingInitSelectionCallback(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 772, "@@@  eVEND_EVT_InitSelect( ) \n");
    EV_DisableCharacterDisplay(VEND_RYO);
    SetHighModelXYZ(ctrlGetCharaByName(VEND_HAND), 1, 0, 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(VEND_HAND), 0, 21, &vendSelectionHandRotation);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_HAND), 0, reinterpret_cast<std::byte *>(vendSelectionHandCommand), 0);
    EV_EnableCharacterDisplay(VEND_HAND);
    EV_EnsureCharacterDraw(VEND_HAND);
    EventSetPosition position{.cleanup = 0, .character = VEND_HAND, .parent = VEND_BASE, .position = vendHandOffset, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{.cleanup = 0, .character = VEND_HAND, .parent = VEND_BASE, .rotation = {.x = 4580, .y = 0, .z = 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    position = {.cleanup = 0, .character = VEND_BASE, .parent = vendingMachine, .position = work.buttons[work.selected], .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = VEND_BASE, .parent = vendingMachine, .rotation = {}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    work.contentState = 1;
    EventSetCamera camera{.cleanup = 0, .camera = 1101, .eye = VEND_BASE, .target = VEND_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HideVendingLamps();
    HLib::SetCurrentTaskTo(VendingSelectionVoiceCallback);
    HLib::SetupTaskDelay(5);
}

void VendingSelectionVoiceCallback(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    if (work.firstSelection) {
        work.voiceState = 3;
        work.firstSelection = 0;
    } else
        work.voiceState = 4;
    PlayVendingVoice(work, 0);
    HLib::SetCurrentTaskTo(VendingSelectCallback);
    HLib::SetupTaskDelay(50);
}

std::int32_t InterpolateVendingHand(const Vector3 &target, const Vector3 &previous, std::int32_t step, std::int32_t delay) {
    std::int32_t index = 0;
    do {
        const float factor = vendInterpolation[index];
        const Vector3 position{.x = (target.x - previous.x) * factor + previous.x,
            .y = (target.y - previous.y) * factor + previous.y,
            .z = (target.z - previous.z) * factor + previous.z};
        EVT_AT_SetPos(VEND_BASE, vendingMachine, position, index + delay);
        index += step;
    } while (index < 8);
    return delay + 8 / step;
}

void VendingHandWaitCallback(void *data) {
    VendingHandWaitWork &work = *static_cast<VendingHandWaitWork *>(data);
    if (work.state == 2) {
        work.state = 3;
        EV_debug_log("VEND\\eVEND_et_vend.c", 1466, "EVVendHand_END in eVEND_ET_SetHandUpYubiWait\n");
        HLib::EV_HLKillOtherHierarchyTask(work.task);
    }
    if (work.state == 4) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 1470, "EVVendHand_ERROR in eVEND_ET_SetHandUpYubiWait\n");
        HLib::EV_HLKillOtherHierarchyTask(work.task);
    }
    if (work.timer > work.threshold) {
        ApplyCharacterHandPoseCommand(
            ctrlGetCharaByName(work.character), work.side, reinterpret_cast<std::byte *>(work.command), static_cast<std::int16_t>(work.duration));
        work.state = 2;
        HLib::SetupTaskDelay(work.duration);
    }
    ++work.timer;
}

std::int32_t QueueVendingHandWait() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(VendingHandWaitCallback, 3, 0x0b, sizeof(VendingHandWaitWork), AREA_EVENT_TASK);
    VendingHandWaitWork *work = static_cast<VendingHandWaitWork *>(HLib::GetTaskParameter(task));
    work->task = task;


    std::memcpy(work->command, vendWaitHandCommand, sizeof(work->command));
    work->character = VEND_HAND;
    work->side = 0;
    work->duration = 5;
    work->threshold = 5;
    work->timer = 0;
    work->state = 0;
    return 10;
}

std::int32_t PressVendingButton(const Vector3 &button) {
    const float x = button.x, y = button.y - .01f, z = button.z, pressedZ = z - .008f;
    const Vector3 target{.x = x, .y = y, .z = z};
    const std::int32_t begin = InterpolateVendingHand(target, button, 1, 7);
    QueueVendingHandWait();
    const float deltaX = x - x, deltaY = y - y;
    for (std::int32_t index = 0; index < 8; index += 4) {
        const float factor = vendInterpolation[index];
        EVT_AT_SetPos(VEND_BASE, vendingMachine, {.x = factor * deltaX + x, .y = factor * deltaY + y, .z = factor * (pressedZ - z) + z}, index + begin);
    }
    const std::int32_t end = begin + 2;
    for (std::int32_t index = 0; index < 8; index += 4) {
        const float factor = vendInterpolation[index];
        EVT_AT_SetPos(VEND_BASE, vendingMachine, {.x = factor * deltaX + x, .y = factor * deltaY + y, .z = factor * (z - pressedZ) + pressedZ}, index + end);
    }
    EV_DisableCharacterDisplay(VEND_RYO);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1102, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetSound(190, end);
    EVT_AT_SetSound(193, end + 2);
    return end + 2;
}

void VendingAutoWaitCallback(void *data) {
    VendingAutoWaitWork &work = *static_cast<VendingAutoWaitWork *>(data);
    if (work.state < 0) {
        ++work.timer;
        return;
    }
    if (work.state <= 1) {
        if (work.timer >= work.begin) {
            work.callback(work.argument);
            ++work.timer;
            work.state = 2;
            return;
        }
        work.state = 1;
        ++work.timer;
        return;
    }
    if (work.state == 2) {
        if (work.timer >= work.end)
            work.state = 3;
        else
            work.callback(work.argument);
        ++work.timer;
        return;
    }
    if (work.state >= 3 && work.state <= 5)
        HLib::EV_HLKillOtherHierarchyTask(work.task);
    ++work.timer;
}

void DetermineVendingPrize(VendingMachineWork &work, float random) {
    const std::int32_t previous = work.previousSelected;
    work.previousSelected = 8;
    if (!(random >= .01)) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 967, "Dual-S val=%f\n", random);
        work.prizeItem = 4016;
    } else if (random < .02) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 970, "Dual-G val=%f\n", random);
        work.prizeItem = 4017;
    } else if (random < .0425) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 973, "PROB_AKIRA val=%f\n", random);
        work.prizeItem = 4000;
    } else if (random < .065) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 976, "JUKY  val=%f\n", random);
        work.prizeItem = 4002;
    } else if (random < .0875) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 979, "SARA  val=%f\n", random);
        work.prizeItem = 4004;
    } else if (random < .11) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 982, "RAU  val=%f\n", random);
        work.prizeItem = 4006;
    } else if (random < .1325) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 985, "PAI  val=%f\n", random);
        work.prizeItem = 4008;
    } else if (random < .155) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 988, "URFU  val=%f\n", random);
        work.prizeItem = 4010;
    } else if (random < .1775) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 991, "JEFLY  val=%f\n", random);
        work.prizeItem = 4012;
    } else if (random < .2) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 994, "KAGE  val=%f\n", random);
        work.prizeItem = 4014;
    } else {
        work.prizeState = 3;
        work.previousSelected = previous;
    }
}

void VendingSelectCallback(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    const std::int32_t minutes = shenGetMinutesUntilNextFrameMinute();
    if (++vendingElapsed > 3000 || minutes < 1) {
        work.voiceState = 10;
        PlayVendingVoice(work, 0);
        PATI_StartMoneyCount(5, 50);
        HLib::SetCurrentTaskTo(VendingEndCallback);
        HLib::SetupTaskDelay(70);
    }
    GetBattleDataConfig(0, VEND_CANCEL);
    GetBattleDataConfig(0, VEND_ACCEPT);
    const std::uint16_t input = static_cast<std::uint16_t>(GetCurrentControlInput(1));
    if (work.wait > 0) {
        --work.wait;
        return;
    }
    if (!GetBattleDataConfig(1, VEND_ACCEPT) || !GetBattleDataConfig(2, VEND_ACCEPT) || vendingValidSelection) {
        if (GetBattleDataConfig(1, VEND_CANCEL) && GetBattleDataConfig(2, VEND_CANCEL)) {
            work.voiceState = 7;
            PlayVendingVoice(work, 0);
            AddThisToCashOrSegaCoins(5, ECashOrCoins::Cash2);
            EV_debug_log("VEND\\eVEND_et_vend.c", 925, "Put B button at selecting Can\n");
            HLib::SetCurrentTaskTo(VendingEndCallback);
            return;
        }
        vendingValidSelection = 0;
        const std::int32_t previous = work.selected;
        switch (input) {
        case 0x8000:
            if (previous != work.upperButtons - 1 && previous < work.buttonCount - 1)
                ++work.selected;
            break;
        case 0x4000:
            if (previous > 0 && previous != work.upperButtons)
                --work.selected;
            break;
        case 0x1000:
            if (previous >= work.upperButtons)
                work.selected = previous + work.upperButtons - work.buttonCount;
            break;
        case 0x2000:
            if (previous < work.upperButtons) {
                work.selected = previous + work.buttonCount - work.upperButtons;
                if (work.selected < work.upperButtons)
                    work.selected = work.upperButtons;
            }
            break;
        default:
            break;
        }
        if (previous == work.selected)
            return;
        vendingValidSelection = 1;
        work.wait = InterpolateVendingHand(work.buttons[work.selected], work.buttons[previous], 1, 0);
        return;
    }
    work.voiceState = 5;
    work.contentState = 2;
    work.previousSelected = work.selected;
    PlayVendingVoice(work, 0);
    const std::int32_t delay = PressVendingButton(work.buttons[work.selected]);
    vendingLamps.selected = work.selected;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(VendingAutoWaitCallback, 3, 0x0b, sizeof(VendingAutoWaitWork), AREA_EVENT_TASK);
    VendingAutoWaitWork *wait = static_cast<VendingAutoWaitWork *>(HLib::GetTaskParameter(task));
    wait->task = task;
    wait->begin = delay - 5;
    wait->end = delay - 5 < delay - 7 ? delay - 7 : delay - 4;
    wait->timer = 0;
    wait->state = 0;
    wait->callback = RestoreVendingLamps;
    wait->argument = &vendingLamps;
    if (work.prizeState == 0) {
        work.prizeState = 1;
        DetermineVendingPrize(work, GetRandomNumberInRange(1.0f));
    }
    if (work.prizeState == 1) {
        work.prizeState = 2;
        vendingElapsed = 0;
        SelectVendingCanRecords(work);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::LoadAfs, static_cast<std::uintptr_t>(vendingArchive),
            static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
        HLib::SetCurrentTaskTo(VendingPrizeCallback);
        HLib::SetupTaskDelay(30);
    } else {
        SelectVendingCanRecords(work);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::LoadAfs, static_cast<std::uintptr_t>(vendingArchive),
            static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
        HLib::SetCurrentTaskTo(VendingDrinkCallback);
    }
    HLib::SetupTaskDelay(delay);
}

void VendingPartCallback(void *data) {
    VendingPartWork &work = *static_cast<VendingPartWork *>(data);
    EV_scenApplyPosOrRotToPartOf(
        work.character, static_cast<std::uint16_t>(work.part), &work.position, &work.rotation, EventCharacterPartTransformMode::Absolute);
    if (work.cleanup)
        HLib::DestroyCurrentTask();
}

std::int32_t PickUpVendingCan(std::uint32_t sound) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1,
        .target = VEND_CAN,
        .character = VEND_RYO,
        .bone = 14,
        .position = vendCanAttachment,
        .rotation = vendCanAttachmentRotation,
        .mode = 0,
        .delay = 82};
    EV_DisableCharacterDisplay(VEND_HAND);
    EV_EnableCharacterDisplay(VEND_RYO);
    EVT_AT_ObjectDisp_1(VEND_CAN, 82, 0);
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 5062, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5062);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1104, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetSound(198, 80);
    EVT_AT_SetSound(sound ? 196 : 195, 290);
    Vector3 first, second;
    Vector3i rotation;
    EV_scenApplyPosOrRotToPartOf(VEND_CAN, 152, &first, &rotation, EventCharacterPartTransformMode::Get);
    EV_scenApplyPosOrRotToPartOf(VEND_CAN, 153, &second, &rotation, EventCharacterPartTransformMode::Get);
    second.y -= 100.0f;
    task = HLib::EnqueueTaskWithParameter(VendingPartCallback, 6, 0x0b, sizeof(VendingPartWork), AREA_EVENT_TASK);
    HLib::SetupTimedTask(task, 290);
    *static_cast<VendingPartWork *>(HLib::GetTaskParameter(task)) = {.character = VEND_CAN, .part = 153, .position = first, .rotation = rotation, .cleanup = 1};
    task = HLib::EnqueueTaskWithParameter(VendingPartCallback, 6, 0x0b, sizeof(VendingPartWork), AREA_EVENT_TASK);
    HLib::SetupTimedTask(task, 290);
    *static_cast<VendingPartWork *>(HLib::GetTaskParameter(task)) = {
        .character = VEND_CAN, .part = 152, .position = second, .rotation = rotation, .cleanup = 1};
    return 170;
}

std::int32_t AnimateVendingDrink(std::int32_t delay) {
    const std::int32_t detach = delay + 690;
    HLib::Task *task;
    if (detach == 0) {
        EventCling work;
        work.cleanup = 0;
        work.target = VEND_CAN;
        work.character = VEND_RYO;
        work.mode = 1;
        work.delay = 0;
        EVT_AT_Cling(&work);
    } else {
        task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
        RegisterEventTask(task);
        EventCling *work = static_cast<EventCling *>(HLib::GetTaskParameter(task));
        work->cleanup = 1;
        work->target = VEND_CAN;
        work->character = VEND_RYO;
        work->mode = 1;
        work->delay = static_cast<float>(detach);
    }
    EVT_AT_ObjectDisp_1(VEND_CAN, detach, 1);
    EVT_AT_SetMot(VEND_RYO, 5059, 0, 1, 0, delay);
    EVT_AT_SetCam(1105, vendingMachine, vendingMachine, 0, 0, delay);
    EVT_AT_SetCam(1106, vendingMachine, vendingMachine, 0, 0, delay + 575);
    EVT_AT_SetSound(192, delay + 210);
    EVT_AT_SetSound(192, delay + 285);
    EVT_AT_SetSound(192, delay + 470);
    EVT_AT_SetSound(197, detach);
    EVT_AT_SetFaceLooks(VEND_RYO, 3, 30, .5f, delay + 200);
    EVT_AT_SetFaceLooks(VEND_RYO, 0, 30, 1, delay + 300);
    return delay + 765;
}

std::int32_t PickUpVendingPrize() {
    GetMotionFrameCount(5062);
    GetMotionFrameCount(5061);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1,
        .target = VEND_CAN,
        .character = VEND_RYO,
        .bone = 14,
        .position = vendCanAttachment,
        .rotation = vendCanAttachmentRotation,
        .mode = 0,
        .delay = 82};
    EV_DisableCharacterDisplay(VEND_HAND);
    EV_EnableCharacterDisplay(VEND_RYO);
    EVT_AT_ObjectDisp_1(VEND_CAN, 82, 0);
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 5062, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5062);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1104, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(1109, vendingMachine, vendingMachine, 0, 0, 110);
    EVT_AT_SetSound(198, 80);
    EVT_AT_ObjectDisp_1(vendingMachine, 230, 1);
    EVT_AT_ObjectDisp_1(VEND_CAN, 230, 1);
    EVT_AT_SetMot(VEND_RYO, 5061, 0, 1, 0x40000000U, 200);
    EVT_AT_SetCam(1110, vendingMachine, vendingMachine, 0, 0, 200);
    Queue_EVT_AT_CtrlHandUp_1(VEND_RYO, 0, reinterpret_cast<std::byte *>(vendPrizeHandCommand), 5, 230);
    EVT_AT_ObjectDisp_1(vendingMachine, 390, 0);
    EVT_AT_SetFaceLooks(VEND_RYO, 4, 30, .5f, 230);
    EVT_AT_SetFaceLooks(VEND_RYO, 3, 30, .3f, 280);
    return 390;
}

void VendingDrinkCallback(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 1014, "@@@     eVEND_EVT_DrinkJuice( ) \n");
    work.voiceState = 9;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    work.contentState = 4;
    ApplyCharacterHandPoseCommand(
        ctrlGetCharaByName(VEND_RYO), 0, reinterpret_cast<std::byte *>(work.canKind == 0 ? vendCanHandCommand : vendAlternateCanCommand), 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(VEND_RYO), 0, 21, work.canKind == 0 ? &vendCanHandRotation : &vendAlternateHandRotation);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 1, reinterpret_cast<std::byte *>(vendLeftDrinkCommand), 190);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 1, reinterpret_cast<std::byte *>(vendLeftExitCommand), 410);
    const std::int32_t delay = AnimateVendingDrink(PickUpVendingCan(work.sound));
    PlayVendingVoice(work, delay - 240);
    HLib::SetCurrentTaskTo(VendingEndCallback);
    HLib::SetupTaskDelay(delay);
}

void VendingItemCallback(void *data) {
    VendingItemWork &work = *static_cast<VendingItemWork *>(data);
    AddInventoryItem(work.item, 1, 1);
    if (work.cleanup)
        HLib::DestroyCurrentTask();
}

void QueueVendingItem(std::uint32_t item, std::int32_t delay) {
    if (!delay) {
        AddInventoryItem(item, 1, 1);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(VendingItemCallback, 6, 0x0b, sizeof(VendingItemWork), AREA_EVENT_TASK);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    *static_cast<VendingItemWork *>(HLib::GetTaskParameter(task)) = {.cleanup = 1, .item = item};
}

void VendingPrizeCallback(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    work.voiceState = 8;
    if (vendingState != 1) {
        if (vendingState != 2)
            return;
        vendingState = 1;
        if (vendingArchive != -1) {
            const bool pending = EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0) != 0;
            if (pending || work.contentState == 4)
                EV_ControlPackageResources(PakCategory::Package, pending ? 13 : 12, static_cast<std::uintptr_t>(vendingArchive),
                    static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
        }
        work.contentState = 1;
        EV_debug_log("VEND\\eVEND_et_vend.c", 1105, "HIT_prosesing_02\n");
        HLib::SetCurrentTaskTo(VendingInitSelectionCallback);
        return;
    }
    vendingState = 2;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    work.contentState = 4;
    EV_debug_log("VEND\\eVEND_et_vend.c", 1062, "HIT_prosesing_01\n");
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 0, reinterpret_cast<std::byte *>(vendCanHandCommand), 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(VEND_RYO), 0, 21, &vendCanHandRotation);
    const std::int32_t delay = PickUpVendingPrize();
    PlayVendingVoice(work, delay - 270);
    PlayVendingVoice(work, delay - 190);
    PlayVendingVoice(work, delay - 130);
    EVT_AT_SetPoint(VEND_RYO, VEND_CAN, 0, 1, 0, delay - 190, 190, 0);
    QueueVendingItem(work.prizeItem, delay - 120);
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 6,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay - 10)});
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 40,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay)});
    HLib::SetupTaskDelay(delay);
}

void VendingEndCallback(void *) {
    EV_debug_log("VEND\\eVEND_et_vend.c", 1123, "@@@             eVEND_EVT_VendEnd( ) \n");
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    HLib::SetCurrentTaskTo(VendingKillCallback);
    EventFilmControl film{.cleanup = 0,
        .frames = 10,
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
    HLib::SetupTaskDelay(10);
}

void ReleaseVendingResources(VendingMachineWork &work) {
    EV_EnableCharacterDisplay(VEND_RYO);
    EV_DisableCharacterDisplay(vendingMachine);
    if (DoesScenarioCharacterExist(work.originalMachine))
        EV_EnableCharacterDisplay(work.originalMachine);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("VEND"), 0);
    ET_AT_KillWork(work.characters);
}

void VendingKillCallback(void *data) {
    using enum CameraState;
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 1162, "@@@     EV_KillVend( ) \n");
    if (vendingState != 1) {
        if (vendingState != 2)
            return;
        work.voiceState = 11;
        if (vendingArchive != -1) {
            const bool pending = EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0) != 0;
            if (pending || work.contentState == 4)
                EV_ControlPackageResources(PakCategory::Package, pending ? 13 : 12, static_cast<std::uintptr_t>(vendingArchive),
                    static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
            work.contentState = 5;
            EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Unmount, static_cast<std::uintptr_t>(vendingArchive), 0);
            vendingArchive = -1;
        }
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetHighModelXYZ(ctrlGetCharaByName(VEND_HAND), 0, 0, 0);
        EV_DisableCharacterDisplay(VEND_HAND);
        hndlSetFlags(ctrlGetCharaByName(VEND_HAND));
        hndlSetFlags(ctrlGetCharaByName(VEND_RYO));
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(VEND_RYO), 0, 0, 1);
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(work.task);
        HLib::EV_HLKillOtherHierarchyTask(work.procTask);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 0);
        SetPlayerControlEnabled(1);
        ReleaseVendingResources(work);
        LoadCutsceneAndSoundProgramForArea();
        EV_NPC_NewLoadSw(1);
        return;
    }
    vendingState = 2;
    if (work.voiceState == 7 || work.voiceState == 10) {
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 10,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 100});
        HLib::SetupTaskDelay(100);
    } else {
        EventFilmControl film{.cleanup = 0,
            .frames = 10,
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
    }
}
} // namespace

void InitializeVendingMachineEvent() {
    vendingLamps = {.selected = 0, .parts = {172, 173, 174, 175, 176, 177, 178, 179}, .positions = {}};
    vendingPrizeVoice = 1;
    vendingState = 0;
    vendingElapsed = 0;
    vendingValidSelection = 0;
    vendingArchive = -1;
}

void RunVendingMachineEvent(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepNpcClearWork *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = VendingInitEnqueue;
    work->callbackData = reinterpret_cast<void *>(argument);
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
} // namespace shenmue::s2
