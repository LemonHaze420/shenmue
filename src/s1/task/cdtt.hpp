#pragma once

#include "s1/hlib.hpp"
#include "s1/ui_runtime.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

using CDTTMode = UICdttMode;

enum class CDTTWidgetId : std::uint8_t {
    BackgroundPanel = 0,
    BlackBackground = 1,
    HeaderPanel = 2,
    LocationLabelParent = 3,
    DateLabel = 4,
    TimeLabel = 5,
    LocationNameLabel = 6,
    LocationSubNameLabel = 7,
    LoadingText = 8,
};

struct CDTTLoadScreenParameters {
    std::int32_t mode;
    std::int32_t setting;
};



struct MapLoadingScreenEntry {
    const char *areaId;
    const char *filename;
};

struct CDTTWidgetState {
    CDTTWidgetId id;
    std::uint8_t authoredOrder;
    std::uint8_t visible;
    std::uint8_t authored;
};

struct CDTTSurfaceState {
    std::int32_t primaryHeader{-1};
    std::int32_t specialDateHeader{-1};
    std::array<std::int32_t, 10> digitSurfaces{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::uint8_t digitSurfaceCount{};
};


struct CDTTState {
    HLib::Task *task{};
    CDTTMode mode{CDTTMode::None};
    std::int16_t alpha{};
    std::uint8_t fadingOut{};
    std::uint8_t presentationAuthored{};
    std::uint8_t renderSuppressed{};
    std::uint8_t postProcessPass{};
    std::uint16_t reserved016{};
    CDTTSurfaceState surfaces{};
    std::array<CDTTWidgetState, 9> widgets{};
    UICdttPresentationPayload payload{};
    UICdttPresentationText text{};
    std::uint32_t presentationGeneration{};
};

static_assert(sizeof(CDTTLoadScreenParameters) == 8);
static_assert(offsetof(CDTTLoadScreenParameters, mode) == 0);
static_assert(offsetof(CDTTLoadScreenParameters, setting) == 4);
static_assert(sizeof(CDTTWidgetState) == 4);


extern CDTTMode g_cdttMode;

extern const std::byte *g_cdttLoadScreenChunk;

extern HLib::Task *g_cdttTask;

extern std::int16_t g_cdttAlpha;

extern std::int32_t g_cdttLoadNumCount;

extern std::uint8_t g_cdttIntroDateTimeSet;

const CDTTState &CDTT_GetState();
void CDTT_SetMode(CDTTMode mode);
void CDTT_SetPresentationPayload(const UICdttPresentationPayload &payload);
void IncrementCurrentDateTime_Wrapper();
void RenderLoadingScreen(std::int32_t name);
bool CDTT_PrepareLoadingScreen(std::uint32_t areaId = 0);
HLib::Task *CDTT_Init();
void CDTT_Callback(HLib::TaskData *taskData);
void CDTT_FreeCallback(HLib::TaskData *taskData);
void CDTT_BeginFadeOut();
void CDTT_ResetPublishedState();
void CDTT_Clear();
bool CDTT_ApplyLoadScreenUtility(const CDTTLoadScreenParameters &params);
bool CDTT_IsLoadingUIActive();


inline constexpr std::array<MapLoadingScreenEntry, 86> kMapIdList{{
    {"D000", "LDDOBUIT.GZ"},
    {"DKTY", "LDBUNKAD.GZ"},
    {"KOTO", "LDBUNKAD.GZ"},
    {"JHD0", "LDHAZUKI.GZ"},
    {"JOMO", "LDOMOYA.GZ"},
    {"OMIN", "LDOMOYA.GZ"},
    {"YDB1", "LDCHIKA.GZ"},
    {"JU00", "LDYAMANO.GZ"},
    {"JD00", "LDSAKURA.GZ"},
    {"DTAT", "LDTATOO.GZ"},
    {"TATQ", "LDTATOO.GZ"},
    {"DAZA", "LDASIARK.GZ"},
    {"MFSY", "LDSINYKK.GZ"},
    {"ARAR", "LDASIARK.GZ"},
    {"MS08", "LDDAI8SO.GZ"},
    {"DTKY", "LDRIYOMA.GZ"},
    {"MKSG", "LDKYUSOK.GZ"},
    {"DSKI", "LDSEKAIR.GZ"},
    {"DCHA", "LDAJIITI.GZ"},
    {"DBHB", "LDBARHAR.GZ"},
    {"DGCT", "LDGAMEYU.GZ"},
    {"DSBA", "LDSOBAYA.GZ"},
    {"DJAZ", nullptr},
    {"DBYO", "LDBARYKS.GZ"},
    {"DRSA", "LDROSIYA.GZ"},
    {"DCBN", "LDTOMATO.GZ"},
    {"DRHT", "LDRYURIH.GZ"},
    {"DRME", "LDMANPUK.GZ"},
    {"DMAJ", "LDDAISAN.GZ"},
    {"DPIZ", "LDBPIZZA.GZ"},
    {"DSUS", "LDTAKARA.GZ"},
    {"DYKZ", "LDNAGAIK.GZ"},
    {"DURN", "LDRAPISU.GZ"},
    {"DSLI", "LDLINDA.GZ"},
    {"DKPA", "LDKARAOK.GZ"},
    {"JABE", "LDABESYO.GZ"},
    {"MN01", "LDDAI1SO.GZ"},
    {"MN02", "LDDAI2SO.GZ"},
    {"MN03", "LDDAI3SO.GZ"},
    {"MN04", "LDDAI4SO.GZ"},
    {"MN05", "LDDAI5SO.GZ"},
    {"MN06", "LDDAI6SO.GZ"},
    {"MN07", "LDDAI7SO.GZ"},
    {"MN08", "LDDAI8SO.GZ"},
    {"MN09", "LDDAI9SO.GZ"},
    {"MN10", "LDDAI10S.GZ"},
    {"MN11", "LDDAI11S.GZ"},
    {"MN12", "LDDAI12S.GZ"},
    {"MN13", "LDDAI13S.GZ"},
    {"MN14", "LDDAI14S.GZ"},
    {"MN15", "LDDAI15S.GZ"},
    {"MN16", "LDDAI16S.GZ"},
    {"MN17", "LDDAI17S.GZ"},
    {"OP00", "LD4DAYAF.GZ"},
    {"JOMO", "LDKITAKU.GZ"},
    {"DHQB", "LDBARHAR.GZ"},
    {"DSLT", "LDSLOTHO.GZ"},
    {"MS8A", "LDDAI8SO.GZ"},
    {"MS8S", "LDDAI8SO.GZ"},
    {"MO01", "LDJIMSYO.GZ"},
    {"MKS1", "LDDAI1SO.GZ"},
    {"MKS2", "LDDAI2SO.GZ"},
    {"MKS3", "LDDAI3SO.GZ"},
    {"MKS4", "LDDAI4SO.GZ"},
    {"MKS5", "LDDAI5SO.GZ"},
    {"MKS6", "LDDAI6SO.GZ"},
    {"MKS7", "LDDAI7SO.GZ"},
    {"MKS8", "LDDAI8SO.GZ"},
    {"MKS9", "LDDAI9SO.GZ"},
    {"TOKI", "LDROSIYA.GZ"},
    {"JD00", "LDKOUJIG.GZ"},
    {"GMCT", "LDGAMEYU.GZ"},
    {"MN18", "LDDAI18S.GZ"},
    {"MO99", "LDDAI8SO.GZ"},
    {"MKS0", "LDDAI10S.GZ"},
    {"MKYU", "LDKYUKEI.GZ"},
    {"MA00", "LDSINYKK.GZ"},
    {"JOMO", "LDRYROOM.GZ"},
    {"TERY", "LDSINYKK.GZ"},
    {"MSYO", "LDWASYOK.GZ"},
    {"BETD", "LDHAZUKI.GZ"},
    {"DNOZ", "LDSAKURA.GZ"},
    {"MSBS", "LDHAZUKI.GZ"},
    {"YDMA", "LDHAZUKI.GZ"},
    {"OP00", "LDOPNING.GZ"},
    {"9999", nullptr},
}};

} // namespace shenmue::s1
