#include "s2/areas/area_0502.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/weather.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hiki.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/screen_film.hpp"
#include "s2/ship_spray.hpp"
#include "s2/sound_program.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t splash0502;
float splashAge0502;
void Prepare0502Character(shen_name character);
void Restore0502Character(shen_name character);
} // namespace
void Initialize0502()
{
    splash0502 = 0;
}
#elif !defined(SHENMUE_EVENT_ENTRY)
shen_name AREA_0502_CHARACTERS[] = {SHEN_NAME4('A', 'A', '1', '_'), SHEN_NAME4('H', 'A', 'K', '_'),
                                    SHEN_NAME4('K', 'N', 'T', '_'), SHEN_NAME4('L', 'Y', 'O', '_'),
                                    SHEN_NAME4('S', 'Y', 'O', '_'), SHEN_NAME4('U', 'G', 'N', '_'),
                                    SHEN_NAME4('C', 'H', '1', '_'), SHEN_NAME4('C', 'H', '5', '_'),
                                    SHEN_NAME4('C', 'H', '6', '_'), 0xffffffff};
static short splashAge0502 = 0;
static unsigned int splash0502;
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_EVENT_ENTRY)
void Update0502(void *data)
{
    Area0502Parameter *parent = (Area0502Parameter *)data;
    switch (parent->state) {
    case 1:
        Queue0502Author(parent);
        break;
    case 3:
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_UTL_FreeNPCChara(AREA_0502_CHARACTERS);
#else
        for (const shen_name character : AREA_0502_CHARACTERS) {
            if (character.name == 0xffffffffU)
                break;
            if (DoesScenarioCharacterExist(character)) {
                EV_npcLockUnlock(character);
                EV_npcFree(character);
            }
        }
#endif
        FTBL.EV_NPC_NewLoadSw_0(1);
        parent->state = 4;
        break;
    case 4:
        Cleanup0502(parent);
        break;
    }
}

