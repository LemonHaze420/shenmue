#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/anbd_event.hpp"

extern Vector3 ANBD_BOARD_LOCAL_POSITION, ANBD_BOARD_ORIGIN;
extern Vector3i ANBD_BOARD_ROTATION;

void ANBDMain(void)
{
    const char *directory;
    Vector3i rotation;
    Vector3 origin;
    Vector3 position;
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    directory = FTBL.Filepath_Gen(0);
    FTBL.EV_ControlPackageResources(0, 0, directory, (int)"ANANBORD", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    position = ANBD_BOARD_LOCAL_POSITION;
    origin = ANBD_BOARD_ORIGIN;
    rotation = ANBD_BOARD_ROTATION;
    FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    FTBL.EV_AT_SetPos(ANBD_BOARD_CHARACTER, ANBD_RECURSIVE_XYZ, &position);
    rotation = ANBD_BOARD_ROTATION;
    if (!FTBL.EV_GetPlayerFlag(11, 29))
        rotation.z = (rotation.z + 1092) & 0xffff;
    FTBL.EV_AT_SetAngY_0(ANBD_BOARD_CHARACTER, ANBD_RECURSIVE_XYZ, &rotation, 0);
}

Vector3 ANBD_BOARD_LOCAL_POSITION = {1.23f, 3.23f, 0.0f};
Vector3 ANBD_BOARD_ORIGIN = {95.2169f, 98.2f, 130.5461f};
Vector3i ANBD_BOARD_ROTATION = {0, 24576, 0};
#else
#include "s2/anbd_event.hpp"
#include "s2/event_runtime.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

namespace shenmue::s2 {

// @verified - p
void ANBDInit() {
}

// @verified - p
void ANBDMain(std::uintptr_t) {
    (void)EV_pakUtils(PakCategory::Package, ANBD_PACKAGE_WAIT_MODE, 0, 0, 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    (void)EV_pakUtils(
        PakCategory::Package, ANBD_PACKAGE_LOAD_MODE, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(ANBD_BOARD_PACKAGE), 0);
    (void)EV_pakUtils(PakCategory::Package, ANBD_PACKAGE_WAIT_MODE, 0, 0, 0);
    Vector3 position = ANBD_BOARD_LOCAL_POSITION;
    Vector3i rotation = ANBD_BOARD_ROTATION;
    scenApplyTransRot(&ANBD_BOARD_ORIGIN, &rotation, &position);
    EV_AT_SetPos(ANBD_BOARD_CHARACTER, ANBD_RECURSIVE_XYZ, position);
    rotation = ANBD_BOARD_ROTATION;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, ANBD_BOARD_PLAYER_FLAG) == 0) {
        rotation.z = static_cast<std::uint16_t>(static_cast<std::uint32_t>(rotation.z) + ANBD_BOARD_ROLL_ADJUSTMENT);
    }
    (void)EV_AT_SetAngY(ANBD_BOARD_CHARACTER, ANBD_RECURSIVE_XYZ, rotation, nullptr);
}

const ScriptAreaDefinition anbdScriptDefinition{
    .areaId = AREA_ANBD,
    .field04 = 0,
    .init = &ANBDInit,
    .main = &ANBDMain,
};

} // namespace shenmue::s2
#endif
