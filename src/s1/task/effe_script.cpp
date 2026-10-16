#include "s1/task/effe_internal.hpp"

#include "s1/task/effe.hpp"

#include <array>
#include <cstring>

namespace shenmue::s1::effe_detail {

template <typename T> const T *EffectScriptPointer(std::uint32_t offset) {
    return offset ? reinterpret_cast<const T *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) : nullptr;
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1::effe_detail {

float EffectParameterFloat(const std::uint32_t *parameters, std::size_t index) {
    float value = 0.0f;
    std::memcpy(&value, parameters + index, sizeof(value));
    return value;
}

void SetEffectResult(ScenarioContext *context, std::int32_t value) {
    if (context && context->stack)
        context->stack->accumulator = static_cast<std::uint32_t>(value);
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_ScriptWeatherUtilsForFIXC(ScenarioContext *context, void *rawParameters) {
    const auto *const parameters = static_cast<const std::uint32_t *>(rawParameters);
    if (!parameters)
        return;
    const auto setResult = [context](WeatherFixc *fixc) {
        if (context && context->stack)
            context->stack->accumulator = WeatherFixcOffset(fixc);
    };
    const auto fixcFromOffset = [](std::uint32_t offset) -> WeatherFixc * {
        if (!offset || !g_pCurrentTaskData)
            return nullptr;
        return reinterpret_cast<WeatherFixc *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset));
    };

    switch (parameters[0]) {
    case 1: {
        WeatherFixc *const fixc = AllocateWeatherFixc(0, static_cast<std::int32_t>(parameters[1]));
        AttachWeatherNode(fixc);
        setResult(fixc);
        break;
    }
    case 6: {
        const Vector3 center{std::bit_cast<float>(parameters[1]), std::bit_cast<float>(parameters[2]), std::bit_cast<float>(parameters[3])};
        WeatherFixc *const fixc =
            AllocateWeatherFixc(6, 0, &center, std::bit_cast<float>(parameters[4]), std::bit_cast<float>(parameters[5]), std::bit_cast<float>(parameters[6]));
        AttachWeatherNode(fixc);
        setResult(fixc);
        break;
    }
    case 7:
        FreeWeatherFixc(fixcFromOffset(parameters[1]));
        break;
    case 8: {
        WeatherFixc *const fixc = AllocateWeatherFixc(1, static_cast<std::int32_t>(parameters[1]));
        AttachWeatherNode(fixc);
        setResult(fixc);
        break;
    }
    case 9:
        if (g_tmpWeatherData)
            FreeWeatherFixc(g_tmpWeatherData);
        break;
    case 10:
        if (g_tmpWeatherData && g_effeWeatherFlag) {
            RemoveActiveWeatherNode(g_tmpWeatherData);
            g_effeWeatherFlag = false;
        }
        break;
    case 11:
        AllocFIXC();
        break;
    case 12:
        g_bForceDisableSnow = static_cast<std::int32_t>(parameters[1]);
        break;
    case 13:
        g_shouldLoadWeatherSprites = static_cast<std::int32_t>(parameters[1]);
        break;
    case 14:
        SetShouldLoadWeatherSprites(static_cast<std::int32_t>(parameters[1]));
        break;
    default:
        break;
    }
}

void Script_sakuraUtils(ScenarioContext *context, void *rawParameters) {
    const auto *p = static_cast<const std::uint32_t *>(rawParameters);
    if (!p)
        return;
    const std::int32_t slot = static_cast<std::int32_t>(p[1]);

    switch (p[0]) {
    case 0: {
        const Vector3 *position = EffectScriptPointer<Vector3>(p[1]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[2]);
        SetEffectResult(
            context, position && range ? CreateSakuraEffe(*position, *range, static_cast<std::int32_t>(p[3]), EffectParameterFloat(p, 4), p[5]) : -1);
        break;
    }
    case 1:
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            SetSakuraActive(*task, true);
        break;
    case 2:
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            SetSakuraActive(*task, false);
        break;
    case 3:
        FreeSakuraTask(SakuraTaskAt(slot));
        break;
    case 4:
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            SetEffectResult(context, (task->flags >> 1U) & 1U);
        else
            SetEffectResult(context, -1);
        break;
    case 5: {
        EffeTaskRecord *task = SakuraTaskAt(slot);
        const Vector3 *position = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && position && range)
            SetSakuraPositionRange(*task, *position, *range);
        break;
    }
    case 6: {
        EffeTaskRecord *task = SakuraTaskAt(slot);
        const Vector3 *velocity = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && velocity && range)
            SetSakuraVelocityRange(*task, *velocity, *range);
        break;
    }
    case 7:
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            SetSakuraProbability(*task, EffectParameterFloat(p, 2));
        break;
    case 8:
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            SetSakuraDimensions(*task, EffectParameterFloat(p, 2), EffectParameterFloat(p, 2));
        break;
    case 0x0a:
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            task->particleScale = EffectParameterFloat(p, 2);
        break;
    case 0x0d:
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            SetSakuraDimensions(*task, EffectParameterFloat(p, 2), EffectParameterFloat(p, 3));
        break;
    case 0x0f:
        LoadSakuraResource();
        break;
    case 0x32: {
        EffeTaskRecord *task = SakuraTaskAt(slot);
        const Vector3 *amount = EffectScriptPointer<Vector3>(p[2]);
        if (task && amount)
            TranslateSakuraParticles(*task, *amount);
        break;
    }
    default:
        break;
    }
}

