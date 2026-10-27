#include "s2/areas/area_op02.hpp"
#include "s2/author.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/system_modules.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <cstring>

namespace shenmue::s2::areas {
namespace {
std::int32_t op02WaveEnabled{};
std::int32_t op02WaveFrame{};

std::array<std::byte, 0x34> op02PresentationParameters{};
std::array<std::int32_t, 3> op02TextureReferences{};
std::array<std::uint16_t, 4> op02PreviousTriggers{};
std::array<std::byte, 0x100> op02DistanceKernel{};
constexpr std::array<float, 64> op02PairwiseKernel{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.02160000056028366f, -0.045099999755620956f, -0.10199999809265137f,
    -0.20200000703334808f, -0.45100000500679016f};
constexpr GlowParameters op02FirstGlow{10, 0.0f, {0.4343920052051544f, 0.579725980758667f, 0.8518329858779907f, 0.24970600008964539f}};
constexpr GlowParameters op02LaterGlow{10, 0.0f, {0.1901569962501526f, 0.24313700199127197f, 0.23902000486850739f, 0.2515980005264282f}};
void OP02Nullsub() {
}
void StoreOp02PresentationWord(std::size_t offset, std::uint32_t value) {
    std::memcpy(op02PresentationParameters.data() + offset, &value, sizeof(value));
}
void ReadOp02Triggers() {
    for (std::uint8_t index = 0; index < 4; ++index)
        op02PreviousTriggers[index] = static_cast<std::uint16_t>(EVT_GetControllerInput(index, INPUT_DEVICE_STATE));
}
} // namespace

// @verified - p
void AreaOP02Init() {
    StoreOp02PresentationWord(0x00, 1139802112U);
    StoreOp02PresentationWord(0x04, 1106247680U);
    StoreOp02PresentationWord(0x08, 1065353216U);
    StoreOp02PresentationWord(0x0c, 1065353216U);
    StoreOp02PresentationWord(0x10, 1125515264U);
    op02PresentationParameters[0x14] = std::byte{};
    op02PresentationParameters[0x15] = std::byte{};
    StoreOp02PresentationWord(0x18, 0);
    StoreOp02PresentationWord(0x1e, 8388736U);
    op02PresentationParameters[0x22] = std::byte{};
    StoreOp02PresentationWord(0x28, 0);
    StoreOp02PresentationWord(0x2c, 0);
    StoreOp02PresentationWord(0x30, 0);
    op02TextureReferences.fill(-1);
    op02PreviousTriggers.fill(1);
    op02WaveEnabled = 0;
    ResetAuthorAudioState();
    if (IsAuthorMultichannelAudioEnabled()) {
        InitializeAuthorMultichannelAudio();
        (void)GetDebugForceUIEnabled(static_cast<UIElement>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(OP02_AIX_PATH))));
    }
    if (IsAuthorMultichannelAudioEnabled() && IsAuthorMultichannelAudioActive()) {
        SetSamplePlaybackSuppressed(1);
        SetAuthorStreamingAudioEnabled(0);
    }
}

// @verified - p
void OP02SetHourAndMinute(std::int8_t hour, std::int8_t minute) {
    DateTime date;
    GetCurrentDateTime(date);
    date.hour = hour;
    date.minute = minute;
    SetCurrentDateTimeTo(date);
}

// @verified - p
void OP02SetLightingAndWeather(std::int16_t mode) {
    EV_SetWeatherInstruction(0x0c, 0);
    EV_lightLoad(mode == 1 ? OP02_LIGHT_DAY : OP02_LIGHT_NIGHT);
}

// @verified - p
void OP02ResetWeather() {
    EV_SetWeatherInstruction(0x0c, 1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
}

// @verified - p
void OP02PrepareSceneCharacters() {
    for (shen_name character : OP02_SCENE_CHARACTERS) {
        EV_EnableCharacterDisplay(character);
        EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(character, false);
    }
}

// @verified - p
void OP02RestoreSceneCharacters() {
    for (shen_name character : OP02_SCENE_CHARACTERS) {
        EV_EnableCharacterDisplay(character);
        EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(character, true);
    }
}

// @verified - p
void OP02WaveCallback(void *) {
    BuildWaveLightingTable(1, 1.0f, 1.0f, Vector3{1.0f, 1.0f, 1.0f}, Vector3{1.0f, 1.0f, 1.0f}, 10.0f, 1.0f);
    if (!op02WaveEnabled)
        return;
    SetMapModelSlotEnabled(OP02_WAVE_SLOTS[op02WaveFrame % 16], 0);
    op02WaveFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(op02WaveFrame) + 1U);
    SetMapModelSlotEnabled(OP02_WAVE_SLOTS[op02WaveFrame % 16], 1);
    SetTextureAnimationUV(0.0f, 0.014999999664723873f);
    ApplyTextureAnimationResource(3);
}

// @verified - p
void OP02Prepare(Op02EventWork *work) {
    DispatchPostFxFilterRender(1);
    SetSnapshotDataOption(1);
    SetPostFxFilterMode(1);
    AssignAreaNpcLoadContext(OP02_AUTHOR_CHARACTERS, work->task);
    g_usingCustomAuthorCharacterList = 0;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (work->task)
        HLib::SetupTimedTask(work->task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(OP02_LOAD_TASK.name));
    SetPlayerControlEnabled(0);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_ToggleCharacterFlag(OP02_PLAYER);
    shenSetClockAndCompassRenderTo(1);
    OP02Nullsub();
    SetIsSkySetup(0);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_FreeAuthorPak(1, "0001");
    GenerateFFT(1.25f);
    FillFftDistance(FillFftKernel(184.0f, GenerateDistanceFalloffRowMajor(2.0f, op02DistanceKernel)));
    ApplyFft(op02PairwiseKernel.data(), op02PairwiseKernel.data());
    FinalizeFft(0.01489999983459711f, 0.0f);
    SetWaveLighting(0.10000000149011612f, Vector3{0.0f, 0.871999979019165f, -0.48899999260902405f});
    op02WaveEnabled = 1;
    HLib::EnqueueTaskWithoutParameter(OP02WaveCallback, 3, 2, static_cast<std::int32_t>(OP02_WAVE_TASK.name));
    op02WaveFrame = 0;
    for (std::int32_t slot : OP02_WAVE_SLOTS)
        SetMapModelSlotEnabled(slot, 0);
    EV_SetAlignedShadowRange(200.0f);
}

