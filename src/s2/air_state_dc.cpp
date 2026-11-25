#include "s2/air_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/file_loader_dc.hpp"
#include "s2/player_held_air_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/texture_cache_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/texture_dc.hpp"
#endif
#include <cstdio>
#include "common/matrix_stack.hpp"
#include "platform/dc/mt7.hpp"
#include "platform/dc/ninja.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/date_time.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/map_info.hpp"
#include <cstring>
#include "s2/cold_archive_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/scene_data.hpp"

namespace shenmue {
namespace s2 {

void DrawAir(void *);
MT7Node *GetAirDomeModelRoot(std::int32_t index);
MT7Node *GetAirCloudModelRoot(std::int32_t index);
MT7Node *GetAirPlanetModelRoot(std::int32_t index);
void LoadAirModels();
void LoadAirMapTexture(MapInfoChunk *mapInfo);
std::int32_t ReadAirScrollMode(const MapInfoChunk *chunk);
void DrawAirNode(MT7Node *node, float alpha, std::int32_t layer, std::int32_t cloud);
void UpdateAirCloudColours(std::int32_t index);
void UpdateAirDomeColours();
void LoadAirTextures(std::int32_t texture, float scale, std::int32_t layer);
void AdvanceAirTextureTransition();
void CancelAirTextureTransition();
void UpdateAirSeconds();
std::uint32_t GetWeatherCloudOrSkyColour(std::int32_t channel);
void UpdateAirDomeBlend();
void UpdateAirTimeOfDay();
float GetAirTimeOfDay();
void UpdateCelestialPosition();
void UpdateCelestialCoordinates();

void FindAirMeshPackets(MT7Node *node, void *output);
MT7Model *ReadAirCloudModel(void *source, std::int32_t *textures);
void UpdateAirMeshPackets();
void ScrollAirCloudPackets(AirCloudLayer *layer, std::int32_t mode, float u, float v);
AirCloudLayer g_airCloudLayers[2];
AirDomeLayer g_airDomeLayers[2];
AirDrawState g_airDrawState;

AirPlanetDrawRecord g_airPlanet[1] = {{SHEN_FIELD(position){}, SHEN_FIELD(orbitInput){}, SHEN_FIELD(rotation){}, SHEN_FIELD(intermediateRotation){},
    SHEN_FIELD(tilt) 0, SHEN_FIELD(timeRotation){}, SHEN_FIELD(scale){}, SHEN_FIELD(intensity) 0.0f}};

void PublishGlobalLighting();
std::byte *g_armMotionMemory = 0;

// @verified - d
void AllocateARMM() {
    g_armMotionMemory = static_cast<std::byte *>(HLib::AllocateTaskData(kArmMotionMemoryBytes, ARM_MOTION_STORAGE));
    HLib::SetTaskDataMotionFlag(g_armMotionMemory);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InitializeAirLayers() {
    std::int32_t index;
    AirCloudLayer *cloud = g_airCloudLayers;
    for (index = 0; index < 2; ++index, ++cloud) {
        cloud->packets = 0;
        cloud->packetEnd = 0;
        cloud->header = 0;
        cloud->model = 0;
        cloud->scroll.x = 0.0f;
        cloud->scroll.y = 0.0f;
        cloud->retainedScroll.x = 0.0f;
        cloud->retainedScroll.y = 0.0f;
        cloud->scrollRate.x = 0.0005f;
        cloud->scrollRate.y = 0.0005f;
        cloud->firstColour = UINT32_MAX;
        cloud->secondColour = UINT32_MAX;
        for (std::int32_t level = 0; level < 8; ++level) {
            cloud->colours[level][0] = UINT16_MAX;
            cloud->colours[level][1] = UINT16_MAX;
        }
        if (index == 0)
            cloud->blend = 1.0f;
        else
            cloud->blend = 0.0f;
        cloud->blendStep = 0.0f;
        cloud->blendScale = 1.0f;
        cloud->textureHandle = -1;
        cloud->textureIndex = -1;
        cloud->offset.x = 0.0f;
        cloud->offset.y = -115.0f;
        cloud->offset.z = 0.0f;
    }
    AirDomeLayer *dome = g_airDomeLayers;
    for (index = 0; index < 2; ++index, ++dome) {
        dome->packets = 0;
        dome->packetEnd = 0;
        dome->header = 0;
        if (index == 0) {
            dome->firstColour = 0xff204cf8U;
            dome->secondColour = 0xffacecffU;
        } else {
            dome->firstColour = UINT32_MAX;
            dome->secondColour = UINT32_MAX;
        }
        for (std::int32_t level = 0; level < 5; ++level) {
            dome->colours[level][0] = UINT16_MAX;
            dome->colours[level][1] = UINT16_MAX;
        }
        if (index == 0)
            dome->blend = 1.0f;
        else
            dome->blend = 0.0f;
    }
    AirPlanetDrawRecord *planet = g_airPlanet;
    for (index = 0; index < 1; ++index, ++planet) {
        std::int32_t axis = 0;
        char *const base = reinterpret_cast<char *>(planet);
        char *const orbitInput = base + offsetof(AirPlanetDrawRecord, orbitInput);
        char *const rotation = base + offsetof(AirPlanetDrawRecord, rotation);
        char *const intermediateRotation = base + offsetof(AirPlanetDrawRecord, intermediateRotation);
        char *const tilt = base + offsetof(AirPlanetDrawRecord, tilt);
        char *const scale = base + offsetof(AirPlanetDrawRecord, scale);
        for (; axis < 3; ++axis) {
            const std::size_t component = axis * sizeof(float);
            reinterpret_cast<float *>(planet)[axis] = 0.0f;
            *reinterpret_cast<float *>(orbitInput + component) = 0.0f;
            *reinterpret_cast<std::int32_t *>(rotation + component) = 0;
            *reinterpret_cast<std::int32_t *>(intermediateRotation + component) = 0;
            *reinterpret_cast<std::int32_t *>(tilt + component) = 0;
            *reinterpret_cast<float *>(scale + component) = 20.0f;
        }
        planet->orbitInput.x = 3200.0f;
        planet->timeRotation.z = 32768;
        planet->tilt = 6144;
        planet->intensity = 0.40000001f;
    }
}
#else
// @verified - d
void InitializeAirLayers() {
    std::int32_t index;
    AirCloudLayer *cloud = g_airCloudLayers;
    for (index = 0; index < 2; ++index, ++cloud) {
        cloud->packets = 0;
        cloud->packetEnd = 0;
        cloud->header = 0;
        cloud->model = 0;
        cloud->scroll.x = 0.0f;
        cloud->scroll.y = 0.0f;
        cloud->retainedScroll.x = 0.0f;
        cloud->retainedScroll.y = 0.0f;
        cloud->scrollRate.x = 0.0005f;
        cloud->scrollRate.y = 0.0005f;
        cloud->firstColour = UINT32_MAX;
        cloud->secondColour = UINT32_MAX;
        for (std::int32_t level = 0; level < 8; ++level) {
            cloud->colours[level][0] = UINT16_MAX;
            cloud->colours[level][1] = UINT16_MAX;
        }
        if (index == 0)
            cloud->blend = 1.0f;
        else
            cloud->blend = 0.0f;
        cloud->blendStep = 0.0f;
        cloud->blendScale = 1.0f;
        cloud->textureHandle = -1;
        cloud->textureIndex = -1;
        cloud->offset.x = 0.0f;
        cloud->offset.y = -115.0f;
        cloud->offset.z = 0.0f;
    }
    AirDomeLayer *dome = g_airDomeLayers;
    for (index = 0; index < 2; ++index, ++dome) {
        dome->packets = 0;
        dome->packetEnd = 0;
        dome->header = 0;
        if (index == 0) {
            dome->firstColour = 0xff204cf8U;
            dome->secondColour = 0xffacecffU;
        } else {
            dome->firstColour = UINT32_MAX;
            dome->secondColour = UINT32_MAX;
        }
        for (std::int32_t level = 0; level < 5; ++level) {
            dome->colours[level][0] = UINT16_MAX;
            dome->colours[level][1] = UINT16_MAX;
        }
        if (index == 0)
            dome->blend = 1.0f;
        else
            dome->blend = 0.0f;
    }
    AirPlanetDrawRecord *planet = g_airPlanet;
    for (index = 0; index < 1; ++index, ++planet) {
        char *const base = reinterpret_cast<char *>(planet);
        char *const position = base + offsetof(AirPlanetDrawRecord, position);
        char *const orbitInput = base + offsetof(AirPlanetDrawRecord, orbitInput);
        char *const rotation = base + offsetof(AirPlanetDrawRecord, rotation);
        char *const intermediateRotation = base + offsetof(AirPlanetDrawRecord, intermediateRotation);
        char *const tilt = base + offsetof(AirPlanetDrawRecord, tilt);
        char *const scale = base + offsetof(AirPlanetDrawRecord, scale);
        for (std::int32_t axis = 0; axis < 3; ++axis) {
            const std::size_t component = axis * sizeof(float);
            *reinterpret_cast<float *>(position + component) = 0.0f;
            *reinterpret_cast<float *>(orbitInput + component) = 0.0f;
            *reinterpret_cast<std::int32_t *>(rotation + component) = 0;
            *reinterpret_cast<std::int32_t *>(intermediateRotation + component) = 0;
            *reinterpret_cast<std::int32_t *>(tilt + component) = 0;
            *reinterpret_cast<float *>(scale + component) = 20.0f;
        }
        planet->orbitInput.x = 3200.0f;
        planet->timeRotation.z = 32768;
        planet->tilt = 6144;
        planet->intensity = 0.40000001f;
    }
}
#endif

// @verified - d
void SwapAirCloudLayers() {
    const AirCloudLayer temporary = g_airCloudLayers[0];
    g_airCloudLayers[0] = g_airCloudLayers[1];
    g_airCloudLayers[1] = temporary;
}

void SetAirTexturePacketMode(std::uint32_t *packet, std::int32_t mode, std::uint32_t value) {
    switch (mode) {
    case 0:
        packet[3] = (packet[3] & 0xc7ffffffU) | value;
        break;
    case 1:
        packet[2] = (packet[2] & 0xffffU) | (value != 0 ? 0x94000000U : 0x20800000U);
        break;
    }
}

// @verified - d
std::int32_t ReloadAirTexture(std::int32_t texture, const std::byte *source) {
    const std::uint32_t *header = reinterpret_cast<const std::uint32_t *>(source + 0x10);
    if (*header == SHEN_NAME(GBIX))
        header += 3;
    const std::int16_t *const dimensions = reinterpret_cast<const std::int16_t *>(header + 3);
    const std::int32_t bytes = dimensions[1] * dimensions[0] * 2;
    dc::sdk::PurgeDataCache(reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(header + 4) & ~static_cast<std::uintptr_t>(31)), bytes / 32 + 1);
    ReloadTextureCacheSurface(g_textureIndexMap[texture].surface, source + 0x2c);
    return texture;
}

void SetAirLayerTexturePacketMode(AirCloudLayer *layer, std::int32_t mode, std::uint32_t value) {
    SetAirTexturePacketMode(layer->header, mode, value);
}

// @verified - d
void ResetAirDrawState() {
    g_airDrawState.domes[0] = 0;
    g_airDrawState.domes[1] = 0;
    g_airDrawState.planetState = 0;
    g_airDrawState.planet = 0;
    g_airDrawState.transitionActive = 0;
    g_airDrawState.transitionFrames = 0;
    g_airDrawState.clearColour = 0;
    g_airDrawState.drawEnabled = 1;
    g_airDrawState.timeOfDay = 0.0f;
    g_airDrawState.previousSeconds = 0;
    g_airDrawState.currentSeconds = 0;
}

// @verified - d
void InitializeAirPresentation(const void *mapInfo) {
    InitializeAirLayers();
    ResetAirDrawState();
    LoadAirModels();
    LoadAirMapTexture(static_cast<MapInfoChunk *>(const_cast<void *>(mapInfo)));
}

// @verified - d
void AirDrawCallback(void *parameter) {
    static_cast<AirDrawTaskParameters *>(parameter)->mode = 0;
    UpdateAirTimeOfDay();
    UpdateAirSeconds();
    UpdateCelestialPosition();
    UpdateAirDomeBlend();
    AdvanceAirTextureTransition();
    UpdateAirCloudColours(0);
    UpdateAirCloudColours(1);
    UpdateAirDomeColours();
    UpdateAirMeshPackets();
    if (g_airDrawState.drawEnabled)
        HLib::ExecuteCallbackNow(DrawAir, 0);
    const std::uint32_t colour = g_airDrawState.drawEnabled ? GetWeatherCloudOrSkyColour(1) : g_airDrawState.clearColour;
    dc::sdk::njSetBackColor(colour, colour, colour);
}

extern const float g_airDomeHeights[2];
const float g_airDomeHeights[2] = {115.0f, -205.0f};

// @verified - d
void DrawAir(void *) {
    const std::int32_t vertexColourType = dc::sdk::GetMT7VertexColourType();
    dc::sdk::SetMT7VertexColourType(0);
    std::int32_t enabled[16];
    std::int32_t index;
    for (index = 0; index < 16; ++index) {
        enabled[index] = static_cast<std::int16_t>(dc::sdk::g_lightRecords[index][0]);
        dc::sdk::SetLightEnabled(index, 0);
    }
    PublishGlobalLighting();
    for (index = 0; index < 2; ++index) {
        if (g_airDomeLayers[index].blend > 0.0f) {
            matrix_stack::Matrix4x4_Push(0);
            Matrix4x4::preTranslate(0.0f, g_airDomeHeights[index], 0.0f);
            Matrix4x4::Scale(1.3f, 1.0f, 1.3f);
            DrawAirNode(GetAirDomeModelRoot(index), g_airDomeLayers[index].blend, index, 0);
            PopCurrentMatrix(1);
        }
    }
    AirCloudLayer *layer = g_airCloudLayers;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::TranslateVector(&layer->offset);
    Matrix4x4::Scale(1.3f, 2.6f, 1.3f);
    for (index = 0; index < 2; ++index, ++layer) {
        if (layer->blend > 0.0f) {
            matrix_stack::Matrix4x4_Push(0);
            dc::sdk::SetMT7PolygonFogMode(2);
            DrawAirNode(GetAirCloudModelRoot(index), layer->blend * layer->blendScale, index, 1);
            dc::sdk::SetMT7PolygonFogMode(0);
            PopCurrentMatrix(1);
        }
    }
    PopCurrentMatrix(1);
    AirPlanetDrawRecord *planet = g_airPlanet;
    for (index = 0; index < 1; ++index, ++planet) {
        if (planet->intensity > 0.0f) {
            matrix_stack::Matrix4x4_Push(0);
            Matrix4x4::preTranslate(planet->position.x, planet->position.y, planet->position.z);
            Matrix4x4::RotateAroundY_Wrapper(planet->rotation.y);
            Matrix4x4::RotateAroundX_Wrapper(planet->rotation.x);
            Matrix4x4::Scale(planet->scale.x, planet->scale.y, planet->scale.z);
            dc::sdk::SetModelScale(planet->scale.x);
            DrawAirNode(GetAirPlanetModelRoot(index), planet->intensity, 0, 0);
            dc::sdk::SetModelScale(1.0f);
            PopCurrentMatrix(1);
        }
    }
    for (index = 0; index < 16; ++index)
        dc::sdk::SetLightEnabled(index, enabled[index]);
    PublishGlobalLighting();
    dc::sdk::SetMT7VertexColourType(vertexColourType);
}

// @verified - d
HLib::Task *Queue_CtrlAir(std::uint8_t index) {
    const std::int8_t slot = index;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AirDrawCallback, slot, 6, sizeof(AirDrawTaskParameters), AIR_DRAW_TASK);
    static_cast<AirDrawTaskParameters *>(HLib::GetTaskParameter(task))->mode = 0;
    return task;
}

// @verified - d
void InitializeAirModels(void *firstDome, void *secondDome, void *firstCloud, void *secondCloud) {
    g_airDrawState.domes[0] = InitializeMT7Model(firstDome);
    g_airDrawState.domes[1] = InitializeMT7Model(secondDome);
    g_airCloudLayers[0].model = ReadAirCloudModel(firstCloud, &g_airCloudLayers[0].textureHandle);
    g_airCloudLayers[1].model = ReadAirCloudModel(secondCloud, &g_airCloudLayers[1].textureHandle);
    AirCloudLayer *cloud = g_airCloudLayers;
    for (std::int32_t index = 0; index < 2; ++index, ++cloud)
        FindAirMeshPackets(GetAirCloudModelRoot(index), cloud);
    AirDomeLayer *dome = g_airDomeLayers;
    for (std::int32_t index = 0; index < 2; ++index, ++dome)
        FindAirMeshPackets(GetAirDomeModelRoot(index), dome);
}

// @verified - d
void InitializeAirPlanetModel(void *source, std::int32_t) {
    g_airDrawState.planet = InitializeMT7Model(source);
    g_airDrawState.planetState = 0;
}

MT7Node *GetAirDomeModelRoot(std::int32_t index) {
    return index ? g_airDrawState.domes[1]->root : g_airDrawState.domes[0]->root;
}

MT7Node *GetAirCloudModelRoot(std::int32_t index) {
    return g_airCloudLayers[index].model->root;
}

MT7Node *GetAirPlanetModelRoot(std::int32_t index) {
    if (index == 0)
        return g_airDrawState.planet->root;
    return 0;
}

// @verified - d
void LoadAirModels() {
    MapInfoChunk *const source = static_cast<MapInfoChunk *>(shenLoadFile("misc", "DOOM.BIN", AIR_MODEL_STORAGE));
    MapInfoChunk *const firstDome = FindNativeChunk("DOM0", source) + 1;
    MapInfoChunk *const secondDome = FindNativeChunk("DOM1", source) + 1;
    MapInfoChunk *const firstCloud = FindNativeChunk("AIR0", source) + 1;
    MapInfoChunk *secondCloud = FindNativeChunk("AIR1", source);
    ++secondCloud;
    InitializeAirModels(firstDome, secondDome, firstCloud, secondCloud);
    InitializeAirPlanetModel(FindNativeChunk("MOON", source) + 1, 0);
}

// @verified - d
void LoadAirMapTexture(MapInfoChunk *mapInfo) {
    const std::int32_t mode = ReadAirScrollMode(FindNativeChunk("SCRL", mapInfo));
    if (mode == -1)
        g_airDrawState.drawEnabled = 0;
    else if (mode != -2)
        LoadAirTextures(mode, 1.0f, 0);
}

// @verified - d
std::int32_t ReadAirScrollMode(const MapInfoChunk *chunk) {
    std::int32_t mode = -2;
    if (chunk) {
        mode = *reinterpret_cast<const std::int32_t *>(chunk + 1);
        if (mode <= -2)
            mode = -2;
    }
    return mode;
}

// @verified - d
void DrawAirNode(MT7Node *node, float alpha, std::int32_t layer, std::int32_t cloud) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (layer) {
        float offset = -0.01f;
        if (cloud)
            offset = 0.01f;
        Matrix4x4::preTranslate(node->position.x, node->position.y + offset, node->position.z);
    }

