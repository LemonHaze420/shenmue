#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;

enum ConversationCameraDomain : std::uint8_t {
    kConvDomainSameAxis = 0,
    kConvDomainCrossAxis = 1,
    kConvDomainCrossAxisSpan = 2,
    kConvDomainDirect0 = 3,
    kConvDomainDirect1 = 4,
};

enum ConversationPathSpanType : std::uint8_t {
    kConvSpanNone = 0,
    kConvSpan00To25 = 1,
    kConvSpan25To50 = 2,
    kConvSpan50To75 = 3,
    kConvSpan75To100 = 4,
    kConvSpan00To75 = 5,
    kConvSpan25To100 = 6,
    kConvSpan00To50 = 7,
    kConvSpan25To75 = 8,
    kConvSpan50To100 = 9,
    kConvSpan00To100 = 10,
};

#pragma pack(push, 1)
struct ConversationCameraCandidate {
    std::uint8_t enabled;
    std::uint8_t domain;
    std::uint8_t index;
    std::uint8_t reserved;
};
static_assert(sizeof(ConversationCameraCandidate) == 4);

struct ConversationCameraPathSpan {
    std::uint8_t domain;
    std::uint8_t slot;
    std::uint8_t type;
};
static_assert(sizeof(ConversationCameraPathSpan) == 3);
#pragma pack(pop)

struct ConversationCameraAnchorPair {
    std::uint32_t anchorB;
    std::uint32_t anchorA;
};
static_assert(sizeof(ConversationCameraAnchorPair) == 8);

// One SCNF conversation cue. Terminal cues are two words long (size 2) and carry no actors.
struct ScntConversationCue {
    std::uint8_t size;
    std::uint8_t kind;
    std::uint8_t variant;
    std::uint8_t flags;
    float durationSeconds;
    std::int32_t actor0TaskDataOffset;
    std::int32_t actor1TaskDataOffset;
};
static_assert(sizeof(ScntConversationCue) == 0x10);

struct ConversationCameraSlotTable {
    std::array<std::uint8_t, 24> domains;
    std::array<std::uint8_t, 24> slots;
};
static_assert(sizeof(ConversationCameraSlotTable) == 0x30);

struct ConversationParticipantRestoreState {
    std::uint32_t hadMotmFlag2;
    std::uint32_t savedPoseEvalStage;
    std::uint32_t hadMapcBit0;
    std::uint32_t characterId;
};
static_assert(sizeof(ConversationParticipantRestoreState) == 0x10);

struct ConversationAuthorPoint {
    float distance;
    std::int32_t angle;
    float height;
};
static_assert(sizeof(ConversationAuthorPoint) == 0xc);

struct ConversationAuthorShot {
    std::int32_t flippedSign;
    std::int32_t fovStart;
    std::int32_t fovEnd;
    ConversationAuthorPoint eye;
    ConversationAuthorPoint target;
    ConversationAuthorPoint eyeEnd;
    ConversationAuthorPoint targetEnd;
};
static_assert(sizeof(ConversationAuthorShot) == 0x3c);

// Cross-axis authored shot pair: `closeRange` applies at 0.9 m anchor separation, `farRange` at 3.0 m.
struct ConversationAuthorSetup {
    ConversationAuthorShot closeRange;
    ConversationAuthorShot farRange;
};
static_assert(sizeof(ConversationAuthorSetup) == 0x78);

