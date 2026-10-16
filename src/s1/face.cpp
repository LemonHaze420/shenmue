#include "s1/face.hpp"

#include "s1/task/char.hpp"
#include "s1/task/kuti.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/boot.hpp"
#include "s1/face_cache.hpp"
#include "s1/file_loader.hpp"
#include "s1/initialization.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/vfma.hpp"
#include "s1/mt5_types.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/motion.hpp"
#include "s1/render_array.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/scene.hpp"
#include "s1/timing.hpp"
#include "common/utility.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>

namespace shenmue::s1 {

extern void *g_YKBE;
extern void *g_FMOT;
extern std::uint8_t g_debugFaceUnknown;
extern std::uint8_t g_debugUnknown2;
extern std::uint8_t g_debugUnknown3;
std::int64_t isValidCEWP(void *model);
FaceModelCacheSlot g_faceModelCache[8]{};
std::int32_t g_clipperGlobalFlag = 0;

std::uint8_t g_faceLookLimitFlag{};
std::array<char, 64> g_faceDirectory{};

void Clipper_SetGlobalFlag(std::int32_t value) {
    g_clipperGlobalFlag = value;
}
std::int32_t Clipper_GetGlobalFlag() {
    return g_clipperGlobalFlag;
}

namespace {

constexpr std::uint32_t kClipComponent = SHEN_NAME(CLIP);
constexpr std::uint32_t kFaceComponent = SHEN_NAME(FACE);



constexpr std::array<std::uint8_t, 6> kVisemeMap{0, 3, 2, 5, 4, 1};

constexpr std::int32_t kFaceTalkBankToken = SHEN_NAME(FMOT);
constexpr std::int32_t kHLibUseStatus = 0x20455355;
constexpr std::size_t kFaceTalkBankSize = 48128;
constexpr std::array<const char *, 8> kRandomTalkModels{
    "HGY_F.MT5",
    "HGR_F.MT5",
    "HGM_F.MT5",
    "HGS_F.MT5",
    "HGC_F.MT5",
    "OIS_F.MT5",
    "HGN_F.MT5",
    "KC1_F.MT5",
};
constexpr std::array<const char *, 8> kRandomTalkTables{
    "TALK_Y.BIN",
    "TALK_R.BIN",
    "TALK_G.BIN",
    "TALK_K.BIN",
    "TALK_C.BIN",
    "TALK_O.BIN",
    "TALK_N.BIN",
    "TALK_KC.BIN",
};

std::array<FaceMotionProgram, 5> g_faceMotionPrograms{};

std::uint16_t g_faceMotionProgramCount{};
constexpr std::size_t kFaceRigChannelCount = 25;

#pragma pack(push, 1)
struct FaceVertexRecord {
    Vector3 position;
    Vector3 normal;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FaceLookState {
    std::byte reserved00[0x10];
    std::int8_t flags;
    std::byte reserved11[3];
    Vector3 targetPosition;
    Mt5RuntimeNode *targetNode;
    std::byte reserved28[0x0c];
    std::int32_t currentPitchA;
    std::int32_t currentYawA;
    std::int32_t reserved3c;
    std::int32_t currentPitchB;
    std::int32_t currentYawB;
    std::int32_t reserved48;
    std::int32_t targetPitchA;
    std::int32_t targetYawA;
    std::int32_t reserved54;
    std::int32_t targetPitchB;
    std::int32_t targetYawB;
    std::int32_t reserved60;
    std::int32_t deltaPitchA;
    std::int32_t deltaYawA;
    std::int32_t reserved6c;
    std::int32_t deltaPitchB;
    std::int32_t deltaYawB;
    std::int32_t pitchMaximum;
    std::int32_t pitchMinimum;
    std::int32_t yawAMaximum;
    std::int32_t yawAMinimum;
    std::int32_t yawBMaximum;
    std::int32_t yawBMinimum;
    std::int16_t blendFrames;
    std::int16_t blendFrame;
};
#pragma pack(pop)

static_assert(sizeof(FaceLookState) == 0x94);
static_assert(offsetof(FaceLookState, flags) == 0x10);
static_assert(offsetof(FaceLookState, currentPitchA) == 0x34);
static_assert(offsetof(FaceLookState, blendFrames) == 0x90);

static_assert(sizeof(FaceVertexRecord) == 24);

struct FaceHmdlChannelRuntime {
    float blinkWeight;
    float blinkTime;
    std::uint8_t blinkPending;
    std::byte reserved09[3];
    float yaw;
    float pitch;
    float targetYaw;
    float targetPitch;
    Vector3 origin;
    Vector3 target;
};

static_assert(sizeof(FaceHmdlChannelRuntime) == 0x34);



struct FaceHmdlExpressionSlot {
    std::uint32_t index;
    float blend;
    float fadeSeconds;
    float timer;
    std::uint8_t pending;
    std::byte reserved11[3];
};

static_assert(sizeof(FaceHmdlExpressionSlot) == 0x14);


constexpr const char *kFaceTargetGroups[6]{"UpperFace", "LowerFace", "FullFace", "LeftEyeLid", "RightEyeLid", "Mouth"};
constexpr const char *kFacialExpressions[13]{"Expressionless", "Laugh", "Angry", "Grieve", "Suffer", "VeryUpset", "VerySad", "Amazed", "Impatient",
    "ReallyLaugh", "WakingUp", "SufferTerribly", "SufferingLaughter"};
constexpr const char *kMouthRigs[6]{"Mouth0", "Mouth1", "Mouth2", "Mouth3", "Mouth4", "Mouth5"};
static_assert(offsetof(FaceHmdlChannelRuntime, yaw) == 0x0c);
static_assert(offsetof(FaceHmdlChannelRuntime, targetYaw) == 0x14);
static_assert(offsetof(FaceHmdlChannelRuntime, origin) == 0x1c);
static_assert(offsetof(FaceHmdlChannelRuntime, target) == 0x28);



struct FaceBlendState {
    FaceVertexRecord *vertices;
    Vector3 *deltas;
    Vector3 *targets;
    std::int16_t vertexCount;
    float eyeBlendFrame;
    float eyeBlendFrames;
    float mouthBlendFrame;
    float mouthBlendFrames;
};
static_assert(sizeof(FaceBlendState) == 0x30);
static_assert(offsetof(FaceBlendState, eyeBlendFrame) == 0x1c);





std::array<std::byte *, 8> g_faceTableFiles{};
std::array<FaceTableBinding, 8> g_faceTables{};
std::array<FaceVertexRecord *, 8> g_faceVertexCopies{};
std::array<Vector3 *, 8> g_faceRestPositions{};
std::array<Vector3 *, 8> g_faceTargetPositions{};
std::array<Vector3 *, 8> g_faceBlendDeltas{};
std::array<std::uint8_t *, 8> g_faceEyeVertexFlags{};
std::array<FaceBlendState, 8> g_faceBlendStates{};
Vector3 *g_faceEyePassPositions{};
std::array<Vector3, kFaceRigChannelCount> g_faceChannelDeltas{};
FaceCalcContext g_faceCalcContext{};

char UpperAscii(char value) {
    return value >= 'a' && value <= 'z' ? static_cast<char>(value - ('a' - 'A')) : value;
}

bool EqualInsensitive(const char *left, const char *right) {
    while (*left || *right) {
        if (UpperAscii(*left++) != UpperAscii(*right++))
            return false;
    }
    return true;
}

std::uint8_t faceFindCacheSlotByModelName(const char *filename) {
    for (std::uint8_t index = 0; index < 8; ++index) {
        if (EqualInsensitive(g_faceModelCache[index].filename, filename))
            return index;
    }

    for (std::uint8_t index = 0; index < 8; ++index) {
        if (EqualInsensitive(g_faceModelCache[index].filename, "NULL"))
            return index;
    }
    return 8;
}
// FACE_Init's blocks: the HLib heap allocation it inlines, marked protected from unnamed frees.
void *AllocateProtectedFaceBlock(std::int64_t size, std::uint32_t token) {
    void *const block = HLib_AllocHeapMemBlock(size, static_cast<std::int32_t>(token));
    if (block)
        (static_cast<HLib::TaskData *>(block) - 1)->alignment |= 0x10;
    return block;
}

bool FaceTalkBankIsLive() {
    if (!g_faceMotionPrograms[4].program)
        return false;
    const HLib::TaskData *const header = reinterpret_cast<const HLib::TaskData *>(g_faceMotionPrograms[4].program) - 1;
    return header->status == kHLibUseStatus && header->taskToken == kFaceTalkBankToken;
}

void EnsureFaceTalkBank() {
    if (FaceTalkBankIsLive())
        return;
    g_faceMotionPrograms[4] = {};

    std::uint32_t hgmlSize = 0;
    const bool hasHgml = MAPINFO_FindChunkPayload(SHEN_NAME(HGML), &hgmlSize) != nullptr;
    if (!hasHgml && g_areaId != SHEN_NAME(JU00))
        return;
    g_faceMotionPrograms[4].program = static_cast<std::byte *>(AllocateProtectedFaceBlock(kFaceTalkBankSize, kFaceTalkBankToken));
}

void LoadRandomTalkBank(const char *modelName) {
    std::size_t index = 0;
    while (index < kRandomTalkModels.size() && !EqualInsensitive(modelName, kRandomTalkModels[index]))
        ++index;
    if (index == kRandomTalkModels.size())
        return;
    EnsureFaceTalkBank();
    FaceMotionProgram &talk = g_faceMotionPrograms[4];
    if (!talk.program)
        return;

    char directory[64]{};
    std::snprintf(directory, sizeof(directory), "scene/%02d/model/face", g_sceneId);
    const char *const filename = kRandomTalkTables[index];
    char path[272]{};
    std::snprintf(path, sizeof(path), "%s/%s", directory, filename);
    const std::uint64_t size = OpenFileInternal(path);
    (void)shenLoadFile(directory, filename, talk.program, static_cast<std::int32_t>(size));
    const FmotProgramHeader *const header = reinterpret_cast<const FmotProgramHeader *>(talk.program);
    talk.eyeChannels = reinterpret_cast<const FmotChannelDescriptor *>(talk.program + header->eyeChannelTableOffset);
    talk.mouthChannels = reinterpret_cast<const FmotChannelDescriptor *>(talk.program + header->mouthChannelTableOffset);
    talk.keyRecordCount = header->keyRecordCount;
}

PackModel *FindFaceModelInReadyPack(const char *filename) {
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    if (!pack || !pack->usePackCache || !pack->readyPack)
        return nullptr;
    return PACK_FindCharacterModel(pack->readyPack, filename);
}

std::uint8_t faceLoadModelIntoCacheSlot(const char *filename, std::uint8_t index) {
    if (!filename)
        return 0;
    FaceModelCacheSlot &slot = g_faceModelCache[index];
    if (slot.state != 0)
        return 1;
    PackModel *owner = FindFaceModelInReadyPack(filename);
    if (!owner)
        owner = MT5_LoadPackModel(g_faceDirectory.data(), filename);
    if (!owner || !owner->model)
        return 0;
    slot.modelOwner = owner;
    slot.modelRoot = owner->model;
    return 2;
}



std::byte *LoadFaceTable(const char *filename) {
    if (std::strcmp(filename, "YKB_FTBL.BIN") == 0)
        return static_cast<std::byte *>(g_YKBE);
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask)
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    if (pack && pack->usePackCache && pack->readyPack) {
        if (void *const table = PACK_FindFileData(pack->readyPack, filename, make_shen_name('B', 'I', 'N', ' ')))
            return static_cast<std::byte *>(table);
    }
    const std::uint64_t size = shenOpenFile(g_faceDirectory.data(), filename);
    std::byte *const table = static_cast<std::byte *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(size), SHEN_NAME(FENV)));
    if (table)
        (reinterpret_cast<HLib::TaskData *>(table) - 1)->alignment |= 0x10;
    (void)shenLoadFile(g_faceDirectory.data(), filename, table, static_cast<std::int32_t>(size));
    return table;
}

} // namespace

// FACE_Callback's binding rule: field_20 is the cache slot's current model.
bool FACE_IsBoundToCachedModel(const FaceWork &face) {
    const std::uint8_t cacheIndex = static_cast<std::uint8_t>(static_cast<std::uint16_t>(face.eyeMode) >> 8U);
    return face.field20 != 0 && cacheIndex < 8 && reinterpret_cast<void *>(face.field20) == g_faceModelCache[cacheIndex].modelRoot;
}

