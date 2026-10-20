#include "s2/areas/area_WBSS.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/field_collision.hpp"
#include "s2/pause.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/warehouse_script.hpp"
#include <array>
#include <bit>
#include <cmath>
#include <xmmintrin.h>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, siz{SHEN_NAME(SIZ_)}, cloth{SHEN_NAME(a148)}, anchor{SHEN_NAME(sus0)};
struct WipeRecord {
    std::uint8_t column, row;
    std::uint16_t reserved;
    std::int32_t yaw, kind;
    shen_name character;
    std::uint32_t flag;
};
struct ScaleRecord {
    shen_name character;
    float scale;
};
static_assert(sizeof(WipeRecord) == 20);

#pragma pack(push, 4)
struct WipeWork {
    std::uint8_t finished;
    std::int8_t kind;
    std::uint16_t reserved02;
    std::int32_t state, reserved08, phase, frame;
    std::array<std::byte, 0x1c> reserved14;
    std::int32_t yawTicks;
    std::uint64_t reserved34;
    std::int16_t yawStep;
    std::array<std::byte, 6> reserved3e;
    std::int32_t area;
    std::uint32_t line;
    std::uint32_t reserved4c;
};
#pragma pack(pop)
static_assert(sizeof(WipeWork) == 0x50);
static_assert(offsetof(WipeWork, area) == 0x44);
static_assert(offsetof(WipeWork, yawStep) == 0x3c);
std::uint32_t exitScript[]{0x0U, 0x4U, 0x0U, 0x0U, 0x40000001U, 0x80020016U, 0x5f4f5952U, 0x40cU, 0x8001000dU, 0x28U, 0x80010031U, 0x31a033U, 0x80000032U,
    0x8001000dU, 0x14U, 0x8001000eU, 0x1U, 0x40000002U, 0x80020016U, 0x5f4f5952U, 0x40cU, 0x8001000dU, 0x1eU, 0x80010031U, 0x31a034U, 0x80000032U, 0x8001000dU,
    0x28U, 0x80010031U, 0x31a035U, 0x80000032U, 0x8001000eU, 0x1U, 0x40000003U, 0x80020016U, 0x5f4f5952U, 0x40cU, 0x8001000dU, 0x1eU, 0x80010031U, 0x31a036U,
    0x80000032U, 0x8001000dU, 0x14U, 0x8001000eU, 0x1U, 0x40000004U, 0x40000005U, 0x40000006U, 0x40000007U, 0x40000008U, 0x8001000eU, 0x1U, 0x0U};
std::uint32_t wipe[]{0x0U, 0x4U, 0x0U, 0x0U, 0x40000000U, 0x80020016U, 0x5f4f5952U, 0x406U, 0x8001000dU, 0x8cU, 0x8001000eU, 0x5U, 0x40000001U, 0x80020016U,
    0x5f4f5952U, 0x406U, 0x8001000dU, 0x8cU, 0x8001000eU, 0x1U, 0x40000002U, 0x80020016U, 0x5f4f5952U, 0x407U, 0x8001000dU, 0xa5U, 0x8001000eU, 0x9U,
    0x4000000aU, 0x80020016U, 0x30737573U, 0x408U, 0x8001000dU, 0x78U, 0x80020016U, 0x30737573U, 0x409U, 0x8001000eU, 0x5U, 0x4000000bU, 0x80020016U,
    0x30737573U, 0x408U, 0x8001000dU, 0x78U, 0x8001000eU, 0x1U, 0x0U};
std::uint32_t mainScript[]{0x0U, 0x4U, 0x0U, 0x0U, 0x8001000dU, 0x5U, 0x80010005U, 0x5f5a4953U, 0x80010005U, 0x5f494153U, 0x80010015U, 0x5f4b4e4bU, 0x80000004U,
    0x80020007U, 0x24U, 0x0U, 0x80030006U, 0x2e1U, 0x2U, 0x1U, 0x40000001U, 0x80030006U, 0x47U, 0x3U, 0x4U, 0x40000002U, 0x80010001U, 0x37545541U, 0x80000002U,
    0x80010008U, 0x9U, 0x40000003U, 0x80010001U, 0x30545541U, 0x80010009U, 0xfffffffbU, 0x8001000cU, 0x5U, 0x80010008U, 0x5U, 0x40000004U, 0x80010001U,
    0x35545541U, 0x80010009U, 0xfffffffbU, 0x8001000cU, 0x5U, 0x80010008U, 0x5U, 0x40000005U, 0x8006001aU, 0x5aU, 0x5bU, 0x6U, 0x7U, 0x5bU, 0x96U, 0x40000006U,
    0x8001000cU, 0x4U, 0x80010001U, 0x31545541U, 0x80000002U, 0x80010008U, 0x9U, 0x40000009U, 0x80050011U, 0x5f5a4953U, 0x0U, 0x0U, 0x0U, 0x0U, 0x80050011U,
    0x5f494153U, 0x0U, 0x0U, 0x0U, 0x0U, 0x80010019U, 0x44U, 0x80020014U, 0x38343161U, 0x5f4f5952U, 0x8001000eU, 0x1U, 0x40000007U, 0x80010001U, 0x32545541U,
    0x80010009U, 0xffffff88U, 0x8001000cU, 0x7U, 0x80010009U, 0xfffffffbU, 0x8001000cU, 0x8U, 0x80000002U, 0x80010008U, 0x8U, 0x40000008U, 0x8001000eU, 0x2U,
    0x400003e7U, 0x80010005U, 0x5f5a4953U, 0x80010005U, 0x5f494153U, 0x80010015U, 0x5f4b4e4bU, 0x8001000eU, 0x1U, 0x0U};
