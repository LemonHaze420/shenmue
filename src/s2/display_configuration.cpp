#include "s2/display_configuration.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <dxgi.h>
#endif

namespace shenmue::s2 {
namespace {
S2DisplayOutput *outputs;
std::uint32_t outputCount;
std::uint32_t outputCapacity;

#if defined(_WIN32)
d3t::DisplayModeInfo *AppendMode(S2DisplayOutput &output) {
    if (output.modeCount >= output.modeCapacity) {
        const std::uint32_t capacity = output.modeCapacity < 8 ? 16 : output.modeCapacity * 2;
        void *const storage = std::realloc(output.modes, sizeof(d3t::DisplayModeInfo) * capacity);
        if (!storage)
            return nullptr;
        output.modes = static_cast<d3t::DisplayModeInfo *>(storage);
        output.modeCapacity = capacity;
    }
    d3t::DisplayModeInfo *const mode = &output.modes[output.modeCount++];
    *mode = {};
    return mode;
}

void StoreMode(d3t::DisplayModeInfo &destination, const DXGI_MODE_DESC &source) {
    destination.width = source.Width;
    destination.height = source.Height;
    destination.refreshRateNumerator = source.RefreshRate.Numerator;
    destination.refreshRateDenominator = source.RefreshRate.Denominator;
    std::snprintf(destination.description, sizeof(destination.description), "%u x %u", source.Width, source.Height);
}

void ReadDisplayModes(S2DisplayOutput &output, IDXGIOutput *deviceOutput) {
    UINT count = 0;
    HRESULT result = deviceOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_ENUM_MODES_INTERLACED, &count, nullptr);
    if (SUCCEEDED(result)) {
        DXGI_MODE_DESC *const modes = static_cast<DXGI_MODE_DESC *>(std::malloc(sizeof(DXGI_MODE_DESC) * count));
        if (!modes && count)
            return;
        result = deviceOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_ENUM_MODES_INTERLACED, &count, modes);
        if (SUCCEEDED(result)) {
            for (UINT index = 0; index < count; ++index) {
                const DXGI_MODE_DESC &mode = modes[index];
                if (mode.Width < 640 || mode.Height < 480)
                    continue;
                d3t::DisplayModeInfo *previous = nullptr;
                for (std::uint32_t i = 0; i < output.modeCount; ++i) {
                    if (output.modes[i].width == mode.Width && output.modes[i].height == mode.Height) {
                        previous = &output.modes[i];
                        break;
                    }
                }
                if (previous) {
                    const float rate = static_cast<float>(mode.RefreshRate.Numerator) / static_cast<float>(mode.RefreshRate.Denominator);
                    const float oldRate = static_cast<float>(previous->refreshRateNumerator) / static_cast<float>(previous->refreshRateDenominator);
                    const float remainder = std::fmod(rate, 30.0f);
                    if ((remainder < 1.0f || remainder > 29.0f) && rate > oldRate) {
                        previous->refreshRateNumerator = mode.RefreshRate.Numerator;
                        previous->refreshRateDenominator = mode.RefreshRate.Denominator;
                    }
                } else {
                    d3t::DisplayModeInfo *const destination = AppendMode(output);
                    if (destination)
                        StoreMode(*destination, mode);
                }
            }
        }
        if (!output.modeCount && count) {
            d3t::DisplayModeInfo *const destination = AppendMode(output);
            if (destination)
                StoreMode(*destination, modes[count - 1]);
        }
        std::free(modes);
    } else if (result == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE) {
        DXGI_OUTPUT_DESC description{};
        if (SUCCEEDED(deviceOutput->GetDesc(&description))) {
            d3t::DisplayModeInfo *const destination = AppendMode(output);
            if (!destination)
                return;
            destination->width = description.DesktopCoordinates.right - description.DesktopCoordinates.left;
            destination->height = description.DesktopCoordinates.bottom - description.DesktopCoordinates.top;
            destination->refreshRateNumerator = 1;
            destination->refreshRateDenominator = 60;
            std::snprintf(destination->description, sizeof(destination->description), "%u x %u", destination->width, destination->height);
        }
    }
}
#endif
}