namespace {



void ReleaseFaceCacheReference(std::size_t index, const char *expectedName) {
    FaceModelCacheSlot &slot = g_faceModelCache[index];
    if (EqualInsensitive(slot.filename, "YKB_F.MT5") || !EqualInsensitive(slot.filename, expectedName))
        return;
    if (--slot.state > 0)
        return;
    std::memcpy(slot.filename, "NULL", 5);
    slot.state = 0;
    const bool releasedModel = slot.modelOwner && !PACK_FindModelOwner(static_cast<PackModel *>(slot.modelOwner));
    if (releasedModel)
        FreePVR(slot.modelOwner);
    slot.modelRoot = nullptr;
    slot.modelOwner = nullptr;
    if (std::byte *const table = g_faceTableFiles[index]) {
        if (releasedModel) {
            (reinterpret_cast<HLib::TaskData *>(table) - 1)->alignment &= ~0x10U;
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(table), 0);
        }
        g_faceTableFiles[index] = nullptr;
        g_faceTables[index] = {};
    }
}

// An FTBL offset: small values are relative to the loaded table, larger ones (already relocated) to g_pCurrentTaskData.
std::byte *FaceTableAddress(std::byte *table, std::int32_t offset) {
    return (static_cast<std::uint32_t>(offset) & 0xffff0000U) == 0 ? table + offset : g_pCurrentTaskData + offset;
}

template <typename T> T *FaceTaskRelative(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(g_pCurrentTaskData + offset) : nullptr;
}

std::int32_t FaceTaskOffset(const void *address) {
    return address ? static_cast<std::int32_t>(static_cast<const std::byte *>(address) - g_pCurrentTaskData) : 0;
}

Mt5RuntimeNode *faceFindModelNodeByWordRecursive(Mt5RuntimeNode *node, std::uint16_t word);

float FACE_DecodeKeyFloat(std::uint16_t raw) {
    if ((raw & 0x7fffU) == 0)
        return 0.0f;
    const std::int32_t signedRaw = static_cast<std::int16_t>(raw);
    const std::uint32_t bits = ((static_cast<std::uint32_t>((signedRaw & 0x3fff) + 0xe000) << 14U) - (static_cast<std::uint32_t>(signedRaw & 0x4000) << 14U)) |
        ((static_cast<std::uint32_t>(signedRaw) << 16U) & 0x80000000U);
    float value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}



// @verified - p
float FACE_FindOrInterpolateChannelKey(std::int16_t keyCount, const FmotChannelKey *keys, std::int32_t frame) {
    std::int32_t upper = keyCount;
    std::int32_t lower = 0;
    do {
        const std::int32_t middle = (lower + upper) / 2;
        if (keys[middle].frame < frame)
            lower = middle;
        else
            upper = middle;
    } while (upper > lower + 1);
    if (upper == 0)
        return FACE_DecodeKeyFloat(keys[0].value);
    const std::int32_t previous = upper - 1;
    if (upper == keyCount || keys[previous].frame == frame)
        return FACE_DecodeKeyFloat(keys[previous].value);

    const float span = static_cast<float>(keys[upper].frame - keys[previous].frame);
    const float t = static_cast<float>(frame - keys[previous].frame) / span;
    const float t2 = t * t;
    const float t3 = t2 * t;
    const float value0 = FACE_DecodeKeyFloat(keys[previous].value);
    const float value1 = FACE_DecodeKeyFloat(keys[upper].value);
    const float tangent0 = FACE_DecodeKeyFloat(keys[previous].outgoingTangent);
    const float tangent1 = FACE_DecodeKeyFloat(keys[upper].incomingTangent);
    const float values = ((t3 + t3) - t2 * 3.0f + 1.0f) * value0 + (t2 * 3.0f - (t3 + t3)) * value1;
    const float tangents = ((t3 - (t2 + t2)) + t) * tangent0 + (t3 - t2) * tangent1;
    return tangents * span + values;
}

std::int16_t FACE_RadiansToFixed16(float radians) {
    return static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(radians * 32768.0f) / 3.141592653589793));
}

void FACE_ApplyRigTransform(const FtblChannelRigRecord &record) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(record.targetPosition.x, record.targetPosition.y, record.targetPosition.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(FACE_RadiansToFixed16(record.euler.z)));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(FACE_RadiansToFixed16(record.euler.y)));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(FACE_RadiansToFixed16(record.euler.x)));
}

Vector3 FACE_LocalDirection(const Matrix4x4 &transform, const Vector3 &target) {
    const float dx = target.x - transform.rows[3].x;
    const float dy = target.y - transform.rows[3].y;
    const float dz = target.z - transform.rows[3].z;
    return {
        dx * transform.rows[0].x + dy * transform.rows[0].y + dz * transform.rows[0].z,
        dx * transform.rows[1].x + dy * transform.rows[1].y + dz * transform.rows[1].z,
        dx * transform.rows[2].x + dy * transform.rows[2].y + dz * transform.rows[2].z,
    };
}

void FACE_AimAt(const Matrix4x4 &basis, const Vector3 &target) {
    const Vector3 local = FACE_LocalDirection(basis, target);
    const float distanceXY = std::sqrt(local.x * local.x + local.y * local.y);
    const float sinZ = distanceXY > 0.0f ? local.y / distanceXY : 0.0f;
    const float cosZ = distanceXY > 0.0f ? local.x / distanceXY : 1.0f;
    const float distance = std::sqrt(distanceXY * distanceXY + local.z * local.z);
    const float sinY = distance > 0.0f ? -local.z / distance : 0.0f;
    const float cosY = distance > 0.0f ? distanceXY / distance : 1.0f;
    Matrix4x4::RotateAroundZ(sinZ, cosZ);
    Matrix4x4::RotateAroundY(sinY, cosY);
}



// @verified - p
void FACE_SolveSingleSegmentAim(const Matrix4x4 &basis, Vector3 &target, const FtblChannelRigRecord &rig) {
    FACE_AimAt(basis, target);
    Matrix4x4::preTranslate(rig.firstSegmentLength, 0.0f, 0.0f);
    target = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}



// @verified - p
void FACE_SolveTwoSegmentIK(const Matrix4x4 &basis, Vector3 &target, const FtblChannelRigRecord &lengthRig, const FtblChannelRigRecord &rig) {
    const Vector3 local = FACE_LocalDirection(basis, target);
    const float distanceXY = std::sqrt(local.x * local.x + local.y * local.y);
    const float distance = std::sqrt(distanceXY * distanceXY + local.z * local.z);
    FACE_AimAt(basis, target);

    const float upper = lengthRig.firstSegmentLength;
    const float lower = lengthRig.secondSegmentLength;
    const float distanceSquared = distance * distance;
    float upperCos = static_cast<float>(static_cast<double>(distanceSquared + upper * upper - lower * lower) /
        ((static_cast<double>(distance) + static_cast<double>(distance)) * static_cast<double>(upper)));
    upperCos = std::clamp(upperCos, -1.0f, 1.0f);
    const float upperSin = std::sqrt(static_cast<float>(1.0 - static_cast<double>(upperCos * upperCos)));
    Matrix4x4::RotateAroundZ(-upperSin, upperCos);
    Matrix4x4::preTranslate(rig.firstSegmentLength, 0.0f, 0.0f);

    float lowerCos = static_cast<float>(static_cast<double>(lower * lower + upper * upper - distanceSquared) /
        ((static_cast<double>(upper) + static_cast<double>(upper)) * static_cast<double>(lower)));
    lowerCos = std::clamp(lowerCos, -1.0f, 1.0f);
    lowerCos = -lowerCos;
    const float lowerSin = std::sqrt(static_cast<float>(1.0 - static_cast<double>(lowerCos * lowerCos)));
    Matrix4x4::RotateAroundZ(lowerSin, lowerCos);
    Matrix4x4::preTranslate(rig.secondSegmentLength, 0.0f, 0.0f);
    target = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}



void FACE_BuildChannelTransformsAndSolve(std::int32_t frame, std::int32_t channelCount, const FtblChannelRigRecord *basisRigs, const FtblChannelRigRecord *rigs,
    const std::byte *program, const FmotChannelDescriptor *descriptors) {
    if (!basisRigs || !rigs || !program || !descriptors)
        return;
    Matrix4x4_Push();
    for (std::int32_t channel = 0; channel < channelCount; ++channel) {
        const FmotChannelDescriptor &descriptor = descriptors[channel];
        if (descriptor.keyCountX == 0) {
            g_faceChannelDeltas[channel] = {};
            continue;
        }
        Vector3 target{
            FACE_FindOrInterpolateChannelKey(descriptor.keyCountX, reinterpret_cast<const FmotChannelKey *>(program + descriptor.keyIndexX * 16), frame),
            FACE_FindOrInterpolateChannelKey(descriptor.keyCountY, reinterpret_cast<const FmotChannelKey *>(program + descriptor.keyIndexY * 16), frame),
            FACE_FindOrInterpolateChannelKey(descriptor.keyCountZ, reinterpret_cast<const FmotChannelKey *>(program + descriptor.keyIndexZ * 16), frame),
        };
        FACE_ApplyRigTransform(basisRigs[channel]);
        const Matrix4x4 basis = workTransformMatrix;
        FACE_ApplyRigTransform(rigs[channel]);
        if (basisRigs[channel].solverKind == 1)
            FACE_SolveSingleSegmentAim(basis, target, rigs[channel]);
        else
            FACE_SolveTwoSegmentIK(basis, target, basisRigs[channel], rigs[channel]);
        const Vector3 &rest = rigs[channel].restPosition;
        g_faceChannelDeltas[channel] = {target.x - rest.x, target.y - rest.y, target.z - rest.z};
    }
    Matrix4x4::Pop(1);
}



// @verified - p
void faceAccumulateVertexOffsetsFromInfluences(Vector3 *destination, const Vector3 *source, const FtblBlendHeader &blend, std::int32_t vertexCount) {
    const float *const weights = FaceTaskRelative<const float>(blend.weightsOffset);
    const std::int32_t *const indices = FaceTaskRelative<const std::int32_t>(blend.indicesOffset);
    std::int32_t influence = 0;
    for (std::int32_t vertex = 0; vertex < vertexCount; ++vertex) {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        const std::int32_t count = FaceTaskRelative<const std::int32_t>(blend.countsOffset)[vertex];
        for (std::int32_t index = 0; index < count; ++index, ++influence) {
            const Vector3 &delta = g_faceChannelDeltas[indices[influence]];
            const float weight = weights[influence];
            x = x + weight * delta.x;
            y = y + weight * delta.y;
            z = z + weight * delta.z;
        }
        destination[vertex] = {x + source[vertex].x, y + source[vertex].y, z + source[vertex].z};
    }
}



// @verified - p
void FACE_Calc(std::int32_t eyeFrame, std::int32_t mouthFrame, FaceWork &face, const FaceCalcContext &context, const Vector3 *restPositions) {
    const auto *const rigs = reinterpret_cast<const FtblChannelRigRecord *>(face.fieldF0);
    const FtblBlendHeader &blend = *reinterpret_cast<const FtblBlendHeader *>(face.fieldF8);
    const std::int32_t vertexCount = static_cast<std::int32_t>(face.field100);
    FACE_BuildChannelTransformsAndSolve(eyeFrame, context.channelCount, context.channelRigs, rigs, context.program, context.eyeChannels);
    Vector3 *const eyePass = g_faceEyePassPositions;
    faceAccumulateVertexOffsetsFromInfluences(eyePass, restPositions, blend, vertexCount);
    std::uint8_t *const eyeVertexFlags = reinterpret_cast<std::uint8_t *>(face.field110);
    for (std::int16_t index = 0; index < vertexCount; ++index) {
        eyeVertexFlags[index] =
            eyePass[index].x != restPositions[index].x || eyePass[index].y != restPositions[index].y || eyePass[index].z != restPositions[index].z;
    }
    FACE_BuildChannelTransformsAndSolve(mouthFrame, context.channelCount, context.channelRigs, rigs, context.program, context.mouthChannels);
    faceAccumulateVertexOffsetsFromInfluences(reinterpret_cast<Vector3 *>(face.field120), eyePass, blend, vertexCount);
}