std::uint32_t voice[]{0x0U, 0x4U, 0x0U, 0x0U, 0x40000009U, 0x8001000dU, 0x1eU, 0x80010031U, 0x31a025U, 0x80000032U, 0x8001000dU, 0x28U, 0x80010031U, 0x31a026U,
    0x80000032U, 0x8001000dU, 0x14U, 0x8001000eU, 0x1U, 0x4000000aU, 0x8001000dU, 0x1eU, 0x80010031U, 0x31a027U, 0x80000032U, 0x8001000dU, 0x28U, 0x80010031U,
    0x31a028U, 0x80000032U, 0x8001000dU, 0x14U, 0x8001000eU, 0x1U, 0x4000000bU, 0x8001000dU, 0x1eU, 0x80010031U, 0x31a030U, 0x80000032U, 0x8001000dU, 0x23U,
    0x80010031U, 0x31a031U, 0x80000032U, 0x8001000dU, 0xfU, 0x80010031U, 0x31a032U, 0x80000032U, 0x8001000dU, 0x14U, 0x8001000eU, 0x1U, 0x40000008U,
    0x8001000dU, 0x1eU, 0x80010031U, 0x31a037U, 0x80000032U, 0x8001000dU, 0x23U, 0x80010031U, 0x31a001U, 0x80000032U, 0x8001000dU, 0x2dU, 0x80010031U,
    0x31a002U, 0x80000032U, 0x8001000dU, 0x14U, 0x8001000eU, 0x2U, 0x5f4f5953U, 0xffffffffU};
std::uint32_t doorOpen[]{0x0U, 0x0U, 0x1U, 0x0U, 0x5f414e41U, 0xbfaccccdU, 0x3fa28f5cU, 0x3f2147aeU, 0x0U, 0xfdddU, 0x0U, 0xffffffffU, 0x0U, 0x0U, 0x0U, 0x0U};
std::uint32_t doorStart[]{0x0U, 0x0U, 0x1U, 0x0U, 0x1eU, 0x2aaaU, 0x28U, 0x31c7U, 0x3cU, 0x31c7U, 0x78U, 0x31c7U, 0x0U, 0x31c7U, 0x0U, 0x31c7U};
std::uint32_t doorEnd[]{0x0U, 0x31c7U, 0x384U, 0x31c7U, 0x514U, 0xe38U, 0x51eU, 0x71cU, 0x528U, 0x444U, 0x532U, 0x222U, 0x53cU, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U};
std::uint32_t leftHand[]{0x7cfU, 0xff4aU, 0xf3ecU, 0x666U, 0xb4U, 0xf99cU, 0x222U, 0xfccfU, 0xd25U, 0x0U, 0xfd2aU, 0xee38U, 0x0U, 0xfc74U, 0xf1c5U, 0x0U, 0x0U,
    0xfb05U, 0x0U, 0x0U, 0xfc74U, 0x0U, 0x0U, 0xeaaaU, 0x0U, 0x0U, 0xe93eU, 0x0U, 0xfe94U, 0xf779U, 0x0U, 0x0U, 0xff4aU, 0x0U, 0x2d9U, 0xeeedU, 0x0U, 0xfe95U,
    0xe2d9U, 0x0U, 0xfe94U, 0xfbbcU, 0x0U, 0x0U, 0x0U, 0x0U, 0x223U, 0xdf49U, 0x0U, 0x667U, 0xeeedU, 0x0U, 0xfd2aU, 0xfa50U, 0xfa50U, 0xb6U, 0x0U};
std::uint32_t rightHand[]{0x0U, 0x2d6U, 0xf779U, 0x0U, 0xb6U, 0xff4aU, 0x0U, 0xe940U, 0xeedU, 0x0U, 0x0U, 0xf113U, 0x0U, 0x0U, 0xf4a0U, 0x0U, 0x0U, 0xfb07U,
    0x0U, 0x222U, 0x71cU, 0x0U, 0x0U, 0xf99aU, 0x0U, 0x0U, 0xeccfU, 0x0U, 0x0U, 0xfddeU, 0x0U, 0x222U, 0x71cU, 0x0U, 0x0U, 0x16cU, 0x0U, 0x0U, 0xeaacU, 0x0U,
    0x0U, 0x0U, 0x0U, 0x222U, 0x5afU, 0x0U, 0x0U, 0xf113U, 0x0U, 0x0U, 0xf335U, 0x0U, 0x0U, 0xb6U, 0x0U, 0x222U, 0x443U};
