#include "s2/lens_flare_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/file_loader_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/weather_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/weather_air_dc.hpp"
#include "platform/dc/sdk.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_pose_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/field_sphere_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/math.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/modifier_vertices_dc.hpp"
#endif
#include "shenmue.h"
#include "s2/weather.hpp"
#include "s2/weather_data_dc.hpp"
#include "s2/date_time.hpp"

namespace shenmue {
namespace s2 {

void LoadLensflare();
void DrawSunLensFlare(void *parameter);
void UpdateSunForDateTime(const DateTime &date, float latitude, Vector3 &direction, float scale);

MT7Model *g_lensFlareModels[14] = {};
LensFlareState g_lensFlareState = {};

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const Vector3 *g_sunOcclusionOrigin = 0, *g_sunOcclusionEnd = 0;
#endif

// @verified - d
void shenLoadSetLensflare() {
    LoadLensflare();
    g_lensFlareState.diskScale = 0.5f;
    g_lensFlareState.enabled = 1;
    g_lensFlareState.explicitDirection = 0;
}

// @verified - d
void LoadLensflare() {
    MapInfoChunk *chunk = static_cast<MapInfoChunk *>(shenLoadFile("misc", "LENSFLAR.BIN", LENS_FLARE_STORAGE));
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(g_lensFlareModels) / sizeof(g_lensFlareModels[0]));
        ++index, chunk = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(chunk) + chunk->nextByteOffset))
        g_lensFlareModels[index] = InitializeMT7Model(chunk + 1);
}

// @verified - d
void SetWeatherLightManager(std::int32_t enabled) {
    g_lensFlareState.enabled = enabled;
}

// @verified - d
void SetSunDirectionVector(const Vector3 *direction) {
    if (direction) {
        g_lensFlareState.direction = *direction;
        g_lensFlareState.explicitDirection = 1;
    } else
        g_lensFlareState.explicitDirection = 0;
}

