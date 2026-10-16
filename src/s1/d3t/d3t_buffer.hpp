#pragma once

#include "d3t/buffer.hpp"

namespace shenmue::s1 {

using d3t::BufferDescription;
using d3t::BufferResource;
using d3t::CreateBuffer;
using d3t::D3TBufferBackendView;
using D3TDeviceContext = d3t::D3TDeviceContext;
using d3t::DX11_CopySceneNodeToGPU;
using d3t::FreeBuffers;
using d3t::GetBufferBackendView;
using d3t::GetBufferCpuData;
using d3t::GetD3DDeviceContext;
using d3t::GetSizeForModelBuffer;
using d3t::GetWriteMappedBufferPointer;
using d3t::ValidateAndCommitBuffer;
using d3t::g_D3DDevice;

namespace DX11 = d3t::DX11;

} // namespace shenmue::s1