// @verified - p
void FACE_PrecomputeBlendDeltasForVariant(FaceWork &face, FaceClipControllerState *clip) {
    auto *const faceState = reinterpret_cast<std::uint8_t *>(&face.field50);
    const std::int8_t commandCount = static_cast<std::int8_t>(faceState[3]);
    FaceBlendState &state = g_faceBlendStates[static_cast<std::uint8_t>(static_cast<std::uint16_t>(face.eyeMode) >> 8U)];
    std::int16_t eyeFrames = 1;
    std::int32_t mouthFrames = 1;
    const bool eyeActive = faceState[1] == 1;
    const bool mouthActive = clip && clip->field29 == 1;
    if (eyeActive) {
        eyeFrames = static_cast<std::int16_t>(face.field58);
        state.eyeBlendFrames = static_cast<float>(eyeFrames);
        state.eyeBlendFrame = 0.0f;
        faceState[1] = 2;
    }
    if (mouthActive) {
        mouthFrames = clip->blendDuration % 1000;
        state.mouthBlendFrame = 0.0f;
        state.mouthBlendFrames = static_cast<float>(mouthFrames);
        clip->field29 = 2;
    }
    state.vertexCount = static_cast<std::int16_t>(face.field100);
    state.vertices = reinterpret_cast<FaceVertexRecord *>(face.field118);
    state.deltas = reinterpret_cast<Vector3 *>(face.field128);
    state.targets = reinterpret_cast<Vector3 *>(face.field120);
    const std::uint8_t *const eyeVertexFlags = reinterpret_cast<const std::uint8_t *>(face.field110);
    for (std::int32_t index = 0; index < state.vertexCount; ++index) {
        const std::uint8_t flag = eyeVertexFlags[index];
        std::int32_t frames = mouthFrames;
        const Vector3 &current = state.vertices[index].position;
        Vector3 &target = state.targets[index];
        Vector3 &delta = state.deltas[index];
        if (flag != 0 || !mouthActive) {
            if (flag != 1 || !eyeActive)
                continue;
            delta = {target.x - current.x, target.y - current.y, target.z - current.z};
            if (commandCount > 1) {
                const float share = static_cast<float>(commandCount);
                delta = {delta.x / share, delta.y / share, delta.z / share};
                target = {current.x + delta.x, current.y + delta.y, current.z + delta.z};
            }
            frames = eyeFrames;
        } else {
            delta = {target.x - current.x, target.y - current.y, target.z - current.z};
        }
        delta = {delta.x / static_cast<float>(frames), delta.y / static_cast<float>(frames), delta.z / static_cast<float>(frames)};
    }
}




// @verified - p
std::uint8_t FACE_AccumulateOrFinalizeBlends(const FaceWork &face, const FaceClipControllerState *clip, FaceBlendState &state) {
    const auto *const faceState = reinterpret_cast<const std::uint8_t *>(&face.field50);
    const std::uint8_t *const eyeVertexFlags = reinterpret_cast<const std::uint8_t *>(face.field110);
    std::uint8_t result = 0;
    if (clip && clip->field29 != 0) {
        state.mouthBlendFrame += g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 30.0f : 1.0f;
        if (state.mouthBlendFrame < state.mouthBlendFrames) {
            for (std::int32_t index = 0; index < state.vertexCount; ++index) {
                if (eyeVertexFlags[index] != 0)
                    continue;
                const float step = ScaleByFrameTime30fps(1.0f);
                Vector3 &current = state.vertices[index].position;
                current.x = step * state.deltas[index].x + current.x;
                current.y = step * state.deltas[index].y + current.y;
                current.z = step * state.deltas[index].z + current.z;
            }
        } else {
            for (std::int32_t index = 0; index < state.vertexCount; ++index) {
                if (eyeVertexFlags[index] == 0)
                    state.vertices[index].position = state.targets[index];
            }
            result = 1;
        }
    } else {
        result = 1;
    }

    if (faceState[1] == 0 || faceState[1] == 3)
        return result | 2U;
    state.eyeBlendFrame += g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 30.0f : 1.0f;
    if (!(state.eyeBlendFrame < state.eyeBlendFrames)) {
        if (static_cast<std::int8_t>(faceState[3]) <= 1) {
            for (std::int32_t index = 0; index < state.vertexCount; ++index) {
                if (eyeVertexFlags[index] == 1)
                    state.vertices[index].position = state.targets[index];
            }
        }
        return result | 2U;
    }
    for (std::int32_t index = 0; index < state.vertexCount; ++index) {
        if (eyeVertexFlags[index] != 1)
            continue;
        const float step = ScaleByFrameTime30fps(1.0f);
        Vector3 &current = state.vertices[index].position;
        current.x = step * state.deltas[index].x + current.x;
        current.y = step * state.deltas[index].y + current.y;
        current.z = step * state.deltas[index].z + current.z;
    }
    return result;
}

Mt5RuntimeNode *faceFindModelNodeByWordRecursive(Mt5RuntimeNode *node, std::uint16_t word) {
    if (!node)
        return nullptr;
    if (!MT5_IsRegisteredModelNode(node))
        return nullptr;
    if (node->nodeKind == word)
        return node;
    std::byte *const base = MT5_ModelBaseForNode(node);
    if (node->childOffset) {
        if (Mt5RuntimeNode *const child = faceFindModelNodeByWordRecursive(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), word)) {
            return child;
        }
    }
    if (node->siblingOffset) {
        return faceFindModelNodeByWordRecursive(reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset), word);
    }
    return nullptr;
}




std::uint8_t facemot(FaceWork &face) {
    const char *const faceName = reinterpret_cast<const char *>(face.faceName);
    const std::uint8_t index = faceFindCacheSlotByModelName(faceName);
    if (index >= 8)
        return index;
    FaceModelCacheSlot &slot = g_faceModelCache[index];
    FaceTableBinding &tables = g_faceTables[index];
    if (slot.state != 0) {
        slot.state = 1;
    } else {
        if (!faceLoadModelIntoCacheSlot(faceName, index))
            return 8;
        std::strcpy(slot.filename, faceName);
        slot.state = 1;
        std::byte *const table = LoadFaceTable(reinterpret_cast<const char *>(face.faceTableName));
        g_faceTableFiles[index] = table;
        FtblPrologue prologue{};
        std::memcpy(&prologue, table, sizeof(prologue));
        tables.channelRigs = reinterpret_cast<const FtblChannelRigRecord *>(FaceTableAddress(table, static_cast<std::int32_t>(prologue.channelBlockOffset)));
        const std::int32_t blendOffset = static_cast<std::int32_t>(prologue.blendHeaderOffset);
        FtblBlendHeader *const blend = reinterpret_cast<FtblBlendHeader *>(FaceTableAddress(table, blendOffset));
        tables.blendHeader = blend;
        if ((static_cast<std::uint32_t>(blendOffset) & 0xffff0000U) != 0) {
            tables.influenceCounts = FaceTaskRelative<const std::int32_t>(blend->countsOffset);
            tables.influenceIndices = FaceTaskRelative<const std::int32_t>(blend->indicesOffset);
            tables.influenceWeights = FaceTaskRelative<const float>(blend->weightsOffset);
        } else {
            tables.influenceCounts = reinterpret_cast<const std::int32_t *>(table + blend->countsOffset);
            tables.influenceIndices = reinterpret_cast<const std::int32_t *>(table + blend->indicesOffset);
            tables.influenceWeights = reinterpret_cast<const float *>(table + blend->weightsOffset);
        }

        Mt5RuntimeNode *const faceNode = faceFindModelNodeByWordRecursive(static_cast<Mt5RuntimeNode *>(slot.modelRoot), 3);
        std::byte *const modelBase = MT5_ModelBaseForNode(faceNode);
        const std::byte *const mesh = modelBase + faceNode->sceneBindingOffset;
        std::int32_t vertexOffset{};
        std::int32_t vertexCount{};
        std::memcpy(&vertexOffset, mesh + 4, sizeof(vertexOffset));
        std::memcpy(&vertexCount, mesh + 8, sizeof(vertexCount));
        const FaceVertexRecord *const vertices = reinterpret_cast<const FaceVertexRecord *>(modelBase + vertexOffset);
        FaceVertexRecord *const copies = g_faceVertexCopies[index];
        Vector3 *const rest = g_faceRestPositions[index];
        Vector3 *const targets = g_faceTargetPositions[index];
        for (std::int32_t vertex = 0; vertex < vertexCount; ++vertex) {
            copies[vertex] = vertices[vertex];
            rest[vertex] = vertices[vertex].position;
            targets[vertex] = vertices[vertex].position;
        }
    }

    face.eyeMode = static_cast<std::int16_t>((static_cast<std::uint16_t>(face.eyeMode) & 0x00ffU) | (static_cast<std::uint16_t>(index) << 8U));
    face.field20 = reinterpret_cast<std::uint64_t>(slot.modelRoot);
    Mt5RuntimeNode *const faceNode = faceFindModelNodeByWordRecursive(static_cast<Mt5RuntimeNode *>(slot.modelRoot), 3);
    face.field28 = reinterpret_cast<std::uint64_t>(faceNode);
    const std::byte *const mesh = MT5_ModelBaseForNode(faceNode) + faceNode->sceneBindingOffset;
    face.fieldF0 = reinterpret_cast<std::uint64_t>(tables.channelRigs);
    FtblBlendHeader *const blend = tables.blendHeader;
    face.fieldF8 = reinterpret_cast<std::int64_t>(blend);
    if ((static_cast<std::uint32_t>(blend->countsOffset) & 0xffff0000U) == 0)
        blend->countsOffset = FaceTaskOffset(tables.influenceCounts);
    if ((static_cast<std::uint32_t>(blend->indicesOffset) & 0xffff0000U) == 0)
        blend->indicesOffset = FaceTaskOffset(tables.influenceIndices);
    if ((static_cast<std::uint32_t>(blend->weightsOffset) & 0xffff0000U) == 0)
        blend->weightsOffset = FaceTaskOffset(tables.influenceWeights);
    const std::byte *const table = g_faceTableFiles[index];
    std::memcpy(&face.field30, table, sizeof(face.field30));
    // The eight random NPC faces take the free-talk program AllocateFACE loaded instead of their table's.
    for (const char *const model : kRandomTalkModels) {
        if (EqualInsensitive(faceName, model)) {
            face.field30 = 4;
            break;
        }
    }
    std::memcpy(&face.fieldC8, table + 0x0c, 3 * sizeof(face.fieldC8));
    std::memcpy(&face.field100, mesh + 8, sizeof(face.field100));
    face.field118 = reinterpret_cast<std::uint64_t>(g_faceVertexCopies[index]);
    face.field120 = reinterpret_cast<std::uint64_t>(g_faceTargetPositions[index]);
    face.field128 = reinterpret_cast<std::uint64_t>(g_faceBlendDeltas[index]);
    face.field110 = reinterpret_cast<std::uint64_t>(g_faceEyeVertexFlags[index]);
    face.field108 = 0;
    Mt5RuntimeNode *const node77 = faceFindModelNodeByWordRecursive(faceNode, 77);
    Mt5RuntimeNode *const node78 = faceFindModelNodeByWordRecursive(faceNode, 78);
    face.field70 = reinterpret_cast<std::uint64_t>(node77 && node77->nodeKind == 77 ? node77 : faceNode);
    face.field78 = reinterpret_cast<std::uint64_t>(node78 && node78->nodeKind == 78 ? node78 : faceNode);
    face.field9c = 0;
    face.fieldA4.y = 0.0f;
    face.fieldA4.z = 0.0f;
    face.field40 = 0;
    face.key = 0;
    return index;
}

FaceClipControllerState *FindClip(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kClipComponent);
    return component ? reinterpret_cast<FaceClipControllerState *>(component + 1) : nullptr;
}

FaceWork *FindFace(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kFaceComponent);
    return component ? reinterpret_cast<FaceWork *>(component + 1) : nullptr;
}

Matrix4x4 MultiplyFaceSkinningMatrices(const Matrix4x4 &left, const Matrix4x4 &right) {
    Matrix4x4 output{};
    const auto *const lhs = reinterpret_cast<const float *>(&left);
    const auto *const rhs = reinterpret_cast<const float *>(&right);
    auto *const result = reinterpret_cast<float *>(&output);
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            float value = lhs[row * 4] * rhs[column];
            for (std::size_t item = 1; item < 4; ++item)
                value += lhs[row * 4 + item] * rhs[item * 4 + column];
            result[row * 4 + column] = value;
        }
    }
    return output;
}




