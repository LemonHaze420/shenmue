#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/area_gambling.hpp"

const GamblingFreeBoardPlacement aberdeenBS_0[5] = {
    {SHEN_NAME4('A', 'K', 'T', '0'), {246.2f, 107.2f, 232.5f}, 10.0f, 180.0f, 0.0f, 10, 50},
    {SHEN_NAME4('A', 'K', '0', '0'), {271.7f, 98.0f, 253.7f}, 10.0f, 90.0f, 0.0f, 100, 500},
    {SHEN_NAME4('A', 'K', 'T', '2'), {297.6f, 107.2f, 230.3f}, 355.0f, 90.0f, 345.0f, 50, 200},
    {SHEN_NAME4('A', 'K', '0', '0'), {285.6f, 100.0f, 224.1f}, 0.0f, 90.0f, 0.0f, 5, 20},
    {0, {0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, 10, 10},
};
const GamblingTablePlacement aberdeenBS[15] = {
    {SHEN_NAME4('A', 'K', 'T', '0'), {253.4f, 106.1f, 228.7f}, 270.0f, 10, 50, {1, 0}},
    {SHEN_NAME4('A', 'K', 'T', '1'), {267.5f, 96.85f, 265.2f}, 90.0f, 100, 500, {5, 0}},
    {SHEN_NAME4('A', 'K', 'T', '2'), {301.7f, 106.1f, 238.1f}, 180.0f, 50, 200, {0, 0}},
    {SHEN_NAME4('Q', 'R', '0', '0'), {118.2f, 93.025f, 189.5f}, 160.0f, 10, 100, {6, 0}},
    {SHEN_NAME4('Q', 'T', 'B', '2'), {251.6f, 94.9f, 248.1f}, 210.0f, 20, 100, {5, 0}},
    {SHEN_NAME4('Q', 'T', 'B', '2'), {245.2f, 94.9f, 238.8f}, 45.0f, 100, 1000, {2, 0}},
    {SHEN_NAME4('Q', 'T', 'B', '2'), {244.9f, 94.9f, 244.7f}, 105.0f, 50, 300, {4, 0}},
    {SHEN_NAME4('B', 'G', 'S', 'M'), {100.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {0, 0}},
    {SHEN_NAME4('B', 'G', 'S', 'M'), {150.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {1, 0}},
    {SHEN_NAME4('B', 'G', 'S', 'M'), {200.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {2, 0}},
    {SHEN_NAME4('B', 'G', 'S', 'M'), {250.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {3, 0}},
    {SHEN_NAME4('B', 'G', 'S', 'M'), {300.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {4, 0}},
    {SHEN_NAME4('B', 'G', 'S', 'M'), {350.0f, 0.0f, 0.0f}, 0.0f, 10, 50, {5, 0}},
    {SHEN_NAME4('Q', 'R', '0', '9'), {118.2f, 93.025f, 189.5f}, 160.0f, 10, 100, {6, 0}},
    {0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0, {0, 0}},
};
const GamblingBoardPlacement wanChaiBS[12] = {
    {GAMBLING_NO_PARENT, {280.74f, 100.03f, 224.22f}, 90.0f, 5, 20},
    {SHEN_NAME4('W', 'N', '0', '0'), {240.6f, 104.5f, 160.7f}, 180.0f, 10, 100},
    {SHEN_NAME4('W', 'R', '0', '0'), {178.6f, 101.3f, 149.6f}, 0.0f, 10, 50},
    {SHEN_NAME4('W', 'R', '0', '0'), {151.0f, 100.0f, 172.8f}, 270.0f, 20, 100},
    {SHEN_NAME4('W', 'S', '0', '0'), {47.0f, 98.2f, 145.8f}, 180.0f, 50, 300},
    {SHEN_NAME4('W', 'S', '0', '0'), {52.5f, 98.2f, 140.8f}, 270.0f, 100, 500},
    {SHEN_NAME4('Q', '3', '0', '0'), {229.6f, 100.0f, 166.2f}, 10.0f, 100, 500},
    {SHEN_NAME4('Q', 'R', '0', '0'), {154.6f, 100.0f, 193.8f}, 0.0f, 10, 100},
    {SHEN_NAME4('U', 'E', 'D', 'E'), {0.0f, 0.0f, 0.0f}, 0.0f, 10, 50},
    {SHEN_NAME4('U', 'E', 'D', 'E'), {0.0f, 0.0f, 0.0f}, 0.0f, 10, 50},
    {SHEN_NAME4('Q', 'R', '0', '9'), {154.6f, 100.0f, 193.8f}, 0.0f, 10, 100},
    {0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0},
};
const GamblingBoardPlacement kowloonBS[8] = {
    {SHEN_NAME4('Q', 'R', '0', '0'), {124.0f, 93.0f, 191.0f}, 167.5f, 5, 20},
    {SHEN_NAME4('Q', 'R', '0', '0'), {151.3f, 100.0f, 193.8f}, 350.0f, 10, 50},
    {SHEN_NAME4('Q', '3', '0', '0'), {225.9f, 100.0f, 168.8f}, 94.6f, 20, 100},
    {SHEN_NAME4('Q', 'A', '0', '0'), {291.9f, 110.8f, 172.4f}, 0.0f, 10, 100},
    {SHEN_NAME4('Q', 'A', '0', '0'), {292.0f, 125.2f, 172.2f}, 345.0f, 100, 500},
    {SHEN_NAME4('Q', 'R', '0', '9'), {124.0f, 93.0f, 191.0f}, 167.5f, 5, 20},
    {SHEN_NAME4('Q', 'R', '0', '9'), {151.3f, 100.0f, 193.8f}, 350.0f, 10, 50},
    {0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0},
};
const char *const gamblingDigitModels[10] = {
    "DSTKMM0G", "DSTKMM1G", "DSTKMM2G", "DSTKMM3G", "DSTKMM4G", "DSTKMM5G", "DSTKMM6G", "DSTKMM7G", "DSTKMM8G", "DSTKMM9G"};
unsigned int g_areaBoardCharacterIndex = 0;

void SetGamble(void)
{
    FTBL.DebugLogToFileRemoved("Gamble Init Start\n");
    GamblePrepareNullsub();
    GambleInit();
    FreeGambleEvent();
    FTBL.DebugLogToFileRemoved("Gamble Init End\n");
}

void GamblePrepareNullsub(void)
{
}

void GambleInit(void)
{
    Vector3 position;
    Vector3i rotation = {0, 0, 0};
    Vector3 scale;
    int i;
    shen_name table, character;
    for (i = 0; aberdeenBS_0[i].area; i++) {
        if (g_currentMap == aberdeenBS_0[i].area) {
            position = aberdeenBS_0[i].position;
            LoadBoardSet(GAMBLING_NO_PARENT, position.x, position.y, position.z, aberdeenBS_0[i].angleX, aberdeenBS_0[i].angleY,
                aberdeenBS_0[i].angleZ, aberdeenBS_0[i].firstDigits, aberdeenBS_0[i].secondDigits);
        }
    }
    for (i = 0; aberdeenBS[i].area; i++) {
        if (g_currentMap == aberdeenBS[i].area) {
            GAMBLING_PACKAGE("BGSMSET")
            GAMBLING_PACKAGE("BOARDSET")
            table = FTBL.CreateEventCharacter(3, "DSTTM00G", 0);
            FTBL.EV_EnableCharacterDisplay(table);
            rotation.y = GAMBLING_ANGLE(aberdeenBS[i].angleY);
            FTBL.EV_AT_SetPos(table, 0x78000000, &aberdeenBS[i].position);
            FTBL.EV_AT_SetAngY_0(table, 0x78000000, &rotation, 0);
            SetupObjects(i);
            if (IsNotInGamblingHouse())
                LoadBoardSet(table, 1.0f, 0.0f, -0.5f, 0.0f, aberdeenBS[i].angleY, 0.0f, aberdeenBS[i].firstDigits, aberdeenBS[i].secondDigits);
            character = FTBL.CreateEventCharacter(3, "CHRW1T1G", 0);
            FTBL.EV_EnableCharacterDisplay(character);
            SetGamblingLocalPlacement(character, table, 0.0f, 0.0f, -1.1f);
        }
    }
    for (i = 0; wanChaiBS[i].area; i++) {
        if (g_currentMap == wanChaiBS[i].area) {
            scale.x = 2.0f;
            scale.y = 2.0f;
            scale.z = 1.5f;
            GAMBLING_PACKAGE("UEDESET")
            GAMBLING_PACKAGE("BOARDSET")
            table = FTBL.CreateEventCharacter(3, "WSC1MISG", 0);
            FTBL.EV_EnableCharacterDisplay(table);
            FTBL.mtwkSetMotionworkScale3(table, &scale);
            position = wanChaiBS[i].position;
            position.y += 0.75f;
            rotation.y = GAMBLING_ANGLE(wanChaiBS[i].angleY);
            FTBL.EV_AT_SetPos(table, 0x78000000, &position);
            FTBL.EV_AT_SetAngY_0(table, 0x78000000, &rotation, 0);
            character = FTBL.CreateEventCharacter(3, "TYUHIN1G", 0);
            FTBL.EV_EnableCharacterDisplay(character);
            SetGamblingLocalPlacement(character, table, -0.12f, 0.0f, 0.07f);
            if (IsNotInGamblingHouse())
                LoadBoardSet(table, 1.0f, -0.75f, -0.5f, 0.0f, wanChaiBS[i].angleY, 0.0f, wanChaiBS[i].firstDigits, wanChaiBS[i].secondDigits);
            character = FTBL.CreateEventCharacter(3, "CHRW1T1G", 0);
            FTBL.EV_EnableCharacterDisplay(character);
            SetGamblingLocalPlacement(character, table, 0.0f, -0.75f, -0.9f);
        }
    }
    for (i = 0; kowloonBS[i].area; i++) {
        if (g_currentMap == kowloonBS[i].area) {
            GAMBLING_PACKAGE("KACHSET")
            GAMBLING_PACKAGE("BOARDSET")
            table = FTBL.CreateEventCharacter(3, "TABLMOTG", 0);
            FTBL.EV_EnableCharacterDisplay(table);
            rotation.y = GAMBLING_ANGLE(kowloonBS[i].angleY);
            FTBL.EV_AT_SetPos(table, 0x78000000, &kowloonBS[i].position);
            FTBL.EV_AT_SetAngY_0(table, 0x78000000, &rotation, 0);
            character = FTBL.CreateEventCharacter(3, "KACOT20G", 0);
            FTBL.EV_EnableCharacterDisplay(character);
            SetGamblingLocalPlacement(character, table, 0.31f, 0.76f, 0.14f);
            character = FTBL.CreateEventCharacter(3, "KCHG1DBG", 0);
            FTBL.EV_EnableCharacterDisplay(character);
            if (i == 0)
                SetGamblingLocalPlacement(character, table, 0.0f, 0.75f, -0.1f);
            else
                SetGamblingLocalPlacement(character, table, 0.0f, 0.75f, 0.0f);
            FTBL.AT_SetLocalBaseChara(character, table);
            rotation.y = 8192;
            FTBL.EV_AT_SetAngY_0(character, 0x38000000, &rotation, 0);
            FTBL.AT_SetLocalBaseChara(character, GAMBLING_NO_PARENT);
            if (IsNotInGamblingHouse())
                LoadBoardSet(table, 1.5f, 0.0f, -0.3f, 0.0f, kowloonBS[i].angleY, 0.0f, kowloonBS[i].firstDigits, kowloonBS[i].secondDigits);
            character = FTBL.CreateEventCharacter(3, "CHRW1T1G", 0);
            FTBL.EV_EnableCharacterDisplay(character);
            SetGamblingLocalPlacement(character, table, 0.0f, 0.0f, -0.8f);
        }
    }
}

void PlaceGamblingBoardDigits(shen_name parent, int digits, short axis)
{
    float positions[4] = {0.1356f, 0.0758f, 0.0152f, -0.0445f};
    float heights[2] = {0.391f, 0.1078f};
    float depths[2] = {0.0568f, 0.1211f};
    Vector3 position;
    Vector3 scale = {1.5f, 1.5f, 1.5f};
    Vector3i rotation = {0, 0, 0};
    int started = 0;
    int divisor = 10000;
    int sample = 4;
    int digit;
    const char *model;
    shen_name character;
    do {
        digit = digits / divisor;
        if (digit | started) {
            model = digit <= 9 ? gamblingDigitModels[digit] : gamblingDigitModels[0];
            do {
                character = FTBL.CreateEventCharacter(3, model, 0);
                FTBL.EV_EnableCharacterDisplay(character);
            } while (0);
            FTBL.mtwkSetMotionworkScale3(character, &scale);
            position.x = positions[sample] * 1.5f;
            position.y = heights[axis] * 1.5f;
            position.z = depths[axis] * 1.5f;
            FTBL.AT_SetLocalBaseChara(character, parent);
            FTBL.EV_AT_SetPos(character, 0x38000000, &position);
            FTBL.EV_AT_SetAngY_0(character, 0x38000000, &rotation, 0);
            FTBL.AT_SetLocalBaseChara(character, GAMBLING_NO_PARENT);
            started = 1;
        }
        divisor /= 10;
        sample--;
    } while (divisor > 0);
}

void SetupObjects(short index)
{
    const char *models[7][6] = {
        {"DSTPM01G", "DSTNMA0G", "DSTIM01G", 0, 0, 0},
        {"DSTPM02G", "DSTNMM0G", "DSTIM03G", 0, 0, 0},
        {"DSTPM03G", "DSTNMA2G", "DSTIM04G", 0, 0, 0},
        {"DSTPM04G", "DSTNMM1G", "DSTIM02G", "DSTSM01G", "DSTSM02G", 0},
        {"DSTPM05G", "DSTNMM2G", "DSTIM05G", "DSTSM01G", "DSTSM02G", 0},
        {"DSTPM06G", "DSTNMM1G", "DSTIM02G", "DSTSM01G", "DSTSM02G", 0},
        {"DSTPM07G", "DSTNMA1G", "DSTIM06G", 0, 0, 0},
    };
    Vector3i rotation = {0, 0, 0};
    int object;
    shen_name character;
    for (object = 0; models[aberdeenBS[index].objectKinds[0]][object]; object++) {
        character = FTBL.CreateEventCharacter(3, models[aberdeenBS[index].objectKinds[0]][object], 0);
        FTBL.EV_EnableCharacterDisplay(character);
        rotation.y = GAMBLING_ANGLE(aberdeenBS[index].angleY);
        FTBL.EV_AT_SetPos(character, 0x78000000, &aberdeenBS[index].position);
        FTBL.EV_AT_SetAngY_0(character, 0x78000000, &rotation, 0);
    }
}

int IsNotInGamblingHouse(void)
{
    int outside = 1;
    switch (g_currentMap) {
    case SHEN_NAME4('A', 'K', 'T', '3'):
    case SHEN_NAME4('A', 'K', 'T', '0'):
    case SHEN_NAME4('A', 'K', 'T', '1'):
    case SHEN_NAME4('A', 'K', 'T', '2'):
    case SHEN_NAME4('Q', 'A', 'H', 'F'):
    case SHEN_NAME4('Q', 'A', 'J', 'I'):
        outside = 0;
        break;
    }
    return outside;
}

void FreeGambleEvent(void)
{
    FTBL.scenFreeEvent("GSET.EV3");
}

void SetGamblingLocalPlacement(shen_name character, shen_name parent, float x, float y, float z)
{
    Vector3 position = {x, y, z};
    Vector3i rotation = {0, 0, 0};
    FTBL.AT_SetLocalBaseChara(character, parent);
    FTBL.EV_AT_SetPos(character, 0x38000000, &position);
    FTBL.EV_AT_SetAngY_0(character, 0x38000000, &rotation, 0);
}

void LoadBoardSet(shen_name parent, float x, float y, float z, float angleX, float angleY, float angleZ, int firstDigits, int secondDigits)
{
    Vector3 scale = {1.5f, 1.5f, 1.5f};
    Vector3i rotation = {GAMBLING_ANGLE(angleX), GAMBLING_ANGLE(angleY), GAMBLING_ANGLE(angleZ)};
    shen_name name;
    GAMBLING_PACKAGE("BOARDSET")
    name = (g_areaBoardCharacterIndex++ << 24) + GAMBLING_BOARD_BASE;
    FTBL.CreateEventCharacter(3, "DSTKM01G", name);
    FTBL.EV_EnableCharacterDisplay(name);
    FTBL.mtwkSetMotionworkScale3(name, &scale);
    SetGamblingLocalPlacement(name, parent, x, y, z);
    FTBL.EV_AT_SetAngY_0(name, 0x78000000, &rotation, 0);
    PlaceGamblingBoardDigits(name, firstDigits, 0);
    PlaceGamblingBoardDigits(name, secondDigits, 1);
}
#else
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
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(archive), 0))
        return;
    const char *const directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    EV_pakUtils(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(archive));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
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
            mtwkSetMotionworkScale3(character, &scale);
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
    mtwkSetMotionworkScale3(name, &scale);
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
        mtwkSetMotionworkScale3(table, &scale);
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
#endif
