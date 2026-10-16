#include "s1/task/scen_state.hpp"

namespace shenmue::s1::scen_detail {

FormattedScenarioString FormatScenarioString(std::uint32_t formatOffset, const std::uint32_t *arguments) {
    FormattedScenarioString result;
    const char *const format = ScenarioPointer<const char>(formatOffset);
    if (!format || !arguments)
        return result;

    std::size_t outputLength = 0;
    std::size_t argumentIndex = 0;
    const auto append = [&](const char *text) {
        while (*text && outputLength + 1 < result.message.size())
            result.message[outputLength++] = *text++;
        result.message[outputLength] = '\0';
    };

    for (std::size_t index = 0; format[index];) {
        if (format[index] != '%') {
            if (outputLength + 1 < result.message.size())
                result.message[outputLength++] = format[index];
            result.message[outputLength] = '\0';
            ++index;
            continue;
        }

        ++index;
        std::int32_t width = -1;
        std::int32_t precision = -1;
        bool parsingPrecision = false;
        while (format[index]) {
            const unsigned char value = static_cast<unsigned char>(format[index]);
            if (std::isdigit(value)) {
                if (parsingPrecision)
                    precision = static_cast<std::int32_t>(value - '0');
                else
                    width = static_cast<std::int32_t>(value - '0');
                ++index;
                continue;
            }
            if (format[index] == '.') {
                parsingPrecision = true;
                ++index;
                continue;
            }
            break;
        }

        const char type = format[index++];
        const std::uint32_t raw = arguments[argumentIndex++];
        std::array<char, 16> part{};
        std::array<char, 16> spec{};
        const char *substitution = part.data();
        switch (type) {
        case 'c':
            std::snprintf(part.data(), part.size(), "%c", static_cast<std::int32_t>(raw));
            break;
        case 'd':
            if (width < 0 && precision < 0)
                std::snprintf(spec.data(), spec.size(), "%%d");
            else if (precision < 0)
                std::snprintf(spec.data(), spec.size(), "%%%dd", width);
            else if (width < 0)
                std::snprintf(spec.data(), spec.size(), "%%.%dd", precision);
            else
                std::snprintf(spec.data(), spec.size(), "%%%d.%dd", width, precision);
            std::snprintf(part.data(), part.size(), spec.data(), static_cast<std::int32_t>(raw));
            break;
        case 'f':
            if (width < 0 && precision < 0)
                std::snprintf(spec.data(), spec.size(), "%%f");
            else if (precision < 0)
                std::snprintf(spec.data(), spec.size(), "%%%df", width);
            else if (width < 0)
                std::snprintf(spec.data(), spec.size(), "%%.%df", precision);
            else
                std::snprintf(spec.data(), spec.size(), "%%%d.%df", width, precision);
            std::snprintf(part.data(), part.size(), spec.data(), static_cast<double>(std::bit_cast<float>(raw)));
            break;
        case 'l':
            std::snprintf(part.data(), part.size(), "%d", static_cast<std::int32_t>(raw));
            break;
        case 's':
            substitution = ScenarioPointer<const char>(raw);
            if (!substitution)
                return result;
            break;
        case 'x':
            std::snprintf(part.data(), part.size(), "%x", raw);
            break;
        default:
            return result;
        }

        const std::size_t substitutionLength = std::strlen(substitution);
        result.substitutionLength += static_cast<std::uint32_t>(substitutionLength);
        append(substitution);
    }
    return result;
}

void ConvertScenarioMessageFromShiftJis(std::array<char, 256> &message) {
    for (std::size_t index = 0; index < message.size() && message[index]; ++index) {
        const std::uint8_t lead = static_cast<std::uint8_t>(message[index]);
        if (!((lead >= 0x81U && lead <= 0x9fU) || (lead >= 0xe0U && lead <= 0xfcU)) || index + 1 >= message.size()) {
            continue;
        }
        std::uint8_t trail = static_cast<std::uint8_t>(message[index + 1]);
        std::uint8_t convertedLead = static_cast<std::uint8_t>(2U * static_cast<std::uint8_t>(lead < 0xe0U ? lead : lead - 0x40U) + 31U);
        if (trail >= 0x80U)
            --trail;
        std::uint8_t convertedTrail = static_cast<std::uint8_t>(trail - 31U);
        if (convertedTrail >= 127U) {
            convertedTrail = static_cast<std::uint8_t>(convertedTrail - 94U);
            ++convertedLead;
        }
        message[index] = static_cast<char>(convertedLead | 0x80U);
        message[++index] = static_cast<char>(convertedTrail | 0x80U);
    }
}

std::uint32_t ParseControlPointCodeDigits(std::uint32_t code) {
    std::array<std::uint8_t, 4> bytes{};
    std::memcpy(bytes.data(), &code, sizeof(code));
    if (!std::isdigit(static_cast<unsigned char>(bytes[3])))
        return 0xffffffffU;
    constexpr std::array<std::uint16_t, 4> weights{1000, 100, 10, 1};
    std::uint32_t value = 0;
    for (std::int32_t index = 3; index >= 0; --index) {
        const auto digit = static_cast<unsigned char>(bytes[static_cast<std::size_t>(index)]);
        if (std::isdigit(digit))
            value += static_cast<std::uint32_t>(digit - '0') * weights[static_cast<std::size_t>(index)];
    }
    return value;
}

} // namespace shenmue::s1::scen_detail
