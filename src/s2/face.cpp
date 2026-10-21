#include "s2/face.hpp"

#include "s2/character.hpp"
#include "s2/hlib.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/mt7.hpp"
#include "s2/model_component.hpp"
#include "s2/camera.hpp"
#include "s2/motion_work.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <new>

namespace shenmue::s2 {
namespace {

constexpr std::uint32_t faceSignature = SHEN_NAME(FACE);
constexpr shen_name faceToken{SHEN_NAME(FACX)};
constexpr std::array<std::uint8_t, 16> clipMouthExpressions{
    0, 3, 2, 5, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
std::uint32_t facialRandomState = 0xffffffffU;
std::uint32_t eyeControlFlags = 0xffffffffU;

template <class T> bool ReadFaceValue(std::span<const std::byte> bytes, std::size_t offset, T &value) {
    if (offset > bytes.size() || sizeof(T) > bytes.size() - offset)
        return false;

    std::memcpy(&value, bytes.data() + offset, sizeof(T));
    return true;
}

// ResetFacialModelTransitions retains the authored coefficients and deformation descriptors.
void ResetFacialModelTransitions(FacialModelData &data) {
    data.flags &= 0xfffff400U;
    data.upperFaceDuration = 0.0f;
    data.lowerFaceDuration = 0.0f;
    data.eyelidDuration = 0.0f;
    data.mouthDuration = 0.0f;
    data.expressionScale = 1.0f;
    data.eyelid0OuterWeight = 0.0f;
    data.eyelid1OuterWeight = 0.0f;
    data.eyelid0InnerWeight = 0.0f;
    data.eyelid1InnerWeight = 0.0f;
    data.eyelid0Mode = 1;
    data.eyelid1Mode = 1;
    if (data.shapeCoefficients[11] == 0.0f)
        data.shapeCoefficients[11] = 1.0f;
    data.modelOffset = 0;
    data.reserved10 = 0;
    data.reserved14 = 0;
}



void InitializeDefaultFacialModel(FacialModelData &data) {
    ResetFacialModelTransitions(data);
    data.signature = faceToken.name;
    data.version = 4;
    data.lookLimits = {};
    for (FacialMorphGroup &group : data.groups)
        group.count = 0;
}

bool BindFacialVertex(const MT7Model &model, FacialMorphControl &control) {
    for (const MT7Node &node : model.nodes) {
        if (!node.xb01Offset || !node.meshVertexStride || !node.meshVertexCount)
            continue;

        std::uint32_t commandWords{};
        if (!ReadFaceValue(model.source, node.xb01Offset + 28U, commandWords))
            return false;

        const std::uint64_t begin = std::uint64_t(node.xb01Offset) + 44U + std::uint64_t(commandWords) * 4U;
        const std::uint64_t size = std::uint64_t(node.meshVertexCount) * node.meshVertexStride;
        const std::uint64_t position = control.vertex.positionOffset;
        if (position < begin || position - begin > size || sizeof(Vector3) > size - (position - begin))
            continue;

        control.buffer = FindMT7MutableVertexBuffer(node);
        if (!control.buffer)
            return false;

        control.byteOffset = static_cast<std::uint32_t>(position - begin);
        return true;
    }
    return false;
}

Vector3 ReadFacialPosition(const FacialMorphControl &control) {
    const auto *bytes = static_cast<const std::byte *>(d3t::GetBufferCpuData(control.buffer->handle, nullptr));
    Vector3 position;
    std::memcpy(&position, bytes + control.byteOffset, sizeof(position));
    return position;
}

void WriteFacialPosition(const FacialMorphControl &control, Vector3 position, bool markDirty) {
    if (markDirty)
        position.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(position.x) | 1U);

    auto *bytes = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, control.buffer->handle, 1, 0));
    if (bytes)
        std::memcpy(bytes + control.byteOffset, &position, sizeof(position));
}

// BeginNeutralFacialGroup snapshots the current mesh, not the previous target.
void BeginNeutralFacialGroup(std::vector<FacialMorphControl> &group, float duration) {
    for (FacialMorphControl &control : group) {
        control.vertex.target = control.vertex.neutral;
        control.vertex.start = ReadFacialPosition(control);
        if (duration <= 0.0f)
            WriteFacialPosition(control, control.vertex.target, false);
    }
}

// InterpolateFacialGroup uses integer fixed-angle sine inside an otherwise float tween.
void InterpolateFacialGroup(std::vector<FacialMorphControl> &group, float frame, float duration) {
    if (group.empty() || duration == 0.0f)
        return;

    const std::int32_t angle = static_cast<std::int32_t>(frame) * 65536 / static_cast<std::int32_t>(duration);
    const float amount = frame / duration - sin_fixed16(static_cast<std::int16_t>(angle)) * 0.1f;
    for (FacialMorphControl &control : group) {
        const Vector3 &from = control.vertex.start;
        const Vector3 &to = control.vertex.target;
        WriteFacialPosition(control, {from.x + (to.x - from.x) * amount, (to.y - from.y) * amount + from.y, (to.z - from.z) * amount + from.z}, true);
    }
}

void FinishFacialGroup(std::vector<FacialMorphControl> &group) {
    for (FacialMorphControl &control : group) {
        control.vertex.start = control.vertex.target;
        WriteFacialPosition(control, control.vertex.target, true);
    }
}

// UploadFacialModel uploads the mesh selected by the first populated group.
void UploadFacialModel(const FacialModelResource &face) {
    for (const auto &group : face.vertices) {
        if (group.empty())
            continue;

        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, group.front().buffer->handle);
        return;
    }
}

void SnapshotFacialTarget(FacialMorphControl &control, float duration) {
    control.vertex.start = ReadFacialPosition(control);
    if (duration == 0.0f)
        WriteFacialPosition(control, control.vertex.target, false);
}

// PrepareOuterEyelids: two outer-eyelid groups, including expression-specific
// vertical curvature. The coefficient at +0xd4 offsets only the closure.
void PrepareOuterEyelids(FacialModelResource &face, float duration, float first, float second) {
    const float adjustment = face.data.shapeCoefficients[29];
    const std::array<float, 2> weights{first, second};
    for (std::size_t side = 0; side != 2; ++side) {
        const std::size_t index = side + 1;
        if (!face.data.groups[index].count)
            continue;

        const float weight = weights[side];
        const float curveWeight = std::fmin(weight, 0.3f);
        float closure = weight;
        if (adjustment != 0.0f && closure < 0.8f)
            closure = std::fmin(0.8f, adjustment + closure);
        const float coefficient = face.data.shapeCoefficients[side + 2];
        const float displacement = closure * (coefficient * 0.0024f);
        for (FacialMorphControl &control : face.vertices[index]) {
            FacialMorphVertex &vertex = control.vertex;
            vertex.target = vertex.neutral;
            vertex.target.z -= displacement * vertex.weight1;
            if (face.data.upperExpression == 2) {
                const float curve = curveWeight * (coefficient * 0.0048f);
                const auto angle = static_cast<std::uint16_t>(4096 - static_cast<std::int32_t>((1.0f - vertex.weight0) * -36864.0f));
                const auto folded = static_cast<std::int16_t>(angle > 0x8000U ? -angle : angle);
                vertex.target.z += (sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) + 0.7f) * curve;
            } else if (face.data.upperExpression == 15) {
                const float curve = curveWeight * (coefficient * 0.002f);
                const float position = 1.0f - vertex.weight0;
                const float fraction = position <= 0.5f ? 0.0f : (position - 0.5f) + (position - 0.5f);
                vertex.target.z -= sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(fraction * 16384.0f))) * curve;
            }
            SnapshotFacialTarget(control, duration);
        }
        face.data.flags |= 1U << index;
    }
}

// PrepareInnerEyelids: inner eyelids translate in X/Z and mark target-X bit zero.
void PrepareInnerEyelids(FacialModelResource &face, float duration, float first, float second) {
    const std::array<float, 2> weights{first, second};
    for (std::size_t side = 0; side != 2; ++side) {
        const std::size_t index = side + 4;
        if (!face.data.groups[index].count)
            continue;

        const float displacement = weights[side] * (face.data.shapeCoefficients[side + 4] * 0.0045f);
        const float horizontal = displacement * 0.3f;
        for (FacialMorphControl &control : face.vertices[index]) {
            FacialMorphVertex &vertex = control.vertex;
            vertex.target.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(horizontal * vertex.weight1 + vertex.neutral.x) | 1U);
            vertex.target.y = vertex.neutral.y;
            vertex.target.z = displacement * vertex.weight1 + vertex.neutral.z;
            SnapshotFacialTarget(control, duration);
        }
        face.data.flags |= 1U << index;
    }
}

// EyeCtrl_Two/Three preserve a running blink and only publish new targets.
void SetOuterEyelidWeights(FacialModelResource &face, float duration, float first, float second) {
    face.data.eyelid0Mode = first == 0.0f && second == 0.0f;
    face.data.eyelid0OuterWeight = first;
    face.data.eyelid1OuterWeight = second;
    if (face.data.eyelidDuration <= 0.0f)
        PrepareOuterEyelids(face, duration, first, second);
}

void SetInnerEyelidWeights(FacialModelResource &face, float duration, float first, float second) {
    face.data.eyelid1Mode = first == 0.0f && second == 0.0f;
    face.data.eyelid0InnerWeight = first;
    face.data.eyelid1InnerWeight = second;
    if (face.data.eyelidDuration <= 0.0f)
        PrepareInnerEyelids(face, duration, first, second);
}

void BeginFacialBlink(FacialModelResource &face) {
    FacialModelData &data = face.data;
    if (!(data.eyelidDuration <= 0.0f && data.upperFaceDuration <= 0.0f))
        return;

    data.flags |= 0x600U;
    data.eyelidFrame = 0.0f;
    data.eyelidDuration = 2.0f;
    const float first = data.eyelid0OuterWeight <= 1.0f ? 2.0f - data.eyelid0InnerWeight : data.eyelid0OuterWeight;
    const float second = data.eyelid1OuterWeight <= 1.0f ? 2.0f - data.eyelid1InnerWeight : data.eyelid1OuterWeight;
    PrepareOuterEyelids(face, 2.0f, first, second);
}