const WipeRecord records[]{
    {7, 2, 0, 0, 4, SHEN_NAME(WL00), 1600},
    {7, 3, 0, 0, 4, SHEN_NAME(WL01), 1601},
    {7, 9, 0, 0, 4, SHEN_NAME(WL02), 1602},
    {7, 10, 0, 0, 4, SHEN_NAME(WL03), 1603},
    {7, 11, 0, 0, 4, SHEN_NAME(WL04), 1604},
    {7, 14, 0, 0, 4, SHEN_NAME(WL06), 1606},
    {7, 15, 0, 0, 4, SHEN_NAME(WL07), 1607},
    {7, 16, 0, 0, 4, SHEN_NAME(WL08), 1608},
    {7, 17, 0, 0, 4, SHEN_NAME(WL09), 1609},
    {7, 19, 0, 0, 4, SHEN_NAME(WL10), 1610},
    {7, 20, 0, 0, 4, SHEN_NAME(WL11), 1611},
    {7, 21, 0, 0, 4, SHEN_NAME(WL12), 1612},
    {7, 22, 0, 0, 4, SHEN_NAME(WL13), 1613},
    {7, 23, 0, 0, 4, SHEN_NAME(WL14), 1614},
    {7, 24, 0, 0, 4, SHEN_NAME(WL15), 1615},
    {22, 1, 0, 32768, 5, SHEN_NAME(WL16), 1616},
    {22, 9, 0, 32768, 5, SHEN_NAME(WL17), 1617},
    {22, 10, 0, 32768, 5, SHEN_NAME(WL18), 1618},
    {22, 11, 0, 32768, 5, SHEN_NAME(WL19), 1619},
    {22, 14, 0, 32768, 5, SHEN_NAME(WL21), 1621},
    {22, 15, 0, 32768, 5, SHEN_NAME(WL22), 1622},
    {22, 16, 0, 32768, 5, SHEN_NAME(WL23), 1623},
    {22, 17, 0, 32768, 5, SHEN_NAME(WL24), 1624},
    {22, 19, 0, 32768, 5, SHEN_NAME(WL25), 1625},
    {22, 20, 0, 32768, 5, SHEN_NAME(WL26), 1626},
    {22, 21, 0, 32768, 5, SHEN_NAME(WL27), 1627},
    {22, 22, 0, 32768, 5, SHEN_NAME(WL28), 1628},
    {22, 23, 0, 32768, 5, SHEN_NAME(WL29), 1629},
    {22, 24, 0, 32768, 5, SHEN_NAME(WL30), 1630},
    {8, 25, 0, 49152, 6, SHEN_NAME(WL31), 1631},
    {9, 25, 0, 49152, 6, SHEN_NAME(WL32), 1632},
    {10, 25, 0, 49152, 6, SHEN_NAME(WL33), 1633},
    {11, 25, 0, 49152, 6, SHEN_NAME(WL34), 1634},
    {19, 25, 0, 49152, 6, SHEN_NAME(WL38), 1635},
    {20, 25, 0, 49152, 6, SHEN_NAME(WL39), 1636},
    {21, 25, 0, 49152, 6, SHEN_NAME(WL40), 1637},
    {1, 14, 0, 49152, 11, SHEN_NAME(CHR1), 1150},
    {1, 14, 0, 49152, 10, SHEN_NAME(CHR6), 1149},
    {1, 1, 0, 49152, 8, SHEN_NAME(CHR7), 1638},
    {1, 1, 0, 49152, 9, SHEN_NAME(CHR8), 1148},
    {0, 0, 0, 0, 0, shen_name{0x0U}, 0},
};
const ScaleRecord scales[]{
    {SHEN_NAME(FR00), std::bit_cast<float>(0x40000000U)},
    {SHEN_NAME(FR01), std::bit_cast<float>(0x40000000U)},
    {SHEN_NAME(FR02), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR03), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR04), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR05), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR06), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR07), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR08), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR09), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR0A), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR0B), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR0C), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR0D), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR0E), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR0F), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR10), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR11), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR12), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR13), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR14), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR15), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR16), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR17), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR18), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR19), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR1A), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR1B), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR1C), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR1D), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR1E), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR1F), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR20), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR21), std::bit_cast<float>(0x3f800000U)},
    {SHEN_NAME(FR22), std::bit_cast<float>(0x3f800000U)},
    {shen_name{0x0U}, std::bit_cast<float>(0x0U)},
};
std::int32_t doorTicks{}, doorYaw{}, doorYawStep{}, doorIndex{}, areaIndex{}, scaleTicks{};
std::uint32_t *doorTimeline{};
HLib::Task *doorTask{};
std::uint8_t admission{}, selectedArea{}, selectedKind{}, previouslyWiped{};
float wallAlpha{}, clothMotion{};
std::uint32_t smoke{};
std::uint32_t smokePhase{};
Vector3 previousSmoke{}, previousCloth{}, previousPlayer{};
std::array<std::uint32_t, 4> clothAngles{};
bool openingWipe{};
void Scenario(void *);
void ScenarioInit(void *);
void Door(void *);
void MoveCloth(void *);
void AddWallAlpha(void *);
void Face(void *);
void SuccessFirst(void *);
void SuccessSecond(void *);
std::int32_t Truncate(float value) {
    return _mm_cvttss_si32(_mm_set_ss(value));
}
Vector3 Position(shen_name name, std::uint32_t bone = CHARACTER_TRANSLATION_ROOT, std::uint32_t mode = 0) {
    Vector3 value{};
    EV_GetCharaPosition(name, bone, &value, mode);
    return value;
}
Vector3i Rotation(shen_name name) {
    Vector3i value{};
    GetScenarioCharacterEulerAngles(name, &value, EventTransformFlags::Local);
    return value;
}
void FadeIn(std::uint32_t ticks = 30) {
    SetScreenFilmFade(ticks, 255, 0, 0, 0, 0, 0, 0, 0);
}
void FadeOut(std::uint32_t ticks = 30) {
    SetScreenFilmFade(ticks, 0, 0, 0, 0, 255, 0, 0, 0);
}
void RecordStory(std::uint8_t operation) {
    EV_stepSetFlags(operation, EV_GetShenmueFlag(0x47) ? 0x47 : EV_GetShenmueFlag(0x25) ? 0x25 : 0x2e1);
}
// @verified - p
void RandomizeWall(std::int32_t index) {
    const WipeRecord &record = records[index];
    if (record.column == 1)
        return;
    Vector3 position{(record.column * .89999998f + 323.0f) + .44999999f, (GetRandomNumberInRange(.5f) + (record.row < 13 ? 129.7f : 128.8f)) - .30000001f,
        (record.row * .89999998f + 237.60001f) + .44999999f};
    Vector3i rotation{record.yaw, 0, 49152};
    const Vector3 scale{GetRandomNumberInRange(.5f) + .80000001f, GetRandomNumberInRange(.5f) + .80000001f, GetRandomNumberInRange(.5f) + .80000001f};
    if (record.yaw == 0 || record.yaw == 0x8000) {
        position.x = GetRandomNumberInRange(.001f) + (record.yaw ? 343.92001f : 329.53f);
        rotation.y = shenGetRandomInt32(65536);
    } else if (record.yaw == 49152) {
        position.z = GetRandomNumberInRange(.001f) + 260.66f;
        rotation.z = shenGetRandomInt32(65536);
    }
    EV_AT_SetPos(record.character, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(record.character, EventTransformFlags::AllAxes, rotation);
    SetMotionworkFlagForCharaById(record.character, &scale);
}
// @verified - p
void PrepareWalls() {
    for (std::int32_t i = 0; records[i].column; ++i) {
        const auto &record = records[i];
        const bool done = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(record.flag)) != 0;
        if (record.column != 1) {
            if (done) {
                SetScenarioCharacterTransparency(record.character, .5f);
                EV_ToggleCharacterFlag(record.character);
                EV_EnableCharacterDisplay(record.character);
            } else {
                EV_ToggleCharacterFlag(record.character);
                EV_DisableCharacterDisplay(record.character);
            }
            RandomizeWall(i);
        }
    }
}
// @verified - p
void UpdateScale() {
    scaleTicks = (scaleTicks + 1) % -24;
    for (const auto &record : scales) {
        if (!record.character.name)
            break;
        SetScenarioCharacterTransparency(record.character, GetRandomNumberInRange(.5f) + .5f);
    }
    if (EVT_GetControllerInput(1, 3)) {
        const float scale = EVT_GetControllerInput(1, 3) * .0078125f + 1.0f;
        EV_set_debug_text_pos(1, 6);
        EV_debug_log_to_screen("Scale:%4.2f", scale);
        for (const auto &record : scales) {
            if (!record.character.name)
                break;
            const Vector3 value{scale * record.scale, scale * record.scale, scale * record.scale};
            SetMotionworkFlagForCharaById(record.character, &value);
        }
    }
}
// @verified - p
void PrepareCloth() {
    SetLowQualityHands(FindCharacterByScenarioName(ryo));
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ryo), 0, reinterpret_cast<std::byte *>(leftHand), 10);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ryo), 1, reinterpret_cast<std::byte *>(rightHand), 10);
    EV_SetCharaControllerFlags(cloth, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
    const Vector3 position{.152f, 0, -.013f};
    const Vector3i rotation{0, 0, 49152};
    AttachScenarioCharacter(cloth, ryo, 14, &position, &rotation);
}
// @verified - p
void QueueDoor(std::uint32_t *timeline) {
    if (doorTask)
        HLib::DestroyTaskInternal(doorTask);
    doorTimeline = timeline;
    doorTicks = doorYawStep = 0;
    doorYaw = static_cast<std::int32_t>(timeline[1]);
    doorIndex = 1;
    doorTask = HLib::EnqueueTaskWithoutParameter(Door, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
}
// @verified - p
void Door(void *) {
    ++doorTicks;
    doorYaw += doorYawStep;
    Vector3i rotation{0, doorYaw, 0};
    EV_set_debug_text_pos(2, 20);
    EV_AT_SetAngY(SHEN_NAME(2DLG), EventTransformFlags::AllAxes, rotation);
    rotation.y = 0;
    EV_AT_SetAngY(SHEN_NAME(2DRG), EventTransformFlags::AllAxes, rotation);
    if (doorTicks >= static_cast<std::int32_t>(doorTimeline[2 * doorIndex])) {
        if (!doorTimeline[2 * (doorIndex + 1)]) {
            doorTask = nullptr;
            HLib::DestroyCurrentTask();
        }
        const std::int32_t old = doorIndex++;
        const float delta = static_cast<float>(static_cast<std::int32_t>(doorTimeline[2 * doorIndex + 1]) - doorYaw) /
            static_cast<float>(static_cast<std::int32_t>(doorTimeline[2 * doorIndex]) - static_cast<std::int32_t>(doorTimeline[2 * old]));
        doorYawStep = Truncate(delta);
    }
}
// @verified - p
void MoveCloth(void *) {
    Vector3 position = Position(cloth, CHARACTER_TRANSLATION_ROOT, 0x40000000U);
    const Vector3 delta{previousCloth.x - position.x, previousCloth.y - position.y, previousCloth.z - position.z};
    previousCloth = position;
    const float movement = std::fmin(.1f, (std::fabs(delta.y) + std::fabs(delta.x)) + std::fabs(delta.z));
    clothMotion *= .98000002f;
    if (movement > clothMotion)
        clothMotion = movement;
    constexpr std::array<std::uint32_t, 4> bases{3000, 3200, 3400, 3700}, factors{20000, 27000, 25000, 23000};
    const std::uint32_t truncatedMotion = static_cast<std::uint32_t>(Truncate(clothMotion));
    for (std::size_t i = 0; i < 4; ++i) {
        clothAngles[i] += bases[i] + factors[i] * truncatedMotion;
        Vector3i rotation;
        EV_scenApplyPosOrRotToPartOf(cloth, static_cast<std::uint16_t>(0x98 + i), &position, &rotation, EventCharacterPartTransformMode::Get);
        rotation.x = Truncate((clothMotion * 30000.0f + 200.0f) * sin_fixed16(static_cast<std::int16_t>(clothAngles[i])));
        EV_scenApplyPosOrRotToPartOf(cloth, static_cast<std::uint16_t>(0x98 + i), &position, &rotation, EventCharacterPartTransformMode::Absolute);
    }
}
// @verified - p
std::int32_t FindWall(const Vector3 &position) {
    float minimum = 9.8999996f;
    std::int32_t result = -1;
    Vector3 point{0, 0, .5f};
    const auto yaw = Rotation(ryo);
    ApplyScenarioTranslationAndRotation(&position, &yaw, &point);
    EV_set_debug_text_pos(0, 17);
    for (std::int32_t i = 0; records[i].column; ++i) {
        const auto &record = records[i];
        const float floor = record.row < 13 ? 127.88f : 126.98f;
        Vector3 wall = Position(record.character);
        const auto wallRotation = Rotation(record.character);
        (void)wallRotation;
        const float dx = wall.x - point.x, dz = wall.z - point.z;
        wall.y = point.y;
        const float distance = GetEuclideanDistance(&wall, &point);
        if (distance <= 1.8f) {
            std::uint16_t angle = static_cast<std::uint16_t>(yaw.y - InverseFixedAtan2(dx, dz));
            if (angle >= 0x8000)
                angle = static_cast<std::uint16_t>(-angle);
            if (angle < 0x3ddd && ryoNotBusy() && position.y > floor - .05f && floor + .05f > position.y && minimum > distance) {
                minimum = distance;
                result = i;
            }
        }
    }
    return result;
}
// @verified - p
void BeginWipe(WipeWork &work) {
    const Vector3 player = Position(ryo);
    Vector3 projected{(player.x - previousPlayer.x) * 5.0f + player.x, player.y, (player.z - previousPlayer.z) * 5.0f + player.z};
    previousPlayer = player;
    DateTime time{};
    GetCurrentDateTime(time);
    if (static_cast<std::int8_t>(time.hour) >= 20) {
        if (ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            work.state = 17;
            work.phase = 0;
        }
        if (srchHasActiveLockonTarget())
            srchRequestLockonExit();
    }
    if (static_cast<std::int8_t>(time.hour) >= 19 && EV_GetShenmueFlag(0x3f2)) {
        if (ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            work.state = 18;
            work.phase = 0;
        }
        if (srchHasActiveLockonTarget())
            srchRequestLockonExit();
    }
    SetPlayerGameFlags(PlayerGameFlagMode::Bit30, reinterpret_cast<void *>(1));
    if (!admission)
        areaIndex = FindWall(projected);
    if (areaIndex != -1) {
        Vector3 offset{0, .1f, 0};
        const Vector3 wall = Position(records[areaIndex].character);
        const auto rotation = Rotation(records[areaIndex].character);
        ApplyScenarioTranslationAndRotation(&wall, &rotation, &offset);
        GetBattleDataConfig(0, SHEN_NAME(BTBA));
    }
    if (GetBattleDataConfig(2, SHEN_NAME(BTBA)) && areaIndex != -1 && ryoNotBusy()) {
        admission = 1;
        SetPlayerControlEnabled(0);
        SetPlayerGameFlags(PlayerGameFlagMode::EnableAreaControl, nullptr);
    }
    if (admission != 2 && (admission != 1 || !SetPlayerGameFlags(PlayerGameFlagMode::QueryAreaControl, nullptr)))
        return;
    SetPlayerGameFlags(PlayerGameFlagMode::DisableAreaControl, nullptr);
    admission = 0;
    const auto &record = records[areaIndex];
    const std::int32_t kind = record.kind;
    SetPlayerControlEnabled(0);
    Vector3 target{0, .5f, 0};
    const Vector3 wall = Position(record.character);
    const auto rotation = Rotation(record.character);
    ApplyScenarioTranslationAndRotation(&wall, &rotation, &target);
    target.y = Position(ryo).y;
    work.area = areaIndex;
    selectedArea = static_cast<std::uint8_t>(areaIndex);
    selectedKind = static_cast<std::uint8_t>(kind);
    previouslyWiped = static_cast<std::uint8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(record.flag)));
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(record.flag), 1);
    std::int32_t yaw = 0;
    switch (kind) {
    case -11:
        target.x = 343.45001f;
        target.z = 253.28999f;
        yaw = 49152;
        break;
    case -10:
        target.z = 253.36f;
        target.x = 329.98001f;
        yaw = 16384;
        break;
    case 4:
    case 10:
        target.z += .18000001f;
        target.x = 329.98001f;
        yaw = 16384;
        break;
    case 5:
    case 11:
        target.z -= .2f;
        target.x = 343.45001f;
        yaw = 49152;
        break;
    case 6:
        target.z = 260.22f;
        yaw = 32768;
        break;
    case 8:
        target.z = 237.78999f;
        target.x = 333.85999f;
        break;
    case 9:
        target.z = 237.78999f;
        target.x = 339.01999f;
        break;
    default:
        break;
    }
    const auto playerRotation = Rotation(ryo);
    work.yawTicks = 0;
    work.reserved34 = 0;
    work.yawStep = static_cast<std::int16_t>(static_cast<std::int16_t>(yaw - playerRotation.y) / 30);
    Vector3i anchorRotation = playerRotation;
    anchorRotation.y = yaw;
    EV_AT_SetAngY(anchor, EventTransformFlags::AllAxes, anchorRotation);
    EV_AT_SetPos(anchor, EventTransformFlags::AllAxes, target);
    MotionWork *motion = scenFindMotionwork(ryo);
    mtwkSetRequestedRootMode(motion, 1);
    SetMotionFrameTime(motion, 18, MotionRegion::LowerBody);
    SetMotionSpeed(motion, 1, MotionRegion::LowerBody);
    CopyMotionTransition(motion, &target, static_cast<std::uint16_t>(yaw), 0, 0);
    SetMotionRegionFlag(motion, 69, 0, MotionRegion::LowerBody);
    SetMotionTimeRelated(motion, 1, 5, 5136);
    SetMotionRegionEvalTime(motion, 17, MotionRegion::LowerBody);
    if (kind == 8)
        work.finished = 1;
    work.kind = static_cast<std::int8_t>(kind);
    EVS_LoopCamera(2034, anchor, anchor);
    work.state = 9;
    work.phase = 0;
}
// @verified - p
void StartWipeScript(WipeWork &work) {
    switch (work.kind) {
    case 4:
    case 5:
    case 6:
        QueueWarehouseScriptLoop(wipe);
        SeekWarehouseScriptLabel(wipe, 11);
        break;
    case 8:
    case 9:
    case 10:
    case 11: {
        QueueWarehouseScriptLoop(wipe);
        SeekWarehouseScriptLabel(wipe, 10);
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(Face, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(zou_)));
        HLib::SetupTimedTask(task, 120);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1183, 1);
        break;
    }
    default:
        break;
    }
}
// @verified - p
void FinishWipe(WipeWork &work) {
    using enum CameraState;
    SetSmokeEffectFalloff(smoke, 31);
    switch (work.phase) {
    case 0:
        if (!mtwkSetExtraFlag(scenFindMotionwork(ryo), 58, 1))
            ++work.phase;
        break;
    case 1: {
        camCtrlSetFovTransitionTo(false);
        if (work.area >= 0) {
            if (!previouslyWiped) {
                SetScenarioCharacterTransparency(records[work.area].character, 0);
                EV_EnableCharacterDisplay(records[work.area].character);
            }
            Position(records[work.area].character);
            Rotation(records[work.area].character);
        }
        StartWipeScript(work);
        ++work.phase;
        work.frame = 0;
        wallAlpha = 0;
        smokePhase = 0;
        break;
    }
    case 2: {
        Vector3 position = Position(ryo, 14);
        position.y += .1f;
        const Vector3 zero{};
        SetParticleEffectPosition(0, smoke, &position, &zero);
        const Vector3 velocity{(position.x - previousSmoke.x) / 9.0f, (position.y - previousSmoke.y) / 9.0f, (position.z - previousSmoke.z) / 9.0f};
        SetParticleEffectVelocity(0, smoke, velocity, zero);
        previousSmoke = position;
        if (work.frame == 1 || work.frame == 86)
            SetParticleEffectBirthRate(0, smoke, 0);
        if (wipe[3]) {
            SetParticleEffectBirthRate(0, smoke, 0);
            ++work.phase;
        }
        if (work.frame == 71 && !previouslyWiped)
            HLib::EnqueueTaskWithoutParameter(AddWallAlpha, 4, 0x0b, 1601531770);
        break;
    }
    case 3:
        if (work.kind < 8 || previouslyWiped)
            work.phase = 6;
        else {
            Vector3 target = Position(records[work.area].character);
            UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &target, 1);
            QueueWarehouseScriptLoop(voice);
            SeekWarehouseScriptLabel(voice, static_cast<std::uint16_t>(work.kind));
            ++work.phase;
        }
        break;
    case 4:
        if (voice[3]) {
            if (work.finished) {
                work.state = 8;
                work.phase = 0;
            } else {
                QueueWarehouseScriptLoop(exitScript);
                ++work.line;
                SeekWarehouseScriptLabel(exitScript, static_cast<std::uint16_t>(work.line));
                ++work.phase;
            }
        }
        break;
    case 5:
        if (exitScript[3]) {
            SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ryo), 0, 10, .60000002f);
            UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
            ++work.phase;
        }
        break;
    case 6:
        if (work.finished)
            work.state = 8;
        else {
            camCtrlSetState(Init);
            if (work.kind < 8 || previouslyWiped) {
                EV_EnableCharacterDisplay(records[work.area].character);
                camCtrlSetFovTransitionTo(false);
                camCtrlUtil(CameraUtilityMode::RestorePrevious, 0);
                CameraUtilityParameters parameter{1032805417U, 0};
                camCtrlUtil(CameraUtilityMode::SetEaseFactor, reinterpret_cast<std::intptr_t>(&parameter));
            } else
                camCtrlSetFovTransitionTo(true);
            SetPlayerControlEnabled(1);
            EV_EventSetUpHelpIcon_0(0x44, 0);
            work.state = 7;
        }
        work.phase = 0;
        break;
    default:
        break;
    }
}
// @verified - p
void AddWallAlpha(void *) {
    EV_set_debug_text_pos(20, 10);
    SetScenarioCharacterTransparency(records[selectedArea].character, wallAlpha);
    if (wallAlpha > .5f)
        HLib::DestroyCurrentTask();
    wallAlpha += .029999999f;
}
// @verified - p
void Face(void *) {
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ryo), 1, 10, .60000002f);
    HLib::DestroyCurrentTask();
}
// @verified - p
void SuccessFirst(void *) {
    QueueExtraTelop("JJ", 99);
    QueueExtraTelop("JJ", 1);
    HLib::SetCurrentTaskTo(SuccessSecond);
    HLib::SetupTaskDelay(2);
}
// @verified - p
void SuccessSecond(void *) {
    EV_LoadCutsceneFromCurrentScene("0031A050");
    EV_LoadAudio("0031A050", 0);
    LoadWarehouseAuthorPackages();
}
// @verified - p
void ScenarioInit(void *data) {
    auto &work = *static_cast<WipeWork *>(data);
    EV_FreeAuthorPak(1, "0031");
    EV_LoadCutsceneFromCurrentScene("0031E020");
    EV_LoadAudio("0031E020", 1);
    EV_spmBeginSampleExecute();
    EV_SetCharaControllerFlags(cloth, 1, NpcCharacterControllerState::Disabled);
    ToggleFlagOnFieldEntity(SHEN_NAME(dor2), 1);
    smoke = static_cast<std::uint32_t>(CreateSmokeEffect(Vector3{337, 128, 257}, Vector3{}, 99, .05999999865889549f, shen_name{0}, 0));
    SetSmokeEffectFalloff(smoke, 35);
    SetSmokeEffectFadeFactor(smoke, 132);
    SetParticleEffectVelocity(0, smoke, Vector3{0, -.0039215689f, 0}, Vector3{0, .0039215689f, 0});
    SetParticleEffectColour(0, smoke, 0, 50, 50, 48);
    SetParticleEffectInitialScale(0, smoke, .16078431904315948f);
    SetParticleEffectSize(0, smoke, 1.5686274766921997f, 1.5686274766921997f);
    SetParticleEffectBirthRate(0, smoke, 0);
    ResumeParticleEffect(0, smoke);
    work.line = 0;
    work.state = work.phase = 0;
    HLib::SetCurrentTaskTo(Scenario);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SUS2"), 0);
    HLib::EnqueueTaskWithoutParameter(MoveCloth, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(zou_)));
    openingWipe = false;
}
// @verified - p
void Scenario(void *data) {
    using enum CameraState;
    auto &work = *static_cast<WipeWork *>(data);
    UpdateScale();
    ++work.frame;
    switch (work.state) {
    case 0:
        if (work.phase == 0) {
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1639)) {
                QueueWarehouseScriptLoop(mainScript);
                SeekWarehouseScriptLabel(mainScript, 999);
            } else {
                QueueDoor(doorStart);
                QueueWarehouseScriptLoop(mainScript);
                mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
                openingWipe = true;
            }
            ++work.phase;
        } else if (work.phase == 1) {
            SetAuthorStateFlag(33, 1);
            switch (mainScript[3]) {
            case 1:
                SetAuthorStateFlag(36, 1);
                EV_SetRefBoardParamsFor(ryo, 1);
                EV_SetRefBoardParamsFor(siz, 1);
                work.state = 5;
                work.phase = work.frame = 0;
                break;
            case 2:
                SetAuthorStateFlag(34, 1);
                RecordStory(1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1819, 1);
                work.state = 6;
                work.phase = work.frame = 0;
                break;
            case 4:
                QueueDoor(doorEnd);
                mainScript[3] = 0;
                break;
            case 5:
                EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
                mainScript[3] = 0;
                break;
            case 6:
                EV_SetCharaControllerFlags(siz, 2, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(siz, 3, NpcCharacterControllerState::Disabled);
                mainScript[3] = 0;
                break;
            case 7:
                spmFadeOut();
                mainScript[3] = 0;
                break;
            case 8:
                Loop3DSoundEffect(228);
                mainScript[3] = 0;
                break;
            default:
                break;
            }
        }
        break;
    case 5:
        if (work.phase == 0) {
            mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
            SetMotionFrameTime(scenFindMotionwork(ryo), 0, MotionRegion::LowerBody);
            FadeIn();
            if (openingWipe)
                EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, Vector3i{0, 49152, 0});
            mtwkRequestMotion(scenFindMotionwork(ryo), 0xf002, MotionRegion::LowerBody);
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            PrepareCloth();
            QueueDoor(doorOpen);
            ++work.phase;
            work.finished = 0;
        } else if (work.phase == 1) {
            SetPlayerControlEnabled(1);
            EV_EventSetUpHelpIcon_0(0x44, 0);
            work.state = 7;
            work.phase = work.frame = 0;
        }
        break;
    case 6:
        if (work.frame == 30) {
            work.state = 20;
            work.phase = work.frame = 0;
            EV_FreeAuthorPak(0, "0031");
            EV_CleanupMapControlTask();
            HLib::MarkCurrentTaskForCleanup();
            EV_scenQueueLoad(1, SHEN_NAME(WB00), 5);
        }
        break;
    case 7:
        if (work.frame == 0)
            EV_EventSetUpHelpIcon_0(0x44, 0);
        BeginWipe(work);
        StartWarehouseLockonSequence();
        break;
    case 8:
        if (work.phase == 0) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
            EV_stepSetFlags(0, 0x25);
            FadeOut();
            HLib::SetupTaskDelay(32);
            spmFadeOut();
            ++work.phase;
        } else if (work.phase == 1) {
            SetPlayerControlEnabled(0);
            work.state = 19;
            work.phase = work.frame = 0;
        }
        break;
    case 9:
        FinishWipe(work);
        break;
    case 16:
        work.state = 20;
        work.phase = work.frame = 0;
        EV_FreeAuthorPak(0, "0031");
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1819, 1);
        RecordStory(2);
        EV_scenQueueLoad(1, SHEN_NAME(WB00), 5);
        break;
    case 17:
        if (work.phase == 0) {
            SetMotionSpeed(scenFindMotionwork(ryo), 0, MotionRegion::LowerBody);
            FadeOut();
            HLib::SetupTaskDelay(30);
            ++work.phase;
        } else if (work.phase == 1) {
            if (IsAuthorStateFlagSet(35))
                EV_SetAuthorActiveTo(1);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(AUT6)));
            FadeIn();
            ++work.phase;
        } else if (work.phase == 2 && !EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            SetAuthorStateFlag(35, 1);
            spmFadeOut();
            HLib::SetupTaskDelay(30);
            work.state = 16;
            work.phase = work.frame = 0;
        }
        break;
    case 18:
        if (work.phase == 0) {
            SetMotionSpeed(scenFindMotionwork(ryo), 0, MotionRegion::LowerBody);
            FadeOut(60);
            HLib::SetupTaskDelay(60);
            ++work.phase;
        } else if (work.phase == 1) {
            ResetScenarioCharacterAttachment(cloth);
            EV_stepSetFlags(0, 0x3f2);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(HELP)));
            FadeIn();
            ++work.phase;
        } else if (work.phase == 2 && !EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            work.state = 7;
            work.phase = work.frame = 0;
            EV_AT_SetPos(siz, EventTransformFlags::AllAxes, Vector3{});
            FadeIn();
            SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
            SetMotionFrameTime(scenFindMotionwork(ryo), 0, MotionRegion::LowerBody);
            mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
            mtwkRequestMotion(scenFindMotionwork(ryo), 0xf002, MotionRegion::LowerBody);
            PrepareCloth();
            SetPlayerControlEnabled(1);
            HLib::SetupTaskDelay(30);
        }
        break;
    case 19:
        EV_FreeAuthorPak(0, "0031");
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        HLib::EnqueueTaskWithoutParameter(SuccessFirst, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(sap_)));
        break;
    default:
        break;
    }
}
// @verified - p
void Init() {
    doorTicks = doorYaw = doorYawStep = doorIndex = 0;
    doorTimeline = nullptr;
    doorTask = nullptr;
    admission = 0;
    previousSmoke = {};
    areaIndex = 0;
    smoke = smokePhase = 0;
    previousCloth = {};
    clothAngles = {};
    clothMotion = 0;
    previousPlayer = {};
    ResetWarehouseLockonSequence();
    scaleTicks = 0;
}
// @verified - p
void Main(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    FinishLoading();
    SetCharacterMapEventRegionEnabled(ryo, 99, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ScenarioInit, 4, 0x0b, 0x50, SHEN_NAME(EVNT));
    PrepareWalls();
    HLib::SetupTimedTask(task, 32);
}
} // namespace
const ScriptAreaDefinition areaWBSSScriptDefinition{.areaId = SHEN_NAME(WBSS), .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2::areas