void Queue0502Author(Area0502Parameter *parent)
{
    HLibTask *task;
    if (g_currentMap == AREA_0502_MAP) {
        FTBL.QueueImmediateNextFadeTask(0);
        FTBL.ProcessAicaCommand(0x1200a0, 0);
    }
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0502Author, 4, 11, sizeof(Area0502AuthorParameter), AREA_0502_EVENT);
    ((Area0502AuthorParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    parent->voice = FTBL.shenPlayAudioWithFlags("funatukiba2", 1);
    if (g_currentMap != AREA_0502_MAP)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    Prepare0502Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void Update0502Author(void *data)
{
    Area0502AuthorParameter *child = (Area0502AuthorParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position;
    Vector3 local = {0.0f, 0.0f, 13.5f};
    Vector3i rotation;
    short frame = FTBL.EV_authSceneCtrl(-6);
#else
    const std::int32_t frame = static_cast<std::int32_t>(FTBL.EV_authSceneCtrl(-6));
#endif
    if (frame < 1041) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.GetCharaPosition(SHEN_NAME4('a', '1', '4', '3'), 0xffffffff, &position, 0);
        FTBL.GetScenarioCharacterEulerAngles(SHEN_NAME4('a', '1', '4', '3'), &rotation, 0);
#else
        Vector3 position{};
        Vector3i rotation{};
        EV_GetCharaPosition(AREA_0502_PROPS[0], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(AREA_0502_PROPS[0], &rotation, EventTransformFlags::Local);
#endif
        rotation.y += 0x8000;
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EventFunction862(&local);
        local.z = 2.0f;
        FTBL.EventFunction866(&local);
#else
        AssignShipSprayLocalPosition(Vector3{0.0f, 0.0f, 13.5f});
        AssignHikiLocalPosition(Vector3{0.0f, 0.0f, 2.0f});
#endif
        position.y = 99.470001f;
        FTBL.AssignShipSprayPosition(&position);
        FTBL.AssignShipSprayYaw(rotation.y);
        FTBL.AssignHikiPosition(&position);
        FTBL.AssignHikiRotation(rotation.y);
    } else if (frame == 1041) {
#if defined(SHENMUE_EVENT_OVERLAY)
        position.x = 0.0f;
        position.y = -1000.0f;
        position.z = 0.0f;
#else
        const Vector3 position{0.0f, -1000.0f, 0.0f};
#endif
        FTBL.AssignHikiPosition(&position);
        FTBL.AssignShipSprayPosition(&position);
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (frame) {
    case 550:
        FTBL.PFX_SetDeltaTimeTo(0.0045454544f);
        break;
    case 1731:
        Create0502Splash(0);
        break;
    case 1814:
        Create0502Splash(1);
        break;
    case 1956:
        Create0502Splash(2);
        break;
    }
#else
    if (frame == 550)
        g_shipSprayFadeRate = 0.0045454544f;
    else if (frame == 1731)
        Create0502Splash(0);
    else if (frame == 1814)
        Create0502Splash(1);
    else if (frame == 1956)
        Create0502Splash(2);
#endif
    if (!FTBL.EV_authSceneCtrl(-1)) {
        Restore0502Characters();
        child->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void Create0502Splash(Area0502SplashIndex index)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 spawn = {0.2f, 0.0f, 0.2f};
    Vector3 velocity = {-0.0050980374f, 0.047450982f, 0.028627448f};
    Vector3 variance = {0.02784314f, 0.041176476f, 0.052156858f};
    Vector3 positions[] = {{1434.160034f, 99.3199997f, 792.450012f}, {1435.219971f, 100.4000015f, 791.609985f}, {1434.0f, 99.0999985f, 792.429993f}};
    if (index < 4) {
#endif
        FTBL.HLib_EnqueueTaskWithoutParameter(Update0502Splash, 4, 11, AREA_0502_EVENT_TOKEN);
#if !defined(SHENMUE_EVENT_OVERLAY)
        const Vector3 &position = AREA_0502_SPLASH_POSITIONS[index];
        const Vector3 spawn{0.2f, 0.0f, 0.2f};
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        splash0502 = FTBL.CreateSplashEffect(&positions[index], &spawn, 500, 0.02f, 0, 90.0f, 0, 0);
        FTBL.SetParticleEffectColour(1, splash0502, 70, 255, 255, 255);
        FTBL.SetParticleEffectPosition(1, splash0502, &positions[index], &spawn);
        FTBL.SetParticleEffectVelocity(1, splash0502, &velocity, &variance);
#else
    splash0502 = static_cast<std::uint32_t>(
        CreateSplashEffect(position, spawn, 500, 0.02f, shen_name{}, std::bit_cast<std::int32_t>(90.0f), nullptr, nullptr));
    FTBL.SetParticleEffectColour(1, splash0502, 70, 255, 255, 255);
    FTBL.SetParticleEffectPosition(1, splash0502, &position, &spawn);
    SetParticleEffectVelocity(1, splash0502, Vector3{-0.0050980374f, 0.047450982f, 0.028627448f}, Vector3{0.02784314f, 0.041176476f, 0.052156858f});
#endif
        FTBL.SetParticleEffectBirthRate(1, splash0502, 0.078431375f);
        FTBL.SetParticleEffectSize(1, splash0502, 0.025098039f, 0.025098039f);
        FTBL.ResumeParticleEffect(1, splash0502);
#if defined(SHENMUE_EVENT_OVERLAY)
    }
#endif
}

void Update0502Splash(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    if (splashAge0502 == 35U) {
#else
    if (static_cast<std::int16_t>(static_cast<std::int32_t>(splashAge0502)) == 35) {
#endif
        splashAge0502 = 0;
        FTBL.SuspendParticleEffect(1, splash0502);
        FTBL.FreeParticleEffect(1, splash0502);
        FTBL.HLib_DestroyCurrentTask();
    } else {
#if defined(SHENMUE_EVENT_OVERLAY)
        splashAge0502 = splashAge0502 + 1;
#else
        splashAge0502 += g_stabilizeFps ? g_stabilizedFrameTime * 30.0f : 1.0f;
#endif
    }
}

void Setup0502Environment()
{
    FTBL.SetPlayerControlEnabled(0);
    if (g_currentMap == AREA_0502_MAP) {
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.EV_SetNewSceneIndex();
        FTBL.EV_LoadCutsceneFromCurrentScene("0502A000");
    } else {
#if defined(SHENMUE_EVENT_OVERLAY)
        float distance[64], first[64], second[64];
#endif
        FTBL.SetNewWindPhaseAndSpeed(4096, 0.15000001f);
#if defined(SHENMUE_EVENT_OVERLAY)
        {
            Vector3 lighting = {0.3f, 0.8f, 0.5f};
            void *firstKernel, *secondKernel;
            FTBL.GenerateFFT(0.5f);
            FTBL.fft_fill_dist(FTBL.fft_fill_krn(3.92f, FTBL.generate_distance_falloff_row_major(1.29f, distance)));
            firstKernel = FTBL.fft_fill_krn(0.06f, FTBL.generate_distance_falloff_kernel_8x8(1.035f, first));
            secondKernel = FTBL.fft_fill_krn(0.073f, FTBL.generate_distance_falloff_kernel_8x8(0.564f, second));
            FTBL.fft_bins_load_pairwise_tables(firstKernel, secondKernel);
            FTBL.FFTUtil(0.0f, -0.002f);
            FTBL.set_lighting_flags(0.1f, &lighting);
#else
        std::array<std::byte, 256> distance, first, second;
        GenerateFFT(0.5f);
        GenerateDistanceFalloffRowMajor(1.29f, distance);
        FillFftKernel(3.92f, distance.data());
        FillFftDistance(distance.data());
        GenerateDistanceFalloffKernel8x8(0.564f, first);
        FillFftKernel(0.073f, first.data());
        GenerateDistanceFalloffKernel8x8(1.035f, second);
        FillFftKernel(0.06f, second.data());
        ApplyFft(second.data(), first.data());
        FinalizeFft(0.0f, -0.002f);
        SetWaveLighting(0.1f, Vector3{0.3f, 0.8f, 0.5f});
#endif
            FTBL.HLib_EnqueueTaskWithoutParameter(Update0502Waves, 3, 2, AREA_0502_EVENT_TOKEN);
#if defined(SHENMUE_EVENT_OVERLAY)
        }
#endif
    }
}

void Cleanup0502(Area0502Parameter *parent)
{
#if !defined(SHENMUE_EVENT_OVERLAY)
    using enum CameraState;
#else
    Vector3 position = {1427.8f, 100.6f, 809.1f};
    Vector3i rotation = {0, 16384, 0};
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0502_MAP) {
#else
    EV_CleanupMapControlTask();
    if (g_currentMap != AREA_0502_MAP) {
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        return;
    }
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetScenarioCharacterKillMode(SHEN_NAME4('a', '4', '4', '3'), 1);
        FTBL.SetScenarioCharacterKillMode(SHEN_NAME4('a', '4', '4', '4'), 1);
        FTBL.SetScenarioCharacterKillMode(SHEN_NAME4('a', '4', '4', '5'), 1);
        FTBL.SetScenarioCharacterKillMode(SHEN_NAME4('a', '2', '1', '9'), 1);
        FTBL.SetScenarioCharacterKillMode(SHEN_NAME4('a', '1', '4', '3'), 1);
#else
    for (std::size_t index = 2; index < std::size(AREA_0502_PROPS); ++index)
        FTBL.SetScenarioCharacterKillMode(AREA_0502_PROPS[index], 1);
    FTBL.SetScenarioCharacterKillMode(AREA_0502_PROPS[0], 1);
#endif
        FTBL.hsFadeVoice(parent->voice, 30);
        FTBL.LoadCutsceneAndSoundProgramForArea();
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_LoadAudio(FTBL.shenGetCurrentSbt(), 0);
#else
        EV_LoadAudio(shenGetCurrentSbt(), 0);
#endif
        FTBL.EV_spmBeginSampleExecute(0);
#if !defined(SHENMUE_EVENT_OVERLAY)
        MotionWork *const motion = FindMotionWork(ctrlGetScenarioChara(AREA_0502_RYO));
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(AREA_0502_RYO), 0xf003, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(AREA_0502_RYO), 71, 0, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(AREA_0502_RYO), 13, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(AREA_0502_RYO), 1.0f, 0);
        FTBL.EV_AT_SetPos(AREA_0502_RYO, 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(AREA_0502_RYO, 0x78000000, &rotation, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(AREA_0502_RYO), 17, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(AREA_0502_RYO), 71, 0, 0);
#else
    FTBL.mtwkRequestMotion(motion, 0xf003, 0);
    FTBL.SetMotionRegionFlag(motion, 71, 0, 0);
    FTBL.mtwkSetExtraFlag(motion, 13, 0);
    FTBL.mtwkSetSpeed(motion, 1.0f, 0);
    EV_AT_SetPos(AREA_0502_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{1427.8f, 100.6f, 809.1f});
    EV_AT_SetAngY(AREA_0502_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 16384, 0}, nullptr);
    FTBL.mtwkSetExtraFlag(motion, 17, 0);
    FTBL.SetMotionRegionFlag(motion, 71, 0, 0);
#endif
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetState(0);
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.SetRenderClockAndCompassTo(0);
#if defined(SHENMUE_EVENT_OVERLAY)
        {
            Vector3 hidden = {0.0f, -1000.0f, 0.0f};
            FTBL.AssignShipSprayPosition(&hidden);
            FTBL.AssignHikiPosition(&hidden);
        }
    } else {
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
#else
    const Vector3 position{0.0f, -1000.0f, 0.0f};
    FTBL.AssignShipSprayPosition(&position);
    FTBL.AssignHikiPosition(&position);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void Cleanup0502Task(void *data)
{
    Area0502Parameter *parent = (Area0502Parameter *)data;
    if (!FTBL.IsItemSelectable()) {
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.SetPlayerControlEnabled(1);
    }
}
#endif
void Prepare0502Characters()
{
    Prepare0502Character(AREA_0502_RYO);
#if defined(SHENMUE_EVENT_OVERLAY)
    Hide0502Prop(SHEN_NAME4('a', '1', '4', '3'));
    Hide0502Prop(SHEN_NAME4('a', '2', '0', '5'));
    Hide0502Prop(SHEN_NAME4('a', '4', '4', '3'));
    Hide0502Prop(SHEN_NAME4('a', '4', '4', '4'));
    Hide0502Prop(SHEN_NAME4('a', '4', '4', '5'));
    Hide0502Prop(SHEN_NAME4('a', '2', '1', '9'));
    {
        int i;
        for (i = 0; AREA_0502_CHARACTERS[i] != 0xffffffff; i++)
            Prepare0502Character(AREA_0502_CHARACTERS[i]);
    }
#else
    for (const shen_name prop : AREA_0502_PROPS)
        FTBL.EV_SetCharacterAlphaEnabled(prop, false);
    for (const shen_name character : AREA_0502_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        Prepare0502Character(character);
    }
#endif
}
void Restore0502Characters()
{
    Restore0502Character(AREA_0502_RYO);
#if defined(SHENMUE_EVENT_OVERLAY)
    Restore0502Prop(SHEN_NAME4('a', '1', '4', '3'));
    Restore0502Prop(SHEN_NAME4('a', '2', '0', '5'));
    Restore0502Prop(SHEN_NAME4('a', '4', '4', '3'));
    Restore0502Prop(SHEN_NAME4('a', '4', '4', '4'));
    Restore0502Prop(SHEN_NAME4('a', '4', '4', '5'));
    Restore0502Prop(SHEN_NAME4('a', '2', '1', '9'));
    {
        int i;
        for (i = 0; AREA_0502_CHARACTERS[i] != 0xffffffff; i++)
            Restore0502Character(AREA_0502_CHARACTERS[i]);
    }
#else
    for (const shen_name prop : AREA_0502_PROPS) {
        FTBL.EV_SetCharacterAlphaEnabled(prop, true);
        FTBL.EV_DisableCharacterDisplay(prop);
    }
    for (const shen_name character : AREA_0502_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        Restore0502Character(character);
    }
#endif
}
#if !defined(SHENMUE_EVENT_OVERLAY)
namespace {
#endif
void Prepare0502Character(shen_name character)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
#else
    FTBL.mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(character)), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}
void Restore0502Character(shen_name character)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
#else
    FTBL.mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(character)), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
void Hide0502Prop(shen_name character)
{
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}
void Restore0502Prop(shen_name character)
{
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
    FTBL.EV_DisableCharacterDisplay(character);
}
#endif
void Update0502Waves(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    float white = 1.0f;
    FTBL.BuildLightingTableFrom(0, white, white, white, white, white, white, white, white, 50.0f, white);
    if (!FTBL.EV_authSceneCtrl(-1))
        FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(1));
    else {
        FTBL.DebugLogToFileRemoved("EV_nlTexanimUV\n");
        FTBL.nlSetTexanimUV(0, 0.002f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(1));
    }
#else
    BuildWaveLightingTable(0, 1.0f, 1.0f, Vector3{1.0f, 1.0f, 1.0f}, Vector3{1.0f, 1.0f, 1.0f}, 50.0f, 1.0f);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetTextureAnimationUV(0.0f, 0.002f);
        ApplyTextureAnimationResource(1);
    } else
        ApplyWaveTextureResource(1);
#endif
}
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || defined(SHENMUE_EVENT_ENTRY)
void Start0502(Area0502StartParameters argument)
{
    HLibTask *task;
    Area0502Parameter *parent;
    FTBL.SetRenderClockAndCompassTo(1);
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0502, 4, 11, sizeof(Area0502Parameter), AREA_0502_EVENT);
    parent = (Area0502Parameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 1;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->reserved0e = 0;
    parent->reserved10 = 0;
#else
    parent->reserved14 = 0;
#endif
    FTBL.InitializeShipSpray();
    FTBL.HIKI_Enqueue();
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap != AREA_0502_MAP) {
        parent->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0502A.MOT");
        parent->selection = 0;
    } else {
        parent->selection = *argument;
        parent->request = argument;
    }
#else
    if (g_currentMap == AREA_0502_MAP) {
        parent->request = reinterpret_cast<const std::int32_t *>(argument);
        parent->selection = *parent->request;
    } else {
        parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0502A.MOT"));
        parent->selection = 0;
    }
#endif
    Setup0502Environment();
#if defined(SHENMUE_EVENT_OVERLAY)
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0502_CHARACTERS, task);
#else
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0502_CHARACTERS, parent->self);
#endif
}
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition area0502ScriptDefinition{AREA_0502, 0, Initialize0502, Start0502};
} // namespace shenmue::s2::areas
#endif
