#pragma once

#include "s2/search_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

struct SearchSignModel {
    std::uint32_t name;
    char file[16];
};
typedef char Layout_search_model_dc_0[(sizeof(SearchSignModel) == 0x14) ? 1 : -1];
#define SHENMUE_SIGN(a, b, c, d, path) {SHEN_FIELD(name) SHEN_NAME4(a, b, c, d), SHEN_FIELD(file) path}
#define SHENMUE_SIGN_END {SHEN_FIELD(name) SHEN_NAME4('_', '_', '_', '_'), SHEN_FIELD(file) ""}
static const SearchSignModel searchSignModelsAK00[]= {SHENMUE_SIGN('K', '4', '0', '4', "AK00WM0G.MT7"), SHENMUE_SIGN('K', '4', '0', '2', "AK00WM1G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '0', "AK00WM2G.MT7"), SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsAR02[]= {SHENMUE_SIGN('K', '4', '0', '0', "AR02WM0G.MT7"), SHENMUE_SIGN('K', '4', '0', '3', "AR02WM1G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '4', "AR02WM2G.MT7"), SHENMUE_SIGN('K', '4', '0', '1', "AR02WM3G.MT7"), SHENMUE_SIGN('K', '4', '0', '2', "AR02WM4G.MT7"),
    SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsAR03[]= {SHENMUE_SIGN('K', '4', '0', '2', "AR03WM0G.MT7"), SHENMUE_SIGN('K', '4', '0', '3', "AR03WM1G.MT7"),
    SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsWS00[]= {SHENMUE_SIGN('K', '4', '0', '5', "WS00MM0G.MT7"), SHENMUE_SIGN('K', '4', '0', '8', "WS00MM1G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '9', "WS00MM2G.MT7"), SHENMUE_SIGN('K', '4', '0', '6', "WS00MM3G.MT7"), SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsWK00[]= {SHENMUE_SIGN('K', '4', '0', '6', "WK001M0G.MT7"), SHENMUE_SIGN('K', '4', '0', '7', "WK001M1G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '8', "WK001M2G.MT7"), SHENMUE_SIGN('K', '4', '0', '9', "WK001M3G.MT7"), SHENMUE_SIGN('K', '4', '1', '0', "WK001M4G.MT7"),
    SHENMUE_SIGN('K', '4', '1', '1', "WK001M5G.MT7"), SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsWN00[]= {SHENMUE_SIGN('K', '4', '0', '0', "WN001M4G.MT7"), SHENMUE_SIGN('K', '4', '0', '1', "WN001M0G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '2', "WN001M1G.MT7"), SHENMUE_SIGN('K', '4', '0', '3', "WN001M2G.MT7"), SHENMUE_SIGN('K', '4', '0', '4', "WN001M3G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '5', "WT001M4G.MT7"), SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsWB00[]= {SHENMUE_SIGN('K', '4', '0', '1', "WT001M4G.MT7"), SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsWT00[]= {SHENMUE_SIGN('K', '4', '0', '5', "WT001M0G.MT7"), SHENMUE_SIGN('K', '4', '0', '7', "WT001M2G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '8', "WT001M3G.MT7"), SHENMUE_SIGN('K', '4', '0', '9', "WT001M4G.MT7"), SHENMUE_SIGN('K', '4', '0', '6', "WT001M5G.MT7"),
    SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsWR00[]= {SHENMUE_SIGN('K', '4', '0', '5', "WR00MM0G.MT7"), SHENMUE_SIGN('K', '4', '0', '3', "WR00MM1G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '4', "WR00MM2G.MT7"), SHENMUE_SIGN_END};
static const SearchSignModel searchSignModelsWE00[]= {SHENMUE_SIGN('K', '4', '1', '1', "WE00MM0G.MT7"), SHENMUE_SIGN('K', '4', '0', '7', "WE00MM1G.MT7"),
    SHENMUE_SIGN('K', '4', '1', '0', "WE00MM2G.MT7"), SHENMUE_SIGN('K', '4', '0', '9', "WE00MM3G.MT7"), SHENMUE_SIGN('K', '4', '0', '6', "WE00MM4G.MT7"),
    SHENMUE_SIGN('K', '4', '0', '8', "WE00MM5G.MT7"), SHENMUE_SIGN_END};
#undef SHENMUE_SIGN
#undef SHENMUE_SIGN_END

extern Vector3 searchTemporaryPosition;
extern std::uint32_t searchTemporaryYaw;
void shenFreeTempModel();
void srchIncrementTemporaryModel();
void srchStaticApplySearchPosRot(void *data);
const SearchSignModel *SignModelsForCurrentArea();

} } // namespace shenmue::s2

#endif
