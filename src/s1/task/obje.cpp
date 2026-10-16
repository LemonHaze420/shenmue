#include "s1/task/obje.hpp"

#include "s1/boot.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/lighting.hpp"
#include "s1/mt5_render.hpp"
#include "s1/task/effe.hpp"
#include "s1/mt5_types.hpp"
#include "s1/scene.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/came.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/pack.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace shenmue::s1 {

std::int32_t g_ActiveGridTier = 0;
std::int32_t g_occlusionLUTUpdateCount = 0;
std::int32_t g_shouldUseLODVisibilityThisFrame = 0;
std::uint32_t g_objeOcclusionLookupMask = 0;
std::int16_t g_objeCameraGridX = 0;
std::int16_t g_objeCameraGridZ = 0;
std::int32_t g_dbg_draw_enabled = 0;
std::int32_t g_is_busy_transform = 0;
std::uint8_t g_useCachedMapMatrices = 1;

namespace Weather {
Vector4 g_tempOverrideLighting{};
} // namespace Weather

Matrix4x4 g_objectDebugMatrix{{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f},
}};

void *g_objectDebugAllocation = nullptr;

namespace {
ObjeFrameDiagnostics g_objeFrameDiagnostics{};
struct OcclusionDepthEntry {
    float value;
    std::int32_t source;
};
static_assert(sizeof(OcclusionDepthEntry) == 8);

constexpr std::size_t kOcclusionDepthBinCount = 193;
constexpr std::int32_t kOcclusionDepthBinHalfRange = 96;
std::array<OcclusionDepthEntry, kOcclusionDepthBinCount> g_occlusionDepthTable{};

constexpr std::int16_t kObjeQueue = 12;
constexpr std::uint16_t kObjeOrder = 5;
constexpr std::int32_t kObjeToken = static_cast<std::int32_t>(SHEN_NAME(OBJE));
constexpr std::int32_t kTaskDataToken = SHEN_NAME(TASK);
constexpr std::size_t kMaximumNodeDepth = 64;
constexpr std::int32_t kMaximumSceneRecords = 65536;
static_assert(kObjeToken == SHEN_NAME(OBJE));





constexpr std::array<std::size_t, 4> kNativeGridProfileStarts{0, 162, 321, 480};
constexpr std::array<std::array<std::uint16_t, 11>, 4> kNativeGridProfileCounts{{
    {9, 17, 25, 36, 48, 64, 79, 98, 116, 137, 162},
    {9, 15, 23, 34, 45, 60, 76, 94, 114, 136, 159},
    {9, 15, 23, 34, 45, 60, 76, 94, 114, 134, 159},
    {9, 17, 25, 36, 48, 64, 79, 98, 116, 137, 160},
}};

struct GridCoordinate {
    std::int32_t x;
    std::int32_t z;
};
bool RunIsVisible(const DynamicMapClipCellRun &run, const GridCoordinate &cameraGrid, std::int32_t ownerYaw);

HLib::Task *g_objeTask = nullptr;

std::int32_t NativeGridCoordinate(float position, float gridFactor) {
    float coordinate = position * gridFactor;
    if (!std::isfinite(coordinate))
        return 0;

    if (coordinate < 0.0f)
        coordinate -= 1.0f;

    if (coordinate >= static_cast<float>((std::numeric_limits<std::int32_t>::max)())) {
        return (std::numeric_limits<std::int32_t>::max)();
    }
    if (coordinate <= static_cast<float>((std::numeric_limits<std::int32_t>::min)())) {
        return (std::numeric_limits<std::int32_t>::min)();
    }
    return static_cast<std::int32_t>(coordinate);
}

std::int32_t NativeGridTier(float gridFactor, float lodDistance) {
    const float scaledDistance = gridFactor * lodDistance;
    if (!std::isfinite(scaledDistance) || scaledDistance < 0.0f)
        return 0;
    if (scaledDistance >= 11.0f)
        return 10;
    return static_cast<std::int32_t>(scaledDistance);
}

bool HasStaticSceneRecords(const DynamicMapEntry &map) {
    return map.mapAsset && map.mapAsset->model && map.clipCellPointers && map.sceneObjectData && map.sceneObjectCount > 0 && map.clipMetrics[0] > 0;
}

void PushNativeTransform() {
    *g_matrixStack.top.stack = workTransformMatrix;
    ++g_matrixStack.top.info.index;
    if (g_matrixStack.top.info.index < g_matrixStack.top.info.capacity)
        ++g_matrixStack.top.stack;
}

void PopNativeTransform() {
    std::int16_t count = 1;
    if (g_matrixStack.top.info.index < 1)
        count = g_matrixStack.top.info.index;
    std::int16_t pointerCount = count;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
        pointerCount = static_cast<std::int16_t>(count + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index);
    }
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - count);
    g_matrixStack.top.stack -= pointerCount;
    workTransformMatrix = *g_matrixStack.top.stack;
}