inline constexpr std::array<ConversationAuthorSetup, 32> kConversationAuthorSetups{{
    {.closeRange = {0, 10922, 10922, {2.87f, 45133, 0.21f}, {0.91f, 47672, -0.83f}, {1.63f, 41553, 0.4f}, {0.91f, 44593, -0.82f}},
        .farRange = {0, 10922, 10922, {2.87f, 45133, 0.21f}, {1.64f, 55296, -1.07f}, {1.63f, 41553, 0.4f}, {1.26f, 51712, -1.29f}}},
    {.closeRange = {0, 10922, 10922, {2.79f, 37180, -0.52f}, {0.63f, 41966, -0.54f}, {3.0f, 43833, -0.75f}, {0.63f, 51726, -0.54f}},
        .farRange = {0, 10922, 10922, {2.79f, 37180, -0.52f}, {0.9f, 53760, -0.64f}, {3.0f, 43833, -0.75f}, {1.56f, 59904, -0.64f}}},
    {.closeRange = {0, 10922, 10922, {1.69f, 46064, -0.52f}, {0.48f, 60416, -0.29f}, {2.82f, 39013, -0.79f}, {0.43f, 48160, -0.52f}},
        .farRange = {0, 10922, 10922, {1.96f, 45552, -0.52f}, {2.19f, 50688, 0.12f}, {2.82f, 39013, -0.79f}, {1.1f, 56352, -0.46f}}},
    {.closeRange = {0, 10922, 7281, {0.73f, 39391, 0.24f}, {0.6f, 43281, -0.34f}, {0.73f, 39379, 0.24f}, {0.6f, 41796, -0.19f}},
        .farRange = {0, 10922, 7281, {0.73f, 38400, 0.16f}, {1.41f, 45568, -0.76f}, {0.73f, 38400, 0.16f}, {1.41f, 40448, -0.57f}}},
    {.closeRange = {0, 5461, 10922, {0.63f, 38900, 0.08f}, {0.37f, 40053, 0.04f}, {0.76f, 42003, 0.0f}, {0.37f, 51870, -0.03f}},
        .farRange = {0, 5461, 10922, {0.63f, 36352, 0.1f}, {0.44f, 48125, -0.03f}, {0.76f, 40955, 0.06f}, {1.35f, 52736, -0.38f}}},
    {.closeRange = {0, 10922, 7281, {0.55f, 45217, -0.36f}, {0.3f, 52356, -0.07f}, {0.56f, 39689, 0.11f}, {0.3f, 44654, -0.03f}},
        .farRange = {0, 10922, 7281, {0.68f, 41984, -0.91f}, {1.5f, 44213, -0.49f}, {0.68f, 39424, 0.12f}, {1.5f, 45568, -0.18f}}},
    {.closeRange = {0, 10922, 7281, {0.53f, 39406, 0.47f}, {0.43f, 44058, -0.44f}, {0.52f, 39520, 0.03f}, {0.42f, 41477, -0.01f}},
        .farRange = {0, 10922, 7281, {0.55f, 40448, 0.32f}, {1.09f, 52405, -0.94f}, {0.52f, 37888, 0.09f}, {0.66f, 43701, -0.06f}}},
    {.closeRange = {0, 5461, 12743, {0.4f, 38678, 0.02f}, {0.8f, 37074, 0.12f}, {0.57f, 43885, 0.11f}, {0.66f, 47294, -0.11f}},
        .farRange = {0, 5461, 12743, {0.62f, 36864, 0.06f}, {0.8f, 39605, 0.09f}, {0.7f, 43520, 0.08f}, {1.58f, 52405, -0.33f}}},
    {.closeRange = {0, 5461, 10922, {0.49f, 44143, -0.23f}, {0.49f, 41565, 0.29f}, {0.66f, 44142, -0.25f}, {0.49f, 50450, 0.31f}},
        .farRange = {0, 5461, 10922, {0.49f, 43008, -0.23f}, {0.43f, 50869, 0.29f}, {1.14f, 43520, -0.48f}, {1.66f, 48640, 1.45f}}},
    {.closeRange = {0, 10922, 10922, {0.38f, 45570, -0.07f}, {0.61f, 45559, 0.36f}, {0.34f, 44544, 0.0f}, {0.48f, 46606, 0.21f}},
        .farRange = {0, 10922, 10922, {0.38f, 44546, -0.09f}, {1.09f, 52727, 0.97f}, {0.32f, 42496, 0.0f}, {1.16f, 46606, 0.21f}}},
    {.closeRange = {0, 10922, 9102, {0.34f, 43008, 0.06f}, {0.61f, 41976, -0.03f}, {0.31f, 41985, 0.06f}, {0.48f, 41964, 0.16f}},
        .farRange = {0, 10922, 9102, {0.34f, 43008, 0.06f}, {2.57f, 41976, -0.03f}, {0.31f, 41986, 0.06f}, {2.16f, 39916, 0.03f}}},
    {.closeRange = {0, 10922, 10922, {0.34f, 45056, 0.18f}, {0.61f, 46592, -0.25f}, {0.28f, 45067, 0.08f}, {0.63f, 43512, -0.03f}},
        .farRange = {0, 10922, 10922, {0.34f, 42496, 0.18f}, {1.53f, 44544, -0.99f}, {0.28f, 42507, 0.08f}, {1.74f, 41464, -0.03f}}},
    {.closeRange = {0, 10922, 10922, {1.0f, 40960, -0.16f}, {0.42f, 51200, -0.03f}, {1.06f, 45056, -0.34f}, {0.46f, 63488, -0.07f}},
        .farRange = {0, 10922, 10922, {1.41f, 42496, -0.49f}, {1.29f, 56320, 0.28f}, {1.93f, 44032, -0.69f}, {1.64f, 62464, -0.29f}}},
    {.closeRange = {0, 10922, 10922, {0.95f, 44032, 0.15f}, {0.6f, 50176, -0.28f}, {0.77f, 38400, 0.15f}, {0.59f, 42496, -0.27f}},
        .farRange = {0, 10922, 10922, {1.77f, 44032, -0.13f}, {1.44f, 54784, -0.47f}, {1.38f, 38400, -0.17f}, {1.24f, 47616, -0.21f}}},
    {.closeRange = {0, 10922, 10922, {1.16f, 39936, 0.35f}, {0.45f, 45056, -0.57f}, {1.04f, 46080, 0.25f}, {0.64f, 51200, -0.36f}},
        .farRange = {0, 10922, 10922, {1.16f, 39936, 0.4f}, {1.11f, 54784, -1.0f}, {2.22f, 45056, 0.24f}, {1.49f, 57856, -1.14f}}},
    {.closeRange = {0, 10922, 10922, {0.45f, 58550, 0.0f}, {1.23f, 62496, 0.18f}, {0.73f, 43024, 0.23f}, {0.83f, 44560, -0.42f}},
        .farRange = {0, 10922, 10922, {0.46f, 58550, 0.0f}, {4.0f, 62496, 0.46f}, {0.63f, 40976, 0.25f}, {1.19f, 51216, -0.86f}}},
    {.closeRange = {0, 10922, 10922, {0.48f, 39598, 0.06f}, {0.54f, 45568, 0.15f}, {1.6f, 61440, 0.04f}, {1.68f, 59392, -0.01f}},
        .farRange = {0, 10922, 10922, {0.67f, 40353, 0.03f}, {1.46f, 48640, 0.09f}, {3.82f, 62976, -0.04f}, {2.85f, 60416, -0.26f}}},
    {.closeRange = {0, 10922, 9102, {1.7f, 40448, 0.73f}, {0.5f, 51200, -0.63f}, {1.22f, 46592, 0.02f}, {0.5f, 55808, -0.11f}},
        .farRange = {0, 10922, 9102, {2.69f, 40960, 1.07f}, {1.13f, 55296, -1.17f}, {2.68f, 44544, -0.31f}, {1.55f, 56320, -0.44f}}},
    {.closeRange = {0, 10922, 7281, {1.54f, 46080, -0.73f}, {0.5f, 57856, -0.21f}, {0.58f, 40448, 0.02f}, {0.5f, 44032, 0.03f}},
        .farRange = {0, 10922, 7281, {2.05f, 46080, -1.12f}, {2.2f, 50688, 0.56f}, {0.58f, 38912, 0.02f}, {0.88f, 46080, 0.03f}}},
    {.closeRange = {0, 10922, 10922, {1.48f, 48128, -0.95f}, {0.53f, 57344, -1.68f}, {1.73f, 42496, -0.91f}, {0.48f, 50176, 0.04f}},
        .farRange = {0, 10922, 10922, {2.54f, 46592, -0.95f}, {2.13f, 50688, -2.74f}, {2.31f, 40960, -1.12f}, {1.43f, 50176, 0.32f}}},
    {.closeRange = {1, 10922, 10922, {0.88f, 43008, -0.02f}, {0.5f, 49152, -0.06f}, {0.76f, 22528, 0.37f}, {0.76f, 20480, -0.46f}},
        .farRange = {1, 10922, 10922, {1.24f, 43008, -0.11f}, {1.36f, 54784, -0.15f}, {1.36f, 22016, 0.86f}, {1.5f, 12800, -1.15f}}},
    {.closeRange = {1, 10922, 10922, {1.5f, 39424, 0.41f}, {1.23f, 41472, -0.93f}, {0.64f, 22528, 0.16f}, {1.12f, 21504, -0.28f}},
        .farRange = {1, 10922, 10922, {1.5f, 39424, 0.41f}, {1.23f, 47616, -1.21f}, {0.61f, 25600, 0.08f}, {1.71f, 21504, -0.09f}}},
    {.closeRange = {1, 10922, 10922, {0.38f, 40960, 0.01f}, {0.68f, 43520, 0.16f}, {0.47f, 21504, 0.0f}, {0.97f, 21504, 0.01f}},
        .farRange = {1, 10922, 10922, {0.38f, 40960, 0.01f}, {1.91f, 43520, 0.16f}, {0.35f, 24064, 0.05f}, {2.42f, 22528, 0.01f}}},
    {.closeRange = {0, 10922, 10922, {4.42f, 44511, -1.39f}, {0.6f, 53009, -0.28f}, {3.34f, 36819, 1.04f}, {0.6f, 43844, -0.8f}},
        .farRange = {0, 10922, 10922, {4.42f, 44511, -1.19f}, {3.1f, 47377, 0.25f}, {3.34f, 36307, 1.04f}, {1.14f, 41284, -1.61f}}},
    {.closeRange = {0, 10922, 10922, {5.08f, 48095, -0.39f}, {1.17f, 53521, -0.3f}, {2.36f, 36819, 0.09f}, {1.14f, 38724, -0.23f}},
        .farRange = {0, 10922, 10922, {5.08f, 48095, -0.39f}, {2.35f, 53521, -0.3f}, {1.79f, 38867, -0.36f}, {1.75f, 44356, -0.08f}}},
    {.closeRange = {0, 10922, 10922, {3.32f, 41951, -1.17f}, {1.34f, 45332, 0.04f}, {4.82f, 58835, 0.94f}, {0.81f, 61776, -0.36f}},
        .farRange = {0, 10922, 10922, {3.32f, 41951, -1.28f}, {1.74f, 49428, 0.25f}, {5.29f, 58835, 0.94f}, {2.05f, 60752, -1.0f}}},
    {.closeRange = {0, 10922, 10922, {1.06f, 46608, -0.25f}, {0.61f, 51254, 0.03f}, {0.69f, 45045, -0.14f}, {0.85f, 44533, 0.17f}},
        .farRange = {0, 10922, 10922, {1.15f, 44048, -0.42f}, {1.69f, 48694, 0.77f}, {0.74f, 43509, -0.14f}, {2.56f, 44533, 0.53f}}},
    {.closeRange = {0, 10922, 9102, {1.03f, 37888, -0.1f}, {0.74f, 41520, -0.09f}, {0.87f, 41984, -0.09f}, {0.6f, 45622, 0.1f}},
        .farRange = {0, 10922, 9102, {1.03f, 38400, -0.13f}, {1.3f, 48688, 0.04f}, {0.86f, 39424, -0.09f}, {1.48f, 43574, 0.43f}}},
    {.closeRange = {0, 10922, 9102, {0.89f, 39936, 0.13f}, {0.52f, 47207, -0.23f}, {0.76f, 43509, 0.39f}, {0.91f, 45126, -0.57f}},
        .farRange = {0, 10922, 9102, {0.7f, 40960, 0.17f}, {1.54f, 47207, -0.73f}, {1.17f, 42997, 0.55f}, {2.35f, 45126, -1.74f}}},
    {.closeRange = {0, 10922, 10922, {0.94f, 39321, 0.22f}, {0.65f, 43474, -0.42f}, {0.88f, 42963, 0.02f}, {0.6f, 48588, -0.05f}},
        .farRange = {0, 10922, 10922, {0.94f, 39319, 0.22f}, {1.94f, 42962, -1.03f}, {1.18f, 42962, 0.02f}, {1.69f, 49610, -0.05f}}},
    {.closeRange = {0, 10922, 10922, {0.52f, 44991, 0.0f}, {0.6f, 44977, -0.1f}, {0.73f, 39410, 0.01f}, {0.59f, 44492, 0.03f}},
        .farRange = {0, 10922, 10922, {0.52f, 43450, 0.0f}, {1.99f, 44977, 0.05f}, {0.73f, 39410, 0.01f}, {1.67f, 43972, 0.09f}}},
    {.closeRange = {0, 10922, 10922, {1.67f, 39920, -0.21f}, {0.6f, 45025, -0.24f}, {2.21f, 42964, 0.02f}, {0.59f, 48094, -0.66f}},
        .farRange = {0, 10922, 10922, {1.67f, 40884, -0.21f}, {2.2f, 45025, -0.24f}, {2.21f, 42964, 0.02f}, {2.07f, 48594, -1.05f}}},
}};

