#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name QFEB_TASK{SHEN_NAME(EVNT)}, QFEB_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name QFEB_RYO{SHEN_NAME(RYO_)}, QFEB_REN{SHEN_NAME(REN_)}, QFEB_RKN{SHEN_NAME(RKN_)}, QFEB_KHO{SHEN_NAME(KHO_)};
inline constexpr shen_name QFEB_SOY{SHEN_NAME(SOY_)}, QFEB_KIC{SHEN_NAME(KIC_)}, QFEB_ANC{SHEN_NAME(ANC_)}, QFEB_MRC{SHEN_NAME(MRC_)};
inline constexpr shen_name QFEB_DEAD{SHEN_NAME(DEAD)}, QFEB_RETURN{SHEN_NAME(QTCV)}, QFEB_TITLE{SHEN_NAME(RTR2)}, QFEB_EXIT_TITLE{SHEN_NAME(QF10)};
inline constexpr shen_name QFEB_CAMERA_RKN{SHEN_NAME(damA)}, QFEB_CAMERA_ANC{SHEN_NAME(damB)}, QFEB_CAMERA_MRC{SHEN_NAME(damE)};
inline constexpr shen_name QFEB_AUTHORS[]{SHEN_NAME(AU_0), SHEN_NAME(AU_2), SHEN_NAME(AU_1)};
inline constexpr shen_name QFEB_CHARACTERS[]{QFEB_RYO, QFEB_REN, QFEB_RKN, QFEB_KHO, QFEB_SOY, QFEB_KIC, QFEB_ANC, QFEB_MRC, EVENT_CHARACTER_INVALID};
inline constexpr shen_name QFEB_FIGHT_CHARACTERS[]{QFEB_RYO, QFEB_RKN, QFEB_KHO, QFEB_SOY, QFEB_KIC, QFEB_ANC, QFEB_MRC};
inline constexpr shen_name QFEB_LIVE_ENEMIES[3][3]{{QFEB_RKN, QFEB_KHO, QFEB_SOY}, {QFEB_ANC, QFEB_KIC, QFEB_KHO}, {QFEB_MRC, QFEB_ANC, QFEB_SOY}};
inline constexpr shen_name QFEB_RELEASE_ENEMIES[3][3]{{QFEB_KIC, QFEB_ANC, QFEB_MRC}, {QFEB_SOY, QFEB_RKN, QFEB_MRC}, {QFEB_KIC, QFEB_RKN, QFEB_KHO}};
inline constexpr std::uint16_t QFEB_STANCE_MOTIONS[]{306, 3632, 3636, 3640, 3644, 3648, 3632};
inline constexpr Vector3 QFEB_BATTLE_POSITIONS[3][7]{
    {{232.369995f, 128.800003f, 142.940002f}, {234.889999f, 128.800003f, 138.919998f}, {229.639999f, 128.800003f, 147.210007f},
        {237.860001f, 128.800003f, 142.979996f}, {226.940002f, 128.800003f, 142.860001f}, {235.080002f, 128.800003f, 147.389999f},
        {229.960007f, 128.800003f, 138.520004f}},
    {{214.789993f, 128.800003f, 174.589996f}, {216.130005f, 128.800003f, 172.600006f}, {218.130005f, 128.800003f, 173.880005f},
        {212.919998f, 128.800003f, 172.899994f}, {216.460007f, 128.800003f, 177.589996f}, {212.139999f, 128.800003f, 175.770004f},
        {214.619995f, 128.800003f, 171.669998f}},
    {{194.960007f, 128.800003f, 150.699997f}, {196.210007f, 128.800003f, 147.839996f}, {193.720001f, 128.800003f, 153.479996f},
        {198.139999f, 128.800003f, 151.240005f}, {191.600006f, 128.800003f, 150.710007f}, {196.770004f, 128.800003f, 153.559998f},
        {193.199997f, 128.800003f, 147.910004f}}};
inline constexpr float QFEB_BATTLE_YAWS[3][7]{{355.790009f, 185.789993f, 330.739990f, 90.800003f, 227.339996f, 30.7299995f, 175.789993f},
    {360, 149.960007f, 84.9700012f, 209.940002f, 21.5799999f, 285.010010f, 169.949997f},
    {265.679993f, 164.949997f, 353.589996f, 86.8600006f, 277.660004f, 24.9899998f, 194.949997f}};
inline constexpr Vector3 QFEB_BASE_POSITIONS[]{
    {241.679993f, 128.800003f, 147}, {222.740005f, 128.800003f, 175.820007f}, {188.300003f, 128.800003f, 150.529999f}};
inline constexpr float QFEB_BASE_YAW = 70.2799988f;
inline constexpr Vector3 QFEB_CHOREOGRAPHY_POSITIONS[3][2]{{{245.190002f, 128.800003f, 148.360001f}, {245.970001f, 128.800003f, 147.440002f}},
    {{224.539993f, 128.800003f, 176.619995f}, {224.839996f, 128.800003f, 175.860001f}},
    {{187.830002f, 128.800003f, 150.429993f}, {187.360001f, 128.800003f, 149.339996f}}};
