#include "s2/shadow_cast_dc.h"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

/* @verified - d */
float ComputeShadowLightSource(const Vector3 *position, Vector3 *source, float strength, unsigned long lightIndex) {
    float coefficient = strength;
    float height;
    if (lightIndex || strength == -1.0f)
        coefficient = 0.5f;
    if (strength == -2.0f)
        coefficient = 1.0f;
    switch (GetLightRuntimeKind(lightIndex)) {
    case 2:
        GetLightRuntimeDirection(lightIndex, source);
        source->x = position->x - source->x;
        source->y = position->y - source->y;
        source->z = position->z - source->z;
        break;
    case 3:
    case 4:
        GetLightRuntimePosition(lightIndex, source);
        height = source->y - position->y;
        if (height < -0.3f)
            coefficient = 0.0f;
        else if (height < 1.0f)
            source->y = position->y + 1.0f;
        break;
    default:
        coefficient = 0.0f;
    }
    if (coefficient < 0.0f)
        coefficient = 0.0f;
    else if (coefficient > 1.0f)
        coefficient = 1.0f;
    return coefficient;
}

/* @verified - d */
void ApplyDirectionAlignedShadowProjection(Vector3 position, Vector3 source, ShadowPlane plane) {
    Vector3 direction;
    Matrix4x4_Push(0);
    direction.x = position.x - source.x;
    direction.y = position.y - source.y;
    direction.z = position.z - source.z;
    am2MultiplyPlaneProjection(&plane, &direction);
}

/* @verified - d */
float GetCharacterShadowDistanceFade(const Vector3 *position, const Character *character) {
    Vector3 camera;
    float distance, fade;
    camCtrlGetNextCamPos(&camera);
    distance = CalculatePlanarDistanceBetweenPoints(position, &camera);
    fade = GetShadowRange() - distance;
    if (g_shadowDistanceFadeWidth < 0.01f)
        fade = 1.0f;
    else
        fade /= g_shadowDistanceFadeWidth;
    if (fade < 0.0f)
        fade = 0.0f;
    else if (fade > 1.0f)
        fade = 1.0f;
    if (character)
        fade *= GetCharacterAlpha(character);
    return fade;
}

/* @verified - d */
void DrawMotionCharacterShadow(const Vector3 *position, float groundY, MT7Node *model, MT7Node *shadow, float strength, Character *character) {
    CharacterLightControl *control;
    ShadowPlane plane;
    Vector3 source;
    float fade, weight, alpha;
    long slot;
    if (g_shadowVolumesDisabled || !g_shadowRenderingEnabled || !shadow || strength == 0.0f || !character)
        return;
    control = GetControllerForNode(character);
    if (!control || !control->mode)
        return;
    source = *position;
    source.y += 0.5f;
    flddPushIntersectionFlag(0x20);
    if (FindFieldIntersectionHeight(&source) > position->y - 1.5f)
        return;
    plane.point.x = 0.0f;
    plane.point.y = groundY + 0.013f;
    plane.point.z = 0.0f;
    plane.normal.x = 0.0f;
    plane.normal.y = 1.0f;
    plane.normal.z = 0.0f;
    if (model != shadow)
        PublishSharedShadowNodeTransforms(model, shadow);
    fade = GetCharacterShadowDistanceFade(position, character);
    if (CHARACTER_SHADOW_THRESHOLD > fade)
        return;
    PrepareCharacterShadowLighting();
    for (slot = 0; slot < CHARACTER_LIGHT_COUNT && control->ids[slot] >= 0; ++slot) {
        weight = __builtin_sqrtf(control->intensities[slot]) * control->weights[slot];
        if (CHARACTER_SHADOW_THRESHOLD > weight)
            continue;
        alpha = ComputeShadowLightSource(position, &source, strength, control->ids[slot]);
        alpha *= fade * weight;
        if (CHARACTER_SHADOW_THRESHOLD > alpha)
            continue;
        ApplyDirectionAlignedShadowProjection(*position, source, plane);
        mtUpdateTransparencyCallback(shadow, alpha);
        PopCurrentMatrix(1);
    }
    RestoreCharacterShadowLighting();
}

