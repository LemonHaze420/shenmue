#pragma once

#include "common/types.hpp"
#include "s2/date_time.hpp"
#include "s2/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct PadControlEntry;
struct SnapshotMetadataRecord;

inline constexpr shen_name SNAP_TASK{SHEN_NAME(SNAP)};
inline constexpr shen_name SNAP_CHARACTER_ID_FILE{SHEN_NAME(SNCF)};
inline constexpr std::int32_t SNAP_ALBUM_CAPACITY = 126;

inline constexpr std::int32_t SNAP_ALBUM_FULL_WARNING = 23;
inline constexpr std::uint32_t SNAP_SHUTTER_COMMAND = 0xcab;

struct SnapshotSaveWork {
    std::uint32_t slot;
    std::uint32_t rowPitch;
    std::uint32_t width;
    std::uint32_t height;
    std::byte *pixels;
};
static_assert(sizeof(SnapshotSaveWork) == 0x18);
static_assert(offsetof(SnapshotSaveWork, pixels) == 0x10);

std::int32_t snapResetMetadataRecord(SnapshotMetadataRecord &record);



enum class SnapState : std::int32_t { Idle, Allocate, Capture, Save, Result, Hold, WaitContext, Restore, Retake };


#pragma pack(push, 1)
struct SnapControlWork {
    HLib::Task *task;
    std::int32_t mode; // 1 disables capture on the title/OMAK/viewer maps
    SnapState state;
    std::array<std::byte, 8> reserved10;
    PadControlEntry *padControl;
    float delay;
    std::int32_t captureCountdown;
    std::int32_t warning;
    std::int32_t warningValue;
    std::int32_t presentationRestored;
    std::int32_t reserved34;
    void *characterIdData;
    std::int32_t descriptionSet; // 1 keeps the event's description: no SnapChrID lock-on
    std::int32_t descriptionSetDelay;
    std::int32_t descriptionClearDelay;
    std::int32_t specialAreaFrames;
    std::uint8_t watchSuppressed;
    std::array<std::byte, 3> reserved51;
    std::int32_t naviEnabled;
    std::int32_t snapshotOption;
    std::int32_t battleFlags;
    std::int32_t colouredText;
    std::int32_t reserved64;
    PadControlEntry *previousPadControl;
    DateTime dateTime;
    std::array<std::byte, 5> reserved77;
    std::int32_t fightGaugeVisible;
    std::int32_t wkktGaugeVisible;
    std::int32_t duckRaceOverlayDisabled;
    std::uint64_t reserved88;
    std::array<std::byte, 12> reserved90;
    std::int32_t reserved9c;
};
#pragma pack(pop)
static_assert(sizeof(SnapControlWork) == 0xa0);
static_assert(offsetof(SnapControlWork, state) == 0x0c);
static_assert(offsetof(SnapControlWork, padControl) == 0x18);
static_assert(offsetof(SnapControlWork, delay) == 0x20);
static_assert(offsetof(SnapControlWork, characterIdData) == 0x38);
static_assert(offsetof(SnapControlWork, descriptionSet) == 0x40);
static_assert(offsetof(SnapControlWork, specialAreaFrames) == 0x4c);
static_assert(offsetof(SnapControlWork, watchSuppressed) == 0x50);
static_assert(offsetof(SnapControlWork, snapshotOption) == 0x58);
static_assert(offsetof(SnapControlWork, previousPadControl) == 0x68);
static_assert(offsetof(SnapControlWork, dateTime) == 0x70);
static_assert(offsetof(SnapControlWork, fightGaugeVisible) == 0x7c);
static_assert(offsetof(SnapControlWork, reserved9c) == 0x9c);



#pragma pack(push, 1)
struct SnapshotCharacterIdRecord {
    shen_name afs;
    std::uint32_t reserved04;
    shen_name area;
    std::int32_t sortRule;
    std::array<shen_name, 7> characterIds;
    float radius;
};
#pragma pack(pop)
static_assert(sizeof(SnapshotCharacterIdRecord) == 0x30);
static_assert(offsetof(SnapshotCharacterIdRecord, characterIds) == 0x10);
static_assert(offsetof(SnapshotCharacterIdRecord, radius) == 0x2c);