void Script_InitSakuraEffectTasks_0(ScenarioContext *, void *) {
    constexpr std::array<Vector3, 7> origins{{
        {2.9285f, 8.2785f, -2.24f},
        {0.0485f, 6.9385f, -3.24f},
        {-3.1115f, 4.6785f, -1.24f},
        {-1.6515f, 4.9585f, 0.74f},
        {1.4685f, 4.9385f, 2.68f},
        {3.1285f, 4.3185f, 3.02f},
        {4.3285f, 6.5985f, -0.06f},
    }};
    constexpr Vector3 range{0.7f, 0.7f, 0.7f};
    LoadSakuraResource();
    for (const Vector3 &origin : origins) {
        const std::int32_t slot = CreateSakuraEffe(origin, range, 2000, 0.0f, 0);
        if (EffeTaskRecord *task = SakuraTaskAt(slot))
            SetSakuraActive(*task, true);
    }
    g_sakuraUpdateTask = HLib_EnqueueTaskWithoutParameter(SakuraTaskCallback, 8, 8, static_cast<std::int32_t>(SHEN_NAME(TKEX)));
}

void Script_EffectUtils(ScenarioContext *context, void *rawParameters) {
    const auto *p = static_cast<const std::uint32_t *>(rawParameters);
    if (!p)
        return;
    const std::int32_t slot = static_cast<std::int32_t>(p[1]);
    EffeTaskRecord *task = TaskAt(EffeFamily::Special, slot);
    switch (p[0]) {
    case 0: {
        const Vector3 *position = EffectScriptPointer<Vector3>(p[1]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[2]);
        SetEffectResult(context,
            position && range ?
                CreateSpecialEffe(*position, *range, static_cast<std::int32_t>(p[3]), EffectParameterFloat(p, 4), p[5], static_cast<std::int32_t>(p[6])) :
                -1);
        break;
    }
    case 1:
        if (task)
            SetEffeActive(*task, true);
        break;
    case 2:
        if (task)
            SetEffeActive(*task, false);
        break;
    case 3:
        FreeEffeTask(EffeFamily::Special, slot);
        break;
    case 4:
        SetEffectResult(context, task ? static_cast<std::int32_t>((task->flags >> 1U) & 1U) : -1);
        break;
    case 5: {
        const Vector3 *position = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && position && range)
            SetEffePositionRange(*task, *position, *range);
        break;
    }
    case 6: {
        const Vector3 *velocity = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && velocity && range)
            SetEffeVelocityRange(*task, *velocity, *range);
        break;
    }
    case 7:
        if (task)
            SetEffeProbability(*task, EffectParameterFloat(p, 2));
        break;
    case 8:
        if (task)
            SetEffeDimensions(*task, EffectParameterFloat(p, 2), EffectParameterFloat(p, 3));
        break;
    case 9:
        if (task) {
            const std::uint32_t colour = ((p[2] & 0xffU) << 16U) | ((p[3] & 0xffU) << 8U) | (p[4] & 0xffU);
            g_effeRuntime[static_cast<std::size_t>(task->runtimeSlot)].colour = colour;
            for (std::int32_t index = 0; index < task->particleCount; ++index) {
                task->particles[static_cast<std::size_t>(index)].colour = colour;
            }
        }
        break;
    case 0x0a:
        if (task) {
            task->particleScale = EffectParameterFloat(p, 2);
            if (task->subtype == 0 || (task->subtype >= 2 && task->subtype <= 4))
                g_specialLifetime[static_cast<std::size_t>(task->subtype)] = task->particleScale;
        }
        break;
    case 0x0b:
        if (task) {
            const float intensity = static_cast<float>(static_cast<std::int32_t>(p[2]));
            if (task->fftOwner) {
                std::array<float, 64> table{};
                PrecomputeLogTable(static_cast<float>(static_cast<double>(intensity) * 10.0 / 255.0 - 2.0), table.data());
                ApplyLogClampVectorScaling(static_cast<float>((static_cast<double>(task->decayScale) - 0.7) * 100.0), table.data());
                std::memcpy(g_effeFftData[static_cast<std::size_t>(task->fftOwner->fftIndex)]->falloff.data(), table.data(), sizeof(table));
            }
            if (task->subtype >= 0 && task->subtype <= 4) {
                task->decayStep = static_cast<float>(static_cast<double>(intensity) / 255.0 * 0.02 - 0.004);
                if (task->subtype != 1)
                    g_specialDecayStep[static_cast<std::size_t>(task->subtype)] = task->decayStep;
            }
        }
        break;
    case 0x0c:
        if (task) {
            const float intensity = static_cast<float>(static_cast<std::int32_t>(p[2]));
            if (task->fftOwner) {
                std::array<float, 64> table{};
                PrecomputeLogTable(static_cast<float>((static_cast<double>(task->decayStep) + 0.004) * 10.0 / 0.02 - 2.0), table.data());
                ApplyLogClampVectorScaling(static_cast<float>(static_cast<double>(intensity) * 100.0 / 255.0), table.data());
                std::memcpy(g_effeFftData[static_cast<std::size_t>(task->fftOwner->fftIndex)]->falloff.data(), table.data(), sizeof(table));
            }
            if (task->subtype >= 0 && task->subtype <= 4) {
                task->decayScale = static_cast<float>(static_cast<double>(intensity) / 255.0 + 0.7);
                if (task->subtype != 1)
                    g_specialDecayScale[static_cast<std::size_t>(task->subtype)] = task->decayScale;
            }
        }
        break;
    case 0x0d:
        if (task && task->subtype == 1)
            g_specialIntensity = static_cast<float>(static_cast<std::int32_t>(p[2]));
        break;
    case 0x0e:
        if (task && (task->subtype == 3 || task->subtype == 4)) {
            const std::size_t index = static_cast<std::size_t>(task->subtype - 3);
            g_specialRadial[index] = EffectParameterFloat(p, 2);
            g_specialDamping[index] = EffectParameterFloat(p, 3);
        }
        break;
    case 0x0f:
        if (task && (task->subtype == 3 || task->subtype == 4)) {
            const std::size_t index = static_cast<std::size_t>(task->subtype - 3);
            g_specialMultiplier[index] = EffectParameterFloat(p, 3);
            g_specialFadeStep[index] =
                std::max(static_cast<std::int32_t>((static_cast<double>(EffectParameterFloat(p, 2)) - 1.0) * 13.88888888888889 + 3.0), 0);
        }
        break;
    case 0x10:
        if (task && task->fftOwner) {
            EffeFftData &fft = *g_effeFftData[static_cast<std::size_t>(task->fftOwner->fftIndex)];
            fft.phaseRateScale = EffectParameterFloat(p, 2);
            NormalizeAndScaleFFTInput(fft.phaseRates.data(), fft.phaseRateScale);
        }
        break;
    case 0x11:
        if (task && task->fftOwner) {
            std::array<float, 64> table{};
            PrecomputeLogTable(EffectParameterFloat(p, 2), table.data());
            ApplyLogClampVectorScaling(EffectParameterFloat(p, 3), table.data());
            std::memcpy(g_effeFftData[static_cast<std::size_t>(task->fftOwner->fftIndex)]->falloff.data(), table.data(), sizeof(table));
        }
        break;
    case 0x32: {
        const Vector3 *amount = EffectScriptPointer<Vector3>(p[2]);
        if (task && amount)
            TranslateEffe(*task, *amount);
        break;
    }
    default:
        break;
    }
}

