#include "s2/areas/area_aozr.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name event{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, invalid{0xffffffffU};
struct Board {
    shen_name magic;
    std::byte reserved04[8];
    char audio[12];
    std::int32_t audioVariant;
    char authorBank[12];
    shen_name seller;
    float spacingX, spacingY;
    Vector3 origin, angles;
    std::uint32_t reserved4c;
    std::int16_t transitionFrames[4];
    Vector3 cameraOffsets[4];
    std::int16_t initialX, initialY, columns, rows;
    std::int32_t cellsOffset, startScript, buyScript;
};
struct Cell {
    shen_name character;
    std::int32_t price;
    std::uint32_t item;
    std::int32_t explanation, reserved10;
};
struct ShopWork {
    HLib::Task *self;
    HLib::Task *parent;
    char package[8];
    shen_name seller;
    std::uint32_t flags;
    std::int32_t x, y, camera, transition, elapsed, reserved34;
    const Board *board;
    float zoom;
    std::int32_t bufferedInput;
    std::uint64_t reserved48;
    char savedAudio[8];
};
static_assert(offsetof(Board, cellsOffset) == 0x90 && offsetof(Board, columns) == 0x8c && offsetof(Board, cameraOffsets) == 0x58);
static_assert(sizeof(Cell) == 0x14 && sizeof(ShopWork) == 0x58 && offsetof(ShopWork, savedAudio) == 0x50);
void StartPackage_Callback(void *);
void WaitPackage_Callback(void *);
void GetNpc_Callback(void *);
void Initialize_Callback(void *);
void StartSale_Callback(void *);
void WaitStart_Callback(void *);
void LockOn_Callback(void *);
void AskBuy_Callback(void *);
void WaitBuy_Callback(void *);
void Explain_Callback(void *);
void Deliver_Callback(void *);
void Done_Callback(void *);
const Cell *SelectedCell(const ShopWork *work) {
    return reinterpret_cast<const Cell *>(reinterpret_cast<const std::byte *>(work->board) + work->board->cellsOffset) + work->x +
        work->y * work->board->columns;
}
std::int16_t Variant(const Cell *cell) {
    return static_cast<std::int16_t>(cell->item >> 16);
}
float Snap(float value) {
    return std::fabs(value) < 1.1920929e-7f ? 0.0f : value;
}

shen_name BoardCell(const Board *board, Vector3 *position, Vector3i *rotation, std::int32_t x, std::int32_t y) {
    if (!board || board->magic.name != SHEN_NAME(AOZR) || x < 0 || y < 0 || x >= board->columns || y >= board->rows || board->cellsOffset <= 0)
        return shen_name{};
    Vector3i angles{static_cast<std::int32_t>((board->angles.x * 8192) / 45), static_cast<std::int32_t>((board->angles.y * 8192) / 45),
        static_cast<std::int32_t>((board->angles.z * 8192) / 45)};
    Vector3 point{static_cast<float>(x) * board->spacingX, static_cast<float>(y) * board->spacingY, 0};
    ApplyScenarioTranslationAndRotation(&board->origin, &angles, &point);
    if (position)
        *position = point;
    if (rotation) {
        *rotation = angles;
        rotation->z += 0x8000;
    }
    const auto *cells = reinterpret_cast<const Cell *>(reinterpret_cast<const std::byte *>(board) + board->cellsOffset);
    return cells[x + y * board->columns].character;
}

bool HideBoard(ShopWork *work) {
    if (!work || !work->board)
        return false;
    EV_EnableCharacterDisplay(ryo);
    if (work->seller.name)
        EV_EnableCharacterDisplay(work->seller);
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            const auto name = BoardCell(work->board, nullptr, nullptr, x, y);
            if (name.name)
                EV_DisableCharacterDisplay(name);
        }
    return true;
}
void Beep_Callback(void *data) {
    if (data) {
        Loop3DSoundEffect(0xe00ab);
        HLib::SetupTaskDelay(4);
    }
    HLib::DestroyCurrentTask();
}