struct SnapshotAuthoredRule {
    std::int32_t type;
    std::int32_t firstFrame;
    std::int32_t lastFrame;
    shen_name lockOn;
    shen_name area;
    std::uint32_t reserved14;
    const char *afs;
    std::uint64_t reserved20;
};
static_assert(sizeof(SnapshotAuthoredRule) == 40);
struct SnapshotAuthoredSequence {
    shen_name name;
    std::uint32_t reserved04;
    const SnapshotAuthoredRule *rules;
};
static_assert(sizeof(SnapshotAuthoredSequence) == 16);
struct SnapshotAuthoredMap {
    shen_name map;
    std::int32_t count;
    const SnapshotAuthoredSequence *const *sequences;
    const void *reserved10;
};
static_assert(sizeof(SnapshotAuthoredMap) == 24);

inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0017_0{{{.type = 5, .firstFrame = 0, .lastFrame = 1030, .area = SHEN_NAME(WS00)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0017_0{.name = 0, .rules = SNAPSHOT_RULES_0017_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0017_1{{{.type = 5, .firstFrame = 0, .lastFrame = 1030, .area = SHEN_NAME(AR02)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0017_1{.name = 1, .rules = SNAPSHOT_RULES_0017_1.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0017_2{{{.type = 5, .firstFrame = 0, .lastFrame = 1390, .area = SHEN_NAME(AK00)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0017_2{.name = 2, .rules = SNAPSHOT_RULES_0017_2.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 3> SNAPSHOT_SEQUENCES_0017{
    &SNAPSHOT_SEQUENCE_0017_0, &SNAPSHOT_SEQUENCE_0017_1, &SNAPSHOT_SEQUENCE_0017_2};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0034_0{{{.type = 5, .firstFrame = 0, .lastFrame = 1870, .area = SHEN_NAME(WB01)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0034_0{.name = 1, .rules = SNAPSHOT_RULES_0034_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_0034{&SNAPSHOT_SEQUENCE_0034_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0039_0{{{.type = 5, .firstFrame = 0, .lastFrame = 2050, .area = SHEN_NAME(WSY0)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0039_0{.name = 1, .rules = SNAPSHOT_RULES_0039_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0039_1{{{.type = 5, .firstFrame = 0, .lastFrame = 2385, .area = SHEN_NAME(WTA0)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0039_1{.name = 5, .rules = SNAPSHOT_RULES_0039_1.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 2> SNAPSHOT_SEQUENCES_0039{&SNAPSHOT_SEQUENCE_0039_0, &SNAPSHOT_SEQUENCE_0039_1};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0048_0{{{.type = 1, .firstFrame = 30, .lastFrame = 305, .area = SHEN_NAME(WN00)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0048_0{.name = 2, .rules = SNAPSHOT_RULES_0048_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_0048{&SNAPSHOT_SEQUENCE_0048_0};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_0240_0{
    {{.type = 1, .firstFrame = 1796, .lastFrame = 1901}, {.type = 1, .firstFrame = 1128, .lastFrame = 1222}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0240_0{.name = 0, .rules = SNAPSHOT_RULES_0240_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_0240{&SNAPSHOT_SEQUENCE_0240_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0276_0{{{.type = 2, .firstFrame = 1786, .lastFrame = 1835, .lockOn = SHEN_NAME(JOP_)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0276_0{.name = 2, .rules = SNAPSHOT_RULES_0276_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_0276{&SNAPSHOT_SEQUENCE_0276_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_0549_0{{{.type = 5, .firstFrame = 840, .lastFrame = 1930, .area = SHEN_NAME(BADK)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_0549_0{.name = 1, .rules = SNAPSHOT_RULES_0549_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_0549{&SNAPSHOT_SEQUENCE_0549_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_AB00_0{{{.type = 5, .firstFrame = 0, .lastFrame = 1230, .area = SHEN_NAME(AR30)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_AB00_0{.name = SHEN_NAME(175B), .rules = SNAPSHOT_RULES_AB00_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_AB00{&SNAPSHOT_SEQUENCE_AB00_0};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_ABLB_0{{{.type = 5, .firstFrame = 1771, .lastFrame = 2070, .area = SHEN_NAME(AB00)},
    {.type = 5, .firstFrame = 0, .lastFrame = 1770, .area = SHEN_NAME(AR30)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_ABLB_0{.name = 1, .rules = SNAPSHOT_RULES_ABLB_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_ABLB_1{{{.type = 5, .firstFrame = 0, .lastFrame = 2800, .area = SHEN_NAME(AB00)}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_ABLB_1{.name = 2, .rules = SNAPSHOT_RULES_ABLB_1.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 2> SNAPSHOT_SEQUENCES_ABLB{&SNAPSHOT_SEQUENCE_ABLB_0, &SNAPSHOT_SEQUENCE_ABLB_1};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_CB00_0{
    {{.type = 1, .firstFrame = 70, .lastFrame = 107}, {.type = 1, .firstFrame = 16, .lastFrame = 34}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_0{.name = SHEN_NAME(0000), .rules = SNAPSHOT_RULES_CB00_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_CB00_1{
    {{.type = 1, .firstFrame = 80, .lastFrame = 101}, {.type = 1, .firstFrame = 31, .lastFrame = 65}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_1{.name = SHEN_NAME(0001), .rules = SNAPSHOT_RULES_CB00_1.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_CB00_2{{{.type = 1, .firstFrame = 1, .lastFrame = 39}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_2{.name = SHEN_NAME(0002), .rules = SNAPSHOT_RULES_CB00_2.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_CB00_3{{{.type = 1, .firstFrame = 1, .lastFrame = 39}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_3{.name = SHEN_NAME(0003), .rules = SNAPSHOT_RULES_CB00_3.data()};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_CB00_4{
    {{.type = 1, .firstFrame = 146, .lastFrame = 180}, {.type = 1, .firstFrame = 29, .lastFrame = 45}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_4{.name = SHEN_NAME(0007), .rules = SNAPSHOT_RULES_CB00_4.data()};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_CB00_5{
    {{.type = 1, .firstFrame = 173, .lastFrame = 208}, {.type = 1, .firstFrame = 13, .lastFrame = 48}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_5{.name = SHEN_NAME(0008), .rules = SNAPSHOT_RULES_CB00_5.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_CB00_6{{{.type = 1, .firstFrame = 26, .lastFrame = 26}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_6{.name = SHEN_NAME(0016), .rules = SNAPSHOT_RULES_CB00_6.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_CB00_7{{{.type = 1, .firstFrame = 218, .lastFrame = 333}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_7{.name = SHEN_NAME(0017), .rules = SNAPSHOT_RULES_CB00_7.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_CB00_8{{{.type = 1, .firstFrame = 905, .lastFrame = 1055}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_CB00_8{.name = SHEN_NAME(0018), .rules = SNAPSHOT_RULES_CB00_8.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 9> SNAPSHOT_SEQUENCES_CB00{&SNAPSHOT_SEQUENCE_CB00_0, &SNAPSHOT_SEQUENCE_CB00_1,
    &SNAPSHOT_SEQUENCE_CB00_2, &SNAPSHOT_SEQUENCE_CB00_3, &SNAPSHOT_SEQUENCE_CB00_4, &SNAPSHOT_SEQUENCE_CB00_5, &SNAPSHOT_SEQUENCE_CB00_6,
    &SNAPSHOT_SEQUENCE_CB00_7, &SNAPSHOT_SEQUENCE_CB00_8};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_KMZ3_0{
    {{.type = 1, .firstFrame = 420, .lastFrame = 480}, {.type = 1, .firstFrame = 214, .lastFrame = 355}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_KMZ3_0{.name = SHEN_NAME(AT01), .rules = SNAPSHOT_RULES_KMZ3_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_KMZ3{&SNAPSHOT_SEQUENCE_KMZ3_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_KRK6_0{{{.type = 1, .firstFrame = 26, .lastFrame = 47}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_KRK6_0{.name = SHEN_NAME(QTE1), .rules = SNAPSHOT_RULES_KRK6_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_KRK6{&SNAPSHOT_SEQUENCE_KRK6_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_KSH1_0{{{.type = 1, .firstFrame = 222, .lastFrame = 257}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_KSH1_0{.name = SHEN_NAME(A282), .rules = SNAPSHOT_RULES_KSH1_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_KSH1{&SNAPSHOT_SEQUENCE_KSH1_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QA00_0{
    {{.type = 2, .firstFrame = 77, .lastFrame = 463, .lockOn = SHEN_NAME(REN_), .afs = "0210"}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QA00_0{.name = SHEN_NAME(AUT1), .rules = SNAPSHOT_RULES_QA00_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QA00_1{
    {{.type = 2, .firstFrame = 88, .lastFrame = 366, .lockOn = SHEN_NAME(REN_), .afs = "0210"}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QA00_1{.name = SHEN_NAME(AUT2), .rules = SNAPSHOT_RULES_QA00_1.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QA00_2{{{.type = 1, .firstFrame = 123, .lastFrame = 132, .afs = "0210"}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QA00_2{.name = SHEN_NAME(AUT4), .rules = SNAPSHOT_RULES_QA00_2.data()};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_QA00_3{
    {{.type = 1, .firstFrame = 690, .lastFrame = 743, .afs = "0210"}, {.type = 1, .firstFrame = 0, .lastFrame = 229, .afs = "0210"}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QA00_3{.name = SHEN_NAME(AUT7), .rules = SNAPSHOT_RULES_QA00_3.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QA00_4{{{.type = 1, .firstFrame = 0, .lastFrame = 60, .afs = "0214"}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QA00_4{.name = SHEN_NAME(AUTH), .rules = SNAPSHOT_RULES_QA00_4.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 5> SNAPSHOT_SEQUENCES_QA00{
    &SNAPSHOT_SEQUENCE_QA00_0, &SNAPSHOT_SEQUENCE_QA00_1, &SNAPSHOT_SEQUENCE_QA00_2, &SNAPSHOT_SEQUENCE_QA00_3, &SNAPSHOT_SEQUENCE_QA00_4};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QAXX_0{{{.type = 1, .firstFrame = 859, .lastFrame = 904}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QAXX_0{.name = SHEN_NAME(AUT2), .rules = SNAPSHOT_RULES_QAXX_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_QAXX_1{
    {{.type = 1, .firstFrame = 260, .lastFrame = 491}, {.type = 1, .firstFrame = 0, .lastFrame = 87}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QAXX_1{.name = SHEN_NAME(AUT3), .rules = SNAPSHOT_RULES_QAXX_1.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 2> SNAPSHOT_SEQUENCES_QAXX{&SNAPSHOT_SEQUENCE_QAXX_0, &SNAPSHOT_SEQUENCE_QAXX_1};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QE00_0{{{.type = 1, .firstFrame = 560, .lastFrame = 620}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QE00_0{.name = SHEN_NAME(OMOT), .rules = SNAPSHOT_RULES_QE00_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QE00_1{{{.type = 1, .firstFrame = 661, .lastFrame = 720}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QE00_1{.name = SHEN_NAME(TOUT), .rules = SNAPSHOT_RULES_QE00_1.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QE00_2{{{.type = 1, .firstFrame = 560, .lastFrame = 620}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QE00_2{.name = SHEN_NAME(URA_), .rules = SNAPSHOT_RULES_QE00_2.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 3> SNAPSHOT_SEQUENCES_QE00{
    &SNAPSHOT_SEQUENCE_QE00_0, &SNAPSHOT_SEQUENCE_QE00_1, &SNAPSHOT_SEQUENCE_QE00_2};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QEDJ_0{{{.type = 1, .firstFrame = 1260, .lastFrame = 1270}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QEDJ_0{.name = SHEN_NAME(A020), .rules = SNAPSHOT_RULES_QEDJ_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QEDJ_1{{{.type = 1, .firstFrame = 379, .lastFrame = 490}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QEDJ_1{.name = SHEN_NAME(A030), .rules = SNAPSHOT_RULES_QEDJ_1.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QEDJ_2{{{.type = 1, .firstFrame = 62, .lastFrame = 124}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QEDJ_2{.name = 1000, .rules = SNAPSHOT_RULES_QEDJ_2.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QEDJ_3{{{.type = 1, .firstFrame = 0, .lastFrame = 88}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QEDJ_3{.name = 7210, .rules = SNAPSHOT_RULES_QEDJ_3.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 4> SNAPSHOT_SEQUENCES_QEDJ{
    &SNAPSHOT_SEQUENCE_QEDJ_0, &SNAPSHOT_SEQUENCE_QEDJ_1, &SNAPSHOT_SEQUENCE_QEDJ_2, &SNAPSHOT_SEQUENCE_QEDJ_3};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_QF00_0{
    {{.type = 1, .firstFrame = 2130, .lastFrame = 2320}, {.type = 1, .firstFrame = 144, .lastFrame = 215}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QF00_0{.name = SHEN_NAME(BTL_), .rules = SNAPSHOT_RULES_QF00_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QF00_1{{{.type = 1, .firstFrame = 0, .lastFrame = 740}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QF00_1{.name = 7422, .rules = SNAPSHOT_RULES_QF00_1.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QF00_2{{{.type = 1, .firstFrame = 16, .lastFrame = 150}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QF00_2{.name = 7426, .rules = SNAPSHOT_RULES_QF00_2.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QF00_3{{{.type = 1, .firstFrame = 371, .lastFrame = 416}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QF00_3{.name = SHEN_NAME(KYA0), .rules = SNAPSHOT_RULES_QF00_3.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QF00_4{{{.type = 1, .firstFrame = 151, .lastFrame = 192}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QF00_4{.name = SHEN_NAME(KYA1), .rules = SNAPSHOT_RULES_QF00_4.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QF00_5{{{.type = 1, .firstFrame = 29, .lastFrame = 76}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QF00_5{.name = SHEN_NAME(KYA2), .rules = SNAPSHOT_RULES_QF00_5.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_QF00_6{{{.type = 1, .firstFrame = 36, .lastFrame = 40}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QF00_6{.name = SHEN_NAME(QTE_), .rules = SNAPSHOT_RULES_QF00_6.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 7> SNAPSHOT_SEQUENCES_QF00{&SNAPSHOT_SEQUENCE_QF00_0, &SNAPSHOT_SEQUENCE_QF00_1,
    &SNAPSHOT_SEQUENCE_QF00_2, &SNAPSHOT_SEQUENCE_QF00_3, &SNAPSHOT_SEQUENCE_QF00_4, &SNAPSHOT_SEQUENCE_QF00_5, &SNAPSHOT_SEQUENCE_QF00_6};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_QLBT_0{
    {{.type = 1, .firstFrame = 1986, .lastFrame = 2030}, {.type = 1, .firstFrame = 0, .lastFrame = 180}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_QLBT_0{.name = SHEN_NAME(AU_0), .rules = SNAPSHOT_RULES_QLBT_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_QLBT{&SNAPSHOT_SEQUENCE_QLBT_0};
inline constexpr std::array<SnapshotAuthoredRule, 5> SNAPSHOT_RULES_SYW0_0{{{.type = 1, .firstFrame = 1431, .lastFrame = 1530},
    {.type = 1, .firstFrame = 447, .lastFrame = 468}, {.type = 1, .firstFrame = 318, .lastFrame = 330}, {.type = 1, .firstFrame = 61, .lastFrame = 66}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_SYW0_0{.name = SHEN_NAME(AU_2), .rules = SNAPSHOT_RULES_SYW0_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_SYW0{&SNAPSHOT_SEQUENCE_SYW0_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_TOFB_0{{{.type = 1, .firstFrame = 209, .lastFrame = 250}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_TOFB_0{.name = SHEN_NAME(ATH0), .rules = SNAPSHOT_RULES_TOFB_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_TOFB{&SNAPSHOT_SEQUENCE_TOFB_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_WB00_0{{{.type = 1, .firstFrame = 2004, .lastFrame = 2070}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_WB00_0{.name = SHEN_NAME(0173), .rules = SNAPSHOT_RULES_WB00_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_WB00{&SNAPSHOT_SEQUENCE_WB00_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_WB01_0{{{.type = 1, .firstFrame = 29, .lastFrame = 76}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_WB01_0{.name = SHEN_NAME(GI__), .rules = SNAPSHOT_RULES_WB01_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_WB01{&SNAPSHOT_SEQUENCE_WB01_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_WBBK_0{{{.type = 1, .firstFrame = 0, .lastFrame = 109}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_WBBK_0{.name = SHEN_NAME(AU02), .rules = SNAPSHOT_RULES_WBBK_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_WBBK{&SNAPSHOT_SEQUENCE_WBBK_0};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_WK00_0{{{.type = 1, .firstFrame = 45, .lastFrame = 143, .afs = "0026"}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_WK00_0{.name = SHEN_NAME(WAZA), .rules = SNAPSHOT_RULES_WK00_0.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 1> SNAPSHOT_SEQUENCES_WK00{&SNAPSHOT_SEQUENCE_WK00_0};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_WTA0_0{
    {{.type = 1, .firstFrame = 2380, .lastFrame = 2500}, {.type = 1, .firstFrame = 89, .lastFrame = 160}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_WTA0_0{.name = SHEN_NAME(0034), .rules = SNAPSHOT_RULES_WTA0_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_WTA0_1{{{.type = 1, .firstFrame = 1535, .lastFrame = 2690}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_WTA0_1{.name = SHEN_NAME(0036), .rules = SNAPSHOT_RULES_WTA0_1.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 2> SNAPSHOT_SEQUENCES_WTA0{&SNAPSHOT_SEQUENCE_WTA0_0, &SNAPSHOT_SEQUENCE_WTA0_1};
inline constexpr std::array<SnapshotAuthoredRule, 3> SNAPSHOT_RULES_YTQB_0{
    {{.type = 1, .firstFrame = 993, .lastFrame = 1036}, {.type = 1, .firstFrame = 413, .lastFrame = 536}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_YTQB_0{.name = SHEN_NAME(ATH4), .rules = SNAPSHOT_RULES_YTQB_0.data()};
inline constexpr std::array<SnapshotAuthoredRule, 2> SNAPSHOT_RULES_YTQB_1{{{.type = 1, .firstFrame = 0, .lastFrame = 465}, {}}};
inline constexpr SnapshotAuthoredSequence SNAPSHOT_SEQUENCE_YTQB_1{.name = SHEN_NAME(ATH5), .rules = SNAPSHOT_RULES_YTQB_1.data()};
inline constexpr std::array<const SnapshotAuthoredSequence *, 2> SNAPSHOT_SEQUENCES_YTQB{&SNAPSHOT_SEQUENCE_YTQB_0, &SNAPSHOT_SEQUENCE_YTQB_1};

inline constexpr std::array<SnapshotAuthoredMap, 27> SNAPSHOT_AUTHORED_MAPS{{
    {.map = SHEN_NAME(0017), .count = 3, .sequences = SNAPSHOT_SEQUENCES_0017.data()},
    {.map = SHEN_NAME(0034), .count = 1, .sequences = SNAPSHOT_SEQUENCES_0034.data()},
    {.map = SHEN_NAME(0039), .count = 2, .sequences = SNAPSHOT_SEQUENCES_0039.data()},
    {.map = SHEN_NAME(0048), .count = 1, .sequences = SNAPSHOT_SEQUENCES_0048.data()},
    {.map = SHEN_NAME(0240), .count = 1, .sequences = SNAPSHOT_SEQUENCES_0240.data()},
    {.map = SHEN_NAME(0276), .count = 1, .sequences = SNAPSHOT_SEQUENCES_0276.data()},
    {.map = SHEN_NAME(0549), .count = 1, .sequences = SNAPSHOT_SEQUENCES_0549.data()},
    {.map = SHEN_NAME(AB00), .count = 1, .sequences = SNAPSHOT_SEQUENCES_AB00.data()},
    {.map = SHEN_NAME(ABLB), .count = 2, .sequences = SNAPSHOT_SEQUENCES_ABLB.data()},
    {.map = SHEN_NAME(CB00), .count = 9, .sequences = SNAPSHOT_SEQUENCES_CB00.data()},
    {.map = SHEN_NAME(KMZ3), .count = 1, .sequences = SNAPSHOT_SEQUENCES_KMZ3.data()},
    {.map = SHEN_NAME(KRK6), .count = 1, .sequences = SNAPSHOT_SEQUENCES_KRK6.data()},
    {.map = SHEN_NAME(KSH1), .count = 1, .sequences = SNAPSHOT_SEQUENCES_KSH1.data()},
    {.map = SHEN_NAME(QA00), .count = 5, .sequences = SNAPSHOT_SEQUENCES_QA00.data()},
    {.map = SHEN_NAME(QAXX), .count = 2, .sequences = SNAPSHOT_SEQUENCES_QAXX.data()},
    {.map = SHEN_NAME(QE00), .count = 3, .sequences = SNAPSHOT_SEQUENCES_QE00.data()},
    {.map = SHEN_NAME(QEDJ), .count = 4, .sequences = SNAPSHOT_SEQUENCES_QEDJ.data()},
    {.map = SHEN_NAME(QF00), .count = 7, .sequences = SNAPSHOT_SEQUENCES_QF00.data()},
    {.map = SHEN_NAME(QLBT), .count = 1, .sequences = SNAPSHOT_SEQUENCES_QLBT.data()},
    {.map = SHEN_NAME(SYW0), .count = 1, .sequences = SNAPSHOT_SEQUENCES_SYW0.data()},
    {.map = SHEN_NAME(TOFB), .count = 1, .sequences = SNAPSHOT_SEQUENCES_TOFB.data()},
    {.map = SHEN_NAME(WB00), .count = 1, .sequences = SNAPSHOT_SEQUENCES_WB00.data()},
    {.map = SHEN_NAME(WB01), .count = 1, .sequences = SNAPSHOT_SEQUENCES_WB01.data()},
    {.map = SHEN_NAME(WBBK), .count = 1, .sequences = SNAPSHOT_SEQUENCES_WBBK.data()},
    {.map = SHEN_NAME(WK00), .count = 1, .sequences = SNAPSHOT_SEQUENCES_WK00.data()},
    {.map = SHEN_NAME(WTA0), .count = 2, .sequences = SNAPSHOT_SEQUENCES_WTA0.data()},
    {.map = SHEN_NAME(YTQB), .count = 2, .sequences = SNAPSHOT_SEQUENCES_YTQB.data()},
}};

extern SnapControlWork *g_snapTaskData;

extern std::uint32_t g_snapshotTexture2D;


void SetScreenDimensions(std::uint32_t width, std::uint32_t height);
void CreateTextureCopy();
void CopyTempResourceGpu();


void Queue_SNAP_Control();
void SNAP_Control_Callback(void *parameter);
void snapStopControl() noexcept;
void snapSetDesc(std::int32_t set) noexcept;
bool snapCheckState() noexcept;
void snapAssignFrmCntFlags(std::int32_t setDelay, std::int32_t clearDelay) noexcept;

std::int32_t GetSnapshotControlPhotoCount() noexcept;

} // namespace shenmue::s2
