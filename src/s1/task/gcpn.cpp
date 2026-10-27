#include "s1/task/gcpn.hpp"

#include "s1/file_loader.hpp"
#include "s1/hlib.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/scen.hpp"

#include <cmath>
#include <cstdio>

namespace shenmue::s1 {

extern void *g_shadowMeshData;

namespace {

constexpr std::uint32_t kGcpnToken = SHEN_NAME(GCPN);

const char *g_modelTable;

const char *ModelTableEntry(std::int32_t requestedIndex) {
    const char *cursor = g_modelTable;
    std::uint32_t remaining = static_cast<std::uint32_t>(requestedIndex);
    while (*cursor) {
        --remaining;
        const char *const selected = static_cast<std::int32_t>(remaining) < 0 ? cursor : nullptr;
        do {
            ++cursor;
        } while (*cursor);
        ++cursor;
        if (selected)
            return selected;
    }
    return nullptr;
}

PackModel *LoadGachaModel(std::int32_t requestedIndex) {
    const char *const encoded = ModelTableEntry(requestedIndex);
    if (!encoded)
        return nullptr;

    char directory[64];
    const char *modelName = encoded;
    switch (*modelName) {
    case '#':
        ++modelName;
        std::sprintf(directory, "scene/%02d/model/item", g_sceneId);
        break;
    case '$':
        ++modelName;
        std::sprintf(directory, "scene/%02d/%c%c%c%c", g_sceneId, static_cast<char>(g_areaId), static_cast<char>(g_areaId >> 8U),
            static_cast<char>(g_areaId >> 16U), static_cast<char>(g_areaId >> 24U));
        break;
    case '%':
        ++modelName;
        std::sprintf(directory, "model/item");
        break;
    case '@':
        ++modelName;
        std::sprintf(directory, "scene/%02d/model/chara", g_sceneId);
        break;
    default:
        std::sprintf(directory, "model/chara");
        break;
    }
    void *const file = ReadFile(directory, modelName, SHEN_NAME(GCPN));
    if (!file)
        return nullptr;
    char formattedPath[260]{};
    std::snprintf(formattedPath, sizeof(formattedPath), "%s/%s_/%s", directory, modelName, modelName);
    PackModel *const model = MT5_InitRawModelAndTextures(file, modelName, formattedPath);
    if (!model) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
        return nullptr;
    }
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), formattedPath, 0, 2);
    return model;
}

MapControlData *shenGetMapCtrl() {
    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
}

Character *scenFindChara(ScenarioContext *context, std::uint32_t id) {
    return SCEN_FindCharacterByFunctionId(context, id);
}

void GCPN_Destroy(Character *character, void *payload) {
    auto *const work = static_cast<GCPNWork *>(payload);
    character->model = nullptr;
    character->renderOpacity = 0.0f;
    if (work->modelOwner) {
        FreePVR(work->modelOwner);
        work->modelOwner = nullptr;
    }
}

float SmoothedInputDelta(float current, float target) {
    float delta = target - current;
    if (delta > 3.0f || delta < -3.0f)
        return static_cast<float>(static_cast<double>(delta) * 0.03333);
    if (delta > 0.1f)
        delta = 0.1f;
    return delta >= -0.1f ? delta : -0.1f;
}

void ApplyCamera(Character *character, GCPNWork &work) {
    const Vector3 actorPosition = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    const float horizontal = static_cast<float>(static_cast<double>(work.horizontalOffset) * 0.000390625);
    const float vertical = static_cast<float>(static_cast<double>(work.verticalOffset) * 0.000390625);

    const Vector3 blended{static_cast<float>(static_cast<double>(actorPosition.x - work.actorPosition.x) * 0.7 + static_cast<double>(work.actorPosition.x)),
        static_cast<float>(static_cast<double>(actorPosition.y - work.actorPosition.y) * 0.7 + static_cast<double>(work.actorPosition.y)),
        static_cast<float>(static_cast<double>(actorPosition.z - work.actorPosition.z) * 0.7 + static_cast<double>(work.actorPosition.z))};
    const Vector3 target{blended.x + (work.directionZ * horizontal - work.directionX * vertical), blended.y + -vertical,
        blended.z + (work.directionX * horizontal + work.directionZ * vertical)};
    const float halfHorizontal = horizontal * 0.5f;
    const float halfVertical = vertical * 0.5f;
    const Vector3 eyeBase{(work.directionZ * halfHorizontal + work.cameraTarget.x) - work.directionX * halfVertical, -halfVertical + work.cameraTarget.y,
        (work.directionX * halfHorizontal + work.cameraTarget.z) + work.directionZ * halfVertical};

    const std::int32_t fov = static_cast<std::int32_t>((static_cast<double>(work.fovOffset) * 0.0078125 + 1.0) * 10922.0);
    work.noisePhaseX = static_cast<std::uint16_t>(work.noisePhaseX + 728U);
    const float eyeX = static_cast<float>(static_cast<double>(::shenmue::sin_fixed16(static_cast<std::int16_t>(work.noisePhaseX))) * 0.005 + eyeBase.x);
    work.noisePhaseY = static_cast<std::uint16_t>(work.noisePhaseY + 546U);
    const float eyeY = static_cast<float>(static_cast<double>(::shenmue::sin_fixed16(static_cast<std::int16_t>(work.noisePhaseY))) * 0.005 + eyeBase.y);
    work.noisePhaseZ = static_cast<std::uint16_t>(work.noisePhaseZ + 364U);
    const float eyeZ = static_cast<float>(static_cast<double>(::shenmue::sin_fixed16(static_cast<std::int16_t>(work.noisePhaseZ))) * 0.005 + eyeBase.z);
    const Vector3 eye{eyeX, eyeY, eyeZ};

    TranslateCameraRelative(eye);
    if (!bLockCamera) {
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, target);
    }

    CAME_SetFieldOfView(fov);
}

