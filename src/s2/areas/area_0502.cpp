#include "s2/areas/area_0502.hpp"
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
void Prepare0502Character(shen_name character) {
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}
void Restore0502Character(shen_name character) {
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 6, 0);
    EV_SetCharacterAlphaEnabled(character, true);
}
} // namespace
void Initialize0502() {
    splash0502 = 0;
}
void Start0502(std::uintptr_t argument) {
    SetRenderClockAndCompassTo(1);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0502, 4, 11, sizeof(Area0502Parameter), AREA_0502_EVENT);
    Area0502Parameter *const parent = static_cast<Area0502Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    parent->reserved14 = 0;
    InitializeShipSpray();
    HIKI_Enqueue();
    if (g_currentMap.name == AREA_0502_MAP.name) {
        parent->request = reinterpret_cast<const std::int32_t *>(argument);
        parent->selection = *parent->request;
    } else {
        parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0502A.MOT"));
        parent->selection = 0;
    }
    Setup0502Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0502_CHARACTERS, parent->self);
}
void Update0502(void *data) {
    Area0502Parameter *const parent = static_cast<Area0502Parameter *>(data);
    switch (parent->state) {
    case 1:
        Queue0502Author(parent);
        break;
    case 3:
        for (const shen_name character : AREA_0502_CHARACTERS) {
            if (character.name == 0xffffffffU)
                break;
            if (DoesScenarioCharacterExist(character)) {
                EV_npcLockUnlock(character);
                EV_npcFree(character);
            }
        }
        EV_NPC_NewLoadSw(1);
        parent->state = 4;
        break;
    case 4:
        Cleanup0502(parent);
        break;
    }
}
void Setup0502Environment() {
    SetPlayerControlEnabled(0);
    if (g_currentMap.name == AREA_0502_MAP.name) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        EV_SetNewSceneIndex();
        EV_LoadCutsceneFromCurrentScene("0502A000");
    } else {
        SetNewWindPhaseAndSpeed(4096, 0.15000001f);
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
        HLib::EnqueueTaskWithoutParameter(Update0502Waves, 3, 2, static_cast<std::int32_t>(AREA_0502_EVENT.name));
    }
}
void Update0502Waves(void *) {
    BuildWaveLightingTable(0, 1.0f, 1.0f, Vector3{1.0f, 1.0f, 1.0f}, Vector3{1.0f, 1.0f, 1.0f}, 50.0f, 1.0f);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetTextureAnimationUV(0.0f, 0.002f);
        ApplyTextureAnimationResource(1);
    } else
        ApplyWaveTextureResource(1);
}
void Queue0502Author(Area0502Parameter *parent) {
    if (g_currentMap.name == AREA_0502_MAP.name) {
        QueueImmediateNextFadeTask(nullptr);
        ProcessAicaCommand(0x1200a0, 0);
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0502Author, 4, 11, sizeof(Area0502AuthorParameter), AREA_0502_EVENT);
    static_cast<Area0502AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    parent->voice = shenPlayAudioWithFlags("funatukiba2", 1);
    if (g_currentMap.name != AREA_0502_MAP.name)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    Prepare0502Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
void Prepare0502Characters() {
    Prepare0502Character(AREA_0502_RYO);
    for (const shen_name prop : AREA_0502_PROPS)
        EV_SetCharacterAlphaEnabled(prop, false);
    for (const shen_name character : AREA_0502_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        Prepare0502Character(character);
    }
}
void Restore0502Characters() {
    Restore0502Character(AREA_0502_RYO);
    for (const shen_name prop : AREA_0502_PROPS) {
        EV_SetCharacterAlphaEnabled(prop, true);
        EV_DisableCharacterDisplay(prop);
    }
    for (const shen_name character : AREA_0502_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        Restore0502Character(character);
    }
}
void Update0502Author(void *data) {
    Area0502AuthorParameter *const child = static_cast<Area0502AuthorParameter *>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame < 1041) {
        Vector3 position{};
        Vector3i rotation{};
        EV_GetCharaPosition(AREA_0502_PROPS[0], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(AREA_0502_PROPS[0], &rotation, EventTransformFlags::Local);
        rotation.y += 0x8000;
        AssignShipSprayLocalPosition(Vector3{0.0f, 0.0f, 13.5f});
        AssignHikiLocalPosition(Vector3{0.0f, 0.0f, 2.0f});
        position.y = 99.470001f;
        AssignShipSprayPosition(position);
        AssignShipSprayYaw(rotation.y);
        AssignHikiPosition(position);
        AssignHikiRotation(rotation.y);
    } else if (frame == 1041) {
        const Vector3 position{0.0f, -1000.0f, 0.0f};
        AssignHikiPosition(position);
        AssignShipSprayPosition(position);
    }
    if (frame == 550)
        g_shipSprayFadeRate = 0.0045454544f;
    else if (frame == 1731)
        Create0502Splash(0);
    else if (frame == 1814)
        Create0502Splash(1);
    else if (frame == 1956)
        Create0502Splash(2);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Restore0502Characters();
        child->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void Create0502Splash(std::int32_t index) {
    HLib::EnqueueTaskWithoutParameter(Update0502Splash, 4, 11, static_cast<std::int32_t>(AREA_0502_EVENT.name));
    const Vector3 &position = AREA_0502_SPLASH_POSITIONS[index];
    const Vector3 spawn{0.2f, 0.0f, 0.2f};
    splash0502 = static_cast<std::uint32_t>(CreateSplashEffect(position, spawn, 500, 0.02f, shen_name{}, std::bit_cast<std::int32_t>(90.0f), nullptr, nullptr));
    SetParticleEffectColour(1, splash0502, 70, 255, 255, 255);
    SetParticleEffectPosition(1, splash0502, &position, &spawn);
    SetParticleEffectVelocity(1, splash0502, Vector3{-0.0050980374f, 0.047450982f, 0.028627448f}, Vector3{0.02784314f, 0.041176476f, 0.052156858f});
    SetParticleEffectBirthRate(1, splash0502, 0.078431375f);
    SetParticleEffectSize(1, splash0502, 0.025098039f, 0.025098039f);
    ResumeParticleEffect(1, splash0502);
}
void Update0502Splash(void *) {
    if (static_cast<std::int16_t>(static_cast<std::int32_t>(splashAge0502)) == 35) {
        splashAge0502 = 0.0f;
        SuspendParticleEffect(1, splash0502);
        FreeParticleEffect(1, splash0502);
        HLib::DestroyCurrentTask();
    } else
        splashAge0502 += g_stabilizeFps ? g_stabilizedFrameTime * 30.0f : 1.0f;
}
void Cleanup0502(Area0502Parameter *parent) {
    using enum CameraState;
    EV_CleanupMapControlTask();
    if (g_currentMap.name != AREA_0502_MAP.name) {
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        return;
    }
    for (std::size_t index = 2; index < std::size(AREA_0502_PROPS); ++index)
        SetScenarioCharacterKillMode(AREA_0502_PROPS[index], 1);
    SetScenarioCharacterKillMode(AREA_0502_PROPS[0], 1);
    hsFadeVoice(parent->voice, 30);
    LoadCutsceneAndSoundProgramForArea();
    EV_LoadAudio(shenGetCurrentSbt(), 0);
    EV_spmBeginSampleExecute();
    MotionWork *const motion = FindMotionWork(FindCharacterByScenarioName(AREA_0502_RYO));
    mtwkRequestMotion(motion, 0xf003, MotionRegion::LowerBody);
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(motion, 13, 0);
    SetMotionSpeed(motion, 1.0f, MotionRegion::LowerBody);
    EV_AT_SetPos(AREA_0502_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{1427.8f, 100.6f, 809.1f});
    EV_AT_SetAngY(AREA_0502_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 16384, 0}, nullptr);
    mtwkSetExtraFlag(motion, 17, 0);
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
    camCtrlSetFovTransitionTo(true);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetState(Init);
    HLib::DestroyTask(parent->self);
    SetPlayerControlEnabled(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetRenderClockAndCompassTo(0);
    const Vector3 position{0.0f, -1000.0f, 0.0f};
    AssignShipSprayPosition(position);
    AssignHikiPosition(position);
}
const ScriptAreaDefinition area0502ScriptDefinition{AREA_0502, 0, Initialize0502, Start0502};
} // namespace shenmue::s2::areas
