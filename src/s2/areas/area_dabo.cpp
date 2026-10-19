#include "s2/areas/area_dabo.hpp"
#include "s2/area_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/scene_data.hpp"
#include <array>
#include <cstring>
namespace shenmue::s2 {
namespace dabo {
constexpr shen_name boards[]{SHEN_NAME(BD01), SHEN_NAME(BD02), SHEN_NAME(BD03)};
constexpr shen_name dummy{SHEN_NAME(DMY1)};
std::array<MT7Model *, 4> pocketModels{};
std::byte *dummySource{};
std::array<MT7Node, 30> dummyNodes;
void SelectBoard(DaboEventWork &w) {
    auto &p = *w.pachi;
    auto &entries = p.tables.boards;
    if (w.selectedBoard < 0) {
        w.selectedBoard = 0;
        if (entries[0].model[0])
            do {
                ++w.selectedBoard;
            } while (entries[w.selectedBoard].model[0]);
    }
    if (!entries[w.selectedBoard].model[0])
        w.selectedBoard = 0;
    if (w.selectedBoard == w.previousBoard)
        return;
    w.previousBoard = w.selectedBoard;
    std::size_t tableIndex = 0;
    while (w.selectedBoard >= static_cast<int>(p.tables.tables[tableIndex].firstBoard + p.tables.tables[tableIndex].boardCount))
        ++tableIndex;
    const auto &table = p.tables.tables[tableIndex];
    std::memcpy(&p.board->area, &table.area, 4);
    std::memcpy(&p.board->table, table.tableName, 4);
    const auto &board = entries[w.selectedBoard];
    switch (board.size[0]) {
    case 'M':
        w.boardSize = 0;
        break;
    case 'L':
    case 'X':
        w.boardSize = 1;
        break;
    case 'S':
        w.boardSize = 2;
        break;
    default:
        EV_debug_log("DABO\\eDABO_et_dabo.c", 386, "PATI_TBL : illegal board size.\n");
        break;
    }
    p.nails->character = SHEN_NAME(PIN_);
    for (auto &nail : p.nails->nails) {
        nail.reserved00 = 0;
        nail.position = {0, -1, 0};
        nail.rotation = {};
        nail.scale = {1, 1, 1};
    }
    ParsePatiNailFile(p.nails, board.model);
    AT_SetLocalBaseChara(p.nails->character, p.board->character);
    EV_AT_SetPos(p.nails->character, EventTransformFlags::AllAxes, Vector3{});
    EV_AT_SetAngY(p.nails->character, EventTransformFlags::AllAxes, Vector3i{});
    DisableScenarioCharacterDisplay(p.nails->character);
    for (auto &nail : p.nails->nails) {
        nail.scale = {p.scale * 1.5f, p.scale * 1.5f, p.scale};
        nail.position.x *= p.scale;
        nail.position.y *= p.scale;
        nail.position.z *= p.scale;
    }
    w.gameType = static_cast<int>(p.field8638) - 4;
}
void StartBoard(DaboEventWork &w) {
    auto &board = *w.pachi->board;
    board.task = w.task;
    board.state18 = board.state1c = board.ballState = 0;
    SetMotionRegionFlag(scenFindMotionwork(SHEN_NAME(RYO_)), 71, 0, MotionRegion::LowerBody);


    LoadAreaFunc_Wrapper_0("OTOS.EV3", 0);
    ExecuteCurrAreaMainFunc("OTOS.EV3", reinterpret_cast<std::uintptr_t>(&board));
    HLib::SetupTaskDelay(0);
}
void PlayDebug(void *parameter) {
    auto &w = *static_cast<DaboEventWork *>(parameter);
    if (w.pachi->board->ballState == -1)
        EV_debug_log("DABO\\eDABO_et_dabo.c", 663, "Error:Ball did not goal.\n");
    SelectBoard(w);
}
void UpdateDebug(DaboEventWork &w) {
    const auto input = GetCurrentControlInput(7);
    if (input & 0x8000U)
        ++w.selectedBoard;
    else if (input & 0x4000U)
        --w.selectedBoard;
    SelectBoard(w);
    if (input & 4U) {
        if (++w.gameType > 6)
            w.gameType = 0;
    }
    if (w.gameType != w.previousType)
        w.previousType = w.gameType;
    if (EVT_GetControllerInput(0, 7) & PAD_UP) {
        if (--w.boardSize < 0)
            w.boardSize = 2;
    } else if (EVT_GetControllerInput(0, 7) & PAD_DOWN) {
        if (++w.boardSize >= 3)
            w.boardSize = 0;
    }
    auto &p = *w.pachi;
    if (w.boardSize != w.previousSize) {
        DisableScenarioCharacterDisplay(boards[w.previousSize]);
        EnableScenarioCharacterDisplay(boards[w.boardSize]);
        w.previousSize = w.boardSize;
        p.board->character = boards[w.boardSize];
        p.launcherHeight = w.boardSize == 0 ? 1.09f : w.boardSize == 2 ? .88999999f : 1.89f;
        p.launcherStep = .0049999999f;
        p.launcherMinimum = -.30000001f;
        p.launcherMaximum = .30000001f;
        p.cameraMode = w.boardSize == 0 ? 2 : w.boardSize == 2 ? 1 : p.scale == 1.0f ? 3 : 4;
    }
    if (GetCurrentControlInput(8))
        StartBoard(w);
    else if (GetCurrentControlInput(9)) {
        std::memset(w.counters, 0, sizeof(w.counters));
        p.board->ballState = 0;
        HLib::SetCurrentTaskTo(PlayDebug);
    }
}
void AttachDummyNodes() {
    dummySource = static_cast<std::byte *>(HLib::AllocateTaskData(1920, dummy));
    for (std::size_t i = 0; i < dummyNodes.size(); ++i) {
        auto *source = reinterpret_cast<MT7NodeSource *>(dummySource + i * 64);
        std::memset(source, 0, 64);
        source->id = 152;
        source->scale = {1, 1, 1};
        auto &node = dummyNodes[i];
        node = {};
        node.sourceOffset = static_cast<std::uint32_t>(i * 64);
        node.id = 152;
        node.scale = {1, 1, 1};
    }
    Character *const character = ctrlGetCharaByName(dummy);
    MT7Node *parent = static_cast<MT7Node *>(character->model);
    while (parent->runtimeChild)
        parent = parent->runtimeChild;
    MT7Model *const reader = GetImageModuleSource(character);
    auto *parentSource = reinterpret_cast<MT7NodeSource *>(const_cast<std::byte *>(reader->source.data()) + parent->sourceOffset);
    reinterpret_cast<MT7NodeSource *>(dummySource)->parentOffset = static_cast<std::uint32_t>(HLib::GetDynamicHeapOffset(parentSource));
    parentSource->childOffset = static_cast<std::uint32_t>(HLib::GetDynamicHeapOffset(dummySource));
    dummyNodes[0].runtimeParent = parent;
    parent->runtimeChild = &dummyNodes[0];
    for (std::size_t i = 1; i < dummyNodes.size(); ++i) {
        dummyNodes[i].runtimeParent = &dummyNodes[i - 1];
        dummyNodes[i - 1].runtimeSibling = &dummyNodes[i];
        auto *source = reinterpret_cast<MT7NodeSource *>(dummySource + i * 64);
        auto *previous = reinterpret_cast<MT7NodeSource *>(dummySource + (i - 1) * 64);
        source->parentOffset = static_cast<std::uint32_t>(HLib::GetDynamicHeapOffset(previous));
        previous->siblingOffset = static_cast<std::uint32_t>(HLib::GetDynamicHeapOffset(source));
    }
}
void Setup(DaboEventWork &w) {
    w.selectedBoard = 0;
    w.previousBoard = 1;
    w.boardSize = 0;
    w.previousSize = 1;
    w.gameType = 0;
    w.previousType = 1;
    w.pachi->board->character = boards[0];
    for (auto board : boards) {
        DisableScenarioCharacterDisplay(board);
        EV_AT_SetAngY(board, EventTransformFlags::AllAxes, Vector3i{60074, 32768, 0});
    }
    EnableScenarioCharacterDisplay(boards[w.boardSize]);
    auto *task = HLib::EnqueueTaskWithParameter(DrawPatiNailsCallback, 9, 0x0b, PATI_NAIL_DATA_BYTES, AREA_EVENT_TASK);
    w.pachi->nailDrawTask = task;
    w.pachi->nails = static_cast<PatiNailData *>(HLib::GetTaskParameter(task));
    if (CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("PCKOMGZG"), dummy).name != dummy.name)
        EV_debug_log("DABO\\eDABO_et_dabo.c", 276, "Error:Dummy Create Error\n");
    EV_AT_SetPos(dummy, EventTransformFlags::AllAxes, Vector3{});
    EV_AT_SetAngY(dummy, EventTransformFlags::AllAxes, Vector3i{60074, 32768, 0});
    EnableScenarioCharacterDisplay(dummy);
    PatiModelCache cache;
    cache.count = 0;
    g_patiPocketModelCache = &cache;
    pocketModels[0] = LoadPocketModel("PCKAMITG");
    pocketModels[1] = LoadPocketModel("PCKAMP1G");
    pocketModels[2] = LoadPocketModel("PCKAMP2G");
    pocketModels[3] = LoadPocketModel("PCKAMP3G");
    AttachDummyNodes();
    PATI_TBL_Parser(&w.pachi->tables);
    SelectBoard(w);
}
} // namespace dabo
void DABO_init_func() {
    dabo::pocketModels.fill(nullptr);
    dabo::dummySource = nullptr;
}
void DABO_main_func(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
    auto *task = HLib::EnqueueTaskWithParameter(DaboDebugCallback, 4, 0x0b, sizeof(DaboEventWork), AREA_EVENT_TASK);
    auto &w = *static_cast<DaboEventWork *>(HLib::GetTaskParameter(task));
    w.task = task;
    w.pachi = &w.inlinePachi;
    w.pachi->board = &w.inlineBoard;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("PATI"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    dabo::Setup(w);
}
void DaboDebugCallback(void *parameter) {
    auto &w = *static_cast<DaboEventWork *>(parameter);
    dabo::UpdateDebug(w);
    auto &board = *w.pachi->board;
    const auto mode = w.pachi->cameraMode;
    EVS_LoopCamera(mode == 2 ? 700 : (static_cast<std::uint32_t>(mode - 3) <= 1 ? 702 : 701), board.character, board.character);
}
const ScriptAreaDefinition areaDABOScriptDefinition{.areaId = SHEN_NAME(DABO), .field04 = 0, .init = DABO_init_func, .main = DABO_main_func};
} // namespace shenmue::s2