    if (node->mesh) {
        if (alpha < 1.0f)
            dc::sdk::DrawSingleLightMT7MeshAlpha(node->mesh, alpha);
        else
            dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
    }
    if (node->child)
        DrawAirNode(node->child, alpha, layer, cloud);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawAirNode(node->sibling, alpha, layer, cloud);
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void UpdateAirCloudColours(std::int32_t index) {
    float firstAlpha, firstRed, firstGreen, firstBlue, secondAlpha, secondRed, secondGreen, secondBlue, stepAlpha, stepRed, stepGreen, stepBlue;
    AirCloudLayer *layer = &g_airCloudLayers[index];
    firstAlpha = static_cast<float>(layer->firstColour >> 24);
    firstRed = static_cast<float>(layer->firstColour >> 16 & 0xffU);
    firstGreen = static_cast<float>(layer->firstColour >> 8 & 0xffU);
    firstBlue = static_cast<float>(layer->firstColour & 0xffU);
    secondAlpha = static_cast<float>(layer->secondColour >> 24);
    secondRed = static_cast<float>(layer->secondColour >> 16 & 0xffU);
    secondGreen = static_cast<float>(layer->secondColour >> 8 & 0xffU);
    secondBlue = static_cast<float>(layer->secondColour & 0xffU);
    stepAlpha = secondAlpha - firstAlpha;
    stepAlpha /= 10.0f;
    stepRed = secondRed - firstRed;
    stepRed /= 10.0f;
    stepGreen = secondGreen - firstGreen;
    stepGreen /= 10.0f;
    stepBlue = secondBlue - firstBlue;
    stepBlue /= 10.0f;
    const float *levels = airCloudColourLevels;
    for (std::int32_t level = 0; level < 8; ++level) {
        float factor;
        layer->colours[level][0] =
            (factor = *levels++, (static_cast<std::int32_t>(firstAlpha + stepAlpha * factor) << 8) | static_cast<std::int32_t>(firstRed + stepRed * factor));
        layer->colours[level][1] = (static_cast<std::int32_t>(firstGreen + stepGreen * factor) << 8) | static_cast<std::int32_t>(firstBlue + stepBlue * factor);
    }
}
#else
// @verified - d
void UpdateAirCloudColours(std::int32_t index) {
    AirCloudLayer &layer = g_airCloudLayers[index];
    const float first[4] = {static_cast<float>(layer.firstColour >> 24), static_cast<float>(layer.firstColour >> 16 & 0xffU),
        static_cast<float>(layer.firstColour >> 8 & 0xffU), static_cast<float>(layer.firstColour & 0xffU)};
    const float second[4] = {static_cast<float>(layer.secondColour >> 24), static_cast<float>(layer.secondColour >> 16 & 0xffU),
        static_cast<float>(layer.secondColour >> 8 & 0xffU), static_cast<float>(layer.secondColour & 0xffU)};
    float differences[4];
    for (std::int32_t channel = 0; channel < 4; ++channel)
        differences[channel] = (second[channel] - first[channel]) / 10.0f;
    for (std::int32_t level = 0; level < 8; ++level) {
        std::uint32_t channels[4];
        for (std::int32_t channel = 0; channel < 4; ++channel) {
            float value = first[channel];
            asm("fmac fr0,%1,%0" : "+f"(value) : "f"(differences[channel]), "w"(airCloudColourLevels[level]));
            asm("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(channels[channel]) : "f"(value) : "fpul");
        }
        layer.colours[level][0] = static_cast<std::uint16_t>(channels[0] << 8U | static_cast<std::uint16_t>(channels[1]));
        layer.colours[level][1] = static_cast<std::uint16_t>(channels[2] << 8U | static_cast<std::uint16_t>(channels[3]));
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void UpdateAirDomeColours() {
    float firstAlpha, firstRed, firstGreen, firstBlue, secondAlpha, secondRed, secondGreen, secondBlue, stepAlpha, stepRed, stepGreen, stepBlue;
    AirDomeLayer *layer = &g_airDomeLayers[0];
    firstAlpha = static_cast<float>(layer->firstColour >> 24);
    firstRed = static_cast<float>(layer->firstColour >> 16 & 0xffU);
    firstGreen = static_cast<float>(layer->firstColour >> 8 & 0xffU);
    firstBlue = static_cast<float>(layer->firstColour & 0xffU);
    secondAlpha = static_cast<float>(layer->secondColour >> 24);
    secondRed = static_cast<float>(layer->secondColour >> 16 & 0xffU);
    secondGreen = static_cast<float>(layer->secondColour >> 8 & 0xffU);
    secondBlue = static_cast<float>(layer->secondColour & 0xffU);
    stepAlpha = secondAlpha - firstAlpha;
    stepAlpha /= 10.0f;
    stepRed = secondRed - firstRed;
    stepRed /= 10.0f;
    stepGreen = secondGreen - firstGreen;
    stepGreen /= 10.0f;
    stepBlue = secondBlue - firstBlue;
    stepBlue /= 10.0f;
    const float *levels = airDomeColourLevels;
    for (std::int32_t level = 0; level < 5; ++level) {
        float factor;
        layer->colours[level][0] =
            (factor = *levels++, (static_cast<std::int32_t>(firstAlpha + stepAlpha * factor) << 8) | static_cast<std::int32_t>(firstRed + stepRed * factor));
        layer->colours[level][1] = (static_cast<std::int32_t>(firstGreen + stepGreen * factor) << 8) | static_cast<std::int32_t>(firstBlue + stepBlue * factor);
    }
}
#else
// @verified - d
void UpdateAirDomeColours() {
    AirDomeLayer &layer = g_airDomeLayers[0];
    const float first[4] = {static_cast<float>(layer.firstColour >> 24), static_cast<float>(layer.firstColour >> 16 & 0xffU),
        static_cast<float>(layer.firstColour >> 8 & 0xffU), static_cast<float>(layer.firstColour & 0xffU)};
    const float second[4] = {static_cast<float>(layer.secondColour >> 24), static_cast<float>(layer.secondColour >> 16 & 0xffU),
        static_cast<float>(layer.secondColour >> 8 & 0xffU), static_cast<float>(layer.secondColour & 0xffU)};
    float differences[4];
    for (std::int32_t channel = 0; channel < 4; ++channel)
        differences[channel] = (second[channel] - first[channel]) / 10.0f;
    for (std::int32_t level = 0; level < 5; ++level) {
        std::uint32_t channels[4];
        for (std::int32_t channel = 0; channel < 4; ++channel) {
            float value = first[channel];
            asm("fmac fr0,%1,%0" : "+f"(value) : "f"(differences[channel]), "w"(airDomeColourLevels[level]));
            asm("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(channels[channel]) : "f"(value) : "fpul");
        }
        layer.colours[level][0] = static_cast<std::uint16_t>(channels[0] << 8U | static_cast<std::uint16_t>(channels[1]));
        layer.colours[level][1] = static_cast<std::uint16_t>(channels[2] << 8U | static_cast<std::uint16_t>(channels[3]));
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const char *airTextureChunkNames[2] = {"PVRT", "GBIX"};

void LoadAirTextures(std::int32_t texture, float scale, std::int32_t layer) {
    char tag[8], directory[16], filename[16];
    if (g_airCloudLayers[layer].textureIndex == texture)
        return;
    g_airCloudLayers[layer].textureIndex = texture;
    g_airCloudLayers[layer].blendScale = scale;
    std::sprintf(directory, "sprite");
    std::sprintf(filename, "AIR_TX%02d.SPR", texture);
    shenLoadFile(directory, filename, g_armMotionMemory, 0x8400);
    std::byte *textureData = g_armMotionMemory;
    const std::uint32_t *header = reinterpret_cast<const std::uint32_t *>(textureData + 0x10);
    while (*header != SHEN_NAME4('P', 'V', 'R', 'T') && *header != SHEN_NAME4('G', 'B', 'I', 'X'))
        ++header;
    const char *characters = reinterpret_cast<const char *>(header);
    tag[0] = *characters++;
    tag[1] = *characters++;
    tag[2] = *characters++;
    tag[3] = *characters;
    tag[4] = 0;
    const std::uint32_t *image = std::strcmp(tag, airTextureChunkNames[1]) ? header : header + 3;
    if ((image[2] & 0xffU) == 1)
        SetAirLayerTexturePacketMode(&g_airCloudLayers[layer], 0, 0x08000000U);
    else
        SetAirLayerTexturePacketMode(&g_airCloudLayers[layer], 0, 0x10000000U);
    ReplacePtxTexture(g_airCloudLayers[layer].textureHandle, reinterpret_cast<const TextureChunk *>(textureData));
}
#else
// @verified - d
void LoadAirTextures(std::int32_t texture, float scale, std::int32_t layer) {
    AirCloudLayer &cloud = g_airCloudLayers[layer];
    if (cloud.textureIndex == texture)
        return;
    cloud.textureIndex = texture;
    cloud.blendScale = scale;
    char directory[16];
    char filename[20];
    std::sprintf(directory, "sprite");
    std::sprintf(filename, "AIR_TX%02d.SPR", texture);
    shenLoadFile(directory, filename, g_armMotionMemory, 0x8400);
    const std::uint32_t *header = reinterpret_cast<const std::uint32_t *>(g_armMotionMemory + 0x10);
    while (*header != SHEN_NAME(PVRT) && *header != SHEN_NAME(GBIX))
        ++header;
    if (*header == SHEN_NAME(GBIX))
        header += 3;
    if ((header[2] & 0xffU) == 1)
        SetAirLayerTexturePacketMode(&cloud, 0, 0x08000000U);
    else
        SetAirLayerTexturePacketMode(&cloud, 0, 0x10000000U);
    ReloadAirTexture(cloud.textureHandle, g_armMotionMemory);
}
#endif

// @verified - d
std::int32_t IsAirTextureTransitionActive() {
    return g_airDrawState.transitionActive;
}

// @verified - d
std::int32_t BeginAirTextureTransition(std::int32_t texture, std::int32_t frames, std::int32_t fadePrevious, float scale) {
    if (g_airDrawState.transitionActive)
        return 0;
    if (!GetPlayerHeldAirTransitionState()) {
        g_airDrawState.transitionActive = 1;
        g_airDrawState.transitionFrames = frames;
        LoadAirTextures(texture, scale, 1);
        const float step = 1.0f / static_cast<float>(frames);
        g_airCloudLayers[0].blend = 1.0f;
        if (fadePrevious == 1)
            g_airCloudLayers[1].blend = 1.0f;
        else
            g_airCloudLayers[1].blend = 0.0f;
        g_airCloudLayers[0].blendStep = step;
        g_airCloudLayers[1].blendStep = -step;
    }
    return 1;
}

// @verified - d
void AdvanceAirTextureTransition() {
    if (!g_airDrawState.transitionActive)
        return;
    g_airDrawState.transitionFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_airDrawState.transitionFrames) - 1U);
    if (g_airDrawState.currentSeconds - g_airDrawState.previousSeconds > 60U) {
        g_airCloudLayers[0].blend -= 0.05f;
        g_airCloudLayers[1].blend += 0.05f;
    }
    g_airCloudLayers[0].blend -= g_airCloudLayers[0].blendStep;
    if (g_airCloudLayers[0].blend > 1.0f)
        g_airCloudLayers[0].blend = 1.0f;
    if (0.0f > g_airCloudLayers[0].blend)
        g_airCloudLayers[0].blend = 0.0f;
    g_airCloudLayers[1].blend -= g_airCloudLayers[1].blendStep;
    if (g_airCloudLayers[1].blend > 1.0f)
        g_airCloudLayers[1].blend = 1.0f;
    if (0.0f > g_airCloudLayers[1].blend)
        g_airCloudLayers[1].blend = 0.0f;
    if (g_airDrawState.transitionFrames < 0)
        CancelAirTextureTransition();
}

// @verified - d
void CancelAirTextureTransition() {
    if (g_airDrawState.transitionActive) {
        SwapAirCloudLayers();
        g_airCloudLayers[0].blend = 1.0f;
        g_airCloudLayers[0].blendStep = 0.0f;
        g_airCloudLayers[1].blend = 0.0f;
        g_airCloudLayers[1].blendStep = 0.0f;
        g_airDrawState.transitionActive = 0;
        g_airDrawState.transitionFrames = 0;
    }
}

// @verified - d
std::uint32_t ReadAirSeconds() {
    DateTime date;
    shenGetCurrentDateTime(date);
    return static_cast<std::uint32_t>(ComputeSecondsSince1980(date));
}

// @verified - d
void UpdateAirSeconds() {
    g_airDrawState.previousSeconds = g_airDrawState.currentSeconds;
    g_airDrawState.currentSeconds = ReadAirSeconds();
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetIsSkySetup(std::int32_t value) {
    g_airDrawState.drawEnabled = value;
}

std::int32_t GetIsSkySetup() {
    return g_airDrawState.drawEnabled;
}
#endif

// @verified - d
void EV_SetPlanetLayerColours_Impl(std::uint32_t first, std::uint32_t second, std::int32_t layer) {
    g_airCloudLayers[layer].firstColour = first;
    g_airCloudLayers[layer].secondColour = second;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t GetPlanetLayerColour(std::int32_t channel, std::int32_t layer) {
    return channel ? g_airCloudLayers[layer].secondColour : g_airCloudLayers[layer].firstColour;
}
#endif

// @verified - d
void SetCloudAndSkyColour(std::uint32_t first, std::uint32_t second) {
    g_airDomeLayers[0].firstColour = first;
    g_airDomeLayers[0].secondColour = second;
}

// @verified - d
std::uint32_t GetWeatherCloudOrSkyColour(std::int32_t channel) {
    return channel ? g_airDomeLayers[0].secondColour : g_airDomeLayers[0].firstColour;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetPlanetLayerBlend(float blend, std::int32_t layer) {
    g_airCloudLayers[layer].blend = blend;
}

void SetAirDomeLayerBlend(float blend, std::int32_t layer) {
    g_airDomeLayers[layer].blend = blend;
}

float GetPlanetLayerBlend(std::int32_t layer) {
    return g_airCloudLayers[layer].blend;
}

float GetAirDomeLayerBlend(std::int32_t layer) {
    return g_airDomeLayers[layer].blend;
}
#endif

// @verified - d
void shenUpdatePlanetByLayer(float value, std::int32_t channel, std::int32_t layer) {
    if (channel == 0)
        g_airCloudLayers[layer].scrollRate.x = value;
    else
        g_airCloudLayers[layer].scrollRate.y = value;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float GetPlanetLayerScrollRate(std::int32_t channel, std::int32_t layer) {
    if (channel == 0)
        return g_airCloudLayers[layer].scrollRate.x;
    return g_airCloudLayers[layer].scrollRate.y;
}

void ResetPlanetLayerScroll(std::int32_t layer) {
    GetAirCloudModelRoot(layer);
    AirCloudLayer *const cloud = &g_airCloudLayers[layer];
    ScrollAirCloudPackets(cloud, 1, 0.0f, 0.0f);
    cloud->scroll.x = 0.0f;
    cloud->scroll.y = 0.0f;
    cloud->retainedScroll.x = 0.0f;
    cloud->retainedScroll.y = 0.0f;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetPlanetLayerOffset(const Vector3 *offset, std::int32_t layer) {
    AirCloudLayer *const cloud = &g_airCloudLayers[layer];
    cloud->offset = *offset;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetPlanetLayerOffset(std::int32_t layer) {
    g_airCloudLayers[layer].offset.x = 0.0f;
    g_airCloudLayers[layer].offset.y = -115.0f;
    g_airCloudLayers[layer].offset.z = 0.0f;
}

void SetPlanetLayersTexturePacketMode(std::uint32_t value) {
    AirCloudLayer *cloud = g_airCloudLayers;
    for (std::int32_t layer = 0; layer < 2; ++layer)
        SetAirLayerTexturePacketMode(cloud, 1, value);
}

void GetPlanetPosition(Vector3 *position, std::int32_t index) {
    const AirPlanetDrawRecord *const planet = &g_airPlanet[index];
    position->x = planet->position.x;
    position->y = planet->position.y;
    position->z = planet->position.z;
}
#endif

// @verified - d
void SetPlanetRotByIndex(const Vector3i &rotation, std::int32_t index) {
    g_airPlanet[index].intermediateRotation.x = rotation.x;
    g_airPlanet[index].intermediateRotation.y = rotation.y;
    g_airPlanet[index].intermediateRotation.z = rotation.z;
}

void GetPlanetRotByIndex(Vector3i &rotation, std::int32_t index) {
    rotation.x = g_airPlanet[index].intermediateRotation.x;
    rotation.y = g_airPlanet[index].intermediateRotation.y;
    rotation.z = g_airPlanet[index].intermediateRotation.z;
}

// @verified - d
void SetPlanetLayerParameter(float value, std::int32_t layer) {
    g_airPlanet[layer].intensity = value;
}

// @verified - d
float GetPlanetLayerParameter(std::int32_t layer) {
    return g_airPlanet[layer].intensity;
}

// @verified - d
void UpdateAirDomeBlend() {
    float time = GetAirTimeOfDay();
    if (time < 190000.0f && time > 180000.0f) {
        time -= 180000.0f;
        time /= 10000.0f;
    } else if (!(time < 50000.0f) && time < 60000.0f) {
        time -= 50000.0f;
        time = 1.0f - time / 10000.0f;
    } else if (time < 190000.0f && time >= 50000.0f)
        time = 0.0f;
    else
        time = 1.0f;
    g_airDomeLayers[1].blend = time;
}

// @verified - d
void UpdateAirTimeOfDay() {
    DateTime date;
    shenGetCurrentDateTime(date);
    const float hour = static_cast<float>(date.hour) * 10000.0f;
    const float minute = (static_cast<float>(date.minute) / 60.0f) * 10000.0f;
    const float seconds = (static_cast<float>(date.second) / 60.0f) * 100.0f;
    const float scaled = seconds * 1.666666f;
    g_airDrawState.timeOfDay = hour + minute + scaled;
}

float GetAirTimeOfDay() {
    return g_airDrawState.timeOfDay;
}

// @verified - d
void UpdateCelestialPosition() {
    UpdateCelestialCoordinates();
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void UpdateCelestialRotation(const Vector3i &angles, const Vector3 &input, Vector3 &output) {
    Vector3i savedAngles;
    Vector3 initial, transformed;
    float sine, cosine;
    const std::int32_t *angle = &angles.x;
    const float *position = &input.x;
    savedAngles.x = *angle++;
    savedAngles.y = *angle;
    transformed.x = initial.x = *position++;
    transformed.y = initial.y = *position++;
    transformed.z = initial.z = *position;
    sine = dc::sdk::SinFixed(savedAngles.y);
    cosine = dc::sdk::CosFixed(savedAngles.y);
    transformed.x = initial.x * cosine - initial.z * sine;
    transformed.z = initial.x * sine + initial.z * cosine;
    sine = dc::sdk::SinFixed(savedAngles.x);
    cosine = dc::sdk::CosFixed(savedAngles.x);
    initial.z = transformed.z;
    transformed.z = initial.z * cosine - initial.y * sine;
    transformed.y = initial.z * sine + initial.y * cosine;
    float *destination = &output.x;
    *destination++ = transformed.x;
    *destination++ = transformed.y;
    *destination = transformed.z;
}
#else
// @verified - d
void UpdateCelestialRotation(const Vector3i &angles, const Vector3 &input, Vector3 &output) {
    const std::int32_t firstAngle = angles.x;
    const std::int32_t secondAngle = angles.y;
    const Vector3 initial = input;
    Vector3 transformed = initial;
    const float secondSine = dc::sdk::SinFixed(secondAngle);
    const float secondCosine = dc::sdk::CosFixed(secondAngle);
    transformed.x = initial.x * secondCosine - initial.z * secondSine;
    float z = initial.z * secondCosine;
    asm("fmac fr0,%1,%0" : "+f"(z) : "f"(initial.x), "w"(secondSine));
    transformed.z = z;
    const float firstSine = dc::sdk::SinFixed(firstAngle);
    const float firstCosine = dc::sdk::CosFixed(firstAngle);
    const float rotatedZ = transformed.z;
    transformed.z = rotatedZ * firstCosine - initial.y * firstSine;
    float y = initial.y * firstCosine;
    asm("fmac fr0,%1,%0" : "+f"(y) : "f"(rotatedZ), "w"(firstSine));
    transformed.y = y;
    output.x = transformed.x;
    output.y = transformed.y;
    output.z = transformed.z;
}
#endif

// @verified - d
void UpdateCelestialCoordinates() {
    const float phase = GetAirTimeOfDay() * 0.2730666697025299f;
    g_airPlanet[0].timeRotation.x = static_cast<std::int32_t>(phase) + 16384;
    UpdateCelestialRotation(*reinterpret_cast<const Vector3i *>(&g_airPlanet[0].tilt), g_airPlanet[0].orbitInput, g_airPlanet[0].position);
    g_airPlanet[0].rotation.x = g_airPlanet[0].intermediateRotation.x;
    std::int32_t yaw = g_airPlanet[0].intermediateRotation.y + 16384;
    yaw -= g_airPlanet[0].timeRotation.x;
    g_airPlanet[0].rotation.y = yaw + g_airPlanet[0].timeRotation.z;
    g_airPlanet[0].rotation.z = g_airPlanet[0].intermediateRotation.z;
}

} // namespace s2
} // namespace shenmue

#endif
