#include "s2/areas/pati_event.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_clone.hpp"
#include "s2/mt7.hpp"
#include "d3t/object_store.hpp"
namespace shenmue::s2 {
namespace {
MT7Node patiNailNodes[512];
LegacyNodeCommandOverride patiNailCommands[512];
MT7Node *patiHiddenNailNodes[2];
ModelCloneNode *patiHiddenNailRecords[2];
std::int32_t patiSavedNailMeshOffsets[2];
std::uint32_t patiSavedNailXb01Offsets[2];
std::byte *patiSavedNailMeshes[2];
std::uint32_t patiRegisteredNails;
} // namespace
// @verified - p
void RestorePatiNailModels() {
    for (std::uint32_t index = 0; index < 2; ++index) {
        if (!patiHiddenNailNodes[index])
            continue;
        patiHiddenNailRecords[index]->meshOffset = patiSavedNailMeshOffsets[index];
        patiHiddenNailNodes[index]->xb01Offset = patiSavedNailXb01Offsets[index];
        patiHiddenNailNodes[index]->runtimeMesh = patiSavedNailMeshes[index];
        if (index == 1) {
            patiHiddenNailRecords[index]->siblingOffset = 0;
            patiHiddenNailNodes[index]->runtimeSibling = nullptr;
        }
    }
    patiHiddenNailNodes[0] = nullptr;
    patiHiddenNailNodes[1] = nullptr;
    for (std::uint32_t index = 0; index < patiRegisteredNails; ++index)
        d3t::RetireObjectPlacement(&patiNailNodes[index]);
    patiRegisteredNails = 0;
}
// @verified - p
void ResetPatiNailModelState() {
    patiHiddenNailNodes[0] = nullptr;
    patiHiddenNailNodes[1] = nullptr;
}
// @verified - p
void AttachPatiNails(PatiEventWork *work, shen_name board, const char *nailFile) {
    RestorePatiNailModels();
    Character *character = FindCharacterByScenarioName(board);
    MT7Model &model = *static_cast<MT7Model *>(character->model);
    MT7Node *transform = model.nodes[0].runtimeChild;
    ModelCloneNode *sourceRoot = reinterpret_cast<ModelCloneNode *>(const_cast<std::byte *>(model.source.data()) + model.nodes[0].sourceOffset);
    ModelCloneNode *sourceTransform = static_cast<ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(sourceRoot->childOffset));
    for (std::uint32_t index = 0; index < 512; ++index) {
        work->nailPlacements[index] = {.signature = 152,
            .position = {},
            .rotation = {},
            .scale = {1.0f, 1.0f, 1.0f},
            .meshOffset = 0,
            .childOffset = 0,
            .siblingOffset = 0,
            .parentOffset = 0,
            .transform = nullptr};
        MT7Node &node = patiNailNodes[index];
        node.id = 152;
        node.position = {};
        node.rotation = {};
        node.scale = {1.0f, 1.0f, 1.0f};
        node.xb01Offset = 0;
        node.runtimeMesh = nullptr;
        node.runtimeChild = nullptr;
        node.runtimeSibling = nullptr;
        node.runtimeParent = nullptr;
        node.borrowedMesh = nullptr;
        node.commandOverride = nullptr;
        node.transformOverride = nullptr;
        node.taskData = nullptr;
        node.sourceOffset = index * sizeof(PatiPlacementNode);
        node.visible = true;
    }
    patiHiddenNailNodes[0] = transform->runtimeChild;
    patiHiddenNailNodes[1] = patiHiddenNailNodes[0]->runtimeSibling;
    patiHiddenNailRecords[0] = static_cast<ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(sourceTransform->childOffset));
    patiHiddenNailRecords[1] = static_cast<ModelCloneNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(patiHiddenNailRecords[0]->siblingOffset));
    for (std::uint32_t index = 0; index < 2; ++index) {
        if (!patiHiddenNailNodes[index])
            continue;
        patiSavedNailMeshOffsets[index] = patiHiddenNailRecords[index]->meshOffset;
        patiSavedNailXb01Offsets[index] = patiHiddenNailNodes[index]->xb01Offset;
        patiSavedNailMeshes[index] = patiHiddenNailNodes[index]->runtimeMesh;
        patiHiddenNailRecords[index]->meshOffset = 0;
        patiHiddenNailNodes[index]->xb01Offset = 0;
        patiHiddenNailNodes[index]->runtimeMesh = nullptr;
    }
    work->nailPlacements[0].parentOffset = HLib::GetDynamicHeapOffset(sourceTransform);
    patiNailNodes[0].runtimeParent = transform;
    patiHiddenNailRecords[1]->siblingOffset = HLib::GetDynamicHeapOffset(work->nailPlacements);
    patiHiddenNailNodes[1]->runtimeSibling = patiNailNodes;
    for (std::uint32_t index = 1; index < 512; ++index) {
        work->nailPlacements[index].parentOffset = HLib::GetDynamicHeapOffset(&work->nailPlacements[index - 1]);
        work->nailPlacements[index - 1].siblingOffset = HLib::GetDynamicHeapOffset(&work->nailPlacements[index]);
        patiNailNodes[index].runtimeParent = &patiNailNodes[index - 1];
        patiNailNodes[index - 1].runtimeSibling = &patiNailNodes[index];
    }
    MT7Model *pinModel = static_cast<MT7Model *>(FindCharacterByScenarioName(PATI_PIN)->model);
    const MT7Node &pin = pinModel->nodes[0];
    const ModelCloneNode *pinSource = reinterpret_cast<const ModelCloneNode *>(pinModel->source.data() + pin.sourceOffset);
    EV_DisableCharacterDisplay(PATI_PIN);
    for (PatiNailNode &nail : work->game.nails.nails) {
        nail.position = {0.0f, -1.0f, 0.0f};
        nail.rotation = {};
        nail.scale = {1.0f, 1.0f, 1.0f};
    }
    ParsePatiNailFile(&work->game.nails, nailFile);
    std::uint32_t used = 0;
    for (PatiNailNode &nail : work->game.nails.nails) {
        if (nail.position.y > 0.0f) {
            PatiPlacementNode &placement = work->nailPlacements[used];
            placement.meshOffset = pinSource->meshOffset;
            placement.position = nail.position;
            placement.scale = nail.scale;
            MT7Node &node = patiNailNodes[used];
            node.position = nail.position;
            node.scale = nail.scale;
            mt7_CopyXB01Offset(&node, &pin);
            patiNailCommands[used] = {.nodeSourceOffset = node.sourceOffset,
                .bytes = pinModel->source,
                .xb01Offset = pin.xb01Offset,
                .resources = &(pin.borrowedMesh ? pin.borrowedMesh : &pin)->drawResources};
            node.commandOverride = &patiNailCommands[used];
            d3t::RegisterObjectPlacement(GetMT7ReaderData(pinModel), &node, 0);
            ++used;
        }
        nail.scale.x = work->scale * 1.5f;
        nail.scale.y = work->scale * 1.5f;
        nail.scale.z = work->scale;
        nail.position.x = work->scale * nail.position.x;
        nail.position.y = nail.position.y * work->scale;
        nail.position.z = work->scale * nail.position.z;
    }
    work->nailPlacements[used - 1].siblingOffset = 0;
    patiNailNodes[used - 1].runtimeSibling = nullptr;
    patiRegisteredNails = used;
}
} // namespace shenmue::s2