void InterpolateEyelids(FacialModelResource &face) {
    const FacialModelData &data = face.data;
    if (data.eyelidDuration < std::numeric_limits<float>::epsilon())
        return;

    float amount = data.eyelidFrame / data.eyelidDuration;
    if (data.flags & 0x200U) {
        const auto angle = static_cast<std::int16_t>(static_cast<std::int32_t>(data.eyelidFrame) * 32768 / static_cast<std::int32_t>(data.eyelidDuration));
        const float curve = sin_fixed16(angle) * 0.2f;
        if (data.flags & 0x400U)
            amount -= curve;
        else
            amount = curve + amount;
    }

    for (const std::size_t index : {1U, 2U, 4U, 5U}) {
        if (!(data.flags & (1U << index)))
            continue;

        for (FacialMorphControl &control : face.vertices[index]) {
            const Vector3 &start = control.vertex.start;
            const Vector3 &target = control.vertex.target;
            WriteFacialPosition(
                control, {start.x + (target.x - start.x) * amount, (target.y - start.y) * amount + start.y, (target.z - start.z) * amount + start.z}, true);
        }
    }
}

void FinishEyelids(FacialModelResource &face) {
    for (const std::size_t index : {1U, 2U, 4U, 5U}) {
        if (!(face.data.flags & (1U << index)))
            continue;

        FinishFacialGroup(face.vertices[index]);
        face.data.flags &= ~(1U << index);
    }
}



void SnapshotImmediateFacialTarget(FacialMorphControl &control, float duration) {
    control.vertex.start = ReadFacialPosition(control);
    if (duration <= 0.0f)
        WriteFacialPosition(control, control.vertex.target, false);
}


void PrepareBrowTilt(FacialModelResource &face, float duration, float elevation, float contraction) {
    if (!face.data.groups[0].count)
        return;

    const float horizontal = contraction * 0.008f;
    for (FacialMorphControl &control : face.vertices[0]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        const float shift = horizontal * vertex.weight0;
        vertex.target.y = vertex.neutral.y >= 0.0f ? vertex.neutral.y - shift : shift + vertex.neutral.y;
        vertex.target.z = (elevation * 0.008f) * vertex.weight0 + vertex.neutral.z;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 1U;
}


void PrepareCurvedBrows(FacialModelResource &face, float duration, float scale) {
    if (!face.data.groups[0].count)
        return;

    for (FacialMorphControl &control : face.vertices[0]) {
        auto &vertex = control.vertex;
        vertex.target.x = vertex.neutral.x;
        vertex.target.y = (1.0f - scale * 0.05f) * vertex.neutral.y;
        const auto angle = static_cast<std::uint16_t>(8192 - static_cast<std::int32_t>((1.0f - vertex.weight0) * -36864.0f));
        const auto folded = static_cast<std::int16_t>(angle > 0x8000U ? -angle : angle);
        const float curve = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        vertex.target.z = (curve + 0.3f) * (scale * 0.008f) + vertex.neutral.z;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 1U;
}


void PrepareBrowElevation(FacialModelResource &face, float duration, float elevation) {
    if (!face.data.groups[0].count)
        return;

    for (FacialMorphControl &control : face.vertices[0]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        vertex.target.z = elevation + vertex.neutral.z;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 1U;
}


void PrepareInvertedBrowTilt(FacialModelResource &face, float duration, float elevation, float contraction) {
    if (!face.data.groups[0].count)
        return;

    const float horizontal = contraction * 0.008f;
    for (FacialMorphControl &control : face.vertices[0]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        const float shift = horizontal * vertex.weight0;
        vertex.target.y = vertex.neutral.y >= 0.0f ? vertex.neutral.y - shift : shift + vertex.neutral.y;
        vertex.target.z = (0.6f - vertex.weight0) * (elevation * 0.008f) + vertex.neutral.z;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 1U;
}


void PrepareSuppressedBrowTilt(FacialModelResource &face, float duration, float elevation, float contraction) {
    if (!face.data.groups[0].count)
        return;

    const float horizontal = contraction * 0.008f;
    for (FacialMorphControl &control : face.vertices[0]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        const float shift = horizontal * vertex.weight0;
        vertex.target.y = vertex.neutral.y >= 0.0f ? vertex.neutral.y - shift : shift + vertex.neutral.y;

        float weight = 0.6f - vertex.weight0;
        if (weight > 0.0f)
            weight *= 0.3f;
        vertex.target.z = weight * (elevation * 0.008f) + vertex.neutral.z;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 1U;
}


void PrepareAsymmetricBrowTilt(FacialModelResource &face, float duration, float scale, std::int32_t side) {
    if (!face.data.groups[0].count)
        return;

    const float displacement = scale * 0.008f;
    for (FacialMorphControl &control : face.vertices[0]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        float weight = 0.6f - vertex.weight0;
        if ((side == 1 ? vertex.neutral.y < 0.0f : vertex.neutral.y > 0.0f) && weight > 0.0f)
            weight *= 0.3f;
        vertex.target.z = weight * displacement + vertex.neutral.z;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 1U;
}

// PrepareOneSidedBrow deliberately retains (1-weight)+weight, not a constant one.
void PrepareOneSidedBrow(FacialModelResource &face, float duration, float scale) {
    if (!face.data.groups[0].count)
        return;

    const float displacement = scale * 0.003f;
    for (FacialMorphControl &control : face.vertices[0]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        const float shift = vertex.neutral.y <= 0.0f ? ((1.0f - vertex.weight0) + vertex.weight0) * displacement : displacement * vertex.weight0;
        vertex.target.z -= shift;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 1U;
}


void PrepareNegativeCheek(FacialModelResource &face, float duration, float scale) {
    if (!face.data.groups[3].count)
        return;

    for (FacialMorphControl &control : face.vertices[3]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        if (vertex.neutral.y < 0.0f) {
            vertex.target.y -= (scale * 0.005f) * vertex.weight0;
            vertex.target.z = (scale * 0.001f) * vertex.weight0 + vertex.neutral.z;
        }
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 8U;
}


void PreparePositiveCheek(FacialModelResource &face, float duration, float scale) {
    if (!face.data.groups[3].count)
        return;

    for (FacialMorphControl &control : face.vertices[3]) {
        auto &vertex = control.vertex;
        vertex.target = vertex.neutral;
        if (vertex.neutral.y > 0.0f) {
            vertex.target.y = (scale * 0.005f) * vertex.weight0 + vertex.neutral.y;
            vertex.target.z = (scale * 0.001f) * vertex.weight0 + vertex.neutral.z;
        }
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 8U;
}

Vector3 ReadFacialNormal(const FacialMorphControl &control) {
    const auto *bytes = static_cast<const std::byte *>(d3t::GetBufferCpuData(control.buffer->handle, nullptr));

    Vector3 normal;
    std::memcpy(&normal, bytes + control.byteOffset + sizeof(Vector3), sizeof(normal));
    return normal;
}

// PrepareCheekSmile displaces along the current mesh normal as well as vertical.
void PrepareCheekSmile(FacialModelResource &face, float duration, float scale) {
    if (!face.data.groups[3].count)
        return;
    const float normalScale = (scale * 0.005f) * face.data.shapeCoefficients[7];
    const float elevation = (scale * 0.01f) * face.data.shapeCoefficients[6];
    for (FacialMorphControl &control : face.vertices[3]) {
        auto &vertex = control.vertex;
        const Vector3 normal = ReadFacialNormal(control);
        vertex.target.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>((normalScale * normal.x) * vertex.weight0 + vertex.neutral.x) | 1U);
        vertex.target.y = (normalScale * normal.y) * vertex.weight0 + vertex.neutral.y;
        vertex.target.z = ((normalScale * normal.z) + elevation) * vertex.weight0 + vertex.neutral.z;
        SnapshotImmediateFacialTarget(control, duration);
    }
    face.data.flags |= 8U;
}

void PrepareUpperFace(FacialModelResource &face, const void *model, std::int32_t expression, float duration, float scale) {
    d3t::SetObjectFacialExpression(
        GetMT7ReaderData(static_cast<const MT7Model *>(model)), static_cast<std::uint32_t>(expression), d3t::FacialExpressionLayer::Upper, duration, scale);
    FacialModelData &data = face.data;
    data.upperFaceDuration = duration;
    data.upperExpression = expression;
    data.upperFaceFrame = 0.0f;
    if (expression == 0) {
        BeginNeutralFacialGroup(face.vertices[0], duration);
        data.flags |= 1U;
        data.eyelid0OuterWeight = data.eyelid1OuterWeight = 0.0f;
        data.eyelid0InnerWeight = data.eyelid1InnerWeight = 0.0f;
        if (data.eyelidDuration <= 0.0f) {
            if (data.eyelid0Mode)
                PrepareOuterEyelids(face, duration, 0.0f, 0.0f);
            if (data.eyelid1Mode)
                PrepareInnerEyelids(face, duration, 0.0f, 0.0f);
            data.eyelidFrame = data.upperFaceFrame;
            data.eyelidDuration = data.upperFaceDuration;
        }
        return;
    }

    if (expression < 1 || expression > 22)
        return;
    const float rise = data.shapeCoefficients[0];
    const float fall = data.shapeCoefficients[1];
    switch (expression) {
    case 3:
    case 10:
    case 15:
    case 19:
        PrepareInvertedBrowTilt(face, duration, -(scale * fall), 0.0f);
        break;
    case 4:
        PrepareBrowElevation(face, duration, (scale * 0.003f) * rise);
        break;
    case 5:
        PrepareBrowElevation(face, duration, (scale * -0.003f) * fall);
        break;
    case 7:
        PrepareBrowElevation(face, duration, (scale * -0.004f) * fall);
        break;
    case 8:
        PrepareBrowTilt(face, duration, -(scale * fall), 0.0f);
        break;
    case 9:
        PrepareBrowElevation(face, duration, (scale * 0.004f) * rise);
        break;
    case 21:
        PrepareBrowElevation(face, duration, (scale * 0.0006f) * rise);
        break;
    }

    // FaceLooks_One's four coefficients, in left/right outer/inner order.
    constexpr std::array<std::array<float, 4>, 23> lidWeights{{
        {0,0,0,0}, {0.25f,0.25f,0,0}, {0.3f,0.3f,0.3f,0.3f},
        {0.1f,0.1f,0.5f,0.5f}, {-0.5f,-0.5f,-0.2f,-0.2f},
        {0.2f,0.2f,0.1f,0.1f}, {0.8f,0.25f,0.6f,0.2f},
        {0.2f,0.2f,0.1f,0.1f}, {1,1,0,0}, {0.5f,0.5f,0.2f,0.2f},
        {0.1f,0.1f,0.5f,0.5f}, {1.1f,1.1f,1.1f,1.1f},
        {-0.2f,-0.2f,0,0}, {-0.1f,-0.1f,0.1f,0.1f},
        {0.25f,0.25f,0.8f,0.8f}, {0.056f,0.056f,0.56f,0.56f},
        {-0.2f,-1,1,0.2f}, {-1,-0.2f,0.2f,1}, {-1,-1,0.8f,0.8f},
        {0.1f,0.1f,0.5f,0.5f}, {1.1f,1.1f,1.1f,1.1f},
        {-0.1f,-0.1f,-0.04f,-0.04f}, {-0.1f,-0.1f,0,0}
    }};
    const auto &weights = lidWeights[static_cast<std::size_t>(expression)];
    if (data.eyelid0Mode) {
        data.eyelid0OuterWeight = scale * weights[0];
        data.eyelid1OuterWeight = scale * weights[1];
        if (data.eyelidDuration <= 0.0f)
            PrepareOuterEyelids(face, duration, data.eyelid0OuterWeight, data.eyelid1OuterWeight);
    }
    if (data.eyelid1Mode) {
        data.eyelid0InnerWeight = expression == 13 ? 0.1f : scale * weights[2];
        data.eyelid1InnerWeight = expression == 13 ? 0.1f : scale * weights[3];
        if (data.eyelidDuration <= 0.0f)
            PrepareInnerEyelids(face, duration, data.eyelid0InnerWeight, data.eyelid1InnerWeight);
    }
    if (data.eyelidDuration <= 0.0f) {
        data.eyelidFrame = data.upperFaceFrame;
        data.eyelidDuration = data.upperFaceDuration;
    }
    switch (expression) {
    case 1:
    case 11:
    case 14:
        PrepareBrowTilt(face, duration, -(scale * fall), 0.2f);
        break;
    case 2:
        PrepareCurvedBrows(face, duration, scale * rise);
        break;
    case 6:
        PrepareOneSidedBrow(face, duration, scale);
        break;
    case 12:
        PrepareBrowTilt(face, duration, scale * rise, 0.0f);
        break;
    case 13:
    case 18:
        PrepareSuppressedBrowTilt(face, duration, scale * rise, 0.0f);
        break;
    case 16:
        PrepareAsymmetricBrowTilt(face, duration, scale * rise, 0);
        break;
    case 17:
        PrepareAsymmetricBrowTilt(face, duration, scale * rise, 1);
        break;
    case 20:
        PrepareCurvedBrows(face, duration, (scale * rise) * 0.5f);
        break;
    case 22:
        PrepareBrowTilt(face, duration, (scale * fall) * -0.8f, 0.16f);
        break;
    }
}

using FacialShapeModifier = void (*)(const FacialModelData &, FacialMorphVertex &);
std::array<FacialShapeModifier, 3> mouthModifiers{};


void ModifyUpperAsymmetricLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float inverse = 1.0f - v.weight0;
    const float amount = (data.shapeCoefficients[8] * 0.001f) * data.expressionScale;
    v.target.x = inverse * amount + v.target.x;
    v.target.z = v.weight0 * amount + v.target.z;
    if (v.neutral.y < 0.0f) {
        float distance = inverse + -0.5f;
        if (distance < 0.0f)
            distance = -distance;
        v.target.z = (0.5f - distance) * ((data.shapeCoefficients[8] * 0.002f) * data.expressionScale) + v.target.z;
    }
}

void ModifyLowerAsymmetricLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float amount = (data.shapeCoefficients[9] * 0.001f) * data.expressionScale;
    const float fall = (data.shapeCoefficients[9] * 0.004f) * data.expressionScale;
    v.target.x = (1.0f - v.weight1) * amount + v.target.x;
    const float delta = v.neutral.y <= 0.0f ? (v.weight1 + (1.0f - v.weight1)) * fall : (v.weight1 * amount) + (v.weight1 * amount);
    v.target.z -= delta;
}

void ModifyUpperSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[8] * data.expressionScale;
    const float weight = (1.0f - v.weight0) * scale;
    const float spread = weight * 0.001f;
    v.target.x = weight * 0.001f + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = (weight * 0.004f + scale * 0.001f) + v.target.z;
}

void ModifyLowerSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[12];
    const float scale = data.shapeCoefficients[9] * data.expressionScale;
    const float first = ((1.0f - v.weight0) * scale) * coefficient;
    const float second = (1.0f - v.weight1) * scale;
    const float spread = second * 0.001f;
    const float advance = second * 0.001f - ((v.weight0 * scale) * coefficient) * 0.002f;
    v.target.x = advance + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = ((first * 0.0025f + second * 0.004f) + scale * 0.001f) + v.target.z;
}

void ModifyUpperHalfSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = (data.shapeCoefficients[8] * data.expressionScale) * 0.5f;
    const float weight = (1.0f - v.weight0) * scale;
    const float spread = weight * 0.001f;
    v.target.x = weight * 0.001f + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = (weight * 0.004f + scale * 0.001f) + v.target.z;
}

void ModifyLowerHalfSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[12];
    const float scale = (data.shapeCoefficients[9] * data.expressionScale) * 0.5f;
    const float first = ((1.0f - v.weight0) * scale) * coefficient;
    const float second = (1.0f - v.weight1) * scale;
    const float spread = second * 0.001f;
    const float advance = second * 0.001f - ((v.weight0 * scale) * coefficient) * 0.002f;
    v.target.x = advance + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = ((first * 0.0025f + second * 0.004f) + scale * 0.001f) + v.target.z;
}

void ModifyUpperWideSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[8] * data.expressionScale;
    const float weight = (1.0f - v.weight0) * scale;
    const float spread = weight * 0.006f;
    v.target.x = weight * 0.001f + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = (scale * 0.001f + weight * 0.005f) + v.target.z;
}

void ModifyLowerWideSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[12];
    const float scale = data.shapeCoefficients[9] * data.expressionScale;
    const float first = ((1.0f - v.weight0) * scale) * coefficient;
    const float second = (1.0f - v.weight1) * scale;
    const float retreat = ((v.weight0 * scale) * coefficient) * 0.002f;
    const float spread = second * 0.006f;
    v.target.x = (second * 0.001f - retreat) + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = ((first * 0.0025f + second * 0.005f) + scale * 0.001f) + v.target.z;
}

void ModifyUpperRaisedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[8];
    const float scale = data.expressionScale;
    const float inverse = ((1.0f - v.weight0) * coefficient) * scale;
    const float tip = v.weight0 >= 0.1f ? 0.0f : (0.1f - v.weight0) * 0.0132f;
    const float spread = inverse * 0.002f;
    const float lift = ((coefficient * v.weight0) * scale) * 0.002f;
    v.target.x = inverse * 0.001f + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = ((spread + tip) + lift) + v.target.z;
}

void ModifyLowerRaisedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float inverse = ((1.0f - v.weight1) * coefficient) * data.expressionScale;
    const float spread = inverse * 0.002f;
    const float lift = ((coefficient * v.weight1) * data.expressionScale) * 0.002f;
    v.target.x = inverse * 0.001f + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = (spread + lift) + v.target.z;
}

void ModifyUpperPinchedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[8] * data.expressionScale;
    const float weighted = scale * v.weight0;
    v.target.x = weighted * 0.0005f + v.target.x;
    v.target.z = (weighted * 0.002f - scale * 0.001f) + v.target.z;
}

void ModifyLowerPinchedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[9] * data.expressionScale;
    const float weighted = scale * v.weight1;
    v.target.x = weighted * 0.0005f + v.target.x;
    v.target.z = (weighted * 0.002f - scale * 0.001f) + v.target.z;
}

void ModifyLowerLipDrop(const FacialModelData &data, FacialMorphVertex &v) {
    v.target.z -= ((data.shapeCoefficients[9] * v.weight1) * data.expressionScale) * 0.004f;
}

void ModifyUpperLipCurl(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[8];
    v.target.z =
        (((coefficient * v.weight1) * data.expressionScale) * 0.004f - (((1.0f - v.weight0) * coefficient) * data.expressionScale) * 0.004f) + v.target.z;
}

void ModifyLowerLipCurl(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float inverse = ((1.0f - v.weight1) * coefficient) * data.expressionScale;
    const float weighted = (coefficient * v.weight1) * data.expressionScale;
    const float spread = inverse * 0.004f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z -= weighted * 0.006f + inverse * 0.006f;
}

void ModifyInnerLipSpread(const FacialModelData &data, FacialMorphVertex &v) {
    const float spread = (((1.0f - v.weight1) * data.shapeCoefficients[9]) * data.expressionScale) * 0.001f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
}

void ModifyUpperLipArch(const FacialModelData &data, FacialMorphVertex &v) {
    const float weight = v.weight0 <= 0.25f ? (v.weight0 + 0.05f) * 2.5f : 1.0f - v.weight0;
    v.target.z = ((weight * data.shapeCoefficients[8]) * data.expressionScale) * 0.003f + v.target.z;
}

