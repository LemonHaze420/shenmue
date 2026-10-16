#pragma once

#include "s2/legacy_draw.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace shenmue::s2 {

struct MT7Model;
struct MT7Node;

inline constexpr double MAP_SEAM_MATCH_ULPS = 2.0;
inline constexpr double MAP_SEAM_LOCAL_EDGE_ULPS = 8.0;
inline constexpr double MAP_SEAM_GUARD_ULPS = 4.0;

struct MapSeamEdge {
    std::uint32_t first;
    std::uint32_t second;
    std::uint32_t uses;
    std::uint8_t axis;
    std::int8_t side;
};

struct MapSeamPoint {
    Vector3 original;
    std::array<double, 3> corrected;
    std::uint8_t guardedAxes{};
};

struct MapSeamTile {
    std::size_t nodeIndex;
    std::uint32_t meshOffset;
    std::uint32_t metadata;
    std::uint32_t stride;
    std::int32_t bufferSize;
    std::uint8_t planeAxis;
    Vector3 translation;
    std::vector<MapSeamPoint> points;
    std::vector<MapSeamEdge> edges;
};

struct MapSeamNodeResources {
    std::size_t nodeIndex;
    std::uint32_t meshOffset;
    Vector3 translation;
    std::int32_t ownedVertexBuffer{};
    std::int32_t sourceIndexBuffer;
    std::uint32_t stride;
    std::int32_t bufferSize;
    std::vector<Vector3> originalPositions;
    std::vector<Vector3> correctedPositions;
    LegacyDrawResources resources;
};

struct MapSeamModelResources {
    const MT7Node *nodes;
    std::size_t nodeCount;
    Vector3 rootPosition;
    Vector3i rootRotation;
    Vector3 rootScale;
    std::size_t matchedEdges{};
    float maximumWorldMovement{};
    std::vector<MapSeamNodeResources> correctedNodes;
};

void PrepareMapSeamResources(MT7Model &model);
void ReleaseMapSeamResources(const MT7Model &model);
const LegacyDrawResources &GetCorrectedMapSeamResources(const MT7Model &model, const MT7Node &node, const LegacyDrawResources &source);
const MapSeamModelResources *GetMapSeamResources(const MT7Model &model);

} // namespace shenmue::s2