void GCPN_Update(Character *character, void *payload) {
    auto *const work = static_cast<GCPNWork *>(payload);
    if (work->cameraActive == 0)
        return;

    const auto *const input = static_cast<const PadCaptureEntry *>(shenGetMapCtrl()->padCaptureEntry);
    const std::uint8_t stickX = static_cast<std::uint8_t>(input->leftStick);
    const std::uint8_t stickY = static_cast<std::uint8_t>(input->leftStick >> 8U);
    const std::uint8_t triggerLow = static_cast<std::uint8_t>(input->triggers);
    const std::uint8_t triggerHigh = static_cast<std::uint8_t>(input->triggers >> 8U);

    work->horizontalOffset += SmoothedInputDelta(work->horizontalOffset, static_cast<float>(stickX) - 128.0f);
    work->verticalOffset += SmoothedInputDelta(work->verticalOffset, static_cast<float>(stickY) - 128.0f);
    const float targetFovOffset = static_cast<float>(static_cast<std::int32_t>(triggerHigh) - static_cast<std::int32_t>(triggerLow) - 256) * 0.125f;
    work->fovOffset += SmoothedInputDelta(work->fovOffset, targetFovOffset);
    ApplyCamera(character, *work);
}

} // namespace

GCPNWork *GCPN_Find(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kGcpnToken);
    return component ? reinterpret_cast<GCPNWork *>(component + 1) : nullptr;
}

std::int32_t GCPN_ExecuteScript(ScenarioContext *context, const GCPNScriptParameters &parameters) {
    auto &mutableParameters = const_cast<GCPNScriptParameters &>(parameters);
    const std::int32_t originalMode = mutableParameters.mode;
    if (mutableParameters.mode == 5)
        mutableParameters.mode = 1;

    switch (mutableParameters.mode) {
    case 0:
        g_modelTable = mutableParameters.characterOrTableOffset != 0 ?
            reinterpret_cast<const char *>(g_pCurrentTaskData + mutableParameters.characterOrTableOffset) :
            nullptr;
        return 0;

    case 1: {
        if (!g_modelTable || scenFindChara(context, static_cast<std::uint32_t>(mutableParameters.characterOrTableOffset)))
            return -1;

        Character *const character = AddCharacterHandler(CharacterHandlerPriority::Low);
        if (!character)
            return -1;
        CHID_Init(shenGetMapCtrl()->scenarioTask, static_cast<std::uint32_t>(mutableParameters.characterOrTableOffset), character);
        AllocateFIXO(character);
        auto *const work = static_cast<GCPNWork *>(AllocateHeapMemoryMODL(character, kGcpnToken, sizeof(GCPNWork), GCPN_Update));
        HLib_GetCharacterComponent(character, kGcpnToken)->destroyCallback = GCPN_Destroy;

        if (work) {
            if (originalMode == 5) {
                AllocREFB(character, 6);
                character->flags.value &= ~(CharacterFlagActive | 0x10U);
                character->shadowModel = g_shadowMeshData;
            }
            work->modelOwner = LoadGachaModel(mutableParameters.modelIndexOrVectorOffset);
            if (work->modelOwner)
                character->model = work->modelOwner->model;
            character->renderOpacity = 1.0f;
            work->cameraActive = 0;
        }
        return 0;
    }

    case 2: {
        Character *const character = scenFindChara(context, static_cast<std::uint32_t>(mutableParameters.characterOrTableOffset));
        if (!character || !GCPN_Find(character))
            return -1;
        character->flags.value &= ~CharacterFlagActive;
        ++shenGetMapCtrl()->pendingCharacterRetirements;
        return 0;
    }

    case 3: {
        if (mutableParameters.characterOrTableOffset == 0)
            return -1;
        Character *const character = scenFindChara(context, static_cast<std::uint32_t>(mutableParameters.characterOrTableOffset));
        if (!character)
            return -1;
        GCPNWork *const work = GCPN_Find(character);
        if (!work)
            return -1;
        const auto *const authoredOffset = mutableParameters.modelIndexOrVectorOffset != 0 ?
            reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + mutableParameters.modelIndexOrVectorOffset) :
            nullptr;

        work->actorPosition = GetPositionForEntity(character->entity.node);
        work->cameraTarget = TransformPointByEntity(character->entity.node, *authoredOffset);
        const float dx = work->actorPosition.x - work->cameraTarget.x;
        const float dz = work->actorPosition.z - work->cameraTarget.z;
        const float distance = std::sqrt(dx * dx + dz * dz);

        work->horizontalOffset = 0.0f;
        work->verticalOffset = 0.0f;
        work->directionX = dx / distance;
        work->directionZ = -(dz / distance);
        work->fovOffset = -32.0f;
        work->reserved34 = 0;
        work->reserved38 = 0;
        work->noisePhaseX = 0;
        work->noisePhaseY = 0;
        work->noisePhaseZ = 0;

        g_temporaryGameState = 0;
        ApplyPendingGameState(0);
        SetNewAuthorGameState(9);
        ApplyCamera(character, *work);
        work->cameraActive = 1;
        return 0;
    }

    case 4: {
        Character *const character = scenFindChara(context, static_cast<std::uint32_t>(mutableParameters.characterOrTableOffset));
        GCPNWork *const work = GCPN_Find(character);
        if (!work)
            return -1;
        work->cameraActive = 0;
        return 0;
    }

    default:
        return 0;
    }
}

} // namespace shenmue::s1