void ModifyLowerLipArch(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float scale = data.expressionScale;
    const float pivot = data.shapeCoefficients[12];
    const float lift = (((1.0f - v.weight0) * coefficient) * scale) * pivot;
    v.target.x -= (((v.weight0 * coefficient) * scale) * pivot) * 0.002f;
    const float spread = (((1.0f - v.weight1) * coefficient) * scale) * 0.004f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z -= (coefficient * 0.005f) * scale - lift * 0.0025f;
}

void ModifyUpperLipRetract(const FacialModelData &data, FacialMorphVertex &v) {
    const float amount = ((1.0f - v.weight0) * data.shapeCoefficients[8]) * data.expressionScale;
    const float spread = amount * 0.0005f;
    v.target.x -= amount * 0.0005f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = amount * 0.002f + v.target.z;
}

void ModifyLowerLipRetract(const FacialModelData &data, FacialMorphVertex &v) {
    const float amount = ((1.0f - v.weight1) * data.shapeCoefficients[9]) * data.expressionScale;
    const float spread = amount * 0.0005f;
    v.target.x -= amount * 0.0005f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = amount * 0.002f + v.target.z;
}

void ModifyUpperLipTension(const FacialModelData &data, FacialMorphVertex &v) {
    float weight = 0.8f - v.weight0;
    if (weight < 0.0f)
        weight = -weight;
    const float amount = weight * (data.shapeCoefficients[8] * data.expressionScale);
    const float spread = amount * 0.0042f;
    v.target.x -= amount * 0.002f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = amount * 0.004f + v.target.z;
}

void ModifyLowerLipTension(const FacialModelData &data, FacialMorphVertex &v) {
    float weight = 0.8f - v.weight1;
    if (weight < 0.0f)
        weight = -weight;
    const float coefficient = data.shapeCoefficients[9];
    const float scale = data.expressionScale;
    const float pivot = data.shapeCoefficients[12];
    const float amount = weight * (scale * coefficient);
    const float retreat = (((v.weight0 * coefficient) * scale) * pivot) * 0.001f;
    const float lift = (((1.0f - v.weight0) * coefficient) * scale) * pivot;
    const float spread = amount * 0.0042f;
    v.target.x -= retreat + amount * 0.002f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = (lift * 0.0015f + amount * 0.004f) + v.target.z;
}

void ModifyUpperLipCorner(const FacialModelData &data, FacialMorphVertex &v) {
    const float inverse = 1.0f - v.weight0;
    const float amount = (inverse * data.shapeCoefficients[8]) * data.expressionScale;
    float spread = amount * 0.001f;
    v.target.x = amount * 0.001f + v.target.x;
    float lift = amount * 0.002f;
    if (inverse > 0.9f && inverse < 1.0f) {
        lift = amount * 0.004f;
        spread *= 0.5f;
    }
    v.target.z = lift + v.target.z;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
}

void ModifyLowerLipCorner(const FacialModelData &data, FacialMorphVertex &v) {
    const float inverse = 1.0f - v.weight1;
    const float amount = (inverse * data.shapeCoefficients[9]) * data.expressionScale;
    float spread = amount * 0.001f;
    v.target.x = amount * 0.001f + v.target.x;
    float lift = amount * 0.002f;
    if (inverse > 0.9f && inverse < 1.0f) {
        lift = amount * 0.004f;
        spread *= 0.5f;
    }
    v.target.z = lift + v.target.z;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
}

void ModifyUpperLipTip(const FacialModelData &data, FacialMorphVertex &v) {
    const float relative = v.weight0 - 0.9f;
    const float weight = relative <= 0.0f ? relative + 0.9f : (0.1f - relative) * 9.0f;
    const float amount = (data.shapeCoefficients[8] * data.expressionScale) * weight;
    const float shift = amount * 0.001f;
    v.target.x = shift + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - shift : shift + v.target.y;
    v.target.z = shift + v.target.z;
}

void ModifyLowerLipTip(const FacialModelData &data, FacialMorphVertex &v) {
    const float relative = v.weight1 - 0.9f;
    const float weight = relative <= 0.0f ? relative + 0.9f : (0.1f - relative) * 9.0f;
    const float scale = data.expressionScale;
    const float coefficient = data.shapeCoefficients[9];
    const float pivot = data.shapeCoefficients[12];
    const float amount = weight * (scale * coefficient);
    const float spread = amount * 0.001f;
    v.target.x = (amount * 0.001f - (((v.weight0 * coefficient) * scale) * pivot) * 0.005f) + v.target.x;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z = ((((1.0f - v.weight0) * coefficient) * scale) * pivot * 0.0025f + amount * 0.002f) + v.target.z;
}

void ModifyUpperNegativeCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y < 0.0f)
        ModifyUpperLipCorner(data, v);
}
void ModifyLowerNegativeCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y < 0.0f)
        ModifyLowerLipCorner(data, v);
}
void ModifyUpperPositiveCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y > 0.0f)
        ModifyUpperLipCorner(data, v);
}
void ModifyLowerPositiveCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y > 0.0f)
        ModifyLowerLipCorner(data, v);
}

void ModifyLowerLipArchWeighted(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float scale = data.expressionScale;
    const float pivot = data.shapeCoefficients[12];
    const float lift = (((1.0f - v.weight0) * coefficient) * scale) * pivot;
    v.target.x -= (((v.weight0 * coefficient) * scale) * pivot) * 0.002f;
    const float amount = ((1.0f - v.weight1) * coefficient) * scale;
    const float spread = amount * 0.004f;
    v.target.y = v.neutral.y <= 0.0f ? v.target.y - spread : spread + v.target.y;
    v.target.z -= amount * 0.005f - lift * 0.0025f;
}

// SelectMouthModifiers publishes three ambient callbacks before every mouth shape.
void SelectMouthModifiers(const FacialModelData &data) {
    mouthModifiers = {};
    switch (data.expression) {
    case 1:
        mouthModifiers = {ModifyUpperLipTip, ModifyLowerLipTip, nullptr};
        break;
    case 2:
    case 5:
    case 22:
        mouthModifiers = {ModifyUpperPinchedLip, ModifyLowerPinchedLip, nullptr};
        break;
    case 3:
        mouthModifiers = {ModifyUpperSmile, ModifyLowerSmile, nullptr};
        break;
    case 6:
        mouthModifiers = {ModifyUpperAsymmetricLip, ModifyLowerAsymmetricLip, nullptr};
        break;
    case 9:
        mouthModifiers[1] = ModifyLowerLipDrop;
        break;
    case 10:
        mouthModifiers = {ModifyUpperLipCurl, ModifyLowerLipCurl, ModifyInnerLipSpread};
        break;
    case 11:
    case 14:
        mouthModifiers = {ModifyUpperLipArch, ModifyLowerLipArch, nullptr};
        break;
    case 12:
        mouthModifiers = {ModifyUpperLipRetract, ModifyLowerLipRetract, nullptr};
        break;
    case 13:
        mouthModifiers = {ModifyUpperLipTension, ModifyLowerLipTension, nullptr};
        break;
    case 15:
        mouthModifiers = {ModifyUpperRaisedLip, ModifyLowerRaisedLip, nullptr};
        break;
    case 16:
        mouthModifiers = {ModifyUpperNegativeCorner, ModifyLowerNegativeCorner, nullptr};
        break;
    case 17:
        mouthModifiers = {ModifyUpperPositiveCorner, ModifyLowerPositiveCorner, nullptr};
        break;
    case 18:
        mouthModifiers = {ModifyUpperLipCorner, ModifyLowerLipCorner, nullptr};
        break;
    case 19:
        mouthModifiers = {ModifyUpperWideSmile, ModifyLowerWideSmile, nullptr};
        break;
    case 20:
        mouthModifiers = {ModifyUpperLipArch, ModifyLowerLipArchWeighted, nullptr};
        break;
    case 21:
        mouthModifiers = {ModifyUpperHalfSmile, ModifyLowerHalfSmile, nullptr};
        break;
    }
}

template <class Transform>
void PrepareMouthGroup(FacialModelResource &face, std::size_t group, float duration, FacialShapeModifier modifier, Transform transform) {
    for (FacialMorphControl &control : face.vertices[group]) {
        auto &vertex = control.vertex;
        transform(vertex);
        if (modifier)
            modifier(face.data, vertex);
        vertex.target.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(vertex.target.x) | 1U);
        SnapshotImmediateFacialTarget(control, duration);
    }
}

void MarkMouthGroup(FacialModelResource &face, std::size_t group) {
    if (face.data.groups[group].count)
        face.data.flags |= group == 9 ? 0x800U : 1U << group;
}

void PrepareNeutralMouthGroup(FacialModelResource &face, std::size_t group, float duration) {
    BeginNeutralFacialGroup(face.vertices[group], duration);
    face.data.flags |= group == 9 ? 0x800U : 1U << group;
}


void PrepareMouthClosed(FacialModelResource &face, float duration) {
    SelectMouthModifiers(face.data);
    for (std::size_t slot = 0; slot != 3; ++slot) {
        constexpr std::array<std::size_t, 3> order{6, 8, 7};
        PrepareMouthGroup(face, order[slot], duration, mouthModifiers[slot], [](FacialMorphVertex &v) { v.target = v.neutral; });
    }
    MarkMouthGroup(face, 6);
    MarkMouthGroup(face, 8);
    PrepareNeutralMouthGroup(face, 9, duration);
    MarkMouthGroup(face, 7);
}