void EnumerateS2Displays() {
#if defined(_WIN32)
    IDXGIFactory *factory = nullptr;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&factory))))
        return;
    for (UINT adapterIndex = 0;; ++adapterIndex) {
        IDXGIAdapter *adapter = nullptr;
        const HRESULT adapterResult = factory->EnumAdapters(adapterIndex, &adapter);
        if (adapterResult == DXGI_ERROR_NOT_FOUND)
            break;
        if (FAILED(adapterResult) || !adapter)
            continue;
        for (UINT outputIndex = 0;; ++outputIndex) {
            IDXGIOutput *deviceOutput = nullptr;
            const HRESULT outputResult = adapter->EnumOutputs(outputIndex, &deviceOutput);
            if (outputResult == DXGI_ERROR_NOT_FOUND)
                break;
            if (FAILED(outputResult) || !deviceOutput)
                continue;
            if (outputCount >= outputCapacity) {
                const std::uint32_t capacity = outputCapacity < 8 ? 16 : outputCapacity * 2;
                void *const storage = std::realloc(outputs, sizeof(S2DisplayOutput) * capacity);
                if (!storage) {
                    deviceOutput->Release();
                    continue;
                }
                outputs = static_cast<S2DisplayOutput *>(storage);
                outputCapacity = capacity;
            }
            S2DisplayOutput &output = outputs[outputCount++];
            output = {};
            DXGI_OUTPUT_DESC description{};
            if (SUCCEEDED(deviceOutput->GetDesc(&description))) {
                char deviceName[128]{};
                std::wcstombs(deviceName, description.DeviceName, sizeof(deviceName) - 1);
                DISPLAY_DEVICEA display{.cb = sizeof(DISPLAY_DEVICEA)};
                if (EnumDisplayDevicesA(deviceName, 0, &display, 0))
                    std::snprintf(output.name, sizeof(output.name), "%u. %s", outputCount, display.DeviceString);
                output.left = description.DesktopCoordinates.left;
                output.top = description.DesktopCoordinates.top;
                output.right = description.DesktopCoordinates.right;
                output.bottom = description.DesktopCoordinates.bottom;
                MONITORINFO monitor{.cbSize = sizeof(MONITORINFO)};
                if (GetMonitorInfoA(description.Monitor, &monitor))
                    output.primary = monitor.dwFlags & MONITORINFOF_PRIMARY;
            }
            ReadDisplayModes(output, deviceOutput);
            deviceOutput->Release();
        }
        adapter->Release();
    }
    factory->Release();
#endif
}

void ReleaseS2Displays() {
    for (std::uint32_t index = 0; index < outputCount; ++index)
        std::free(outputs[index].modes);
    std::free(outputs);
    outputs = nullptr;
    outputCount = outputCapacity = 0;
}

const S2DisplayOutput *GetS2DisplayOutput(std::int32_t index) noexcept {
    return static_cast<std::uint32_t>(index) < outputCount ? &outputs[index] : nullptr;
}

const d3t::DisplayModeInfo *GetS2DisplayResolution(std::int32_t display, std::int32_t index) noexcept {
    const S2DisplayOutput *const output = GetS2DisplayOutput(display);
    return output && static_cast<std::uint32_t>(index) < output->modeCount ? &output->modes[index] : nullptr;
}

std::int32_t GetS2PrimaryDisplayIndex() noexcept {
    for (std::uint32_t index = 0; index < outputCount; ++index)
        if (outputs[index].primary)
            return static_cast<std::int32_t>(index);
    return 0;
}

std::int32_t GetS2DesktopResolutionIndex(std::int32_t display) noexcept {
    const S2DisplayOutput *const output = GetS2DisplayOutput(display);
    if (output)
        for (std::uint32_t index = 0; index < output->modeCount; ++index) {
            if (output->modes[index].width == static_cast<std::uint32_t>(output->right - output->left) &&
                output->modes[index].height == static_cast<std::uint32_t>(output->bottom - output->top))
                return static_cast<std::int32_t>(index);
        }
    return 0;
}

std::string_view GetS2DisplayOutputName(std::int32_t index) noexcept {
    const S2DisplayOutput *const output = GetS2DisplayOutput(index);
    return output ? std::string_view(output->name) : std::string_view{};
}

std::string_view GetS2DisplayResolutionName(std::int32_t display, std::int32_t index) noexcept {
    const d3t::DisplayModeInfo *const mode = GetS2DisplayResolution(display, index);
    return mode ? std::string_view(mode->description) : std::string_view{};
}
}
