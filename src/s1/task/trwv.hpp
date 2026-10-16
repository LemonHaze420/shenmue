#pragma once

#include "s1/d3t/d3t_texture.hpp"
#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;
struct TrwvRippleRecord;

#pragma pack(push, 1)

struct TransformedWaveform {
    std::int16_t phaseX;
    std::int16_t phaseY;
    std::int16_t phaseStepX;
    std::int16_t phaseStepY;
    std::uint32_t flags;
    std::uint32_t rippleActive;
    std::int32_t terrainSlot;
    std::int32_t textureSlot;
    std::int32_t auxiliaryTextureSlot;
    float scale;
    float amplitude;
    float frequency;
    float velocityX;
    float velocityZ;
    std::int32_t reserved030;
    float reserved034;
    std::int32_t stripOffsetU;
    std::int32_t stripOffsetV;
    std::int32_t activeStripOffsetU;
    std::int32_t activeStripOffsetV;
    std::int32_t gridDivisionsX;
    std::int32_t gridDivisionsZ;
    std::int32_t shoreOffsetU;
    std::int32_t shoreOffsetV;
    std::int32_t activeShoreOffsetU;
    std::int32_t activeShoreOffsetV;
    std::uint32_t colourArgb;
    std::int32_t reserved064;
    void *model;
    void *modelFile;
    void *rippleModel;
    void *rippleModelFile;
    std::uint8_t *shoreAlphaTables;
    void *velocityFile;
    void *vertexFile;
    TrwvRippleRecord *rippleRecords;
    std::int32_t yaw;
    Vector3 position;
    Vector3 localOffset;
    Vector3 lightDirection;
    float patchSizeX;
    float patchSizeZ;
    std::int32_t positiveExtentX;
    std::int32_t negativeExtentX;
    std::int32_t positiveExtentZ;
    std::int32_t negativeExtentZ;
    std::uint32_t emitterColour;
    std::int32_t reserved0ec;
    void *attachedNode;
    void *shoreModels[4];
    void *shoreModelFiles[4];
    std::byte reserved138[8];
};
#pragma pack(pop)

static_assert(sizeof(TransformedWaveform) == 0x140);
static_assert(offsetof(TransformedWaveform, flags) == 0x08);
static_assert(offsetof(TransformedWaveform, stripOffsetU) == 0x38);
static_assert(offsetof(TransformedWaveform, colourArgb) == 0x60);
static_assert(offsetof(TransformedWaveform, model) == 0x68);
static_assert(offsetof(TransformedWaveform, velocityFile) == 0x90);
static_assert(offsetof(TransformedWaveform, vertexFile) == 0x98);
static_assert(offsetof(TransformedWaveform, yaw) == 0xa8);
static_assert(offsetof(TransformedWaveform, position) == 0xac);
static_assert(offsetof(TransformedWaveform, localOffset) == 0xb8);
static_assert(offsetof(TransformedWaveform, lightDirection) == 0xc4);
static_assert(offsetof(TransformedWaveform, patchSizeX) == 0xd0);
static_assert(offsetof(TransformedWaveform, positiveExtentX) == 0xd8);
static_assert(offsetof(TransformedWaveform, emitterColour) == 0xe8);
static_assert(offsetof(TransformedWaveform, attachedNode) == 0xf0);
static_assert(offsetof(TransformedWaveform, shoreAlphaTables) == 0x88);
static_assert(offsetof(TransformedWaveform, rippleRecords) == 0xa0);
static_assert(offsetof(TransformedWaveform, shoreModels) == 0xf8);
static_assert(offsetof(TransformedWaveform, shoreModelFiles) == 0x118);

struct EffeParticle;

// TRWVPvrEmitterConfig: one of the six MEND bow-spray emitters on an EFFE particle system (commands 200/201).
struct TrwvBowSprayEmitter {
    Vector3 origin;
    Vector3 range;
    Vector3 velocity;
    Vector3 velocityRange;
    float probability;
    float width;
    float height;
    float sway;
    std::int32_t runtimeSlot;
    std::int32_t particleCount;
    EffeParticle *particles;
};

static_assert(sizeof(TrwvBowSprayEmitter) == 0x50);
static_assert(offsetof(TrwvBowSprayEmitter, sway) == 0x3c);
static_assert(offsetof(TrwvBowSprayEmitter, runtimeSlot) == 0x40);
static_assert(offsetof(TrwvBowSprayEmitter, particles) == 0x48);