// @verified - p
void faceLookAtSolverComputeTargets(Character *character, FaceLookState &state) {
    if (!character || !GetTransformForFACE_ForSpecificChara(character, 5) || !state.targetNode)
        return;

    if ((state.flags & 0x7f) != 0) {
        (void)matrix_stack::Matrix4x4_Push();
        const Matrix4x4 &transform = *GetTransformForFACE_ForSpecificChara(character, 5);
        workTransformMatrix = transform;
        Matrix4x4::preTranslate(state.targetNode->position.x, state.targetNode->position.y, state.targetNode->position.z);
        Vector3 eye{transform.rows[3].x, workTransformMatrix.rows[3].y, transform.rows[3].z};
        Matrix4x4::Pop(1);
        const std::uint16_t anchorYaw = MOTN_ExtractEulerAngle(MOTM_GetWork(character)->motion, 5);
        const float deltaZ = state.targetPosition.z - eye.z;
        const std::int32_t pitch = Shenmue_Atan2Fixed(eye.y - state.targetPosition.y, std::fabs(deltaZ));
        state.targetPitchA = pitch;
        state.targetPitchB = pitch;
        const std::int32_t yaw = static_cast<std::uint16_t>(Shenmue_Atan2Fixed(-(state.targetPosition.x - transform.rows[3].x), -deltaZ) - anchorYaw);
        state.targetYawA = yaw;
        state.targetYawB = yaw;
        if (g_debugUnknown3 == 0) {
            FACE_UpdateEyeLookAtModelTarget(character->model, 0, &eye, &state.targetPosition);
            FACE_UpdateEyeLookAtModelTarget(character->model, 1, &eye, &state.targetPosition);
        }
    }
    std::int16_t frames = static_cast<std::int16_t>(state.blendFrames - state.blendFrame);
    if (frames < 1)
        frames = 1;

    constexpr std::int32_t kSignExtend16 = static_cast<std::int32_t>(0xffff0000U);
    if ((state.targetPitchA & 0x8000) != 0) {
        state.targetPitchB |= kSignExtend16;
        state.targetPitchA |= kSignExtend16;
    }
    if ((state.targetYawA & 0x8000) != 0)
        state.targetYawA |= kSignExtend16;
    if ((state.targetYawB & 0x8000) != 0)
        state.targetYawB |= kSignExtend16;

    if (state.targetPitchA > state.pitchMaximum) {
        state.targetPitchB = state.pitchMaximum;
        state.targetPitchA = state.pitchMaximum;
    } else if (state.targetPitchA < state.pitchMinimum) {
        state.targetPitchB = state.pitchMinimum;
        state.targetPitchA = state.pitchMinimum;
    }
    if (state.targetYawA > state.yawAMaximum)
        state.targetYawA = state.yawAMaximum;
    else if (state.targetYawA < state.yawAMinimum)
        state.targetYawA = state.yawAMinimum;
    if (state.targetYawB > state.yawBMaximum)
        state.targetYawB = state.yawBMaximum;
    else if (state.targetYawB < state.yawBMinimum)
        state.targetYawB = state.yawBMinimum;

    state.deltaPitchA = (state.targetPitchA - state.currentPitchA) / frames;
    state.deltaYawA = (state.targetYawA - state.currentYawA) / frames;
    state.deltaPitchB = (state.targetPitchB - state.currentPitchB) / frames;
    state.deltaYawB = (state.targetYawB - state.currentYawB) / frames;
}

// One frame of the look-at blend; past the last frame the angles land on their targets and any mode but 2 (follow) ends.
// @verified - p
void faceAdvanceLookAtState(FaceLookState &state) {
    ++state.blendFrame;
    if (state.blendFrame > state.blendFrames) {
        if ((state.flags & 0x7f) != 2)
            state.flags &= 0x7f;
        state.blendFrame = state.blendFrames;
        state.currentPitchA = state.targetPitchA;
        state.currentYawA = state.targetYawA;
        state.currentPitchB = state.targetPitchB;
        state.currentYawB = state.targetYawB;
    } else {
        state.currentPitchA += state.deltaPitchA;
        state.currentYawA += state.deltaYawA;
        state.currentPitchB += state.deltaPitchB;
        state.currentYawB += state.deltaYawB;
    }
    if ((g_faceLookLimitFlag & 1) != 0) {
        if (state.currentPitchA > state.pitchMaximum) {
            state.currentPitchB = state.pitchMaximum;
            state.currentPitchA = state.pitchMaximum;
        } else if (state.currentPitchA < state.pitchMinimum) {
            state.currentPitchB = state.pitchMinimum;
            state.currentPitchA = state.pitchMinimum;
        }
        if (state.currentYawA > state.yawAMaximum)
            state.currentYawA = state.yawAMaximum;
        else if (state.currentYawA < state.yawAMinimum)
            state.currentYawA = state.yawAMinimum;
        if (state.currentYawB > state.yawBMaximum)
            state.currentYawB = state.yawBMaximum;
        else if (state.currentYawB < state.yawBMinimum)
            state.currentYawB = state.yawBMinimum;
    }
}

bool FACE_SelectCanonicalFace(Character *character, FaceWork &face) {
    const std::uint32_t image = IMGM_GetImageName(character) & 0x00ffffffU;
    const char *model = nullptr;
    const char *table = nullptr;
    switch (image) {
    case make_shen_name('Y', 'K', 'B', '\0'):
    case make_shen_name('Y', 'K', 'C', '\0'):
    case make_shen_name('Y', 'K', 'E', '\0'):
        model = "YKB_F.MT5";
        table = "YKB_FTBL.BIN";
        break;
    case make_shen_name('Y', 'K', 'D', '\0'):
    case make_shen_name('Y', 'K', 'F', '\0'):
        model = "YKD_F.MT5";
        table = "YKD_FTBL.BIN";
        break;
    case make_shen_name('F', 'U', 'B', '\0'):
    case make_shen_name('F', 'U', 'K', '\0'):
        model = "FUK_F.MT5";
        table = "FUK_FTBL.BIN";
        break;
    case make_shen_name('F', 'U', 'C', '\0'):
    case make_shen_name('F', 'U', 'D', '\0'):
        model = "FUC_F.MT5";
        table = "FUC_FTBL.BIN";
        break;
    default:
        return false;
    }
    if (EqualInsensitive(reinterpret_cast<const char *>(face.faceName), model))
        return false;
    std::strcpy(reinterpret_cast<char *>(face.faceName), model);
    std::strcpy(reinterpret_cast<char *>(face.faceTableName), table);
    return true;
}

enum class FaceBlendWeightMode : std::int32_t {
    Absolute,
    Additive,
    Subtractive,
};

// @verified - p
void FACE_SetBlendWeightsNormalized(void *model, float eyeWeight, float mouthWeight, float frames, FaceBlendWeightMode mode) {
    std::byte *const entry = ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model));
    if (!entry)
        return;
    float currentEye{};
    float currentMouth{};
    std::memcpy(&currentEye, entry + 0x5c, sizeof(currentEye));
    std::memcpy(&currentMouth, entry + 0x90, sizeof(currentMouth));
    if (mode == FaceBlendWeightMode::Additive) {
        eyeWeight += currentEye;
        mouthWeight += currentMouth;
    } else if (mode == FaceBlendWeightMode::Subtractive) {
        eyeWeight = currentEye - eyeWeight;
        mouthWeight = currentMouth - mouthWeight;
    }
    eyeWeight = std::fmax(0.0f, std::fmin(1.0f, eyeWeight));
    mouthWeight = std::fmax(0.0f, std::fmin(1.0f, mouthWeight));
    if (eyeWeight < 0.1f)
        eyeWeight = 0.0f;
    if (mouthWeight < 0.1f)
        mouthWeight = 0.0f;
    const float duration = frames * (1.0f / 30.0f);
    if (std::fabs(eyeWeight - currentEye) > 0.00000011920929f) {
        std::memcpy(entry + 0x5c, &eyeWeight, sizeof(eyeWeight));
        std::memcpy(entry + 0x60, &duration, sizeof(duration));
        entry[0x64] = std::byte{1};
    }
    if (std::fabs(mouthWeight - currentMouth) > 0.00000011920929f) {
        std::memcpy(entry + 0x90, &mouthWeight, sizeof(mouthWeight));
        std::memcpy(entry + 0x94, &duration, sizeof(duration));
        entry[0x98] = std::byte{1};
    }
}

void FACE_AdvanceBlinkState(Character *character, FaceWork &face) {
    auto *const state = reinterpret_cast<std::uint8_t *>(&face.field50);
    if (state[0] != 0) {
        float weight = 1.0f;
        FaceBlendWeightMode mode = FaceBlendWeightMode::Absolute;
        switch (state[0]) {
        case 1:
            state[0] = 4;
            state[1] = 1;
            state[2] = 1;
            break;
        case 2:
            state[0] = 4;
            state[1] = 1;
            state[2] = 0;
            weight = 0.0f;
            if (state[3] > 1) {
                mode = FaceBlendWeightMode::Subtractive;
                weight = 1.0f / static_cast<float>(state[3]);
            }
            break;
        case 3:
            state[0] = 5;
            state[1] = 1;
            state[2] = 1;
            break;
        default:
            return;
        }
        if (state[2] != 0 && state[3] > 1) {
            mode = FaceBlendWeightMode::Additive;
            weight = 1.0f / static_cast<float>(state[3]);
        }
        FACE_SetBlendWeightsNormalized(
            character ? character->model : nullptr, weight, weight, static_cast<float>(static_cast<std::int16_t>(face.field58)), mode);
        return;
    }
    if (state[1] != 0)
        return;

    float elapsed{};
    std::memcpy(&elapsed, state + 4, sizeof(elapsed));
    const std::uint8_t blinkState = state[2];
    if (blinkState == 0) {
        elapsed += g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 30.0f : 1.0f;
        if (elapsed >= static_cast<float>(static_cast<std::int16_t>(face.field5a))) {
            elapsed = 0.0f;
            g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
            face.field5a = static_cast<std::uint16_t>(10U * (((g_EnvPrngSeed >> 16U) & 3U) + 6U));
        }
        std::memcpy(state + 4, &elapsed, sizeof(elapsed));
    }
    if (elapsed >= 0.00000011920929f || (g_nAuthorState == 20 && g_debugFaceUnknown == 0))
        return;
    state[1] = 1;
    state[2] = static_cast<std::uint8_t>((blinkState - 1U) & 1U);
    face.field58 = state[2] ? 2 : 4;
    FACE_SetBlendWeightsNormalized(character ? character->model : nullptr, static_cast<float>(state[2]), static_cast<float>(state[2]),
        static_cast<float>(static_cast<std::int16_t>(face.field58)), FaceBlendWeightMode::Absolute);
}

void faceUpdateStateMachine(void *model, FaceClipControllerState &clip);
void FaceUpdate(void *hmdl, Mt5RuntimeNode *sourceModel, FaceClipControllerState &clip);

Mt5RuntimeNode *FindTaggedNode(Mt5RuntimeNode *node, std::uint16_t tag) {
    while (node) {
        if (node->nodeKind == tag)
            return node;
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->childOffset) {
            if (Mt5RuntimeNode *const found = FindTaggedNode(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), tag)) {
                return found;
            }
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset) : nullptr;
    }
    return nullptr;
}
Mt5RuntimeNode *FaceModelChild(Mt5RuntimeNode *model) {
    if (!model || !model->childOffset)
        return nullptr;
    return reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(model) + model->childOffset);
}

Mt5RuntimeNode *FindTaggedModelChild(Mt5RuntimeNode *model, std::uint16_t tag) {
    return FindTaggedNode(FaceModelChild(model), tag);
}

Mt5RuntimeNode *FindTaggedSibling(Mt5RuntimeNode *node, std::uint16_t tag) {
    while (node && node->nodeKind != tag) {
        if (!node->siblingOffset)
            return nullptr;
        node = reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(node) + node->siblingOffset);
    }
    return node;
}

// @verified - p
Mt5RuntimeNode *FaceFindMouthClip(Mt5RuntimeNode *model, std::int16_t siblingIndex, std::int16_t variantIndex) {
    if (!model || !(model->nodeKind == 62 || static_cast<std::int16_t>(model->nodeKind) > 0x7000))
        return nullptr;

    Mt5RuntimeNode *variant = FaceModelChild(model);
    std::int16_t index = 0;
    while (variant) {
        if (variant->nodeKind != 0xffbdU) {
            if (!variant->siblingOffset)
                return nullptr;
            variant = reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(variant) + variant->siblingOffset);
            variant = FindTaggedSibling(variant, 0xffbdU);
        }
        if (!variant)
            return nullptr;
        if (index == variantIndex) {
            Mt5RuntimeNode *clip = FaceModelChild(variant);
            if (!clip)
                return nullptr;
            for (std::int16_t sibling = 0; sibling < siblingIndex; ++sibling) {
                if (!clip->siblingOffset)
                    break;
                clip = reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(clip) + clip->siblingOffset);
            }
            return clip;
        }
        if (!variant->siblingOffset || ++index >= 10)
            return nullptr;
        variant = reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(variant) + variant->siblingOffset);
    }
    return nullptr;
}



