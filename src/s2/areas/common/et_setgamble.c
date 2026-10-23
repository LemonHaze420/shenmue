#include "s2/areas/area_gambling.hpp"
#include "s2/area_event.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

namespace shenmue::s2 {
std::uint32_t g_areaBoardCharacterIndex{};
namespace {
void EnsureGamblingPackage(const char *archive) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(archive), 0))
        return;
    const char *const directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(archive));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
shen_name CreateGamblingModel(const char *model, shen_name name = {}) {
    const shen_name character = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>(model), name);
    EnableScenarioCharacterDisplay(character);
    return character;
}
std::int32_t GamblingAngle(float degrees) {
    return static_cast<std::int32_t>((static_cast<double>(degrees) * 8192.0) / 45.0);
}
void SetGamblingLocalPlacement(shen_name character, shen_name parent, const Vector3 &position) {
    const Vector3i rotation{};
    AT_SetLocalBaseChara(character, parent);
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(character, EventTransformFlags::AllAxes, rotation);
}
} // namespace

bool IsNotInGamblingHouse() {
    return g_currentMap.name != SHEN_NAME(AKT3) && g_currentMap.name != SHEN_NAME(AKT0) && g_currentMap.name != SHEN_NAME(AKT1) &&
        g_currentMap.name != SHEN_NAME(AKT2) && g_currentMap.name != SHEN_NAME(QAHF) && g_currentMap.name != SHEN_NAME(QAJI);
}

void PlaceGamblingBoardDigits(shen_name parent, std::int32_t digits, std::int16_t axis) {
    static constexpr const char *models[]{
        "DSTKMM0G", "DSTKMM1G", "DSTKMM2G", "DSTKMM3G", "DSTKMM4G", "DSTKMM5G", "DSTKMM6G", "DSTKMM7G", "DSTKMM8G", "DSTKMM9G"};
    static constexpr float heights[]{0.391f, 0.1078f};
    static constexpr float depths[]{0.0568f, 0.1211f};

    static constexpr float positions[]{1.5f, -0.0445f, 0.0152f, 0.0758f, 0.1356f};
    const Vector3 scale{1.5f, 1.5f, 1.5f};
    bool started = false;
    std::int32_t divisor = 10000;
    for (float sample : positions) {
        const std::int32_t digit = digits / divisor;
        if (static_cast<std::int32_t>(started) | digit) {
            const shen_name character = CreateGamblingModel(digit >= 10 ? models[0] : models[digit]);
            SetMotionworkFlagForCharaById(character, &scale);
            const Vector3 position{sample * 1.5f, heights[axis] * 1.5f, depths[axis] * 1.5f};
            SetGamblingLocalPlacement(character, parent, position);
            AT_SetLocalBaseChara(character, shen_name{0xffffffffU});
            started = true;
        }
        divisor /= 10;
    }
}

void LoadBoardSet(shen_name parent, float x, float y, float z, float angleX, float angleY, float angleZ, std::int32_t firstDigits, std::int32_t secondDigits) {
    const Vector3 scale{1.5f, 1.5f, 1.5f};
    const Vector3i rotation{GamblingAngle(angleX), GamblingAngle(angleY), GamblingAngle(angleZ)};
    EnsureGamblingPackage("BOARDSET");
    const shen_name name{(g_areaBoardCharacterIndex++ << 24) + SHEN_NAME(mb00)};
    CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("DSTKM01G"), name);
    EnableScenarioCharacterDisplay(name);
    SetMotionworkFlagForCharaById(name, &scale);
    SetGamblingLocalPlacement(name, parent, Vector3{x, y, z});
    EV_AT_SetAngY(name, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    PlaceGamblingBoardDigits(name, firstDigits, 0);
    PlaceGamblingBoardDigits(name, secondDigits, 1);
}

void SetupObjects(std::int16_t index) {
    static constexpr const char *models[7][6]{
        {"DSTPM01G", "DSTNMA0G", "DSTIM01G", nullptr, nullptr, nullptr},
        {"DSTPM02G", "DSTNMM0G", "DSTIM03G", nullptr, nullptr, nullptr},
        {"DSTPM03G", "DSTNMA2G", "DSTIM04G", nullptr, nullptr, nullptr},
        {"DSTPM04G", "DSTNMM1G", "DSTIM02G", "DSTSM01G", "DSTSM02G", nullptr},
        {"DSTPM05G", "DSTNMM2G", "DSTIM05G", "DSTSM01G", "DSTSM02G", nullptr},
        {"DSTPM06G", "DSTNMM1G", "DSTIM02G", "DSTSM01G", "DSTSM02G", nullptr},
        {"DSTPM07G", "DSTNMA1G", "DSTIM06G", nullptr, nullptr, nullptr},
    };
    const auto &board = aberdeenBS[index];
    for (std::int32_t object = 0; models[board.objectKinds[0]][object]; ++object) {
        const shen_name character = CreateGamblingModel(models[board.objectKinds[0]][object]);
        const Vector3i rotation{0, GamblingAngle(board.angleY), 0};
        EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, board.position);
        EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    }
}