/* @verified - d */
void DrawProjectedSharedCharacterShadow(const Vector3 *position, unsigned long lightIndex, float groundY, Character *character, float opacity) {
    MT7Node *model, *shadow;
    ShadowPlane plane;
    Vector3 source;
    if (!position || !(opacity > CHARACTER_SHADOW_THRESHOLD) || !character)
        return;
    model = character->model;
    shadow = character->shadowModel;
    if (!model || !shadow)
        return;
    plane.point.x = 0.0f;
    plane.point.y = groundY + 0.013f;
    plane.point.z = 0.0f;
    plane.normal.x = 0.0f;
    plane.normal.y = 1.0f;
    plane.normal.z = 0.0f;
    if (model != shadow)
        PublishSharedShadowNodeTransforms(model, shadow);
    opacity *= ComputeShadowLightSource(position, &source, -1.0f, lightIndex);
    if (CHARACTER_SHADOW_THRESHOLD > opacity)
        return;
    ApplyDirectionAlignedShadowProjection(*position, source, plane);
    mtUpdateTransparencyCallback(shadow, opacity);
    PopCurrentMatrix(1);
}

/* @verified - d */
void DrawProjectedCharacterShadow(CharacterUpdateCallback callback, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale, float groundY,
    float strength, const MT7Node *sourceRoot, const MT7Node *shadowRoot, Character *character) {
    CharacterLightControl *control;
    ShadowPlane plane;
    Vector3 source;
    long scaled, slot;
    float maximumScale, fade, weight, alpha;
    if (!shadowRoot)
        shadowRoot = sourceRoot;
    if (g_shadowVolumesDisabled || !g_shadowRenderingEnabled || !shadowRoot || strength == 0.0f || !character)
        return;
    control = GetControllerForNode(character);
    if (!control || !control->mode)
        return;
    plane.point.x = 0.0f;
    plane.point.y = groundY + 0.013f;
    plane.point.z = 0.0f;
    plane.normal.x = 0.0f;
    plane.normal.y = 1.0f;
    plane.normal.z = 0.0f;
    if (scale->x != 1.0f || scale->y != 1.0f || scale->z != 1.0f) {
        scaled = 1;
        maximumScale = scale->x;
        if (scale->y > maximumScale)
            maximumScale = scale->y;
        if (scale->z > maximumScale)
            maximumScale = scale->z;
    } else {
        scaled = 0;
        maximumScale = 1.0f;
    }
    fade = GetCharacterShadowDistanceFade(position, character);
    if (CHARACTER_SHADOW_THRESHOLD > fade)
        return;
    PrepareCharacterShadowLighting();
    for (slot = 0; slot < CHARACTER_LIGHT_COUNT && control->ids[slot] >= 0; ++slot) {
        weight = __builtin_sqrtf(control->intensities[slot]) * control->weights[slot];
        if (CHARACTER_SHADOW_THRESHOLD > weight)
            continue;
        alpha = ComputeShadowLightSource(position, &source, strength, control->ids[slot]);
        alpha *= fade * weight;
        if (CHARACTER_SHADOW_THRESHOLD > alpha)
            continue;
        ApplyDirectionAlignedShadowProjection(*position, source, plane);
        Matrix4x4_preTranslate(position->x, position->y, position->z);
        if (rotation->z)
            Matrix4x4_RotateAroundZ(rotation->z);
        if (rotation->y)
            Matrix4x4_RotateAroundY(rotation->y);
        if (rotation->x)
            Matrix4x4_RotateAroundX(rotation->x);
        if (scaled)
            Matrix4x4_Scale(scale->x, scale->y, scale->z);
        SetModelScale(maximumScale + maximumScale);
        callback(shadowRoot, alpha);
        PopCurrentMatrix(1);
    }
    SetModelScale(1.0f);
    RestoreCharacterShadowLighting();
}