void ApplyMapTransform(const DynamicMapEntry &map) {
    if (map.position.x != 0.0f || map.position.y != 0.0f || map.position.z != 0.0f || map.field_44 != 0) {
        Matrix4x4::preTranslate(map.position.x, map.position.y, map.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(map.field_44));
    }
}

Matrix4x4 MapTransform(const Matrix4x4 &cameraTransform, const DynamicMapEntry &map) {
    workTransformMatrix = cameraTransform;
    ApplyMapTransform(map);
    return workTransformMatrix;
}

void ApplyStaticTransform(const DynamicMapStaticTransform &transform) {
    if (transform.position.x != 0.0f || transform.position.y != 0.0f || transform.position.z != 0.0f || transform.yaw != 0) {
        Matrix4x4::preTranslate(transform.position.x, transform.position.y, transform.position.z);
        const std::uint16_t yaw = static_cast<std::uint16_t>(transform.yaw);
        Matrix4x4::RotateAroundY(
            ::shenmue::sin_fixed16(static_cast<std::int16_t>(yaw)), ::shenmue::sin_fixed16(static_cast<std::int16_t>(::shenmue::fixed16_cosine_angle(yaw))));
    }
}

Vector3 TransformPointByWorkMatrix(const Vector3 &point);
Vector3 TransformVectorByWorkMatrix(const Vector3 &vector);

void ScaleEffectTransformsExact() {
    if (g_effectsNeedInitialization) {
        for (EffectLightRenderSlot &effect : g_effectLightRenderSlots) {
            if (!effect.enabled)
                continue;

            const float red = effect.intensity * effect.colourR;
            const float green = effect.intensity * effect.colourG;
            const float blue = effect.intensity * effect.colourB;
            effect.shaderConstants04[5] = red;
            effect.shaderConstants04[6] = green;
            effect.shaderConstants04[7] = blue;
            effect.shaderConstants04[4] = red * 0.3f + green * 0.59f + blue * 0.11f;
        }
    } else {
        EffectLightRenderSlot &effect = g_effectLightRenderSlots[0];
        const float red = effect.colourR * effect.intensity;
        const float green = effect.colourG * effect.intensity;
        const float blue = effect.colourB * effect.intensity;
        effect.shaderConstants04[5] = red;
        effect.shaderConstants04[6] = green;
        effect.shaderConstants04[7] = blue;
        effect.shaderConstants04[4] = red * 0.3f + green * 0.59f + blue * 0.11f;
    }
}

void InitEffectByIndexExact() {
    ScaleEffectTransformsExact();
    if (g_effectsNeedInitialization) {
        for (EffectLightRenderSlot &effect : g_effectLightRenderSlots) {
            if (!effect.enabled)
                continue;

            effect.transformedDirection = TransformVectorByWorkMatrix(effect.negativeNormalizedDirection);
            effect.transformedPosition = TransformPointByWorkMatrix(effect.position);
        }
    } else {
        g_effectLightRenderSlots[0].transformedDirection = TransformVectorByWorkMatrix(g_effectLightRenderSlots[0].negativeNormalizedDirection);
    }
}

void SetEffectOverrideIntensity(float intensity) {
    SetLightIntensity(0, intensity);
}





enum class MapRecordVisibility : std::uint8_t {
    Screen,
    Category,
    CategoryOccluded,
    AnyTest,
};

bool shenIsLocalSphereInsideScreenCullVolume(const Vector3 &localCenter, float radius);
bool CullSphereByNativeMode(const Vector3 &localCenter, float radius, std::int32_t mode);

void SubmitRecord(const DynamicMapClipRecord &record, float ownerAlpha, std::byte *modelBase, MapRecordVisibility visibility) {
    if (!record.node || !modelBase)
        return;

    auto *const node = static_cast<Mt5RuntimeNode *>(record.node);
    if (!node->sceneBindingOffset)
        return;
    void *const binding = modelBase + node->sceneBindingOffset;
    const Matrix4x4 ownerTransform = workTransformMatrix;
    if (!g_useCachedMapMatrices && CachedAttachmentOwnsSubmission(node, ownerAlpha, ownerTransform))
        return;

    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));

    if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);

    bool visible = true;
    if (visibility == MapRecordVisibility::Screen) {
        visible = shenIsLocalSphereInsideScreenCullVolume(record.position, record.radius);
    } else {
        const bool inCategory = CullSphereByNativeMode(record.position, record.radius, record.category / 4);
        if (visibility == MapRecordVisibility::Category)
            visible = inCategory;
        else if (visibility == MapRecordVisibility::CategoryOccluded)
            visible = inCategory && (record.category == 4 || shenIsLocalSphereVisibleInScreenDepthTable(record.position, record.radius));
        else
            visible = inCategory || record.category == 4 || shenIsLocalSphereVisibleInScreenDepthTable(record.position, record.radius);
    }
    if (visible) {
        g_sceneObjectMaxScale = record.maxScale;
        if (!g_useCachedMapMatrices || !CachedAttachmentOwnsSubmission(node, ownerAlpha, workTransformMatrix)) {
            if (ownerAlpha == 1.0f) {
                // With cached matrices the opaque submitters hand a prepared binding's attachment buffer handle (g_tmp_buf_handle) to the
                // draw; the alpha path does not.
                if (g_useCachedMapMatrices && node->attachmentOffset && (*static_cast<const std::uint32_t *>(binding) & 0x10U) != 0U)
                    g_effectBufferHandleOverride = reinterpret_cast<const std::int32_t *>(g_pCurrentTaskData + node->attachmentOffset);
                SubmitSceneObjectForDrawing(binding, modelBase);
            } else
                SubmitToEffectBuffer(binding, ownerAlpha, 0, modelBase);
        }
    }
    workTransformMatrix = ownerTransform;
}