struct WaveComplex {
    float real;
    float imaginary;
};

struct WaveSpectrumRecord {
    float amplitude;
    float phase0;
    float velocity0;
    float phase1;
    float velocity1;
};

// terrainPatchData: 924 dwords per slot. real/imaginary are 17x17 (row stride 17) with column and row 16 mirroring 0.
struct TerrainPatch {
    std::int32_t reserved000;
    std::uint32_t colour;
    std::array<std::int32_t, 24> reserved008;
    std::array<WaveSpectrumRecord, 64> spectrum;
    std::array<float, 289> real;
    std::array<float, 289> imaginary;
};

static_assert(sizeof(TerrainPatch) == 924 * 4);
static_assert(offsetof(TerrainPatch, spectrum) == 26 * 4);
static_assert(offsetof(TerrainPatch, real) == 346 * 4);
static_assert(offsetof(TerrainPatch, imaginary) == 635 * 4);

struct TerrainColourEntry {
    float height;
    std::uint32_t diffuse;
    std::uint32_t specular;
};

static_assert(sizeof(TerrainColourEntry) == 12);

// g_pRuntimeEffectTextures record: CreateDirectXTexture2D fills the head, CreateDynamicTexture the EFFE texture descriptor at +0x30.
struct RuntimeEffectTexture {
    DecodedTextureFormat format;
    std::byte reserved28[0x08];
    std::uint32_t words[4];
    std::uint32_t flags;
    std::uint32_t reserved44;
};

static_assert(sizeof(RuntimeEffectTexture) == 0x48);
static_assert(offsetof(RuntimeEffectTexture, format.texture) == 0x20);
static_assert(offsetof(RuntimeEffectTexture, words) == 0x30);

#pragma pack(push, 1)
// MT5/MT6 model node; every offset is relative to g_pCurrentTaskData once setup_trwave_ptrs has run.
struct WaveModelNode {
    std::uint32_t flags;
    std::int32_t meshOffset;
    std::int32_t rotX;
    std::int32_t rotY;
    std::int32_t rotZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    float posX;
    float posY;
    float posZ;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t offset34;
    std::int32_t offset38;
    std::int32_t offset3C;
};
#pragma pack(pop)

static_assert(sizeof(WaveModelNode) == 0x40);

// Ripple record (40 bytes): one grid point of the 17x17 splash ripple. SetupTRWave runs every handler while the ripple layer is active; the handler
// chain raises the alpha towards the limit, then lets it fall back to 5 at a rate chosen by the point's distance from the centre.
using TrwvRippleHandler = void (*)(TrwvRippleRecord *);
struct TrwvRippleRecord {
    std::uint8_t alpha;
    std::uint8_t step;
    std::uint8_t limit;
    std::uint8_t gridIndex;
    float x;
    std::uint32_t reserved08;
    float z;
    std::byte reserved10[0x10];
    TrwvRippleHandler handler;
};

static_assert(sizeof(TrwvRippleRecord) == 0x28);
static_assert(offsetof(TrwvRippleRecord, handler) == 0x20);

// The ripple list descriptor, published through the list pointer the ripple draw and SetupTRWave read.
struct TrwvRippleList {
    std::int32_t reserved00;
    std::uint32_t count;
    TrwvRippleRecord *records;
};

std::uint32_t InitTransformedWaveform(const Vector3 *origin, const char *textureDirectory, const char *textureFilename, const char *waveformDirectory,
    const char *waveformFilename, float scale, std::uint32_t flags);
HLib::Task *TRWV_Init();
TransformedWaveform *TRWV_GetWaveform(std::uint32_t slot);
HLib::Task *TRWV_GetTask();
void TRWV_Destroy();
void TRWV_Callback(void *taskData);
// The 16-point transform TRWV's water and the EFFE FFT both run: bit reversal, ApplyButterflyPass_16Point, the two half stages and the
// 1/16 scaling of ApplyIFFTScalingAndReordering.
void Run16PointFFTButterflyPasses(std::array<WaveComplex, 16> &values);
// InitializeFFTData's twiddles: 16 entries in pi/16 steps into the tables Generate16PointTwiddles fills. Entries 0-7 equal the 16-point
// ones, which are all ApplyButterflyPass_16Point reads.
void Generate32PointTwiddles();
void Script_MT5Loader_TRWV_TRWAVE(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
