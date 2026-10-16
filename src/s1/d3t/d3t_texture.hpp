#pragma once

#include "d3t/texture.hpp"

namespace shenmue::s1 {

using d3t::AssignShaderCallback;
using d3t::CreateDirectXTexture2D;
using d3t::CreateMipmappedScaledTexture;
using d3t::CreateTexture2D;
using d3t::CreatePVRTextureBufferForSurface;
using d3t::D3TTextureBackendView;
using d3t::DecodedTextureFormat;
using d3t::DecodeS1LegacyTextureFormat;
using d3t::ExchangePVRSurfaceTexture;
using d3t::GenerateMipsAndUpdateResourcesForDirectX;
using d3t::GetD3TNativeTextureFormat;
using d3t::GetPVRSurfaceRecord;
using d3t::GetPVRSurfaceTextureHandle;
using d3t::GetStride;
using d3t::GetTexture2DBackendView;
using d3t::GetTexture2DDescription;
using d3t::GetTexture2DDimensions;
using d3t::g_TextureRefCount;
using d3t::IncrementTextureById;
using d3t::init_format_desc;
using d3t::InitializePvrTextureSlotStorage;
using d3t::InitPVRTaskData;
using d3t::InitTexture2DResource;
using d3t::kInvalidLegacyTextureFormat;
using d3t::QueuedTextureCopy;
using d3t::ReplaceTexture2DResource;
using d3t::SurfaceMapping;
using d3t::SurfaceRecord;
using d3t::Texture2DDescription;
using d3t::TextureResource;
using d3t::TextureUploadDescription;
using d3t::d3t_UploadTexture2DToGPU;

#if defined(_WIN32)
using d3t::AdoptBackBufferTexture2D;
using d3t::ReplaceTexture2DNativeResource;
#endif

} // namespace shenmue::s1