void PrepareMouthA(FacialModelResource &face, float duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face.data);
    const auto &coefficient = face.data.shapeCoefficients;
    const float upperDrop = (amount * 0.004f) * coefficient[8];
    const float width = 1.0f - (amount * 0.05f) * coefficient[10];
    PrepareMouthGroup(face, 6, duration, mouthModifiers[0], [=](FacialMorphVertex &v) {
        v.target = {v.neutral.x, width * v.neutral.y, v.neutral.z - (0.9f - v.weight0) * upperDrop};
    });
    const float lowerDrop = (amount * 0.011f) * coefficient[9];
    const float retreat = (amount * 0.0033f) * coefficient[9];
    PrepareMouthGroup(face, 8, duration, mouthModifiers[1], [=](FacialMorphVertex &v) {
        v.target = {v.neutral.x - retreat, width * v.neutral.y, v.neutral.z - lowerDrop * v.weight1};
    });
    const float innerDrop = (amount * 0.0066f) * coefficient[9];
    PrepareMouthGroup(face, 7, duration, mouthModifiers[2], [=](FacialMorphVertex &v) {
        const float weight = v.weight1 + v.weight0;
        v.target = {v.neutral.x - weight * innerDrop, v.neutral.y, v.neutral.z - weight * (innerDrop * 1.2f)};
    });
    MarkMouthGroup(face, 6);
    MarkMouthGroup(face, 8);
    PrepareNeutralMouthGroup(face, 9, duration);
    MarkMouthGroup(face, 7);
}


void PrepareMouthI(FacialModelResource &face, float duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face.data);
    const auto &coefficient = face.data.shapeCoefficients;
    const float size = amount * 0.005f;
    const float retreat = size * coefficient[11];
    const float elevation = (amount * 0.002f) * coefficient[8];
    const float width = (amount * 0.1f) * coefficient[10] + 1.0f;
    PrepareMouthGroup(face, 6, duration, mouthModifiers[0], [=](FacialMorphVertex &v) {
        const float offset = v.weight0 >= 0.3f ? 0.0f : (0.3f - v.weight0) * retreat;
        v.target = {v.neutral.x - offset * 4.0f, width * v.neutral.y, v.weight0 * elevation + v.neutral.z};
    });
    const float lowerDrop = size * coefficient[9];
    PrepareMouthGroup(face, 8, duration, mouthModifiers[1], [=](FacialMorphVertex &v) {
        const float offset = v.weight1 >= 0.3f ? 0.0f : (0.3f - v.weight1) * retreat;
        v.target = {v.neutral.x - offset * 4.0f, width * v.neutral.y, v.neutral.z - v.weight1 * lowerDrop};
    });
    PrepareNeutralMouthGroup(face, 7, duration);
    const float cornerWidth = (amount * 0.05f) * coefficient[10] + 1.0f;
    PrepareMouthGroup(face, 9, duration, nullptr, [=](FacialMorphVertex &v) { v.target = {v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z}; });
    MarkMouthGroup(face, 9);
    MarkMouthGroup(face, 6);
    MarkMouthGroup(face, 8);
}


void PrepareMouthU(FacialModelResource &face, float duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face.data);
    const auto &coefficient = face.data.shapeCoefficients;
    const float advanceScale = amount * 0.003f;
    const float advance = advanceScale * coefficient[8];
    const float drop = (amount * 0.005f) * coefficient[11];
    const float elevation = (amount * 0.0015f) * coefficient[8];
    const float width = 1.0f - (amount * 0.3f) * coefficient[10];
    PrepareMouthGroup(face, 6, duration, mouthModifiers[0], [=](FacialMorphVertex &v) {
        const float offset = v.weight0 >= 0.3f ? 0.0f : (0.3f - v.weight0) * drop;
        v.target = {v.weight0 * advance + v.neutral.x, width * v.neutral.y, (v.weight0 * elevation + v.neutral.z) - offset};
    });
    const float lowerAdvance = advanceScale * coefficient[9];
    PrepareMouthGroup(face, 8, duration, mouthModifiers[1], [=](FacialMorphVertex &v) {
        const float offset = v.weight1 >= 0.3f ? 0.0f : (0.3f - v.weight1) * drop;
        v.target = {v.weight1 * lowerAdvance + v.neutral.x, width * v.neutral.y, v.neutral.z - offset};
    });
    const float cornerWidth = 1.0f - (amount * 0.1f) * coefficient[10];
    PrepareMouthGroup(face, 9, duration, nullptr, [=](FacialMorphVertex &v) { v.target = {v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z}; });
    MarkMouthGroup(face, 9);
    MarkMouthGroup(face, 6);
    MarkMouthGroup(face, 8);
    PrepareNeutralMouthGroup(face, 7, duration);
}


void PrepareMouthE(FacialModelResource &face, float duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face.data);
    const auto &coefficient = face.data.shapeCoefficients;
    const float widthScale = amount * 0.05f;
    const float elevation = (amount * 0.0015f) * coefficient[8];
    const float retreat = (amount * 0.016f) * coefficient[11];
    const float width = widthScale * coefficient[10] + 1.0f;
    PrepareMouthGroup(face, 6, duration, mouthModifiers[0], [=](FacialMorphVertex &v) {
        const float offset = v.weight0 >= 0.3f ? 0.0f : (0.3f - v.weight0) * retreat;
        v.target = {v.neutral.x - offset, width * v.neutral.y, v.weight0 * elevation + v.neutral.z};
    });
    const float lowerRetreat = retreat + retreat;
    const float lowerDrop = (amount * 0.007f) * coefficient[9];
    PrepareMouthGroup(face, 8, duration, mouthModifiers[1], [=](FacialMorphVertex &v) {
        const float offset = v.weight1 >= 0.3f ? 0.0f : (0.3f - v.weight1) * lowerRetreat;
        v.target = {v.neutral.x - offset, width * v.neutral.y, v.neutral.z - v.weight1 * lowerDrop};
    });
    const float innerDrop = (amount * 0.004f) * coefficient[9];
    PrepareMouthGroup(face, 7, duration, mouthModifiers[2], [=](FacialMorphVertex &v) {
        const float offset = (v.weight1 + v.weight0) * innerDrop;
        v.target = {v.neutral.x - offset, v.neutral.y, v.neutral.z - offset};
    });
    const float cornerWidth = widthScale * coefficient[10] + 1.0f;
    PrepareMouthGroup(face, 9, duration, nullptr, [=](FacialMorphVertex &v) { v.target = {v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z}; });
    MarkMouthGroup(face, 9);
    MarkMouthGroup(face, 6);
    MarkMouthGroup(face, 8);
    MarkMouthGroup(face, 7);
}

// PrepareMouthO omits both outer lip modifiers, but retains the inner one.
void PrepareMouthO(FacialModelResource &face, float duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face.data);
    const auto &coefficient = face.data.shapeCoefficients;
    const float advanceScale = amount * 0.003f;
    const float advance = advanceScale * coefficient[8];
    const float upperDrop = (amount * 0.001f) * coefficient[8];
    const float width = 1.0f - (amount * 0.4f) * coefficient[10];
    PrepareMouthGroup(face, 6, duration, nullptr, [=](FacialMorphVertex &v) {
        v.target = {advance * v.weight0 + v.neutral.x, width * v.neutral.y, (v.weight0 - 1.0f) * upperDrop + v.neutral.z};
    });
    const float lowerDrop = (amount * 0.005f) * coefficient[9];
    const float lowerAdvance = coefficient[9] * advanceScale;
    const float lowerWidth = (amount * 0.24f) * coefficient[10];
    PrepareMouthGroup(face, 8, duration, nullptr, [=](FacialMorphVertex &v) {
        v.target = {lowerAdvance * v.weight1 + v.neutral.x, (1.0f - (v.weight1 + 1.0f) * lowerWidth) * v.neutral.y, v.neutral.z - lowerDrop * v.weight1};
    });
    const float cornerWidth = 1.0f - (amount * 0.1f) * coefficient[10];
    PrepareMouthGroup(face, 9, duration, nullptr, [=](FacialMorphVertex &v) { v.target = {v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z}; });
    const float innerDrop = (amount * 0.004f) * coefficient[9];
    PrepareMouthGroup(face, 7, duration, mouthModifiers[2], [=](FacialMorphVertex &v) {
        const float weight = v.weight1 + v.weight0;
        v.target = {v.neutral.x - weight * (innerDrop * 0.5f), v.neutral.y, v.neutral.z - weight * innerDrop};
    });
    MarkMouthGroup(face, 6);
    MarkMouthGroup(face, 8);
    MarkMouthGroup(face, 9);
    MarkMouthGroup(face, 7);
}

// PrepareMouthLeadIn: two-frame consonant lead-in after A/E.
void PrepareMouthLeadIn(FacialModelResource &face, float duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face.data);
    PrepareMouthGroup(face, 6, duration, mouthModifiers[0], [](FacialMorphVertex &v) { v.target = v.neutral; });
    const float lowerDrop = (amount * 0.005f) * face.data.shapeCoefficients[9];
    PrepareMouthGroup(face, 8, duration, mouthModifiers[1], [=](FacialMorphVertex &v) {
        v.target = {v.neutral.x, v.neutral.y, v.neutral.z - lowerDrop * v.weight1};
    });
    const float innerDrop = (amount * 0.002f) * face.data.shapeCoefficients[9];
    PrepareMouthGroup(face, 7, duration, mouthModifiers[2], [=](FacialMorphVertex &v) {
        v.target = {v.neutral.x - v.weight0 * (innerDrop * 0.5f), v.neutral.y, v.neutral.z - v.weight0 * innerDrop};
    });
    MarkMouthGroup(face, 6);
    MarkMouthGroup(face, 8);
    PrepareNeutralMouthGroup(face, 9, duration);
    MarkMouthGroup(face, 7);
}

