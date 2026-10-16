#include "s2/enhancements/map_seams.hpp"

#include "d3t/buffer.hpp"
#include "s2/mt7.hpp"

#include <cmath>
#include <cstring>
#include <limits>
#include <memory>

namespace shenmue::s2 {
namespace {

std::vector<std::unique_ptr<MapSeamModelResources>> mapSeamModels;

bool SameVector(const Vector3 &a, const Vector3 &b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool TranslationOnly(const MT7Node &node) {
    return std::isfinite(node.position.x) && std::isfinite(node.position.y) && std::isfinite(node.position.z) && node.rotation.x == 0 && node.rotation.y == 0 &&
        node.rotation.z == 0 && node.scale.x == 1.0f && node.scale.y == 1.0f && node.scale.z == 1.0f && !node.transformOverride && !node.commandOverride &&
        node.mdcVertexLinkFlags == 0 && node.mdcWeightedVertices.empty();
}

MapSeamModelResources *FindResources(const MT7Model &model) {
    for (const std::unique_ptr<MapSeamModelResources> &resources : mapSeamModels)
        if (resources->nodes == model.nodes.data() && resources->nodeCount == model.nodes.size())
            return resources.get();
    return nullptr;
}

double CoordinateUlp(double a, double b) {
    const float magnitude = static_cast<float>(std::fmax(std::abs(a), std::abs(b)));
    return static_cast<double>(std::nextafter(magnitude, std::numeric_limits<float>::infinity())) - magnitude;
}

double WorldCoordinate(const MapSeamTile &tile, std::uint32_t vertex, unsigned axis) {
    return static_cast<double>(tile.points[vertex].original[axis]) + tile.translation[axis];
}

bool NearEndpoints(const MapSeamTile &a, std::uint32_t ai, const MapSeamTile &b, std::uint32_t bi) {
    for (unsigned axis = 0; axis < 3; ++axis) {
        const double av = WorldCoordinate(a, ai, axis), bv = WorldCoordinate(b, bi, axis);
        if (std::abs(av - bv) > MAP_SEAM_MATCH_ULPS * CoordinateUlp(av, bv))
            return false;
    }
    return true;
}

void AddTriangleEdge(MapSeamTile &tile, std::uint32_t first, std::uint32_t second) {
    if (first > second) {
        const std::uint32_t swap = first;
        first = second;
        second = swap;
    }
    for (MapSeamEdge &edge : tile.edges) {
        if (edge.first == first && edge.second == second) {
            ++edge.uses;
            return;
        }
    }
    tile.edges.push_back({first, second, 1, 0, 0});
}

bool OpaqueMesh(const MT7Model &model, std::size_t nodeIndex, const MT7Node &owner) {
    std::size_t ownerIndex = nodeIndex;
    for (std::size_t index = 0; index < model.nodes.size(); ++index)
        if (&model.nodes[index] == &owner)
            ownerIndex = index;
    for (const MT7Surface &surface : model.surfaces)
        if ((surface.ownerNode == nodeIndex || surface.ownerNode == ownerIndex) && surface.transparent)
            return false;
    const std::size_t mesh = model.nodes[nodeIndex].xb01Offset;
    if (mesh > model.source.size() || model.source.size() - mesh < 28)
        return false;
    std::uint32_t firstPacket;
    std::memcpy(&firstPacket, model.source.data() + mesh + 24, 4);
    std::size_t packet = mesh + (static_cast<std::size_t>(firstPacket) + 6) * 4;
    for (unsigned guard = 0; guard < 65536 && packet <= model.source.size() && model.source.size() - packet >= 4; ++guard) {
        std::uint32_t header;
        std::memcpy(&header, model.source.data() + packet, 4);
        if (!header)
            return true;
        const std::size_t size = static_cast<std::size_t>(header >> 8U) * 4;
        if (size < 20 || size > model.source.size() - packet)
            return false;
        const std::size_t end = packet + size;
        for (std::size_t command = packet + 20; command < end;) {
            if (end - command < 4)
                return false;
            std::memcpy(&header, model.source.data() + command, 4);
            const unsigned opcode = header & 255U;
            if (!opcode)
                break;
            const std::size_t length = static_cast<std::size_t>(header >> 8U) * 4;
            if (length < 4 || length > end - command)
                return false;
            if (opcode == 13) {
                if (length < 8)
                    return false;
                std::uint32_t count;
                std::memcpy(&count, model.source.data() + command + 4, 4);
                if (count > (length - 8) / 4)
                    return false;
                for (std::uint32_t index = 0; index < count; ++index) {
                    std::uint16_t key;
                    std::memcpy(&key, model.source.data() + command + 8 + index * 4, 2);
                    if (key == 0x10)
                        return false;
                }
            }
            command += length;
        }
        packet = end;
    }
    return false;
}

bool ReadTile(const MT7Model &model, std::size_t index, MapSeamTile &tile) {
    const MT7Node &node = model.nodes[index];
    if (node.parent != 0 || node.runtimeParent != &model.nodes.front() || !node.xb01Offset || !TranslationOnly(node))
        return false;
    const MT7Node &owner = node.borrowedMesh ? *node.borrowedMesh : node;
    const LegacyDrawResources &resources = owner.drawResources;
    if (!OpaqueMesh(model, index, owner))
        return false;
    if (!resources.prepared || resources.vertexBuffers.size() != 1 || !resources.indexBuffer || resources.indexDraws.empty() || resources.waveVertexColours)
        return false;
    if (node.xb01Offset > model.source.size() || model.source.size() - node.xb01Offset < 8)
        return false;
    std::uint32_t flags;
    std::memcpy(&flags, model.source.data() + node.xb01Offset + 4, sizeof(flags));
    if (flags & (4U | 0x40U))
        return false;
    tile.nodeIndex = index;
    tile.meshOffset = node.xb01Offset;
    tile.metadata = resources.vertexBuffers[0].metadata;
    tile.stride = tile.metadata & 0xffffU;
    tile.translation = node.position;
    const std::byte *const bytes = static_cast<const std::byte *>(d3t::GetBufferCpuData(resources.vertexBuffers[0].handle, &tile.bufferSize));
    std::int32_t indexBytes;
    const std::byte *const indices = static_cast<const std::byte *>(d3t::GetBufferCpuData(resources.indexBuffer, &indexBytes));
    if (!bytes || !indices || indexBytes <= 0 || tile.stride < 12 || tile.stride > 256 || tile.bufferSize <= 0 || tile.bufferSize % tile.stride != 0)
        return false;
    const std::size_t count = static_cast<std::size_t>(tile.bufferSize) / tile.stride;
    if (count < 4 || count > 4096)
        return false;
    Vector3 low{}, high{};
    for (std::size_t vertex = 0; vertex < count; ++vertex) {
        Vector3 position;
        std::memcpy(&position, bytes + vertex * tile.stride, sizeof(position));
        if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z))
            return false;
        if (!vertex)
            low = high = position;
        for (unsigned axis = 0; axis < 3; ++axis) {
            low[axis] = std::fmin(low[axis], position[axis]);
            high[axis] = std::fmax(high[axis], position[axis]);
        }
        tile.points.push_back({position,
            {static_cast<double>(position.x) + tile.translation.x, static_cast<double>(position.y) + tile.translation.y,
                static_cast<double>(position.z) + tile.translation.z},
            0});
    }
    unsigned flatAxes = 0;
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (low[axis] == high[axis]) {
            tile.planeAxis = static_cast<std::uint8_t>(axis);
            ++flatAxes;
        }
    }
    if (flatAxes != 1)
        return false;
    for (const LegacyIndexDraw &draw : resources.indexDraws) {
        const std::size_t command = static_cast<std::size_t>(node.xb01Offset) + draw.commandOffset;
        if (draw.primitive != 5 || draw.indexCount % 3 != 0 || command >= model.source.size() || static_cast<unsigned>(model.source[command]) != 16 ||
            draw.startIndex > static_cast<std::uint32_t>(indexBytes / 2) || draw.indexCount > static_cast<std::uint32_t>(indexBytes / 2) - draw.startIndex)
            return false;
        for (std::uint32_t triangle = 0; triangle < draw.indexCount; triangle += 3) {
            std::uint16_t vertices[3];
            std::memcpy(vertices, indices + (draw.startIndex + triangle) * 2U, sizeof(vertices));
            if (vertices[0] >= count || vertices[1] >= count || vertices[2] >= count)
                return false;
            AddTriangleEdge(tile, vertices[0], vertices[1]);
            AddTriangleEdge(tile, vertices[1], vertices[2]);
            AddTriangleEdge(tile, vertices[2], vertices[0]);
        }
    }
    unsigned sides[3][2]{};
    for (MapSeamEdge &edge : tile.edges) {
        if (edge.uses != 1)
            continue;
        for (unsigned axis = 0; axis < 3; ++axis) {
            if (axis == tile.planeAxis)
                continue;
            const double tolerance = MAP_SEAM_LOCAL_EDGE_ULPS * CoordinateUlp(low[axis], high[axis]);
            for (unsigned side = 0; side < 2; ++side) {
                const float bound = side ? high[axis] : low[axis];
                if (std::abs(tile.points[edge.first].original[axis] - bound) <= tolerance &&
                    std::abs(tile.points[edge.second].original[axis] - bound) <= tolerance) {
                    edge.axis = static_cast<std::uint8_t>(axis);
                    edge.side = side ? 1 : -1;
                    ++sides[axis][side];
                }
            }
        }
    }
    for (unsigned axis = 0; axis < 3; ++axis)
        if (axis != tile.planeAxis && (!sides[axis][0] || !sides[axis][1]))
            return false;
    return true;
}

