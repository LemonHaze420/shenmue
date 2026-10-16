#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

float EffeDot(const Vector3 &left, const Vector3 &right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

Vector3 NormalizeEffeVector(const Vector3 &value, const Vector3 &fallback) {
    const float lengthSquared = EffeDot(value, value);
    if (lengthSquared <= 0.0000001f)
        return fallback;
    const float inverse = 1.0f / std::sqrt(lengthSquared);
    return {value.x * inverse, value.y * inverse, value.z * inverse};
}

EffeProjectionBasis BuildEffeProjectionBasis() {
    constexpr float kAngleToRadians = 6.2831853071795864769f / 65536.0f;
    const Vector3 forward = NormalizeEffeVector(
        {g_CameraEyePos.x - g_CameraTargetPos.x, g_CameraEyePos.y - g_CameraTargetPos.y, g_CameraEyePos.z - g_CameraTargetPos.z}, {0.0f, 0.0f, 1.0f});
    Vector3 right = NormalizeEffeVector({forward.z, 0.0f, -forward.x}, {1.0f, 0.0f, 0.0f});
    Vector3 up{
        forward.y * right.z - forward.z * right.y,
        forward.z * right.x - forward.x * right.z,
        forward.x * right.y - forward.y * right.x,
    };
    const float roll = -static_cast<float>(static_cast<std::int16_t>(ZANGLE)) * kAngleToRadians;
    const float rollCosine = std::cos(roll);
    const float rollSine = std::sin(roll);
    const Vector3 rolledRight{
        right.x * rollCosine + up.x * rollSine,
        right.y * rollCosine + up.y * rollSine,
        right.z * rollCosine + up.z * rollSine,
    };
    up = {
        up.x * rollCosine - right.x * rollSine,
        up.y * rollCosine - right.y * rollSine,
        up.z * rollCosine - right.z * rollSine,
    };
    right = rolledRight;
    const std::int32_t fixedFov = computedCameraPitch > 0 ? computedCameraPitch : currentPerspectiveFov;
    const float tangent = std::tan(static_cast<float>(fixedFov) * kAngleToRadians * 0.5f);
    const float aspect = g_originalScreenHeight ? static_cast<float>(g_originalScreenWidth) / static_cast<float>(g_originalScreenHeight) : 4.0f / 3.0f;
    return {
        right,
        up,
        forward,
        (std::max)(tangent, 0.0000001f),
        aspect,
        (std::min)(aspect - (4.0f / 3.0f) + 1.0f, 1.33f),
    };
}

Vector3 TransformEffePoint(const Vector3 &point) {
    return {
        point.x * workTransformMatrix.rows[0].x + point.y * workTransformMatrix.rows[1].x + point.z * workTransformMatrix.rows[2].x +
            workTransformMatrix.rows[3].x,
        point.x * workTransformMatrix.rows[0].y + point.y * workTransformMatrix.rows[1].y + point.z * workTransformMatrix.rows[2].y +
            workTransformMatrix.rows[3].y,
        point.x * workTransformMatrix.rows[0].z + point.y * workTransformMatrix.rows[1].z + point.z * workTransformMatrix.rows[2].z +
            workTransformMatrix.rows[3].z,
    };
}

Vector3 EffeWorldToView(const Vector3 &world, const EffeProjectionBasis &basis) {
    const Vector3 relative{
        world.x - g_CameraTargetPos.x,
        world.y - g_CameraTargetPos.y,
        world.z - g_CameraTargetPos.z,
    };
    return {
        EffeDot(relative, basis.right),
        EffeDot(relative, basis.up),
        EffeDot(relative, basis.forward),
    };
}

void SetEffeParticlesPaused(bool paused) {
    for (EffeRuntimeTexture &runtime : g_effeRuntime) {
        if (paused)
            runtime.flags |= kParticleCallbacksSuppressedFlag;
        else
            runtime.flags &= ~kParticleCallbacksSuppressedFlag;
    }
}

// The 0x60 callback-only walker: the task owns the allocation. Suppressing
// callbacks never substitutes storage or advances particle simulation.
void RunEffeParticleCallbacks96(const EffeRuntimeTexture &runtime) {
    if ((runtime.flags & kParticleCallbacksSuppressedFlag) != 0U)
        return;
    const Matrix4x4 saved = workTransformMatrix;
    for (std::int32_t index = 0; index < runtime.particleCount; ++index) {
        EffeParticle &particle = static_cast<EffeParticle *>(runtime.particles)[index];
        particle.update(&particle);
        workTransformMatrix = saved;
    }
}

// ConstructParticleSystemElements. The next callback comes from the next record +0x08,
// not from the 0x60 task's update selector. No active/lifetime gate exists.
void ConstructParticleSystemElements(const EffeRuntimeTexture &runtime) {
    if ((runtime.flags & kParticleCallbacksSuppressedFlag) != 0U)
        return;
    const Matrix4x4 saved = workTransformMatrix;
    for (std::int32_t index = 0; index < runtime.particleCount; ++index) {
        EffeAlternateParticle &particle = static_cast<EffeAlternateParticle *>(runtime.particles)[index];
        particle.update(&particle);
        workTransformMatrix = saved;
    }
}



void ClearVertsForDrawing(D3TDeviceContext *context, const EffeRuntimeTexture &runtime) {
#if defined(_WIN32)
    const auto count = static_cast<std::uint32_t>(runtime.particleCount);
    if (count > UINT32_MAX / 0xa8U || !GetMappedConstantBuffer(context, 0, count * 0xa8U)) {
        return;
    }
    for (std::int32_t index = 0; index < runtime.particleCount; ++index) {
        EffeAlternateParticle &particle = static_cast<EffeAlternateParticle *>(runtime.particles)[index];
        particle.update(&particle);
    }
#else
    static_cast<void>(context);
    static_cast<void>(runtime);
#endif
}



void ClearVertsForDrawing_0(D3TDeviceContext *context, const EffeRuntimeTexture &runtime) {
#if defined(_WIN32)
    const auto count = static_cast<std::uint32_t>(runtime.particleCount);
    if (count > UINT32_MAX / 0xa8U)
        return;
    GetMappedConstantBuffer(context, 0, count * 0xa8U);
#else
    static_cast<void>(context);
    static_cast<void>(runtime);
#endif
}

Vector4 TransformEffePoint4(const Vector3 &point) {
    const Vector3 xyz = TransformEffePoint(point);
    return {xyz.x, xyz.y, xyz.z,
        point.x * workTransformMatrix.rows[0].w + point.y * workTransformMatrix.rows[1].w + point.z * workTransformMatrix.rows[2].w +
            workTransformMatrix.rows[3].w};
}

// SubmitAlphaQuads' perspective-0 draw (ConstructAndCommitVertsForDrawing_4, ConstructVertsForDrawing, ParticleSystem_Draw and _0, or with
// 0x02000000 set the callback-free PrepareCameraMatrices_1/_0/_2 and PrepareCameraMatrices). Each particle's callback runs from the emitter
// matrix; an active particle then becomes a view-space billboard, Z-rotated for selectors 0/2 or X/Y/Z-rotated for 1/3, projected through the
// screen transform. Selectors 2/3 write the UV rectangle; 0/1 leave the mapped UV words.
std::uint32_t DrawEffeParticleQuads(
    D3TDeviceContext *context, EffeRuntimeTexture &runtime, std::uint32_t selector, const EffeMaterialWords &material, std::uint32_t texture) {
    if (!g_matrixStack.top.stack || !g_matrixStack.entries[2].stack)
        return 0;
    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = {};
    workTransformMatrix.rows[0].x = 320.0f;
    workTransformMatrix.rows[1].y = -240.0f;
    workTransformMatrix.rows[2].z = 1.0f;
    workTransformMatrix.rows[3] = {320.0f, 240.0f, 0.0f, 1.0f};
    Matrix4x4::Multiply(g_matrixStack.entries[2].stack);
    g_particleScreenProjection = workTransformMatrix;
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    const Matrix4x4 submitted = workTransformMatrix;
    const bool callbacks = (runtime.flags & kParticleCallbacksSuppressedFlag) == 0U;
    const bool euler = (selector & 1U) != 0U;
    const bool writeTexcoords = (selector & 2U) != 0U;
    EffeParticleVertex *output = MapEffeParticleVertices(context, static_cast<std::uint32_t>(runtime.particleCount));
    std::uint32_t count = 0;
    if (output) {
        for (std::int32_t index = 0; index < runtime.particleCount; ++index) {
            workTransformMatrix = *(g_matrixStack.top.stack - 1);
            EffeParticle &particle = static_cast<EffeParticle *>(runtime.particles)[index];
            if (callbacks)
                particle.update(&particle);
            if ((particle.flags & kActiveTextureFlag) != 0) {
                const Vector4 center = TransformEffePoint4(particle.position);
                workTransformMatrix.rows[0] = {1.0f, 0.0f, 0.0f, workTransformMatrix.rows[0].w};
                workTransformMatrix.rows[1] = {0.0f, 1.0f, 0.0f, workTransformMatrix.rows[1].w};
                workTransformMatrix.rows[2] = {0.0f, 0.0f, 1.0f, workTransformMatrix.rows[2].w};
                workTransformMatrix.rows[3] = center;
                if (euler) {
                    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(particle.phase0));
                    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(particle.phase1));
                    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(particle.phase2));
                } else {
                    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(particle.phase1));
                }
                const float width = particle.scale * runtime.halfWidth;
                const float height = particle.scale * runtime.halfHeight;
                matrix_stack::Matrix4x4_Push();
                workTransformMatrix = g_particleScreenProjection;
                Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
                const std::array<Vector4, 4> corners{
                    TransformEffePoint4({-width, height, 0.0f}),
                    TransformEffePoint4({width, height, 0.0f}),
                    TransformEffePoint4({width, -height, 0.0f}),
                    TransformEffePoint4({-width, -height, 0.0f}),
                };
                if (corners[0].w > 0.0f && corners[1].w > 0.0f && corners[2].w > 0.0f && corners[3].w > 0.0f) {
                    constexpr std::array<std::size_t, 6> triangles{0, 1, 2, 0, 2, 3};
                    for (const std::size_t corner : triangles) {
                        const Vector4 &point = corners[corner];
                        output->position = {point.x / point.w, point.y / point.w, point.z / point.w, 1.0f / point.w};
                        output->diffuse = particle.colour;
                        if (writeTexcoords) {
                            output->u = corner == 1U || corner == 2U ? 1.0f : 0.0f;
                            output->v = corner >= 2U ? 1.0f : 0.0f;
                        }
                        ++output;
                    }
                    ++count;
                }
                Matrix4x4::Pop();
            }
            workTransformMatrix = submitted;
        }
    }
    const std::uint32_t drawn =
        output ? CommitEffeParticleVertices(context, count, texture, (runtime.flags & kTransparentParticlePassFlag) != 0, &material) : 0;
    Matrix4x4::Pop();
    return drawn;
}