void UpdateCamera(ShopWork *work) {
    if (!work || !work->board)
        return;
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    Vector3 target = eye;
    Vector3i angles{};
    BoardCell(work->board, &target, &angles, work->x, work->y);
    if (work->transition <= 0)
        work->transition = 0;
    else {
        for (int i = 0; i < 3; ++i)
            target[i] = Snap(Snap(Snap(eye[i] - target[i]) * .55f) + target[i]);
        if (--work->transition == 0 && SelectedCell(work)->explanation != -1) {
            auto *task = HLib::EnqueueTaskWithParameter(Beep_Callback, 4, 0x0b, 4, event);
            if (task) {
                auto *data = static_cast<std::uint32_t *>(HLib::GetTaskParameter(task));
                if (data)
                    *data = 0;
            }
        }
    }
    camCtrlSetState(CameraState::Callback);
    camCtrlSetTargetPosition(target);
    Vector3 offset = work->camera >= 1 && work->camera <= 4 ? work->board->cameraOffsets[work->camera - 1] : Vector3{0, 0, .5f};
    for (auto &axis : offset)
        axis = Snap(axis * work->zoom);
    ApplyScenarioTranslationAndRotation(&target, &angles, &offset);
    camCtrlSetNextPosition(offset);
    camCtrlSetFov(5404);
}

bool ShowBoard(ShopWork *work) {
    if (!work || !work->board)
        return false;
    EV_NPC_NewLoadSw(0);
    EV_DisableCharacterDisplay(ryo);
    if (work->seller.name)
        EV_DisableCharacterDisplay(work->seller);
    HLib::SetCurrentTaskTo(LockOn_Callback);
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            const auto name = BoardCell(work->board, nullptr, nullptr, x, y);
            if (name.name)
                EV_EnableCharacterDisplay(name);
        }
    const Vector3 scale{2, 2, 2};
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            const auto name = BoardCell(work->board, nullptr, nullptr, x, y);
            if (name.name)
                SetMotionworkFlagForCharaById(name, &scale);
        }
    return true;
}

std::int32_t MoveSelection(ShopWork *work, bool horizontal, bool positive) {
    if (!work)
        return 0;
    auto &selected = horizontal ? work->x : work->y;
    const auto count = horizontal ? work->board->columns : work->board->rows;
    const auto old = selected;
    auto exists = [&](int index) { return BoardCell(work->board, nullptr, nullptr, horizontal ? index : work->x, horizontal ? work->y : index).name != 0; };
    int index = old + (positive ? 1 : -1);
    if (positive) {
        if (old > count - 1)
            return 0;
        while (index < count && !exists(index))
            ++index;
        if (index >= count) {
            index = 0;
            while (index < old && !exists(index))
                ++index;
            if (index >= old)
                return 0;
        }
    } else {
        if (old < 0)
            return 0;
        while (index >= 0 && !exists(index))
            --index;
        if (index < 0) {
            index = count - 1;
            while (index > old && !exists(index))
                --index;
            if (index <= old)
                return 0;
        }
    }
    selected = index;
    return std::min(2, std::abs(index - old));
}

bool ExplainSelection(ShopWork *work) {
    work->elapsed = 0;
    const char *text;
    switch (SelectedCell(work)->explanation) {
    case 0x7fffffd0:
        text = "これは、見たまんまジッポだよ";
        break;
    case 0x7fffffd1:
        text = "こいつは、良いライターだぜ";
        break;
    case 0x7fffffd2:
        text = "うん、１９７０年ものだな";
        break;
    case 0x7fffffe0:
        text = "これは、まがたまだよ";
        break;
    case 0x7fffffe1:
        text = "きっとご利益あるぜ";
        break;
    case 0x7ffffff0:
        text = "実は、白いのとセットなんだ";
        break;
    case 0x7ffffff1:
        text = "白黒、両方買っていきなよ";
        break;
    default:
        return false;
    }
    dbgPrintEventMessage(text);
    HLib::SetCurrentTaskTo(Explain_Callback);
    HLib::SetupTaskDelay(60);
    return true;
}

