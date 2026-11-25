#include "s2/weather_table.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/weather_data_dc.hpp"
#include "s2/weather_air_dc.hpp"
#include <cmath>
#include <cstring>

namespace shenmue {
namespace s2 {

std::int32_t InterpolateWeatherInteger(std::int32_t first, std::int32_t second, float blend);

// @verified - d
float InterpolateWeatherValue(float first, float second, float blend) {
    float difference = second - first;
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        difference = 0.0f;
    } else {
        difference *= blend;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            difference = 0.0f;
    }
    return difference + first;
}

// @verified - d
std::uint32_t InterpolateWeatherColour(std::int32_t first, std::int32_t second, float blend) {
    std::int32_t initial, alpha, red, green, blue;
    float difference;
    initial = (first >> 24) & 0xff;
    difference = static_cast<float>(((second >> 24) & 0xff) - initial);
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        difference = 0.0f;
    } else {
        difference *= blend;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            difference = 0.0f;
    }
    alpha = static_cast<std::int32_t>(difference) + initial;
    initial = (first >> 16) & 0xff;
    difference = static_cast<float>(((second >> 16) & 0xff) - initial);
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        difference = 0.0f;
    } else {
        difference *= blend;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            difference = 0.0f;
    }
    red = static_cast<std::int32_t>(difference) + initial;
    initial = (first >> 8) & 0xff;
    difference = static_cast<float>(((second >> 8) & 0xff) - initial);
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        difference = 0.0f;
    } else {
        difference *= blend;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            difference = 0.0f;
    }
    green = static_cast<std::int32_t>(difference) + initial;
    initial = first & 0xff;
    difference = static_cast<float>((second & 0xff) - initial);
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        difference = 0.0f;
    } else {
        difference *= blend;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            difference = 0.0f;
    }
    blue = static_cast<std::int32_t>(difference) + initial;
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

// @verified - d
std::uint32_t AddWeatherColours(std::uint32_t first, std::uint32_t second) {
    std::uint32_t alpha = (first >> 24) + (second >> 24);
    std::uint32_t red = ((first >> 16) & 0xffU) + ((second >> 16) & 0xffU);
    std::uint32_t green = ((first >> 8) & 0xffU) + ((second >> 8) & 0xffU);
    std::uint32_t blue = (first & 0xffU) + (second & 0xffU);
    if (alpha > 255)
        alpha = 255;
    if (red > 255)
        red = 255;
    if (green > 255)
        green = 255;
    if (blue > 255)
        blue = 255;
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

// @verified - d
void ClearWeatherTableSample(WeatherTableSample *sample) {
    sample->seconds = 0;
    ClearWeatherSamplePrimary(&sample->primary);
    ClearWeatherSampleEnvironment(&sample->environment);
}

// @verified - d
void AccumulateWeatherSample(WeatherTableSample *output, const WeatherTableSample *sample, float weight) {
    float seconds = static_cast<float>(sample->seconds) * weight;
    if (std::fabs(seconds) < 1.1920928955078125e-07f)
        seconds = 0.0f;
    output->seconds += static_cast<std::int32_t>(seconds);
    AccumulateWeatherSamplePrimary(&output->primary, &sample->primary, weight);
    AccumulateWeatherSampleEnvironment(&output->environment, &sample->environment, weight);
}

// @verified - d
void InterpolateWeatherSample(const WeatherTableSample *first, const WeatherTableSample *second, float blend, WeatherTableSample *output) {
    output->seconds = InterpolateWeatherInteger(first->seconds, second->seconds, blend);
    InterpolateWeatherSamplePrimary(&first->primary, &second->primary, blend, &output->primary);
    InterpolateWeatherSampleEnvironment(&first->environment, &second->environment, blend, &output->environment);
}

// @verified - d
void SampleWeatherTable(const WeatherTable *table, std::int32_t seconds, WeatherTableSample *output) {
    std::int32_t start = 0, end = 0;
    const WeatherTableSample *current;
    if (table->count == 1) {
        current = reinterpret_cast<const WeatherTableSample *>(table->rows);
        ClearWeatherTableSample(output);
        AccumulateWeatherSample(output, current, 1.0f);
        output->seconds = start;
        return;
    }
    current = reinterpret_cast<const WeatherTableSample *>(table->rows);
    const WeatherTableSample *next = current + 1;
    std::int32_t index = 1;
    for (; index < table->count; ++index) {
        start = current->seconds;
        end = next->seconds;
        if (start <= seconds && seconds < end)
            break;
        ++current;
        ++next;
    }
    if (index == table->count) {
        next = reinterpret_cast<const WeatherTableSample *>(table->rows);
        end = 86400;
    }
    InterpolateWeatherSample(current, next, static_cast<float>(seconds - start) / static_cast<float>(end - start), output);
    output->seconds = seconds;
}

// @verified - d
void EvaluateWeatherTable(
    const WeatherTable *tables, std::int32_t count, std::int32_t seconds, float cloud, float rain, float snow, WeatherTableSample *output) {
    cloud *= 4.0f;
    if (cloud > 1.0f)
        cloud = 1.0f;
    rain *= 3.0f;
    if (rain > 1.0f)
        rain = 1.0f;
    snow *= 3.0f;
    if (snow > 1.0f)
        snow = 1.0f;
    float clear, sum, weight;
    float weights[4];
    float maximum = cloud;
    if (rain > maximum)
        maximum = rain;
    if (snow > maximum)
        maximum = snow;
    clear = 1.0f - maximum;
    sum = ((clear + cloud) + rain) + snow;
    weight = clear / sum;
    if (std::fabs(weight) < 1.1920928955078125e-07f)
        weight = 0.0f;
    weights[0] = weight;
    weight = cloud / sum;
    if (std::fabs(weight) < 1.1920928955078125e-07f)
        weight = 0.0f;
    weights[1] = weight;
    weight = rain / sum;
    if (std::fabs(weight) < 1.1920928955078125e-07f)
        weight = 0.0f;
    weights[2] = weight;
    weight = snow / sum;
    if (std::fabs(weight) < 1.1920928955078125e-07f)
        weight = 0.0f;
    weights[3] = weight;
    ClearWeatherTableSample(output);
    for (std::int32_t index = 0; index < count; ++index) {
        if (weights[index] > 0.0f) {
            WeatherTableSample sample;
            SampleWeatherTable(tables + index, seconds, &sample);
            AccumulateWeatherSample(output, &sample, weights[index]);
        }
    }
}

} // namespace s2
} // namespace shenmue

#endif