void Script_SetEFPT(ScenarioContext *context, void *rawParameters) {
    const auto *p = static_cast<const std::uint32_t *>(rawParameters);
    if (!p)
        return;
    const std::int32_t slot = static_cast<std::int32_t>(p[1]);
    EffeTaskRecord *task = TaskAt(EffeFamily::Efpt, slot);
    switch (p[0]) {
    case 0:
    case 0x0a: {
        const Vector3 *position = EffectScriptPointer<Vector3>(p[1]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[2]);
        const bool withFloor = p[0] == 0x0a;
        const float floor = withFloor ? EffectParameterFloat(p, 6) : 0.0f;
        SetEffectResult(context,
            position && range ? CreateEfptEffe(*position, *range, static_cast<std::int32_t>(p[3]), EffectParameterFloat(p, 4), p[5], floor,
                                    EffectScriptPointer<char>(p[withFloor ? 7 : 6]), EffectScriptPointer<char>(p[withFloor ? 8 : 7])) :
                                -1);
        break;
    }
    case 1:
        if (task)
            SetEffeActive(*task, true);
        break;
    case 2:
        if (task)
            SetEffeActive(*task, false);
        break;
    case 3:
        FreeEffeTask(EffeFamily::Efpt, slot);
        break;
    case 4:
        SetEffectResult(context, task ? static_cast<std::int32_t>((task->flags >> 1U) & 1U) : -1);
        break;
    case 5: {
        const Vector3 *position = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && position && range)
            SetEffePositionRange(*task, *position, *range);
        break;
    }
    case 6: {
        const Vector3 *velocity = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && velocity && range)
            SetEffeVelocityRange(*task, *velocity, *range);
        break;
    }
    case 7:
        if (task)
            SetEffeProbability(*task, EffectParameterFloat(p, 2));
        break;
    case 8:
        if (task)
            SetEffeDimensions(*task, EffectParameterFloat(p, 2), EffectParameterFloat(p, 2));
        break;
    case 9:
        if (task)
            task->particleScale = EffectParameterFloat(p, 2);
        break;
    case 0x0b:
        if (task) {
            EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task->runtimeSlot)];
            runtime.colour = (p[2] << 24U) | ((p[3] & 0xffU) << 16U) | ((p[4] & 0xffU) << 8U) | (p[5] & 0xffU);
            for (std::int32_t index = 0; index < task->particleCount; ++index)
                task->particles[static_cast<std::size_t>(index)].colour = runtime.colour;
        }
        break;
    case 0x12:
        if (task) {
            task->floor = EffectParameterFloat(p, 2);
            for (std::int32_t index = 0; index < task->particleCount; ++index)
                task->particles[static_cast<std::size_t>(index)].attribute = reinterpret_cast<std::intptr_t>(&task->floor);
        }
        break;
    case 0x32: {
        const Vector3 *amount = EffectScriptPointer<Vector3>(p[2]);
        if (task && amount)
            TranslateEffe(*task, *amount);
        break;
    }
    default:
        break;
    }
}

