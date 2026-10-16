#pragma once

#include "s2/fight.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct FightDialogueRecord {
    std::uint16_t delay, camera;
    std::uint8_t played;
    std::byte reserved05[3];
    const char *voice;
    std::int16_t next;
    std::byte reserved12[6];
};
static_assert(sizeof(FightDialogueRecord) == 24);
static_assert(offsetof(FightDialogueRecord, voice) == 8);
static_assert(offsetof(FightDialogueRecord, next) == 16);
inline FightDialogueRecord dialogueRecords[]{{.next = -1}, {.next = -1}, {.next = -1}, {.voice = "0100A002", .next = -1}, {.voice = "0100A003", .next = -1},
    {.voice = "0100A004", .next = -1}, {.voice = "0100A005", .next = -1}, {.voice = "0100A006", .next = -1}, {.voice = "0100A007", .next = -1},
    {.voice = "0100A008", .next = -1}, {.delay = 30, .voice = "0100A009", .next = -1}, {.delay = 30, .voice = "0100A010", .next = -1},
    {.delay = 30, .voice = "0100A011", .next = -1}, {.delay = 30, .voice = "0100A012", .next = -1}, {.voice = "0100K005", .next = -1},
    {.voice = "0100L018", .next = -1}, {.voice = "0100M016", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1},
    {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1},
    {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1},
    {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1},
    {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "", .next = -1}, {.voice = "0100G001", .next = -1}, {.voice = "0100G002", .next = -1},
    {.voice = "0100G003", .next = -1}, {.voice = "0100G004", .next = -1}, {.voice = "0100G005", .next = -1}, {.voice = "0100G006", .next = -1},
    {.voice = "0100G007", .next = -1}, {.voice = "0100G008", .next = -1}, {.voice = "0100G009", .next = -1}, {.voice = "0100G010", .next = -1},
    {.voice = "0100G011", .next = -1}, {.voice = "0100G012", .next = -1}, {.delay = 30, .voice = "0100I001", .next = 48},
    {.delay = 60, .voice = "0100A009", .next = -1}, {.voice = "0100I002", .next = 50}, {.delay = 60, .camera = 1, .voice = "0100A017", .next = 51},
    {.delay = 80, .next = -2}, {.camera = 2, .voice = "0100I003", .next = 53}, {.delay = 55, .voice = "0100A018", .next = 54}, {.delay = 1, .next = -2},
    {.delay = 30, .voice = "0100I004", .next = -1}, {.delay = 30, .voice = "0100I005", .next = -1}, {.delay = 30, .voice = "0100I006", .next = 58},
    {.delay = 90, .voice = "0100I007", .next = -1}, {.camera = 3, .next = 60}, {.delay = 15, .voice = "0032B016", .next = 61},
    {.delay = 90, .camera = 9, .next = 62}, {.delay = 15, .voice = "0032A014", .next = 63}, {.delay = 75, .next = -2}, {.camera = 8, .next = 65},
    {.delay = 15, .voice = "0032B022", .next = 66}, {.delay = 80, .camera = 4, .next = 67}, {.delay = 15, .voice = "0032A026", .next = 68},
    {.delay = 90, .next = -2}, {.next = -1}, {.camera = 4, .next = 71}, {.delay = 15, .voice = "0032A027", .next = 72}, {.delay = 35, .camera = 8, .next = 73},
    {.delay = 15, .voice = "0032B023", .next = 74}, {.delay = 70, .next = -2}, {.next = -1}, {.voice = "0026B061", .next = -1},
    {.voice = "0026B059", .next = -1}, {.voice = "0026B060", .next = -1}, {.voice = "0026B063", .next = -1}, {.voice = "0026B064", .next = -1},
    {.voice = "0026B066", .next = -1}, {.voice = "0026B067", .next = -1}, {.voice = "0026B068", .next = -1}, {.voice = "0026B069", .next = -1},
    {.voice = "0026B070", .next = -1}, {.voice = "0026B071", .next = -1}, {.voice = "0026B072", .next = -1}, {.voice = "0026B065", .next = -1},
    {.voice = "0026B062", .next = -1}, {.voice = "0026B073", .next = -1}, {.voice = "0100N001", .next = -1}, {.voice = "0100N002", .next = -1},
    {.voice = "0100N003", .next = -1}, {.voice = "0100N004", .next = -1}, {.voice = "0100N005", .next = -1}, {.voice = "0100N006", .next = -1},
    {.voice = "0100N007", .next = -1}, {.voice = "0100N008", .next = -1}, {.voice = "0100N009", .next = -1}, {.voice = "0100N015", .next = -1},
    {.voice = "0100N016", .next = -1}, {.voice = "0100N002", .next = -1}, {.voice = "0100N017", .next = -1}, {.voice = "0100N018", .next = -1},
    {.voice = "0100K027", .next = -1}, {.voice = "0100K022", .next = -1}, {.voice = "0100L015", .next = -1}, {.voice = "0100L016", .next = -1},
    {.voice = "0100K008", .next = -1}, {.voice = "0100L017", .next = -1}, {.voice = "0100L017", .next = -1}, {.voice = "00000000", .next = -1},
    {.voice = "0026B074", .next = -1}, {.voice = "0026B068", .next = -1}, {.voice = "0026B075", .next = -1}, {.voice = "0026B076", .next = -1},
    {.voice = "0026B077", .next = -1}, {.voice = "0026B078", .next = -1}, {.voice = "0026B067", .next = -1}, {.voice = "0026B079", .next = -1},
    {.voice = "0026B080", .next = -1}, {.voice = "0026B081", .next = -1}, {.voice = "0026B069", .next = -1}, {.voice = "0026B082", .next = -1},
    {.voice = "0026B083", .next = -1}, {.voice = "0026B084", .next = -1}, {.voice = "0026B085", .next = -1}, {.voice = "0026B063", .next = -1},
    {.voice = "0026B086", .next = -1}, {.voice = "0026B064", .next = -1}, {.voice = "0026B066", .next = -1}, {.voice = "0026B087", .next = -1},
    {.voice = "0026B088", .next = -1}, {.voice = "0026B089", .next = -1}, {.voice = "0026B065", .next = -1}, {.voice = "0100B072", .next = -1},
    {.voice = "0100B073", .next = -1}, {.voice = "0100B074", .next = -1}, {.voice = "0100B075", .next = -1}, {.voice = "0100A041", .next = -1},
    {.voice = "0100A042", .next = -1}, {.camera = 1, .voice = "0100A043", .next = 150}, {.voice = "0100P004", .next = -1}, {.voice = "0100P005", .next = -1},
    {.voice = "0100P001", .next = -1}, {.voice = "0100P002", .next = -1}, {.voice = "0100P003", .next = -1}, {.voice = "0100A044", .next = -1},
    {.camera = 1, .voice = "0100A047", .next = 151}, {.delay = 75, .next = -2}, {.delay = 75, .next = -2}, {.voice = "0100Q009", .next = -1},
    {.voice = "0100A081", .next = -1}, {.delay = 22, .camera = 5, .voice = "0100Q010", .next = 155}, {.delay = 50, .camera = 1, .next = 156},
    {.delay = 40, .next = -2}, {.delay = 30, .camera = 7, .voice = "0100A050", .next = 158}, {.delay = 120, .next = -2},
    {.camera = 7, .voice = "0100A052", .next = 160}, {.delay = 50, .next = -2}, {.voice = "0100R007", .next = -1},
    {.camera = 6, .voice = "0100R008", .next = 163}, {.delay = 90, .camera = 1, .voice = "0100A054", .next = 164}, {.delay = 90, .next = -2},
    {.voice = "0100R009", .next = -1}, {.voice = "0100R010", .next = -1}, {.voice = "0100R011", .next = -1}, {.voice = "0100R012", .next = -1},
    {.voice = "0100R013", .next = -1}, {.voice = "0100R014", .next = -1}, {.voice = "0100R015", .next = -1}, {.voice = "0100R016", .next = -1},
    {.voice = "0100R017", .next = -1}, {.voice = "0032B019", .next = -1}, {.voice = "0032B022", .next = -1}, {.voice = "0032B023", .next = -1},
    {.voice = "0064B007", .next = -1}, {.voice = "0064A006", .next = -1}, {.voice = "0064B008", .next = -1}, {.voice = "0064A007", .next = -1},
    {.voice = "0064B004", .next = -1}, {.voice = "0064B021", .next = -1}, {.voice = "0064B022", .next = -1}, {.voice = "0232A023", .next = -1},
    {.voice = "0232A024", .next = -1}, {.voice = "0232A025", .next = -1}, {.voice = "0232A026", .next = -1}, {.voice = "0232A027", .next = -1},
    {.voice = "0232B048", .next = -1}, {.voice = "0232B049", .next = -1}, {.voice = "0232B050", .next = -1}, {.voice = "0232B051", .next = -1},
    {.voice = "0232B052", .next = -1}, {.voice = "0100N010", .next = -1}, {.voice = "0100N011", .next = -1}, {.voice = "0100N012", .next = -1},
    {.voice = "0100N013", .next = -1}, {.voice = "0100N014", .next = -1}, {.voice = "0100J008", .next = -1}, {.voice = "0100J009", .next = -1},
    {.voice = "0100J010", .next = -1}, {.delay = 22, .camera = 5, .voice = "0100Q017", .next = 203}, {.delay = 80, .next = -2},
    {.delay = 22, .camera = 5, .voice = "0100Q018", .next = 205}, {.delay = 80, .next = -2}, {.delay = 22, .camera = 5, .voice = "0100Q019", .next = 207},
    {.delay = 80, .next = -2}, {.voice = "0100A083", .next = -1}, {.voice = "0100Q020", .next = -1}, {.voice = "0100A082", .next = -1},
    {.voice = "0100Q011", .next = -1}, {.voice = "0100Q013", .next = -1}, {.voice = "0100Q014", .next = -1}, {.voice = "0100L015", .next = -1},
    {.voice = "0100L016", .next = -1}, {.voice = "0275D002", .next = -1}, {.voice = "0275D003", .next = -1}, {.voice = "0275D004", .next = -1},
    {.voice = "0275D005", .next = -1}, {.voice = "0275G002", .next = -1}, {.voice = "0275G003", .next = -1}, {.voice = "0275G004", .next = -1},
    {.voice = "0275G005", .next = -1}, {.voice = "0275C005", .next = -1}, {.voice = "0275C006", .next = -1}, {.voice = "0275C007", .next = -1},
    {.voice = "0275C008", .next = -1}, {.voice = "0275D006", .next = -1}, {.voice = "0275D007", .next = -1}, {.voice = "0275D008", .next = -1},
    {.voice = "0275G006", .next = -1}, {.voice = "0275G007", .next = -1}, {.voice = "0275G008", .next = -1}, {.voice = "0275C009", .next = -1},
    {.voice = "0275C010", .next = -1}, {.voice = "0275C011", .next = -1}, {.voice = "0275D009", .next = -1}, {.delay = 30, .voice = "0275D010", .next = -1},
    {.delay = 30, .voice = "0275D011", .next = -1}, {.delay = 30, .voice = "0275D012", .next = -1}, {.delay = 30, .voice = "0275G009", .next = -1},
    {.voice = "0275G010", .next = -1}, {.delay = 30, .voice = "0275G011", .next = -1}, {.delay = 30, .voice = "0275G012", .next = -1},
    {.delay = 30, .voice = "0275C012", .next = -1}, {.voice = "0275C013", .next = -1}, {.delay = 30, .voice = "0275C014", .next = -1},
    {.delay = 30, .voice = "0275C015", .next = -1}, {.next = -1}};