/* @verified - d */
void DrawProjectedFixedShadow(
    const Vector3 *position, const Vector3i *rotation, const Vector3 *scale, float groundY, void *mesh, float strength, Character *character) {
    CharacterLightControl *control;
    ShadowPlane plane;
    Vector3 source;
    long scaled, slot;
    float maximumScale, fade, weight, alpha;
    if (g_shadowVolumesDisabled || !g_shadowRenderingEnabled || !mesh || strength == 0.0f || !character)
        return;
    control = GetControllerForNode(character);
    if (!control || !control->mode)
        return;
    plane.point.x = 0.0f;
    plane.point.y = groundY + 0.013f;
    plane.point.z = 0.0f;
    plane.normal.x = 0.0f;
    plane.normal.y = 1.0f;
    plane.normal.z = 0.0f;
    if (scale->x != 1.0f || scale->y != 1.0f || scale->z != 1.0f) {
        scaled = 1;
        maximumScale = scale->x;
        if (scale->y > maximumScale)
            maximumScale = scale->y;
        if (scale->z > maximumScale)
            maximumScale = scale->z;
    } else {
        scaled = 0;
        maximumScale = 1.0f;
    }
    fade = GetCharacterShadowDistanceFade(position, character);
    if (CHARACTER_SHADOW_THRESHOLD > fade)
        return;
    PrepareCharacterShadowLighting();
    for (slot = 0; slot < CHARACTER_LIGHT_COUNT && control->ids[slot] >= 0; ++slot) {
        weight = __builtin_sqrtf(control->intensities[slot]) * control->weights[slot];
        if (CHARACTER_SHADOW_THRESHOLD > weight)
            continue;
        alpha = ComputeShadowLightSource(position, &source, strength, control->ids[slot]);
        alpha *= fade * weight;
        if (CHARACTER_SHADOW_THRESHOLD > alpha)
            continue;
        ApplyDirectionAlignedShadowProjection(*position, source, plane);
        Matrix4x4_preTranslate(position->x, position->y, position->z);
        if (rotation->z)
            Matrix4x4_RotateAroundZ(rotation->z);
        if (rotation->y)
            Matrix4x4_RotateAroundY(rotation->y);
        if (rotation->x)
            Matrix4x4_RotateAroundX(rotation->x);
        if (scaled)
            Matrix4x4_Scale(scale->x, scale->y, scale->z);
        SetModelScale(maximumScale + maximumScale);
        if (alpha < 1.0f)
            DrawSingleLightMT7MeshAlpha(mesh, alpha);
        else
            DrawSingleLightMT7Mesh(mesh);
        PopCurrentMatrix(1);
    }
    SetModelScale(1.0f);
    RestoreCharacterShadowLighting();
}

/* @verified - d */
void DrawCharacterShadowSprite(const Vector3 *position, float groundY, Character *character) {
    CharacterLightControl *control;
    ShadowPlane plane;
    Vector3 source;
    long slot;
    float fade, weight, alpha;
    if (g_shadowVolumesDisabled || !g_shadowRenderingEnabled || !g_shadowSprite || !g_shadowSprite->mesh || !character)
        return;
    control = GetControllerForNode(character);
    if (!control || !control->mode)
        return;
    plane.point.x = 0.0f;
    plane.point.y = groundY + 0.013f;
    plane.point.z = 0.0f;
    plane.normal.x = 0.0f;
    plane.normal.y = 1.0f;
    plane.normal.z = 0.0f;
    fade = GetCharacterShadowDistanceFade(position, character);
    if (CHARACTER_SHADOW_THRESHOLD > fade)
        return;
    PrepareCharacterShadowLighting();
    for (slot = 0; slot < CHARACTER_LIGHT_COUNT && control->ids[slot] >= 0; ++slot) {
        weight = __builtin_sqrtf(control->intensities[slot]) * control->weights[slot];
        if (CHARACTER_SHADOW_THRESHOLD > weight)
            continue;
        alpha = ComputeShadowLightSource(position, &source, -1.0f, control->ids[slot]);
        alpha *= fade * weight;
        if (CHARACTER_SHADOW_THRESHOLD > alpha)
            continue;
        ApplyDirectionAlignedShadowProjection(*position, source, plane);
        source.x -= position->x;
        source.y -= position->y;
        source.z -= position->z;
        am2Vector3NormalizeInPlace(&source);
        Matrix4x4_preTranslate(source.x * 0.2f + position->x, position->y, source.z * 0.2f + position->z);
        weight = source.x * source.x + source.z * source.z;
        if (weight > 0.000001f) {
            weight = 1.0f / __builtin_sqrtf(weight);
            Matrix4x4_RotateYSinCos(-source.x * weight, -source.z * weight);
        }
        Matrix4x4_Scale(0.5f, 1.7f, 1.0f);
        SetModelScale(3.0f);
        if (alpha < 1.0f)
            DrawSingleLightMT7MeshAlpha(g_shadowSprite->mesh, alpha);
        else
            DrawSingleLightMT7Mesh(g_shadowSprite->mesh);
        PopCurrentMatrix(1);
    }
    SetModelScale(1.0f);
    RestoreCharacterShadowLighting();
}

#endif