struct ConversationSameAxisShot {
    std::int32_t flippedSign;
    Vector3 eye;
    Vector3 target;
    Vector3 eyeEnd;
    Vector3 targetEnd;
};
static_assert(sizeof(ConversationSameAxisShot) == 0x34);


struct ConversationCameraSlotFilterEntry {
    std::uint8_t reserved;
    std::uint8_t domain;
    std::uint8_t index;
    std::uint8_t count;
};
static_assert(sizeof(ConversationCameraSlotFilterEntry) == 4);

struct ConversationCyclePolicy {
    std::uint8_t policy;
    std::uint32_t characterId;
};
static_assert(sizeof(ConversationCyclePolicy) == 8);

inline constexpr std::array<ConversationCyclePolicy, 289> kConvCamCycleFilterPolicyByCharaId{{
    {0, SHEN_NAME(AKIR)},
    {0, SHEN_NAME(SORY)},
    {0, SHEN_NAME(IWAO)},
    {0, SHEN_NAME(JAKR)},
    {0, SHEN_NAME(FUKU)},
    {0, SHEN_NAME(INE_)},
    {0, SHEN_NAME(KISY)},
    {0, SHEN_NAME(TAIJ)},
    {0, SHEN_NAME(CHAI)},
    {0, SHEN_NAME(KURA)},
    {0, SHEN_NAME(KURB)},
    {0, SHEN_NAME(KAME)},
    {0, SHEN_NAME(JIMY)},
    {0, SHEN_NAME(TONY)},
    {0, SHEN_NAME(SMTH)},
    {1, SHEN_NAME(AKMI)},
    {2, SHEN_NAME(YOKO)},
    {0, SHEN_NAME(YUKK)},
    {0, SHEN_NAME(SYZU)},
    {0, SHEN_NAME(JUKY)},
    {2, SHEN_NAME(OISI)},
    {0, SHEN_NAME(NAMS)},
    {0, SHEN_NAME(SETA)},
    {0, SHEN_NAME(MTUK)},
    {0, SHEN_NAME(HORS)},
    {1, SHEN_NAME(SNKC)},
    {0, SHEN_NAME(YOSE)},
    {0, SHEN_NAME(MRIG)},
    {0, SHEN_NAME(NITO)},
    {0, SHEN_NAME(TSUC)},
    {0, SHEN_NAME(HATO)},
    {0, SHEN_NAME(ITOI)},
    {1, SHEN_NAME(TURU)},
    {0, SHEN_NAME(KURI)},
    {1, SHEN_NAME(AOKI)},
    {1, SHEN_NAME(MADA)},
    {0, SHEN_NAME(ETKO)},
    {1, SHEN_NAME(BUSS)},
    {0, SHEN_NAME(TOM_)},
    {1, SHEN_NAME(HIRA)},
    {0, SHEN_NAME(HISA)},
    {1, SHEN_NAME(YURI)},
    {1, SHEN_NAME(NANS)},
    {0, SHEN_NAME(HRKO)},
    {0, SHEN_NAME(AKSK)},
    {0, SHEN_NAME(MRUA)},
    {2, SHEN_NAME(HOND)},
    {1, SHEN_NAME(ONRR)},
    {1, SHEN_NAME(TATM)},
    {1, SHEN_NAME(KYUR)},
    {1, SHEN_NAME(UNO_)},
    {1, SHEN_NAME(KYOH)},
    {0, SHEN_NAME(ASDA)},
    {0, SHEN_NAME(YAMA)},
    {0, SHEN_NAME(MISM)},
    {0, SHEN_NAME(KOND)},
    {1, SHEN_NAME(RNKT)},
    {0, SHEN_NAME(NORK)},
    {2, SHEN_NAME(YIWA)},
    {0, SHEN_NAME(YNHS)},
    {0, SHEN_NAME(KKBN)},
    {1, SHEN_NAME(YOPA)},
    {0, SHEN_NAME(CATM)},
    {2, SHEN_NAME(KJIY)},
    {2, SHEN_NAME(TSHP)},
    {0, SHEN_NAME(EIIU)},
    {1, SHEN_NAME(PEDR)},
    {1, SHEN_NAME(PAUR)},
    {0, SHEN_NAME(TOM1)},
    {1, SHEN_NAME(KEBA)},
    {1, SHEN_NAME(KEBB)},
    {1, SHEN_NAME(KEBC)},
    {1, SHEN_NAME(KEBD)},
    {2, SHEN_NAME(STP1)},
    {1, SHEN_NAME(SAGA)},
    {0, SHEN_NAME(MSYU)},
    {0, SHEN_NAME(RYBI)},
    {1, SHEN_NAME(KYAS)},
    {0, SHEN_NAME(SIIN)},
    {0, SHEN_NAME(SKGK)},
    {1, SHEN_NAME(TKNN)},
    {1, SHEN_NAME(KIM_)},
    {1, SHEN_NAME(KOKA)},
    {1, SHEN_NAME(MIKM)},
    {1, SHEN_NAME(NSMR)},
    {2, SHEN_NAME(TKYM)},
    {1, SHEN_NAME(MTUR)},
    {1, SHEN_NAME(EDST)},
    {0, SHEN_NAME(MYKN)},
    {0, SHEN_NAME(TMNY)},
    {0, SHEN_NAME(MYTY)},
    {1, SHEN_NAME(HROT)},
    {1, SHEN_NAME(KYKW)},
    {2, SHEN_NAME(MWAK)},
    {0, SHEN_NAME(MKII)},
    {0, SHEN_NAME(HREO)},
    {0, SHEN_NAME(SCKO)},
    {1, SHEN_NAME(EIKK)},
    {1, SHEN_NAME(KUKT)},
    {0, SHEN_NAME(JOHN)},
    {0, SHEN_NAME(CMAL)},
    {1, SHEN_NAME(SATM)},
    {1, SHEN_NAME(BIKA)},
    {1, SHEN_NAME(BIKB)},
    {1, SHEN_NAME(TMMR)},
    {1, SHEN_NAME(MEYS)},
    {1, SHEN_NAME(TYHG)},
    {1, SHEN_NAME(KJHS)},
    {1, SHEN_NAME(TYMK)},
    {2, SHEN_NAME(FSMI)},
    {2, SHEN_NAME(FSMF)},
    {1, SHEN_NAME(SIND)},
    {0, SHEN_NAME(MASR)},
    {0, SHEN_NAME(ONO_)},
    {1, SHEN_NAME(MIKI)},
    {1, SHEN_NAME(HATR)},
    {1, SHEN_NAME(YAMO)},
    {1, SHEN_NAME(NOMR)},
    {0, SHEN_NAME(MEGM)},
    {0, SHEN_NAME(MAYM)},
    {1, SHEN_NAME(YKDM)},
    {1, SHEN_NAME(YOS1)},
    {1, SHEN_NAME(YOS2)},
    {1, SHEN_NAME(YOB1)},
    {1, SHEN_NAME(YOB2)},
    {1, SHEN_NAME(YJJI)},
    {1, SHEN_NAME(YBBA)},
    {1, SHEN_NAME(SUMI)},
    {0, SHEN_NAME(KOMN)},
    {0, SHEN_NAME(SAJO)},
    {2, SHEN_NAME(KOTD)},
    {1, SHEN_NAME(SKMT)},
    {0, SHEN_NAME(KOTA)},
    {0, SHEN_NAME(YASU)},
    {0, SHEN_NAME(KAYO)},
    {2, SHEN_NAME(KTEL)},
    {2, SHEN_NAME(JTEL)},
    {2, SHEN_NAME(TTEL)},
    {2, SHEN_NAME(BTEL)},
    {2, SHEN_NAME(TTYA)},
    {0, SHEN_NAME(SOBA)},
    {0, SHEN_NAME(ASOU)},
    {0, SHEN_NAME(HINA)},
    {2, SHEN_NAME(TKNB)},
    {0, SHEN_NAME(TOSK)},
    {1, SHEN_NAME(SERA)},
    {1, SHEN_NAME(HMRO)},
    {1, SHEN_NAME(BSJM)},
    {0, SHEN_NAME(BOB_)},
    {1, SHEN_NAME(FKSM)},
    {0, SHEN_NAME(KJMA)},
    {0, SHEN_NAME(NRSK)},
    {1, SHEN_NAME(ASNO)},
    {1, SHEN_NAME(NAKA)},
    {0, SHEN_NAME(NMKI)},
    {0, SHEN_NAME(YMGC)},
    {1, SHEN_NAME(KOGA)},
    {0, SHEN_NAME(TMRA)},
    {1, SHEN_NAME(MITA)},
    {2, SHEN_NAME(YAYI)},
    {1, SHEN_NAME(HTSI)},
    {0, SHEN_NAME(JOE_)},
    {0, SHEN_NAME(SAKI)},
    {2, SHEN_NAME(MIHO)},
    {1, SHEN_NAME(ENDO)},
    {1, SHEN_NAME(YOSI)},
    {1, SHEN_NAME(TKSA)},
    {0, SHEN_NAME(TAGW)},
    {0, SHEN_NAME(RYKO)},
    {2, SHEN_NAME(IRIE)},
    {1, SHEN_NAME(KURT)},
    {1, SHEN_NAME(HANA)},
    {1, SHEN_NAME(SIMZ)},
    {2, SHEN_NAME(TLBK)},
    {0, SHEN_NAME(DICK)},
    {0, SHEN_NAME(MARK)},
    {0, SHEN_NAME(RBRT)},
    {0, SHEN_NAME(HARY)},
    {0, SHEN_NAME(JONZ)},
    {0, SHEN_NAME(TAKI)},
    {0, SHEN_NAME(HIRI)},
    {0, SHEN_NAME(TAKE)},
    {0, SHEN_NAME(AKTG)},
    {1, SHEN_NAME(TOKI)},
    {1, SHEN_NAME(KNJI)},
    {0, SHEN_NAME(RIKA)},
    {1, SHEN_NAME(STSI)},
    {0, SHEN_NAME(GORO)},
    {2, SHEN_NAME(1DOR)},
    {1, SHEN_NAME(TDSI)},
    {1, SHEN_NAME(HRNO)},
    {2, SHEN_NAME(RDOR)},
    {1, SHEN_NAME(KUDO)},
    {1, SHEN_NAME(MURI)},
    {0, SHEN_NAME(FLD1)},
    {0, SHEN_NAME(FLD2)},
    {0, SHEN_NAME(FLD3)},
    {0, SHEN_NAME(FLD4)},
    {0, SHEN_NAME(FLD5)},
    {0, SHEN_NAME(FLD6)},
    {0, SHEN_NAME(FLD7)},
    {0, SHEN_NAME(FLD8)},
    {0, SHEN_NAME(FLD9)},
    {0, SHEN_NAME(FLDA)},
    {0, SHEN_NAME(FLDB)},
    {0, SHEN_NAME(FLDC)},
    {0, SHEN_NAME(FLDD)},
    {1, SHEN_NAME(KEBE)},
    {0, SHEN_NAME(KEBF)},
    {0, SHEN_NAME(KEBG)},
    {1, SHEN_NAME(KEBH)},
    {1, SHEN_NAME(KEBI)},
    {1, SHEN_NAME(KEBJ)},
    {0, SHEN_NAME(KEBK)},
    {1, SHEN_NAME(KEBL)},
    {1, SHEN_NAME(YOHI)},
    {1, SHEN_NAME(NGAI)},
    {0, SHEN_NAME(ITOH)},
    {0, SHEN_NAME(SKRD)},
    {1, SHEN_NAME(YJIH)},
    {0, SHEN_NAME(TJMA)},
    {0, SHEN_NAME(JONO)},
    {1, SHEN_NAME(UDGW)},
    {0, SHEN_NAME(OKYS)},
    {1, SHEN_NAME(ISDA)},
    {1, SHEN_NAME(MURA)},
    {0, SHEN_NAME(SNGA)},
    {1, SHEN_NAME(MITI)},
    {1, SHEN_NAME(MSTA)},
    {0, SHEN_NAME(MRSK)},
    {0, SHEN_NAME(MNWA)},
    {0, SHEN_NAME(HTNK)},
    {0, SHEN_NAME(HRSK)},
    {1, SHEN_NAME(NMNO)},
    {1, SHEN_NAME(ISYM)},
    {1, SHEN_NAME(KYMA)},
    {1, SHEN_NAME(ENKI)},
    {1, SHEN_NAME(NGSM)},
    {1, SHEN_NAME(TKHS)},
    {1, SHEN_NAME(HIDE)},
    {1, SHEN_NAME(KTRO)},
    {1, SHEN_NAME(KENI)},
    {1, SHEN_NAME(YMST)},
    {1, SHEN_NAME(SNDO)},
    {1, SHEN_NAME(NMTO)},
    {1, SHEN_NAME(KWMT)},
    {1, SHEN_NAME(TAEN)},
    {1, SHEN_NAME(KYSN)},
    {1, SHEN_NAME(SATO)},
    {1, SHEN_NAME(YSKT)},
    {1, SHEN_NAME(STAK)},
    {1, SHEN_NAME(IZWA)},
    {2, SHEN_NAME(LDOR)},
    {1, SHEN_NAME(SGRH)},
    {1, SHEN_NAME(MTRI)},
    {0, SHEN_NAME(TERY)},
    {1, SHEN_NAME(YKHI)},
    {2, SHEN_NAME(DTRY)},
    {2, SHEN_NAME(2DOR)},
    {2, SHEN_NAME(YDOR)},
    {2, SHEN_NAME(RINS)},
    {1, SHEN_NAME(ECHO)},
    {1, SHEN_NAME(KDOR)},
    {1, SHEN_NAME(RKKI)},
    {1, SHEN_NAME(GRKN)},
    {1, SHEN_NAME(MAGO)},
    {0, SHEN_NAME(CHRL)},
    {2, SHEN_NAME(ADOR)},
    {0, SHEN_NAME(MORN)},
    {0, SHEN_NAME(HDEI)},
    {0, SHEN_NAME(TEYI)},
    {1, SHEN_NAME(TOMH)},
    {0, SHEN_NAME(FLDE)},
    {1, SHEN_NAME(FLDF)},
    {0, SHEN_NAME(FLDG)},
    {1, SHEN_NAME(TMHN)},
    {1, SHEN_NAME(DJUN)},
    {2, SHEN_NAME(HMON)},
    {2, SHEN_NAME(MDOR)},
    {0, SHEN_NAME(GJHM)},
    {0, SHEN_NAME(GJBM)},
    {0, SHEN_NAME(GJHF)},
    {0, SHEN_NAME(GJBF)},
    {1, SHEN_NAME(YUMM)},
    {1, SHEN_NAME(SYKU)},
    {1, SHEN_NAME(HTSK)},
    {1, SHEN_NAME(SMIK)},
    {1, SHEN_NAME(SNJY)},
    {2, SHEN_NAME(GRYU)},
}};