void LockOn_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board) {
        EV_EnableCharacterDisplay(ryo);
        if (work && work->seller.name)
            EV_EnableCharacterDisplay(work->seller);
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    std::int16_t input = static_cast<std::int16_t>(GetCurrentControlInput(7) & 0xf9fb);
    const auto *cell = SelectedCell(work);
    if (cell->explanation != -1) {
        GetBattleDataConfig(0, SHEN_NAME(BTIL));
        if (GetBattleDataConfig(1, SHEN_NAME(BTIL)) && GetBattleDataConfig(2, SHEN_NAME(BTIL)))
            input |= 4;
    }
    GetBattleDataConfig(0, SHEN_NAME(BTEV));
    if (GetBattleDataConfig(1, SHEN_NAME(BTEV)) && GetBattleDataConfig(2, SHEN_NAME(BTEV)))
        input |= 0x400;
    GetBattleDataConfig(0, SHEN_NAME(BTCC));
    if (GetBattleDataConfig(1, SHEN_NAME(BTCC)) && GetBattleDataConfig(2, SHEN_NAME(BTCC)))
        input |= 0x200;
    if (input & 0x400) {
        if (work->board->buyScript == -1)
            HLib::SetCurrentTaskTo(AskBuy_Callback);
        else
            HideBoard(work);
    } else if (input & 0x200)
        HLib::SetCurrentTaskTo(Done_Callback);
    else if (input & 4)
        ExplainSelection(work);
    else if (work->transition > 0) {
        if (input & 0xf000)
            work->bufferedInput = input & 0xf000;
    } else {
        if (!(input & 0xf000))
            input = static_cast<std::int16_t>(work->bufferedInput);
        work->bufferedInput = 0;
        const auto previousVariant = Variant(cell);
        work->transition = 1;
        int distance = 0;
        if ((input & 0x4000) && (distance = MoveSelection(work, true, false))) {
        } else if (input < 0 && (distance = MoveSelection(work, true, true))) {
        } else if ((input & 0x1000) && (distance = MoveSelection(work, false, false))) {
        } else if ((input & 0x2000) && (distance = MoveSelection(work, false, true))) {
        } else
            work->transition = 0;
        if (work->transition) {
            const bool nextVariant = Variant(SelectedCell(work)) != 0;
            work->camera = previousVariant ? (nextVariant ? 3 : 4) : (nextVariant ? 2 : 1);
            work->transition = distance * work->board->transitionFrames[work->camera - 1];
        }
    }
    const auto amount = Snap(-static_cast<float>(GetCurrentControlInput(4)) * .001953125f);
    const auto goal = Snap(amount + 1);
    work->zoom = Snap(work->zoom * .5f);
    work->zoom = Snap(Snap(goal * .5f) + work->zoom);
    UpdateCamera(work);
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            Vector3 position;
            Vector3i rotation;
            const auto name = BoardCell(work->board, &position, &rotation, x, y);
            if (name.name) {
                EV_AT_SetPos(name, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, position);
                EV_AT_SetAngY(name, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, rotation);
            }
        }
}
void Explain_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (work) {
        ctrlStopSubtitles();
        if (!ShowBoard(work))
            HLib::SetCurrentTaskTo(Done_Callback);
    }
}

void Deliver_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->cellsOffset < 0) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    switch (work->elapsed) {
    case 0:
        AddThisToCashOrSegaCoins(-SelectedCell(work)->price, ECashOrCoins::Cash2);
        HLib::SetupTaskDelay(20);
        work->elapsed = 1;
        break;
    case 1:
        ctrlStopSubtitles();
        HLib::SetupTaskDelay(40);
        work->elapsed = 2;
        break;
    case 2:
        dbgPrintEventMessage("ほい。毎度～");
        HLib::SetupTaskDelay(40);
        work->elapsed = 3;
        break;
    case 3:
        ctrlStopSubtitles();
        work->elapsed = 4;
        break;
    case 4:
        if (!GetShouldRenderIcons(RenderIconSlot::MoneyMessage)) {
            AddInventoryItem(SelectedCell(work)->item, 1, 1);
            HLib::SetupTaskDelay(40);
            work->elapsed = 200;
        }
        break;
    case 100:
        ctrlStopSubtitles();
        dbgPrintEventMessage("・・・そうかい。＆まぁ見るだけならただだ");
        HLib::SetupTaskDelay(60);
        work->elapsed = 101;
        break;
    case 101:
        ctrlStopSubtitles();
        work->elapsed = 200;
        break;
    case 200:
        if (!ShowBoard(work))
            HLib::SetCurrentTaskTo(Done_Callback);
        else
            HLib::SetupTaskDelay(30);
        break;
    default:
        break;
    }
}