GridCoordinate GridForTransform(const Vector3 &position, std::int32_t yaw) {
    const float deltaX = g_CameraEyePos.x - position.x;
    const float deltaZ = g_CameraEyePos.z - position.z;
    const std::uint16_t wrappedYaw = static_cast<std::uint16_t>(yaw);
    const float sine = ::shenmue::sin_fixed16(static_cast<std::int16_t>(wrappedYaw));
    const float cosine = ::shenmue::sin_fixed16(static_cast<std::int16_t>(::shenmue::fixed16_cosine_angle(wrappedYaw)));
    return {
        NativeGridCoordinate(cosine * deltaX + sine * deltaZ, g_mapBlockScale),
        NativeGridCoordinate(cosine * deltaZ - sine * deltaX, g_mapBlockScale),
    };
}

Vector3 TransformPointByWorkMatrix(const Vector3 &point) {
    return {
        point.x * workTransformMatrix.rows[0].x + point.y * workTransformMatrix.rows[1].x + point.z * workTransformMatrix.rows[2].x +
            workTransformMatrix.rows[3].x,
        point.x * workTransformMatrix.rows[0].y + point.y * workTransformMatrix.rows[1].y + point.z * workTransformMatrix.rows[2].y +
            workTransformMatrix.rows[3].y,
        point.x * workTransformMatrix.rows[0].z + point.y * workTransformMatrix.rows[1].z + point.z * workTransformMatrix.rows[2].z +
            workTransformMatrix.rows[3].z,
    };
}
Vector3 TransformVectorByWorkMatrix(const Vector3 &vector) {
    return {
        vector.x * workTransformMatrix.rows[0].x + vector.y * workTransformMatrix.rows[1].x + vector.z * workTransformMatrix.rows[2].x,
        vector.x * workTransformMatrix.rows[0].y + vector.y * workTransformMatrix.rows[1].y + vector.z * workTransformMatrix.rows[2].y,
        vector.x * workTransformMatrix.rows[0].z + vector.y * workTransformMatrix.rows[1].z + vector.z * workTransformMatrix.rows[2].z,
    };
}

bool IsLocalSphereInsideViewCullVolume(const Vector3 &localCenter, float radius) noexcept {
    const Vector3 view = TransformPointByWorkMatrix(localCenter);
    const float depthFarEdge = -view.z + radius;
    if (depthFarEdge < 0.0f || -view.z - radius > g_CurrLODDistance_0) {
        return false;
    }

    const Vector2 scale = prevScreenSpaceToWorldScale;
    const float halfWidth = depthFarEdge * scale.x + radius;
    const float halfHeight = depthFarEdge * scale.y + radius;
    return view.x <= halfWidth && -halfWidth < view.x && view.y <= halfHeight && -halfHeight < view.y;
}

bool shenIsLocalSphereInsideInnerViewCullVolume(const Vector3 &localCenter, float radius) {

    const Vector3 view = TransformPointByWorkMatrix(localCenter);
    const float depthFarEdge = -view.z + radius;
    if (depthFarEdge < 0.0f || -view.z - radius > g_CurrLODDistance_1) {
        return false;
    }

    const Vector2 scale = prevScreenSpaceToWorldScale;
    const float halfWidth = depthFarEdge * scale.x + radius;
    const float halfHeight = depthFarEdge * scale.y + radius;
    return view.x <= halfWidth && -halfWidth < view.x && view.y <= halfHeight && -halfHeight < view.y;
}

bool shenIsLocalSphereInsideScreenCullVolume(const Vector3 &localCenter, float radius) {
    const Vector3 view = TransformPointByWorkMatrix(localCenter);
    const float depthFarEdge = -view.z + radius;
    if (depthFarEdge < 0.0f)
        return false;

    const Vector2 scale = prevScreenSpaceToWorldScale;
    const float halfWidth = depthFarEdge * scale.x + radius;
    const float halfHeight = depthFarEdge * scale.y + radius;
    return view.x <= halfWidth && -halfWidth < view.x && view.y <= halfHeight && -halfHeight < view.y;
}