// @verified - p
bool FaceBindToChannel(FaceClipControllerState &clip, Mt5RuntimeNode *model) {
    clip.channelTag = static_cast<std::uint16_t>(-68);
    Mt5RuntimeNode *const channelNode = FindTaggedModelChild(model, 0xffbcU);
    std::byte *base = nullptr;
    auto *const binding = static_cast<std::byte *>(MT5_SceneBindingForNode(channelNode, &base));

    std::int16_t count{};
    std::int32_t vertexOffset{};
    std::memcpy(&vertexOffset, binding + 4, sizeof(vertexOffset));
    std::memcpy(&count, binding + 8, sizeof(count));
    auto *const source = reinterpret_cast<const FaceVertexRecord *>(base + vertexOffset);

    std::uint16_t index = 0;
    while (g_kutiScratchBanks[index].allocated != 0) {
        if (++index >= g_kutiScratchBanks.size()) {
            clip.channelIndex = 3;
            return false;
        }
    }
    KutiScratchBank &bank = g_kutiScratchBanks[index];
    bank.allocated = 1;
    clip.channelIndex = index;
    bank.vertexCount = count;
    clip.channelVertices = const_cast<FaceVertexRecord *>(source);
    auto *const current = static_cast<FaceVertexRecord *>(bank.pools[0]);
    for (std::int16_t vertex = 0; vertex < count; ++vertex)
        current[vertex] = source[vertex];
    return true;
}



// @verified - p
void FacePrecomputeDeltas(FaceClipControllerState &clip, std::int16_t frameCount) {
    if (static_cast<std::int16_t>(clip.channelIndex) >= 3)
        return;
    KutiScratchBank &bank = g_kutiScratchBanks[clip.channelIndex];
    auto *const clipNode = static_cast<Mt5RuntimeNode *>(clip.clip);
    std::byte *base = nullptr;
    auto *const binding = static_cast<std::byte *>(MT5_SceneBindingForNode(clipNode, &base));
    std::int32_t targetOffset{};
    std::memcpy(&targetOffset, binding + 4, sizeof(targetOffset));
    const auto *const source = reinterpret_cast<const FaceVertexRecord *>(base + targetOffset);
    const std::int16_t duration = static_cast<std::int16_t>(frameCount % 1000);
    bank.frameCount = duration;
    bank.frame = 0;
    const auto *const current = static_cast<const FaceVertexRecord *>(bank.pools[0]);
    auto *const delta = static_cast<Vector3 *>(bank.pools[1]);
    auto *const target = static_cast<Vector3 *>(bank.pools[2]);
    const float divisor = static_cast<float>(duration);
    for (std::int16_t index = 0; index < bank.vertexCount; ++index) {
        target[index] = source[index].position;
        delta[index].x = (target[index].x - current[index].position.x) / divisor;
        delta[index].y = (target[index].y - current[index].position.y) / divisor;
        delta[index].z = (target[index].z - current[index].position.z) / divisor;
    }
}


// @verified - p
bool FaceUpdateDeltaXYZ(KutiScratchBank &bank) {
    auto *const current = static_cast<FaceVertexRecord *>(bank.pools[0]);
    const auto *const delta = static_cast<const Vector3 *>(bank.pools[1]);
    const auto *const target = static_cast<const Vector3 *>(bank.pools[2]);
    if (++bank.frame < bank.frameCount) {
        for (std::int16_t index = 0; index < bank.vertexCount; ++index) {
            current[index].position.x = delta[index].x + current[index].position.x;
            current[index].position.y = delta[index].y + current[index].position.y;
            current[index].position.z = delta[index].z + current[index].position.z;
        }
        return false;
    }
    for (std::int16_t index = 0; index < bank.vertexCount; ++index)
        current[index].position = target[index];
    return true;
}


// @verified - p
void FaceAttach(FaceClipControllerState &clip, Mt5RuntimeNode *model) {
    const std::uint16_t channelIndex = clip.channelIndex;
    if (static_cast<std::int16_t>(channelIndex) >= 3)
        return;
    clip.channelIndex = 3;
    g_kutiScratchBanks[channelIndex].allocated = 0;
    Mt5RuntimeNode *const channelNode = FindTaggedModelChild(model, clip.channelTag);
    std::byte *base = nullptr;
    auto *const binding = static_cast<std::byte *>(MT5_SceneBindingForNode(channelNode, &base));
    std::int32_t relative = 0;
    if (clip.channelVertices) {
        const std::ptrdiff_t offset = static_cast<std::byte *>(clip.channelVertices) - base;
        relative = static_cast<std::int32_t>(offset);
    }
    std::memcpy(binding + 4, &relative, sizeof(relative));
    clip.clip = nullptr;
    clip.activeLipCues = nullptr;
    clip.activeSequenceState = 0;
    clip.morphComplete = 1;
}

void FACE_InitializeWork(FaceWork &face) {



    const std::uint32_t frameState = 0x08000000U;
    std::memcpy(reinterpret_cast<std::byte *>(&face) + 0x36, &frameState, sizeof(frameState));
    face.field20 = 0;
    face.field28 = 0;
    face.field3a = 0;
    face.field30 = 1;
    face.eyePatternMode = 0;
    face.field40 = 0;
    face.key = 0;
    face.fieldE8 = 129;
    face.field58 = 3;
    face.field50 = 0;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const std::int16_t blinkUnit = static_cast<std::int16_t>(((g_EnvPrngSeed >> 16U) & 3U) + 6U);
    face.field5a = static_cast<std::uint16_t>(10 * blinkUnit);
    face.field5c = 0x10000U;
    face.field60 = 1;
    face.field64 = 0;
    face.field6c = 0;
    face.field70 = 0;
    face.field78 = 0;
    face.field84 = 0;
    face.field90 = 0;
    face.field9c = 0;
    face.fieldA4.y = 0.0f;
    face.fieldA4.z = 0.0f;
    face.fieldB4 = 0;
    face.fieldC0 = 0;
    face.fieldC8 = 0;
    face.fieldD0 = 0;
    face.fieldD8 = 0;
    face.fieldE0 = 0;
}

// @verified - p
void CalcFACE(Character *character, void *payload) {



    if (!character)
        return;
    auto *const clip = static_cast<FaceClipControllerState *>(payload);
    ImageModuleWork *const image = IMGM_GetWork(character);
    if (!image || !image->images)
        return;
    MdlmImageSlot *const slot = image->images[image->requestedImage];
    PackModel *const displayed = slot ? slot->selectedModel : nullptr;
    if (!displayed)
        return;

    MotionManagerWork *const motion = MOTM_GetWork(character);
    if (motion && (motion->motion.poseEvaluationStage == 2 || motion->motion.poseEvaluationStage == 3) && (character->motionFlags & 0x10U) == 0) {
        FaceAttach(*clip, static_cast<Mt5RuntimeNode *>(displayed->model));
        return;
    }

    FaceWork *const face = FindFace(character);
    if (face && static_cast<std::uint8_t>(face->eyeMode) != 0) {
        faceUpdateStateMachine(character->model, *clip);
    } else if (!VFMA_IsFighting()) {
        FaceUpdate(character->model, static_cast<Mt5RuntimeNode *>(displayed->model), *clip);
    }

    if (clip->field54 == 1) {
        if (++clip->field42 < static_cast<std::int16_t>(clip->field44)) {
            const auto step = static_cast<std::int32_t>(static_cast<std::uint64_t>(clip->field4c) >> 32U);
            clip->rotationY += static_cast<std::uint32_t>(step);
        } else {
            clip->rotationY = static_cast<std::uint32_t>(clip->field4c);
            clip->field54 = 2;
        }
    }
}

void faceEntry(Character *character) {
    FaceWork *const face = FindFace(character);
    FaceClipControllerState *const clip = FindClip(character);
    KUTI_BeginFaceHandoff(face, clip);
    if (character->model)
        clip->characterFaceHandler(character);
    KUTI_EndFaceHandoff();
}

void FACE_Callback(Character *character, void *payload) {
    auto *const face = static_cast<FaceWork *>(payload);
    if (FACE_SelectCanonicalFace(character, *face))
        (void)facemot(*face);
    std::uint8_t cacheIndex = static_cast<std::uint8_t>(static_cast<std::uint16_t>(face->eyeMode) >> 8U);
    if (static_cast<std::uint8_t>(face->eyeMode) != 0 && face->field20 != 0 &&
        (cacheIndex >= 8 || reinterpret_cast<void *>(face->field20) != g_faceModelCache[cacheIndex].modelRoot)) {
        face->eyeMode = static_cast<std::int16_t>(static_cast<std::uint16_t>(face->eyeMode) & 0xff00U);
    }
    cacheIndex = static_cast<std::uint8_t>(static_cast<std::uint16_t>(face->eyeMode) >> 8U);
    if (static_cast<std::uint8_t>(face->eyeMode) == 0 || face->field20 == 0 || cacheIndex >= 8 ||
        reinterpret_cast<void *>(face->field20) != g_faceModelCache[cacheIndex].modelRoot) {
        if (FaceClipControllerState *const invalidClip = FindClip(character))
            invalidClip->field29 = 0;
        reinterpret_cast<std::uint8_t *>(&face->field50)[1] = 0;
        return;
    }

    FaceClipControllerState *const clip = FindClip(character);
    auto *const faceState = reinterpret_cast<std::uint8_t *>(&face->field50);
    auto &look = *reinterpret_cast<FaceLookState *>(&face->field50);
    const std::uint8_t lookMode = static_cast<std::uint8_t>(look.flags) & 0x7fU;
    if (lookMode == 2) {
        faceLookAtSolverComputeTargets(character, look);
    } else if (lookMode == 1) {
        const std::uint16_t targetSlot = g_activeDialogueSlot;
        if (targetSlot == 129) {
            face->fieldE8 = 129;
        } else if (face->fieldE8 == targetSlot) {
            faceLookAtSolverComputeTargets(character, look);
        } else {
            face->fieldE8 = targetSlot;
            if (clip && targetSlot != clip->field1a) {
                look.flags = static_cast<std::int8_t>(0x81);
                look.blendFrames = 8;
                look.blendFrame = 0;
                look.targetPosition = g_activeDialoguePosition;
                faceLookAtSolverComputeTargets(character, look);
            }
        }
    }
    if (look.flags < 0)
        faceAdvanceLookAtState(look);
    FACE_AdvanceBlinkState(character, *face);
    if ((clip && clip->field29 == 1) || faceState[1] == 1) {
        const auto *const eyeFrames = reinterpret_cast<const std::int16_t *>(&face->field5c);
        const std::int32_t eyeFrame = face->eyePatternMode + eyeFrames[faceState[2] & 1U];
        const std::int32_t mouthFrame = face->eyePatternMode + (clip ? (clip->alternatingShape ? clip->visemeIndex : clip->field32) : 0);
        const FaceMotionProgram &motion = g_faceMotionPrograms[face->field30];
        g_faceCalcContext.channelRigs = g_faceTables[cacheIndex].channelRigs;
        g_faceCalcContext.program = motion.program;
        g_faceCalcContext.eyeChannels = motion.eyeChannels;
        g_faceCalcContext.blendFrameCount = 12;
        g_faceCalcContext.mouthChannels = motion.mouthChannels;
        face->frameCount = 4;
        FACE_Calc(eyeFrame, mouthFrame, *face, g_faceCalcContext, g_faceRestPositions[cacheIndex]);
        FACE_PrecomputeBlendDeltasForVariant(*face, clip);
    }
    FaceBlendState &blendState = g_faceBlendStates[cacheIndex];
    if (blendState.vertices && (faceState[1] == 2 || (clip && clip->field29 == 2))) {
        const std::uint8_t blends = FACE_AccumulateOrFinalizeBlends(*face, clip, blendState);
        if ((blends & 1U) != 0 && clip)
            clip->field29 = 0;
        if ((blends & 2U) != 0)
            faceState[1] = static_cast<std::int8_t>(faceState[3]) > 1 ? 3 : 0;
    }
}