inline FightDialogueRecord *const dialogueBanks[]{dialogueRecords, dialogueRecords + 1, dialogueRecords + 2, dialogueRecords + 3};
struct FightDialogueCamera {
    std::uint16_t camera;
    std::uint8_t isolate;
    std::int8_t expression;
    shen_name character, target;
};
inline constexpr FightDialogueCamera dialogueCameras[]{{.expression = -1}, {.camera = 620, .isolate = 1, .expression = -1, .character = SHEN_NAME(RYO_)},
    {.camera = 621, .isolate = 1, .expression = -1, .character = SHEN_NAME(VNC_)},
    {.camera = 9000, .isolate = 1, .expression = -1, .character = SHEN_NAME(SYE_), .target = SHEN_NAME(RYO_)},
    {.camera = 9001, .isolate = 1, .expression = 6, .character = SHEN_NAME(RYO_), .target = SHEN_NAME(SYE_)},
    {.camera = 2311, .isolate = 1, .expression = 0, .character = SHEN_NAME(KFT_)},
    {.camera = 2311, .isolate = 1, .expression = 0, .character = SHEN_NAME(AW1_)},
    {.camera = 2321, .isolate = 1, .expression = 0, .character = SHEN_NAME(AW1_)},
    {.camera = 9002, .isolate = 1, .expression = -1, .character = SHEN_NAME(SYE_), .target = SHEN_NAME(RYO_)},
    {.camera = 9001, .isolate = 1, .expression = 1, .character = SHEN_NAME(RYO_), .target = SHEN_NAME(SYE_)}};

} // namespace shenmue::s2