void GuardEndpoint(MapSeamTile &tile, std::uint32_t vertex, const MapSeamTile &peer, std::uint32_t other, unsigned normalAxis, int side) {
    MapSeamPoint &point = tile.points[vertex];
    for (unsigned axis = 0; axis < 3; ++axis) {
        const double a = WorldCoordinate(tile, vertex, axis), b = WorldCoordinate(peer, other, axis);
        const double middle = static_cast<float>((a + b) * 0.5);
        if (axis == normalAxis) {
            // Four world-coordinate ULPs on either side form the optional eight-ULP overlap guardband.
            const double value = middle + side * MAP_SEAM_GUARD_ULPS * CoordinateUlp(a, b);
            if (point.guardedAxes & (1U << axis))
                point.corrected[axis] = side > 0 ? std::fmax(point.corrected[axis], value) : std::fmin(point.corrected[axis], value);
            else
                point.corrected[axis] = value;
            point.guardedAxes |= static_cast<std::uint8_t>(1U << axis);
        } else if (!(point.guardedAxes & (1U << axis))) {
            point.corrected[axis] = middle;
        }
    }
}

bool RootUnchanged(const MT7Model &model, const MapSeamModelResources &state) {
    const MT7Node &root = model.nodes.front();
    return TranslationOnly(root) && SameVector(root.position, state.rootPosition) && root.rotation.x == state.rootRotation.x &&
        root.rotation.y == state.rootRotation.y && root.rotation.z == state.rootRotation.z && SameVector(root.scale, state.rootScale) &&
        !root.transformOverride;
}

} // namespace