bool shenIsLocalSphereInScreenLODBounds(const Vector3 &localCenter, float radius) {
    const Vector3 view = TransformPointByWorkMatrix(localCenter);
    if (-view.z - radius > g_CurrLODDistance_0)
        return false;
    const float depthFarEdge = -view.z + radius;
    if (depthFarEdge < g_CurrLODDistance_1)
        return false;
    const Vector2 scale = prevScreenSpaceToWorldScale;
    const float halfWidth = depthFarEdge * scale.x + radius;
    const float halfHeight = depthFarEdge * scale.y + radius;
    return view.x <= halfWidth && -halfWidth < view.x && view.y <= halfHeight && -halfHeight < view.y;
}

void RasterizeOcclusionProxyDepthSpan(float screenX, float depth, float radius) noexcept {
    const std::int32_t source = g_occlusionLUTUpdateCount++;

    const float reciprocal = static_cast<float>(1.0 / static_cast<double>(depth));
    const auto binFor = [reciprocal](float edge) {
        const float projected = static_cast<float>(static_cast<double>(edge * reciprocal) * kOcclusionDepthBinHalfRange);
        return std::clamp(static_cast<std::int32_t>(projected), -kOcclusionDepthBinHalfRange, kOcclusionDepthBinHalfRange);
    };
    const std::int32_t first = binFor(screenX - radius) + kOcclusionDepthBinHalfRange;
    const std::int32_t last = binFor(screenX + radius) + kOcclusionDepthBinHalfRange;
    OcclusionDepthEntry &lastEntry = g_occlusionDepthTable[static_cast<std::size_t>(last)];
    if (depth < lastEntry.value) {
        lastEntry.value = depth;
        lastEntry.source = source;
    } else if (g_occlusionDepthTable[static_cast<std::size_t>(first)].source == lastEntry.source)
        return;

    for (std::int32_t bin = last - 1; bin >= first; --bin) {
        OcclusionDepthEntry &entry = g_occlusionDepthTable[static_cast<std::size_t>(bin)];
        if (entry.value > depth) {
            entry.value = depth;
            entry.source = source;
        }
    }
}

void ProjectOcclusionProxyToScreenBins(const Vector4 &proxy, float cameraY) noexcept {
    const Vector3 reference = TransformPointByWorkMatrix(Vector3{proxy.x, cameraY, proxy.z});
    if (reference.z <= 0.0f)
        return;

    const Vector3 view = TransformPointByWorkMatrix(Vector3{proxy.x, proxy.y, proxy.z});
    const float depth = -view.z;
    const float radius = proxy.w;
    const float farEdge = radius + depth;
    if (farEdge <= 0.0f || g_CurrLODDistance_0 <= depth - radius) {
        return;
    }

    const Vector2 scale = screenSpaceToWorldScale;
    const float horizontalLimit = scale.y * farEdge + radius;
    const float verticalLimit = scale.x * farEdge + radius;
    if (view.x <= -horizontalLimit || horizontalLimit <= view.x || view.y <= -verticalLimit || verticalLimit <= view.y) {
        return;
    }
    RasterizeOcclusionProxyDepthSpan(view.x, depth, radius);
}