// @verified - p
void faceUpdateStateMachine(void *model, FaceClipControllerState &clip) {
    faceBindNewPhoneme(&clip);
    if (clip.activeLipCues && clip.halfDuration != 0xffffU)
        faceAdvanceSequence(&clip);

    switch (clip.startState) {
    case 0:
        if ((clip.field18 >> 8U) != 0) {
            if (clip.field29 == 0) {
                clip.field29 = 1;
                clip.alternatingShape = static_cast<std::uint8_t>((clip.alternatingShape - 1U) & 1U);
                clip.blendDuration = 4;
            }
        } else if (clip.alternatingShape != 0) {
            clip.field29 = 1;
            clip.alternatingShape = 0;
            clip.blendDuration = 8;
        }
        if (!clip.enabled)
            return;
        faceApplyMorph(model, clip.alternatingShape ? clip.visemeIndex : clip.field32, static_cast<float>(clip.blendDuration), 1.0f);
        break;
    case 1:
        clip.startState = 3;
        clip.field29 = 1;
        clip.alternatingShape = 0;
        if (clip.enabled) {
            faceApplyMorph(model, clip.field32, static_cast<float>(clip.blendDuration), 1.0f);
        }
        break;
    case 2:
        clip.startState = 3;
        clip.field29 = 1;
        clip.alternatingShape = 1;
        if (clip.enabled) {
            faceApplyMorph(model, clip.visemeIndex, static_cast<float>(clip.blendDuration), 1.0f);
        }
        break;
    default:
        break;
    }
}



// @verified - p
void FaceUpdate(void *hmdl, Mt5RuntimeNode *sourceModel, FaceClipControllerState &clip) {
    faceBindNewPhoneme(&clip);
    if (clip.activeLipCues && clip.halfDuration != 0xffffU)
        faceAdvanceSequence(&clip);

    bool selectedClip = false;
    switch (clip.startState) {
    case 0:
        if ((clip.field18 >> 8U) == 0 || clip.morphComplete == 0)
            break;
        clip.blendDuration = 4;
        clip.alternatingShape = static_cast<std::uint8_t>((clip.alternatingShape - 1U) & 1U);
        clip.clip = FaceFindMouthClip(sourceModel, clip.alternatingShape, static_cast<std::int8_t>(clip.reserved55));
        selectedClip = clip.clip != nullptr;
        break;
    case 1:
        clip.startState = 3;
        if (clip.alternatingShape == 0)
            break;
        clip.alternatingShape = 0;
        clip.blendDuration = 8;
        clip.clip = FaceFindMouthClip(sourceModel, 0, 0);
        if (clip.enabled)
            faceApplyMorph(hmdl, 0, 8.0f, 1.0f);
        selectedClip = clip.clip != nullptr;
        break;
    case 2:
        clip.startState = 3;
        if (clip.morphComplete == 0)
            break;
        clip.blendDuration = 4;
        if (clip.visemeIndex != 0) {
            clip.alternatingShape = static_cast<std::uint8_t>((clip.alternatingShape - 1U) & 1U);
        } else {
            clip.alternatingShape = 0;
            clip.blendDuration = 8;
        }
        clip.clip = FaceFindMouthClip(sourceModel, clip.alternatingShape, static_cast<std::int8_t>(clip.reserved55));
        if (clip.enabled) {
            const std::int32_t viseme = clip.alternatingShape ? (g_debugFaceUnknown != 0 ? g_debugFaceUnknown : clip.visemeIndex) : 0;
            faceApplyMorph(hmdl, viseme, static_cast<float>(clip.blendDuration), 1.0f);
        }
        selectedClip = clip.clip != nullptr;
        break;
    default:
        break;
    }

    if (selectedClip && FindTaggedModelChild(sourceModel, 0xffbcU)) {
        if (clip.channelIndex == 3)
            FaceBindToChannel(clip, sourceModel);
        FacePrecomputeDeltas(clip, clip.blendDuration);
        clip.morphComplete = 0;
    }

    if (static_cast<std::int16_t>(clip.channelIndex) >= 3)
        return;
    if (!clip.morphComplete)
        clip.morphComplete = FaceUpdateDeltaXYZ(g_kutiScratchBanks[clip.channelIndex]) ? 1 : 0;
    if ((clip.field18 >> 8U) == 0 && !clip.activeLipCues) {
        if (clip.alternatingShape != 0) {
            clip.blendDuration = 8;
            clip.alternatingShape = 0;
            clip.clip = FaceFindMouthClip(sourceModel, 0, 0);
            if (clip.clip) {
                FacePrecomputeDeltas(clip, 8);
                clip.morphComplete = 0;
            }
        } else if (clip.morphComplete && static_cast<std::int16_t>(clip.channelIndex) < 3) {
            FaceAttach(clip, sourceModel);
        }
    }
}



// @verified - p
void FACE_UpdateModelChannel(void *model, std::int32_t eventId, std::uint32_t channel, float durationFrames, float weight) {
    if (durationFrames >= 1000.0f) {
        if (durationFrames < 2000.0f)
            return;
        durationFrames -= 2000.0f;
    }
    const std::uintptr_t handle = reinterpret_cast<std::uintptr_t>(model);
    std::byte *const entry = ResolveHmdlEntry(handle);
    if (!entry)
        return;
    if (channel == 0) {
        isValidCEWP(model);
    } else if (channel >= 2) {
        FACE_UpdateModelChannel(model, eventId, 0, durationFrames, weight);
        FACE_UpdateModelChannel(model, eventId, 1, durationFrames, weight);
        return;
    }

    std::byte *const channelEntry = entry + 0x20 + 20 * channel;
    std::int32_t currentEvent{};
    float currentWeight{};
    std::memcpy(&currentEvent, channelEntry, sizeof(currentEvent));
    std::memcpy(&currentWeight, channelEntry + 4, sizeof(currentWeight));
    if (currentEvent == eventId && std::fabs(currentWeight - weight) <= 0.01f)
        return;
    const float duration = durationFrames * (1.0f / 30.0f);
    const std::uint32_t zero = 0;
    std::memcpy(channelEntry, &eventId, sizeof(eventId));
    std::memcpy(channelEntry + 4, &weight, sizeof(weight));
    std::memcpy(channelEntry + 8, &duration, sizeof(duration));
    std::memcpy(channelEntry + 12, &zero, sizeof(zero));
    channelEntry[16] = std::byte{1};
    if (channel == 1)
        entry[0x58] = std::byte{1};
}

} // namespace



// @verified - p
void HMDL_InitCEWP(RenderArrayEntry &entry) {
    std::uint8_t &initialized = *reinterpret_cast<std::uint8_t *>(&entry.cewpInitialized);
    if (initialized)
        return;
    initialized = 1;
    void *const model = reinterpret_cast<void *>(entry.modelData);
    if (ResolveHmdlEntry(entry.modelData)) {
        FACE_UpdateModelChannel(model, 0, 0, 0.0f, 1.0f);
        FACE_UpdateModelChannel(model, 0, 1, 0.0f, 1.0f);
    }
    std::byte *const resolved = ResolveHmdlEntry(entry.modelData);
    if (!resolved)
        return;
    FaceHmdlExpressionSlot &mouth = reinterpret_cast<FaceHmdlExpressionSlot *>(resolved + 0x20)[2];
    if (!mouth.index && !(std::fabs(mouth.blend - 1.0f) > 0.0099999998f))
        return;
    mouth.blend = 1.0f;
    mouth.fadeSeconds = 0.0f;
    mouth.index = 0;
    mouth.timer = 0.0f;
    mouth.pending = 1;
    isValidCEWP(model);
}



void FACE_Init() {
    std::snprintf(g_faceDirectory.data(), g_faceDirectory.size(), "scene/%02d/model/face", g_sceneId);
    g_faceCalcContext.channelRigs = nullptr;
    g_faceCalcContext.blendFrameCount = 12;
    g_faceCalcContext.channelCount = static_cast<std::int32_t>(kFaceRigChannelCount);
    g_faceLookLimitFlag = 1;
    g_faceCalcContext.eyeChannels = nullptr;
    g_faceCalcContext.mouthChannels = nullptr;
    std::uint32_t hgmlSize = 0;
    const bool hgml = MAPINFO_FindChunkPayload(SHEN_NAME(HGML), &hgmlSize) != nullptr;
    const std::size_t slotCount = hgml ? 2 : 8;
    const std::int64_t vertexCount = hgml ? 5000 : 400;
    for (std::size_t index = 0; index < slotCount; ++index) {
        const char slot = static_cast<char>('0' + index);
        g_faceVertexCopies[index] = static_cast<FaceVertexRecord *>(AllocateProtectedFaceBlock(vertexCount * 24, make_shen_name('F', 'F', slot, '0')));
        g_faceRestPositions[index] = static_cast<Vector3 *>(AllocateProtectedFaceBlock(vertexCount * 12, make_shen_name('F', 'F', slot, '1')));
        g_faceTargetPositions[index] = static_cast<Vector3 *>(AllocateProtectedFaceBlock(vertexCount * 12, make_shen_name('F', 'F', slot, '2')));
        g_faceBlendDeltas[index] = static_cast<Vector3 *>(AllocateProtectedFaceBlock(vertexCount * 12, make_shen_name('F', 'F', slot, '3')));
        g_faceEyeVertexFlags[index] = static_cast<std::uint8_t *>(AllocateProtectedFaceBlock(vertexCount, make_shen_name('F', 'F', slot, '4')));
    }
    g_faceEyePassPositions = static_cast<Vector3 *>(AllocateProtectedFaceBlock(hgml ? 7500 * 12 : 600 * 12, SHEN_NAME(FF80)));
    g_faceTableFiles = {};
    g_faceTables = {};
    g_faceBlendStates = {};
    std::byte *program = static_cast<std::byte *>(g_FMOT);
    g_faceMotionProgramCount = 4;
    for (std::size_t index = 0; index < 4; ++index) {
        const FmotProgramHeader *const header = reinterpret_cast<const FmotProgramHeader *>(program);
        g_faceMotionPrograms[index] = {.program = program,
            .keyRecordCount = header->keyRecordCount,
            .eyeChannels = reinterpret_cast<const FmotChannelDescriptor *>(program + header->eyeChannelTableOffset),
            .mouthChannels = reinterpret_cast<const FmotChannelDescriptor *>(program + header->mouthChannelTableOffset)};
        program += header->nextProgramOffset;
    }
    EnsureFaceTalkBank();
}