const MapSeamModelResources *GetMapSeamResources(const MT7Model &model) {
    return FindResources(model);
}

void ReleaseMapSeamResources(const MT7Model &model) {
    for (std::size_t index = 0; index < mapSeamModels.size(); ++index) {
        if (mapSeamModels[index]->nodes != model.nodes.data())
            continue;
        for (MapSeamNodeResources &node : mapSeamModels[index]->correctedNodes)
            if (node.ownedVertexBuffer)
                d3t::ReleaseBufferByHandle(&node.ownedVertexBuffer);
        mapSeamModels[index] = std::move(mapSeamModels.back());
        mapSeamModels.pop_back();
        return;
    }
}

void PrepareMapSeamResources(MT7Model &model) {
    if (model.nodes.empty() || FindResources(model) || !model.wbtrDeformers.empty() || !model.mdcVertexLinks.empty() || !model.mdpVertexLinks.empty())
        return;
    const MT7Node &root = model.nodes.front();
    if (!TranslationOnly(root))
        return;
    std::unique_ptr<MapSeamModelResources> state = std::make_unique<MapSeamModelResources>();
    state->nodes = model.nodes.data();
    state->nodeCount = model.nodes.size();
    state->rootPosition = root.position;
    state->rootRotation = root.rotation;
    state->rootScale = root.scale;
    std::vector<MapSeamTile> tiles;
    for (std::size_t index = 1; index < model.nodes.size(); ++index) {
        MapSeamTile tile{};
        if (ReadTile(model, index, tile))
            tiles.push_back(std::move(tile));
    }
    for (std::size_t first = 0; first < tiles.size(); ++first) {
        MapSeamTile &a = tiles[first];
        for (std::size_t second = first + 1; second < tiles.size(); ++second) {
            MapSeamTile &b = tiles[second];
            if (a.meshOffset != b.meshOffset || a.metadata != b.metadata || a.planeAxis != b.planeAxis)
                continue;
            for (const MapSeamEdge &ae : a.edges) {
                if (!ae.side)
                    continue;
                for (const MapSeamEdge &be : b.edges) {
                    if (ae.axis != be.axis || ae.side != -be.side)
                        continue;
                    std::uint32_t bf = be.first, bs = be.second;
                    if (!NearEndpoints(a, ae.first, b, bf) || !NearEndpoints(a, ae.second, b, bs)) {
                        bf = be.second;
                        bs = be.first;
                        if (!NearEndpoints(a, ae.first, b, bf) || !NearEndpoints(a, ae.second, b, bs))
                            continue;
                    }
                    GuardEndpoint(a, ae.first, b, bf, ae.axis, ae.side);
                    GuardEndpoint(a, ae.second, b, bs, ae.axis, ae.side);
                    GuardEndpoint(b, bf, a, ae.first, be.axis, be.side);
                    GuardEndpoint(b, bs, a, ae.second, be.axis, be.side);
                    ++state->matchedEdges;
                }
            }
        }
    }
    for (const MapSeamTile &tile : tiles) {
        bool changed = false;
        MapSeamNodeResources corrected{};
        corrected.nodeIndex = tile.nodeIndex;
        corrected.meshOffset = tile.meshOffset;
        corrected.translation = tile.translation;
        corrected.stride = tile.stride;
        corrected.bufferSize = tile.bufferSize;
        for (const MapSeamPoint &point : tile.points) {
            const Vector3 position = !point.guardedAxes ?
                point.original :
                Vector3{static_cast<float>(point.corrected[0] - tile.translation.x), static_cast<float>(point.corrected[1] - tile.translation.y),
                    static_cast<float>(point.corrected[2] - tile.translation.z)};
            corrected.originalPositions.push_back(point.original);
            corrected.correctedPositions.push_back(position);
            if (!SameVector(position, point.original)) {
                changed = true;
                const Vector3 delta{position.x - point.original.x, position.y - point.original.y, position.z - point.original.z};
                state->maximumWorldMovement = std::fmax(state->maximumWorldMovement, std::sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z));
            }
        }
        if (changed)
            state->correctedNodes.push_back(std::move(corrected));
    }
    mapSeamModels.push_back(std::move(state));
    MapSeamModelResources &prepared = *mapSeamModels.back();
    for (MapSeamNodeResources &corrected : prepared.correctedNodes) {
        const MT7Node &node = model.nodes[corrected.nodeIndex];
        const LegacyDrawResources &source = node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources;
        corrected.resources = source;
        corrected.sourceIndexBuffer = source.indexBuffer;
        d3t::CreateFixedSizeBuffer(corrected.bufferSize, &corrected.ownedVertexBuffer, 0);
        if (!corrected.ownedVertexBuffer || corrected.ownedVertexBuffer == source.vertexBuffers[0].handle) {
            ReleaseMapSeamResources(model);
            return;
        }
        corrected.resources.vertexBuffers[0].handle = corrected.ownedVertexBuffer;
    }
}