void BuildFrustumOcclusionTableImpl() {
    for (OcclusionDepthEntry &entry : g_occlusionDepthTable) {
        entry.source = 65536;
        entry.value = 20000.0f;
    }
    g_occlusionLUTUpdateCount = 0;
    const Vector3 target = g_CameraTargetPos;
    const std::int32_t cameraYaw = g_fCamAccumYaw;
    const GridCoordinate targetGrid{NativeGridCoordinate(target.x, g_mapBlockScale), NativeGridCoordinate(target.z, g_mapBlockScale)};
    const std::int32_t tier = NativeGridTier(g_mapBlockScale, g_CurrLODDistance_0);
    const auto *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const bool useRuntimeGrid = control && (control->mapRuntimeFlags & 1U) != 0;
    for (std::size_t slot = 0; slot < g_dynamicMapSlots.size(); ++slot) {
        DynamicMapAuxiliary &auxiliary = g_dynamicMapAuxiliary[slot];
        if (!auxiliary.clipCells) {
            auxiliary.clipCellsReady = 0;
            continue;
        }
        if (!auxiliary.clipCellsReady)
            continue;

        const DynamicMapEntry &map = g_dynamicMapSlots[slot];
        const Vector3 &position = map.position;
        const std::int32_t yaw = map.field_44;
        const auto yawSine = [yaw] { return ::shenmue::sin_fixed16(static_cast<std::int16_t>(yaw)); };
        const auto yawCosine = [yaw] {
            const std::uint16_t angle = static_cast<std::uint16_t>(yaw);
            const std::uint16_t folded = angle <= 0x8000U ? angle : static_cast<std::uint16_t>(-angle);
            return ::shenmue::sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        };
        const auto *const proxies = static_cast<const Vector4 *>(auxiliary.clipCells);
        Matrix4x4_Push();
        if (useRuntimeGrid) {
            GridCoordinate grid = targetGrid;
            if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw == 0) {
                const float sine = yawSine();
                const float cosine = yawCosine();
                const float deltaX = target.x - position.x;
                const float deltaZ = target.z - position.z;
                grid = {NativeGridCoordinate(sine * deltaZ + cosine * deltaX, g_mapBlockScale),
                    NativeGridCoordinate(cosine * deltaZ - sine * deltaX, g_mapBlockScale)};
                Matrix4x4::preTranslate(position.x, position.y, position.z);
                Matrix4x4::RotateAroundY(sine, cosine);
            }

            const std::uint32_t sector = (static_cast<std::uint32_t>(cameraYaw - yaw) >> 12U) & 0x0fU;
            const std::size_t start = kNativeGridProfileStarts[sector & 3U];
            const std::size_t count = kNativeGridProfileCounts[sector & 3U][static_cast<std::size_t>(tier)];
            const auto *const runs = static_cast<const DynamicMapClipCellRun *>(auxiliary.pathData);
            for (std::int32_t runIndex = 0; runs && runIndex < auxiliary.pathCellCount; ++runIndex) {
                const DynamicMapClipCellRun &run = runs[runIndex];
                const std::int16_t relativeX = static_cast<std::int16_t>(run.cellX - grid.x);
                const std::int16_t relativeZ = static_cast<std::int16_t>(run.cellZ - grid.z);
                bool listed = false;
                for (std::size_t index = 0; index < count && !listed; ++index) {
                    const std::uint32_t packed = kNativeGridProfiles[start + index];
                    std::int32_t x = static_cast<std::int16_t>(packed & 0xffffU);
                    std::int32_t z = static_cast<std::int16_t>(packed >> 16U);
                    for (std::uint32_t turn = 0; turn < (sector >> 2U); ++turn) {
                        const std::int32_t previousX = x;
                        x = -z;
                        z = previousX;
                    }
                    listed = relativeX == static_cast<std::int16_t>(x) && relativeZ == static_cast<std::int16_t>(z);
                }
                if (!listed)
                    continue;
                for (std::int32_t cell = 0; cell < run.count; ++cell)
                    ProjectOcclusionProxyToScreenBins(proxies[run.firstIndex + cell], target.y);
            }
        } else {
            if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw != 0) {
                Matrix4x4::preTranslate(position.x, position.y, position.z);
                Matrix4x4::RotateAroundY(yawSine(), yawCosine());
            }
            for (std::int32_t index = 0; index < auxiliary.clipCellCount; ++index)
                ProjectOcclusionProxyToScreenBins(proxies[index], target.y);
        }
        Matrix4x4::Pop(1);
    }
}

bool IsViewSphereVisibleInScreenDepthTable(const Vector3 &view, float radius) noexcept {
    const float depth = view.z;
    if (depth <= 0.0f)
        return true;

    const float nearDepth = depth - radius;
    const auto binFor = [depth](float edge) {
        const float projected = edge / depth * static_cast<float>(kOcclusionDepthBinHalfRange);
        return std::clamp(static_cast<std::int32_t>(projected), -kOcclusionDepthBinHalfRange, kOcclusionDepthBinHalfRange);
    };

    const std::int32_t first = binFor(view.x - radius) + kOcclusionDepthBinHalfRange;
    const std::int32_t last = binFor(view.x + radius) + kOcclusionDepthBinHalfRange;
    for (std::int32_t bin = first; bin <= last; ++bin) {
        if (g_occlusionDepthTable[static_cast<std::size_t>(bin)].value >= nearDepth) {
            return true;
        }
    }
    return false;
}

bool IsLocalSphereVisibleInScreenDepthTable(const Vector3 &localCenter, float radius) noexcept {
    Vector3 view = TransformPointByWorkMatrix(localCenter);
    view.z = -view.z;
    return IsViewSphereVisibleInScreenDepthTable(view, radius);
}

bool CullSphereByNativeMode(const Vector3 &localCenter, float radius, std::int32_t mode) {
    using CullFunction = bool (*)(const Vector3 &, float);
    static constexpr std::array<CullFunction, 5> functions{
        IsLocalSphereInsideViewCullVolume,
        IsLocalSphereInsideViewCullVolume,
        shenIsLocalSphereInsideInnerViewCullVolume,
        shenIsLocalSphereInScreenLODBounds,
        shenIsLocalSphereInsideScreenCullVolume,
    };
    return functions[static_cast<std::size_t>(std::clamp(mode, 0, 4))](localCenter, radius);
}