struct ConversationCameraState {
    Character *cachedRyo;
    Character *selectedOtherParticipant;
    Character *cueParticipant0;
    Character *cueParticipant1;
    Character *anchorEntityB;
    Character *anchorEntityA;
    std::uint32_t ryoCharaId;
    std::uint32_t selectedOtherCharaId;
    std::uint32_t anchorCharaIdB;
    std::uint32_t anchorCharaIdA;
    std::uint8_t initialCueAnchorOverride;
    std::uint8_t cueStartCount;
    std::int16_t activeCueDurationFrames30Hz;
    std::int16_t activeCueBlendFrame;
    std::int16_t totalConversationDurationFrames30Hz;
    std::int16_t transitionStepCounter;
    std::uint8_t reserved04a[2];
    float elapsedCueFrames30Hz;
    float activeCuePathAlpha;
    std::int8_t mirrorSign;
    std::uint8_t mirrorSecondaryEnabled;
    std::int8_t sideFlipSign;
    std::uint8_t reserved057;
    std::int16_t anchorBSmoothingProgressFrames;
    std::int16_t anchorBSmoothingStartFrame;
    std::uint8_t anchorBSmoothingActive;
    std::uint8_t reserved05d;
    std::int16_t anchorASmoothingProgressFrames;
    std::int16_t anchorASmoothingStartFrame;
    std::uint8_t anchorASmoothingActive;
    std::uint8_t autoFovWideningActive;
    std::uint32_t autoFovWidenAngle16;
    std::int32_t conversationFovAngle16;
    Vector3 smoothedAnchorB;
    Vector3 smoothedAnchorA;
    Vector3 reserved084;
    Vector3 reserved090;
    Vector3 previousAnchorB;
    Vector3 previousAnchorA;
    Vector3 anchorBCorrectionRef;
    Vector3 anchorACorrectionRef;
    std::uint8_t anchorCorrectionHoldFlags;
    std::uint8_t reserved0cd[3];
    float anchorCorrectionMagnitude;
    float anchorCorrectionScale;
    float cameraClearanceScale;
    float anchorDistanceXZ;
    float anchorHeightDelta;
    std::uint8_t targetCollisionLockActive;
    std::uint8_t reserved0e5[3];
    Vector3 targetCollisionLockPos;
    ConversationCameraSlotTable cueCameraSlotTable;
    std::uint8_t primarySlotCursor;
    std::uint8_t secondarySlotCursor;
    std::uint8_t specialSlotCursor;
    std::uint8_t cameraShotCounter;
    std::uint8_t nonPrimaryCameraDomainStreak;
    std::uint8_t reserved129[3];
    std::uint8_t invertedCueSetup;
    std::uint8_t initialSideSelectionFlag;
    std::uint8_t inversionBaseCueIndex;
    std::array<std::uint8_t, 24> cueDoorInteractionFlags;
    std::uint8_t doorInteractionTransitionArmed;
    std::int16_t doorInteractionReadyFrameCount;
    std::int16_t conversationFrameCounter;
    std::int16_t conversationDeltaFrameCounter;
    std::int16_t reserved14e;
    float transitionEaseYaw;
    Vector3 interactionCueLocation;
    Vector3 transitionStartEye;
    Vector3 transitionStartTarget;
    std::int32_t transitionStartFovAngle16;
    Vector3 initialRyoWorldPos;
    std::int32_t initialRyoYaw;
    std::uint8_t compactedLeadCueBiasPending;
    std::uint8_t singleParticipantFallbackActive;
    std::uint8_t reserved18e;
    std::uint8_t reserved18f;
    std::int32_t scriptedCameraSlotOverride;
    std::uint8_t cueEndReached;
    std::uint8_t dynamicInteractionCameraActive;
    std::uint8_t reserved196[2];
    ScntConversationCue *conversationCueLoadPtr;
    std::array<ScntConversationCue *, 24> conversationCues;
    std::uint8_t conversationCueCount;
    std::uint8_t participantCount;
    std::uint8_t activeCueIndex;
    std::uint8_t timelineCueIndex;
    std::uint8_t preserveInitialParticipantOrder;
    std::uint8_t conversationCameraOverrideActive;
    std::uint8_t suppressCameraApply;
    std::uint8_t reserved267;
    std::array<Character *, 5> participantEntities;
    std::array<ConversationParticipantRestoreState, 5> participantRestore;
};
static_assert(offsetof(ConversationCameraState, initialCueAnchorOverride) == 0x40);
static_assert(offsetof(ConversationCameraState, elapsedCueFrames30Hz) == 0x4c);
static_assert(offsetof(ConversationCameraState, mirrorSign) == 0x54);
static_assert(offsetof(ConversationCameraState, anchorBSmoothingProgressFrames) == 0x58);
static_assert(offsetof(ConversationCameraState, anchorASmoothingProgressFrames) == 0x5e);
static_assert(offsetof(ConversationCameraState, autoFovWidenAngle16) == 0x64);
static_assert(offsetof(ConversationCameraState, smoothedAnchorB) == 0x6c);
static_assert(offsetof(ConversationCameraState, anchorCorrectionHoldFlags) == 0xcc);
static_assert(offsetof(ConversationCameraState, targetCollisionLockActive) == 0xe4);
static_assert(offsetof(ConversationCameraState, cueCameraSlotTable) == 0xf4);
static_assert(offsetof(ConversationCameraState, primarySlotCursor) == 0x124);
static_assert(offsetof(ConversationCameraState, invertedCueSetup) == 0x12c);
static_assert(offsetof(ConversationCameraState, cueDoorInteractionFlags) == 0x12f);
static_assert(offsetof(ConversationCameraState, doorInteractionReadyFrameCount) == 0x148);
static_assert(offsetof(ConversationCameraState, transitionEaseYaw) == 0x150);
static_assert(offsetof(ConversationCameraState, interactionCueLocation) == 0x154);
static_assert(offsetof(ConversationCameraState, transitionStartEye) == 0x160);
static_assert(offsetof(ConversationCameraState, transitionStartFovAngle16) == 0x178);
static_assert(offsetof(ConversationCameraState, initialRyoYaw) == 0x188);
static_assert(offsetof(ConversationCameraState, scriptedCameraSlotOverride) == 0x190);
static_assert(offsetof(ConversationCameraState, conversationCueLoadPtr) == 0x198);
static_assert(offsetof(ConversationCameraState, conversationCueCount) == 0x260);
static_assert(offsetof(ConversationCameraState, participantEntities) == 0x268);
static_assert(offsetof(ConversationCameraState, participantRestore) == 0x290);
static_assert(sizeof(ConversationCameraState) == 0x2e0);

