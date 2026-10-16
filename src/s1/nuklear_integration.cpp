#include "s1/nuklear_integration.hpp"
#include "s1/d3t/namespace.hpp"
#include "s1/archive_data.hpp"

#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/shader_initialization.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdlib>

namespace shenmue::s1 {

const char *UI_Elements_Init();
std::int64_t LoadSplash();
namespace d3t {
void LoadFont(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *sourcePath);
} // namespace d3t

std::uint32_t g_nuklearTexture = 0;
std::uint32_t g_nuklearInputLayout = 0;
std::uint32_t g_nuklearShaderLoad = 0;
std::uint32_t g_nuklearVertexShader = 0;
std::uint32_t g_nuklearPixelShader = 0;
std::int32_t g_nuklearVertexBuffer = 0;
std::int32_t g_nuklearIndexBuffer = 0;
std::uint32_t g_uiScreenWidth = 0;
std::uint32_t g_uiScreenHeight = 0;
std::int32_t g_uiLoadedLanguage = -1;
std::uint8_t g_dbg_is_jp_ver = 0;
void *g_uiStringTable = nullptr;
void *g_uiStringBuffer = nullptr;
std::uint64_t g_uiStringBufferSize = 0;

namespace {

std::int32_t g_pendingUiLanguage = 0;
std::uint32_t g_previousUiRenderHandle = 0;
std::uint64_t g_splashRenderState = 0;
std::uint32_t g_splashRenderFlags = 0;

#pragma pack(push, 1)
struct FontLoadRequest {
    std::uint16_t generation;
    std::uint32_t renderHandle;
    std::uint32_t language;
    std::byte reserved[70];
};
#pragma pack(pop)

static_assert(offsetof(FontLoadRequest, renderHandle) == 2);
static_assert(offsetof(FontLoadRequest, language) == 6);
static_assert(sizeof(FontLoadRequest) == 80);

std::array<FontLoadRequest, 8192> g_fontLoadRequests{};
std::atomic<std::uint16_t> g_fontLoadRequestIndex{0};




GameLanguage GetSteamUiLanguage() {
    return GameLanguage::English;
}

} // namespace

std::int64_t SelectAppropriateFontDefinition(GameLanguage language, std::uint32_t renderHandle) {
    const std::uint16_t index = g_fontLoadRequestIndex.fetch_add(1, std::memory_order_seq_cst) & 0x1fffU;
    FontLoadRequest &request = g_fontLoadRequests[index];
    ++request.generation;
    request.renderHandle = renderHandle;
    request.language = language;

    const char *path = "/ui/font/english.fontdef";
    switch (language) {
    case 1:
        path = "/ui/font/Japanese.fontdef";
        break;
    case 2:
        path = "/ui/font/French.fontdef";
        break;
    case 3:
        path = "/ui/font/German.fontdef";
        break;
    case 4:
        path = "/ui/font/Korean.fontdef";
        break;
    case 5:
        path = "/ui/font/ChineseT.fontdef";
        break;
    case 6:
        path = "/ui/font/ChineseS.fontdef";
        break;
    default:
        break;
    }

    D3TShaderLoadTask task{};
    task.path = path;
    task.renderHandle = renderHandle;
    task.thread.callback = d3t::LoadFont;
    task.thread.data = &request.renderHandle;
    return PushShaderLoadToThreadQueue(&task);
}

std::uint8_t g_d3t_dbg_forceAllUiLanguages = 0;

bool GetIsJPVersion() {
    return g_dbg_is_jp_ver == 0;
}

GameLanguage GetCurrentLanguageId(GameLanguage requestedLanguage) {
    if (requestedLanguage < 0)
        requestedLanguage = GetSteamUiLanguage();

    constexpr std::array<GameLanguage, 7> allLanguages{English, Japanese, French, German, Korean, ChineseTrad, ChineseSimp};

    constexpr std::array<GameLanguage, 1> japaneseLanguages{Japanese};
    constexpr std::array<GameLanguage, 6> otherLanguages{English, French, German, Korean, ChineseTrad, ChineseSimp};

    const GameLanguage *supportedLanguages = g_dbg_is_jp_ver ? japaneseLanguages.data() : otherLanguages.data();
    std::size_t supportedLanguageCount = g_dbg_is_jp_ver ? japaneseLanguages.size() : otherLanguages.size();

    // this is dead-code in the actual build
    if (g_d3t_dbg_forceAllUiLanguages != 0) {
        supportedLanguages = allLanguages.data();
        supportedLanguageCount = allLanguages.size();
    }

    for (std::size_t index = 0; index < supportedLanguageCount; ++index) {
        if (supportedLanguages[index] == requestedLanguage)
            return requestedLanguage;
    }

    return supportedLanguages[0];
}
std::uint32_t LoadNuklearBuffer(std::uint32_t renderHandle) {
    D3TFVFDescription description{};
    description.elementCount = 3;
    description.elements[0] = {
        "POSITION",
        0,
        20,
        0,
        0,
        0,
        0,
        0,
        0,
    };
    description.elements[1] = {
        "TEXCOORD",
        0,
        20,
        0,
        0,
        0,
        0,
        0,
        0,
    };
    description.elements[2] = {
        "COLOR",
        0,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
    };
    for (std::uint32_t index = 0; index < description.elementCount; ++index) {
        D3TFVFElement &element = description.elements[index];
        element.semanticHash = d3t_HashData(element.semanticName, static_cast<std::uint32_t>(std::strlen(element.semanticName)));
    }
    g_nuklearInputLayout = CreateFVF(&description);
    g_nuklearShaderLoad = LoadPrecompiledShader("/engine/assets/shaders/nuklear", renderHandle);
    return g_nuklearShaderLoad;
}

void CreateDXTextureForNuklearBuffer() {
    InitTexture2DResource(g_nuklearTexture);
    const Texture2DDescription description{
        g_uiScreenWidth,
        1080,
        1,
        1,
        2,
        4,
        0,
        1,
        0,
        0,
        nullptr,
    };
    g_nuklearTexture = CreateTexture2D(&description);
}

namespace d3t {
void InitNuklearTexture2D() {
    const std::uint32_t renderHandle = g_activeRenderHandle;
    UI_Elements_Init();

    g_uiScreenWidth = 1920;
    g_uiScreenHeight = 1080;
    CreateDXTextureForNuklearBuffer();

    const std::int32_t language = GetCurrentLanguageId((GameLanguage)-1);
    g_pendingUiLanguage = language;
    g_requestedUiLanguage = language;
    if (language != g_uiLoadedLanguage) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBuffer = nullptr;
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = language;
        ReadSubtitleGlyphsAndSubs(language);
    }

    SelectAppropriateFontDefinition(static_cast<GameLanguage>(g_pendingUiLanguage), renderHandle);
    LoadNuklearBuffer(renderHandle);
    g_previousUiRenderHandle = renderHandle;
    LoadSplash();

    g_splashRenderState = 0;
    g_splashRenderFlags = 0;
    g_previousUiRenderHandle = 0;
}
} // namespace d3t

} // namespace shenmue::s1
