#pragma once

#include "scn3/native/data.hpp"

#include <string>

namespace shenmue::scenario {

struct NativeModeCondition {
    std::uint32_t byteOffset;
    std::uint32_t value;
    bool negate = false;
};

struct NativeArgumentType {
    const char *handler;
    std::uint32_t byteOffset;
    const char *type;
    bool pointer;
    std::span<const NativeModeCondition> guards;
};

struct NativeParameterRead {
    std::uint32_t byteOffset;
    std::span<const NativeModeCondition> guards;
    std::uint32_t byteSize = sizeof(std::uint32_t);
};

struct NativePointerParameter {
    std::uint32_t byteOffset;
    const char *type;
    bool readOnly;
};

struct NativeArgumentLayout {
    const char *command;
    std::span<const NativeParameterRead> reads;
    std::int32_t callbackOffset;
    std::int32_t countOffset;
    std::int32_t tailOffset;
    std::span<const NativePointerParameter> pointers{};
    std::uint32_t parameterBytes = 0;
    std::span<const std::uint32_t> floatArguments{};
    std::span<const NativeModeCondition> guards{};
    std::int32_t variadicOffset = -1;
};

struct NativePrototype {
    std::string_view command;
    std::string_view result;
    std::string_view parameters;
};

const NativePrototype *nativePrototype(std::string_view handler);

std::span<const std::string> nativeParameterTypes(std::string_view handler);
const NativeArgumentLayout *nativeArgumentLayout(std::string_view handler, std::span<const std::optional<std::uint32_t>> arguments = {});
bool nativeCallHasModes(std::string_view handler);
std::span<const NativeArgumentLayout> nativeArgumentLayouts();
bool nativeCallIgnoresParameters(std::string_view handler);
const NativeArgumentType *nativeArgumentForHandler(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments);

enum class NativeAccess : std::uint8_t { Read, Write, ReadWrite };

NativeAccess nativePointerAccess(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments);

std::uint32_t nativePointerReads(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments);

struct NativeParameter {
    std::string_view type;
    std::string_view name;
};
NativeParameter nativeParameter(std::string_view handler, std::size_t index, std::span<const std::optional<std::uint32_t>> arguments);

} // namespace shenmue::scenario
