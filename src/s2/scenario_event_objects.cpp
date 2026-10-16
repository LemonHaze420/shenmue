#include "s2/scenario_event_objects.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {


constexpr shen_name SceneName(char first, char second, char third, char fourth) {
    return shen_name{make_shen_name(first, second, third, fourth)};
}

constexpr ScenarioEventObjectSpatialRegion RectangleRegion(
    shen_name eventObject, shen_name map, float minimumX, float minimumZ, float maximumX, float maximumZ) {
    return {eventObject, map, ScenarioEventObjectRegionShape::Rectangle, minimumX, minimumZ, maximumX, maximumZ, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}

constexpr ScenarioEventObjectSpatialRegion CircleRegion(shen_name eventObject, shen_name map, float centerX, float centerZ, float radius) {
    return {eventObject, map, ScenarioEventObjectRegionShape::Circle, 0.0f, 0.0f, 0.0f, 0.0f, centerX, centerZ, radius, 0.0f, 0.0f};
}

constexpr ScenarioEventObjectSpatialRegion HeightRegion(shen_name eventObject, shen_name map, float minimumY, float maximumY) {
    return {eventObject, map, ScenarioEventObjectRegionShape::HeightRange, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, minimumY, maximumY};
}

constexpr std::array<ScenarioEventObjectFallback, 2> scenarioEventObjectFallbacks{{
    {SceneName('A', 'R', '1', '5'), SceneName('A', 'R', '0', '2')},
    {SceneName('W', 'E', 'G', '1'), SceneName('W', 'E', 'G', '0')},
}};

constexpr std::array<ScenarioEventObjectRelation, 103> scenarioEventObjectRelations{{
    {SceneName('A', 'K', '1', '0'), SceneName('A', 'K', 'T', '0')},
    {SceneName('A', 'K', '1', '0'), SceneName('A', 'K', 'T', '1')},
    {SceneName('A', 'K', '1', '0'), SceneName('A', 'K', 'T', '2')},
    {SceneName('A', 'K', '1', '0'), SceneName('A', 'K', 'T', '3')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', '0', '0')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', 'T', '0')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', 'T', '1')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', 'T', '2')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', 'T', '3')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', 'A', '3')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', 'Y', '0')},
    {SceneName('A', 'K', '1', '1'), SceneName('A', 'K', 'S', '0')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', '0', '0')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'T', '0')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'T', '1')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'T', '2')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'T', '3')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'S', '0')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'S', '1')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'A', '3')},
    {SceneName('A', 'K', 'K', '0'), SceneName('A', 'K', 'Y', '0')},
    {SceneName('A', 'R', 'R', '0'), SceneName('A', 'R', '0', '2')},
    {SceneName('A', 'R', 'R', '0'), SceneName('A', 'R', 'C', '0')},
    {SceneName('A', 'R', 'R', '0'), SceneName('A', 'R', 'A', '0')},
    {SceneName('A', 'R', 'R', '0'), SceneName('A', 'R', 'M', '0')},
    {SceneName('A', 'R', 'R', '0'), SceneName('A', 'R', 'S', 'F')},
    {SceneName('A', 'R', 'Q', '0'), SceneName('A', 'R', '0', '3')},
    {SceneName('A', 'R', 'Q', '0'), SceneName('A', 'R', 'Z', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', '0', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'T', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'T', '1')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'T', '2')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'T', '3')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'S', '1')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'A', '3')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'Y', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'K', 'S', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', '0', '1')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', '0', '2')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', 'C', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', 'A', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', 'M', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', 'S', 'F')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', '0', '3')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'R', 'Z', '0')},
    {SceneName('A', 'A', '1', '0'), SceneName('A', 'B', '0', '0')},
    {SceneName('A', 'S', '0', '0'), SceneName('A', 'K', 'S', '1')},
    {SceneName('A', 'S', '0', '0'), SceneName('A', 'R', 'A', '0')},
    {SceneName('A', 'R', '1', '5'), SceneName('A', 'R', 'C', '0')},
    {SceneName('A', 'R', '1', '5'), SceneName('A', 'R', 'A', '0')},
    {SceneName('A', 'R', '1', '5'), SceneName('A', 'R', 'M', '0')},
    {SceneName('A', 'R', '1', '5'), SceneName('A', 'R', 'X', '0')},
    {SceneName('A', 'R', '1', '5'), SceneName('A', 'R', 'X', '6')},
    {SceneName('A', 'R', '1', '5'), SceneName('A', 'R', 'X', '7')},
    {SceneName('W', 'S', 'S', 'S'), SceneName('W', 'S', '0', '0')},
    {SceneName('W', 'S', 'S', 'S'), SceneName('W', 'S', 'Y', '0')},
    {SceneName('W', 'K', 'K', '0'), SceneName('W', 'K', '0', '0')},
    {SceneName('W', 'K', 'K', '0'), SceneName('W', 'K', 'A', '0')},
    {SceneName('W', 'A', '1', '1'), SceneName('W', 'E', 'G', '0')},
    {SceneName('W', 'E', 'E', '0'), SceneName('W', 'E', '0', '0')},
    {SceneName('W', 'E', 'E', '0'), SceneName('W', 'E', 'S', 'M')},
    {SceneName('W', 'E', 'E', '0'), SceneName('W', 'E', 'T', '0')},
    {SceneName('W', 'E', 'E', '0'), SceneName('W', 'E', 'C', 'F')},
    {SceneName('W', 'E', 'E', '0'), SceneName('W', 'E', 'G', '0')},
    {SceneName('W', 'E', 'E', '0'), SceneName('W', 'E', 'S', '1')},
    {SceneName('W', 'E', 'E', '0'), SceneName('W', 'E', 'M', '1')},
    {SceneName('W', 'T', 'T', '0'), SceneName('W', 'T', '0', '0')},
    {SceneName('W', 'T', 'T', '0'), SceneName('W', 'T', 'A', '0')},
    {SceneName('W', 'B', 'B', '0'), SceneName('W', 'B', '0', '0')},
    {SceneName('W', 'B', 'B', '0'), SceneName('W', 'B', '0', '1')},
    {SceneName('W', 'R', 'R', '0'), SceneName('W', 'R', '0', '0')},
    {SceneName('W', 'R', 'R', '0'), SceneName('W', 'R', 'S', '2')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'S', '0', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'S', 'Y', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'E', '0', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'E', 'S', 'M')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'E', 'T', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'E', 'C', 'F')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'E', 'G', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'E', 'S', '1')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'E', 'M', '1')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'K', '0', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'K', 'A', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'R', '0', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'R', 'S', '2')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'N', '0', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'T', '0', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'T', 'A', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'B', '0', '0')},
    {SceneName('W', 'A', '1', '0'), SceneName('W', 'B', '0', '1')},
    {SceneName('Q', 'C', '1', '0'), SceneName('Q', 'C', '0', '0')},
    {SceneName('Q', 'C', '1', '0'), SceneName('Q', 'C', '0', '1')},
    {SceneName('Q', 'C', '1', '0'), SceneName('Q', 'C', '0', '6')},
    {SceneName('Q', 'C', '1', '0'), SceneName('Q', 'C', 'A', 'E')},
    {SceneName('Q', 'D', '1', '0'), SceneName('Q', 'D', '0', '0')},
    {SceneName('Q', 'D', '1', '0'), SceneName('Q', 'D', '0', '1')},
    {SceneName('Q', 'F', '1', '0'), SceneName('Q', 'F', '0', '0')},
    {SceneName('Q', 'F', '1', '0'), SceneName('Q', 'F', '0', '1')},
    {SceneName('Q', 'F', '1', '0'), SceneName('Q', 'F', '0', '2')},
    {SceneName('Q', 'F', '1', '0'), SceneName('Q', 'F', '0', '6')},
    {SceneName('Q', 'F', '1', '1'), SceneName('Q', 'F', '3', '9')},
    {SceneName('Q', 'F', '1', '1'), SceneName('Q', 'F', '4', '0')},
    {SceneName('Q', 'F', '1', '1'), SceneName('Q', 'K', 'B', 'T')},
}};