bool RunIsVisible(const DynamicMapClipCellRun &run, const GridCoordinate &cameraGrid, std::int32_t ownerYaw) {
    if (run.cellX == 0x4fff)
        return true;

    const std::uint32_t direction = (static_cast<std::uint32_t>(g_prev_temp_dir - ownerYaw) >> 12U) & 0x0fU;
    const std::size_t baseProfile = direction & 3U;
    const std::uint32_t quarterTurns = direction >> 2U;
    const std::size_t start = kNativeGridProfileStarts[baseProfile];
    const std::size_t count = kNativeGridProfileCounts[baseProfile][static_cast<std::size_t>(std::clamp(g_ActiveGridTier, 0, 10))];
    const std::int32_t relativeX = run.cellX - cameraGrid.x;
    const std::int32_t relativeZ = run.cellZ - cameraGrid.z;

    for (std::size_t index = 0; index < count; ++index) {
        const std::uint32_t packed = kNativeGridProfiles[start + index];
        std::int32_t x = static_cast<std::int16_t>(packed & 0xffffU);
        std::int32_t z = static_cast<std::int16_t>(packed >> 16U);
        for (std::uint32_t turn = 0; turn < quarterTurns; ++turn) {
            const std::int32_t previousX = x;
            x = -z;
            z = previousX;
        }

        if (relativeX == x && relativeZ == z)
            return true;
    }
    return false;
}

void SubmitMapRecords(const DynamicMapEntry &map, const GridCoordinate &cameraGrid, std::int32_t ownerYaw, float ownerAlpha, bool ownerOcclusionEnabled) {
    std::byte *const modelBase = map.mapAsset && map.mapAsset->model ? MT5_ModelBaseForNode(map.mapAsset->model) : nullptr;
    if (!modelBase)
        return;

    if (!map.clipCellPointers || !map.sceneObjectData)
        return;

    auto *const runs = static_cast<DynamicMapClipCellRun *>(map.clipCellPointers);
    auto **const records = static_cast<DynamicMapClipRecord **>(map.sceneObjectData);
    const std::int32_t totalRecords = std::clamp(map.clipMetrics[0], 0, kMaximumSceneRecords);
    const std::int32_t runCount = std::max(map.sceneObjectCount, 0);



    const bool lodVisibility = g_shouldUseLODVisibilityThisFrame != 0 && ownerOcclusionEnabled;
    for (std::int32_t runIndex = 0; runIndex < runCount; ++runIndex) {
        const DynamicMapClipCellRun &run = runs[runIndex];
        if (!RunIsVisible(run, cameraGrid, ownerYaw))
            continue;
        const MapRecordVisibility visibility = run.cellX == 0x4fff ? MapRecordVisibility::Screen :
            ownerAlpha != 1.0f                                     ? MapRecordVisibility::AnyTest :
            lodVisibility                                          ? MapRecordVisibility::CategoryOccluded :
                                                                     MapRecordVisibility::Category;

        ++g_objeFrameDiagnostics.visibleRuns;
        const std::int32_t first = std::clamp(run.firstIndex, 0, totalRecords);
        const std::int32_t end = std::clamp(first + std::max(run.count, 0), first, totalRecords);
        g_objeFrameDiagnostics.candidateRecords += static_cast<std::uint32_t>(end - first);
        for (std::int32_t recordIndex = first; recordIndex < end; ++recordIndex) {
            if (records[recordIndex]) {
                SubmitRecord(*records[recordIndex], ownerAlpha, modelBase, visibility);
                ++g_objeFrameDiagnostics.submittedRecords;
            }
        }
    }
}




void SubmitVisibleSceneNodeRecursive(const Mt5RuntimeNode *node, bool occlusion, std::byte *modelBase) {
    for (;;) {
        if (node->siblingOffset)
            PushNativeTransform();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        if (node->sceneBindingOffset) {
            const DynamicMapClipRecord &record = g_clipRecords[g_clipRecordCount];
            if (record.category != 255 && CullSphereByNativeMode(record.position, record.radius, record.category / 4) &&
                (!occlusion || shenIsLocalSphereVisibleInScreenDepthTable(record.position, record.radius))) {
                g_sceneObjectMaxScale = record.maxScale;
                SubmitSceneObjectForDrawing(modelBase + node->sceneBindingOffset, modelBase);
            }
            ++g_clipRecordCount;
        }
        if (node->childOffset)
            SubmitVisibleSceneNodeRecursive(reinterpret_cast<const Mt5RuntimeNode *>(modelBase + node->childOffset), occlusion, modelBase);
        if (!node->siblingOffset)
            return;
        PopNativeTransform();
        node = reinterpret_cast<const Mt5RuntimeNode *>(modelBase + node->siblingOffset);
    }
}