void PrepareFacialMouth(FacialModelResource &face, const void *model, std::int32_t expression, float duration, float scale) {
    d3t::SetObjectMouthExpression(GetMT7ReaderData(static_cast<const MT7Model *>(model)), static_cast<std::uint32_t>(expression), duration, scale);
    face.data.mouthDuration = duration;
    face.data.mouthFrame = 0.0f;
    switch (expression) {
    case 0:
        PrepareMouthClosed(face, duration);
        break;
    case 1:
        PrepareMouthA(face, duration, scale);
        break;
    case 2:
        PrepareMouthI(face, duration, scale);
        break;
    case 3:
        PrepareMouthU(face, duration, scale);
        break;
    case 4:
        PrepareMouthE(face, duration, scale);
        break;
    case 5:
        PrepareMouthO(face, duration, scale);
        break;
    case 6:
        if (face.data.mouthExpression == 1 || face.data.mouthExpression == 4)
            PrepareMouthLeadIn(face, duration, scale);
        else if (face.data.mouthExpression == 5)
            PrepareMouthO(face, duration, scale * 0.5f);
        else
            PrepareNeutralMouthGroup(face, 7, duration);
        break;
    }
    face.data.mouthExpression = expression;
}

void PrepareLowerFace(FacialModelResource &face, const void *model, std::int32_t expression, float duration, float scale) {
    d3t::SetObjectFacialExpression(
        GetMT7ReaderData(static_cast<const MT7Model *>(model)), static_cast<std::uint32_t>(expression), d3t::FacialExpressionLayer::Lower, duration, scale);
    FacialModelData &data = face.data;
    data.lowerFaceDuration = duration;
    data.expressionScale = scale;
    data.expression = expression;
    data.lowerFaceFrame = 0.0f;
    switch (expression) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
    case 7:
    case 11:
    case 12:
    case 20:
    case 22:
        BeginNeutralFacialGroup(face.vertices[3], duration);
        data.flags |= 8U;
        break;
    case 3:
    case 8:
    case 9:
    case 10:
    case 14:
    case 15:
    case 19:
        PrepareCheekSmile(face, duration, scale);
        break;
    case 6:
    case 16:
        PrepareNegativeCheek(face, duration, scale);
        break;
    case 13:
    case 21:
        PrepareCheekSmile(face, duration, scale * 0.5f);
        break;
    case 17:
        PreparePositiveCheek(face, duration, scale);
        break;
    }
    if (data.mouthDuration <= std::numeric_limits<float>::epsilon())
        PrepareFacialMouth(face, model, data.mouthExpression, duration, scale);
}

void UpdateFacialMorphs(FacialModelResource &face) {
    FacialModelData &data = face.data;
    const float delta = g_stabilizeFps != 0 ? g_stabilizedFrameTime * 30.0f : 1.0f;
    if (data.eyelidDuration > 0.0f) {
        if (data.eyelidDuration <= data.eyelidFrame) {
            if (data.flags & 0x200U) {
                if (data.flags & 0x400U) {
                    FinishEyelids(face);
                    data.flags &= ~0x600U;
                    data.eyelidFrame = 0.0f;
                    data.eyelidDuration = 3.0f;
                    PrepareOuterEyelids(face, 3.0f, data.eyelid0OuterWeight, data.eyelid1OuterWeight);
                    PrepareInnerEyelids(face, 3.0f, data.eyelid0InnerWeight, data.eyelid1InnerWeight);
                }
            } else {
                FinishEyelids(face);
                data.eyelidDuration = 0.0f;
            }
        } else {
            if (data.eyelidFrame > 0.0f)
                InterpolateEyelids(face);
            data.eyelidFrame = delta + data.eyelidFrame;
        }
    }
    const auto updateFace = [&](std::size_t index, float &frame, float &duration) {
        if (duration <= 0.0f)
            return;

        const std::uint32_t flag = 1U << index;
        if (duration <= frame) {
            if (data.flags & flag) {
                FinishFacialGroup(face.vertices[index]);
                data.flags &= ~flag;
            }
            duration = 0.0f;
        } else {
            if (frame > 0.0f && (data.flags & flag))
                InterpolateFacialGroup(face.vertices[index], frame, duration);
            frame = delta + frame;
        }
    };
    updateFace(0, data.upperFaceFrame, data.upperFaceDuration);
    updateFace(3, data.lowerFaceFrame, data.lowerFaceDuration);
    if (data.mouthDuration > 0.0f) {
        const bool finished = data.mouthDuration <= data.mouthFrame;
        if (finished || data.mouthFrame > 0.0f) {
            for (const std::size_t index : {6U, 8U, 9U, 7U}) {
                const std::uint32_t flag = index == 9 ? 0x800U : 1U << index;
                if (!(data.flags & flag))
                    continue;

                if (finished) {
                    FinishFacialGroup(face.vertices[index]);
                    data.flags &= ~flag;
                } else
                    InterpolateFacialGroup(face.vertices[index], data.mouthFrame, data.mouthDuration);
            }
        }

        if (finished)
            data.mouthDuration = 0.0f;
        else
            data.mouthFrame = delta + data.mouthFrame;
    }
    UploadFacialModel(face);
}

FacialTaskData *FindFacialTask(const Character *character) {
    return character ? static_cast<FacialTaskData *>(FindModelComponent(character, faceToken)) : nullptr;
}

FacialModelResource *FindFacialModel(const Character *character) {
    if (!character || !character->facialModel || !character->model)
        return nullptr;
    return reinterpret_cast<FacialModelResource *>(character->facialModel);
}

// The head rig's facing for setEyeLookFlags: bone 5's X axis on the XZ plane, or the entity yaw
// when that axis is vertical.
// @verified - p
std::int16_t GetEyeLookHeadYaw(Character *character) {
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(charaGetBoneTransform(FindMotionWork(character), 5));
    const Vector4 origin = workTransformMatrix.rows[3];
    Matrix4x4::preTranslate(100.0f, 0.0f, 0.0f);
    const float x = workTransformMatrix.rows[3].x - origin.x;
    const float z = workTransformMatrix.rows[3].z - origin.z;
    Matrix4x4::Pop(1);
    if (z * z + x * x >= 1.0f)
        return static_cast<std::int16_t>(FixedAtan2(-z, x));
    return static_cast<std::int16_t>(FindMotionWork(character)->runtime.entityRotation.y);
}