void GambleInit() {
    for (const auto &board : aberdeenBS_0) {
        if (!board.area.name)
            break;
        if (g_currentMap.name == board.area.name)
            LoadBoardSet(shen_name{0xffffffffU}, board.position.x, board.position.y, board.position.z, board.angleX, board.angleY, board.angleZ,
                board.firstDigits, board.secondDigits);
    }
    for (std::int16_t index = 0; aberdeenBS[index].area.name; ++index) {
        const auto &board = aberdeenBS[index];
        if (g_currentMap.name != board.area.name)
            continue;
        EnsureGamblingPackage("BGSMSET");
        EnsureGamblingPackage("BOARDSET");
        const shen_name table = CreateGamblingModel("DSTTM00G");
        const Vector3i rotation{0, GamblingAngle(board.angleY), 0};
        EV_AT_SetPos(table, EventTransformFlags::World | EventTransformFlags::AllAxes, board.position);
        EV_AT_SetAngY(table, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        SetupObjects(index);
        if (IsNotInGamblingHouse())
            LoadBoardSet(table, 1.0f, 0.0f, -0.5f, 0.0f, board.angleY, 0.0f, board.firstDigits, board.secondDigits);
        SetGamblingLocalPlacement(CreateGamblingModel("CHRW1T1G"), table, Vector3{0.0f, 0.0f, -1.1f});
    }
    for (const auto &board : wanChaiBS) {
        if (!board.area.name)
            break;
        if (g_currentMap.name != board.area.name)
            continue;
        EnsureGamblingPackage("UEDESET");
        EnsureGamblingPackage("BOARDSET");
        const shen_name table = CreateGamblingModel("WSC1MISG");
        const Vector3 scale{2.0f, 2.0f, 1.5f};
        SetMotionworkFlagForCharaById(table, &scale);
        const Vector3 position{board.position.x, board.position.y + 0.75f, board.position.z};
        const Vector3i rotation{0, GamblingAngle(board.angleY), 0};
        EV_AT_SetPos(table, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(table, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        SetGamblingLocalPlacement(CreateGamblingModel("TYUHIN1G"), table, Vector3{-0.12f, 0.0f, 0.07f});
        if (IsNotInGamblingHouse())
            LoadBoardSet(table, 1.0f, -0.75f, -0.5f, 0.0f, board.angleY, 0.0f, board.firstDigits, board.secondDigits);
        SetGamblingLocalPlacement(CreateGamblingModel("CHRW1T1G"), table, Vector3{0.0f, -0.75f, -0.9f});
    }
    for (std::int32_t index = 0; kowloonBS[index].area.name; ++index) {
        const auto &board = kowloonBS[index];
        if (g_currentMap.name != board.area.name)
            continue;
        EnsureGamblingPackage("KACHSET");
        EnsureGamblingPackage("BOARDSET");
        const shen_name table = CreateGamblingModel("TABLMOTG");
        Vector3i rotation{0, GamblingAngle(board.angleY), 0};
        EV_AT_SetPos(table, EventTransformFlags::World | EventTransformFlags::AllAxes, board.position);
        EV_AT_SetAngY(table, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        SetGamblingLocalPlacement(CreateGamblingModel("KACOT20G"), table, Vector3{0.31f, 0.76f, 0.14f});
        const shen_name box = CreateGamblingModel("KCHG1DBG");
        SetGamblingLocalPlacement(box, table, Vector3{0.0f, 0.75f, index ? 0.0f : -0.1f});
        AT_SetLocalBaseChara(box, table);
        rotation.y = 8192;
        EV_AT_SetAngY(box, EventTransformFlags::AllAxes, rotation);
        AT_SetLocalBaseChara(box, shen_name{0xffffffffU});
        if (IsNotInGamblingHouse())
            LoadBoardSet(table, 1.5f, 0.0f, -0.3f, 0.0f, board.angleY, 0.0f, board.firstDigits, board.secondDigits);
        SetGamblingLocalPlacement(CreateGamblingModel("CHRW1T1G"), table, Vector3{0.0f, 0.0f, -0.8f});
    }
}
} // namespace shenmue::s2
