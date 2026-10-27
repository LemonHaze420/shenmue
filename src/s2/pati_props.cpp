#include "s2/pati.hpp"
#include "s2/pati_tables.hpp"
#include "d3t/object_store.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/inventory.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/mt7.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace shenmue::s2 {
namespace {
struct PatiBoardProps {
    MT7Model model;
    std::vector<LegacyNodeCommandOverride> commands;
    PatiPlacementNode *primary;
    PatiPlacementNode *accessories;
    std::uint32_t primaryCount;
};
PatiBoardProps patiBoardProps[PATI_BOARD_CAPACITY];
std::uint32_t patiBoardPropCount;

shen_name PatiCharacterName(const char *name) {
    shen_name result;
    std::memcpy(&result, name, sizeof(result));
    return result;
}
void AssignPatiPropMesh(PatiBoardProps &props, std::uint32_t index, MT7Model *source, std::uint16_t id, std::uint32_t sourceNode = 0) {
    if (!source)
        return;
    MT7Node &node = props.model.nodes[index];
    MT7Node &mesh = source->nodes[sourceNode];
    d3t::RegisterObjectPlacement(GetMT7ReaderData(source), &node, 0);
    node.id = id;
    node.xb01Offset = mesh.xb01Offset;
    node.runtimeMesh = mesh.runtimeMesh;
    node.borrowedMesh = mesh.borrowedMesh ? mesh.borrowedMesh : &mesh;
    LegacyNodeCommandOverride &commands = props.commands[index];
    commands = {.nodeSourceOffset = node.sourceOffset, .bytes = source->source, .xb01Offset = mesh.xb01Offset, .resources = &node.borrowedMesh->drawResources};
    node.commandOverride = &commands;
}
PatiPlacementNode *GetPatiPlacementSource(const PatiBoardProps &props, const MT7Node *node) {
    if (!node)
        return nullptr;
    const std::uintptr_t pointer = reinterpret_cast<std::uintptr_t>(node);
    const std::uintptr_t first = reinterpret_cast<std::uintptr_t>(props.model.nodes.data());
    const std::uintptr_t end = first + props.model.nodes.size() * sizeof(MT7Node);
    if (pointer < first || pointer >= end)
        return nullptr;
    const std::size_t index = (pointer - first) / sizeof(MT7Node);
    return index < props.primaryCount ? &props.primary[index] : &props.accessories[index - props.primaryCount];
}
void StorePatiPlacementNodes(PatiBoardProps &props, MT7Model &characterModel) {
    for (std::size_t index = 0; index < props.model.nodes.size(); ++index) {
        const MT7Node &node = props.model.nodes[index];
        PatiPlacementNode *const destination = index < props.primaryCount ? &props.primary[index] : &props.accessories[index - props.primaryCount];
        PatiPlacementNode *const child = GetPatiPlacementSource(props, node.runtimeChild);
        PatiPlacementNode *const sibling = GetPatiPlacementSource(props, node.runtimeSibling);
        PatiPlacementNode *const parent = GetPatiPlacementSource(props, node.runtimeParent);
        *destination = {.signature = node.id,
            .position = node.position,
            .rotation = node.rotation,
            .scale = node.scale,
            .meshOffset = node.xb01Offset && node.runtimeMesh ? HLib::GetDynamicHeapOffset(node.runtimeMesh) : 0,
            .childOffset = child ? HLib::GetDynamicHeapOffset(child) : 0,
            .siblingOffset = sibling ? HLib::GetDynamicHeapOffset(sibling) : 0,
            .parentOffset = parent ? HLib::GetDynamicHeapOffset(parent) : 0,
            .transform = nullptr};
    }
    std::byte *const root = const_cast<std::byte *>(characterModel.source.data()) + characterModel.nodes[0].sourceOffset;
    props.primary[0].parentOffset = HLib::GetDynamicHeapOffset(root);
    const std::int32_t child = HLib::GetDynamicHeapOffset(props.primary);
    std::memcpy(root + 44, &child, sizeof(child));
}
}

void ReleasePatiProps() {
    for (std::uint32_t index = 0; index < patiBoardPropCount; ++index) {
        PatiBoardProps &props = patiBoardProps[index];
        for (MT7Node &node : props.model.nodes)
            d3t::RetireObjectPlacement(&node);
        props.model.nodes.clear();
        props.commands.clear();
        HLib::FreeTaskData(props.primary);
        HLib::FreeTaskData(props.accessories);
        props.primary = nullptr;
        props.accessories = nullptr;
    }
    patiBoardPropCount = 0;
}