constexpr std::array<ScenarioEventObjectSpatialRegion, 106> scenarioEventObjectSpatialRegions{{
    RectangleRegion(SceneName('A', 'B', '1', '0'), SceneName('A', 'B', '0', '0'), 254.10000610351562f, 539.5f, 277.5f, 545.7999877929688f),
    RectangleRegion(SceneName('A', 'B', '2', '0'), SceneName('A', 'B', '0', '0'), 116.0f, 547.0f, 122.80000305175781f, 557.7999877929688f),
    RectangleRegion(SceneName('A', 'R', '1', '2'), SceneName('A', 'R', '0', '2'), 199.8000030517578f, 303.0f, 210.39999389648438f, 322.1000061035156f),
    RectangleRegion(SceneName('A', 'R', '1', '3'), SceneName('A', 'R', '0', '2'), 226.8000030517578f, 344.8999938964844f, 236.8000030517578f, 353.0f),
    RectangleRegion(SceneName('A', 'R', '1', '4'), SceneName('A', 'R', '0', '2'), 244.0f, 341.79998779296875f, 260.6000061035156f, 367.29998779296875f),
    RectangleRegion(SceneName('A', 'R', 'K', '0'), SceneName('A', 'R', '0', '1'), 173.0f, 302.5f, 187.0f, 313.0f),
    RectangleRegion(SceneName('A', 'R', 'X', '0'), SceneName('A', 'R', '0', '2'), 188.5f, 376.6000061035156f, 231.1999969482422f, 434.3999938964844f),
    RectangleRegion(SceneName('A', 'R', 'X', '6'), SceneName('A', 'R', '0', '2'), 255.60000610351562f, 392.5f, 264.5f, 405.8999938964844f),
    RectangleRegion(
        SceneName('A', 'R', 'X', '7'), SceneName('A', 'R', '0', '2'), 243.60000610351562f, 423.20001220703125f, 251.8000030517578f, 428.6000061035156f),
    RectangleRegion(SceneName('A', 'R', 'X', 'A'), SceneName('A', 'R', '0', '3'), 299.79998779296875f, 472.6000061035156f, 308.5f, 483.5f),
    RectangleRegion(SceneName('A', 'R', '3', '0'), SceneName('A', 'R', '0', '3'), 261.3999938964844f, 471.6000061035156f, 273.70001220703125f, 475.5f),
    RectangleRegion(SceneName('A', 'R', '3', '0'), SceneName('A', 'B', '0', '0'), 261.3999938964844f, 471.6000061035156f, 273.70001220703125f, 475.5f),
    RectangleRegion(SceneName('A', 'R', '3', '0'), SceneName('A', 'R', '0', '3'), 257.0f, 460.3999938964844f, 265.5f, 471.6000061035156f),
    RectangleRegion(SceneName('A', 'R', '3', '0'), SceneName('A', 'B', '0', '0'), 257.0f, 460.3999938964844f, 265.5f, 471.6000061035156f),
    RectangleRegion(SceneName('A', 'R', 'K', 'L'), SceneName('A', 'R', '0', '3'), 380.70001220703125f, 424.3999938964844f, 406.0f, 453.6000061035156f),
    RectangleRegion(SceneName('W', 'B', '1', '0'), SceneName('W', 'B', '0', '0'), 326.70001220703125f, 227.0f, 344.20001220703125f, 277.70001220703125f),
    RectangleRegion(SceneName('W', 'B', '1', '0'), SceneName('W', 'B', '0', '1'), 326.70001220703125f, 227.0f, 344.20001220703125f, 277.70001220703125f),
    RectangleRegion(SceneName('W', 'B', '2', '0'), SceneName('W', 'B', '0', '0'), 327.3999938964844f, 230.0f, 353.0f, 276.6000061035156f),
    RectangleRegion(SceneName('W', 'B', '2', '0'), SceneName('W', 'N', '0', '0'), 280.29998779296875f, 277.70001220703125f, 346.0f, 289.79998779296875f),
    RectangleRegion(SceneName('W', 'B', '2', '0'), SceneName('W', 'T', '0', '0'), 280.29998779296875f, 277.70001220703125f, 346.0f, 289.79998779296875f),
    CircleRegion(SceneName('W', 'N', 'N', '6'), SceneName('W', 'N', '0', '0'), 202.6999969482422f, 203.89999389648438f, 4.5f),
    RectangleRegion(SceneName('W', 'N', 'N', '7'), SceneName('W', 'N', '0', '0'), 187.5f, 220.3000030517578f, 196.5f, 225.5f),
    CircleRegion(SceneName('W', 'N', 'N', '9'), SceneName('W', 'N', '0', '0'), 223.10000610351562f, 206.5f, 3.0999999046325684f),
    CircleRegion(SceneName('W', 'N', 'N', '9'), SceneName('W', 'N', '0', '0'), 202.6999969482422f, 203.89999389648438f, 4.5f),
    RectangleRegion(SceneName('W', 'N', 'N', 'A'), SceneName('W', 'N', '0', '0'), 233.60000610351562f, 200.0f, 242.1999969482422f, 209.0f),
    RectangleRegion(SceneName('W', 'N', '2', '0'), SceneName('W', 'N', '0', '0'), 223.1999969482422f, 154.60000610351562f, 232.1999969482422f, 191.0f),
    RectangleRegion(
        SceneName('W', 'N', '2', '0'), SceneName('W', 'N', '0', '0'), 232.1999969482422f, 154.60000610351562f, 244.10000610351562f, 162.1999969482422f),
    RectangleRegion(SceneName('W', 'N', '2', '0'), SceneName('W', 'N', '0', '0'), 237.0f, 138.5f, 244.10000610351562f, 154.60000610351562f),
    RectangleRegion(
        SceneName('W', 'R', '1', '0'), SceneName('W', 'R', '0', '0'), 180.60000610351562f, 183.60000610351562f, 185.60000610351562f, 194.39999389648438f),
    RectangleRegion(SceneName('W', 'S', 'C', '0'), SceneName('W', 'S', '0', '0'), 56.79999923706055f, 180.0f, 63.20000076293945f, 186.39999389648438f),
    CircleRegion(SceneName('W', 'S', 'C', '1'), SceneName('W', 'S', '0', '0'), 27.299999237060547f, 186.89999389648438f, 16.0f),
    RectangleRegion(SceneName('W', 'S', 'K', 'B'), SceneName('W', 'S', '0', '0'), 99.0f, 186.0f, 126.0f, 222.0f),
    RectangleRegion(SceneName('W', 'S', 'K', 'B'), SceneName('W', 'S', '0', '0'), 98.0f, 202.39999389648438f, 101.0f, 206.5f),
    RectangleRegion(SceneName('W', 'T', 'X', '0'), SceneName('W', 'T', '0', '0'), 231.5f, 337.5f, 261.79998779296875f, 356.79998779296875f),
    RectangleRegion(SceneName('W', 'N', '1', '0'), SceneName('W', 'B', '0', '0'), 262.0f, 250.0f, 272.0f, 276.5f),
    RectangleRegion(SceneName('W', 'N', '1', '0'), SceneName('W', 'T', '0', '0'), 262.0f, 250.0f, 272.0f, 276.5f),
    RectangleRegion(SceneName('W', 'T', '1', '0'), SceneName('W', 'B', '0', '0'), 260.0f, 276.3999938964844f, 285.0f, 290.0f),
    RectangleRegion(SceneName('W', 'T', '1', '0'), SceneName('W', 'N', '0', '0'), 260.0f, 276.3999938964844f, 285.0f, 290.0f),
    RectangleRegion(SceneName('W', 'E', 'W', 'E'), SceneName('W', 'E', '0', '0'), 53.0f, 93.0f, 130.5f, 133.0f),
    RectangleRegion(SceneName('W', 'E', 'E', 'A'), SceneName('W', 'E', '0', '0'), 130.5f, 73.0f, 244.5f, 133.0f),
    HeightRegion(SceneName('W', 'E', 'G', '3'), SceneName('W', 'E', 'G', '0'), 107.0f, 200.0f),
    HeightRegion(SceneName('W', 'E', 'G', '2'), SceneName('W', 'E', 'G', '0'), 103.9000015258789f, 200.0f),
    RectangleRegion(SceneName('Q', 'T', 'B', 'B'), SceneName('Q', '1', '0', '0'), 219.5f, 204.0f, 253.0f, 221.0f),
    RectangleRegion(
        SceneName('Q', '1', '3', '0'), SceneName('Q', '1', '0', '0'), 172.6999969482422f, 187.8000030517578f, 177.6999969482422f, 194.3000030517578f),
    RectangleRegion(
        SceneName('Q', '1', '4', '0'), SceneName('Q', '1', '0', '0'), 183.1999969482422f, 78.5999984741211f, 198.89999389648438f, 108.0999984741211f),
    RectangleRegion(SceneName('Q', '1', '1', '0'), SceneName('Q', '1', '0', '0'), 168.1999969482422f, 73.19999694824219f, 259.0f, 154.60000610351562f),
    RectangleRegion(SceneName('Q', '1', '2', '0'), SceneName('Q', '1', '0', '0'), 168.1999969482422f, 154.60000610351562f, 259.0f, 222.5f),
    RectangleRegion(SceneName('Q', '2', '1', '0'), SceneName('Q', '2', '0', '0'), 216.89999389648438f, 61.70000076293945f, 299.0f, 116.80000305175781f),
    RectangleRegion(SceneName('Q', '2', '2', '0'), SceneName('Q', '2', '0', '0'), 251.10000610351562f, 116.9000015258789f, 293.0f, 156.0f),
    RectangleRegion(SceneName('Q', 'T', 'B', 'B'), SceneName('Q', '3', '0', '0'), 219.5f, 204.0f, 253.0f, 221.0f),
    RectangleRegion(
        SceneName('Q', 'A', 'A', 'Q'), SceneName('Q', '3', '0', '0'), 245.8000030517578f, 194.3000030517578f, 256.3999938964844f, 201.60000610351562f),
    RectangleRegion(SceneName('Q', 'A', 'A', 'R'), SceneName('Q', '3', '0', '0'), 256.29998779296875f, 192.1999969482422f, 267.0f, 201.5f),
    RectangleRegion(SceneName('Q', '3', '3', '0'), SceneName('Q', '3', '0', '0'), 258.20001220703125f, 158.3000030517578f, 265.0f, 165.60000610351562f),
    RectangleRegion(SceneName('Q', '3', '2', '0'), SceneName('Q', '3', '0', '0'), 210.0f, 163.8000030517578f, 256.5f, 208.89999389648438f),
    RectangleRegion(SceneName('Q', '3', '1', '0'), SceneName('Q', '3', '0', '0'), 253.5f, 142.10000610351562f, 296.70001220703125f, 208.89999389648438f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', '0', '0'), 245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', 'E', '1'), 245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', 'E', '6'), 245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', '1', '1'), 245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', '0', '0'), 256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', 'E', '1'), 256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', 'E', '6'), 256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '2'), SceneName('Q', 'A', '1', '1'), 256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', '0', '0'), 215.0f, 195.1999969482422f, 235.0f, 216.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', 'E', '1'), 215.0f, 195.1999969482422f, 235.0f, 216.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', 'E', '6'), 215.0f, 195.1999969482422f, 235.0f, 216.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', '1', '1'), 215.0f, 195.1999969482422f, 235.0f, 216.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', '0', '0'), 213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', 'E', '1'), 213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', 'E', '6'), 213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f),
    RectangleRegion(SceneName('Q', 'A', '1', '3'), SceneName('Q', 'A', '1', '1'), 213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f),
    RectangleRegion(SceneName('Q', 'A', '1', '0'), SceneName('Q', 'A', '0', '0'), 274.3999938964844f, 123.0f, 303.5f, 167.89999389648438f),
    RectangleRegion(SceneName('Q', 'A', '1', '0'), SceneName('Q', 'A', 'W', '1'), 274.3999938964844f, 123.0f, 303.5f, 167.89999389648438f),
    RectangleRegion(SceneName('Q', 'A', '1', '0'), SceneName('Q', 'A', 'W', '6'), 274.3999938964844f, 123.0f, 303.5f, 167.89999389648438f),
    RectangleRegion(
        SceneName('Q', 'A', '1', '4'), SceneName('Q', 'A', '0', '0'), 283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f),
    RectangleRegion(
        SceneName('Q', 'A', '1', '4'), SceneName('Q', 'A', 'W', '1'), 283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f),
    RectangleRegion(
        SceneName('Q', 'A', '1', '4'), SceneName('Q', 'A', 'W', '6'), 283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f),
    RectangleRegion(
        SceneName('Q', 'A', '1', '4'), SceneName('Q', 'A', '2', '2'), 283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f),
    CircleRegion(SceneName('Q', 'B', '1', '0'), SceneName('Q', 'B', '0', '0'), 204.89999389648438f, 191.60000610351562f, 1.7999999523162842f),
    CircleRegion(SceneName('Q', 'B', '1', '1'), SceneName('Q', 'B', '0', '0'), 208.6999969482422f, 195.5f, 1.7000000476837158f),
    RectangleRegion(SceneName('Q', 'C', 'F', 'E'), SceneName('Q', 'C', '0', '6'), 166.5f, 147.8000030517578f, 180.8000030517578f, 177.39999389648438f),
    RectangleRegion(SceneName('Q', 'D', '2', '0'), SceneName('Q', 'D', '0', '0'), 190.0f, 65.5999984741211f, 220.5f, 90.5999984741211f),
    RectangleRegion(SceneName('Q', 'D', '2', '0'), SceneName('Q', 'D', '0', '1'), 190.0f, 65.5999984741211f, 220.5f, 90.5999984741211f),
    RectangleRegion(SceneName('Q', 'E', 'F', 'O'), SceneName('Q', 'E', '0', '6'), 278.29998779296875f, 61.0f, 281.79998779296875f, 66.5f),
    RectangleRegion(SceneName('Q', 'E', 'F', 'P'), SceneName('Q', 'E', '0', '6'), 283.5f, 59.5f, 288.79998779296875f, 66.5f),
    RectangleRegion(SceneName('Q', 'E', 'F', 'Q'), SceneName('Q', 'E', '0', '6'), 289.0f, 61.29999923706055f, 292.70001220703125f, 66.5f),
    RectangleRegion(SceneName('Q', 'E', 'F', 'R'), SceneName('Q', 'E', '0', '6'), 294.3999938964844f, 59.5f, 299.6000061035156f, 66.5f),
    RectangleRegion(SceneName('Q', 'E', 'F', 'S'), SceneName('Q', 'E', '0', '6'), 299.8999938964844f, 61.099998474121094f, 303.5f, 66.5f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'E', '0', '0'), 234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'E', '0', '1'), 234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'E', '0', '6'), 234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'L', 'B', 'T'), 234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'E', '0', '0'), 220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'E', '0', '1'), 220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'E', '0', '6'), 220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f),
    RectangleRegion(SceneName('Q', 'E', '1', '0'), SceneName('Q', 'L', 'B', 'T'), 220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f),
    RectangleRegion(
        SceneName('Q', 'E', '1', '1'), SceneName('Q', 'E', '0', '0'), 238.8000030517578f, 57.599998474121094f, 267.1000061035156f, 79.0999984741211f),
    RectangleRegion(
        SceneName('Q', 'E', '1', '1'), SceneName('Q', 'E', '0', '1'), 238.8000030517578f, 57.599998474121094f, 267.1000061035156f, 79.0999984741211f),
    RectangleRegion(
        SceneName('Q', 'E', '1', '1'), SceneName('Q', 'E', '0', '6'), 238.8000030517578f, 57.599998474121094f, 267.1000061035156f, 79.0999984741211f),
    RectangleRegion(SceneName('Q', 'E', '1', '2'), SceneName('Q', 'E', '0', '0'), 267.1000061035156f, 58.0f, 309.20001220703125f, 110.0f),
    RectangleRegion(SceneName('Q', 'E', '1', '2'), SceneName('Q', 'E', '0', '1'), 267.1000061035156f, 58.0f, 309.20001220703125f, 110.0f),
    RectangleRegion(SceneName('Q', 'E', '1', '2'), SceneName('Q', 'E', '0', '6'), 267.1000061035156f, 58.0f, 309.20001220703125f, 110.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '5'), SceneName('Q', 'A', '0', '0'), 256.29998779296875f, 216.5f, 283.29998779296875f, 244.0f),
    RectangleRegion(SceneName('Q', 'A', '1', '5'), SceneName('Q', 'J', 'B', 'T'), 256.29998779296875f, 216.5f, 283.29998779296875f, 244.0f),
    RectangleRegion(SceneName('Q', 'F', '1', '2'), SceneName('Q', 'U', 'G', '0'), 246.60000610351562f, 116.5f, 254.5f, 129.10000610351562f),
    RectangleRegion(SceneName('K', 'E', 'S', 'S'), SceneName('K', 'E', 'S', '1'), 60.0f, -18.0f, 142.5f, 39.0f),
}};
bool ContainsPlayerSearchLocation(const ScenarioEventObjectSpatialRegion &region, const Vector3 &location) {
    switch (region.shape) {
    case ScenarioEventObjectRegionShape::Rectangle:
        return location.x >= region.minimumX && region.maximumX >= location.x && location.z >= region.minimumZ && region.maximumZ >= location.z;
    case ScenarioEventObjectRegionShape::Circle: {
        const float deltaZ = location.z - region.centerZ;
        const float deltaX = location.x - region.centerX;
        return region.radius > std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
    }
    case ScenarioEventObjectRegionShape::HeightRange:
        return location.y >= region.minimumY && region.maximumY >= location.y;
    }
    return false;
}

