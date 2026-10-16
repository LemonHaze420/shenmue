#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"

#include <bit>

namespace shenmue::s2 {

inline constexpr shen_name AUTHOR_VIEWER_503E_AREA{SHEN_NAME(503E)}, AUTHOR_VIEWER_TASK{SHEN_NAME(EVNT)}, AUTHOR_VIEWER_RYO{SHEN_NAME(RYO_)};
inline constexpr char AUTHOR_VIEWER_503E_MOTION[] = "0503E.MOT";
inline constexpr shen_name AUTHOR_VIEWER_332E_AREA{SHEN_NAME(332E)};
inline constexpr char AUTHOR_VIEWER_332E_MOTION[] = "0332E.MOT", AUTHOR_VIEWER_332E_CONVERSATION[] = "0332";
inline constexpr const char *AUTHOR_VIEWER_332E_DEBUG_NAMES[]{"0332E010.c\n", "0332E011.c\n", "0332E012.c\n", "0332E013.c\n", "0332E014.c\n", "0332E015.c\n",
    "0332E016.c\n", "0332E017.c\n", "0332E018.c\n", "0332E060.c\n", "0332E061.c\n", "0332E062.c\n"};
inline constexpr shen_name AUTHOR_VIEWER_081E_AREA{SHEN_NAME(081E)}, AUTHOR_VIEWER_182E_AREA{SHEN_NAME(182E)};
inline constexpr shen_name AUTHOR_VIEWER_182E_SECOND_CHARACTER{SHEN_NAME(a946)};
inline constexpr char AUTHOR_VIEWER_081E_MOTION[] = "0081E.MOT", AUTHOR_VIEWER_081E_CONVERSATION[] = "0081";
inline constexpr char AUTHOR_VIEWER_182E_MOTION[] = "0182E.MOT", AUTHOR_VIEWER_182E_CONVERSATION[] = "0182";
inline constexpr const char *AUTHOR_VIEWER_081E_DEBUG_NAMES[]{"0081E000.c\n", "0081E001.c\n"};
inline constexpr shen_name AUTHOR_VIEWER_015E_AREA{SHEN_NAME(015E)}, AUTHOR_VIEWER_295E_AREA{SHEN_NAME(295E)};
inline constexpr shen_name AUTHOR_VIEWER_015E_A714{SHEN_NAME(a714)}, AUTHOR_VIEWER_015E_A718{SHEN_NAME(a718)};
inline constexpr shen_name AUTHOR_VIEWER_015E_A016{SHEN_NAME(a016)}, AUTHOR_VIEWER_015E_A826{SHEN_NAME(a826)};
inline constexpr shen_name AUTHOR_VIEWER_015E_A824{SHEN_NAME(a824)}, AUTHOR_VIEWER_015E_A846{SHEN_NAME(a846)};
inline constexpr shen_name AUTHOR_VIEWER_015E_CHARACTERS[]{
    AUTHOR_VIEWER_015E_A714, AUTHOR_VIEWER_015E_A718, AUTHOR_VIEWER_015E_A016, AUTHOR_VIEWER_015E_A826, AUTHOR_VIEWER_015E_A824, AUTHOR_VIEWER_015E_A846};
inline constexpr shen_name AUTHOR_VIEWER_295E_REN{SHEN_NAME(REN_)}, AUTHOR_VIEWER_295E_INVALID_CHARACTER{0xffffffffU};
inline constexpr shen_name AUTHOR_VIEWER_295E_CHARACTERS[]{AUTHOR_VIEWER_295E_REN, AUTHOR_VIEWER_295E_INVALID_CHARACTER};
inline constexpr char AUTHOR_VIEWER_015E_MOTION[] = "0015E.MOT", AUTHOR_VIEWER_015E_CONVERSATION[] = "0015";
inline constexpr char AUTHOR_VIEWER_295E_MOTION[] = "0295E.MOT", AUTHOR_VIEWER_295E_CONVERSATION[] = "0295";
inline constexpr const char *AUTHOR_VIEWER_015E_DEBUG_NAMES[]{"0015E030.c\n", "0015E060.c\n"};
inline constexpr char AUTHOR_VIEWER_295E_DEBUG_NAME[] = "0295E020.c\n";
inline constexpr Vector3 AUTHOR_VIEWER_295E_ROOT_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x42666666U), .z = 0.0f};
static_assert(AUTHOR_VIEWER_015E_A714.name == 0x34313761U && AUTHOR_VIEWER_015E_A718.name == 0x38313761U);
static_assert(AUTHOR_VIEWER_015E_A016.name == 0x36313061U && AUTHOR_VIEWER_015E_A826.name == 0x36323861U);
static_assert(AUTHOR_VIEWER_015E_A824.name == 0x34323861U && AUTHOR_VIEWER_015E_A846.name == 0x36343861U);
static_assert(sizeof(AUTHOR_VIEWER_295E_CHARACTERS) == 8 && AUTHOR_VIEWER_295E_REN.name == 0x5f4e4552U);
static_assert(std::bit_cast<std::uint32_t>(AUTHOR_VIEWER_295E_ROOT_POSITION.y) == 0x42666666U);