void WaitBuy_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->cellsOffset <= 0) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (work->elapsed == 100)
        ctrlStopSubtitles();
    ++work->elapsed;
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (work->elapsed <= 100)
        ctrlStopSubtitles();
    if (shenGetCtrlSpeakIconFlag() == 80 && SelectedCell(work)->price > 0) {
        if (static_cast<std::uint64_t>(SelectedCell(work)->price) > GetPlayerStat(PlayerStatsId::Cash)) {
            work->elapsed = 100;
            dbgPrintEventMessage("●青・・・いや、金が足りないな");
        } else {
            work->elapsed = 0;
            dbgPrintEventMessage("●青ああ");
        }
        HLib::SetCurrentTaskTo(Deliver_Callback);
        HLib::SetupTaskDelay(40);
    } else if (!ShowBoard(work))
        HLib::SetCurrentTaskTo(Done_Callback);
}
void AskBuy_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->cellsOffset <= 0) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    dbgPrintEventMessage("これでいいんだな？");
    std::int16_t choices[]{-1, 80, 154, -1};
    SetupControlSpeakAndUI(choices, 21600, -1, 2);
    HLib::SetCurrentTaskTo(WaitBuy_Callback);
    work->elapsed = 0;
}
void WaitStart_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (work->elapsed == 100)
        ctrlStopSubtitles();
    ++work->elapsed;
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (work->elapsed <= 100)
        ctrlStopSubtitles();
    if (shenGetCtrlSpeakIconFlag() != 162) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    UpdateCamera(work);
    if (!ShowBoard(work))
        HLib::SetCurrentTaskTo(Done_Callback);
}

void StartSale_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (work->seller.name) {
        mtwkSetRequestedRootMode(scenFindMotionwork(work->seller), 0);
        mtwkRequestMotion(scenFindMotionwork(work->seller), 0xf087, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(work->seller), 0);
        mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
        mtwkRequestMotion(scenFindMotionwork(ryo), 0xf002, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
        Vector3 player, seller;
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::TurnMotion);
        EV_GetCharaPosition(work->seller, CHARACTER_TRANSLATION_ROOT, &seller, EventTransformFlags::TurnMotion);
        Vector3i rotation{0, static_cast<std::int16_t>(atan2_fixed16(-Snap(player.x - seller.x), -Snap(player.z - seller.z))), 0};
        EV_AT_SetAngY(work->seller, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, rotation);
        rotation.y += 0x8000;
        EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, rotation);
        if (work->package[0])
            EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
        EVS_LoopCamera(1000, work->seller, work->seller);
    }
    dbgPrintEventMessage("おっ、兄ちゃん。見てってくれよ");
    work->elapsed = 0;
    std::int16_t choices[]{-1, 162, 154, -1};
    SetupControlSpeakAndUI(choices, 21600, -1, 2);
    HLib::SetCurrentTaskTo(WaitStart_Callback);
}
void Initialize_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->magic.name != SHEN_NAME(AOZR)) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags |= 2;
    work->x = work->board->initialX;
    work->y = work->board->initialY;
    work->transition = 0;
    work->camera = Variant(SelectedCell(work)) ? 3 : 1;
    if (work->board->startScript == -1)
        HLib::SetCurrentTaskTo(StartSale_Callback);
}

void PrepareSeller(shen_name name) {
    const bool hasMotion = scenFindMotionwork(name) != nullptr;
    if (hasMotion) {
        const Vector4 values{1, 1, 1, 1};
        SetScenarioReflectorBoardValues(name, &values);
        EV_SetRefBoardParamsFor(name, 1);
    }
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 0, NpcCharacterControllerState::Disabled);
    if (hasMotion)
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
    EV_SetCharacterAlphaEnabled(name, false);
}
void GetNpc_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (EV_ForceLoadNpc(work->seller)) {
        PrepareSeller(work->seller);
        HLib::SetCurrentTaskTo(Initialize_Callback);
    }
}
void WaitPackage_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(work->package)))
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    work->board = static_cast<const Board *>(LoadCurrentPakBinary("main.bin"));
    if (!work->board) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (work->board->audio[0] && work->board->audioVariant >= 0) {
        EV_LoadAudio(work->board->audio, work->board->audioVariant);
        work->flags |= 4;
    }
    work->seller = work->board->seller;
    if (work->seller.name && EV_CheckWarmStartNpcRoute(work->seller)) {
        work->flags |= 1;
        HLib::SetCurrentTaskTo(GetNpc_Callback);
    } else
        HLib::SetCurrentTaskTo(Initialize_Callback);
}
void StartPackage_Callback(void *data) {
    if (!data)
        HLib::SetCurrentTaskTo(Done_Callback);
    else if (!filmCheckFadeTrackColour()) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        HLib::SetCurrentTaskTo(WaitPackage_Callback);
    }
}