shen_name ResolveScenarioEventObject(const Vector3 &playerSearchLocation) {
    for (const ScenarioEventObjectSpatialRegion &region : scenarioEventObjectSpatialRegions) {
        if (region.map.name == g_currentMap.name && ContainsPlayerSearchLocation(region, playerSearchLocation)) {
            return region.eventObject;
        }
    }
    for (const ScenarioEventObjectFallback &fallback : scenarioEventObjectFallbacks) {
        if (fallback.map.name == g_currentMap.name)
            return fallback.eventObject;
    }
    return {};
}

void RebuildScenarioEventObjectMapNames() {
    Vector3 playerSearchLocation{};
    plymUpdatePlayerSearchLocation(&playerSearchLocation);

    std::array<shen_name, 6> resolvedNames{};
    const shen_name spatialName = ResolveScenarioEventObject(playerSearchLocation);
    resolvedNames[0] = spatialName;
    std::size_t resolvedCount = spatialName.name != 0 ? 1 : 0;

    for (const ScenarioEventObjectRelation &relation : scenarioEventObjectRelations) {
        if (relation.currentName.name != g_currentMap.name && relation.currentName.name != spatialName.name) {
            continue;
        }
        if (resolvedCount < resolvedNames.size())
            resolvedNames[resolvedCount++] = relation.eventObject;
    }
    PublishScenarioEventObjectMapNames(resolvedNames);
}
} // namespace event_runtime_detail


void PublishScenarioEventObjectMapNames(std::span<const shen_name> names) {
    scenarioEventObjectMapNames.fill({});
    const std::size_t count = std::min(names.size(), scenarioEventObjectMapNames.size());
    std::copy_n(names.begin(), count, scenarioEventObjectMapNames.begin());
}

std::span<const shen_name> GetScenarioEventObjectMapNames() {
    return scenarioEventObjectMapNames;
}

void ResetScenarioEventObjectMapNames() {
    scenarioEventObjectMapNames.fill({});
}

} // namespace shenmue::s2
