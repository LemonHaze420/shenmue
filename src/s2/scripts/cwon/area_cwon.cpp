#include "s2/date_time.hpp"
#include "s2/scripts/cwon/area_cwon.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
#include <cstring>
namespace shenmue::s2 {
PakAfsPartition *cwonPack;
HLib::Task *cwonDynamixTask;
HLib::Task *cwonNpcLoadTask;
std::int32_t cwonWaveDisabled, cwonSpringDisabled, cwonPipeStep, cwonSmoke, cwonPackIndex, cwonPackState;
Vector3 cwonSmokePosition, cwonSmokeVolume, cwonSmokeVelocity, cwonSmokeVariance;
const shen_name CWON_CHARACTERS[]{SHEN_NAME(WON_), CWON_INVALID};
const shen_name CWON_ADDITIONAL_CHARACTERS[]{SHEN_NAME(SAM_), SHEN_NAME(LLY_), CWON_INVALID};
const shen_name CWON_AUTHOR_CHARACTERS[]{CWON_RYO, CWON_WON, shen_name{}};
const shen_name CWON_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01), SHEN_NAME(AT02), SHEN_NAME(AT03), SHEN_NAME(AT04), SHEN_NAME(AT05), SHEN_NAME(AT06),
    SHEN_NAME(AT07), SHEN_NAME(AT08), SHEN_NAME(AT09), SHEN_NAME(AT10), SHEN_NAME(AT11), SHEN_NAME(AT12), SHEN_NAME(AT13), SHEN_NAME(AT14), SHEN_NAME(AT15),
    SHEN_NAME(AT16), SHEN_NAME(AT17), SHEN_NAME(AT18), SHEN_NAME(AT19), SHEN_NAME(AT20), SHEN_NAME(AT21), SHEN_NAME(AT22), SHEN_NAME(AT23), SHEN_NAME(AT24),
    SHEN_NAME(AT25), SHEN_NAME(AT26), SHEN_NAME(AT27), SHEN_NAME(AT28), SHEN_NAME(AT29)};
namespace {
std::array<std::byte, 0x100> distanceKernel, firstKernel, secondKernel;
constexpr Vector3 playerPositions[]{{232.0f, 100.21299743652344f, 374.6000061035156f}, {216.0f, 99.31410217285156f, 409.0f},
    {256.55999755859375f, 100.9000015258789f, 381.8999938964844f}, {256.55999755859375f, 100.9000015258789f, 381.8999938964844f},
    {263.29998779296875f, 100.9000015258789f, 419.0f}, {253.85000610351562f, 100.0f, 426.25f}, {239.3000030517578f, 98.45999908447266f, 466.3500061035156f},
    {260.0f, 101.0f, 397.5f}, {207.60000610351562f, 99.0999984741211f, 390.6000061035156f}};
constexpr float playerYaws[]{270, 310, 185, 185, 180, 160, 0, 45, 0};
void ApplyLighting() {
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    DateTime date;
    GetCurrentDateTime(date);
    if (static_cast<std::uint8_t>(date.hour - 5) > 13U) {
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(12, 1);
        EV_SetObjectFlagByIndex(13, 0);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(12, 0);
        EV_SetObjectFlagByIndex(13, 1);
    }
}
} // namespace

void AreaCWONInit() {
    cwonSmokePosition = {std::bit_cast<float>(0x436a199aU), std::bit_cast<float>(0x42c4a666U), std::bit_cast<float>(1139294208U)};
    cwonPipeStep = 0;
    cwonPack = nullptr;
    cwonPackState = 0;
    cwonPackIndex = 0;
    cwonDynamixTask = nullptr;
    cwonNpcLoadTask = nullptr;
    cwonWaveDisabled = 0;
    cwonSpringDisabled = 0;
    cwonSmoke = 0;
    cwonSmokeVolume = {std::bit_cast<float>(0x3ba3d70aU), 0, std::bit_cast<float>(0x3ba3d70aU)};
    cwonSmokeVelocity = {};
    cwonSmokeVariance = {std::bit_cast<float>(0x3c9a33ceU), 0, std::bit_cast<float>(0x3c9a33ceU)};
    distanceKernel.fill(std::byte{});
    firstKernel.fill(std::byte{});
    secondKernel.fill(std::byte{});
}

void AreaCWONMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    nullsub();
    SetRenderClockAndCompassTo(1);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(CwonMainCallback, 4, 11, sizeof(CwonEventWork), CWON_TASK);
    if (!task)
        return;
    auto *work = static_cast<CwonEventWork *>(HLib::GetTaskParameter(task));
    if (!work)
        return;
    std::memset(work, 0, sizeof(*work));
    std::int32_t entry = static_cast<std::int32_t>(argument);
    if (g_currentMap.name == SHEN_NAME(AR02)) {
        EV_SetNewSceneIndex();
        SetAreaNpcLowQualityHands(false);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 1);
    } else {
        entry = g_entryPoint;
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("MPK00"), 0);
    }
    if (entry >= 9) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 422, "Entry Error\n");
        return;
    }
    CwonInitialize(work, task, entry);
    CwonInitializePlayer(work);
    cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(CWON_CHARACTERS, task);
}
const ScriptAreaDefinition areaCWONScriptDefinition{CWON_AREA, 0, AreaCWONInit, AreaCWONMain};
void CwonMainCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    if (!work->startCallback) {
        HLib::DestroyCurrentTask();
        return;
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 3);
    SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(2), nullptr);
    CwonPrepareCharacters(CWON_CHARACTERS);
    HLib::SetCurrentTaskTo(work->startCallback);
    SetAreaNpcCharactersVisible(false);
    SetAreaNpcLowQualityHands(false);
    if (work->entry > 6)
        cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(CWON_ADDITIONAL_CHARACTERS, HLib::GetCurrentTask());
    work->dummyTask = HLib::EnqueueTaskWithoutParameter(CwonDummyControlCallback, 4, 11, SHEN_NAME(dctl));
}
void CwonInitialize(CwonEventWork *work, HLib::Task *task, std::int32_t entry) {
    constexpr HLib::TaskCallback starts[]{nullptr, CwonRootStreetIntroCallback, CwonRootMarketIntroCallback, CwonRootMarketIntroCallback,
        CwonRootWarehouseIntroCallback, CwonRootWarehouseIntroCallback, CwonInitializeBattleCallback, CwonInitializeBattleCallback,
        CwonInitializeBattleCallback};
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    nullsub();
    for (shen_name name : {CWON_CENTER, CWON_DUMMY, CWON_SECOND_DUMMY}) {
        CreateEventCharacter(CharacterCreationHighModel, nullptr, name);
        EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, {});
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, {});
    }
    EV_stepSetFlags(0, 5);
    ApplyLighting();
    if (g_currentMap.name == CWON_AREA.name) {
        InitializeWaterTextureAnimation(6);
        InitializeSpringTextureAnimation(24);
        GenerateFFT(2.0f);
        FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, distanceKernel)));
        void *first = FillFftKernel(0.0021875000093132257f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, firstKernel));
        void *second = FillFftKernel(0.0035250000655651093f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, secondKernel));
        ApplyFft(second, first);
        FinalizeFft(0.0010000000474974513f, 0.0010000000474974513f);
        SetWaveLighting(0.1f, {0.0598f, 0.9768f, 0.2054f});
        HLib::EnqueueTaskWithoutParameter(CwonWaveCallback, 3, 2, SHEN_NAME(wave));
        HLib::EnqueueTaskWithoutParameter(CwonSpringCallback, 3, 2, SHEN_NAME(fntn));
        EV_AssignLightParams(1, 15);
        EV_AssignLightParams(1, 16);
    }
    work->entry = static_cast<std::uint8_t>(entry);
    work->task = task;
    work->temporarySlot = 0;
    work->startCallback = starts[entry];
    work->smoke = -1;
    cwonWaveDisabled = 0;
    cwonSpringDisabled = 0;
    for (std::int32_t i = 0; i < 4; ++i)
        if (work->motions[i]) {
            EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 633, "Motion Delete %d\n", i);
            DeleteMotion(work->motions[i]);
            work->motions[i] = 0;
        }
    work->authorState = static_cast<std::uint8_t>(work->entry - 2) <= 6U ? -1 : 0;
    EV_FreeAuthorPak(1, "0011");
    EV_FreeAuthorPak(1, "0012");
    if (g_currentMap.name != SHEN_NAME(AR02)) {
        cwonPack = reinterpret_cast<PakAfsPartition *>(EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Mount,
            reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>("CWONPACK.afs")));
        cwonPackState = -1;
        cwonPackIndex = -1;
    }
    EV_spmBeginSampleExecute();
}
void CwonInitializePlayer(CwonEventWork *work) {
    CwonStartPlayerAt(work->entry);
    EV_mtwkSetEnabled(scenFindMotionwork(CWON_RYO), 1);
    EV_SetCharacterAlphaEnabled(CWON_RYO, false);
    SetScenarioCharacterRadius(CWON_RYO, 20.0f);
    Character *player = FindCharacterByScenarioName(CWON_RYO);
    SetFaceFlagsForChara_0(player, FacialNodeReplacement);
    SetHighModelAndFaceFlagsForChara(player, FacialExpressionId::Angry, 1, 1);
    SetLowQualityHands(player);
    SetHighModelXYZ(player, 1, 0, 0);
    SetHighModelXYZ(player, 1, 1, 0);
}
void CwonStartPlayerAt(std::int32_t entry) {
    EventSetPosition position{0, CWON_RYO, CWON_CENTER, playerPositions[entry], 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, CWON_RYO, CWON_CENTER, playerYaws[entry], 0};
    EVT_AT_SetAngY_Callback(&angle);
}
void CwonPrepareCharacters(const shen_name *names) {
    for (; names->name != CWON_INVALID.name; ++names)
        if (DoesScenarioCharacterExist(*names)) {
            EV_EnableCharacterDisplay(*names);
            EV_EnsureCharacterDraw(*names);
            Character *character = FindCharacterByScenarioName(*names);
            SetFaceFlagsForChara_0(character, FacialNodeReplacement);
            SetHighModelAndFaceFlagsForChara(character, FacialExpressionId::Normal, 1, 1);
            mtwkSetRequestedRootMode(scenFindMotionwork(*names), 3);
            if (names->name == CWON_WON.name) {
                mtwkSetRequestedRootMode(scenFindMotionwork(*names), 0);
                SetScenarioCharacterType(*names, CharacterTypeStencilShadow);
            }
        }
}
void CwonWaveCallback(void *) {
    if (cwonWaveDisabled)
        return;
    GetInterpolatedWeatherValues();
    const auto colours = GetWeatherWaveColours();
    const auto specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1, 1,
        {float((diffuse >> 16) & 255U) / 255 * .75f, float((diffuse >> 8) & 255U) / 255 * .75f, float(diffuse & 255U) / 255 * .8999999761581421f},
        {float((specular >> 16) & 255U) / 255 * .31f, float((specular >> 8) & 255U) / 255 * .31f, float(specular & 255U) / 255 * .3f}, 42.540000915527344f, 1);
    SetMapModelSlotEnabled(15, 0);
    SetMapModelSlotEnabled(16, 0);
    AdvanceWaterTextureAnimation();
}
void CwonSpringCallback(void *) {
    if (!cwonSpringDisabled) {
        SetMapModelSlotEnabled(17, 0);
        SetMapModelSlotEnabled(18, 0);
        AdvanceSpringTextureAnimation();
    }
}
} // namespace shenmue::s2
