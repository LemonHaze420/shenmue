#include "scn3/bytecode/format.hpp"
#include "scn3/frontend/source.hpp"

namespace shenmue::scenario {

std::optional<uint32_t> vectorComponentOffsetFromSuffix(const std::string &suffix) {
    if (suffix.size() != 1) {
        return std::nullopt;
    }
    switch (static_cast<char>(std::toupper(static_cast<unsigned char>(suffix[0])))) {
    case 'X':
        return 0u;
    case 'Y':
        return 4u;
    case 'Z':
        return 8u;
    case 'W':
        return 12u;
    default:
        return std::nullopt;
    }
}

static std::optional<SourceSlotRef> slotRefFromName(std::string_view name, std::string_view prefix, bool versions) {
    const auto hexEnd = [&](size_t from) {
        while (from < name.size() && ((name[from] >= '0' && name[from] <= '9') || (name[from] >= 'A' && name[from] <= 'F')))
            ++from;
        return from;
    };
    if (!name.starts_with(prefix))
        return std::nullopt;
    size_t cursor = prefix.size(), end = hexEnd(cursor);
    if (end == cursor)
        return std::nullopt;
    const uint32_t base = static_cast<uint32_t>(std::stoul(std::string(name.substr(cursor, end - cursor)), nullptr, 16));
    cursor = end;
    if (versions && cursor < name.size() && name[cursor] == 'v') {
        end = cursor + 1;
        while (end < name.size() && name[end] >= '0' && name[end] <= '9')
            ++end;
        if (end == cursor + 1)
            return std::nullopt;
        cursor = end;
    }
    std::string_view field = name.substr(cursor);
    if (field.ends_with(".u8"))
        field.remove_suffix(3);
    else if (field.ends_with(".u16"))
        field.remove_suffix(4);
    if (field.empty())
        return SourceSlotRef{base, base, false};
    if (field.size() == 2 && field[0] == '.') {
        const std::optional<uint32_t> delta = vectorComponentOffsetFromSuffix(std::string(1, field[1]));
        if (!delta)
            return std::nullopt;
        return SourceSlotRef{base, base + *delta, true};
    }
    if (field[0] != '_' || field.size() == 1)
        return std::nullopt;
    for (size_t index = 1; index < field.size(); ++index)
        if (!((field[index] >= '0' && field[index] <= '9') || (field[index] >= 'A' && field[index] <= 'F')))
            return std::nullopt;
    const uint32_t delta = static_cast<uint32_t>(std::stoul(std::string(field.substr(1)), nullptr, 16));
    return SourceSlotRef{base, base + delta, true};
}

std::optional<SourceSlotRef> localSlotRefFromName(const std::string &name) {
    return slotRefFromName(name, "local_", true);
}

std::optional<SourceSlotRef> taskSlotRefFromName(const std::string &name) {
    return slotRefFromName(name, "task_", false);
}

std::optional<uint32_t> argSlotOffsetFromName(const std::string &name) {
    if (name.size() <= 3 || !name.starts_with("arg"))
        return std::nullopt;
    for (size_t index = 3; index < name.size(); ++index)
        if (name[index] < '0' || name[index] > '9')
            return std::nullopt;
    return 8u + static_cast<uint32_t>(std::stoul(name.substr(3), nullptr, 10) * 4u);
}

bool sourceIdentifierWantsU16(const std::string &name) {
    return name.size() > 4 && name.substr(name.size() - 4) == ".u16";
}

uint8_t sourceIdentifierAccessWidth(const std::string &name) {
    if (name.size() > 3 && name.substr(name.size() - 3) == ".u8") {
        return 1;
    }
    if (sourceIdentifierWantsU16(name)) {
        return 2;
    }
    return 4;
}

std::string stripSourceWidthSuffix(std::string name) {
    if (name.size() > 3 && name.substr(name.size() - 3) == ".u8") {
        name.resize(name.size() - 3);
    } else if (sourceIdentifierWantsU16(name)) {
        name.resize(name.size() - 4);
    }
    return name;
}

uint8_t sourceImmWidthFromName(const std::string &name) {
    if (name == "imm8" || name == "i8")
        return 1;
    if (name == "imm16" || name == "i16")
        return 2;
    if (name == "imm32" || name == "i32")
        return 3;
    if (name == "imm0")
        return 0;
    throw std::runtime_error("Expected imm8/imm16/imm32, got " + name);
}

std::optional<uint8_t> sourceImmediateWidthCallName(const std::string &name) {
    if (name == "i8")
        return 1;
    if (name == "i16")
        return 2;
    if (name == "i32")
        return 3;
    return std::nullopt;
}

} // namespace shenmue::scenario