inline constexpr float QFEB_CHOREOGRAPHY_YAWS[3][2]{{64.7900009f, 77.5999985f}, {55.2099991f, 55.2000008f}, {245.539993f, 235.169998f}};
inline constexpr shen_name QFEB_CHOREOGRAPHY_NPCS[3][2]{{QFEB_KHO, QFEB_RKN}, {QFEB_ANC, QFEB_KIC}, {QFEB_ANC, QFEB_MRC}};
inline constexpr Vector3 QFEB_CHOREOGRAPHY_NPC_POSITIONS[3][2]{{{239.800003f, 128.800003f, 145.100006f}, {239.690002f, 128.800003f, 146.050003f}},
    {{222, 128.800003f, 174.710007f}, {221.520004f, 128.800003f, 175.570007f}},
    {{190.179993f, 128.800003f, 150.669998f}, {189.300003f, 128.800003f, 151.470001f}}};
inline constexpr float QFEB_CHOREOGRAPHY_NPC_YAWS[3][2]{{79.4499969f, 29.4500008f}, {70.0999985f, 45}, {275.670013f, 170.460007f}};
inline constexpr Vector3 QFEB_PROXIMITY_POINTS[]{
    {187.320007f, 128.800003f, 151.080002f}, {224.130005f, 128.800003f, 175}, {245.649994f, 128.800003f, 147.479996f}};
inline constexpr Vector3 QFEB_CAMERA_EYES[]{
    {235.75f, 130.179993f, 144.089996f}, {219.772293f, 130.645203f, 172.815903f}, {195.055496f, 130.959610f, 156.472198f}};
inline constexpr Vector3 QFEB_CAMERA_TARGETS[]{{232.369995f, 129.740005f, 142.970001f}, {215.563400f, 130, 173.716599f}, {195.680298f, 130, 150.986801f}};
inline constexpr Vector3 QFEB_EXIT_POSITIONS[]{
    {257.730011f, 125.199997f, 168}, {223.139999f, 125.199997f, 179.399994f}, {188.850006f, 125.199997f, 146.850006f}, {257.170013f, 125.199997f, 166.630005f}};
inline constexpr float QFEB_EXIT_YAWS[]{90, 0, 180, 90};
inline constexpr Vector3 QFEB_REN_EXIT_POSITIONS[]{{257.040009f, 125.199997f, 166.309998f}, {225.220001f, 125.199997f, 178.830002f},
    {189.690002f, 125.199997f, 147.300003f}, {257.890015f, 132.399994f, 168.539993f}};
inline constexpr std::int32_t QFEB_REN_EXIT_YAWS[]{16384, 0, 32768, 12178};
struct QfebAuthorReturnPose {
    Vector3 position;
    Vector3i rotation;
};
inline constexpr QfebAuthorReturnPose QFEB_AUTHOR_RETURN_POSES[]{{.position = {248.039993f, 128.800003f, 154.240005f}, .rotation = {0, 0, 0}},
    {.position = {224.809998f, 127, 185.589996f}, .rotation = {0, 0, 0}}, {.position = {187.279999f, 127, 140.070007f}, .rotation = {0, 32768, 0}}};
inline Vector3i qfebAuthorReturnRotation;
inline constexpr Vector3i QFEB_AUTHOR_DEFAULT_ROTATION{0, 32768, 0};
inline constexpr std::int32_t QFEB_RETURN_ARGUMENTS[3][3]{{2, 1, 1}, {0, 1, 1}, {1, 1, 1}};
struct QfebEventWork {
    HLib::Task *task;
    std::byte unused08[0x2c];
    std::int32_t phase;
    std::uint64_t reserved38;
    std::byte unused40[0x78];
    std::int32_t npcBatchOverride;
    std::uint32_t reservedbc;
    EventReadCharactersWork *characters;
    std::uint8_t entry;
    std::int8_t delay;
    std::byte reservedca[2];
    std::uint32_t battleUpdates, battleFrames;
    std::uint8_t battleFade, exitFade;
    std::byte reservedd6[2];
    std::int32_t idleFrames, phaseFrames;
    std::uint8_t voiceFrames, previousVoice;
    std::byte reservede2[6];
};
struct QfebAuthorWork {
    HLib::Task *task;
    std::uint32_t unused08;
    shen_name author;
    std::int16_t state;
    std::byte unused12[6];
};
struct QfebAuthorLoopWork {
    QfebAuthorWork *parent;
    std::uint32_t frame, reserved0c;
};
static_assert(sizeof(QfebEventWork) == 0xe8 && offsetof(QfebEventWork, phase) == 0x34 && offsetof(QfebEventWork, npcBatchOverride) == 0xb8);
static_assert(offsetof(QfebEventWork, characters) == 0xc0 && offsetof(QfebEventWork, entry) == 0xc8 && offsetof(QfebEventWork, battleFrames) == 0xd0);
static_assert(offsetof(QfebEventWork, idleFrames) == 0xd8 && offsetof(QfebEventWork, voiceFrames) == 0xe0);
static_assert(sizeof(QfebAuthorWork) == 0x18 && offsetof(QfebAuthorWork, state) == 0x10 && sizeof(QfebAuthorLoopWork) == 0x10);
static_assert(sizeof(QfebAuthorReturnPose) == 0x18 && offsetof(QfebAuthorReturnPose, rotation) == 0xc);
void QfebInit();
void QfebMain(std::uintptr_t argument);
extern const ScriptAreaDefinition qfebScriptDefinition;
} // namespace shenmue::s2