inline constexpr std::size_t kConvPrimaryCandidateCount = 16;
inline constexpr std::size_t kConvCandidateCount = 48;

extern ConversationCameraState g_Conv;
extern std::array<ConversationCameraCandidate, 4> g_recentConvCamSelections;

extern std::array<ConversationCameraCandidate, kConvCandidateCount> g_convCamCandidates;
extern std::array<ConversationCameraPathSpan, 32> g_secondarySlotPathSpanEntries;
extern std::array<ConversationCameraAnchorPair, 3> g_convActiveConversation;
extern std::array<ConversationCameraCandidate, kConvCandidateCount> g_enabledConvCamSlotCandidates;
extern std::int8_t g_enabledConvCamCandidateCount;
extern std::int8_t g_enabledPrimaryConvCamCandidateCount;
extern std::int8_t g_enabledSecondaryConvCamCandidateCount;
extern std::uint8_t g_recentConvCamSelectionWriteCursor;
extern std::uint8_t g_recentConvCamSelectionReadCursor;
extern std::uint8_t g_recentConvCamAnchorPairWriteCursor;
extern std::uint8_t g_recentConvCamAnchorPairReadCursor;
extern std::uint8_t g_convCamMode;
extern std::int32_t g_conversationCueParticipantSign;


extern std::array<ScntConversationCue, 24> g_scntConversationCueWorkbuf;
extern std::array<ScntConversationCue, 16> g_scntConversationCueBuffer;
extern ScntConversationCue g_tmpConversationCue;
extern Character *g_conversationParticipantA;
extern Character *g_conversationParticipantB;
extern std::uint8_t g_shouldComputeBetweenTwoEntities;

