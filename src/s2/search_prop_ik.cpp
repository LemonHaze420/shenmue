#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/search_prop_ik_dc.hpp"

#include "s2/search_commands_dc.hpp"
#include "s2/search_prop_ik_data_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/character_transparency_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/camera_search_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/sound_program.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include <cstdlib>
#include <cstring>
#include "s2/search_state_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include <cmath>
#include <cstdio>
#include "s2/camera_state_query_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/search_model_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/scenario_flags.hpp"
#include <float.h>
#else
#include "s2/search_internal.hpp"
#include "s2/search_prop_ik_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cmath>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::array<SearchPropIKConfig, 15> propConfigurations = std::bit_cast<std::array<SearchPropIKConfig, 15>>(searchPropIKConfigurationBytes);

std::array<std::uint8_t, 228 * 11> propPoseCommands = searchPropIKPoseBytes;

std::int16_t propAngles[2]{};

Vector3 propWristAngles{};

Matrix4x4 propArmTransform{};

Vector3 propSinglePivot{.02f, 0, .03f};

Vector3 propPairedPivot{.02f, 0, .03f};

constexpr shen_name examineButton{SHEN_NAME(BTEV)};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

SearchPropIKConfig &Config(SearchPropIKSlot &slot);
Vector3 Translation();
Vector3 TransformVector(const Vector3 &point);
void SwizzleBasis(const Matrix4x4 &matrix);
std::int16_t Angle(float x, float z);
MT7Node *MeshNode(MT7Node *node);
void SetMarkerVisible(MT7Node &node, bool visible);
void GetStageTangents(SearchPropIKSlot &slot, std::uint32_t stage, Vector3 &right, Vector3 &left);
void CaptureArmRelativeTransform(SearchPropIKSlot &slot);
void BuildPropTransforms(SearchPropIKSlot &slot);
void SetupPropHandCamera(SearchPropIKSlot &slot);
void UpdatePropChestAndCamera(SearchPropIKSlot &slot);
void CapturePropPivot(SearchPropIKSlot &slot);
void UpdatePropFromArm(SearchPropIKSlot &slot);
void RotateHeldProp(SearchTaskData &search, SearchPropIKSlot &slot);
void PlaySelectionSound(std::size_t index);
void SolveStageGrip(SearchTaskData &search, SearchPropIKSlot &slot, std::uint32_t stage);

} // namespace

std::int32_t srchGetPropIKLookup(std::uint16_t marker);
void srchBindPropIK(SearchTaskData &search);
void srchClearPropIKSlot(std::int32_t index);
bool srchPushPropIK(SearchTaskData &search);
bool srchPopPropIK(SearchTaskData &search);
void srchBeginPropIKReturn();
void srchFinishPropIKItem(bool animate);
void srchReadSelectionSounds(const std::byte *config);
bool srchUpdatePropIK(SearchTaskData &search);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static std::int32_t SearchCommandEnd(SearchTaskData &search);

static std::int32_t SearchCommandItem(SearchTaskData &search);

static std::int32_t SearchCommandFlagRestore(SearchTaskData &search);

static std::int32_t SearchCommandFlagWait(SearchTaskData &search);

static std::int32_t SearchCommandChangeArea(SearchTaskData &search);

static std::int32_t SearchCommandFilm(SearchTaskData &search);

static std::int32_t SearchCommandIfStoryFlag(SearchTaskData &search);

static std::int32_t SearchCommandMessage(SearchTaskData &search);

static std::int32_t SearchCommandLight(SearchTaskData &search);

static std::int32_t SearchCommandSetInternalFlag(SearchTaskData &search);

static std::int32_t SearchCommandRandomMessage(SearchTaskData &search);

static std::int32_t SearchCommandDialogue(SearchTaskData &search);

static std::int32_t SearchCommandRandomDialogue(SearchTaskData &search);

static std::int32_t SearchCommandWaitProp(SearchTaskData &search);

static std::int32_t SearchCommandButtons(SearchTaskData &search);

static std::int32_t SearchCommandBindProp(SearchTaskData &search);

static std::int32_t SearchCommandReturnProp(SearchTaskData &search);

static std::int32_t SearchCommandRestart(SearchTaskData &search);

static std::int32_t SearchCommandChoices(SearchTaskData &search);

static std::int32_t SearchCommandDisableExamine(SearchTaskData &search);

static std::int32_t SearchCommandStepFlag(SearchTaskData &search);

static std::int32_t SearchCommandIfInternalFlag(SearchTaskData &search);

static std::int32_t SearchCommandIfCash(SearchTaskData &search);

static std::int32_t SearchCommandTurnPlayer(SearchTaskData &search);

void srchResetIK();

std::int32_t srchGetPropIKIndex();

bool srchIsPropIKSettled();

bool srchIsPropIKMoving();

bool srchIsPropIKActive();

void srchBindPropIK(SearchTaskData &search);

void srchFinishPropIKItem(std::int32_t animate);

void srchApplyObjectTransform(const MT7Node *node, std::uint32_t *yaw);

void srchApplyPropHierarchyTransform(const MT7Node &node);

void srchBeginPropIKReturn();

void srchRestartPropIK();

std::int32_t srchUpdatePropIK(SearchTaskData &search);

std::uint32_t srchRegisterAndPush(MT7Node *root);

bool srchLockTarget(SearchTaskData &search, shen_name name, bool faceTarget);

void srchRequestRelease();

void srchReset();

shen_name srchGetTargetName(std::uint32_t index);

void srchRequestLockonExit();

SearchPropIKSlot g_searchPropIKSlots[4];

__attribute__((aligned(8))) Matrix4x4 searchPropIKTransformData[4][6] = {}, searchPropIKAuxiliaryData[4][4] = {};

std::int32_t searchPropIKIndex = 0;

std::int16_t searchPropIKRotationVelocity[2];

ConversationBuffer *searchDialogue = 0, *randomSearchDialogue = 0;

__attribute__((aligned(4))) char randomSearchBank[32] = {};

SearchCommand const searchCommands[26] = {SearchCommandEnd, SearchCommandItem, SearchCommandFlagRestore, SearchCommandFlagWait, SearchCommandChangeArea, 0,
    SearchCommandFilm, 0, SearchCommandIfStoryFlag, SearchCommandMessage, SearchCommandLight, SearchCommandSetInternalFlag, SearchCommandRandomMessage,
    SearchCommandDialogue, SearchCommandRandomDialogue, SearchCommandWaitProp, SearchCommandButtons, SearchCommandBindProp, SearchCommandReturnProp,
    SearchCommandRestart, SearchCommandChoices, SearchCommandDisableExamine, SearchCommandStepFlag, SearchCommandIfInternalFlag, SearchCommandIfCash,
    SearchCommandTurnPlayer};