void SubmitMapModel(const DynamicMapEntry &map) {
    auto *const model = map.mapAsset ? static_cast<Mt5RuntimeNode *>(map.mapAsset->model) : nullptr;
    if (!model)
        return;
    g_clipRecordCount = 0;
    g_clipRecords = static_cast<DynamicMapClipRecord *>(map.clipRuntime);
    PushNativeTransform();
    ApplyMapTransform(map);
    const bool occlusion = g_occlusionLUTUpdateCount != 0 && map.occlusionEnabled != 0;
    if (!CachedAttachmentOwnsSubmission(model, 1.0f, workTransformMatrix))
        SubmitVisibleSceneNodeRecursive(model, occlusion, MT5_ModelBaseForNode(model));
    PopNativeTransform();
}

void ConstructStaticSceneObjects() {
    if (!g_updateRenderFrameMode)
        return;
    g_objeCameraGridX = static_cast<std::int16_t>(NativeGridCoordinate(g_CameraEyePos.x, g_mapBlockScale));
    g_objeCameraGridZ = static_cast<std::int16_t>(NativeGridCoordinate(g_CameraEyePos.z, g_mapBlockScale));

    const Matrix4x4 savedTransform = workTransformMatrix;
    for (std::size_t slot = 0; slot < g_dynamicMapSlots.size(); ++slot) {
        DynamicMapEntry &map = g_dynamicMapSlots[slot];
        if (!map.renderEnabled || !map.mapAsset || !map.mapAsset->model) {
            continue;
        }

        ++g_objeFrameDiagnostics.mapOwners;
        PushNativeTransform();

        const bool temporaryLighting = map.lightingOverride01 != 0.0f;
        Vector4 savedLighting{};
        float savedIntensity = 0.0f;
        std::int32_t savedLightingMode = 0;
        if (temporaryLighting) {
            savedLighting = g_sceneLightingBaseColour;
            if (const ShenmueLightSlot *const light = LGHT_GetDataSlot(0))
                savedIntensity = light->intensity;
            savedLightingMode = g_cachedGlobalLightingAdjustFlag;
            const float x = Weather::g_tempOverrideLighting.x;
            g_sceneLightingBaseColour = {x, x, x, x};
            g_sceneLightingActiveColour = {x, x, x, x};
            SetEffectOverrideIntensity(Weather::g_tempOverrideLighting.y);
            g_cachedGlobalLightingAdjustFlag = 1;
            g_effectsNeedInitialization = 0;
            InitEffectByIndexExact();
        }

        const bool debugDraw = g_dbg_draw_enabled != 0 && map.debugDrawFlag != 0;
        if (!map.staticSceneMode || debugDraw) {
            SubmitMapModel(map);
            if (debugDraw) {
                PopNativeTransform();
                PushNativeTransform();
                g_is_busy_transform = 1;
                Matrix4x4::Multiply(&g_objectDebugMatrix);
                SubmitMapModel(map);
                g_is_busy_transform = 0;
            }
        } else if (std::fabs(map.scale) >= (std::numeric_limits<float>::epsilon)()) {
            ApplyMapTransform(map);
            SubmitMapRecords(map, GridForTransform(map.position, map.field_44), map.field_44, map.scale, map.occlusionEnabled != 0);
        }

        if (temporaryLighting) {
            g_sceneLightingBaseColour = savedLighting;
            g_sceneLightingActiveColour = savedLighting;
            SetEffectOverrideIntensity(savedIntensity);
            g_cachedGlobalLightingAdjustFlag = savedLightingMode;
            g_effectsNeedInitialization = savedLightingMode != 1;
            InitEffectByIndexExact();
        }
        PopNativeTransform();
    }
    workTransformMatrix = savedTransform;
}

void UpdateTransformMatrices() {
    if (!g_updateRenderFrameMode)
        return;

    const Matrix4x4 savedTransform = workTransformMatrix;
    for (const DynamicMapStaticTransform &transform : g_dynamicMapStaticTransforms) {
        if (!transform.enabled || transform.sourceMapSlot == 0x7fff || transform.sourceMapSlot < 0 ||
            static_cast<std::size_t>(transform.sourceMapSlot) >= g_dynamicMapSlots.size())
            continue;

        const DynamicMapEntry &map = g_dynamicMapSlots[static_cast<std::size_t>(transform.sourceMapSlot)];
        if (!map.mapAsset || !map.mapAsset->model)
            continue;

        ++g_objeFrameDiagnostics.transformedOwners;
        PushNativeTransform();
        ApplyStaticTransform(transform);
        SubmitMapRecords(map, GridForTransform(transform.position, transform.yaw), transform.yaw, 1.0f, transform.field_18 != 0);
        PopNativeTransform();
    }
    workTransformMatrix = savedTransform;
}