void SubmitAlphaQuads(std::size_t slot) {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext ? DX11::g_ppD3DDeviceContext : g_RenderDeviceContext;
    if (!context)
        return;
    EffeRuntimeTexture &runtime = g_effeRuntime[slot];
    g_effeCurrentSystem = &runtime;
    if ((runtime.flags & kActiveTextureFlag) == 0U || !runtime.particles || runtime.particleCount <= 0) {
        return;
    }
    if ((runtime.flags & kCallbackOnlyParticleFlag) != 0U) {
        if ((runtime.flags & kAlternateParticleRecordFlag) != 0U)
            ConstructParticleSystemElements(runtime);
        else
            RunEffeParticleCallbacks96(runtime);
        return;
    }
    const std::uint32_t selector = (runtime.flags >> 27U) & 7U;
    if (selector > 3U) {
        const Matrix4x4 saved = workTransformMatrix;
        if ((runtime.flags & kParticleCallbacksSuppressedFlag) != 0U)
            ClearVertsForDrawing_0(context, runtime);
        else
            ClearVertsForDrawing(context, runtime);
        workTransformMatrix = saved;
        return;
    }


    if (selector == 2U && static_cast<std::uint32_t>(g_perspectiveProjection - 1) <= 2U)
        return;
    // CreateAlphaTextureForParticleEffects: descriptor words 0-2 set the render state; word 3 is the texture when word 0 bit 3 is set.
    const EffeMaterialWords material{.texture = runtime.descriptor[0], .render = runtime.descriptor[1], .alpha = runtime.descriptor[2]};
    const std::uint32_t texture = (runtime.descriptor[0] & 8U) != 0U && runtime.descriptor[3] ? runtime.descriptor[3] : d3t::g_defaultWhiteTexture;
    const std::uint32_t submitted = DrawEffeParticleQuads(context, runtime, selector, material, texture);
    g_effeSubmittedParticleCount += submitted;
    if (!submitted)
        return;
    ++g_effeSubmittedParticleSystems;
    const EffeTaskRecord *const validation = TaskAt(EffeFamily::Generic, g_effeRenderValidationSlot);
    if (validation && validation->runtimeSlot == static_cast<std::int32_t>(slot))
        g_effeRenderValidationSubmitted = true;
}

} // namespace shenmue::s1::effe_detail