// setEyeLookFlags: the target is measured from the solved head rig, not from the drawn node.
// @verified - p
void ResolveFacialEyeTarget(Character *character, FacialEyeControl &eyes) {
    if (!charaGetBoneTransform(FindMotionWork(character), 5) || !eyes.headNode)
        return;

    if ((eyes.rotationFlags & 0x7fU) != 0) {
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(charaGetBoneTransform(FindMotionWork(character), 5));
        Vector3 origin{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        const Vector3 &offset = eyes.headNode->position;
        Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
        origin.y = workTransformMatrix.rows[3].y;
        Matrix4x4::Pop(1);
        const std::int16_t baseAngle = GetEyeLookHeadYaw(character);
        const float x = eyes.target.x - origin.x;
        const float z = eyes.target.z - origin.z;
        const std::int32_t pitch = static_cast<std::int16_t>(FixedAtan2(origin.y - eyes.target.y, std::sqrt(z * z + x * x))) / 4;
        const std::int32_t yaw = static_cast<std::int16_t>(FixedAtan2(-z, x) - baseAngle) / 4;
        eyes.targetHeadRotation.x = eyes.targetEyeRotation.x = pitch;
        eyes.targetHeadRotation.y = eyes.targetEyeRotation.y = static_cast<std::uint16_t>(yaw);
    }
    std::int32_t duration = static_cast<std::int16_t>(eyes.transitionFrames - eyes.elapsedFrames);
    if (duration < 1)
        duration = 1;
    constexpr std::int32_t halfwordSign = static_cast<std::int32_t>(0xffff0000U);
    if (eyes.targetHeadRotation.x & 0x8000) {
        eyes.targetEyeRotation.x |= halfwordSign;
        eyes.targetHeadRotation.x |= halfwordSign;
    }
    if (eyes.targetHeadRotation.y & 0x8000)
        eyes.targetHeadRotation.y |= halfwordSign;
    if (eyes.targetEyeRotation.y & 0x8000)
        eyes.targetEyeRotation.y |= halfwordSign;

    if (eyes.targetHeadRotation.x > eyes.pitchUpper)
        eyes.targetHeadRotation.x = eyes.targetEyeRotation.x = eyes.pitchUpper;
    else if (eyes.targetHeadRotation.x < eyes.pitchLower)
        eyes.targetHeadRotation.x = eyes.targetEyeRotation.x = eyes.pitchLower;

    if (eyes.targetHeadRotation.y > eyes.headYawUpper)
        eyes.targetHeadRotation.y = eyes.headYawUpper;
    else if (eyes.targetHeadRotation.y < eyes.headYawLower)
        eyes.targetHeadRotation.y = eyes.headYawLower;

    if (eyes.targetEyeRotation.y > eyes.eyeYawUpper)
        eyes.targetEyeRotation.y = eyes.eyeYawUpper;
    else if (eyes.targetEyeRotation.y < eyes.eyeYawLower)
        eyes.targetEyeRotation.y = eyes.eyeYawLower;

    eyes.headRotationStep.x = (eyes.targetHeadRotation.x - eyes.headRotation.x) / duration;
    eyes.headRotationStep.y = (static_cast<std::int16_t>(eyes.targetHeadRotation.y) - static_cast<std::int16_t>(eyes.headRotation.y)) / duration;
    eyes.eyeRotationStep[0] = (eyes.targetEyeRotation.x - eyes.eyeRotation.x) / duration;
    eyes.eyeRotationStep[1] = (static_cast<std::int16_t>(eyes.targetEyeRotation.y) - static_cast<std::int16_t>(eyes.eyeRotation.y)) / duration;
}

// AdvanceFacialEyeRotation. Mode 2 remains live after the transition completes.
void AdvanceFacialEyeRotation(FacialEyeControl &eyes) {
    eyes.elapsedFrames = static_cast<std::int16_t>(eyes.elapsedFrames + 1);
    if (eyes.elapsedFrames > eyes.transitionFrames) {
        const std::uint8_t mode = eyes.rotationFlags & 0x7fU;
        if (mode != 2)
            eyes.rotationFlags = mode;

        eyes.elapsedFrames = eyes.transitionFrames;
        eyes.headRotation.x = eyes.targetHeadRotation.x;
        eyes.headRotation.y = eyes.targetHeadRotation.y;
        eyes.eyeRotation.x = eyes.targetEyeRotation.x;
        eyes.eyeRotation.y = eyes.targetEyeRotation.y;
    } else {
        eyes.headRotation.x += eyes.headRotationStep.x;
        eyes.headRotation.y += eyes.headRotationStep.y;
        eyes.eyeRotation.x += eyes.eyeRotationStep[0];
        eyes.eyeRotation.y += eyes.eyeRotationStep[1];
    }
    if (eyeControlFlags & 1U) {
        if (eyes.headRotation.x > eyes.pitchUpper)
            eyes.headRotation.x = eyes.eyeRotation.x = eyes.pitchUpper;
        else if (eyes.headRotation.x < eyes.pitchLower)
            eyes.headRotation.x = eyes.eyeRotation.x = eyes.pitchLower;

        if (eyes.headRotation.y > eyes.headYawUpper)
            eyes.headRotation.y = eyes.headYawUpper;
        else if (eyes.headRotation.y < eyes.headYawLower)
            eyes.headRotation.y = eyes.headYawLower;
        if (eyes.eyeRotation.y > eyes.eyeYawUpper)
            eyes.eyeRotation.y = eyes.eyeYawUpper;
        else if (eyes.eyeRotation.y < eyes.eyeYawLower)
            eyes.eyeRotation.y = eyes.eyeYawLower;
    }
}



void facxApplyEyeControl(Character *character, FacialTaskData &task) {
    FacialEyeControl &eyes = task.eyes;
    switch (eyes.rotationFlags & 0x7fU) {
    case 1:
        if (g_facialFocusId == facialFocusNone) {
            task.focusId = facialFocusNone;
        } else if (task.focusId == g_facialFocusId) {
            ResolveFacialEyeTarget(character, eyes);
        } else {
            task.focusId = g_facialFocusId;
            const void *const clip = FindModelComponent(character, CHARACTER_CLIPPER_COMPONENT);
            if (clip) {
                std::uint16_t clipFocusId;
                std::memcpy(&clipFocusId, static_cast<const std::byte *>(clip) + characterClipFocusIdOffset, sizeof(clipFocusId));
                if (clipFocusId != g_facialFocusId)
                    SetEyeToChara(character, &g_facialFocusTarget, 8, 1);
            }
        }
        break;
    case 2:
        ResolveFacialEyeTarget(character, eyes);
        break;
    }
    const void *const placement = GetMT7ReaderData(static_cast<const MT7Model *>(character->model));
    d3t::ApplyEyeLookTarget(placement, 0, 0, eyes.eyeRotation.x, static_cast<std::int16_t>(eyes.eyeRotation.y), &eyes.target);
    d3t::ApplyEyeLookTarget(placement, 1, 0, eyes.headRotation.x, static_cast<std::int16_t>(eyes.headRotation.y), &eyes.target);

    if (!(eyes.rotationFlags & 0x80U))
        return;

    AdvanceFacialEyeRotation(eyes);
    const float pitch = eyes.headRotation.x > 0 && eyes.pitchUpper != 0 ? static_cast<float>(eyes.headRotation.x) / static_cast<float>(eyes.pitchUpper) : 0.0f;
    if (FacialModelResource *const face = FindFacialModel(character)) {
        face->data.shapeCoefficients[29] = pitch * 0.7f;
        if (face->data.eyelid0Mode && !(face->data.flags & 0x600U))
            PrepareOuterEyelids(*face, std::fmax(face->data.eyelidDuration, 0.0f), face->data.eyelid0OuterWeight, face->data.eyelid1OuterWeight);
    }

    if (eyes.headNode)
        eyes.headNode->rotation = {0, eyes.headRotation.x, eyes.headRotation.y};
    if (eyes.eyeNode)
        eyes.eyeNode->rotation = {0, eyes.eyeRotation.x, eyes.eyeRotation.y};
}




void UpdateFacialTask(Character *character, void *payload) {
    auto &task = *static_cast<FacialTaskData *>(payload);
    const Vector3 position = GetCharacterWorldPosition(character);
    if (!g_cameraControl)
        return;

    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    const float x = position.x - camera.x;
    const float z = position.z - camera.z;
    if (!(z * z + x * x <= 100.0f))
        return;

    if (task.clip && ++task.clipFrame > task.clipDuration) {
        const std::int16_t *const clip = task.clip;
        if (clip[0] >= 0) {
            const std::int32_t duration = clip[1] / 2;
            std::uint8_t expression = clipMouthExpressions[clip[0] & 0xf];
            task.clipFrame = 1;
            task.clipDuration = duration;
            if (!task.clipLeadIn) {
                if ((clip[0] & 0xfff0) && expression != 3 && duration > 2) {
                    task.clipDuration = 2;
                    expression = 6;
                    task.clipLeadIn = 1;
                }
            } else {
                task.clipLeadIn = 0;
                task.clipDuration = duration - 2;
            }

            if (!task.clipLeadIn)
                task.clip = clip + 2;

            if (FacialModelResource *const face = FindFacialModel(character))
                PrepareFacialMouth(*face, character->model, expression, static_cast<float>(std::clamp(duration, 3, 10)), 1.0f);
        } else {
            task.clip = nullptr;
            task.blinkFrame = 0;
        }
    }
    FacialModelResource *const face = FindFacialModel(character);
    if (task.blinkInterval && ++task.blinkFrame > task.blinkInterval) {
        if (face)
            BeginFacialBlink(*face);
        facialRandomState = facialRandomState * 0x41c64e6dU + 12345U;
        task.blinkFrame = 30 * ((facialRandomState >> 16) & 3U);
    }
    if (face)
        UpdateFacialMorphs(*face);
    facxApplyEyeControl(character, task);
}

} // namespace

bool ReadFacialModelResource(MT7Model &model, std::span<const std::byte> container) {
    std::size_t faceOffset = 0;
    std::size_t cursor = 0;
    bool foundFace = false;
    bool foundModel = false;
    while (cursor < container.size()) {
        std::array<std::uint32_t, 2> header{};
        if (!ReadFaceValue(container, cursor, header) || header[1] < 8 || header[1] > container.size() - cursor)
            break;
        const std::uint32_t signature = header[0];
        const bool modelChunk = signature == SHEN_NAME(MDL7) || signature == SHEN_NAME(MDO7) || signature == SHEN_NAME(MDC7) || signature == SHEN_NAME(MDP7) ||
            signature == SHEN_NAME(MDLX) || signature == SHEN_NAME(MDOX) || signature == SHEN_NAME(MDCX) || signature == SHEN_NAME(MDPX);
        if (modelChunk) {
            if (foundModel)
                break;
            foundModel = true;
        } else if (signature == faceSignature || signature == faceToken.name) {
            if (foundFace)
                break;
            foundFace = true;
            faceOffset = cursor;
        } else if (signature != SHEN_NAME(LIGT) && signature != SHEN_NAME(CLSG) && signature != SHEN_NAME(CLSX)) {
            break;
        }
        cursor += header[1];
    }

    if (!foundFace && std::none_of(model.nodes.begin(), model.nodes.end(), [](const MT7Node &node) {
            return node.parent >= 0 && (static_cast<std::uint32_t>(node.id) & 0xffffU) == 0x46U;
        }))
        return true;

    auto face = std::unique_ptr<FacialModelResource>(new (std::nothrow) FacialModelResource{});
    if (!face)
        return false;
    if (foundFace && !ReadFaceValue(container, faceOffset, face->data))
        return false;

    if (!foundFace || face->data.version != 4) {
        InitializeDefaultFacialModel(face->data);
    } else {

        ResetFacialModelTransitions(face->data);

        for (std::size_t index = 0; index < face->vertices.size(); ++index) {
            const FacialMorphGroup &descriptor = face->data.groups[index];
            if (descriptor.count <= 0)
                continue;

            const std::int64_t begin = static_cast<std::int64_t>(faceOffset) + descriptor.offset;
            if (begin < 0 || static_cast<std::uint64_t>(begin) >= container.size())
                return false;

            std::size_t recordOffset = static_cast<std::size_t>(begin);
            auto &group = face->vertices[index];
            group.reserve(static_cast<std::size_t>(descriptor.count));
            for (std::int32_t recordIndex = 0; recordIndex < descriptor.count; ++recordIndex) {
                FacialMorphControl control{};
                if (!ReadFaceValue(container, recordOffset, control.vertex))
                    return false;

                if (model.signature == SHEN_NAME(MDCX)) {
                    std::int32_t words{};
                    const std::int64_t wordCountOffset = std::int64_t(control.vertex.bufferOffset) + 28;
                    if (wordCountOffset < 0 || !ReadFaceValue(model.source, static_cast<std::size_t>(wordCountOffset), words))
                        return false;

                    const std::int64_t position = std::int64_t(control.vertex.bufferOffset) + 44 + std::int64_t(words) * 4 + control.vertex.positionOffset;
                    if (position < 0 || position > std::numeric_limits<std::int32_t>::max())
                        return false;
                    control.vertex.positionOffset = static_cast<std::int32_t>(position);
                }

                if (!BindFacialVertex(model, control))
                    return false;

                recordOffset += (control.vertex.bufferOffset & 1U) ? 0x30U : 0x34U;
                group.push_back(control);
            }
        }
    }
    model.facial = std::move(face);
    return true;
}

namespace {
void RebuildFacialNodeParents(MT7Model &model, std::int32_t index, std::int32_t parent) {
    while (index >= 0) {
        MT7Node &node = model.nodes[static_cast<std::size_t>(index)];
        node.parent = parent;
        if (node.firstChild >= 0)
            RebuildFacialNodeParents(model, node.firstChild, index);
        index = node.nextSibling;
    }
}
}