// @verified - p
void FACE_UpdateEyeLookAtModelTarget(void *model, std::uint32_t channel, const Vector3 *origin, const Vector3 *target) {
    if (channel >= 2)
        return;
    std::byte *const entry = ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model));
    if (!entry)
        return;
    const std::uint64_t *const headRigNodes = reinterpret_cast<const RenderArrayEntry *>(entry)->headRigNodes;
    if (!headRigNodes)
        return;
    const std::uint64_t handle = headRigNodes[channel == 0 ? 1 : 4];
    const std::uint64_t parentHandle = GetHashForSkinningBuffer(handle);
    if (static_cast<std::uint32_t>(handle) == 0 || static_cast<std::uint32_t>(handle >> 32U) == 0 || static_cast<std::uint32_t>(parentHandle) == 0 ||
        static_cast<std::uint32_t>(parentHandle >> 32U) == 0) {
        return;
    }

    auto &state = *reinterpret_cast<FaceHmdlChannelRuntime *>(entry + (channel == 0 ? 0x5c : 0x90));
    state.origin = {};
    state.target = {};
    if (origin)
        state.origin = *origin;
    constexpr float kEpsilon = 0.00000011920929f;
    if (!target || (std::fabs(target->x) <= kEpsilon && std::fabs(target->y) <= kEpsilon && std::fabs(target->z) <= kEpsilon)) {
        state.targetYaw = 0.0f;
        state.targetPitch = 0.0f;
        return;
    }
    state.target = *target;

    Matrix4x4 local{};
    (void)LookupSkinningTransform(handle, local);
    Matrix4x4 originTransform{{
        {1.0f, 0.0f, 0.0f, local.rows[0].w},
        {0.0f, 1.0f, 0.0f, local.rows[1].w},
        {0.0f, 0.0f, 1.0f, local.rows[2].w},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    originTransform = MultiplyFaceSkinningMatrices(ComputeGlobalSkinningTransform(parentHandle), originTransform);
    if (origin && g_debugUnknown2 != 0) {
        originTransform.rows[0].w = origin->x;
        originTransform.rows[1].w = origin->y;
        originTransform.rows[2].w = origin->z;
    }
    const Matrix4x4 targetTransform{{
        {1.0f, 0.0f, 0.0f, target->x},
        {0.0f, 1.0f, 0.0f, target->y},
        {0.0f, 0.0f, 1.0f, target->z},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    Matrix4x4 inverseOrigin{};
    const Matrix4x4 relative = MultiplyFaceSkinningMatrices(*matrix_stack::createTransformedMatrix(&originTransform, &inverseOrigin), targetTransform);
    Vector3 direction{relative.rows[0].w, relative.rows[1].w, relative.rows[2].w};
    const float inverseLength = 1.0f / std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    direction.x *= inverseLength;
    direction.y *= inverseLength;
    direction.z *= inverseLength;
    state.targetYaw = std::atan2(direction.z, direction.y);
    state.targetPitch = -std::asin(direction.x);
    if (g_debugUnknown3 == 0) {
        state.targetYaw = std::fmax(-g_eyeRigParameters.yawLimit, std::fmin(g_eyeRigParameters.yawLimit, state.targetYaw));
        state.targetPitch = std::fmax(-g_eyeRigParameters.pitchLimit, std::fmin(g_eyeRigParameters.pitchLimit, state.targetPitch));
    }
}




void HMDL_PerformBlink(RenderArrayEntry &entry, std::int32_t side) {
    const std::uint64_t *const nodes = entry.headRigNodes;
    if (!nodes)
        return;
    const std::uint64_t eye = nodes[side ? 4 : 1];
    const std::uint64_t lid = nodes[side ? 3 : 0];
    const std::uint64_t lidOther = nodes[side ? 5 : 2];
    FaceHmdlChannelRuntime &channel = *reinterpret_cast<FaceHmdlChannelRuntime *>(reinterpret_cast<std::byte *>(&entry) + (side ? 0x90 : 0x5c));
    if (eye >> 32U) {
        const float frameTime = g_bEnableFrameTimeSmoothing ? fFrameTimeMs : 0.033333335f;
        channel.yaw = (channel.targetYaw - channel.yaw) * g_blinkDeltaStepTime * frameTime + channel.yaw;
        channel.pitch = (channel.targetPitch - channel.pitch) * g_blinkDeltaStepTime * frameTime + channel.pitch;
        Matrix4x4 matrix{};
        static_cast<void>(LookupSkinningTransform(eye, matrix));
        const Vector3 position{matrix.rows[0].w, matrix.rows[1].w, matrix.rows[2].w};
        matrix_stack::RotateMatrixByAngles(&matrix, channel.yaw, 0.0f, channel.pitch);
        matrix.rows[0].w = position.x;
        matrix.rows[1].w = position.y;
        matrix.rows[2].w = position.z;
        static_cast<void>(AppendHashForSkinningBuffer(eye, matrix));
    }
    const float lidPitch = g_eyeRigParameters.lidPitchScale * channel.pitch;
    const float lidYaw = g_eyeRigParameters.lidYawScale * channel.yaw;
    if (!lid || !(lid >> 32U) || !lidOther || !(lidOther >> 32U))
        return;
    Matrix4x4 matrix{};
    static_cast<void>(LookupSkinningTransform(lid, matrix));
    const Vector3 position{matrix.rows[0].w, matrix.rows[1].w, matrix.rows[2].w};
    matrix_stack::RotateMatrixByAngles(&matrix, lidYaw, 0.0f, lidPitch);
    matrix.rows[0].w = position.x;
    matrix.rows[1].w = position.y;
    matrix.rows[2].w = position.z;
    static_cast<void>(AppendHashForSkinningBuffer(lid, matrix));
    static_cast<void>(AppendHashForSkinningBuffer(lidOther, matrix));
}



void HMDL_PerformFacialExpression(RenderArrayEntry &entry, const std::uint32_t (&masks)[6], std::uint32_t eyeMode) {
    FaceHmdlChannelRuntime &eye = *reinterpret_cast<FaceHmdlChannelRuntime *>(reinterpret_cast<std::byte *>(&entry) + (eyeMode == 3 ? 0x5c : 0x90));
    if (!eye.blinkPending)
        return;
    const FaceHmdlExpressionSlot *const slots = reinterpret_cast<const FaceHmdlExpressionSlot *>(reinterpret_cast<std::byte *>(&entry) + 0x20);
    char name[128]{};
    if (eye.blinkWeight <= 0.0f) {
        std::snprintf(name, sizeof(name), "SM1Exp_%s-%s", kFacialExpressions[slots[0].index < 13 ? slots[0].index : 0],
            kMouthRigs[slots[2].index < 6 ? slots[2].index : 0]);
    } else if (eyeMode == 3) {
        std::snprintf(name, sizeof(name), "%s", "Exp_LeftBlink");
    } else if (eyeMode == 4) {
        std::snprintf(name, sizeof(name), "%s", "Exp_RightBlink");
    } else {
        return;
    }
    const FacialExpressionRequest request{.name = name, .targetMask = masks[eyeMode], .blend = eye.blinkWeight, .fadeSeconds = eye.blinkTime};
    static_cast<void>(StartFacialExpressionWork(entry.handle, &request));
    eye.blinkPending = 0;
}





void HMDL_PerformFacialExpressions(RenderArrayEntry &entry) {
    std::uint32_t modelType = 0;
    std::memcpy(&modelType, entry.rigState, sizeof(modelType));
    if (modelType == 500)
        return;
    std::byte *const base = reinterpret_cast<std::byte *>(&entry);
    FaceHmdlExpressionSlot *const slots = reinterpret_cast<FaceHmdlExpressionSlot *>(base + 0x20);
    FaceHmdlChannelRuntime *const eyes = reinterpret_cast<FaceHmdlChannelRuntime *>(base + 0x5c);
    const float frameTime = g_bEnableFrameTimeSmoothing ? fFrameTimeMs : 0.033333335f;
    slots[0].timer += frameTime;
    slots[1].timer += frameTime;
    slots[2].timer += frameTime;
    std::uint32_t masks[6]{};
    for (std::size_t group = 0; group < 6; ++group)
        masks[group] = FindFacialTargetMask(entry.handle, kFaceTargetGroups[group]);

    char name[128]{};
    if (slots[0].pending) {
        std::snprintf(name, sizeof(name), "SM1Exp_%s-%s", kFacialExpressions[slots[0].index < 13 ? slots[0].index : 0],
            kMouthRigs[slots[2].index < 6 ? slots[2].index : 0]);
        FacialExpressionRequest request{.name = name, .targetMask = masks[0], .blend = slots[0].blend, .fadeSeconds = slots[0].fadeSeconds};
        if (eyes[0].blinkWeight <= 1.1920929e-07f) {
            request.targetMask |= masks[3];
            eyes[0].blinkPending = 0;
        }
        if (eyes[1].blinkWeight <= 1.1920929e-07f) {
            request.targetMask |= masks[4];
            eyes[1].blinkPending = 0;
        }
        if (slots[0].index == slots[1].index && slots[0].blend == slots[1].blend) {
            if (slots[1].pending) {
                if (!(request.fadeSeconds >= slots[1].fadeSeconds))
                    request.fadeSeconds = slots[1].fadeSeconds;
                request.targetMask |= masks[1];
                slots[1].pending = 0;
            }
            if (slots[2].pending && slots[1].blend == slots[2].blend) {
                if (!(request.fadeSeconds >= slots[2].fadeSeconds))
                    request.fadeSeconds = slots[2].fadeSeconds;
                request.targetMask |= masks[5];
                slots[2].pending = 0;
            }
        }
        static_cast<void>(StartFacialExpressionWork(entry.handle, &request));
        slots[0].pending = 0;
    }
    if (slots[1].pending) {
        std::snprintf(name, sizeof(name), "SM1Exp_%s-%s", kFacialExpressions[slots[1].index < 13 ? slots[1].index : 0],
            kMouthRigs[slots[2].index < 6 ? slots[2].index : 0]);
        FacialExpressionRequest request{.name = name, .targetMask = masks[1], .blend = slots[1].blend, .fadeSeconds = slots[1].fadeSeconds};
        if (slots[2].pending && slots[1].blend == slots[2].blend) {
            if (!(request.fadeSeconds >= slots[2].fadeSeconds))
                request.fadeSeconds = slots[2].fadeSeconds;
            slots[2].pending = 0;
            request.targetMask = masks[5] | masks[1];
        }
        static_cast<void>(StartFacialExpressionWork(entry.handle, &request));
        slots[1].pending = 0;
    }
    if (slots[2].pending) {
        std::snprintf(name, sizeof(name), "SM1Exp_%s-%s", kFacialExpressions[slots[1].index < 13 ? slots[1].index : 0],
            kMouthRigs[slots[2].index < 6 ? slots[2].index : 0]);
        const FacialExpressionRequest request{.name = name, .targetMask = masks[5], .blend = slots[2].blend, .fadeSeconds = slots[2].fadeSeconds};
        static_cast<void>(StartFacialExpressionWork(entry.handle, &request));
        slots[2].pending = 0;
    }
    HMDL_PerformFacialExpression(entry, masks, 3);
    HMDL_PerformFacialExpression(entry, masks, 4);
}

std::uint8_t FACE_FindCacheSlotByModelName(const char *filename) {
    return faceFindCacheSlotByModelName(filename);
}

FaceClipControllerState *CLIP_GetWork(Character *character) {
    return FindClip(character);
}

void FACE_ConfigureClip(Character *character, std::int8_t mode, bool flag) {
    FaceClipControllerState *const clip = CLIP_GetWork(character);
    if (!clip)
        return;
    if (mode != 77)
        clip->reserved55 = static_cast<std::uint8_t>(std::clamp<std::int32_t>(mode, 0, 9));
    clip->startState = 0;
    clip->field18 = static_cast<std::uint16_t>((clip->field18 & 0x00ffU) | (static_cast<std::uint16_t>(flag) << 8U));
}

FaceWork *FACE_GetWork(Character *character) {
    return FindFace(character);
}
const void *FACE_GetChannelVertices(std::uint16_t channelIndex) {
    if (channelIndex >= g_kutiScratchBanks.size())
        return nullptr;
    const KutiScratchBank &bank = g_kutiScratchBanks[channelIndex];
    return bank.allocated ? bank.pools[0] : nullptr;
}

void FACE_ResetCache() {
    for (std::size_t index = 0; index < std::size(g_faceModelCache); ++index) {
        FaceModelCacheSlot &slot = g_faceModelCache[index];
        if (slot.modelRoot)
            ReleaseFaceCacheReference(index, slot.filename);
        slot.modelRoot = nullptr;
        slot.modelOwner = nullptr;
        slot.state = 0;
        std::memcpy(slot.filename, "NULL", 5);
    }
}



// @verified - p
void faceApplyMorph(void *model, std::int32_t morphIndex, float speed, float weight) {
    if (speed >= 1000.0f) {
        if (speed < 2000.0f)
            return;
        speed -= 2000.0f;
    }
    std::byte *const entry = ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model));
    if (!entry)
        return;

    std::int32_t currentIndex{};
    float currentWeight{};
    std::memcpy(&currentIndex, entry + 0x48, sizeof(currentIndex));
    std::memcpy(&currentWeight, entry + 0x4c, sizeof(currentWeight));
    if (currentIndex == morphIndex && std::fabs(currentWeight - weight) <= 0.01f) {
        return;
    }

    const float duration = speed * (1.0f / 30.0f);
    const std::uint32_t zero = 0;
    std::memcpy(entry + 0x48, &morphIndex, sizeof(morphIndex));
    std::memcpy(entry + 0x4c, &weight, sizeof(weight));
    std::memcpy(entry + 0x50, &duration, sizeof(duration));
    std::memcpy(entry + 0x54, &zero, sizeof(zero));
    entry[0x58] = std::byte{1};
    isValidCEWP(model);
}

// @verified - p
void faceBindNewPhoneme(FaceClipControllerState *clip) {
    if (!clip->generatedLipCues)
        return;
    FaceLipCue *const cues = clip->generatedLipCues;
    if (clip->activeSequenceState && clip->activeLipCues == cues) {
        clip->generatedLipCues = nullptr;
        return;
    }

    clip->activeLipCues = cues;
    clip->cueIndex = 0;
    std::int16_t viseme = cues->viseme;
    if (viseme == -1) {
        clip->activeSequenceState = 0;
        clip->halfDuration = 0xffffU;
        clip->activeLipCues = nullptr;
        clip->cueIndex = 0;
        clip->shapeDuration = 0;
        clip->blendDuration = 8;
        clip->startState = 1;
        clip->generatedLipCues = nullptr;
        return;
    }
    if (viseme >= static_cast<std::int16_t>(kVisemeMap.size()))
        viseme = 0;
    clip->activeSequenceState = 1;
    clip->visemeIndex = kVisemeMap[static_cast<std::size_t>(viseme)];
    clip->blendDuration = 4;
    clip->startState = 2;
    clip->halfDuration = static_cast<std::uint16_t>(cues->duration / 2);
    if (static_cast<std::int16_t>(clip->halfDuration) <= 0)
        clip->halfDuration = 1;
    clip->shapeDuration = static_cast<std::int16_t>(cues->duration - 2 * static_cast<std::int16_t>(clip->halfDuration));
    clip->generatedLipCues = nullptr;
}

// @verified - p
std::int16_t faceAdvanceSequence(FaceClipControllerState *clip) {
    const auto remaining = static_cast<std::int16_t>(--clip->halfDuration);
    if (remaining <= 0) {
        const std::int16_t cueIndex = static_cast<std::int16_t>(++clip->cueIndex);
        FaceLipCue &cue = clip->activeLipCues[cueIndex];
        std::int16_t viseme = cue.viseme;
        if (viseme == -1) {
            clip->activeSequenceState = 0;
            clip->activeLipCues = nullptr;
            clip->halfDuration = 0xffffU;
            clip->cueIndex = 0;
            clip->shapeDuration = 0;
            clip->blendDuration = 8;
            clip->startState = 1;
            return -1;
        }
        if (viseme >= static_cast<std::int16_t>(kVisemeMap.size()))
            viseme = 0;
        clip->activeSequenceState = 1;
        clip->startState = 2;
        clip->visemeIndex = kVisemeMap[static_cast<std::size_t>(viseme)];
        if (viseme == 0)
            clip->blendDuration = 8;
        else if (viseme >= 1)
            clip->blendDuration = 4;
        const std::int32_t duration = (static_cast<std::int16_t>(clip->shapeDuration) + cue.duration) / 2;
        clip->halfDuration = static_cast<std::uint16_t>(duration);
        if (static_cast<std::int16_t>(clip->halfDuration) <= 0)
            clip->halfDuration = 1;
        clip->shapeDuration = static_cast<std::int16_t>(cue.duration - 2 * static_cast<std::int16_t>(clip->halfDuration));
        return clip->shapeDuration;
    }

    if (remaining <= 4 && clip->activeSequenceState != 2) {
        std::int16_t viseme = clip->activeLipCues[static_cast<std::int16_t>(clip->cueIndex + 1)].viseme;
        if (viseme != -1) {
            if (viseme >= static_cast<std::int16_t>(kVisemeMap.size()))
                viseme = 0;
            clip->activeSequenceState = 2;
            clip->startState = 2;
            clip->visemeIndex = kVisemeMap[static_cast<std::size_t>(viseme)];
            if (viseme == 0)
                clip->blendDuration = 8;
            else if (viseme >= 1)
                clip->blendDuration = 4;
        }
    }
    return remaining;
}

std::int16_t Clipper_Alloc(Character *character) {
    auto *const clip = static_cast<FaceClipControllerState *>(AllocateHeapMemoryMODL(character, kClipComponent, sizeof(FaceClipControllerState), CalcFACE));

    clip->field18 = 0;
    clip->generatedLipCues = nullptr;
    clip->clip = nullptr;
    clip->startState = 0;
    clip->field29 = 0;
    clip->alternatingShape = 0;
    clip->shapeDuration = 0;
    clip->blendDuration = 2;
    clip->halfDuration = 0xffffU;
    clip->enabled = 1;
    clip->field32 = 0;
    clip->visemeIndex = 1;
    clip->channelTag = static_cast<std::uint16_t>(-67);
    clip->channelIndex = 3;
    clip->activeSequenceState = 0;
    clip->activeLipCues = nullptr;
    clip->cueIndex = 0;
    clip->field42 = 0;
    clip->reserved55 = 0;
    clip->morphComplete = 1;
    clip->field54 = 0;
    clip->rotationY = 0;
    clip->field4c = 0;
    clip->field44 = 0;

    clip->characterFaceHandler = character ? character->figurePoseCallback : nullptr;
    if (character)
        character->figurePoseCallback = faceEntry;

    std::size_t clipIndex = 0;
    while (clipIndex < g_clipMemory.size() && g_clipMemory[clipIndex])
        ++clipIndex;
    if (clipIndex < g_clipMemory.size()) {
        clip->field1a = static_cast<std::uint16_t>(clipIndex);
        g_clipMemory[clipIndex] = character;
    }
    const std::int16_t cursor = static_cast<std::int16_t>(clipIndex);
    if (cursor > g_clipSlotHighWaterIndex)
        g_clipSlotHighWaterIndex = cursor;
    return g_clipSlotHighWaterIndex;
}

FaceWork *AllocateFACE(Character *character, const char *faceName) {
    auto *const face = static_cast<FaceWork *>(AllocateHeapMemoryMODL(character, kFaceComponent, sizeof(FaceWork), FACE_Callback));

    std::strcpy(reinterpret_cast<char *>(face->faceName), faceName);
    std::strncpy(reinterpret_cast<char *>(face->faceTableName), faceName, 3);
    reinterpret_cast<char *>(face->faceTableName)[3] = '\0';
    std::strcpy(reinterpret_cast<char *>(face->faceTableName) + 3, "_FTBL.BIN");
    FACE_InitializeWork(*face);
    LoadRandomTalkBank(reinterpret_cast<const char *>(face->faceName));
    return face;
}

std::int32_t FACE_ScriptFaceMotion(Character *character, std::int32_t mode) {
    FaceWork *const face = FindFace(character);
    if (!face)
        return -1;
    if (mode == 0) {
        const std::uint8_t slot = facemot(*face);
        return slot < 8 ? static_cast<std::int32_t>(slot) : -1;
    }
    if (mode == 1) {
        const std::uint8_t slot = static_cast<std::uint8_t>(static_cast<std::uint16_t>(face->eyeMode) >> 8U);
        return slot < 8 ? 1 : -1;
    }
    return -1;
}

void FACE_ScriptReleaseMotion(Character *character) {
    if (!character || !MOTM_GetWork(character))
        return;
    FaceWork *const face = FindFace(character);
    if (!face)
        return;

    const std::uint8_t index = faceFindCacheSlotByModelName(reinterpret_cast<const char *>(face->faceName));
    if (index >= 8)
        return;
    ReleaseFaceCacheReference(index, reinterpret_cast<const char *>(face->faceName));
}



void npcReleaseFace(Character *character) {
    if (!character || !HLib_GetCharacterComponent(character, SHEN_NAME(MOTM)))
        return;
    FaceWork *const face = FindFace(character);
    if (!face)
        return;
    FACE_UpdateExpression(character, 0, 0, 0);
    const std::uint8_t index = static_cast<std::uint8_t>(static_cast<std::uint16_t>(face->eyeMode) >> 8U);
    if (index < 8)
        ReleaseFaceCacheReference(index, reinterpret_cast<const char *>(face->faceName));
}


void FACE_SetLcgState(Character *character, std::int32_t mode, std::int32_t interval, std::int32_t flags) {
    if (!HLib_GetCharacterComponent(character, SHEN_NAME(MOTM)))
        return;
    FaceWork *const face = FindFace(character);
    if (!face)
        return;
    auto *const state = reinterpret_cast<std::uint8_t *>(&face->field50);
    state[0] = static_cast<std::uint8_t>(mode);
    state[1] = 1;
    state[3] = static_cast<std::uint8_t>(flags);
    std::uint32_t elapsed = 0;
    std::memcpy(state + 4, &elapsed, sizeof(elapsed));
    const std::int16_t frames = static_cast<std::int16_t>(interval);
    face->field58 = static_cast<std::uint16_t>(frames >= 1 ? frames : 1);
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    face->field5a = static_cast<std::uint16_t>(10U * (((g_EnvPrngSeed >> 16U) & 3U) + 6U));
    if (mode == 0)
        face->field58 = 3;
}


void FACE_UpdateExpression(Character *character, std::int32_t eyeMode, std::int32_t eyePattern, std::int32_t timing) {
    FaceWork *const face = FindFace(character);
    if (!face)
        return;
    const std::uint8_t cacheIndex = static_cast<std::uint8_t>(static_cast<std::uint16_t>(face->eyeMode) >> 8U);
    if (cacheIndex >= 8)
        return;

    const std::int16_t frames = static_cast<std::int16_t>(timing >= 1 ? timing : 1);
    float channelFrames = static_cast<float>(frames);
    bool updateChannels = true;
    if (channelFrames >= 1000.0f) {
        if (channelFrames < 2000.0f)
            updateChannels = false;
        else
            channelFrames -= 2000.0f;
    }
    if (updateChannels) {
        FACE_UpdateModelChannel(character->model, eyePattern, 0, channelFrames, 1.0f);
        FACE_UpdateModelChannel(character->model, eyePattern, 1, channelFrames, 1.0f);
    }
    if (face->field20 != reinterpret_cast<std::uint64_t>(g_faceModelCache[cacheIndex].modelRoot)) {
        face->field20 = 0;
        face->field28 = 0;
        face->eyeMode = 0;
    }
    if (!face->field20)
        return;
    reinterpret_cast<std::uint8_t *>(&face->eyeMode)[0] = static_cast<std::uint8_t>(eyeMode);
    face->frameCount = frames;
    face->eyePatternMode = static_cast<std::int16_t>(6 * eyePattern);
    reinterpret_cast<std::uint8_t *>(&face->field50)[1] = 0;
}



// @verified - p
void FACE_SetLookTarget(Character *character, std::int32_t mode, const Vector3 *target, std::int32_t blendFrames) {
    if (!character)
        return;
    FaceWork *const face = FindFace(character);
    if (!face)
        return;
    auto &state = *reinterpret_cast<FaceLookState *>(&face->field50);
    state.blendFrames = static_cast<std::int16_t>(blendFrames);
    state.flags = static_cast<std::int8_t>(static_cast<std::uint8_t>(mode) | 0x80U);
    state.blendFrame = 0;
    state.targetPosition = *target;
    if (static_cast<std::uint8_t>(mode) == 0) {
        state.flags = static_cast<std::int8_t>(0x80);
        state.targetPosition = {};
        state.targetPitchA = 0;
        state.targetYawA = 0;
        state.targetPitchB = 0;
        state.targetYawB = 0;
        if (g_debugUnknown3 == 0) {
            FACE_UpdateEyeLookAtModelTarget(character->model, 0, nullptr, nullptr);
            FACE_UpdateEyeLookAtModelTarget(character->model, 1, nullptr, nullptr);
        }
    }
    faceLookAtSolverComputeTargets(character, state);
}

void FACE_ApplyScriptMorph(Character *character, std::int32_t channel, std::int32_t morph, std::int32_t timing) {
    if (!character || !MOTM_GetWork(character))
        return;

    if (timing < 1000) {
        FACE_UpdateModelChannel(character->model, channel, 0, static_cast<float>(timing), 1.0f);
        FACE_UpdateModelChannel(character->model, channel, 1, static_cast<float>(timing), 1.0f);
    } else if (timing >= 2000) {
        FACE_UpdateModelChannel(character->model, channel, 0, static_cast<float>(timing - 2000), 1.0f);
        FACE_UpdateModelChannel(character->model, channel, 1, static_cast<float>(timing - 2000), 1.0f);
        faceApplyMorph(character->model, morph, static_cast<float>(timing), 1.0f);
        return;
    }

    FaceWork *const face = FindFace(character);
    FaceClipControllerState *const clip = FindClip(character);
    const std::int16_t frames = static_cast<std::int16_t>(timing >= 1 ? timing : 1);
    if (clip)
        clip->blendDuration = frames;
    const std::int16_t duration = static_cast<std::int16_t>(frames % 1000);
    if (face) {
        face->frameCount = duration;
        face->eyePatternMode = static_cast<std::int16_t>(6 * channel);
    }
    if (clip) {
        if (morph == -1) {
            clip->startState = 1;
            clip->visemeIndex = 1;
        } else {
            clip->startState = 2;
            clip->visemeIndex = static_cast<std::int16_t>(morph);
        }
    }
    if (face) {
        auto *const state = reinterpret_cast<std::uint8_t *>(&face->field50);
        if (static_cast<std::int8_t>(state[3]) <= 1) {
            if (state[1] == 0)
                face->field58 = duration;
            state[1] = 1;
        }
    }
}


bool faceHasRemainingDuration(Character *character) {
    const CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kClipComponent);
    if (!component || (component->flags & 1U) == 0)
        return false;
    FaceClipControllerState *const clip = FindClip(character);
    return clip && static_cast<std::int16_t>(clip->halfDuration) > 0;
}

bool faceSetGeneratedLipCues(Character *character, FaceLipCue *cues, bool enabled) {
    if (!character)
        return false;
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CLIP));
    if (!component || (component->flags & 1U) == 0)
        return false;
    auto *const clip = reinterpret_cast<FaceClipControllerState *>(component + 1);
    clip->generatedLipCues = cues;
    clip->enabled = enabled ? 1 : 0;
    return true;
}

} // namespace shenmue::s1