// @verified - p
void OP02Release(Op02EventWork *work) {
    EV_FreeAuthorPak(0, "0001");
    SelectPostFxBank(1, 0);
    for (const shen_name *character = OP02_AUTHOR_CHARACTERS; character->name != 0xffffffffU; ++character) {
        if (DoesScenarioCharacterExist(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    for (std::int32_t &texture : op02TextureReferences) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    HLib::DestroyTaskInternal(work->task);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    shenQueueChangeModuleAndScene(SystemModule::Title, 1, AREA_OP02, work->mode == 3 ? 0 : 49);
    if (IsAuthorMultichannelAudioEnabled() && IsAuthorMultichannelAudioActive()) {
        ReleaseAuthorMultichannelAudio();
        ResetAuthorAudioState();
    }
}

// @verified - p
void OP02QueueAuthor(Op02EventWork *work) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(OP02AuthorLoopCallback, 4, 0x0b, 8, OP02_TASK);
    *static_cast<Op02EventWork **>(HLib::GetTaskParameter(task)) = work;
    ReadOp02Triggers();
    OP02PrepareSceneCharacters();
    if (IsAuthorMultichannelAudioEnabled() && IsAuthorMultichannelAudioActive())
        StartAuthorMultichannelAudio(0);
    else
        shenPlayAudioWithFlags("adv", 1);
    SelectPostFxBank(1, 1);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(0));
    work->mode = 2;
    work->state = 0;
}

// @verified - p
void OP02AuthorLoopCallback(void *data) {
    Op02EventWork *const work = *static_cast<Op02EventWork **>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining);
    bool updateGlow = true;
    switch (frame) {
    case 0:
        OP02SetLightingAndWeather(2);
        work->state = 0;
        break;
    case 165:
        OP02SetHourAndMinute(12, 40);
        OP02SetLightingAndWeather(1);
        work->state = 1;
        break;
    case 274:
        OP02SetLightingAndWeather(2);
        work->state = 2;
        break;
    case 394:
        OP02SetHourAndMinute(12, 45);
        OP02ResetWeather();
        work->state = 3;
        break;
    case 545:
        work->state = 4;
        break;
    case 812:
        OP02SetLightingAndWeather(1);
        work->state = 5;
        break;
    case 965:
        SetIsSkySetup(1);
        OP02SetHourAndMinute(9, 45);
        OP02ResetWeather();
        work->state = 6;
        break;
    case 1051:
        SetIsSkySetup(0);
        EV_SetWeatherInstruction(0x0c, 0);
        EV_lightLoad(OP02_LIGHT_DAY);
        work->state = 7;
        break;
    case 1168:
        OP02SetLightingAndWeather(2);
        work->state = 8;
        break;
    case 1420:
        OP02SetLightingAndWeather(1);
        work->state = 9;
        break;
    case 1490:
        OP02SetLightingAndWeather(2);
        work->state = 10;
        break;
    case 1665:
        OP02SetHourAndMinute(12, 0);
        OP02ResetWeather();
        work->state = 11;
        break;
    case 1905:
        work->state = 12;
        break;
    case 2014:
        OP02SetLightingAndWeather(1);
        work->state = 13;
        break;
    case 2111:
        OP02SetHourAndMinute(9, 45);
        OP02ResetWeather();
        work->state = 14;
        break;
    default:
        updateGlow = false;
        break;
    }
    if (updateGlow)
        UploadGlowParams(work->state == 0 ? op02FirstGlow : op02LaterGlow);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        OP02RestoreSceneCharacters();
        work->mode = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
    RefreshTriggerInputs();
    EnableElement(static_cast<UIElement>(79));
    bool abort = (EVT_GetControllerInput(0, INPUT_PRESSED) & 0x0edeU) != 0;
    for (std::uint8_t index = 0; index < 4 && !abort; ++index) {
        abort = op02PreviousTriggers[index] == 0 ? EVT_GetControllerInput(index, INPUT_DEVICE_STATE) != 0 :
                                                   EVT_GetControllerInput(index, INPUT_DEVICE_UNAVAILABLE) != 0;
    }
    if (abort) {
        OP02RestoreSceneCharacters();
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        work->mode = 4;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
    ReadOp02Triggers();
}

// @verified - p
void OP02MainCallback(void *data) {
    Op02EventWork *const work = static_cast<Op02EventWork *>(data);
    if (work->mode == 1)
        OP02QueueAuthor(work);
    else if (work->mode == 3 || work->mode == 4)
        OP02Release(work);
}

// @verified - p
void AreaOP02Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(OP02MainCallback, 4, 0x0b, sizeof(Op02EventWork), OP02_TASK);
    Op02EventWork *const work = static_cast<Op02EventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->mode = 1;
    work->state = 0;
    OP02Prepare(work);
}

const ScriptAreaDefinition areaOP02ScriptDefinition{.areaId = AREA_OP02, .field04 = 0, .init = AreaOP02Init, .main = AreaOP02Main};
} // namespace shenmue::s2::areas