// @verified - d
void UpdateSunLensFlare() {
    if (!g_lensFlareState.enabled)
        return;
    if (!g_lensFlareState.explicitDirection) {
        if (GetCurrentActiveLight() != 1)
            return;
        if (GetWeatherWindVector() > 0.1f)
            return;
        if (GetWeatherPressure() > 0.1f)
            return;
        DateTime date;
        shenGetCurrentDateTime(date);
        UpdateSunForDateTime(date, 35.0f, g_lensFlareState.direction, 3200.0f);
        if (g_lensFlareState.direction.y < 0.0f)
            return;
    }
    if (camCtrlCheckWorldDepthPosition(g_lensFlareState.direction, 1.0f)) {
        g_lensFlareState.jitterScale = dc::sdk::RandomFloat() * 0.1f + 0.1f;
        HLib::ExecuteCallbackNow(DrawSunLensFlare, &g_lensFlareState);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateSunFilmFade(float, float falloff) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const float value = (64.0f * falloff) * falloff;
    filmSetFadeColourTo(control->filmTask, static_cast<std::uint32_t>(static_cast<std::int32_t>(value)));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t TestSunOcclusionCharacter(Character *character, void *) {
    Vector3 position;
    Vector3 origin;
    Vector3 end;
    Vector3 bonePosition;
    Vector3 offset;
    Vector3 *const originPoint = &origin;
    *originPoint = *g_sunOcclusionOrigin;
    Vector3 *const endPoint = &end;
    *endPoint = *g_sunOcclusionEnd;
    if (!IsOfThisVisibilityType(character, CVM_All))
        return 1;
    GetCharacterLocalPosition(character, &position);
    origin.y = position.y;
    end.y = position.y;
    offset.x = position.x - originPoint->x;
    offset.y = 0.0f;
    offset.z = position.z - origin.z;
    if (dc::sdk::njInnerProduct(&offset, endPoint) < 1.0f)
        return 1;
    MotionWork *const motion = charFindMotionwork(character);
    if (!motion)
        return 1;
    if (!mtwkGetBonePosition(motion, static_cast<MotionBoneId>(6), &bonePosition))
        return 1;
    if (!TestFieldLineSphere(g_sunOcclusionOrigin, g_sunOcclusionEnd, &position, 1.69851339f))
        return 1;
    if (!TestFieldLineSphere(&origin, &end, &position, 0.5f))
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t IsSunOccluded(const Vector3 *origin, const Vector3 *direction) {
    Vector3 end;
    if (am2Vector3Length(direction) > 10.0f)
        dc::sdk::SetVectorLength(&end, direction, 10.0f);
    else
        end = *direction;
    am2Vector3AddInPlace(&end, origin);
    if (TestFieldWorldSegment(origin, &end))
        return 1;
    g_sunOcclusionOrigin = origin;
    g_sunOcclusionEnd = direction;
    return !ForEachLiveCharacterInMapOrder(0, TestSunOcclusionCharacter, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawLensFlareBillboard(std::int32_t index, float depth, float scale, float alpha) {
    Matrix4x4 current;
    Matrix4x4 billboard = {SHEN_FIELD(rows){{SHEN_FIELD(x) 1.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 1.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 1.0f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 1.0f}}};
    const Vector4 &source = current.rows[3];
    const float totalScale = scale * 0.15f;
    Vector4 &target = billboard.rows[3];
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(0.0f, 0.0f, depth);
    matrix_stack::GetMatrix(&current);
    target.x = source.x;
    target.y = source.y;
    target.z = source.z;
    matrix_stack::LoadMatrix(&billboard);
    Matrix4x4::Scale(totalScale, totalScale, totalScale);
    dc::sdk::DrawSingleLightMT7MeshAlpha(g_lensFlareModels[index]->root->mesh, alpha);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RotateLensFlareAlignment(const Vector3 *direction) {
    Vector3 axis;
    axis.x = 0.0f;
    axis.y = 0.0f;
    axis.z = 1.0f;
    Vector3 aligned;
    aligned.x = direction->x;
    aligned.y = 0.0f;
    aligned.z = direction->z;
    am2Vector3NormalizeInPlace(&aligned);
    Matrix4x4_BuildRotationMatrix(axis, aligned);
    aligned.x = direction->y;
    aligned.y = 0.0f;
    aligned.z = __builtin_sqrtf(((direction->x) * (direction->x) + (direction->z * direction->z)));
    Matrix4x4::RotateAroundZ_Wrapper(0x4000);
    am2Vector3NormalizeInPlace(&aligned);
    Matrix4x4_BuildRotationMatrix(axis, aligned);
    Matrix4x4::RotateAroundZ_Wrapper(0xc000);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawSunLensFlare(void *parameter) {
    Vector3 screenPosition, view, alignment, eye, delta, projection, base;
    Vector2 screen;
    const LensFlareState *const state = reinterpret_cast<const LensFlareState *>(parameter);
    const Vector3 *const position = &screenPosition;
    float diskScale = state->diskScale;
    float alpha = state->jitterScale, falloff, center, facing, length;
    Matrix4x4::TransformPoint(state->direction, screenPosition);
    if (!(0.0f > screenPosition.z))
        return;
    Vector2 *const point = &screen;
    reinterpret_cast<void (*)(float, float)>(matrix_stack::UpdateWorldToScreenMatrix)(640.0f, 480.0f);
    matrix_stack::ProjectWorldPointToScreen(&state->direction, point);
    if (!(screen.x > -300.0f && 940.0f > screen.x && point->y > -200.0f && 680.0f > point->y))
        return;
    Vector3 *const viewAxis = &view, *const toSun = &alignment, *const camera = &eye;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(camCtrlGetViewMatrix(0));
    viewAxis->x = 0.0f;
    viewAxis->y = 0.0f;
    viewAxis->z = -1.0f;
    Matrix4x4::Transform(*viewAxis, *viewAxis);
    PopCurrentMatrix(1);
    camCtrlGetNextCamPos(camera);
    toSun->x = state->direction.x - camera->x;
    toSun->y = state->direction.y - camera->y;
    toSun->z = state->direction.z - camera->z;
    if (IsSunOccluded(camera, toSun))
        return;
    do {
        base.x = 0.0f;
    } while (0);
    base.y = 0.0f;
    base.z = -0.4f;
    center = falloff = 0.99f;
    Vector3 *const offset = &delta, *const flare = &projection;
    if (-100.0f > screen.x)
        falloff = (screen.x + 300.0f) / 200.0f;
    else if (point->x > 740.0f)
        falloff = (940.0f - point->x) / 200.0f;
    if (-100.0f > point->y)
        falloff *= (200.0f + point->y) / 100.0f;
    else if (point->y > 580.0f)
        falloff *= (680.0f - point->y) / 100.0f;
    if (point->x > -20.0f && 660.0f > point->x && point->y > -20.0f && 500.0f > point->y) {
        if (20.0f > point->x)
            center = (point->x + 20.0f) / 40.0f;
        else if (point->x > 620.0f)
            center = (660.0f - point->x) / 40.0f;
        if (20.0f > point->y)
            center *= (point->y + 20.0f) / 40.0f;
        else if (point->y > 460.0f)
            center *= (500.0f - point->y) / 40.0f;
    } else
        center = 0.0f;
    alpha *= falloff;
    am2Vector3NormalizeInPlace(viewAxis);
    am2Vector3NormalizeInPlace(toSun);
    facing = am2Vector3Dot(viewAxis, toSun);
    UpdateSunFilmFade(facing, falloff);
    flare->x = (position->x * -1.0f) / position->z;
    flare->y = (position->y * -1.0f) / position->z;
    flare->z = -1.0f;
    offset->x = base.x - flare->x;
    offset->y = base.y - flare->y;
    offset->z = base.z - flare->z;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    if (diskScale > 0.0f) {
        float distance = 3072.0f, glow = facing * 4.0f;
        matrix_stack::Matrix4x4_Push(0);
        am2Vector3Scale(flare, flare, distance);
        matrix_stack::TranslateVector(flare);
        distance *= 0.15f;
        diskScale *= distance;
        Matrix4x4::Scale(diskScale, diskScale, diskScale);
        dc::sdk::DrawSingleLightMT7MeshAlpha(g_lensFlareModels[13]->root->mesh, center * 0.8f);
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::Scale(0.165f, 0.165f, 0.165f);
        dc::sdk::DrawSingleLightMT7MeshAlpha(g_lensFlareModels[12]->root->mesh, alpha);
        PopCurrentMatrix(1);
        glow *= 0.15f;
        Matrix4x4::Scale(glow, glow, glow);
        dc::sdk::DrawSingleLightMT7MeshAlpha(g_lensFlareModels[9]->root->mesh, center * 0.05f);
        PopCurrentMatrix(1);
    }
    matrix_stack::TranslateVector(&base);
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::Scale(0.1f, 0.1f, 0.1f);
    dc::sdk::DrawSingleLightMT7MeshAlpha(g_lensFlareModels[3]->root->mesh, alpha);
    PopCurrentMatrix(1);
    RotateLensFlareAlignment(offset);
    length = __builtin_sqrtf(offset->x * offset->x + offset->y * offset->y + offset->z * offset->z) * 0.7f;
    DrawLensFlareBillboard(11, length * -1.7f, 0.7f, alpha);
    DrawLensFlareBillboard(0, length * -0.3f, 0.5f, alpha);
    DrawLensFlareBillboard(1, length * -0.25f, 0.5f, alpha);
    DrawLensFlareBillboard(2, length * -0.15f, 0.5f, alpha);
    DrawLensFlareBillboard(4, length * 0.1f, 0.1f, alpha);
    DrawLensFlareBillboard(5, length * 0.125f, 0.5f, alpha);
    DrawLensFlareBillboard(7, length * 0.15f, 0.5f, alpha);
    DrawLensFlareBillboard(6, length * 0.16f, 0.5f, alpha);
    DrawLensFlareBillboard(8, length * 0.2f, 0.5f, alpha);
    DrawLensFlareBillboard(9, length * 0.25f, 0.3f, alpha);
    DrawLensFlareBillboard(10, length * 0.3f, 0.3f, alpha);
    PopCurrentMatrix(1);
}
#endif

void UpdateSunForDateTime(const DateTime &date, float latitude, Vector3 &direction, float scale) {
    volatile float intensity = scale;
    const std::int32_t *monthEnds = weatherAccumulatedMonthDays;
    const std::int32_t month = date.month - 1;
    std::int32_t days = date.day;
    days += monthEnds[month];
    days -= 21;
    const std::int32_t seasonalDays = monthEnds[5] - days;
    const float seasonalAngle = (((static_cast<float>(seasonalDays) / 365.0f) * 360.0f) / 180.0f) * 3.14f;
    float time = static_cast<float>(date.hour) + static_cast<float>(date.minute) / 60.0f;
    time += static_cast<float>(date.second) / 3600.0f;
    const float sunAngle = -(time - 12.0f) * 15.0f;
    const float seasonal = dc::sdk::CosRadians(seasonalAngle);
    const float daily = dc::sdk::CosRadians((3.14f * sunAngle) / 180.0f);
    latitude -= 23.5f * seasonal;
    const float elevation = (((90.0f - latitude) * daily) * 8192.0f) / 45.0f;
#if SHENMUE_S2_GCC295
    std::int32_t fixed = static_cast<std::int32_t>(elevation);
#else
    std::int32_t fixed = TruncateToInt32(elevation);
#endif
    const std::uint32_t elevationAngle = static_cast<std::uint32_t>(fixed) & 0xffffU;
    direction.y = intensity * dc::sdk::SinFixed(elevationAngle);
    const float cosine = dc::sdk::CosFixed(elevationAngle);
    float horizontalScale = intensity;
    horizontalScale *= cosine;
    const float phase = (sunAngle * 8192.0f) / 45.0f;
#if SHENMUE_S2_GCC295
    fixed = static_cast<std::int32_t>(phase);
#else
    fixed = TruncateToInt32(phase);
#endif
    const std::uint32_t angle = static_cast<std::uint32_t>(fixed) & 0xffffU;
    direction.x = dc::sdk::SinFixed(angle) * horizontalScale;
    direction.z = dc::sdk::CosFixed(angle) * horizontalScale;
}

} // namespace s2
} // namespace shenmue

#endif