SearchPropIKSelection searchPropIKSelection[3] = {};

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t propAngles[2] = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3i propWristAngles = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 propArmTransform = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 propSinglePivot = {.02f, 0, .03f};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 propPairedPivot = {.02f, 0, .03f};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 propLookOffset = {0, .4f, -.1f};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float propIKBlendStep = 0.033333335f;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const shen_name examineButton(SHEN_NAME4('B', 'T', 'E', 'V'));
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static inline void srchReleaseCommandControl(SearchResourceContext &context) {
    if (context.flags & 0x10U) {
        SetPlayerControlEnabled(1);
        context.flags &= ~0x10U;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static inline void srchAcquireCommandControl(SearchResourceContext &context) {
    if (SetPlayerControlEnabled(0))
        context.flags |= 0x10U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static inline MapControlData *srchMapControl() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandEnd(SearchTaskData &search) {
    activeSearchConfig = 0;
    search.context->cursor = 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandItem(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (search.context->flags & 1U) {
        if (GetShouldRenderIcons(ItemEffectMessage))
            return 0;
        if (search.context->flags & 0x10U) {
            SetPlayerControlEnabled(1);
            search.context->flags &= ~0x10U;
        }
        search.context->flags &= ~1U;
        search.context->cursor = command += 8;
        return 2;
    }
    const std::uint8_t *const bytes = reinterpret_cast<const std::uint8_t *>(command);
    AddInventoryItem(static_cast<std::uint32_t>(*reinterpret_cast<const std::int16_t *>(command + 2)), bytes[4], bytes[5]);
    srchFinishPropIKItem(bytes[5]);
    if (!bytes[5]) {
        search.context->cursor = command += 8;
        return 2;
    }
    search.context->flags |= 1U;
    if (SetPlayerControlEnabled(0))
        search.context->flags |= 0x10U;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandFlagRestore(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return srchIsPropIKActive() ? 1 : 3;
    if (shenEvaluateFlagFor(static_cast<std::int32_t>(search.currentTarget->name), static_cast<std::int8_t>(command[2]))) {
        EV_SetCharDisplay(srchMapControl()->ryo, CVM_DisplayEnabled, 1);
        shenFreeTempModel();
        MT7Node *const root = GetPropRoot();
        UpdatePropSearchVisibility(root->child, 0, 0);
        srchRegisterAndPush(root->child);
        mtwkResetBlends();
        srchReset();
        camCtrlSetLookAroundMode();
        srchResetIK();
        mtwkSetOverridesEnabled(1);
    }
    search.context->cursor = command += 4;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandFlagWait(SearchTaskData &search) {
    const std::byte *const command = search.context->cursor;
    if (camCtrlIsTransitionActive()) {
        if (srchIsPropIKActive())
            return 1;
        return 3;
    }
    shenEvaluateFlagFor(static_cast<std::int32_t>(search.currentTarget->name), static_cast<std::int8_t>(command[2]));
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandChangeArea(SearchTaskData &search) {
    const std::byte *const command = search.context->cursor;
    shenQueueChangeModuleAndScene(
        SystemModule_Game, g_actNumber, shen_name(*reinterpret_cast<const std::uint32_t *>(command + 4)), *reinterpret_cast<const std::int16_t *>(command + 2));
    srchRequestRelease();
    search.context->cursor = 0;
    srchResetIK();
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandFilm(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (search.context->flags & 2U) {
        if (DoesScreenFilmColourExist(srchMapControl()->filmTask))
            return 1;
        if (search.context->flags & 0x10U) {
            SetPlayerControlEnabled(1);
            search.context->flags &= ~0x10U;
        }
        search.context->flags &= ~2U;
        search.context->cursor = command += 8;
        return 2;
    }
    WaitForNextTask_0(srchMapControl()->filmTask, 1, 30);
    if (SetPlayerControlEnabled(0))
        search.context->flags |= 0x10U;
    search.context->flags |= 2U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandIfStoryFlag(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (EV_GetShenmueFlag(*reinterpret_cast<const std::int16_t *>(command + 2)))
        search.context->cursor = command += 8;
    else
        search.context->cursor = command + *reinterpret_cast<const std::uint32_t *>(command + 4);
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandMessage(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (camCtrlIsTransitionActive()) {
        if (srchIsPropIKActive())
            return 1;
        return 3;
    }
    if (search.context->flags & 4U) {
        search.context->remainingTicks -= 1.0f;
        SearchResourceContext *const context = search.context;
        if (context->remainingTicks > 0.0f)
            return 1;
        if (context->flags & 0x10U) {
            SetPlayerControlEnabled(1);
            search.context->flags &= ~0x10U;
        }
        scenDestroySubtitles(srchMapControl()->scenarioTask);
        search.context->flags &= ~4U;
        search.context->cursor = command += 12;
        return 2;
    }
    if (reinterpret_cast<std::int16_t *>(command)[3]) {
        search.context->cursor = command += 12;
        return 2;
    }
    if (!reinterpret_cast<std::int16_t *>(command)[2])
        reinterpret_cast<std::int16_t *>(command)[3] = 1;
    if (search.context->strings) {
        SCEN_ShowEventMessage(srchMapControl()->scenarioTask, 1,
            reinterpret_cast<const char *>(*reinterpret_cast<const std::uint32_t *>(command + 8) + reinterpret_cast<std::uint32_t>(search.context->strings)));
        search.context->flags |= 4U;
        if (SetPlayerControlEnabled(0))
            search.context->flags |= 0x10U;
        search.context->remainingTicks = static_cast<float>(reinterpret_cast<std::int16_t *>(command)[1]);
        return 1;
    }
    search.context->cursor = 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandLight(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return srchIsPropIKActive() ? 1 : 3;
    if (*reinterpret_cast<const std::uint32_t *>(command + 4))
        EnableLightEntry(static_cast<std::uint32_t>(*reinterpret_cast<const std::int16_t *>(command + 2)));
    else
        DisableLightEntry(static_cast<std::uint32_t>(*reinterpret_cast<const std::int16_t *>(command + 2)));
    search.context->cursor = command += 8;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandSetInternalFlag(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return srchIsPropIKActive() ? 1 : 3;
    if (*reinterpret_cast<const std::uint32_t *>(command + 4))
        ClearOrSetShenmueFlags(*reinterpret_cast<const std::int16_t *>(command + 2), 1);
    else
        ClearOrSetShenmueFlags(*reinterpret_cast<const std::int16_t *>(command + 2), 0);
    search.context->cursor = command += 8;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandRandomMessage(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    const char *choices[3];
    std::int32_t count;
    if (camCtrlIsTransitionActive()) {
        if (srchIsPropIKActive())
            return 1;
        return 3;
    }
    count = 0;
    if (search.context->flags & 4U) {
        search.context->remainingTicks -= 1.0f;
        SearchResourceContext *const context = search.context;
        if (context->remainingTicks > 0.0f)
            return 1;
        if (context->flags & 0x10U) {
            SetPlayerControlEnabled(1);
            search.context->flags &= ~0x10U;
        }
        scenDestroySubtitles(srchMapControl()->scenarioTask);
        search.context->flags &= ~4U;
        search.context->cursor = command += 20;
        return 2;
    }
    if (reinterpret_cast<std::int16_t *>(command)[3]) {
        search.context->cursor = command += 20;
        return 2;
    }
    if (!reinterpret_cast<std::int16_t *>(command)[2])
        reinterpret_cast<std::int16_t *>(command)[3] = 1;
    for (std::int32_t index = 0; index < 3; ++index) {
        const std::int32_t *const offsets = reinterpret_cast<const std::int32_t *>(command + 8);
        const std::int32_t offset = offsets[index];
        if (offset != -1)
            choices[count++] = reinterpret_cast<const char *>(search.context->strings + offset);
    }
    if (count <= 0) {
        search.context->cursor = command += 20;
        return 2;
    }
    const std::int32_t selected = static_cast<std::int32_t>(dc::sdk::RandomFloat() * static_cast<float>(count));
    SCEN_ShowEventMessage(srchMapControl()->scenarioTask, 1, choices[selected]);
    search.context->remainingTicks = static_cast<float>(reinterpret_cast<std::int16_t *>(command)[1]);
    search.context->flags |= 4U;
    if (SetPlayerControlEnabled(0))
        search.context->flags |= 0x10U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandDialogue(SearchTaskData &search) {
    char name[12], code[16];
    const char *bank;
    if (camCtrlIsTransitionActive()) {
        if (srchIsPropIKActive())
            return 1;
        return 3;
    }
    std::byte *command = search.context->cursor;
    if (search.context->flags & 8U) {
        bank = reinterpret_cast<const char *>(search.context->strings + *reinterpret_cast<const std::uint32_t *>(command + 4));
        std::strcpy(name, bank);
        if (cvsHasValidConverseData(searchDialogue))
            return 1;
        if (search.context->flags & 0x10U) {
            SetPlayerControlEnabled(1);
            search.context->flags &= ~0x10U;
        }
        search.context->flags &= ~8U;
        search.context->cursor = command += 12;
        return 2;
    }
    if (static_cast<std::int8_t>(command[3])) {
        search.context->cursor = command += 12;
        return 2;
    }
    if (!static_cast<std::int8_t>(command[2]))
        command[3] = static_cast<std::byte>(1);
    bank = reinterpret_cast<const char *>(search.context->strings + *reinterpret_cast<const std::uint32_t *>(command + 4));
    std::strcpy(name, bank);
    const std::int32_t voice = *reinterpret_cast<const std::int32_t *>(command + 8);
    std::sprintf(code, "%s%c%c%c%c", bank, static_cast<std::int8_t>(voice), static_cast<std::int8_t>(static_cast<std::uint32_t>(voice) >> 8U),
        static_cast<std::int8_t>(voice << 8 >> 24), static_cast<std::int8_t>(voice >> 24));
    if (EV_FreeAuthorPak(1, name)) {
        searchDialogue = EV_PlayDialogue(code);
        search.context->flags |= 8U;
        if (SetPlayerControlEnabled(0))
            search.context->flags |= 0x10U;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandRandomDialogue(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    std::int8_t choices[3][4];
    std::int32_t count;
    if (camCtrlIsTransitionActive()) {
        if (srchIsPropIKActive())
            return 1;
        return 3;
    }
    if (search.context->flags & 8U) {
        if (cvsHasValidConverseData(randomSearchDialogue))
            return 1;
        if (search.context->flags & 0x10U) {
            SetPlayerControlEnabled(1);
            search.context->flags &= ~0x10U;
        }
        search.context->flags &= ~8U;
        search.context->cursor = command += 20;
        return 2;
    }
    if (static_cast<std::int8_t>(command[3])) {
        search.context->cursor = command += 20;
        return 2;
    }
    if (!static_cast<std::int8_t>(command[2]))
        command[3] = static_cast<std::byte>(1);
    std::uint32_t *const bank = reinterpret_cast<std::uint32_t *>(randomSearchBank);
    bank[0] = 0;
    bank[1] = 0;
    bank[2] = 0;
    bank[3] = 0;
    bank[4] = 0;
    bank[5] = 0;
    bank[6] = 0;
    bank[7] = 0;
    randomSearchDialogue = 0;
    count = 0;
    for (std::int32_t index = 0; index < 3; ++index) {
        *reinterpret_cast<std::uint32_t *>(choices[index]) = 0;
        const std::uint32_t *const voices = reinterpret_cast<const std::uint32_t *>(command + 8);
        const std::uint32_t choice = voices[index];
        if (choice)
            *reinterpret_cast<std::uint32_t *>(choices[count++]) = choice;
    }
    if (count <= 0) {
        search.context->cursor = command += 20;
        return 1;
    }
    const std::int32_t selected = static_cast<std::int32_t>(dc::sdk::RandomFloat() * static_cast<float>(count));
    const char *const bankName = reinterpret_cast<const char *>(search.context->strings + *reinterpret_cast<const std::uint32_t *>(command + 4));
    std::sprintf(randomSearchBank, "%s", bankName);
    char code[32];
    std::sprintf(code, "%s%c%c%c%c", randomSearchBank, choices[selected][0], choices[selected][1], choices[selected][2], choices[selected][3]);
    if (EV_FreeAuthorPak(1, randomSearchBank)) {
        randomSearchDialogue = EV_PlayDialogue(code);
        search.context->flags |= 8U;
        if (SetPlayerControlEnabled(0))
            search.context->flags |= 0x10U;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandWaitProp(SearchTaskData &search) {
    if (!srchIsPropIKSettled())
        return 0;
    search.context->cursor += 8;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandButtons(SearchTaskData &search) {
    if (srchIsPropIKMoving())
        return 0;
    std::byte *command = search.context->cursor;
    const std::int16_t mask = *reinterpret_cast<const std::int16_t *>(command + 2);
    if (mask & 0x210) {
        if (btnCheck(shen_name(SHEN_NAME4('B', 'T', 'C', 'C'))) || searchExitRequested) {
            search.context->cursor = command += 8;
            return 2;
        }
        RegisterButtonMediation(shen_name(SHEN_NAME4('B', 'T', 'C', 'C')), 0, 0x20004U);
    }
    if (mask & 0x24) {
        if (btnCheck(shen_name(SHEN_NAME4('B', 'T', 'E', 'V')))) {
            search.context->cursor = command += 8;
            return 2;
        }
        RegisterButtonMediation(shen_name(SHEN_NAME4('B', 'T', 'E', 'V')), 0, 0x40004U);
    }
    if (mask & 0x400) {
        if (btnCheck(shen_name(SHEN_NAME4('B', 'T', 'F', 'T')))) {
            search.context->cursor = command += 8;
            return 2;
        }
        RegisterButtonMediation(shen_name(SHEN_NAME4('B', 'T', 'F', 'T')), 0, 0x10004U);
    }
    if (mask & 2) {
        if (btnCheck(shen_name(SHEN_NAME4('B', 'T', 'I', 'F')))) {
            search.context->cursor = command += 8;
            return 2;
        }
        RegisterButtonMediation(shen_name(SHEN_NAME4('B', 'T', 'I', 'F')), 0, 0x80004U);
    }
    search.context->cursor = command + *reinterpret_cast<const std::uint32_t *>(command + 4);
    if (*reinterpret_cast<const std::int16_t *>(search.context->cursor) == 16 && SearchCommandButtons(search) == 2)
        return 2;
    search.context->cursor = command;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandBindProp(SearchTaskData &search) {
    srchBindPropIK(search);
    search.context->cursor += 4;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandReturnProp(SearchTaskData &search) {
    srchBeginPropIKReturn();
    search.context->cursor += 4;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandRestart(SearchTaskData &search) {
    activeSearchConfig = 0;
    search.context->cursor = 0;
    srchRestartPropIK();
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandChoices(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (search.context->flags & 0x20U) {
        if (GetShouldRenderIcons(ActionSelector))
            return 1;
        const std::uint32_t selected = shenGetCtrlSpeakIconFlag();
        if (search.context->flags & 0x10U) {
            SetPlayerControlEnabled(1);
            search.context->flags &= ~0x10U;
        }
        if (reinterpret_cast<const std::int16_t *>(command)[1] == 1)
            CloseMoneyIcon();
        search.context->flags &= ~0x20U;
        if (selected == static_cast<std::uint32_t>(static_cast<std::int32_t>(searchChoiceOptions[reinterpret_cast<const std::int16_t *>(command)[1]].first)))
            search.context->cursor = command += 12;
        else
            search.context->cursor = command + *reinterpret_cast<const std::uint32_t *>(command + 8);
        return 2;
    }
    if (reinterpret_cast<const std::uint16_t *>(command)[1] > 1) {
        search.context->cursor = command + *reinterpret_cast<const std::uint32_t *>(command + 8);
        return 2;
    }
    const std::int16_t *const half = reinterpret_cast<const std::int16_t *>(command);
    std::int16_t options[4];
    options[0] = -1;
    options[3] = -1;
    options[1] = searchChoiceOptions[half[1]].first;
    options[2] = searchChoiceOptions[half[1]].second;
    const std::int32_t timeout = half[2] ? half[2] : 32767;
    const std::uint16_t mode = half[3] ? 2 : 0;
    SetupControlSpeakAndUI_Internal(options, timeout, -1, mode, searchChoiceOptions[half[1]].style);
    if (half[1] == 1) {
        SetPlayerCashOrMoney(0, 2);
        KeepMoneyIconOpen();
    }
    if (SetPlayerControlEnabled(0))
        search.context->flags |= 0x10U;
    search.context->flags |= 0x20U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandDisableExamine(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const cursor = context.cursor;
    search.state |= 8U;
    context.cursor = cursor + 4;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandStepFlag(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return srchIsPropIKActive() ? 1 : 3;
    EV_stepSetFlags(static_cast<std::uint8_t>(*reinterpret_cast<const std::uint32_t *>(command + 4)), *reinterpret_cast<const std::int16_t *>(command + 2));
    search.context->cursor = command + 8;
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandIfInternalFlag(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (GetInternalFlag(*reinterpret_cast<const std::int16_t *>(command + 2)))
        search.context->cursor = command += 8;
    else
        search.context->cursor = command + *reinterpret_cast<const std::uint32_t *>(command + 4);
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandIfCash(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    if (GetPlayerStats(2) >= *reinterpret_cast<const std::int32_t *>(command + 4))
        search.context->cursor = command += 12;
    else
        search.context->cursor = command + *reinterpret_cast<const std::uint32_t *>(command + 8);
    return 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t SearchCommandTurnPlayer(SearchTaskData &search) {
    std::byte *command = search.context->cursor;
    const std::int32_t requested = *reinterpret_cast<const std::int16_t *>(command + 2);
    MotionWork *const motion = charFindMotionwork(srchMapControl()->ryo);
    if (srchGetPropIKIndex() || srchIsPropIKActive()) {
        search.context->cursor = command += 4;
        return 2;
    }
    Vector3 base, target, direction;
    srchGetLookTarget(&target);
    srchGetLookBase(&base);
    direction.x = base.x - target.x;
    direction.y = base.y - target.y;
    direction.z = base.z - target.z;
    const std::int16_t yaw = static_cast<std::int16_t>(static_cast<std::uint16_t>(dc::sdk::ArcTan2Fixed(-direction.x, -direction.z)));
    const std::int16_t delta = static_cast<std::int16_t>(requested - yaw);
    if (delta || direction.y != 0.0f) {
        std::int32_t step;
        if (delta > 910)
            step = 910;
        else if (delta < -910)
            step = -910;
        else
            step = delta;
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(step);
        Matrix4x4::TransformPoint(direction, direction);
        PopCurrentMatrix(1);
        base.x = target.x + direction.x;
        base.y = target.y;
        base.z = target.z + direction.z;
        if (delta)
            base.y += direction.y * (1.0f - ::fabsf(static_cast<float>(step)) / ::fabsf(static_cast<float>(delta)));
        srchSetLookBase(&base);
    }
    if (search.context->flags & 0x40U) {
        if (910.0f > ::fabsf(static_cast<float>(delta)) && 1.1920929e-07f > ::fabsf(direction.y) && !camCtrlIsTransitionActive() &&
            !mtwkSetExtraFlag(motion, 58, 1)) {
            search.context->flags &= ~0x40U;
            if (search.context->flags & 0x10U) {
                SetPlayerControlEnabled(1);
                search.context->flags &= ~0x10U;
            }
            srchRequestRelease();
            search.context->cursor = command += 4;
            return 2;
        }
    } else {
        mtwkRequestMotion(motion, 0, LowerBody);
        mtwkCopyTransition(motion, 0, requested, 13, 0);
        camCtrlSwitchToLockedOn(6);
        if (SetPlayerControlEnabled(0))
            search.context->flags |= 0x10U;
        search.context->flags |= 0x40U;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t randomSearchFlag;

void srchClearRandomSearchFlag() {
    randomSearchFlag = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline void srchReleaseCommandControl(SearchResourceContext &context);
static inline void srchAcquireCommandControl(SearchResourceContext &context);
static inline MapControlData *srchMapControl();
static std::int32_t SearchCommandEnd(SearchTaskData &search);
static std::int32_t SearchCommandItem(SearchTaskData &search);
static std::int32_t SearchCommandFlagRestore(SearchTaskData &search);
static std::int32_t SearchCommandFlagWait(SearchTaskData &search);
static std::int32_t SearchCommandChangeArea(SearchTaskData &search);
static std::int32_t SearchCommandFilm(SearchTaskData &search);
static std::int32_t SearchCommandIfStoryFlag(SearchTaskData &search);
static std::int32_t SearchCommandMessage(SearchTaskData &search);
static std::int32_t SearchCommandLight(SearchTaskData &search);
static std::int32_t SearchCommandSetInternalFlag(SearchTaskData &search);
static std::int32_t SearchCommandRandomMessage(SearchTaskData &search);
static std::int32_t SearchCommandDialogue(SearchTaskData &search);
static std::int32_t SearchCommandRandomDialogue(SearchTaskData &search);
static std::int32_t SearchCommandWaitProp(SearchTaskData &search);
static std::int32_t SearchCommandButtons(SearchTaskData &search);
static std::int32_t SearchCommandBindProp(SearchTaskData &search);
static std::int32_t SearchCommandReturnProp(SearchTaskData &search);
static std::int32_t SearchCommandRestart(SearchTaskData &search);
static std::int32_t SearchCommandChoices(SearchTaskData &search);
static std::int32_t SearchCommandDisableExamine(SearchTaskData &search);
static std::int32_t SearchCommandStepFlag(SearchTaskData &search);
static std::int32_t SearchCommandIfInternalFlag(SearchTaskData &search);
static std::int32_t SearchCommandIfCash(SearchTaskData &search);
static std::int32_t SearchCommandTurnPlayer(SearchTaskData &search);
std::int32_t srchExecuteCommands(SearchTaskData &search);
bool srchProcessSearchIK(SearchTaskData &search);
void srchReadSelectionSounds(const std::byte *configuration);
const SearchConfiguration *FindSearchConfiguration(const SearchTaskData &search, shen_name name);
void srchClearPropIKSlot(std::int32_t index);
void srchResetIK();
std::int32_t srchGetPropIKIndex();
bool srchIsPropIKSettled();
bool srchIsPropIKMoving();
bool srchIsPropIKActive();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t srchGetPropIKLookup(std::uint32_t marker);
#else
std::int32_t srchGetPropIKLookup(std::uint16_t marker);
#endif
void srchBindPropIK(SearchTaskData &search);
bool srchPushPropIK(SearchTaskData &search);
bool srchPopPropIK(SearchTaskData &search);
void srchFinishPropIKItem(std::int32_t animate);
void srchApplyObjectTransform(const MT7Node *node, std::uint32_t *yaw);
bool ApplyInverseSearchNode(const MT7Node &node);
void srchApplyPropHierarchyTransform(const MT7Node &node);
void srchApplyPropTargetTransform(const MT7Node &node, std::uint32_t *yaw);
void srchBeginPropIKReturn();
void srchRestartPropIK();
std::int32_t srchUpdatePropIK(SearchTaskData &search);
bool srchGetEulerAngles(const SearchTarget *target, Vector3 *position, std::uint32_t *yaw);

namespace {

Vector3 PropVector(float x, float y, float z);
Vector3i PropRotation(std::int32_t x, std::int32_t y, std::int32_t z);
Matrix4x4 ReadPropMatrix();
MT7Node *FindNestedPropMarker(MT7Node *node, std::uint32_t excludedMarker);
Matrix4x4 *Matrices(SearchPropIKSlot &slot);
Matrix4x4 *Targets(SearchPropIKSlot &slot);
SearchPropIKConfig &Config(SearchPropIKSlot &slot);
Vector3 Translation();
Vector3 TransformVector(const Vector3 &point);
void Translate(const Vector3 &point);
void SwizzleBasis(const Matrix4x4 &matrix);
std::int32_t Angle(float x, float z);
Character *Player();
MT7Node *MeshNode(MT7Node *node);
void SetMarkerVisible(MT7Node &node, bool visible);
void ApplyStagePose(SearchPropIKSlot &slot, std::uint32_t stage);
void GetStageTangents(SearchPropIKSlot &slot, std::int8_t stage, Vector3 &right, Vector3 &left);
void CaptureArmRelativeTransform(SearchPropIKSlot &slot);
void BuildPropTransforms(SearchPropIKSlot &slot);
void SetupPropHandCamera(SearchPropIKSlot &slot);
void UpdatePropChestAndCamera(SearchPropIKSlot &slot);
void CapturePropPivot(SearchPropIKSlot &slot);
void UpdatePropFromArm(SearchPropIKSlot &slot);
void RotateHeldProp(SearchTaskData &search, SearchPropIKSlot &slot);
void PlaySelectionSound(std::size_t index);
void FinishSlotTick(SearchPropIKSlot &slot);
void SolveStageGrip(SearchTaskData &search, SearchPropIKSlot &slot, std::uint32_t stage);

} // namespace
#else
void srchReadSelectionSounds(const std::byte *config);
void srchClearPropIKSlot(std::int32_t index);
std::int32_t srchGetPropIKLookup(std::uint16_t marker);
void srchBindPropIK(SearchTaskData &search);
bool srchPushPropIK(SearchTaskData &search);
bool srchPopPropIK(SearchTaskData &search);
void srchFinishPropIKItem(bool animate);
void srchBeginPropIKReturn();
bool srchUpdatePropIK(SearchTaskData &search);
std::int32_t srchGetCurrentIkIndex();
bool IsInvestigateMode();

namespace {

Matrix4x4 *Matrices(SearchPropIKSlot &slot);
Matrix4x4 *Targets(SearchPropIKSlot &slot);
SearchPropIKConfig &Config(SearchPropIKSlot &slot);
Vector3 Translation();
Vector3 TransformVector(const Vector3 &point);
void Translate(const Vector3 &point);
void SwizzleBasis(const Matrix4x4 &matrix);
std::int16_t Angle(float x, float z);
Character *Player();
MT7Node *MeshNode(MT7Node *node);
void SetMarkerVisible(MT7Node &node, bool visible);
void ApplyStagePose(SearchPropIKSlot &slot, std::uint32_t stage);
void GetStageTangents(SearchPropIKSlot &slot, std::uint32_t stage, Vector3 &right, Vector3 &left);
void CaptureArmRelativeTransform(SearchPropIKSlot &slot);
void BuildPropTransforms(SearchPropIKSlot &slot);
void SetupPropHandCamera(SearchPropIKSlot &slot);
void UpdatePropChestAndCamera(SearchPropIKSlot &slot);
void CapturePropPivot(SearchPropIKSlot &slot);
void UpdatePropFromArm(SearchPropIKSlot &slot);
void RotateHeldProp(SearchTaskData &search, SearchPropIKSlot &slot);
void PlaySelectionSound(std::size_t index);
void FinishSlotTick(SearchPropIKSlot &slot);
void SolveStageGrip(SearchTaskData &search, SearchPropIKSlot &slot, std::uint32_t stage);

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t srchExecuteCommands(SearchTaskData &search) {
    if (search.context->cursor) {
        for (;;) {
            const std::uint32_t operation = *reinterpret_cast<const std::int16_t *>(search.context->cursor);
            if (operation > 25)
                return 0;
            const SearchCommand command = searchCommands[operation];
            std::int32_t result;
            if (command) {
                result = command(search);
            } else {
                search.context->cursor = 0;
                activeSearchConfig = 0;
                break;
            }
            switch (result) {
            case 0:
            case 1:
            case 3:
                return result;
            case 2:
                break;
            }
        }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchProcessSearchIK(SearchTaskData &search) {
    CheckHelpIcon(0x100000, 0, CheckHelpIconOp_FrameOverlay);
    const std::int32_t result = srchExecuteCommands(search);
    if (result != 1) {
        if (result == 3)
            return false;
        if (!srchUpdatePropIK(search))
            return false;
    }
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

shen_name srchGetCurrentTargetName() {
    if (searchControl && searchControl->currentTarget)
        return searchControl->currentTarget->name;
    return shen_name(SHEN_NAME4('0', '0', '0', '0'));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchReadSelectionSounds(const std::byte *configuration) {
    std::int32_t index;
    for (index = 0; index < 3; ++index) {
        searchPropIKSelection[index].sound = 0;
        searchPropIKSelection[index].retained2 = 0;
        searchPropIKSelection[index].bank = 0;
        searchPropIKSelection[index].modifier = 0;
    }
    if (configuration) {
        for (index = 0; index < 3; ++index)
            std::memcpy(&searchPropIKSelection[index], configuration + index * 6 + 0x10, 6);
    }
}

#else

// @verified - p
void srchReadSelectionSounds(const std::byte *config) {
    for (std::size_t index = 0; index < 3; ++index) {
        std::memset(searchPropIKSelection.data() + 6 * index, 0, 5);
        if (config)
            std::memcpy(searchPropIKSelection.data() + 6 * index, config + 16 + 6 * index, 6);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
const SearchConfiguration *FindSearchConfiguration(const SearchTaskData &search, shen_name name) {
    if (name && search.context->resource) {
        const SearchConfiguration *record = reinterpret_cast<const SearchConfiguration *>(search.context->resource + 12);
        while (record->name) {
            if (record->name == name)
                return record;
            record = reinterpret_cast<const SearchConfiguration *>(reinterpret_cast<const std::byte *>(record) + record->size);
        }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchClearPropIKSlot(std::int32_t index) {
    if (g_searchPropIKSlots[index].node) {
        if (searchPropIKLookup[g_searchPropIKSlots[index].lookupIndex] == 7 && g_searchPropIKSlots[index].node->kind)
            g_searchPropIKSlots[index].node->kind = 0;
        if (g_searchPropIKSlots[index].node->mesh)
            g_searchPropIKSlots[index].node->transformOverride = 0;
    }
    g_searchPropIKSlots[index].flags = 0;
    g_searchPropIKSlots[index].active = 0;
    g_searchPropIKSlots[index].node = 0;
    g_searchPropIKSlots[index].configuration = 0;
    g_searchPropIKSlots[index].resource = 0;
    g_searchPropIKSlots[index].owner = 0;
    g_searchPropIKSlots[index].savedCursor = 0;
}

#else

// @verified - p
void srchClearPropIKSlot(std::int32_t index) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[index];
    if (slot.node) {
        if (searchPropIKLookup[slot.lookupIndex] == 7 && (static_cast<std::uint32_t>(slot.node->id) >> 16U))
            SetMarkerVisible(*slot.node, false);
        if (slot.node->xb01Offset)
            SetMT7NodeTransformOverride(*slot.node, nullptr);
    }
    slot.flags = 0;
    slot.active = 0;
    slot.node = nullptr;
    slot.character = nullptr;
    slot.resource = nullptr;
    slot.owner = nullptr;
    slot.frame = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchResetIK() {
    std::int32_t index;
    searchPropIKIndex = 0;
    for (index = 0; index < 4; ++index) {
        srchClearPropIKSlot(index);
        g_searchPropIKSlots[index].transformData = searchPropIKTransformData[index];
        g_searchPropIKSlots[index].auxiliaryData = searchPropIKAuxiliaryData[index];
    }
    searchPropIKRotationVelocity[0] = 0;
    searchPropIKRotationVelocity[1] = 0;
    for (index = 0; index < 3; ++index) {
        searchPropIKSelection[index].sound = 0;
        searchPropIKSelection[index].retained2 = 0;
        searchPropIKSelection[index].bank = 0;
        searchPropIKSelection[index].modifier = 0;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t srchGetPropIKLookup(std::uint32_t marker) {
    char text[2];
    if (static_cast<char>(marker) != 'M')
        return -1;
    text[0] = static_cast<char>(marker >> 8U);
    text[1] = 0;
    std::int32_t digit = text[0] - '0';
    if (static_cast<std::uint32_t>(digit) > 9U)
        digit = text[0] - 55;
    for (std::int32_t index = 0; index < 15; ++index)
        if (digit == searchPropIKLookup[index])
            return index;
    return -1;
}
#endif

#else

// @verified - p
std::int32_t srchGetPropIKLookup(std::uint16_t marker) {
    if (static_cast<std::uint8_t>(marker) != 'M')
        return -1;
    const std::int32_t character = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(marker >> 8U));
    std::int32_t digit = character - '0';
    if (static_cast<std::uint32_t>(digit) > 9U)
        digit = character - 55;
    for (std::int32_t index = 0; index < 15; ++index)
        if (searchPropIKLookup[index] == digit)
            return index;
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t srchGetPropIKIndex() {
    return searchPropIKIndex;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchIsPropIKSettled() {
    const SearchPropIKSlot *const slot = &g_searchPropIKSlots[searchPropIKIndex];
    if ((slot->active & 5U) == 4U)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchIsPropIKMoving() {
    const SearchPropIKSlot *const slot = &g_searchPropIKSlots[searchPropIKIndex];
    if (slot->active & 1U)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchIsPropIKActive() {
    const SearchPropIKSlot *const slot = &g_searchPropIKSlots[searchPropIKIndex];
    if (slot->active)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 PropVector(float x, float y, float z) {
    Vector3 v = {x, y, z};
    return v;
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3i PropRotation(std::int32_t x, std::int32_t y, std::int32_t z) {
    Vector3i v = {x, y, z};
    return v;
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 ReadPropMatrix() {
    Matrix4x4 m;
    matrix_stack::GetMatrix(&m);
    return m;
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MT7Node *FindNestedPropMarker(MT7Node *node, std::uint32_t excludedMarker) {
    const std::uint32_t marker = GetModelMarker(node);
    const std::uint8_t type = static_cast<std::uint8_t>(marker);
    if (marker != excludedMarker && type == 'M')
        return node;
    MT7Node *found;
    if (node->child) {
        found = FindNestedPropMarker(node->child, excludedMarker);
        if (found)
            return found;
    }
    if (node->sibling) {
        found = FindNestedPropMarker(node->sibling, excludedMarker);
        if (found)
            return found;
    }
    return 0;
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 *Matrices(SearchPropIKSlot &slot) {
    return reinterpret_cast<Matrix4x4 *>(slot.transformData);
}
#endif

} // namespace

#else

namespace {

Matrix4x4 *Matrices(SearchPropIKSlot &slot) {
    return reinterpret_cast<Matrix4x4 *>(slot.transformData);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 *Targets(SearchPropIKSlot &slot) {
    return reinterpret_cast<Matrix4x4 *>(slot.auxiliaryData);
}
#endif

} // namespace

#else

namespace {

Matrix4x4 *Targets(SearchPropIKSlot &slot) {
    return reinterpret_cast<Matrix4x4 *>(slot.auxiliaryData);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
SearchPropIKConfig &Config(SearchPropIKSlot &slot) {
    return *slot.configuration;
}
#endif

} // namespace

#else

namespace {

SearchPropIKConfig &Config(SearchPropIKSlot &slot) {
    return *static_cast<SearchPropIKConfig *>(slot.character);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 Translation() {
    Vector3 value;
    matrix_stack::GetTranslation(&value);
    return value;
}
#endif

} // namespace

#else

namespace {

Vector3 Translation() {
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 TransformVector(const Vector3 &point) {
    Vector3 result;
    Matrix4x4::Transform(point, result);
    return result;
}
#endif

} // namespace

#else

namespace {

Vector3 TransformVector(const Vector3 &point) {
    const Vector4 *const r = workTransformMatrix.rows;
    return {(point.y * r[1].x + point.x * r[0].x) + point.z * r[2].x, (point.y * r[1].y + point.x * r[0].y) + point.z * r[2].y,
        (point.y * r[1].z + point.x * r[0].z) + point.z * r[2].z};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void Translate(const Vector3 &point) {
    Matrix4x4::preTranslate(point.x, point.y, point.z);
}
#endif

} // namespace

#else

namespace {

void Translate(const Vector3 &point) {
    Matrix4x4::preTranslate(point.x, point.y, point.z);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SwizzleBasis(const Matrix4x4 &matrix) {
    Matrix4x4 current;
    matrix_stack::GetMatrix(&current);
    for (std::int32_t row = 0; row < 3; ++row) {
        current.rows[row] = matrix.rows[row];
        current.rows[row].w = 0.0f;
    }
    matrix_stack::LoadMatrix(&current);
}
#endif

} // namespace

#else

namespace {

void SwizzleBasis(const Matrix4x4 &matrix) {
    const float *const m = reinterpret_cast<const float *>(&matrix);
    workTransformMatrix.rows[0] = {m[0], m[1], m[2], 0};
    workTransformMatrix.rows[1] = {m[4], m[5], m[6], 0};
    workTransformMatrix.rows[2] = {m[8], m[9], m[10], 0};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t Angle(float x, float z) {
    return dc::sdk::ArcTan2Fixed(x, z);
}
#endif

} // namespace

#else

namespace {

std::int16_t Angle(float x, float z) {
    return static_cast<std::int16_t>(atan2_fixed16(x, z));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Character *Player() {
    return static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
}
#endif

} // namespace

#else

namespace {

Character *Player() {
    return static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MT7Node *MeshNode(MT7Node *node) {
    while (!node->mesh)
        node = node->child;
    return node;
}
#endif

} // namespace

#else

namespace {

MT7Node *MeshNode(MT7Node *node) {
    while (!node->xb01Offset)
        node = node->runtimeChild;
    return node;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMarkerVisible(MT7Node &node, bool visible) {
    node.kind = visible ? 1 : 0;
}
#endif

} // namespace

#else

namespace {

void SetMarkerVisible(MT7Node &node, bool visible) {
    node.id = std::bit_cast<std::int32_t>((static_cast<std::uint32_t>(node.id) & 0xffffU) | (visible ? 0x10000U : 0U));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool srchPopPropIK(SearchTaskData &search) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex - 1];
    MT7Node *const parent = slot.parent->parent;
    if (!parent)
        return false;
    MT7Node *const child = FindNestedPropMarker(parent, static_cast<std::uint32_t>(GetModelMarker(parent)));
    if (!child)
        return false;
    srchRegisterAndPush(child);
    srchClearPropIKSlot(searchPropIKIndex);
    --searchPropIKIndex;
    search.context->cursor = slot.savedCursor;
    const SearchConfiguration *config = FindSearchConfiguration(search, slot.name);
    srchReadSelectionSounds(reinterpret_cast<const std::byte *>(config));
    return true;
}
#endif

#else

// @verified - p
bool srchPopPropIK(SearchTaskData &search) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex - 1];
    MT7Node *const parent = slot.parent->runtimeParent;
    if (!parent)
        return false;
    MT7Node *const child = mtFindChildRecursive(parent, static_cast<std::uint32_t>(IsMDPXorMDP7(parent)));
    if (!child)
        return false;
    srchRegisterAndPush(child);
    srchClearPropIKSlot(searchPropIKIndex);
    --searchPropIKIndex;
    search.context->cursor = reinterpret_cast<std::byte *>(slot.frame);
    const std::span<const std::byte> config = srchGetConfigFor(search, shen_name{std::bit_cast<std::uint32_t>(slot.state)});
    srchReadSelectionSounds(config.data());
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void srchBindPropIK(SearchTaskData &search) {
    SearchPropIKSlot *const slot = &g_searchPropIKSlots[searchPropIKIndex];
    const shen_name name = search.currentTarget->name;
    if (static_cast<std::int8_t>(name) != 'M')
        return;
    slot->name = name;
    MT7Node *const parent = FindModelCharacterRecursive(GetPropRoot(), search.currentTarget->name);
    slot->parent = parent;
    if (parent->mesh)
        slot->node = parent;
    else
        slot->node = slot->parent->child;
    const std::int32_t lookup = srchGetPropIKLookup(search.currentTarget->name);
    slot->lookupIndex = lookup;
    if (lookup == -1)
        return;
    propAngles[0] = 0;
    propAngles[1] = 0;
    searchPropIKRotationVelocity[0] = 0;
    searchPropIKRotationVelocity[1] = 0;
    slot->configuration = &propConfigurations[lookup];
    slot->flags = 0;
    slot->active = 65;
    SetPlayerControlEnabled(0);
}
#endif

#else

// @verified - p
void srchBindPropIK(SearchTaskData &search) {
    const shen_name name = search.currentTarget->name;
    if (static_cast<std::uint8_t>(name.name) != 'M')
        return;
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    slot.state = std::bit_cast<std::int32_t>(name.name);
    slot.parent = FindModelCharacterRecursive(GetPropRoot(), name);
    slot.node = slot.parent->xb01Offset ? slot.parent : slot.parent->runtimeChild;
    slot.lookupIndex = srchGetPropIKLookup(static_cast<std::uint16_t>(name.name));
    if (slot.lookupIndex == -1)
        return;
    propAngles[0] = propAngles[1] = 0;
    searchPropIKResult = 0;
    slot.character = &propConfigurations[slot.lookupIndex];
    slot.flags = 0;
    slot.active = 65;
    SetPlayerControlEnabled(0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool srchPushPropIK(SearchTaskData &search) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    if (searchPropIKIndex == 3)
        return false;
    MT7Node *const root = slot.parent->child;
    if (root) {
        MT7Node *const child = FindNestedPropMarker(root, slot.name);
        if (!child)
            return false;
        slot.savedCursor = search.context->cursor;
        srchClearPropIKSlot(++searchPropIKIndex);
        srchRegisterAndPush(child);
        search.fovStyle = 7;
        srchLockTarget(search, search.targets[3]->name, false);
        return true;
    }
    return false;
}
#endif

#else

// @verified - p
bool srchPushPropIK(SearchTaskData &search) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    if (searchPropIKIndex == 3 || !slot.parent->runtimeChild)
        return false;
    MT7Node *const child = mtFindChildRecursive(slot.parent->runtimeChild, std::bit_cast<std::uint32_t>(slot.state));
    if (!child)
        return false;
    slot.frame = reinterpret_cast<std::uintptr_t>(search.context->cursor);
    srchClearPropIKSlot(++searchPropIKIndex);
    srchRegisterAndPush(child);
    search.fovStyle = 7;
    srchLockTarget(search, search.targets[3]->name, false);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyStagePose(SearchPropIKSlot &slot, std::uint32_t stage) {
    const SearchPropIKStage *const current = &slot.configuration->stages[stage];
    Vector3 offset = slot.configuration->gripOffset;
    if (current->jointTable >= 0) {
        if (slot.leftSide != 1) {
            srchSetJointTable(current->jointTable, 0);
            srchSetXformFor(&offset, 0);
        }
        if (slot.leftSide != 0) {
            offset.z = -offset.z;
            srchSetJointTable(current->jointTable, 1);
            srchSetXformFor(&offset, 1);
        }
    }
}
#endif

} // namespace

#else

namespace {

void ApplyStagePose(SearchPropIKSlot &slot, std::uint32_t stage) {
    SearchPropIKConfig &config = Config(slot);
    if (config.stages[stage].jointTable < 0)
        return;
    Vector3 offset = config.gripOffset;
    if (slot.leftSide != 1) {
        srchSetJointTable(config.stages[stage].jointTable, 0);
        srchSetXformFor(&offset, 0);
    }
    if (slot.leftSide != 0) {
        offset.z = -offset.z;
        srchSetJointTable(config.stages[stage].jointTable, 1);
        srchSetXformFor(&offset, 1);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void srchFinishPropIKItem(std::int32_t animate) {
    SearchPropIKSlot *const slot = &g_searchPropIKSlots[searchPropIKIndex];
    if (animate) {
        if (slot->leftSide != 1) {
            srchSetJointTable(3, 0);
            mtwkApplyHand(HandIK_Unblended, 0);
            srchSetNewTicksTimeFor(30, 0);
            mtwkApplyHand(HandIK_PathFollow, 0);
        }
        if (slot->leftSide != 0) {
            srchSetJointTable(3, 1);
            mtwkApplyHand(HandIK_Unblended, 1);
            srchSetNewTicksTimeFor(30, 1);
            mtwkApplyHand(HandIK_PathFollow, 1);
        }
        slot->active |= 0x108U;
    } else {
        mtwkResetBlends();
        MT7Node *mesh = slot->node;
        while (!mesh->mesh)
            mesh = mesh->child;
        mesh->transformOverride = 0;
        slot->node->kind = 1;
    }
    slot->flags = 6;
}
#endif

#else

// @verified - p
void srchFinishPropIKItem(bool animate) {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    if (animate) {
        for (std::uint8_t side = 0; side < 2; ++side) {
            if ((side == 0 && slot.leftSide == 1) || (side == 1 && slot.leftSide == 0))
                continue;
            srchSetJointTable(3, side);
            mtwkApplyHand(HandIKMode::Unblended, side);
            srchSetNewTicksTimeFor(30, side);
            mtwkApplyHand(HandIKMode::PathFollow, side);
        }
        slot.active |= 0x108U;
    } else {
        mtwkResetBlends();
        SetMT7NodeTransformOverride(*MeshNode(slot.node), nullptr);
        SetMarkerVisible(*slot.node, true);
    }
    slot.flags = 6;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchApplyObjectTransform(const MT7Node *node, std::uint32_t *yaw) {
    matrix_stack::TranslateVector(&node->position);
    Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    if (yaw)
        *yaw = (*yaw + static_cast<std::uint32_t>(node->rotation.y)) & 0xFFFF;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool ApplyInverseSearchNode(const MT7Node &node) {
    bool result;
    if (!node.transformOverride) {
        Vector3 inversePosition;
        inversePosition.x = -node.position.x;
        inversePosition.y = -node.position.y;
        inversePosition.z = -node.position.z;
        Matrix4x4::RotateAroundX_Wrapper(-static_cast<std::uint32_t>(node.rotation.x));
        Matrix4x4::RotateAroundY_Wrapper(-static_cast<std::uint32_t>(node.rotation.y));
        Matrix4x4::RotateAroundZ_Wrapper(-static_cast<std::uint32_t>(node.rotation.z));
        matrix_stack::TranslateVector(&inversePosition);
        result = false;
    } else {
        Matrix4x4 transform = *node.transformOverride;
        matrix_stack::PushUnitMatrix();
        matrix_stack::LoadMatrix(&transform);
        matrix_stack::InvertMatrix(0);
        matrix_stack::GetMatrix(&transform);
        PopCurrentMatrix(1);
        Matrix4x4::MultiplyWithPrePassMatrix(&transform);
        result = true;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchApplyPropHierarchyTransform(const MT7Node &node) {
    Vector3 position;
    GetPropVector4(&position);
    matrix_stack::TranslateVector(&position);
    matrix_stack::PushUnitMatrix();
    for (const MT7Node *parent = node.parent; parent; parent = parent->parent) {
        if (ApplyInverseSearchNode(*parent))
            break;
    }
    matrix_stack::InvertMatrix(0);
    Matrix4x4 transform;
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    Matrix4x4::MultiplyWithPrePassMatrix(&transform);
    srchApplyObjectTransform(&node, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchApplyPropTargetTransform(const MT7Node &node, std::uint32_t *yaw) {
    const MT7Node *current = &node;
    std::uint32_t *const angle = yaw;
    srchApplyPropHierarchyTransform(node);
    if (!current->mesh && current->child) {
        current = current->child;
        srchApplyObjectTransform(current, 0);
    }
    Vector3 translation;
    matrix_stack::GetTranslation(&translation);
    if (current->child && GetModelMarker(current) == GetModelMarker(current->child)) {
        srchApplyObjectTransform(current->child, 0);
        SetCurrentMatrixTranslation(&translation);
    }
    if (angle) {
        Matrix4x4 transform;
        matrix_stack::GetMatrix(&transform);
        *angle = static_cast<std::uint32_t>(dc::sdk::ArcTan2Fixed(transform.rows[2].x, transform.rows[2].z));
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdatePropFromArm(SearchPropIKSlot &slot) {
    Vector3 point, base;
    Matrix4x4 inverse;
    Vector3 origin;
    const std::uint32_t mode = slot.configuration->flags;
    matrix_stack::PushUnitMatrix();
    switch (mode & 0x38U) {
    case 0:
        am2LoadXmtrx(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[4]);
        matrix_stack::InvertMatrix(0);
        matrix_stack::GetMatrix(&inverse);
        matrix_stack::LoadMatrix(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        SwizzledBasis3x4IntoActiveFrom(slot.transformData->rows);
        matrix_stack::GetTranslation(&point);
        break;
    case 0x10: {
        am2LoadXmtrx(&slot.transformData[4]);
        matrix_stack::InvertMatrix(0);
        matrix_stack::GetMatrix(&inverse);
        matrix_stack::LoadMatrix(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        matrix_stack::GetTranslation(&point);
        am2LoadXmtrxPairs(slot.transformData);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::TransformPoint(point, point);
        am2LoadXmtrx(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        matrix_stack::GetTranslation(&origin);
        point.y = origin.y = 0;
        std::int32_t yaw = dc::sdk::ArcTan2Fixed(-point.x, -point.z);
        yaw -= dc::sdk::ArcTan2Fixed(-origin.x, -origin.z);
        yaw = static_cast<std::uint16_t>(yaw);
        am2LoadXmtrxPairs(slot.transformData);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        slot.configuration->stages[3].targetOffset = -(yaw / 182.04445f);
        point.x = 0;
        point.y = 0;
        point.z = 0;
        break;
    }
    default:
        am2LoadXmtrx(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[4]);
        matrix_stack::InvertMatrix(0);
        matrix_stack::GetMatrix(&inverse);
        matrix_stack::LoadMatrix(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        matrix_stack::GetTranslation(&point);
        break;
    }
    am2StoreXmtrx(&slot.transformData[1]);
    am2LoadXmtrxPairs(slot.transformData);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::TransformPoint(point, point);
    if ((mode & 1U) == 0)
        point.x = 0;
    if ((mode & 2U) == 0)
        point.y = 0;
    if ((mode & 4U) == 0)
        point.z = 0;
    am2LoadXmtrxPairs(slot.transformData);
    Matrix4x4::TransformPoint(point, point);
    if (!(mode & 0x40U)) {
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
        matrix_stack::GetTranslation(&base);
    }
    slot.transformData[1].rows[3].x = point.x;
    slot.transformData[1].rows[3].y = point.y;
    slot.transformData[1].rows[3].z = point.z;
    am2LoadXmtrxPairs(&slot.transformData[1]);
    Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
    am2StoreXmtrx(&slot.transformData[1]);
    if (mode & 0x40U)
        matrix_stack::GetTranslation(&base);
    PopCurrentMatrix(1);
    if (!srchIsPropIKSettled() && !(slot.active & 0x400U)) {
        if (slot.leftSide != 2) {
            GetSearchHandJointRotation(slot.leftSide == 1, &propWristAngles);
            matrix_stack::Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
            matrix_stack::TranslateVector(&propSinglePivot);
            matrix_stack::GetMatrix(&propArmTransform);
            PopCurrentMatrix(1);
        } else {
            Vector3 offset, rotated;
            offset.x = .1f;
            offset.z = offset.y = 0;
            propWristAngles.x = 0;
            propWristAngles.y = 0;
            propWristAngles.z = 0;
            matrix_stack::Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
            matrix_stack::GetMatrix(&propArmTransform);
            matrix_stack::LoadMatrix(mtwkGetSearchArmTransform(1));
            matrix_stack::GetTranslation(&propPairedPivot);
            propPairedPivot.x = (propPairedPivot.x + propArmTransform.rows[3].x) * .5f;
            propPairedPivot.y = (propPairedPivot.y + propArmTransform.rows[3].y) * .5f;
            propPairedPivot.z = (propPairedPivot.z + propArmTransform.rows[3].z) * .5f;
            matrix_stack::LoadMatrix(&propArmTransform);
            Matrix4x4::Transform(offset, rotated);
            propArmTransform.rows[3].x = propPairedPivot.x + rotated.x;
            propArmTransform.rows[3].y = propPairedPivot.y + rotated.y;
            propArmTransform.rows[3].z = propPairedPivot.z + rotated.z;
            matrix_stack::InvertMatrix(0);
            Matrix4x4::TransformPoint(propPairedPivot, propPairedPivot);
            propPairedPivot.x += offset.x;
            propPairedPivot.y += offset.y;
            propPairedPivot.z += offset.z;
            PopCurrentMatrix(1);
        }
    }
    if (!(slot.active & 0x100U))
        srchSetLookBase(&base);
}
#endif

} // namespace

#else

namespace {

void UpdatePropFromArm(SearchPropIKSlot &slot) {
    SearchPropIKConfig &config = Config(slot);
    Matrix4x4 *const matrices = Matrices(slot);
    const std::uint32_t mode = config.flags;
    Vector3 point{}, base;
    Matrix4x4_Push(&transformMatrix_prePass);
    if ((mode & 0x38U) == 0x10U) {
        Matrix4x4::SetActiveTransform(&matrices[4]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        const Matrix4x4 inverse = workTransformMatrix;
        Matrix4x4::SetActiveTransform(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        point = Translation();
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        Matrix4x4::TransformPoint(point, point);
        Matrix4x4::SetActiveTransform(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Vector3 origin = Translation();
        point.y = origin.y = 0;
        const std::int32_t yaw = Angle(-point.x, -point.z) - Angle(-origin.x, -origin.z);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
        config.stages[3].targetOffset = -(yaw / 182.04445f);
        point = {};
    } else {
        Matrix4x4::SetActiveTransform(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[4]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        const Matrix4x4 inverse = workTransformMatrix;
        Matrix4x4::SetActiveTransform(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        if (!(mode & 0x38U))
            SwizzleBasis(matrices[0]);
        point = Translation();
    }
    matrices[1] = workTransformMatrix;
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::TransformPoint(point, point);
    if (!(mode & 1U))
        point.x = 0;
    if (!(mode & 2U))
        point.y = 0;
    if (!(mode & 4U))
        point.z = 0;
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::TransformPoint(point, point);
    if (!(mode & 0x40U)) {
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        base = Translation();
    }
    matrices[1].rows[3].x = point.x;
    matrices[1].rows[3].y = point.y;
    matrices[1].rows[3].z = point.z;
    Matrix4x4::SetActiveTransform(&matrices[1]);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    matrices[1] = workTransformMatrix;
    if (mode & 0x40U)
        base = Translation();
    Matrix4x4::Pop(1);
    const std::uint32_t active = g_searchPropIKSlots[searchPropIKIndex].active;
    if (((active & 1U) || !(active & 4U)) && !(slot.active & 0x400U)) {
        if (slot.leftSide == 2) {
            propWristAngles = {};
            Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
            propArmTransform = workTransformMatrix;
            Matrix4x4::SetActiveTransform(mtwkGetSearchArmTransform(1));
            propPairedPivot = Translation();
            propPairedPivot = {(propPairedPivot.x + propArmTransform.rows[3].x) * .5f, (propPairedPivot.y + propArmTransform.rows[3].y) * .5f,
                (propPairedPivot.z + propArmTransform.rows[3].z) * .5f};
            Matrix4x4::SetActiveTransform(&propArmTransform);
            const Vector3 offset{0, .1f, 0};
            const Vector3 rotated = TransformVector(offset);
            propArmTransform.rows[3].x = propPairedPivot.x + rotated.x;
            propArmTransform.rows[3].y = propPairedPivot.y + rotated.y;
            propArmTransform.rows[3].z = propPairedPivot.z + rotated.z;
            Matrix4x4::InvertInPlace(&workTransformMatrix);
            Matrix4x4::TransformPoint(propPairedPivot, propPairedPivot);
            propPairedPivot.y += .1f;
        } else {
            Vector3i wrist;
            GetSearchHandJointRotation(slot.leftSide == 1, &wrist);
            propWristAngles = {static_cast<float>(static_cast<std::int16_t>(wrist.x)), static_cast<float>(static_cast<std::int16_t>(wrist.y)),
                static_cast<float>(static_cast<std::int16_t>(wrist.z))};
            Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
            Translate(propSinglePivot);
            propArmTransform = workTransformMatrix;
        }
        Matrix4x4::Pop(1);
    }
    if (!(slot.active & 0x100U))
        srchSetLookBase(&base);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 *GetCurrentMatrixBasis(Matrix4x4 *destination) {
    if (!destination)
        return 0;
    Matrix4x4 current;
    am2StoreXmtrx(&current);
    for (std::int32_t row = 2; row >= 0; --row) {
        destination->rows[row].z = current.rows[row].z;
        destination->rows[row].y = current.rows[row].y;
        destination->rows[row].x = current.rows[row].x;
    }
    return destination;
}

void BuildPropTransforms(SearchPropIKSlot &slot) {
    Vector3 origin = {0, 0, 0};
    Vector3 translation;
    Matrix4x4 mirrored;
    matrix_stack::PushUnitMatrix();
    srchApplyPropHierarchyTransform(*slot.parent);
    am2StoreXmtrx(slot.transformData);
    if (slot.parent != slot.node)
        srchApplyObjectTransform(slot.node, 0);
    am2StoreXmtrx(&slot.transformData[2]);
    matrix_stack::PushUnitMatrix();
    if (slot.node->child && GetModelMarker(slot.node) == GetModelMarker(slot.node->child))
        srchApplyObjectTransform(slot.node->child, 0);
    am2StoreXmtrx(&slot.transformData[3]);
    PopCurrentMatrix(1);
    Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
    GetCurrentMatrixBasis(slot.transformData);
    if (slot.leftSide == 2) {
        matrix_stack::GetTranslation(&translation);
        slot.transformData->rows[3].y = translation.y;
    }
    matrix_stack::Matrix4x4_Push(slot.transformData);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
    am2StoreXmtrx(&slot.transformData[2]);
    PopCurrentMatrix(1);
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    if (type == 6 || type == 9) {
        matrix_stack::Matrix4x4_Push(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        Matrix4x4::TransformPoint(origin, origin);
        PopCurrentMatrix(1);
        slot.configuration->stages[1].position.x = -origin.x;
        slot.configuration->endpoints[1].x = -origin.x - slot.configuration->endpoints[0].x;
    }
    for (std::int32_t i = 0; i < 4; ++i) {
        Matrix4x4 *const target = &slot.auxiliaryData[i];
        const SearchPropIKStage *const stage = &slot.configuration->stages[i];
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::TranslateVector(&stage->position);
        if (stage->rotation[2])
            Matrix4x4::RotateAroundZ_Wrapper(stage->rotation[2]);
        if (stage->rotation[1])
            Matrix4x4::RotateAroundY_Wrapper(stage->rotation[1]);
        if (stage->rotation[0])
            Matrix4x4::RotateAroundX_Wrapper(stage->rotation[0]);
        am2StoreXmtrx(target);
        PopCurrentMatrix(1);
    }
    am2LoadXmtrxPairs(slot.transformData);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::MultiplyWithPrePassMatrix(slot.auxiliaryData);
    am2StoreXmtrx(&slot.transformData[5]);
    mirrored = slot.transformData[5];
    mirrored.rows[3].x = -mirrored.rows[3].x;
    am2LoadXmtrxPairs(&slot.transformData[5]);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::MultiplyWithPrePassMatrix(&mirrored);
    am2StoreXmtrx(&slot.transformData[5]);
    PopCurrentMatrix(1);
}
#endif

} // namespace

#else

namespace {

void BuildPropTransforms(SearchPropIKSlot &slot) {
    Matrix4x4 *const matrices = Matrices(slot);
    SearchPropIKConfig &config = Config(slot);
    Matrix4x4_Push(&transformMatrix_prePass);
    srchApplyPropHierarchyTransform(*slot.parent);
    matrices[0] = workTransformMatrix;
    if (slot.parent != slot.node)
        srchApplyObjectTransform(slot.node, nullptr);
    matrices[2] = workTransformMatrix;
    Matrix4x4_Push(&transformMatrix_prePass);
    if (slot.node->runtimeChild && IsMDPXorMDP7(slot.node) == IsMDPXorMDP7(slot.node->runtimeChild))
        srchApplyObjectTransform(slot.node->runtimeChild, nullptr);
    matrices[3] = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
    for (std::size_t row = 0; row < 3; ++row) {
        matrices[0].rows[row].x = workTransformMatrix.rows[row].x;
        matrices[0].rows[row].y = workTransformMatrix.rows[row].y;
        matrices[0].rows[row].z = workTransformMatrix.rows[row].z;
    }
    if (slot.leftSide == 2) {
        Vector3 point = Translation();
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        Matrix4x4::TransformPoint(point, point);
        point.x = 0;
        Matrix4x4::InvertInPlace(&workTransformMatrix);
        Matrix4x4::TransformPoint(point, point);
        Matrix4x4::Pop(1);
        matrices[0].rows[3].x = point.x;
        matrices[0].rows[3].y = point.y;
        matrices[0].rows[3].z = point.z;
    }
    Matrix4x4_Push(&matrices[0]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    matrices[2] = workTransformMatrix;
    Matrix4x4::Pop(1);
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    if (type == 6 || type == 9) {
        Matrix4x4_Push(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        const Vector3 origin{};
        Vector3 point;
        Matrix4x4::TransformPoint(origin, point);
        Matrix4x4::Pop(1);
        config.stages[0].position.x = -point.x;
        config.endpoints[1].x = -point.x - config.endpoints[0].x;
    }
    for (std::size_t i = 0; i < 4; ++i) {
        const SearchPropIKStage &stage = config.stages[i];
        Matrix4x4_Push();
        Translate(stage.position);
        if (stage.rotation[2])
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(stage.rotation[2]));
        if (stage.rotation[1])
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(stage.rotation[1]));
        if (stage.rotation[0])
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(stage.rotation[0]));
        Targets(slot)[i] = workTransformMatrix;
        Matrix4x4::Pop(1);
    }
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(Targets(slot));
    matrices[5] = workTransformMatrix;
    Matrix4x4 mirrored = matrices[5];
    mirrored.rows[3].x = -mirrored.rows[3].x;
    Matrix4x4::SetActiveTransform(&matrices[5]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(&mirrored);
    matrices[5] = workTransformMatrix;
    Matrix4x4::Pop(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void CaptureArmRelativeTransform(SearchPropIKSlot &slot) {
    matrix_stack::Matrix4x4_Push(0);
    if (slot.active & 0x20U)
        am2LoadXmtrx(&reinterpret_cast<Matrix4x4 *>(slot.transformData)[1]);
    else {
        am2LoadXmtrx(&reinterpret_cast<Matrix4x4 *>(slot.transformData)[0]);
        Matrix4x4::MultiplyWithPrePassMatrix(&reinterpret_cast<Matrix4x4 *>(slot.transformData)[2]);
    }
    Matrix4x4::MultiplyWithPrePassMatrix(&reinterpret_cast<Matrix4x4 *>(slot.transformData)[3]);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::MultiplyWithPrePassMatrix(mtwkGetSearchArmTransform(slot.leftSide == 1));
    am2StoreXmtrx(&reinterpret_cast<Matrix4x4 *>(slot.transformData)[4]);
    PopCurrentMatrix(1);
}
#endif

} // namespace

#else

namespace {

void CaptureArmRelativeTransform(SearchPropIKSlot &slot) {
    Matrix4x4 *const matrices = Matrices(slot);
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&matrices[(slot.active & 0x20U) ? 1 : 0]);
    if (!(slot.active & 0x20U))
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(mtwkGetSearchArmTransform(slot.leftSide == 1));
    matrices[4] = workTransformMatrix;
    Matrix4x4::Pop(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void CapturePropPivot(SearchPropIKSlot &slot) {
    const Vector3 *const pivot = &slot.configuration->stages[0].position;
    Matrix4x4 *const target = &slot.auxiliaryData[2];
    Vector3 point;
    matrix_stack::Matrix4x4_Push(0);
    am2LoadXmtrx(&slot.transformData[1]);
    Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
    matrix_stack::TranslateVector(pivot);
    matrix_stack::GetTranslation(&point);
    target->rows[3].x = point.x;
    target->rows[3].y = point.y;
    target->rows[3].z = point.z;
    PopCurrentMatrix(1);
}
#endif

} // namespace

#else

namespace {

void CapturePropPivot(SearchPropIKSlot &slot) {
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&Matrices(slot)[1]);
    Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[3]);
    Translate(Config(slot).stages[0].position);
    const Vector3 point = Translation();
    Targets(slot)[2].rows[3].x = point.x;
    Targets(slot)[2].rows[3].y = point.y;
    Targets(slot)[2].rows[3].z = point.z;
    Matrix4x4::Pop(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetStageTangents(SearchPropIKSlot &slot, std::int8_t stage, Vector3 &right, Vector3 &left) {
    matrix_stack::Matrix4x4_Push(0);
    right.x = slot.configuration->stages[stage].axis == 0 ? slot.configuration->stages[stage].targetOffset : 0.0f;
    right.y = slot.configuration->stages[stage].axis == 1 ? slot.configuration->stages[stage].targetOffset : 0.0f;
    right.z = slot.configuration->stages[stage].axis == 2 ? slot.configuration->stages[stage].targetOffset : 0.0f;
    left.x = -right.x;
    left.y = right.y;
    left.z = right.z;
    am2LoadXmtrx(slot.transformData);
    Matrix4x4::Transform(right, right);
    Matrix4x4::Transform(left, left);
    PopCurrentMatrix(1);
}
#endif

} // namespace

#else

namespace {

void GetStageTangents(SearchPropIKSlot &slot, std::uint32_t stage, Vector3 &right, Vector3 &left) {
    Matrix4x4_Push();
    const SearchPropIKStage &row = Config(slot).stages[stage];
    right = {row.axis == 0 ? row.targetOffset : 0, row.axis == 1 ? row.targetOffset : 0, row.axis == 2 ? row.targetOffset : 0};
    left = {-right.x, right.y, right.z};
    Matrix4x4::SetActiveTransform(Matrices(slot));
    right = TransformVector(right);
    left = TransformVector(left);
    Matrix4x4::Pop(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdatePropChestAndCamera(SearchPropIKSlot &slot) {
    Vector3 position, look, endpoints[2];
    Vector3i rotation;
    float blend;
    endpoints[0] = slot.configuration->endpoints[0];
    endpoints[1] = slot.configuration->endpoints[1];
    matrix_stack::Matrix4x4_Push(slot.transformData);
    if (slot.leftSide != 2) {
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        matrix_stack::TranslateVector(&slot.configuration->stages[0].position);
        SwizzledBasis3x4IntoActiveFrom(slot.transformData->rows);
    }
    Matrix4x4::TransformPoint(endpoints[0], endpoints[0]);
    Matrix4x4::TransformPoint(endpoints[1], endpoints[1]);
    if (srchGetCurrentTotalTicks(slot.leftSide == 1))
        blend = static_cast<float>(srchGetTicksDifference(slot.leftSide == 1)) / static_cast<float>(srchGetCurrentTotalTicks(slot.leftSide == 1));
    else
        blend = 0;
    if (blend < 0.0f)
        blend = 0.0f;
    else if (blend > 1.0f)
        blend = 1.0f;
    if (slot.active & 0x20U) {
        if (slot.active & 2U)
            blend = 1 - blend;
    } else if (!(slot.active & 6U))
        blend = 1 - blend;
    if (slot.active & 4U)
        blend = 1;
    if (blend < .5f)
        blend = blend * (blend + blend);
    else
        blend = -2.0f * blend * blend + 4.0f * blend - 1.0f;
    if (searchPropIKLookup[slot.lookupIndex] == 8 || searchPropIKLookup[slot.lookupIndex] == 15) {
        if (!(slot.active & 6U))
            blend = 0;
        endpoints[0] = slot.configuration->endpoints[0];
        endpoints[1] = slot.configuration->endpoints[1];
        position.x = (1 - blend) * endpoints[0].x + blend * endpoints[1].x;
        position.y = (1 - blend) * endpoints[0].y + blend * endpoints[1].y;
        position.z = (1 - blend) * endpoints[0].z + blend * endpoints[1].z;
        matrix_stack::Matrix4x4_Push(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        matrix_stack::TranslateVector(&slot.configuration->stages[0].position);
        matrix_stack::GetTranslation(&endpoints[0]);
        matrix_stack::UnitCurrentMatrix();
        Matrix4x4::RotateAroundY_Wrapper(
            static_cast<std::int16_t>(static_cast<std::int16_t>(slot.configuration->stages[1].targetOffset * 8192.0f / 45.0f) * blend));
        Matrix4x4::TransformPoint(endpoints[0], endpoints[0]);
        matrix_stack::LoadMatrix(slot.transformData);
        matrix_stack::TranslateVector(&endpoints[0]);
        Matrix4x4::TransformPoint(position, position);
        PopCurrentMatrix(1);
    } else {
        position.x = (1 - blend) * endpoints[0].x + blend * endpoints[1].x;
        position.y = (1 - blend) * endpoints[0].y + blend * endpoints[1].y;
        position.z = (1 - blend) * endpoints[0].z + blend * endpoints[1].z;
    }
    if (!(slot.active & 6U))
        blend = 0;
    rotation.x = static_cast<std::int32_t>(
        (1 - blend) * static_cast<std::int16_t>(slot.configuration->angles[0].pitch) + static_cast<std::int16_t>(slot.configuration->angles[1].pitch) * blend);
    rotation.y = static_cast<std::int32_t>((1 - blend) * static_cast<std::int16_t>(slot.configuration->angles[0].yaw) +
                     static_cast<std::int16_t>(slot.configuration->angles[1].yaw) * blend) +
        slot.phase;
    rotation.z = 0;
    switch (searchPropIKLookup[slot.lookupIndex]) {
    case 6:
    case 7:
    case 8:
    case 9:
    case 15:
        break;
    default:
        matrix_stack::UnitCurrentMatrix();
        matrix_stack::TranslateVector(&position);
        Matrix4x4::RotateAroundY_Wrapper(rotation.y);
        Matrix4x4::RotateAroundX_Wrapper(rotation.x);
        matrix_stack::TranslateVector(&propLookOffset);
        matrix_stack::GetTranslation(&look);
        srchSetLookTarget(&look);
    }
    PopCurrentMatrix(1);
    SetSearchHandChestTransform(&position, &rotation);
}
#endif

} // namespace

#else

namespace {

void UpdatePropChestAndCamera(SearchPropIKSlot &slot) {
    SearchPropIKConfig &config = Config(slot);
    Matrix4x4 *const matrices = Matrices(slot);
    Vector3 endpoints[2]{config.endpoints[0], config.endpoints[1]};
    Matrix4x4_Push(&matrices[0]);
    if (slot.leftSide != 2) {
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        SwizzleBasis(matrices[0]);
    }
    Matrix4x4::TransformPoint(endpoints[0], endpoints[0]);
    Matrix4x4::TransformPoint(endpoints[1], endpoints[1]);
    const std::int32_t total = srchGetCurrentTotalTicks(slot.leftSide == 1);
    const float remaining = total ? static_cast<float>(srchGetTicksDifference(slot.leftSide == 1)) / total : 0;
    float blend = std::fmax(0, std::fmin(1, remaining));
    if ((slot.active & 0x20U) ? (slot.active & 2U) != 0 : (slot.active & 6U) == 0)
        blend = 1 - blend;
    if (slot.active & 4U)
        blend = 1;
    blend = blend < .5f ? blend * (blend + blend) : (blend * 4 - (blend + blend) * blend) - 1;
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    Vector3 position;
    if (type == 8 || type == 15) {
        if (!(slot.active & 6U))
            blend = 0;
        endpoints[0] = config.endpoints[0];
        endpoints[1] = config.endpoints[1];
        position = {(1 - blend) * endpoints[0].x + blend * endpoints[1].x, (1 - blend) * endpoints[0].y + blend * endpoints[1].y,
            (1 - blend) * endpoints[0].z + blend * endpoints[1].z};
        Matrix4x4_Push(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        Vector3 pivot = Translation();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>((config.stages[3].position.z * 8192.0f / 45.0f) * blend));
        Matrix4x4::TransformPoint(pivot, pivot);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Translate(pivot);
        Matrix4x4::TransformPoint(position, position);
        Matrix4x4::Pop(1);
    } else
        position = {(1 - blend) * endpoints[0].x + blend * endpoints[1].x, (1 - blend) * endpoints[0].y + blend * endpoints[1].y,
            (1 - blend) * endpoints[0].z + blend * endpoints[1].z};
    if (!(slot.active & 6U))
        blend = 0;
    const std::int32_t *const angles = reinterpret_cast<const std::int32_t *>(&config) + 2;
    Vector3i rotation{static_cast<std::int32_t>(angles[0] * (1 - blend) + angles[2] * blend),
        static_cast<std::int16_t>(slot.phase + static_cast<std::int32_t>(angles[1] * (1 - blend) + angles[3] * blend)), 0};
    if ((type < 6 || type > 9) && type != 15) {
        Matrix4x4::SetPrePassToTempTransform();
        Translate(position);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
        Matrix4x4::preTranslate(0, .4f, -.1f);
        const Vector3 look = Translation();
        srchSetLookTarget(&look);
    }
    Matrix4x4::Pop(1);
    SetSearchHandChestTransform(&position, &rotation);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RotateHeldProp(SearchTaskData &search, SearchPropIKSlot &slot) {
    std::int16_t effective[2];
    std::int16_t rotation[3];
    std::int32_t wrist[3];
    Matrix4x4 transform;
    Vector3 offset;
    std::int32_t configIndex = 0;
    Vector3 pivot = {0};
    std::int16_t acceleration, maximum;
    if (!srchIsPropIKSettled() && !(slot.active & 0x400U))
        return;
    switch (searchPropIKLookup[slot.lookupIndex]) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 11:
    case 12:
    case 13:
    case 14: {
        acceleration = 182;
        maximum = 910;
        wrist[0] = static_cast<std::int16_t>(propWristAngles.x);
        wrist[1] = static_cast<std::int16_t>(propWristAngles.y);
        wrist[2] = static_cast<std::int16_t>(propWristAngles.z);
        switch (searchPropIKLookup[slot.lookupIndex]) {
        case 3:
        case 4:
            configIndex = 1;
            pivot = propSinglePivot;
            break;
        case 1:
        case 11:
        case 13:
            configIndex = 2;
            pivot = propSinglePivot;
            break;
        case 14:
            configIndex = 3;
            pivot = propSinglePivot;
            break;
        case 2:
        case 12:
            configIndex = 4;
            pivot = propPairedPivot;
            acceleration = 91;
            maximum = 455;
            break;
        }
        for (std::int32_t i = 0; i < 2; i++)
            effective[i] = static_cast<std::int16_t>(wrist[reinterpret_cast<const std::int8_t *>(propAxisBytes)[i + configIndex * 22]]) + propAngles[i];
        if (slot.active & 0x400U) {
            for (std::int32_t axis = 0; axis < 2; axis++) {
                if (::fabsf(static_cast<float>(propAngles[axis])) > maximum * 3) {
                    if (propAngles[axis] > 0)
                        searchPropIKRotationVelocity[axis] -= acceleration;
                    else
                        searchPropIKRotationVelocity[axis] += acceleration;
                    if (::fabsf(static_cast<float>(searchPropIKRotationVelocity[axis])) > maximum)
                        searchPropIKRotationVelocity[axis] = searchPropIKRotationVelocity[axis] > 0 ? maximum : -maximum;
                } else if (::fabsf(static_cast<float>(searchPropIKRotationVelocity[axis])) > acceleration) {
                    if (searchPropIKRotationVelocity[axis] > 0)
                        searchPropIKRotationVelocity[axis] -= acceleration;
                    else
                        searchPropIKRotationVelocity[axis] += acceleration;
                } else
                    searchPropIKRotationVelocity[axis] = 0;
            }
            if (!searchPropIKRotationVelocity[0] && !searchPropIKRotationVelocity[1] && ::fabsf(static_cast<float>(propAngles[0])) <= maximum * 3 &&
                ::fabsf(static_cast<float>(propAngles[1])) <= maximum * 3)
                slot.active &= ~0x400U;
        } else {
            for (std::int32_t axis = 0; axis < 2; axis++) {
                std::int32_t adjusting = 0;
                for (std::int32_t direction = 0; direction < 2; direction++) {
                    if (!(search.input->held & *reinterpret_cast<const std::uint16_t *>(propAxisBytes + 6 + (configIndex * 22 + (axis * 4 + direction * 2)))))
                        continue;
                    if (reinterpret_cast<const std::int8_t *>(propAxisBytes + 2)[axis * 2 + direction + configIndex * 22]) {
                        if (effective[axis] >= *reinterpret_cast<const std::int16_t *>(propAxisBytes + 14 + (configIndex * 22 + (axis * 4 + direction * 2))))
                            continue;
                        searchPropIKRotationVelocity[axis] += acceleration;
                        if (searchPropIKRotationVelocity[axis] > maximum)
                            searchPropIKRotationVelocity[axis] = maximum;
                    } else {
                        if (effective[axis] <= *reinterpret_cast<const std::int16_t *>(propAxisBytes + 14 + (configIndex * 22 + (axis * 4 + direction * 2))))
                            continue;
                        searchPropIKRotationVelocity[axis] -= acceleration;
                        if (searchPropIKRotationVelocity[axis] < -maximum)
                            searchPropIKRotationVelocity[axis] = -maximum;
                    }
                    adjusting = 1;
                }
                if (!adjusting) {
                    if (searchPropIKRotationVelocity[axis] > 0) {
                        searchPropIKRotationVelocity[axis] -= acceleration;
                        if (searchPropIKRotationVelocity[axis] < 0)
                            searchPropIKRotationVelocity[axis] = 0;
                    }
                    if (searchPropIKRotationVelocity[axis] < 0) {
                        searchPropIKRotationVelocity[axis] += acceleration;
                        if (searchPropIKRotationVelocity[axis] > 0)
                            searchPropIKRotationVelocity[axis] = 0;
                    }
                }
            }
        }
        propAngles[0] += searchPropIKRotationVelocity[0];
        propAngles[1] += searchPropIKRotationVelocity[1];
        rotation[0] = rotation[1] = rotation[2] = 0;
        for (std::int32_t i = 0; i < 2; i++)
            rotation[reinterpret_cast<const std::int8_t *>(propAxisBytes)[i + configIndex * 22]] = propAngles[i];
        if (searchPropIKRotationVelocity[0] || searchPropIKRotationVelocity[1]) {
            mtwkApplyHand(HandIK_HoldTransform, slot.leftSide == 1);
            matrix_stack::Matrix4x4_Push(&propArmTransform);
            Matrix4x4::RotateAroundX_Wrapper(rotation[0]);
            Matrix4x4::RotateAroundZ_Wrapper(rotation[2]);
            Matrix4x4::RotateAroundY_Wrapper(rotation[1]);
            Matrix4x4::preTranslate(-pivot.x, -pivot.y, -pivot.z);
            get_x_from_target(&offset, slot.leftSide == 1);
            matrix_stack::TranslateVector(&offset);
            matrix_stack::GetMatrix(&transform);
        } else {
            mtwkApplyHand(HandIK_Disabled, slot.leftSide == 1);
            matrix_stack::Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
            matrix_stack::TranslateVector(&pivot);
            matrix_stack::GetMatrix(&transform);
            propArmTransform.rows[3].x = transform.rows[3].x;
            propArmTransform.rows[3].y = transform.rows[3].y;
            propArmTransform.rows[3].z = transform.rows[3].z;
            Matrix4x4::preTranslate(-pivot.x, -pivot.y, -pivot.z);
            get_x_from_target(&offset, slot.leftSide == 1);
            matrix_stack::TranslateVector(&offset);
            matrix_stack::GetMatrix(&transform);
        }
        SetSearchHandCurrentTransform(&transform, slot.leftSide == 1);
        PopCurrentMatrix(1);
        break;
    }
    default:
        slot.active &= ~0x400U;
    }
}
#endif

} // namespace

#else

namespace {

void RotateHeldProp(SearchTaskData &search, SearchPropIKSlot &slot) {
    const std::uint32_t active = g_searchPropIKSlots[searchPropIKIndex].active;
    if (((active & 1U) || !(active & 4U)) && !(slot.active & 0x400U))
        return;
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    if (type <= 0 || (type > 4 && (type < 11 || type > 14))) {
        slot.active &= ~0x400U;
        return;
    }
    std::int32_t configIndex = 0, maximum = 910, acceleration = 182;
    Vector3 pivot = propSinglePivot;
    if (type == 1 || type == 11 || type == 13)
        configIndex = 2;
    else if (type == 2 || type == 12) {
        configIndex = 4;
        pivot = propPairedPivot;
        maximum = 455;
        acceleration = 91;
    } else if (type == 3 || type == 4)
        configIndex = 1;
    else
        configIndex = 3;
    const std::uint8_t *const row = searchPropIKAxisBytes.data() + 22 * configIndex;
    std::int16_t velocities[2];
    std::memcpy(velocities, &searchPropIKResult, 4);
    const float wrist[3]{propWristAngles.x, propWristAngles.y, propWristAngles.z};
    const std::int16_t effective[2]{static_cast<std::int16_t>(propAngles[0] + static_cast<std::int16_t>(wrist[row[0]])),
        static_cast<std::int16_t>(propAngles[1] + static_cast<std::int16_t>(wrist[row[1]]))};
    if (slot.active & 0x400U) {
        for (std::size_t i = 0; i < 2; ++i) {
            if (std::abs(static_cast<std::int32_t>(propAngles[i])) <= maximum * 3) {
                if (std::abs(static_cast<std::int32_t>(velocities[i])) <= acceleration)
                    velocities[i] = 0;
                else
                    velocities[i] += velocities[i] > 0 ? -acceleration : acceleration;
            } else {
                velocities[i] += propAngles[i] > 0 ? -acceleration : acceleration;
                if (std::abs(static_cast<std::int32_t>(velocities[i])) > maximum)
                    velocities[i] = velocities[i] > 0 ? maximum : -maximum;
            }
        }
        if (!velocities[0] && !velocities[1] && std::abs(static_cast<std::int32_t>(propAngles[0])) <= maximum * 3 &&
            std::abs(static_cast<std::int32_t>(propAngles[1])) <= maximum * 3)
            slot.active &= ~0x400U;
    } else {
        for (std::size_t axis = 0; axis < 2; ++axis) {
            bool adjusting = false;
            for (std::size_t direction = 0; direction < 2; ++direction) {
                std::uint16_t button;
                std::int16_t limit;
                std::memcpy(&button, row + 6 + 4 * axis + 2 * direction, 2);
                std::memcpy(&limit, row + 14 + 4 * axis + 2 * direction, 2);
                if (!(search.input->held & button))
                    continue;
                if (row[2 + 2 * axis + direction]) {
                    if (effective[axis] >= limit)
                        continue;
                    velocities[axis] += acceleration;
                    if (velocities[axis] > maximum)
                        velocities[axis] = maximum;
                } else {
                    if (effective[axis] <= limit)
                        continue;
                    velocities[axis] -= acceleration;
                    if (velocities[axis] < -maximum)
                        velocities[axis] = -maximum;
                }
                adjusting = true;
            }
            if (!adjusting) {
                if (velocities[axis] > 0) {
                    velocities[axis] -= acceleration;
                    if (velocities[axis] < 0)
                        velocities[axis] = 0;
                }
                if (velocities[axis] < 0) {
                    velocities[axis] += acceleration;
                    if (velocities[axis] > 0)
                        velocities[axis] = 0;
                }
            }
        }
    }
    propAngles[0] += velocities[0];
    propAngles[1] += velocities[1];
    std::memcpy(&searchPropIKResult, velocities, 4);
    std::int16_t rotations[3]{};
    rotations[row[0]] = propAngles[0];
    rotations[row[1]] = propAngles[1];
    Matrix4x4 local;
    if (velocities[0] || velocities[1]) {
        mtwkApplyHand(HandIKMode::HoldTransform, slot.leftSide == 1);
        Matrix4x4_Push(&propArmTransform);
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotations[0]));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotations[2]));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotations[1]));
    } else {
        mtwkApplyHand(HandIKMode::Disabled, slot.leftSide == 1);
        Matrix4x4_Push(mtwkGetSearchArmTransform(slot.leftSide == 1));
        Translate(pivot);
        local = workTransformMatrix;
        propArmTransform.rows[3].x = local.rows[3].x;
        propArmTransform.rows[3].y = local.rows[3].y;
        propArmTransform.rows[3].z = local.rows[3].z;
    }
    Matrix4x4::preTranslate(-pivot.x, -pivot.y, -pivot.z);
    Vector3 offset;
    get_x_from_target(&offset, slot.leftSide == 1);
    Translate(offset);
    local = workTransformMatrix;
    SetSearchHandCurrentTransform(&local, slot.leftSide == 1);
    Matrix4x4::Pop(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PlaySelectionSound(std::size_t index) {
    const SearchPropIKSelection *selection = 0;
    if (index <= 2)
        selection = &searchPropIKSelection[index];
    if (selection)
        Loop3DSoundEffect((selection->modifier << 24) | (selection->bank << 19) | selection->sound);
}
#endif

} // namespace

#else

namespace {

void PlaySelectionSound(std::size_t index) {
    std::uint32_t sound;
    std::uint16_t modifier;
    std::memcpy(&sound, searchPropIKSelection.data() + 6 * index, 4);
    std::memcpy(&modifier, searchPropIKSelection.data() + 6 * index + 4, 2);
    const std::uint32_t material = (sound >> 24U) | (32U * (modifier & 0xffU));
    Loop3DSoundEffect((sound & 0xffffU) | (material << 19U));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FinishSlotTick(SearchPropIKSlot &slot) {
    if (slot.active & 2U) {
        UpdatePropFromArm(slot);
        if (slot.leftSide == 2)
            SearchIK_SetTarget(&Matrices(slot)[5]);
    }
    slot.active &= ~0x200U;
}
#endif

} // namespace

#else

namespace {

void FinishSlotTick(SearchPropIKSlot &slot) {
    if (slot.active & 2U) {
        UpdatePropFromArm(slot);
        if (slot.leftSide == 2)
            SearchIK_SetTarget(&Matrices(slot)[5]);
    }
    slot.active &= ~0x200U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SolveStageGrip(SearchTaskData &search, SearchPropIKSlot &slot, std::uint32_t stage) {
    if (!slot.resource || !slot.owner)
        return;
    Matrix4x4 allocation;
    if (slot.active & 6U)
        allocation = Matrices(slot)[1];
    else {
        matrix_stack::Matrix4x4_Push(Matrices(slot));
        Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[2]);
        allocation = ReadPropMatrix();
        PopCurrentMatrix(1);
    }
    MT7Node *const mesh = MeshNode(slot.node);
    MT7Node temporary = *mesh;
    temporary.child = 0;
    temporary.sibling = 0;
    temporary.transformOverride = &allocation;
    std::byte *const commands = reinterpret_cast<std::byte *>(propPoseCommands);
    std::byte *const begin = commands + 228 * Config(slot).pose;
    for (std::uint8_t side = 0; side < 2; ++side) {
        if ((side == 0 && slot.leftSide == 1) || (side == 1 && slot.leftSide == 0))
            continue;
        Vector3 offset;
        get_x_from_target(&offset, side);
        if (!side && search.currentTarget->name == SHEN_NAME4('M', '1', '1', '3'))
            offset.z += .02f;
        matrix_stack::Matrix4x4_Push(&Targets(slot)[stage]);
        if (side && slot.leftSide == 2)
            Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[5]);
        Matrix4x4::preTranslate(-offset.x, -offset.y, -offset.z);
        const Matrix4x4 local = ReadPropMatrix();
        PopCurrentMatrix(1);
        std::byte *const output = commands + 228 * side;
        std::memcpy(output, begin + 228, 228);
        SolveHandGripPose(static_cast<CharacterHandComponent *>(side ? slot.owner : slot.resource), &local, &temporary, &allocation, 0, begin, output);
        srchSetSearchJointAngles(side, begin, output, 0);
    }
}
#endif

} // namespace

#else

namespace {

void SolveStageGrip(SearchTaskData &search, SearchPropIKSlot &slot, std::uint32_t stage) {
    if (!slot.resource || !slot.owner)
        return;
    constexpr shen_name gripMatrixMemory{SHEN_NAME(MDMT)};
    Matrix4x4 *const allocation = static_cast<Matrix4x4 *>(HLib::AllocateTaskData(64, gripMatrixMemory));
    if (slot.active & 6U)
        *allocation = Matrices(slot)[1];
    else {
        Matrix4x4_Push(Matrices(slot));
        Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[2]);
        *allocation = workTransformMatrix;
        Matrix4x4::Pop(1);
    }
    MT7Node *const mesh = MeshNode(slot.node);
    MT7Node temporary{};
    temporary.sourceOffset = mesh->sourceOffset;
    temporary.id = mesh->id;
    temporary.position = mesh->position;
    temporary.rotation = mesh->rotation;
    temporary.scale = mesh->scale;
    temporary.xb01Offset = mesh->xb01Offset;
    temporary.meshType = mesh->meshType;
    temporary.runtimeMesh = mesh->runtimeMesh;
    temporary.runtimeParent = mesh->runtimeParent;
    temporary.borrowedMesh = mesh->borrowedMesh ? mesh->borrowedMesh : mesh;
    SetMT7NodeTransformOverride(temporary, allocation);
    std::byte *const commands = reinterpret_cast<std::byte *>(propPoseCommands.data());
    std::byte *const begin = commands + 228 * Config(slot).pose;
    for (std::uint8_t side = 0; side < 2; ++side) {
        if ((side == 0 && slot.leftSide == 1) || (side == 1 && slot.leftSide == 0))
            continue;
        Vector3 offset;
        get_x_from_target(&offset, side);
        if (!side && search.currentTarget->name.name == SHEN_NAME(M113))
            offset.z += .02f;
        Matrix4x4_Push(&Targets(slot)[stage]);
        if (side && slot.leftSide == 2)
            Matrix4x4::MultiplyWithPrePassMatrix(&Matrices(slot)[5]);
        Matrix4x4::preTranslate(-offset.x, -offset.y, -offset.z);
        const Matrix4x4 local = workTransformMatrix;
        Matrix4x4::Pop(1);
        std::byte *const output = commands + 228 * side;
        std::memcpy(output, begin + 228, 228);
        SolveHandGripPose(static_cast<HandParams *>(side ? slot.owner : slot.resource), &local, &temporary, allocation, 0, begin, output);
        srchSetSearchJointAngles(side, begin, output, 0);
    }
    HLib::FreeTaskData(allocation);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchBeginPropIKReturn() {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    const std::uint32_t active = slot.active | 0x401U;
    slot.active = active;
    if (!(active & 2U))
        slot.active = active | 0x40U;
    slot.flags = 0;
}

#else

// @verified - p
void srchBeginPropIKReturn() {
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    slot.active |= 0x401U;
    slot.flags = 0;
    if (!(slot.active & 2U))
        slot.active |= 0x40U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void srchRestartPropIK() {
    const std::int32_t index = searchPropIKIndex;
    SearchPropIKSlot &slot = g_searchPropIKSlots[index];
    const bool primary = index == 0;
    if (slot.active) {
        slot.active |= 0x80U;
        return;
    }
    if (!srchLockTarget(*searchControl, searchControl->currentTarget->name, primary))
        srchRequestRelease();
    searchControl->state &= ~4;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetupPropHandCamera(SearchPropIKSlot &slot) {
    Vector3 origin, position, target, base;
    Vector3i rotation;
    Character *const player = srchMapControl()->ryo;
    MotionWork *const motion = charFindMotionwork(player);
    if (!player || !motion)
        return;
    std::int32_t reverse = 0;
    if (slot.active & 0x20U)
        reverse = 1;
    slot.phase = dc::sdk::ArcTan2Fixed(slot.transformData->rows[2].x, slot.transformData->rows[2].z);
    matrix_stack::PushUnitMatrix();
    am2LoadXmtrxPairs(slot.transformData);
    if (slot.leftSide != 2) {
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        matrix_stack::TranslateVector(&slot.configuration->stages[0].position);
        SwizzledBasis3x4IntoActiveFrom(slot.transformData->rows);
    }
    matrix_stack::GetTranslation(&origin);
    matrix_stack::TranslateVector(&slot.configuration->endpoints[1]);
    matrix_stack::GetTranslation(&position);
    PopCurrentMatrix(1);
    slot.resource = GetCharacterHand(player, 0);
    slot.owner = GetCharacterHand(player, 1);
    rotation.x = slot.configuration->angles[reverse].pitch;
    rotation.y = slot.configuration->angles[reverse].yaw + slot.phase;
    rotation.z = 0;
    SetSearchHandOverrideEnabled(true);
    SetSearchHandChestTransform(&position, &rotation);
    switch (searchPropIKLookup[slot.lookupIndex]) {
    case 7:
    case 8:
    case 15:
        matrix_stack::PushUnitMatrix();
        am2LoadXmtrxPairs(slot.transformData);
        if (slot.leftSide != 2) {
            Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
            Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
            matrix_stack::TranslateVector(&slot.configuration->stages[0].position);
            SwizzledBasis3x4IntoActiveFrom(slot.transformData->rows);
        }
        matrix_stack::TranslateVector(&slot.configuration->endpoints[0]);
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        rotation.x = slot.configuration->angles[0].pitch;
        rotation.y = slot.configuration->angles[0].yaw + slot.phase;
        rotation.z = 0;
        break;
    case 6:
    case 9:
        position.x = (slot.configuration->endpoints[0].x + slot.configuration->endpoints[1].x) * .5f;
        position.y = (slot.configuration->endpoints[0].y + slot.configuration->endpoints[1].y) * .5f;
        position.z = (slot.configuration->endpoints[0].z + slot.configuration->endpoints[1].z) * .5f;
        matrix_stack::PushUnitMatrix();
        am2LoadXmtrxPairs(slot.transformData);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[3]);
        matrix_stack::TranslateVector(&slot.configuration->stages[0].position);
        SwizzledBasis3x4IntoActiveFrom(slot.transformData->rows);
        matrix_stack::TranslateVector(&position);
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        rotation.x = slot.configuration->angles[1].pitch - slot.configuration->angles[0].pitch;
        rotation.y = slot.configuration->angles[1].yaw - slot.configuration->angles[0].yaw;
        rotation.x /= 2;
        rotation.y /= 2;
        rotation.x += slot.configuration->angles[0].pitch;
        rotation.y = rotation.y + slot.configuration->angles[0].yaw + slot.phase;
        rotation.z = 0;
        break;
    }
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&position);
    Matrix4x4::RotateAroundY_Wrapper(rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(rotation.x);
    matrix_stack::TranslateVector(&propLookOffset);
    matrix_stack::GetTranslation(&target);
    am2LoadXmtrx(slot.transformData);
    Matrix4x4::MultiplyWithPrePassMatrix(&slot.transformData[2]);
    matrix_stack::GetTranslation(&base);
    PopCurrentMatrix(1);
    srchSetLookBase(&base);
    srchSetLookTarget(&target);
    srchSetRotY(static_cast<std::int16_t>(rotation.y + 0x8000));
    camCtrlSwitchToLockedOn(6);
    EV_SetCharDisplay(player, CVM_DisplayEnabled, 0);
}
#endif

} // namespace

#else

namespace {

void SetupPropHandCamera(SearchPropIKSlot &slot) {
    Character *const player = Player();
    MotionWork *const motion = FindMotionWork(player);
    if (!player || !motion)
        return;
    Matrix4x4 *const matrices = Matrices(slot);
    SearchPropIKConfig &config = Config(slot);
    const bool reverse = (slot.active & 0x20U) != 0;
    slot.phase = Angle(matrices[0].rows[2].x, matrices[0].rows[2].z);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::SetActiveTransform(&matrices[0]);
    if (slot.leftSide != 2) {
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        SwizzleBasis(matrices[0]);
    }
    Translate(config.endpoints[1]);
    Vector3 position = Translation();
    Matrix4x4::Pop(1);
    slot.resource = GetCharacterHand(player, 0);
    slot.owner = GetCharacterHand(player, 1);
    const std::int32_t *const angles = reinterpret_cast<const std::int32_t *>(&config) + 2;
    Vector3i rotation{angles[reverse ? 2 : 0], static_cast<std::int16_t>(slot.phase + angles[reverse ? 3 : 1]), 0};
    SetSearchHandOverrideEnabled(true);
    SetSearchHandChestTransform(&position, &rotation);
    const std::int32_t type = searchPropIKLookup[slot.lookupIndex];
    if (type == 6 || type == 9) {
        position = {(config.endpoints[0].x + config.endpoints[1].x) * .5f, (config.endpoints[0].y + config.endpoints[1].y) * .5f,
            (config.endpoints[0].z + config.endpoints[1].z) * .5f};
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
        Translate(config.stages[0].position);
        SwizzleBasis(matrices[0]);
        Translate(position);
        position = Translation();
        Matrix4x4::Pop(1);
        rotation.x = (angles[2] - angles[0]) / 2 + angles[0];
        rotation.y = static_cast<std::int16_t>(slot.phase + angles[1] + (angles[3] - angles[1]) / 2);
    } else if (type == 7 || type == 8 || type == 15) {
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::SetActiveTransform(&matrices[0]);
        if (slot.leftSide != 2) {
            Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
            Matrix4x4::MultiplyWithPrePassMatrix(&matrices[3]);
            Translate(config.stages[0].position);
            SwizzleBasis(matrices[0]);
        }
        Translate(config.endpoints[0]);
        position = Translation();
        Matrix4x4::Pop(1);
        rotation = {angles[0], static_cast<std::int16_t>(slot.phase + angles[1]), 0};
    }
    Matrix4x4_Push(&transformMatrix_prePass);
    Translate(position);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    Matrix4x4::preTranslate(0, .4f, -.1f);
    const Vector3 target = Translation();
    Matrix4x4::SetActiveTransform(&matrices[0]);
    Matrix4x4::MultiplyWithPrePassMatrix(&matrices[2]);
    const Vector3 base = Translation();
    Matrix4x4::Pop(1);
    srchSetLookBase(&base);
    srchSetLookTarget(&target);
    srchSetRotY(static_cast<std::int16_t>(rotation.y + 0x8000));
    camCtrlSwitchToLockedOn(6);
    EV_SetCharDisplay(player, CVM_DisplayEnabled, 0);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t srchUpdatePropIK(SearchTaskData &search) {
    SearchPropIKSlot *slot = &g_searchPropIKSlots[searchPropIKIndex];
    Vector3 right, left;
    const float blend = propIKBlendStep + SearchIK_GetMaxBlend();
    setSearchIKFlag(blend > 1.0f ? 1.0f : blend);
    if (slot->active & 8U) {
        const HandIKMode mode = getSearchHandConfigModeFor(slot->leftSide == 1);
        if (mode != HandIK_PathFollow && mode != HandIK_HandPose)
            slot->active = (slot->active & ~8U) | 0x200U;
        else
            UpdatePropChestAndCamera(*slot);
        if (slot->active & 2U) {
            UpdatePropFromArm(*slot);
            if (slot->leftSide == 2)
                SearchIK_SetTarget(&slot->transformData[5]);
        }
        return 1;
    }
    if (!(slot->active & 0x101U)) {
        if (!slot->active) {
            bool progressed = false;
            if (searchPropIKIndex) {
                if ((search.input->pressedPriority & 0x200U) || searchExitRequested) {
                    srchPopPropIK(search);
                    slot = &g_searchPropIKSlots[searchPropIKIndex];
                    srchBeginPropIKReturn();
                    progressed = true;
                    slot->active &= ~0x400U;
                } else
                    CheckHelpIcon(2, 0, CheckHelpIconOp_FrameOverlay);
            }
            if (srchGetPropIKLookup(search.currentTarget->name) != -1 && !(search.state & 8)) {
                if (btnCheck(examineButton)) {
                    srchBindPropIK(search);
                    progressed = true;
                } else
                    RegisterButtonMediation(examineButton, 0, 0x40004U);
            }
            if (!progressed)
                return 0;
        } else {
            if ((search.input->pressedPriority & 0x200U) || searchExitRequested)
                srchBeginPropIKReturn();
            else {
                CheckHelpIcon(2, 0, CheckHelpIconOp_FrameOverlay);
                RotateHeldProp(search, *slot);
            }
        }
    }
    if (slot->active & 0x400U) {
        RotateHeldProp(search, *slot);
        if (slot->active & 2U) {
            UpdatePropFromArm(*slot);
            if (slot->leftSide == 2)
                SearchIK_SetTarget(&slot->transformData[5]);
        }
        return 1;
    }
    if (slot->active & 0x40U) {
        MotionWork *const work = charFindMotionwork(static_cast<Character *>(srchMapControl()->ryo));
        if (!mtwkSetRegionFlag(work, 66, 1, LowerBody)) {
            mtwkRequestMotion(work, 0xf002, LowerBody);
            mtwkSetRegionFlag(work, 71, 0, LowerBody);
            return 1;
        }
        slot->leftSide = slot->configuration->hand;
        if (!(slot->active & 0x20U))
            BuildPropTransforms(*slot);
        else {
            switch (searchPropIKLookup[slot->lookupIndex]) {
            case 6:
                slot->leftSide = 1;
                break;
            case 9:
                slot->leftSide = 0;
                break;
            case 7:
                if (slot->node->kind)
                    slot->node->kind = 0;
                break;
            }
        }
        if (BeginSearchHand(static_cast<Character *>(srchMapControl()->ryo), slot->leftSide, 0)) {
            setSearchIKFlag(0);
            SetupPropHandCamera(*slot);
            slot->active &= ~0x40U;
        }
        SetDefaultVS();
        UpdatePropChestAndCamera(*slot);
        return 1;
    }
    switch (slot->flags) {
    case 0: {
        if (shenCheckCurrentFoVModeIs2())
            return 1;
        EV_SetCharDisplay(static_cast<Character *>(srchMapControl()->ryo), CVM_DisplayEnabled, 1);
        mtwkSetOverridesEnabled(0);
        if (slot->active & 0x20U)
            ApplyStagePose(*slot, 2);
        else
            ApplyStagePose(*slot, 0);
        if (slot->leftSide != 1)
            mtwkApplyHand(HandIK_Unblended, 0);
        if (slot->leftSide != 0)
            mtwkApplyHand(HandIK_Unblended, 1);
        if (!(slot->active & 2U))
            WriteHandPoseCommand(static_cast<CharacterHandComponent *>(slot->resource), reinterpret_cast<std::byte *>(propPoseCommands[2]));
        std::int32_t stage = 0;
        if (slot->active & 0x20U) {
            stage = 2;
            if (slot->active & 2U)
                stage = 3;
        }
        GetStageTangents(*slot, stage, right, left);
        if (slot->leftSide != 1) {
            right.x += slot->auxiliaryData[stage].rows[3].x;
            right.y += slot->auxiliaryData[stage].rows[3].y;
            right.z += slot->auxiliaryData[stage].rows[3].z;
            srchSetTargetsWithParams(0, &slot->auxiliaryData[stage], &right, 0, static_cast<float>(slot->configuration->stages[stage].ticks));
            mtwkApplyHand(HandIK_PathFollow, 0);
        }
        if (slot->leftSide) {
            if (slot->leftSide == 1) {
                left.x += slot->auxiliaryData[stage].rows[3].x;
                left.y += slot->auxiliaryData[stage].rows[3].y;
                left.z += slot->auxiliaryData[stage].rows[3].z;
                srchSetTargetsWithParams(1, &slot->auxiliaryData[stage], &left, 0, static_cast<float>(slot->configuration->stages[stage].ticks));
                mtwkApplyHand(HandIK_PathFollow, 1);
            } else if (stage != 3) {
                Matrix4x4 target;
                matrix_stack::Matrix4x4_Push(&slot->auxiliaryData[stage]);
                Matrix4x4::MultiplyWithPrePassMatrix(&slot->transformData[5]);
                matrix_stack::GetMatrix(&target);
                PopCurrentMatrix(1);
                left.x += target.rows[3].x;
                left.y += target.rows[3].y;
                left.z += target.rows[3].z;
                srchSetTargetsWithParams(1, &target, &left, 0, static_cast<float>(slot->configuration->stages[stage].ticks));
                mtwkApplyHand(HandIK_PathFollow, 1);
            }
        }
        slot->active |= 8U;
        if ((slot->active & 0x22U) == 0x22U) {
            slot->flags = 5;
            slot->active &= ~4U;
            PlaySelectionSound(2);
        } else {
            slot->flags = 1;
            if (slot->resource && slot->owner) {
                Matrix4x4 allocation;
                MT7Node temporary;
                Vector3 rightOffset;
                Matrix4x4 rightBase;
                Vector3 leftOffset;
                Matrix4x4 leftBase;
                if (slot->active & 6U)
                    allocation = slot->transformData[1];
                else {
                    matrix_stack::Matrix4x4_Push(slot->transformData);
                    Matrix4x4::MultiplyWithPrePassMatrix(&slot->transformData[2]);
                    matrix_stack::GetMatrix(&allocation);
                    PopCurrentMatrix(1);
                }
                MT7Node *mesh = slot->node;
                while (!mesh->mesh)
                    mesh = mesh->child;
                temporary = *mesh;
                temporary.sibling = 0;
                temporary.child = 0;
                temporary.transformOverride = &allocation;
                if (slot->leftSide != 1) {
                    get_x_from_target(&rightOffset, 0);
                    matrix_stack::Matrix4x4_Push(&slot->auxiliaryData[stage]);
                    Matrix4x4::preTranslate(-rightOffset.x, -rightOffset.y, -rightOffset.z);
                    matrix_stack::GetMatrix(&rightBase);
                    PopCurrentMatrix(1);
                    std::memcpy(propPoseCommands[0], propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose) + 1], sizeof(propPoseCommands[0]));
                    SolveHandGripPose(static_cast<CharacterHandComponent *>(slot->resource), &rightBase, &temporary, &allocation, 0,
                        reinterpret_cast<std::byte *>(propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose)]),
                        reinterpret_cast<std::byte *>(propPoseCommands[0]));
                    srchSetSearchJointAngles(0, reinterpret_cast<std::byte *>(propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose)]),
                        reinterpret_cast<std::byte *>(propPoseCommands[0]), 0);
                }
                if (slot->leftSide != 0) {
                    get_x_from_target(&leftOffset, 1);
                    matrix_stack::Matrix4x4_Push(&slot->auxiliaryData[stage]);
                    if (slot->leftSide == 2)
                        Matrix4x4::MultiplyWithPrePassMatrix(&slot->transformData[5]);
                    Matrix4x4::preTranslate(-leftOffset.x, -leftOffset.y, -leftOffset.z);
                    matrix_stack::GetMatrix(&leftBase);
                    PopCurrentMatrix(1);
                    std::memcpy(propPoseCommands[1], propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose) + 1], sizeof(propPoseCommands[1]));
                    SolveHandGripPose(static_cast<CharacterHandComponent *>(slot->owner), &leftBase, &temporary, &allocation, 0,
                        reinterpret_cast<std::byte *>(propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose)]),
                        reinterpret_cast<std::byte *>(propPoseCommands[1]));
                    srchSetSearchJointAngles(1, reinterpret_cast<std::byte *>(propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose)]),
                        reinterpret_cast<std::byte *>(propPoseCommands[1]), 0);
                }
            }
        }
        break;
    }
    case 1: {
        std::int32_t stage = 1;
        if (slot->active & 0x20U)
            stage = 3;
        if (slot->configuration->stages[stage].axis > 2)
            SetSearchHandRotatingTarget(slot->leftSide == 1, &slot->auxiliaryData[stage], slot->transformData, slot->configuration->stages[stage].axis - 3,
                static_cast<std::uint16_t>(slot->configuration->stages[stage].targetOffset * 8192.0f / 45.0f), slot->configuration->stages[stage].ticks);
        else {
            Vector3 stageRight, stageLeft;
            GetStageTangents(*slot, stage, stageRight, stageLeft);
            if (slot->leftSide != 1) {
                stageRight.x += slot->auxiliaryData[stage].rows[3].x;
                stageRight.y += slot->auxiliaryData[stage].rows[3].y;
                stageRight.z += slot->auxiliaryData[stage].rows[3].z;
                srchSetTargetsWithParams(
                    slot->leftSide == 1, &slot->auxiliaryData[stage], &stageRight, 0, static_cast<float>(slot->configuration->stages[stage].ticks));
            } else {
                stageLeft.x += slot->auxiliaryData[stage].rows[3].x;
                stageLeft.y += slot->auxiliaryData[stage].rows[3].y;
                stageLeft.z += slot->auxiliaryData[stage].rows[3].z;
                srchSetTargetsWithParams(
                    slot->leftSide == 1, &slot->auxiliaryData[stage], &stageLeft, 0, static_cast<float>(slot->configuration->stages[stage].ticks));
            }
        }
        if (slot->leftSide != 1)
            mtwkApplyHand(HandIK_Unblended, 0);
        if (slot->leftSide != 0)
            mtwkApplyHand(HandIK_Unblended, 1);
        mtwkApplyHand(HandIK_PathFollow, slot->leftSide == 1);
        slot->active = (slot->active | 0xaU) & ~4U;
        MT7Node *mesh = slot->node;
        while (!mesh->mesh)
            mesh = mesh->child;
        mesh->transformOverride = &slot->transformData[1];
        CaptureArmRelativeTransform(*slot);
        if (slot->active & 0x20U) {
            slot->flags = 5;
            slot->retained24 = 0;
            PlaySelectionSound(2);
        } else {
            slot->flags = 2;
            PlaySelectionSound(0);
        }
        break;
    }
    case 2:
        if (slot->configuration->flags & 0x40U)
            slot->active = (slot->active & ~1U) | 0x34U;
        else {
            switch (searchPropIKLookup[slot->lookupIndex]) {
            case 0:
            case 6:
            case 8:
            case 9:
            case 15:
                CapturePropPivot(*slot);
                break;
            case 7:
                slot->node->kind = 1;
                break;
            }
            if (searchPropIKLookup[slot->lookupIndex] == 7) {
                if (slot->leftSide != 1)
                    srchSetJointTable(3, 0);
                if (slot->leftSide != 0)
                    srchSetJointTable(3, 1);
            }
            if (slot->leftSide != 1) {
                std::memcpy(propPoseCommands[0], propPoseCommands[2], sizeof(propPoseCommands[0]));
                srchSetNewTicksTimeFor(15, 0);
                mtwkApplyHand(HandIK_PathFollow, 0);
                srchSetSearchJointAngles(0, reinterpret_cast<std::byte *>(propPoseCommands[0]), 0, 1);
            }
            if (slot->leftSide != 0) {
                std::memcpy(propPoseCommands[1], propPoseCommands[2], sizeof(propPoseCommands[1]));
                srchSetNewTicksTimeFor(15, 1);
                mtwkApplyHand(HandIK_PathFollow, 1);
                srchSetSearchJointAngles(1, reinterpret_cast<std::byte *>(propPoseCommands[1]), 0, 1);
            }
            slot->active = (slot->active | 0xcU) & ~2U;
            slot->flags = 3;
        }
        break;
    case 3:
        if (slot->active & 0x200U) {
            slot->active = (slot->active & ~1U) | 0x30U;
            mtwkResetBlends();
            EV_SetCharDisplay(static_cast<Character *>(srchMapControl()->ryo), CVM_DisplayEnabled, 0);
            if (!srchPushPropIK(search)) {
                switch (searchPropIKLookup[slot->lookupIndex]) {
                case 0:
                case 6:
                case 7:
                case 8:
                case 9:
                case 15:
                    camCtrlSwitchToLockedOn(7);
                    break;
                }
            }
        }
        break;
    case 4:
        break;
    case 5: {
        switch (searchPropIKLookup[slot->lookupIndex]) {
        case 3:
        case 4:
        case 14:
            if (slot->leftSide != 1)
                srchSetJointTable(3, 0);
            if (slot->leftSide != 0)
                srchSetJointTable(3, 1);
            break;
        }
        if (slot->leftSide != 1) {
            std::memcpy(propPoseCommands[0], propPoseCommands[2], sizeof(propPoseCommands[0]));
            srchSetNewTicksTimeFor(30, 0);
            mtwkApplyHand(HandIK_PathFollow, 0);
            srchSetSearchJointAngles(0, reinterpret_cast<std::byte *>(propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose)]),
                reinterpret_cast<std::byte *>(propPoseCommands[0]), 1);
        }
        if (slot->leftSide != 0) {
            std::memcpy(propPoseCommands[1], propPoseCommands[2], sizeof(propPoseCommands[1]));
            srchSetNewTicksTimeFor(30, 1);
            mtwkApplyHand(HandIK_PathFollow, 1);
            srchSetSearchJointAngles(1, reinterpret_cast<std::byte *>(propPoseCommands[static_cast<std::int8_t>(slot->configuration->pose)]),
                reinterpret_cast<std::byte *>(propPoseCommands[1]), 1);
        }
        MT7Node *mesh = slot->node;
        while (!mesh->mesh)
            mesh = mesh->child;
        mesh->transformOverride = 0;
        slot->active = (slot->active | 8U) & ~2U;
        slot->flags = 7;
        PlaySelectionSound(1);
        break;
    }
    case 6: {
        mtwkResetBlends();
        EV_SetCharDisplay(static_cast<Character *>(srchMapControl()->ryo), CVM_DisplayEnabled, 0);
        MT7Node *mesh = slot->node;
        while (!mesh->mesh)
            mesh = mesh->child;
        mesh->transformOverride = 0;
        slot->node->kind = 1;
        if (search.context->flags & 1U)
            break;
        if (searchPropIKIndex) {
            std::uint32_t pushed;
            if (GetModelMarker(slot->node->parent) == GetModelMarker(slot->node->parent->child))
                pushed = srchRegisterAndPush(slot->node->parent->child->sibling);
            else
                pushed = srchRegisterAndPush(slot->node->parent->child);
            if (pushed) {
                srchClearPropIKSlot(searchPropIKIndex);
                srchLockTarget(search, search.targets[3]->name, false);
            } else {
                srchPopPropIK(search);
                slot = &g_searchPropIKSlots[searchPropIKIndex];
                srchBeginPropIKReturn();
                slot->active |= 0x100U;
            }
        } else {
            search.state |= 2;
            srchRegisterAndPush(slot->node->parent->child);
            srchResetIK();
        }
        break;
    }
    case 7: {
        mtwkResetBlends();
        bool root = false;
        EV_SetCharDisplay(static_cast<Character *>(srchMapControl()->ryo), CVM_DisplayEnabled, 0);
        if (!searchPropIKIndex) {
            root = true;
            search.fovStyle = 1;
            srchSetLookTarget(0);
            SetPlayerControlEnabled(1);
        }
        if (!srchLockTarget(search, slot->name, root))
            srchRequestRelease();
        srchClearPropIKSlot(searchPropIKIndex);
        if (!(slot->active & 0x80U))
            search.state |= 4;
        slot->active &= ~0x80U;
        break;
    }
    }
    if (slot->active & 2U) {
        UpdatePropFromArm(*slot);
        if (slot->leftSide == 2)
            SearchIK_SetTarget(&slot->transformData[5]);
    }
    slot->active &= ~0x200U;
    return 1;
}
#endif

#else

// @verified - p
bool srchUpdatePropIK(SearchTaskData &search) {
    SearchPropIKSlot *slot = &g_searchPropIKSlots[searchPropIKIndex];
    setSearchIKFlag(std::fmin(1, SearchIK_GetMaxBlend() + 0.033333335f));
    if (slot->active & 8U) {
        const std::uint32_t mode = static_cast<std::uint32_t>(getSearchHandConfigModeFor(slot->leftSide == 1));
        if (((mode - 1U) & 0xfffffffbU) != 0) {
            slot->active &= ~8U;
            slot->active |= 0x200U;
        } else
            UpdatePropChestAndCamera(*slot);
        if (slot->active & 2U) {
            UpdatePropFromArm(*slot);
            if (slot->leftSide == 2)
                SearchIK_SetTarget(&Matrices(*slot)[5]);
        }
        return true;
    }
    if (!(slot->active & 0x101U)) {
        if (slot->active) {
            if ((search.input->pressedPriority & 0x200U) || searchExitRequested)
                srchBeginPropIKReturn();
            else {
                CheckHelpIcon(2, 0, CheckHelpIconOp::FrameOverlay);
                RotateHeldProp(search, *slot);
            }
        } else {
            if (searchPropIKIndex) {
                if ((search.input->pressedPriority & 0x200U) || searchExitRequested) {
                    srchPopPropIK(search);
                    slot = &g_searchPropIKSlots[searchPropIKIndex];
                    srchBeginPropIKReturn();
                    slot->active &= ~0x400U;
                } else
                    CheckHelpIcon(2, 0, CheckHelpIconOp::FrameOverlay);
            }
            if (!slot->active) {
                if (srchGetPropIKLookup(static_cast<std::uint16_t>(search.currentTarget->name.name)) != -1 && !(search.state & 8U)) {
                    if (btnCheck(examineButton))
                        srchBindPropIK(search);
                    else
                        RegisterButtonMediation(examineButton, nullptr, (ButtonMediationShowButtonHint | ButtonMediationBindA));
                }
                if (!slot->active)
                    return false;
            }
        }
    }
    if (slot->active & 0x400U) {
        RotateHeldProp(search, *slot);
        if (slot->active & 2U) {
            UpdatePropFromArm(*slot);
            if (slot->leftSide == 2)
                SearchIK_SetTarget(&Matrices(*slot)[5]);
        }
        return true;
    }
    if (slot->active & 0x40U) {
        Character *const player = Player();
        MotionWork *const work = FindMotionWork(player);
        if (mtwkSetRegionFlag(work, 66, 1, MotionRegion::LowerBody)) {
            slot->leftSide = Config(*slot).hand;
            if (!(slot->active & 0x20U))
                BuildPropTransforms(*slot);
            else {
                const std::int32_t type = searchPropIKLookup[slot->lookupIndex];
                if (type == 6)
                    slot->leftSide = 1;
                else if (type == 7)
                    SetMarkerVisible(*slot->node, false);
                else if (type == 9)
                    slot->leftSide = 0;
            }
            if (BeginSearchHand(player, slot->leftSide, 0)) {
                setSearchIKFlag(0);
                SetupPropHandCamera(*slot);
                slot->active &= ~0x40U;
            }
            SetDefaultVS();
            lightConfigurationChanged = 1;
            UpdatePropChestAndCamera(*slot);
        } else {
            mtwkRequestMotion(work, 0xf002, MotionRegion::LowerBody);
            mtwkSetRegionFlag(work, 71, 0, MotionRegion::LowerBody);
        }
        return true;
    }
    SearchPropIKConfig &config = Config(*slot);
    const std::int32_t type = searchPropIKLookup[slot->lookupIndex];
    std::byte *const commands = reinterpret_cast<std::byte *>(propPoseCommands.data());
    std::byte *const pose = commands + 228 * config.pose;
    Matrix4x4 *const matrices = Matrices(*slot);
    switch (slot->flags) {
    case 0: {
        if (shenCheckCurrentFoVModeIs2())
            return true;
        EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 1);
        mtwkSetOverridesEnabled(0);
        ApplyStagePose(*slot, (slot->active & 0x20U) ? 2 : 0);
        for (std::uint8_t side = 0; side < 2; ++side)
            if ((side == 0 && slot->leftSide != 1) || (side == 1 && slot->leftSide != 0))
                mtwkApplyHand(HandIKMode::Unblended, side);
        if (!(slot->active & 2U))
            WriteHandPoseCommand(*static_cast<HandParams *>(slot->resource), commands + 456);
        const std::uint32_t stage = (slot->active & 0x20U) ? ((slot->active & 2U) | 4U) >> 1U : 0;
        Vector3 right, left;
        GetStageTangents(*slot, stage, right, left);
        if (slot->leftSide != 1) {
            const Matrix4x4 &target = Targets(*slot)[stage];
            Vector3 tangent{right.x + target.rows[3].x, right.y + target.rows[3].y, right.z + target.rows[3].z};
            srchSetTargetsWithParams(0, &target, &tangent, nullptr, config.stages[stage].ticks);
            mtwkApplyHand(HandIKMode::PathFollow, 0);
        }
        if (slot->leftSide && !(slot->leftSide == 2 && stage == 3)) {
            Matrix4x4 target = Targets(*slot)[stage];
            if (slot->leftSide == 2) {
                Matrix4x4_Push(&target);
                Matrix4x4::MultiplyWithPrePassMatrix(&matrices[5]);
                target = workTransformMatrix;
                Matrix4x4::Pop(1);
            }
            const Vector3 tangent{left.x + target.rows[3].x, left.y + target.rows[3].y, left.z + target.rows[3].z};
            srchSetTargetsWithParams(1, &target, &tangent, nullptr, config.stages[stage].ticks);
            mtwkApplyHand(HandIKMode::PathFollow, 1);
        }
        slot->active |= 8U;
        if ((slot->active & 0x22U) == 0x22U) {
            slot->active &= ~4U;
            slot->flags = 5;
            PlaySelectionSound(2);
        } else {
            slot->flags = 1;
            SolveStageGrip(search, *slot, stage);
        }
        break;
    }
    case 1: {
        const std::uint32_t stage = ((slot->active & 0x20U) | 0x10U) >> 4U;
        const SearchPropIKStage &row = config.stages[stage];
        if (row.axis <= 2) {
            Vector3 right, left;
            GetStageTangents(*slot, stage, right, left);
            const bool side = slot->leftSide == 1;
            const Matrix4x4 &target = Targets(*slot)[stage];
            const Vector3 &vector = side ? left : right;
            const Vector3 tangent{vector.x + target.rows[3].x, vector.y + target.rows[3].y, vector.z + target.rows[3].z};
            srchSetTargetsWithParams(side, &target, &tangent, nullptr, row.ticks);
        } else
            SetSearchHandRotatingTarget(
                slot->leftSide == 1, &Targets(*slot)[stage], matrices, row.axis - 3, static_cast<std::uint16_t>(row.targetOffset * 8192.0f / 45.0f), row.ticks);
        for (std::uint8_t side = 0; side < 2; ++side)
            if ((side == 0 && slot->leftSide != 1) || (side == 1 && slot->leftSide != 0))
                mtwkApplyHand(HandIKMode::Unblended, side);
        mtwkApplyHand(HandIKMode::PathFollow, slot->leftSide == 1);
        slot->active = (slot->active & 0xfffffff1U) | 0xaU;
        SetMT7NodeTransformOverride(*MeshNode(slot->node), &matrices[1]);
        CaptureArmRelativeTransform(*slot);
        if (slot->active & 0x20U) {
            slot->parameter = 0;
            slot->flags = 5;
            PlaySelectionSound(2);
        } else {
            slot->flags = 2;
            PlaySelectionSound(0);
        }
        break;
    }
    case 2:
        if (config.flags & 0x40U)
            slot->active = (slot->active & 0xffffffcaU) | 0x34U;
        else {
            if (type == 0 || type == 6 || type == 8 || type == 9 || type == 15)
                CapturePropPivot(*slot);
            else if (type == 7)
                SetMarkerVisible(*slot->node, true);
            for (std::uint8_t side = 0; side < 2; ++side) {
                if ((side == 0 && slot->leftSide == 1) || (side == 1 && slot->leftSide == 0))
                    continue;
                if (type == 7)
                    srchSetJointTable(3, side);
                std::memcpy(commands + 228 * side, commands + 456, 228);
                srchSetNewTicksTimeFor(15, side);
                mtwkApplyHand(HandIKMode::PathFollow, side);
                srchSetSearchJointAngles(side, commands + 228 * side, nullptr, 1);
            }
            slot->flags = 3;
            slot->active = (slot->active | 0xcU) & ~2U;
        }
        break;
    case 3:
        if (slot->active & 0x200U) {
            slot->active = (slot->active & 0xffffffceU) | 0x30U;
            mtwkResetBlends();
            EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 0);
            if (!srchPushPropIK(search) && (type == 0 || type == 6 || type == 7 || type == 15))
                camCtrlSwitchToLockedOn(7);
        }
        break;
    case 5:
        if (type == 3)
            ApplyStagePose(*slot, 3);
        else if (type == 4 || type == 14) {
            if (slot->leftSide != 1)
                srchSetJointTable(3, 0);
            if (slot->leftSide != 0)
                srchSetJointTable(3, 1);
        }
        for (std::uint8_t side = 0; side < 2; ++side) {
            if ((side == 0 && slot->leftSide == 1) || (side == 1 && slot->leftSide == 0))
                continue;
            std::memcpy(commands + 228 * side, commands + 456, 228);
            srchSetNewTicksTimeFor(30, side);
            mtwkApplyHand(HandIKMode::PathFollow, side);
            srchSetSearchJointAngles(side, pose, commands + 228 * side, 1);
        }
        SetMT7NodeTransformOverride(*MeshNode(slot->node), nullptr);
        slot->active = (slot->active & 0xfffffff5U) | 8U;
        slot->flags = 7;
        PlaySelectionSound(1);
        break;
    case 6:
        mtwkResetBlends();
        EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 0);
        SetMT7NodeTransformOverride(*MeshNode(slot->node), nullptr);
        SetMarkerVisible(*slot->node, true);
        if (!(search.context->flags & 1U)) {
            if (searchPropIKIndex) {
                MT7Node *const parent = slot->node->runtimeParent;
                MT7Node *child = parent->runtimeChild;
                if (IsMDPXorMDP7(parent) == IsMDPXorMDP7(child))
                    child = child->runtimeSibling;
                if (srchRegisterAndPush(child)) {
                    srchClearPropIKSlot(searchPropIKIndex);
                    srchLockTarget(search, search.targets[3]->name, false);
                } else {
                    srchPopPropIK(search);
                    slot = &g_searchPropIKSlots[searchPropIKIndex];
                    srchBeginPropIKReturn();
                    slot->active |= 0x100U;
                }
            } else {
                search.state |= 2U;
                srchRegisterAndPush(slot->node->runtimeParent->runtimeChild);
                srchResetIK();
            }
        }
        break;
    case 7: {
        mtwkResetBlends();
        EV_SetCharDisplay(Player(), CVM_DisplayEnabled, 0);
        const bool root = searchPropIKIndex == 0;
        if (root) {
            search.fovStyle = 1;
            srchSetLookTarget(nullptr);
            SetPlayerControlEnabled(1);
        }
        if (!srchLockTarget(search, shen_name{std::bit_cast<std::uint32_t>(slot->state)}, root))
            srchRequestRelease();
        srchClearPropIKSlot(searchPropIKIndex);
        if (!(slot->active & 0x80U))
            search.state |= 4U;
        slot->active &= ~0x80U;
        break;
    }
    default:
        break;
    }
    FinishSlotTick(*slot);
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t srchGetCurrentIkIndex() {
    return searchPropIKIndex;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool IsInvestigateMode() {
    return g_searchPropIKSlots[searchPropIKIndex].active != 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchGetEulerAngles(const SearchTarget *target, Vector3 *position, std::uint32_t *yaw) {
    if (!target || !position)
        return false;
    const std::uint32_t kind = target->name;
    if (yaw)
        *yaw = 0;
    position->x = position->y = position->z = 0.0f;
    if (static_cast<std::int8_t>(kind) == 'M') {
        MT7Node *const node = FindModelCharacterRecursive(GetPropRoot(), target->name);
        if (!node)
            goto failed;
        matrix_stack::PushUnitMatrix();
        srchApplyPropTargetTransform(*node, yaw);
        matrix_stack::GetTranslation(position);
        PopCurrentMatrix(1);
        if (yaw)
            *yaw = (target->yaw + *yaw) & 0xffffU;
    } else {
        *position = target->position;
        if (yaw)
            *yaw = target->yaw & 0xffffU;
    }
    return true;
failed:
    return false;
}

#endif

} // namespace s2
} // namespace shenmue