const LegacyDrawResources &GetCorrectedMapSeamResources(const MT7Model &model, const MT7Node &node, const LegacyDrawResources &source) {
    MapSeamModelResources *const state = FindResources(model);
    if (!state || !RootUnchanged(model, *state) || source.waveVertexColours || !model.wbtrDeformers.empty() || !model.mdcVertexLinks.empty() ||
        !model.mdpVertexLinks.empty() || node.parent != 0 || node.runtimeParent != model.nodes.data() || !TranslationOnly(node))
        return source;
    for (MapSeamNodeResources &corrected : state->correctedNodes) {
        if (&model.nodes[corrected.nodeIndex] != &node)
            continue;
        if (node.xb01Offset != corrected.meshOffset || !SameVector(node.position, corrected.translation) || source.vertexBuffers.size() != 1 ||
            source.vertexBuffers[0].metadata != corrected.resources.vertexBuffers[0].metadata || source.indexBuffer != corrected.sourceIndexBuffer)
            return source;
        std::int32_t size;
        const std::byte *const bytes = static_cast<const std::byte *>(d3t::GetBufferCpuData(source.vertexBuffers[0].handle, &size));
        if (!bytes || size != corrected.bufferSize)
            return source;
        for (std::size_t vertex = 0; vertex < corrected.originalPositions.size(); ++vertex)
            if (std::memcmp(bytes + vertex * corrected.stride, &corrected.originalPositions[vertex], sizeof(Vector3)) != 0)
                return source;
        std::byte *const destination =
            static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, corrected.ownedVertexBuffer, 1, 0));
        if (!destination)
            return source;
        std::memcpy(destination, bytes, static_cast<std::size_t>(size));
        for (std::size_t vertex = 0; vertex < corrected.correctedPositions.size(); ++vertex)
            std::memcpy(destination + vertex * corrected.stride, &corrected.correctedPositions[vertex], sizeof(Vector3));
        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, corrected.ownedVertexBuffer);
        corrected.resources = source;
        corrected.resources.vertexBuffers[0].handle = corrected.ownedVertexBuffer;
        return corrected.resources;
    }
    return source;
}

} // namespace shenmue::s2
