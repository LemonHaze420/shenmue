#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/pati_nails.hpp"

#include <array>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name PACH_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name PACH_CAMERA{SHEN_NAME(CDMY)};
inline constexpr shen_name PACH_PIN{SHEN_NAME(PIN_)};
inline constexpr std::array<shen_name, 16> PACH_BOARDS{{
    SHEN_NAME(BD00),
    SHEN_NAME(BD01),
    SHEN_NAME(BD02),
    SHEN_NAME(BD03),
    SHEN_NAME(BD04),
    SHEN_NAME(BD05),
    SHEN_NAME(BD06),
    SHEN_NAME(BD07),
    SHEN_NAME(BD08),
    SHEN_NAME(BD09),
    SHEN_NAME(BD0A),
    SHEN_NAME(BD0B),
    SHEN_NAME(BD0C),
    SHEN_NAME(BD0D),
    SHEN_NAME(BD0E),
    SHEN_NAME(BD0F),
}};
inline constexpr std::array<float, 3> PACH_BOARD_HEIGHT{1.0f, 1.7999999523162842f, 0.800000011920929f};
inline constexpr Vector3 PACH_PIN_SCALE{1.0f, 1.0f, 1.0f};
inline constexpr std::array<const char *, 3> PACH_IMAGES{"DAIA", "DAIB", "DAIC"};

struct PachDebugLayout {
    std::int32_t mode;
    float horizontalAxis;
    float verticalAxis;
    float depth;
    Vector3 cameraPosition;
    std::int32_t nextProp;
    std::int32_t selectedProp;
    std::int32_t camera;
    std::int32_t propCount;
    std::uint32_t reserved2c;
    std::array<PatiNailNode, 512> props;
};
struct PachDebugTask {
    std::int32_t mode;
    std::int32_t selectedLayout;
    std::array<PachDebugLayout, 16> layouts;
};
static_assert(sizeof(PachDebugLayout) == 0x6030);
static_assert(offsetof(PachDebugLayout, props) == 0x30);
static_assert(sizeof(PachDebugTask) == 0x60308);

void AreaPACHInit();
void PACHInitializeCallback(void *);
void PACHDebugLoopCallback(void *);
std::int32_t PACHFindProp(PachDebugLayout *layout);
void PACHMirrorProps(PachDebugLayout *layout, std::int32_t vertical);
void PACHDebugGrid();
void AreaPACHMain(std::uintptr_t);
extern const ScriptAreaDefinition areaPACHScriptDefinition;

} // namespace shenmue::s2::areas