void InitPati() {
    PatiModelCache cache;
    cache.count = 0;
    PatiTableData data;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("PATI"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    PATI_TBL_Parser(&data);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1721, static_cast<std::uint8_t>(GetItemState(3021)) == 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1722, static_cast<std::uint8_t>(GetItemState(3022)) == 0);
    std::int32_t expandedRadius = 0;
    Vector3 boardPosition{};
    for (std::uint32_t tableIndex = 0; data.tables[tableIndex].area.name; ++tableIndex) {
        PatiTableEntry &table = data.tables[tableIndex];
        if (table.area.name != g_currentMap.name)
            continue;
        const shen_name tableCharacter = PatiCharacterName(table.character);
        const shen_name surfaceCharacter = PatiCharacterName(table.surface);
        Vector3 tableScale{table.scaleX, table.scaleY, 1.0f};
        CreateEventCharacter(CharacterCreationHighModel, nullptr, tableCharacter);
        EV_AT_SetPos(tableCharacter, EventTransformFlags::AllAxes, table.position);
        EV_AT_SetAngY(tableCharacter, EventTransformFlags::AllAxes, table.rotation, nullptr);
        if (surfaceCharacter.name == 0x20202020U) {
            boardPosition.y = 0.0f;
        } else if (static_cast<std::uint8_t>(surfaceCharacter.name) == 'T') {
            if ((surfaceCharacter.name & 0xffff0000U) != 0x4d440000U) {
                CreateEventCharacter(CharacterCreationTable0, reinterpret_cast<const std::byte *>("TABLMOTG"), surfaceCharacter);
                mtwkSetMotionworkScale3(surfaceCharacter, &tableScale);
                AT_SetLocalBaseChara(surfaceCharacter, tableCharacter);
                EV_AT_SetPos(surfaceCharacter, EventTransformFlags::AllAxes, Vector3{});
                EV_AT_SetAngY(surfaceCharacter, EventTransformFlags::AllAxes, Vector3i{}, nullptr);
                EV_EnableCharacterDisplay(surfaceCharacter);
            }
            boardPosition.y = static_cast<float>(tableScale.y * 0.75);
        } else if (static_cast<std::uint8_t>(surfaceCharacter.name) == 'G') {
            if ((surfaceCharacter.name & 0xffff0000U) != 0x4d440000U) {
                CreateEventCharacter(CharacterCreationTable0, reinterpret_cast<const std::byte *>("PCKOMGZG"), surfaceCharacter);
                mtwkSetMotionworkScale3(surfaceCharacter, &tableScale);
                AT_SetLocalBaseChara(surfaceCharacter, tableCharacter);
                EV_AT_SetPos(surfaceCharacter, EventTransformFlags::AllAxes, Vector3{0.0f, 0.004999999888241291f, 0.0f});
                EV_AT_SetAngY(surfaceCharacter, EventTransformFlags::AllAxes, Vector3i{}, nullptr);
                EV_EnableCharacterDisplay(surfaceCharacter);
            }
            boardPosition.y = tableScale.y * 0.004999999888241291f;
        }
        for (std::int32_t boardIndex = 0; boardIndex < table.boardCount; ++boardIndex) {
            PatiBoardEntry &board = data.boards[table.firstBoard + boardIndex];
            const PatiGameOption *option = PATI_GAME_OPTIONS;
            while (option->name[0] && std::strcmp(option->name, table.gameOptions))
                ++option;
            if (!option->name[0])
                break;
            const PatiGameType *type = PATI_GAME_TYPES;
            while (type->name[0] && std::strcmp(type->name, table.gameType))
                ++type;
            if (!type->name[0])
                break;
            const shen_name boardCharacter = PatiCharacterName(board.character);
            const char *boardModel;
            float labelHeight;
            if (board.size[0] == 'M') {
                boardModel = "PCKDMAAG";
                boardPosition.z = -0.169000000f;
                labelHeight = 0.30000001192092896f;
            } else if (board.size[0] == 'S') {
                boardModel = "PCKDMCAG";
                boardPosition.z = -0.074000000f;
                labelHeight = 0.20000000298023224f;
            } else {
                boardModel = board.size[0] == 'X' ? "PCKDMDAG" : "PCKDMBAG";
                boardPosition.z = -0.3840000033378601f;
                labelHeight = 0.6499999761581421f;
            }
            boardPosition.x = board.positionX;
            if (CreateEventCharacter(CharacterCreationTable0, reinterpret_cast<const std::byte *>(boardModel), boardCharacter).name != boardCharacter.name)
                expandedRadius = 0;
            std::int32_t boardExpandedRadius = expandedRadius;
            AT_SetLocalBaseChara(boardCharacter, tableCharacter);
            EV_AT_SetPos(boardCharacter, EventTransformFlags::AllAxes, boardPosition);
            if (option->mode == 6) {
                EV_AT_SetAngY(boardCharacter, EventTransformFlags::AllAxes, Vector3i{50062, 32768, 0}, nullptr);
                if (Character *const character = ctrlGetCharaByName(boardCharacter))
                    character->radius = 200.0f;
                camCtrlSetFarDistance(200.0f);
                boardExpandedRadius = 1;
            } else {
                EV_AT_SetAngY(boardCharacter, EventTransformFlags::AllAxes, Vector3i{60074, 32768, 0}, nullptr);
            }
            Vector3 scale{option->scale, option->scale, option->scale};
            mtwkSetMotionworkScale3(boardCharacter, &scale);
            EV_EnableCharacterDisplay(boardCharacter);
            const std::uint32_t prizeCount = static_cast<std::uint32_t>(std::strlen(board.prizes));
            const std::uint32_t propCount = (option->mode == 8 ? 25 : 13) + 2 * prizeCount;
            PatiBoardProps &props = patiBoardProps[patiBoardPropCount++];
            props.primary = static_cast<PatiPlacementNode *>(HLib::AllocateTaskData(propCount * sizeof(PatiPlacementNode), boardCharacter));
            props.primaryCount = propCount;
            for (std::uint32_t index = 0; index < propCount; ++index)
                props.primary[index] = {.signature = 152,
                    .position = {},
                    .rotation = {},
                    .scale = {1.0f, 1.0f, 1.0f},
                    .meshOffset = 0,
                    .childOffset = 0,
                    .siblingOffset = 0,
                    .parentOffset = 0,
                    .transform = nullptr};
            props.model.source = {reinterpret_cast<const std::byte *>(props.primary), propCount * sizeof(PatiPlacementNode)};
            props.model.nodes.resize(propCount + 2);
            props.commands.resize(propCount + 2);
            for (std::uint32_t index = 0; index < propCount + 2; ++index) {
                MT7Node &node = props.model.nodes[index];
                node = {};
                node.id = 152;
                node.sourceOffset = index * 64;
                node.scale = {1.0f, 1.0f, 1.0f};
                if (index + 1 < propCount)
                    node.runtimeSibling = &props.model.nodes[index + 1];
                if (index)
                    node.runtimeParent = &props.model.nodes[index - 1];
            }
            Character *const character = ctrlGetCharaByName(boardCharacter);
            character->field13c = 1;
            MT7Model *const characterModel = static_cast<MT7Model *>(character->model);
            props.model.nodes[0].runtimeParent = &characterModel->nodes[0];
            characterModel->nodes[0].runtimeChild = &props.model.nodes[0];
            props.model.nodes[1].runtimeSibling = &props.model.nodes[13];
            props.model.nodes[13].runtimeParent = &props.model.nodes[1];
            props.model.nodes[1].runtimeChild = &props.model.nodes[2];
            props.model.nodes[2].runtimeParent = &props.model.nodes[1];
            props.model.nodes[12].runtimeChild = nullptr;
            props.model.nodes[12].runtimeSibling = nullptr;
            props.model.nodes[1].position.x = 0.46000000834465027f;
            float stake = static_cast<float>(std::atof(table.stake));
            float payout = static_cast<float>(std::atof(table.payout));
            float ratio = payout <= 0.0f || stake <= 0.0f ? 0.0f : payout / stake;
            std::int32_t headerModel = 0;
            if (type->mode >= 1 && type->mode <= 8) {
                MT7Model *const stakeModel = LoadPatiModel(PATI_STAKE_MODELS[type->mode - 1], boardExpandedRadius, &cache);
                if (type->mode == 6) {
                    ratio = -1.0f;
                    payout = 0.0f;
                }
                AssignPatiPropMesh(props, 2, stakeModel, 152);
            }
            const char *payoutModel = nullptr;
            switch (option->mode) {
            case 1:
            case 2:
                if (!std::strcmp(board.character, table.selectedBoard)) {
                    headerModel = 1;
                    stake = 0.0f;
                    ratio = 0.0f;
                    props.model.nodes[2].xb01Offset = 0;
                    payoutModel = "PCKHMF8G";
                } else
                    payoutModel = "PCKHMF1G";
                break;
            case 3:
                payoutModel = "PCKHMF5G";
                break;
            case 4:
                if (static_cast<std::int32_t>(payout) == 2)
                    payoutModel = "PCKHMF2G";
                else {
                    payoutModel = static_cast<std::int32_t>(payout) == 5 ? "PCKHMF3G" : "PCKHMF4G";
                    props.model.nodes[2].xb01Offset = 0;
                }
                if (!std::strcmp(board.character, table.selectedBoard))
                    headerModel = 2;
                else {
                    headerModel = 1;
                    stake = 0.0f;
                }
                ratio = 0.0f;
                break;
            case 7:
                payoutModel = "PCKHMF7G";
                headerModel = 2;
                ratio = 0.0f;
                break;
            case 8:
                payoutModel = "PCKHMF6G";
                break;
            default:
                break;
            }
            if (payoutModel)
                AssignPatiPropMesh(props, 3, LoadPatiModel(payoutModel, boardExpandedRadius, &cache), 153);
            if (!props.model.nodes[2].xb01Offset || props.model.nodes[3].xb01Offset) {
                if (!headerModel) {
                    props.model.nodes[2].position.y = 0.11500000208616257f;
                    props.model.nodes[3].position.y = 0.11500000208616257f;
                }
            } else {
                headerModel = 3;
                props.model.nodes[2].position.y = 0.03500000014901161f;
                props.model.nodes[3].position.y = 0.0f;
            }
            if (!headerModel || headerModel == 3) {
                for (std::uint32_t index = 4; index <= 7; ++index)
                    props.model.nodes[index].position.y = 0.11500000208616257f;
            }
            AssignPatiPropMesh(props, 1, LoadPatiModel(PATI_BOARD_HEADER_MODELS[headerModel], boardExpandedRadius, &cache), 154);
            const std::int32_t stakeValue = static_cast<std::int32_t>(stake);
            const std::int32_t ratioValue = static_cast<std::int32_t>(ratio);
            props.model.nodes[4].position.x = stakeValue >= 100 ? 0.0f : stakeValue >= 10 ? 0.0175f : 0.035f;
            props.model.nodes[5].position.x = stakeValue >= 100 ? 0.035f : stakeValue >= 10 ? 0.05249999836087227f : 0.07f;
            props.model.nodes[6].position.x = stakeValue >= 10 && stakeValue < 100 ? 0.05249999836087227f : 0.07f;
            props.model.nodes[7].position.x = stakeValue >= 100 ? 0.105f : stakeValue >= 10 ? 0.08749999850988388f : 0.07f;
            if (stakeValue)
                AssignPatiPropMesh(props, 4, LoadPatiModel("PCKHMSBG", boardExpandedRadius, &cache), 155);
            std::int32_t divisor = 100;
            bool leadingZero = true;
            for (std::uint32_t index = 5; index <= 7; ++index, divisor /= 10) {
                const std::int32_t digit = stakeValue / divisor % 10;
                leadingZero = digit == 0 && leadingZero;
                if (!leadingZero)
                    AssignPatiPropMesh(props, index, LoadPatiModel(PATI_DIGIT_MODELS[digit], boardExpandedRadius, &cache), 156);
                else
                    props.model.nodes[index].xb01Offset = 0;
            }
            props.model.nodes[8].position.x = ratioValue >= 100 ? 0.0f : ratioValue >= 10 ? 0.0175f : 0.035f;
            props.model.nodes[9].position.x = ratioValue >= 100 ? 0.035f : ratioValue >= 10 ? 0.05249999836087227f : 0.07f;
            props.model.nodes[10].position.x = ratioValue >= 10 && ratioValue < 100 ? 0.05249999836087227f : 0.07f;
            props.model.nodes[11].position.x = ratioValue >= 100 ? 0.105f : ratioValue >= 10 ? 0.08749999850988388f : 0.07f;
            if (ratioValue)
                AssignPatiPropMesh(props, 8, LoadPatiModel("PCKHMSCG", boardExpandedRadius, &cache), 157);
            if (ratioValue != -1) {
                divisor = 100;
                leadingZero = true;
                for (std::uint32_t index = 9; index <= 11; ++index, divisor /= 10) {
                    const std::int32_t digit = ratioValue / divisor % 10;
                    leadingZero = digit == 0 && leadingZero;
                    if (!leadingZero)
                        AssignPatiPropMesh(props, index, LoadPatiModel(PATI_DIGIT_MODELS[digit], boardExpandedRadius, &cache), 158);
                    else
                        props.model.nodes[index].xb01Offset = 0;
                }
            } else {
                props.model.nodes[8].position.x = 0.0f;
                props.model.nodes[11].position.x = 0.0f;
                AssignPatiPropMesh(props, 11, LoadPatiModel("PCKHMSAG", boardExpandedRadius, &cache), 159);
            }
            if (board.labelModel[0]) {
                AssignPatiPropMesh(props, 13, LoadPatiModel(board.labelModel, boardExpandedRadius, &cache), 160);
                props.model.nodes[13].position.y = labelHeight;
            }
            std::uint32_t nextNode = 14;
            MT7Model *const separator = LoadPatiModel("PCKAMITG", boardExpandedRadius, &cache);
            for (std::uint32_t index = 0; index < prizeCount; ++index) {
                char &prizeCode = board.prizes[index];
                if (static_cast<std::uint8_t>(prizeCode - '6') <= 2) {
                    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1721) == 1)
                        prizeCode = '6';
                    else
                        prizeCode = static_cast<char>((EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1722) != 1) + '7');
                }
                const PatiPrize *prize = PATI_PRIZES;
                while (prize->code && prize->code != prizeCode)
                    ++prize;
                AssignPatiPropMesh(props, nextNode, LoadPatiModel(prize->model, boardExpandedRadius, &cache), 161);
                const std::uint32_t positionIndex = index + 10 * (prizeCount - 4);
                props.model.nodes[nextNode++].position.x = PATI_PRIZE_X[positionIndex + 12];
                if (index + 1 < prizeCount) {
                    AssignPatiPropMesh(props, nextNode, separator, 162);
                    props.model.nodes[nextNode++].position.x = PATI_PRIZE_SEPARATOR_X[positionIndex] + 0.0024999999441206455f;
                }
            }
            if (option->mode == 8) {
                MT7Model *const markerA = LoadPatiModel("PCKAMP1G", boardExpandedRadius, &cache);
                MT7Model *const markerB = LoadPatiModel("PCKAMP2G", boardExpandedRadius, &cache);
                MT7Model *const markerC = LoadPatiModel("PCKAMP3G", boardExpandedRadius, &cache);
                for (std::uint32_t index = 0; index < 6; ++index) {
                    const char marker = board.prizeModes[index];
                    if (marker == ' ')
                        continue;
                    const PatiPrize *prize = PATI_PRIZES;
                    while (prize->code && prize->code != board.prizeItems[index])
                        ++prize;
                    AssignPatiPropMesh(props, nextNode, LoadPatiModel(prize->model, boardExpandedRadius, &cache), 163);
                    props.model.nodes[nextNode++].position = {PATI_PRIZE_X[2 * index], PATI_PRIZE_X[2 * index + 1], 0.004999999888241291f};
                    AssignPatiPropMesh(props, nextNode,
                        marker == 'A'     ? markerA :
                            marker == 'B' ? markerB :
                                            markerC,
                        marker == 'A'     ? 164 :
                            marker == 'B' ? 165 :
                                            166);
                    props.model.nodes[nextNode++].position = {PATI_PRIZE_X[2 * index], PATI_PRIZE_X[2 * index + 1], 0.004999999888241291f};
                }
            }
            props.accessories = static_cast<PatiPlacementNode *>(HLib::AllocateTaskData(128, boardCharacter));
            MT7Node &accessoryA = props.model.nodes[propCount];
            MT7Node &accessoryB = props.model.nodes[propCount + 1];
            accessoryA.runtimeParent = &props.model.nodes[0];
            accessoryA.runtimeSibling = &accessoryB;
            accessoryB.runtimeParent = &accessoryA;
            props.model.nodes[0].runtimeChild = &accessoryA;
            MT7Model *const accessory = LoadPatiModel(board.model, boardExpandedRadius, &cache);
            MT7Node *const firstAccessory = accessory->nodes[0].runtimeChild;
            MT7Node *const secondAccessory = firstAccessory->runtimeSibling;
            AssignPatiPropMesh(props, propCount, accessory, 167, static_cast<std::uint32_t>(firstAccessory - accessory->nodes.data()));
            AssignPatiPropMesh(props, propCount + 1, accessory, 152, static_cast<std::uint32_t>(secondAccessory - accessory->nodes.data()));
            StorePatiPlacementNodes(props, *characterModel);
            expandedRadius = boardExpandedRadius;
        }
    }
}

} // namespace shenmue::s2