void convCamInit();
void scntStartConversationCamera();
void convUpdateCameraPos();
void convEndConversationCamera();
void convUpdateCharaSeqVisibility();
void convInitStates(std::int32_t state, Character *participantA, Character *participantB, float mirrorSign, float durationFrames30Hz);
void HandleDynamicInteractionState();

inline constexpr std::array<ConversationSameAxisShot, 16> kConversationSameAxisShots{{
    {.flippedSign = 0, .eye = {-0.26f, 0.16f, -0.47f}, .target = {0.12f, 0.04f, 0.06f}, .eyeEnd = {-0.23f, -0.02f, -0.47f}, .targetEnd = {0.13f, 0.1f, 0.06f}},
    {.flippedSign = 0, .eye = {-0.24f, 0.01f, -0.61f}, .target = {0.1f, 0.03f, 0.05f}, .eyeEnd = {-0.18f, 0.02f, -0.47f}, .targetEnd = {0.11f, 0.09f, 0.04f}},
    {.flippedSign = 0, .eye = {-0.18f, 0.02f, -0.46f}, .target = {0.04f, 0.08f, -0.07f}, .eyeEnd = {-0.3f, 0.07f, -0.63f}, .targetEnd = {0.05f, 0.03f, -0.06f}},
    {.flippedSign = 0, .eye = {-0.39f, -0.04f, -0.54f}, .target = {0.11f, 0.05f, -0.01f}, .eyeEnd = {-0.21f, 0.09f, -0.62f}, .targetEnd = {0.1f, 0.02f, 0.02f}},
    {.flippedSign = 1,
        .eye = {-0.29f, -0.05f, -0.46f},
        .target = {0.02f, 0.07f, -0.11f},
        .eyeEnd = {0.39f, -0.07f, -0.44f},
        .targetEnd = {-0.03f, 0.06f, -0.09f}},
    {.flippedSign = 0,
        .eye = {-0.34f, 0.18f, -0.31f},
        .target = {0.07f, 0.08f, -0.03f},
        .eyeEnd = {-0.19f, 0.12f, -0.41f},
        .targetEnd = {0.06f, 0.09f, -0.02f}},
    {.flippedSign = 0, .eye = {-0.16f, 0.07f, -0.4f}, .target = {0.01f, 0.1f, -0.09f}, .eyeEnd = {-0.27f, 0.08f, -0.39f}, .targetEnd = {0.05f, 0.09f, -0.07f}},
    {.flippedSign = 0,
        .eye = {-0.21f, 0.07f, -0.41f},
        .target = {0.04f, 0.09f, -0.02f},
        .eyeEnd = {-0.24f, -0.07f, -0.35f},
        .targetEnd = {0.05f, 0.1f, -0.07f}},
    {.flippedSign = 0, .eye = {-0.19f, 0.0f, -0.42f}, .target = {0.02f, 0.09f, -0.04f}, .eyeEnd = {-0.08f, 0.16f, -0.44f}, .targetEnd = {0.03f, 0.08f, 0.03f}},
    {.flippedSign = 1,
        .eye = {-0.45f, -0.19f, -0.57f},
        .target = {0.07f, 0.02f, -0.03f},
        .eyeEnd = {0.42f, 0.23f, -0.52f},
        .targetEnd = {-0.06f, 0.06f, -0.12f}},
    {.flippedSign = 0,
        .eye = {-0.53f, 0.19f, -0.53f},
        .target = {0.02f, 0.01f, -0.06f},
        .eyeEnd = {-0.25f, -0.19f, -0.77f},
        .targetEnd = {0.02f, -0.01f, -0.05f}},
    {.flippedSign = 0,
        .eye = {-0.2f, 0.09f, -0.61f},
        .target = {-0.02f, 0.04f, -0.03f},
        .eyeEnd = {-0.43f, -0.14f, -0.58f},
        .targetEnd = {0.07f, 0.02f, -0.09f}},
    {.flippedSign = 0,
        .eye = {-0.55f, 0.22f, -0.62f},
        .target = {-0.01f, -0.02f, -0.09f},
        .eyeEnd = {-0.31f, 0.17f, -0.81f},
        .targetEnd = {-0.02f, -0.04f, -0.01f}},
    {.flippedSign = 0, .eye = {-0.5f, -0.19f, -0.62f}, .target = {0.0f, 0.0f, -0.08f}, .eyeEnd = {-0.24f, 0.0f, -0.74f}, .targetEnd = {0.0f, -0.01f, -0.05f}},
    {.flippedSign = 0,
        .eye = {-0.52f, -0.27f, -0.53f},
        .target = {0.08f, 0.01f, -0.03f},
        .eyeEnd = {-0.38f, -0.14f, -0.52f},
        .targetEnd = {0.07f, 0.03f, -0.02f}},
    {.flippedSign = 0,
        .eye = {-0.51f, -0.07f, -0.55f},
        .target = {-0.01f, 0.02f, -0.09f},
        .eyeEnd = {-0.31f, 0.05f, -0.71f},
        .targetEnd = {0.0f, -0.01f, -0.05f}},
}};

} // namespace shenmue::s1