void ConfigureTaskForDestruction(HLib::Task *task) {
    if (!task)
        return;
    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}

} // namespace
void OBJE_ApplyPivotedReflection(Matrix4x4 *plane, const Vector3i *rotation) {
    Matrix4x4::SetActiveTransformToPrePass();
    if (!plane) {
        if (g_dbg_draw_enabled && g_objectDebugAllocation) {
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_objectDebugAllocation), 0);
            g_objectDebugAllocation = nullptr;
        }
        g_dbg_draw_enabled = 0;
        g_objectDebugMatrix = workTransformMatrix;
        Matrix4x4::Pop();
        return;
    }

    if (!g_dbg_draw_enabled) {
        constexpr std::int32_t kMirrorToken = static_cast<std::int32_t>(SHEN_NAME(MIRM));
        g_objectDebugAllocation = HLib_AllocHeapMemBlock(0x700, kMirrorToken);
    }
    g_dbg_draw_enabled = 1;

    if (rotation) {
        const float x = plane->rows[0].x;
        const float y = plane->rows[0].y;
        const float z = plane->rows[0].z;
        Matrix4x4::preTranslate(x, y, z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation->z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation->y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation->x));
        Matrix4x4::preTranslate(-x, -y, -z);
    }

    const float normalX = plane->rows[0].w;
    const float normalY = plane->rows[1].x;
    const float normalZ = plane->rows[1].y;
    const float pointX = plane->rows[0].x;
    const float pointY = plane->rows[0].y;
    const float pointZ = plane->rows[0].z;
    const float dot = ((normalY * pointY) + (normalX * pointX)) + (normalZ * pointZ);
    const float twiceDot = dot + dot;
    const Matrix4x4 reflection{{
        {1.0f - ((normalX + normalX) * normalX), (normalX * -2.0f) * normalY, (normalX * -2.0f) * normalZ, 0.0f},
        {(normalX * -2.0f) * normalY, 1.0f - ((normalY + normalY) * normalY), (normalY * -2.0f) * normalZ, 0.0f},
        {(normalX * -2.0f) * normalZ, (normalY * -2.0f) * normalZ, 1.0f - ((normalZ + normalZ) * normalZ), 0.0f},
        {twiceDot * normalX, twiceDot * normalY, twiceDot * normalZ, 1.0f},
    }};
    Matrix4x4::Multiply(&reflection);
    g_objectDebugMatrix = workTransformMatrix;
    Matrix4x4::Pop();
}

void Shenmue_InitEffectByIndexExact() {
    InitEffectByIndexExact();
}

void BuildFrustumOcclusionTable() {
    BuildFrustumOcclusionTableImpl();
}

bool shenIsLocalSphereInsideViewCullVolume(const Vector3 &localCenter, float radius) noexcept {
    return IsLocalSphereInsideViewCullVolume(localCenter, radius);
}

bool shenIsLocalSphereVisibleInScreenDepthTable(const Vector3 &localCenter, float radius) noexcept {
    return IsLocalSphereVisibleInScreenDepthTable(localCenter, radius);
}

bool shenIsViewSphereVisibleInScreenDepthTable(const Vector3 &viewCenter, float radius) noexcept {
    return IsViewSphereVisibleInScreenDepthTable(viewCenter, radius);
}

HLib::Task *OBJE_GetTask() {
    return g_objeTask;
}

ObjeTaskData *OBJE_GetWork() {
    return g_objeTask && g_objeTask->task_data_ptr ? reinterpret_cast<ObjeTaskData *>(g_objeTask->task_data_ptr) : nullptr;
}

const ObjeFrameDiagnostics &OBJE_GetFrameDiagnostics() {
    return g_objeFrameDiagnostics;
}

HLib::Task *OBJE_Init() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(OBJE_Callback, kObjeQueue, kObjeOrder, kObjeToken);
    if (!task || task->task_data_ptr) {
        ConfigureTaskForDestruction(task);
        g_objeTask = nullptr;
        return nullptr;
    }

    auto *const data = static_cast<ObjeTaskData *>(HLib_AllocHeapMemBlock(sizeof(ObjeTaskData), kTaskDataToken));
    if (!data) {
        ConfigureTaskForDestruction(task);
        g_objeTask = nullptr;
        return nullptr;
    }

    HLib_EnableTokenCheck(data);
    data->state08 = 0;
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    g_objeTask = task;
    return task;
}

void OBJE_Shutdown() {
    if (g_objeTask)
        HLib_DestroyTask(g_objeTask);
    g_objeTask = nullptr;
}

void OBJE_Callback(HLib::TaskData *) {
    g_objeFrameDiagnostics = {};
    if (!g_bShouldDraw)
        return;

    g_shouldUseLODVisibilityThisFrame = g_occlusionLUTUpdateCount;
    g_ActiveGridTier = NativeGridTier(g_fCurrWorldToGridFactor, g_CurrLODDistance);
    ConstructStaticSceneObjects();
    UpdateTransformMatrices();
}

} // namespace shenmue::s1