void Script_EffectUtils_0(ScenarioContext *context, void *rawParameters) {
    const auto *p = static_cast<const std::uint32_t *>(rawParameters);
    if (!p)
        return;
    const std::int32_t slot = static_cast<std::int32_t>(p[1]);
    EffeTaskRecord *task = TaskAt(EffeFamily::Generic, slot);
    switch (p[0]) {
    case 0:
    case 0x0a: {
        const Vector3 *position = EffectScriptPointer<Vector3>(p[1]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[2]);
        const float floor = p[0] == 0x0a ? EffectParameterFloat(p, 8) : -1.0f;
        SetEffectResult(context,
            position && range ? CreateGenericEffe(*position, *range, static_cast<std::int32_t>(p[3]), EffectParameterFloat(p, 4), p[5], floor,
                                    EffectScriptPointer<char>(p[6]), EffectScriptPointer<char>(p[7])) :
                                -1);
        break;
    }
    case 1:
        if (task)
            SetEffeActive(*task, true);
        break;
    case 2:
        if (task)
            SetEffeActive(*task, false);
        break;
    case 3:
        FreeEffeTask(EffeFamily::Generic, slot);
        break;
    case 4:
        SetEffectResult(context, task ? static_cast<std::int32_t>((task->flags >> 1U) & 1U) : -1);
        break;
    case 5: {
        const Vector3 *position = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && position && range)
            SetEffePositionRange(*task, *position, *range);
        break;
    }
    case 6: {
        const Vector3 *velocity = EffectScriptPointer<Vector3>(p[2]);
        const Vector3 *range = EffectScriptPointer<Vector3>(p[3]);
        if (task && velocity && range)
            SetEffeVelocityRange(*task, *velocity, *range);
        break;
    }
    case 7:
        if (task)
            SetEffeProbability(*task, EffectParameterFloat(p, 2));
        break;
    case 8:
        if (task)
            SetEffeDimensions(*task, EffectParameterFloat(p, 2), EffectParameterFloat(p, 3));
        break;
    case 9:
        if (task)
            task->particleScale = EffectParameterFloat(p, 2);
        break;
    case 0x32: {
        EffeTaskRecord *sakura = SakuraTaskAt(slot);
        const Vector3 *amount = EffectScriptPointer<Vector3>(p[2]);
        if (sakura && amount)
            TranslateSakuraParticles(*sakura, *amount);
        break;
    }
    default:
        break;
    }
}

void Script_effeInitTexture2D(ScenarioContext *, void *) {
    for (std::size_t index = 0; index < g_effeRuntime.size(); ++index) {
        EffeRuntimeTexture &runtime = g_effeRuntime[index];
        if (!runtime.resource)
            continue;
        ReleaseEffeTexture(static_cast<std::int32_t>(index));
    }
}

} // namespace shenmue::s1