inline constexpr shen_name AUTHOR_VIEWER_035E_AREA{SHEN_NAME(035E)}, AUTHOR_VIEWER_061E_AREA{SHEN_NAME(061E)};
inline constexpr shen_name AUTHOR_VIEWER_035E_SAI{SHEN_NAME(SAI_)}, AUTHOR_VIEWER_061E_SAM{SHEN_NAME(SAM_)};
inline constexpr shen_name AUTHOR_VIEWER_061E_KEIB{SHEN_NAME(KEIB)}, AUTHOR_VIEWER_INVALID_CHARACTER{0xffffffffU};
inline constexpr shen_name AUTHOR_VIEWER_035E_CHARACTERS[]{AUTHOR_VIEWER_035E_SAI, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr shen_name AUTHOR_VIEWER_061E_CHARACTERS[]{AUTHOR_VIEWER_061E_SAM, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AUTHOR_VIEWER_035E_MOTION[] = "0035E.MOT", AUTHOR_VIEWER_035E_CONVERSATION[] = "0035";
inline constexpr char AUTHOR_VIEWER_061E_MOTION[] = "0061E1.MOT", AUTHOR_VIEWER_061E_CONVERSATION[] = "0061";
inline constexpr const char *AUTHOR_VIEWER_035E_DEBUG_NAMES[]{"0035E030.c\n", "0035E031.c\n"};
inline constexpr char AUTHOR_VIEWER_061E_DEBUG_NAME[] = "0061E040.c\n";
static_assert(sizeof(AUTHOR_VIEWER_035E_CHARACTERS) == 8 && AUTHOR_VIEWER_035E_SAI.name == 0x5f494153U);
static_assert(sizeof(AUTHOR_VIEWER_061E_CHARACTERS) == 8 && AUTHOR_VIEWER_061E_SAM.name == 0x5f4d4153U);
static_assert(AUTHOR_VIEWER_061E_KEIB.name == 0x4249454bU);

inline constexpr shen_name AUTHOR_VIEWER_0006_AREA{SHEN_NAME(0006)}, AUTHOR_VIEWER_0006_A298{SHEN_NAME(a298)};
inline constexpr shen_name AUTHOR_VIEWER_0006_DAY_LIGHT{SHEN_NAME(0001)};
inline constexpr char AUTHOR_VIEWER_0006_MOTION[] = "0006A.MOT";
inline constexpr const char *AUTHOR_VIEWER_0006_DEBUG_NAMES[]{"0006A000.c\n", "0006A001.c\n", "0006A002.c\n", "0006A010.c\n", "0006A011.c\n", "0006A012.c\n"};
inline constexpr std::int32_t AUTHOR_VIEWER_0006_LIGHT_SLOTS[]{0, 1, 2, 4, 5, 6, 7, 8, 9};


struct AuthorViewerTaskParameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int16_t selection;
    std::int16_t state;
};
struct AuthorViewerLoopTaskParameter {
    AuthorViewerTaskParameter *parent;
};
struct AuthorViewer081ELoopTaskParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
struct AuthorViewer182ELoopTaskParameter {
    AuthorViewerTaskParameter *parent;
};
struct AuthorViewer015ELoopTaskParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
struct AuthorViewer295ELoopTaskParameter {
    AuthorViewerTaskParameter *parent;
};
static_assert(sizeof(AuthorViewerTaskParameter) == 0x10);
static_assert(alignof(AuthorViewerTaskParameter) == 8);
static_assert(offsetof(AuthorViewerTaskParameter, self) == 0x00);
static_assert(offsetof(AuthorViewerTaskParameter, motion) == 0x08);
static_assert(offsetof(AuthorViewerTaskParameter, selection) == 0x0c);
static_assert(offsetof(AuthorViewerTaskParameter, state) == 0x0e);
static_assert(sizeof(AuthorViewerLoopTaskParameter) == 0x08);
static_assert(alignof(AuthorViewerLoopTaskParameter) == 8);
static_assert(offsetof(AuthorViewerLoopTaskParameter, parent) == 0x00);
static_assert(sizeof(AuthorViewer081ELoopTaskParameter) == 0x10);
static_assert(alignof(AuthorViewer081ELoopTaskParameter) == 8);
static_assert(offsetof(AuthorViewer081ELoopTaskParameter, parent) == 0x00);
static_assert(offsetof(AuthorViewer081ELoopTaskParameter, currentFrame) == 0x08);
static_assert(sizeof(AuthorViewer182ELoopTaskParameter) == 0x08);
static_assert(alignof(AuthorViewer182ELoopTaskParameter) == 8);
static_assert(offsetof(AuthorViewer182ELoopTaskParameter, parent) == 0x00);
static_assert(sizeof(AuthorViewer015ELoopTaskParameter) == 0x10);
static_assert(alignof(AuthorViewer015ELoopTaskParameter) == 8);
static_assert(offsetof(AuthorViewer015ELoopTaskParameter, parent) == 0x00);
static_assert(offsetof(AuthorViewer015ELoopTaskParameter, currentFrame) == 0x08);
static_assert(sizeof(AuthorViewer295ELoopTaskParameter) == 0x08);
static_assert(alignof(AuthorViewer295ELoopTaskParameter) == 8);
static_assert(offsetof(AuthorViewer295ELoopTaskParameter, parent) == 0x00);


void EV_set_debug_text_pos(std::int32_t column, std::int32_t row);
void EV_debug_log_to_screen(const char *format, ...);
void AuthorViewer332EInit();
void AuthorViewer332EMain(std::uintptr_t argument);
void AuthorViewer332EMainCallback(void *taskData);
std::int64_t QueueAuthorViewer332ELoop(AuthorViewerTaskParameter *parent);
extern const ScriptAreaDefinition authorViewer332EScriptDefinition;
void AuthorViewer503EInit();
void AuthorViewer503EMain(std::uintptr_t argument);
void AuthorViewer503EMainCallback(void *taskData);
std::int64_t QueueAuthorViewer503ELoop(AuthorViewerTaskParameter *parent);

std::int64_t AuthorViewerLoopCallback(void *taskData);
void AuthorViewerLoopTaskCallback(void *taskData);
extern const ScriptAreaDefinition authorViewer503EScriptDefinition;
void AuthorViewer081EInit();
void AuthorViewer081EMain(std::uintptr_t argument);
void AuthorViewer081EMainCallback(void *taskData);
std::int64_t QueueAuthorViewer081ELoop(AuthorViewerTaskParameter *parent);
std::int64_t AuthorViewer081ELoopCallback(void *taskData);
void AuthorViewer081ELoopTaskCallback(void *taskData);
extern const ScriptAreaDefinition authorViewer081EScriptDefinition;
void AuthorViewer182EInit();
std::int64_t AuthorViewer182EMain(std::uintptr_t argument);
void AuthorViewer182EAreaMain(std::uintptr_t argument);
void AuthorViewer182EMainCallback(void *taskData);
std::int64_t QueueAuthorViewer182ELoop(AuthorViewerTaskParameter *parent);
std::int64_t AuthorViewer182ELoopCallback(void *taskData);
void AuthorViewer182ELoopTaskCallback(void *taskData);
extern const ScriptAreaDefinition authorViewer182EScriptDefinition;
void AuthorViewer015EInit();
void AuthorViewer015EMain(std::uintptr_t argument);
void AuthorViewer015EMainCallback(void *taskData);
std::int64_t QueueAuthorViewer015ELoop(AuthorViewerTaskParameter *parent);
std::int64_t AuthorViewer015ELoopCallback(void *taskData);
void AuthorViewer015ELoopTaskCallback(void *taskData);
void DisableAuthorViewer015ECharacters();
void RestoreAuthorViewer015ECharacters();
extern const ScriptAreaDefinition authorViewer015EScriptDefinition;
void AuthorViewer295EInit();
HLib::Task *AuthorViewer295EMain(std::uintptr_t argument);
void AuthorViewer295EAreaMain(std::uintptr_t argument);
void AuthorViewer295EMainCallback(void *taskData);
void SetupAuthorViewer295E();
void CleanupAuthorViewer295E(AuthorViewerTaskParameter *parent);
std::int64_t QueueAuthorViewer295ELoop(AuthorViewerTaskParameter *parent);
std::int64_t AuthorViewer295ELoopCallback(void *taskData);
void AuthorViewer295ELoopTaskCallback(void *taskData);
extern const ScriptAreaDefinition authorViewer295EScriptDefinition;
void AuthorViewer035EInit();
HLib::Task *AuthorViewer035EMain(std::uintptr_t argument);
void AuthorViewer035EAreaMain(std::uintptr_t argument);
void AuthorViewer035EMainCallback(void *taskData);
std::int64_t QueueAuthorViewer035ELoop(AuthorViewerTaskParameter *parent);
std::int64_t AuthorViewer035ELoopCallback(void *taskData);
void AuthorViewer035ELoopTaskCallback(void *taskData);
void CleanupAuthorViewer035E(AuthorViewerTaskParameter *parent);
extern const ScriptAreaDefinition authorViewer035EScriptDefinition;
void AuthorViewer061EInit();
HLib::Task *AuthorViewer061EMain(std::uintptr_t argument);
void AuthorViewer061EAreaMain(std::uintptr_t argument);
void AuthorViewer061EMainCallback(void *taskData);
std::int64_t QueueAuthorViewer061ELoop(AuthorViewerTaskParameter *parent);
std::int64_t AuthorViewer061ELoopCallback(void *taskData);
void AuthorViewer061ELoopTaskCallback(void *taskData);
void CleanupAuthorViewer061E(AuthorViewerTaskParameter *parent);
extern const ScriptAreaDefinition authorViewer061EScriptDefinition;
void AuthorViewer0006Init();
void AuthorViewer0006Main(std::uintptr_t argument);
void AuthorViewer0006MainCallback(void *taskData);
void QueueAuthorViewer0006Loop(AuthorViewerTaskParameter *parent);
void AuthorViewer0006LoopCallback(void *taskData);
extern const ScriptAreaDefinition authorViewer0006ScriptDefinition;

} // namespace shenmue::s2