void Done_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    HLib::Task *parent = nullptr;
    if (work) {
        parent = work->parent;
        if (filmCheckFadeTrackColour())
            return;
        if (work->flags & 2) {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags &= ~2U;
            return;
        }
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        HideBoard(work);
        if ((work->flags & 4) && work->savedAudio[0]) {
            work->flags &= ~4U;
            EV_LoadCutsceneFromCurrentScene(work->savedAudio);
            EV_LoadAudio(work->savedAudio, work->y);
            work->savedAudio[0] = 0;
        }
        auto restore = [](shen_name name) {
            const bool motion = scenFindMotionwork(name) != nullptr;
            EV_SetCharacterAlphaEnabled(name, true);
            if (motion)
                mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
            EV_SetCharaControllerFlags(name, 0, NpcCharacterControllerState::Enabled);
            EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Enabled);
            if (motion)
                EV_SetRefBoardParamsFor(name, 0);
        };
        if (work->seller.name) {
            restore(work->seller);
            AT_SetLocalBaseChara(work->seller, invalid);
            EV_npcLockUnlock(work->seller);
            work->seller = shen_name{};
        }
        EV_NPC_NewLoadSw((work->flags & 16) != 0);
        restore(ryo);
        if ((work->flags & 8) && work->board && work->board->authorBank[0]) {
            EV_FreeAuthorPak(0, work->board->authorBank);
            work->flags &= ~8U;
        }
        if (work->package[0] && EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work->package), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
    }
    SetPlayerControlEnabled(1);
    HLib::MarkCurrentTaskForCleanup();
    if (parent)
        HLib::ResolveDeferredCallback(parent);
    else {
        DateTime time;
        GetCurrentDateTime(time);
        time.hour = 23;
        time.minute = 30;
        SetCurrentDateTimeTo(time);
    }
}
} // namespace
void AreaAOZRInit() {
}

void AreaAOZRMain(std::uintptr_t parameter) {
    const auto argument = static_cast<std::int32_t>(parameter);
    if (argument > 999)
        return;
    const char *sound = shenGetCurrentSbt();
    const auto length = sound ? std::strlen(sound) : 0;
    auto *task = HLib::EnqueueTaskWithParameter(StartPackage_Callback, 4, 0x0b, 0x58 + length, event);
    if (!task)
        return;
    auto *work = static_cast<ShopWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    std::memset(work, 0, 0x58);
    work->self = task;
    work->parent = HLib::GetCurrentTask();
    std::memcpy(work->package, "AOZR", 4);
    const auto index = (argument >> 1) + 1;
    work->package[4] = static_cast<char>((index / 100) % 10 + 48);
    work->package[5] = static_cast<char>((index / 10) % 10 + 48);
    work->package[6] = static_cast<char>(index % 10 + 48);
    work->package[7] = 0;
    work->zoom = 1;
    work->y = static_cast<std::int32_t>(GetSelectedSoundSceneIndex());
    auto *savedAudio = reinterpret_cast<char *>(work) + 0x50;
    if (length)
        std::memcpy(savedAudio, sound, length);
    savedAudio[length] = 0;
    SetPlayerControlEnabled(0);
    HLib::SetupTaskDelay(0);
    SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
    work->flags &= ~2U;
}
const ScriptAreaDefinition areaAOZRScriptDefinition{.areaId = SHEN_NAME(AOZR), .field04 = 0, .init = AreaAOZRInit, .main = AreaAOZRMain};
} // namespace shenmue::s2::areas
