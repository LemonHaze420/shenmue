#pragma once

#include "s1/d3t/namespace.hpp"
#include <cstdint>

enum GameLanguage : std::int32_t {
    English = 0x0,
    Japanese = 0x1,
    French = 0x2,
    German = 0x3,
    Korean = 0x4,
    ChineseTrad = 0x5,
    ChineseSimp = 0x6,
};

namespace shenmue::s1 {

extern std::uint32_t g_nuklearTexture;
extern std::uint32_t g_nuklearInputLayout;
extern std::uint32_t g_nuklearShaderLoad;
extern std::uint32_t g_nuklearVertexShader;
extern std::uint32_t g_nuklearPixelShader;
extern std::int32_t g_nuklearVertexBuffer;
extern std::int32_t g_nuklearIndexBuffer;
extern std::uint32_t g_uiScreenWidth;
extern std::uint32_t g_uiScreenHeight;
extern std::int32_t g_uiLoadedLanguage;
extern std::int32_t g_requestedUiLanguage;
extern std::uint8_t g_dbg_is_jp_ver;
extern void *g_uiStringTable;
extern void *g_uiStringBuffer;
extern std::uint64_t g_uiStringBufferSize;
void ReadSubtitleGlyphsAndSubs(std::int32_t language);
extern std::uint8_t g_d3t_dbg_forceAllUiLanguages;
bool GetIsJPVersion();

GameLanguage GetCurrentLanguageId(GameLanguage requestedLanguage);
std::int64_t SelectAppropriateFontDefinition(GameLanguage language, std::uint32_t renderHandle);
std::uint32_t LoadNuklearBuffer(std::uint32_t renderHandle);
void CreateDXTextureForNuklearBuffer();
namespace d3t {
void InitNuklearTexture2D();
} // namespace d3t

} // namespace shenmue::s1