void GraftFacialModelNode(MT7Model &model, FacialModelResource &face) {
    if (face.originalNode || face.replacementNode)
        return;
    MT7Node &root = model.nodes[0];
    MT7Node *const head = GetChildObjectByIndex(model, &root, 0x46);
    if (head) {
        MT7Node *original = GetChildObjectByIndex(model, &root, 0xffbd);
        if (!original)
            original = GetChildObjectByIndex(model, &root, 0xffbd);
        if (!original) {
            head->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(head->id) & 0xffff0000U) | 0xffbdU);
        } else {
            head->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(head->id) & 0xffff0000U) | static_cast<std::uint16_t>(original->id));
            const std::int32_t headIndex = static_cast<std::int32_t>(head - model.nodes.data());
            const std::int32_t originalIndex = static_cast<std::int32_t>(original - model.nodes.data());
            MT7Node &headParent = model.nodes[static_cast<std::size_t>(head->parent)];
            if (headParent.firstChild == headIndex)
                headParent.firstChild = head->nextSibling;
            else {
                for (std::int32_t index = headParent.firstChild; index >= 0; index = model.nodes[static_cast<std::size_t>(index)].nextSibling) {
                    MT7Node &node = model.nodes[static_cast<std::size_t>(index)];
                    if (node.nextSibling == headIndex) {
                        node.nextSibling = head->nextSibling;
                        break;
                    }
                }
            }
            head->nextSibling = original->nextSibling;
            if (head->firstChild < 0)
                head->firstChild = original->firstChild;
            else {
                std::int32_t index = head->firstChild;
                while (model.nodes[static_cast<std::size_t>(index)].nextSibling >= 0)
                    index = model.nodes[static_cast<std::size_t>(index)].nextSibling;
                model.nodes[static_cast<std::size_t>(index)].nextSibling = original->firstChild;
            }
            face.originalNode = original;
            face.replacementNode = head;
            MT7Node &parent = model.nodes[static_cast<std::size_t>(original->parent)];
            bool replaced = false;
            if (parent.firstChild == originalIndex) {
                parent.firstChild = headIndex;
                replaced = true;
            } else {
                for (std::int32_t index = parent.firstChild; index >= 0; index = model.nodes[static_cast<std::size_t>(index)].nextSibling) {
                    MT7Node &node = model.nodes[static_cast<std::size_t>(index)];
                    if (node.nextSibling == originalIndex) {
                        node.nextSibling = headIndex;
                        replaced = true;
                        break;
                    }
                }
            }
            if (replaced)
                head->facialData =
                    face.data.modelOffset ? static_cast<FacialModelData *>(HLib::GetPointerFromDynamicHeapOffsetBase(face.data.modelOffset)) : nullptr;
        }
    }
    root.parent = -1;
    if (root.firstChild >= 0)
        RebuildFacialNodeParents(model, root.firstChild, 0);
    if (root.nextSibling >= 0)
        RebuildFacialNodeParents(model, root.nextSibling, -1);
    InitializeMT7RuntimeLinks(model);
}

void SetFacialBlinkDelay(Character *character, std::int32_t delay) {
    if (FacialTaskData *const task = FindFacialTask(character))
        task->blinkInterval = delay ? std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(delay) + 90U) : 0;
}


MT7Node *FindFacialSubnode(MT7Node *node, std::uint16_t id) {
    if (static_cast<std::uint16_t>(node->id) == id)
        return node;
    if (MT7Node *const child = mtGetTransform(node)) {
        MT7Node *const found = FindFacialSubnode(child, id);
        if (static_cast<std::uint16_t>(found->id) == id)
            return found;
    }
    if (MT7Node *const sibling = mtGetSibling(node)) {
        MT7Node *const found = FindFacialSubnode(sibling, id);
        if (static_cast<std::uint16_t>(found->id) == id)
            return found;
    }
    return node;
}

bool AttachFacialController(Character *character) {
    eyeControlFlags &= 0xffffff00U;
    FacialModelResource *const face = FindFacialModel(character);
    if (!face)
        return true;

    auto *const task = static_cast<FacialTaskData *>(AllocateModelComponent(character, faceToken, sizeof(FacialTaskData), UpdateFacialTask));
    if (!task)
        return false;

    task->clip = nullptr;
    task->clipFrame = task->clipDuration = 0;
    task->reserved18 = task->clipLeadIn = 0;
    task->blinkInterval = 210;
    task->blinkFrame = 0;
    task->focusId = 129;
    task->enabled = 1;
    FacialEyeControl &eyes = task->eyes;
    eyes = {};
    eyes.reserved00[12] = std::byte{1};
    const auto &limits = face->data.lookLimits;
    eyes.pitchUpper = limits[1];
    eyes.pitchLower = -limits[0];
    eyes.headYawUpper = limits[2];
    eyes.headYawLower = -limits[3];
    eyes.eyeYawUpper = limits[5];
    eyes.eyeYawLower = -limits[4];
    MT7Model &model = *static_cast<MT7Model *>(character->model);
    if (model.nodes.empty())
        return true;

    MT7Node *const root = &model.nodes.front();
    eyes.headNode = FindFacialSubnode(root, 0x51);
    if (eyes.headNode == root) {
        eyes.headNode = FindFacialSubnode(root, 0x4d);
        if (eyes.headNode == root)
            eyes.headNode = nullptr;
    }
    eyes.eyeNode = FindFacialSubnode(root, 0x52);
    if (eyes.eyeNode == root) {
        eyes.eyeNode = FindFacialSubnode(root, 0x4e);
        if (eyes.eyeNode == root)
            eyes.eyeNode = nullptr;
    }

    return true;
}

bool AssignFacialClip(Character *character, const std::int16_t *clip) {
    FacialTaskData *const task = FindFacialTask(character);
    if (!task || !clip || *clip < 0)
        return false;
    task->clip = clip;
    task->clipFrame = 1;
    task->clipLeadIn = 0;
    return true;
}

void SetFaceFlagsForChara(Character *character, std::int32_t enabled) {
    if (FacialTaskData *const task = FindFacialTask(character))
        task->enabled = enabled;
}



void SetFaceFlagsForChara_0(Character *character, std::int32_t enabled) {
    if (!character)
        return;
    auto *const face = reinterpret_cast<FacialModelResource *>(character->facialModel);
    if (face && face->data.modelOffset) {
        auto *const entity = static_cast<FacialModelData *>(HLib::GetPointerFromDynamicHeapOffsetBase(face->data.modelOffset));
        MT7Node *const selected = enabled ? face->replacementNode : face->originalNode;
        selected->facialData = entity;
        if (character->model) {
            auto &model = *static_cast<MT7Model *>(character->model);
            RebuildFacialNodeParents(model, 0, -1);
            InitializeMT7RuntimeLinks(model);
        }
    }
    mtwkSetSequenceFlags(character);
}

bool IsFacialClipActive(const Character *character) {
    const FacialTaskData *const task = FindFacialTask(character);
    return task && task->clip;
}

void SetEyeToChara(Character *character, const Vector3 *target, std::int16_t frames, std::uint8_t mode) {
    FacialTaskData *const task = FindFacialTask(character);
    if (!task)
        return;
    FacialEyeControl &eyes = task->eyes;
    eyes.transitionFrames = frames;
    eyes.rotationFlags = mode | 0x80U;
    eyes.elapsedFrames = 0;
    eyes.target = {};
    if (mode) {
        if (target)
            eyes.target = *target;
    } else {
        eyes.targetHeadRotation.x = eyes.targetHeadRotation.y = 0;
        eyes.targetEyeRotation.x = eyes.targetEyeRotation.y = 0;
    }
    ResolveFacialEyeTarget(character, eyes);
}

void SetHighModelAndFaceFlagsForChara(Character *character, std::int32_t expression, float transitionFrames, float scale) {
    if (!character)
        return;
    if (auto *const face = reinterpret_cast<FacialModelResource *>(character->facialModel)) {
        PrepareUpperFace(*face, character->model, expression, transitionFrames, scale);
        PrepareLowerFace(*face, character->model, expression, transitionFrames, scale);
    }
}


void SetUpperFacialExpression(Character *character, std::int32_t expression, float transitionFrames, float scale) {
    if (FacialModelResource *const face = FindFacialModel(character))
        PrepareUpperFace(*face, character->model, expression, transitionFrames, scale > 2.0f ? 1.0f : scale);
}


void SetLowerFacialExpression(Character *character, std::int32_t expression, float transitionFrames, float scale) {
    if (FacialModelResource *const face = FindFacialModel(character))
        PrepareLowerFace(*face, character->model, expression, transitionFrames, scale);
}

void SetFacialMouthExpression(Character *character, std::int32_t expression, float transitionFrames, float scale) {
    if (FacialModelResource *const face = FindFacialModel(character))
        PrepareFacialMouth(*face, character->model, expression, transitionFrames, scale);
}


void StartFacialBlink(Character *character) {
    if (FacialModelResource *const face = FindFacialModel(character))
        BeginFacialBlink(*face);
}

void AT_SetEyeCtrl(Character *character, float transitionFrames, float leftWeight, float rightWeight) {
    FacialModelResource *const face = FindFacialModel(character);
    if (!face)
        return;
    const float duration = transitionFrames < 1.0f ? 0.0f : transitionFrames;
    face->data.eyelidDuration = 0.0f;

    d3t::SetObjectEyeLidExpressionWeights(GetMT7ReaderData(static_cast<const MT7Model *>(character->model)), rightWeight, leftWeight, duration);
    SetOuterEyelidWeights(*face, duration, leftWeight * 1.8f, rightWeight * 1.8f);
    SetInnerEyelidWeights(*face, duration, leftWeight * 0.2f, rightWeight * 0.2f);
    face->data.eyelidDuration = duration;
    face->data.eyelidFrame = 0.0f;
}

} // namespace shenmue::s2
