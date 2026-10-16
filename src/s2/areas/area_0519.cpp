#include "s2/areas/area_0519.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/areas/numeric_event.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scene_data.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t splash0519;
std::uint16_t splashAge0519;
} // namespace

// @verified - p
void Initialize0519() {
}

// @verified - p
void Start0519(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0519, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0519_MOTION));
    parent->state = 1;
    parent->selection = g_entryPoint;
    Setup0519Package(std::bit_cast<std::int32_t>(parent->selection));
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0519_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0519_CONVERSATION);
    if (std::bit_cast<std::int32_t>(parent->selection) > 3)
        Cleanup0519(parent);
}

// @verified - p
void Update0519(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1)
        Queue0519Author(parent);
    else if (parent->state == 3)
        Cleanup0519(parent);
}

// @verified - p
void Cleanup0519(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0519_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0519_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0519Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0519AuthorTask, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0519Author(AuthorViewer517ELoopTaskParameter *child) {
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0)
        SetupAreaRyoSinAuthorCharacters();
    if ((child->parent->selection == 1 || child->parent->selection == 3) && child->currentFrame == 40)
        Create0519Splash(std::bit_cast<std::int32_t>(child->parent->selection));
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        EnableCharDisplayAndSetMotionworkData(AREA_0519_RYO_);
        EnableCharDisplayAndSetMotionworkData(AREA_0519_SIN_);
        child->parent->state = 3;
        if (child->parent->selection == 0)
            HLib::SetupTimedTask(child->parent->self, 30);
        SetScreenFilmFade(child->parent->selection == 0 ? 30 : 0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0519AuthorTask(void *taskData) {
    Update0519Author(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

// @verified - p
void Setup0519Package(std::int32_t selection) {
    const char *package = selection < 2 ? "KRK2" : "KRK4";
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(package), 0);
    std::array<std::byte, 0x100> distance, first, second;
    GenerateFFT(0.5f);
    FillFftDistance(FillFftKernel(3.9200000762939453f, GenerateDistanceFalloffRowMajor(1.2899999618530273f, distance)));
    void *const firstKernel = FillFftKernel(0.0729999989271164f, GenerateDistanceFalloffKernel8x8(0.5640000104904175f, first));
    void *const secondKernel = FillFftKernel(0.05999999865889549f, GenerateDistanceFalloffKernel8x8(1.034999966621399f, second));
    ApplyFft(secondKernel, firstKernel);
    FinalizeFft(0.0f, 0.004699999932199717f);
    SetWaveLighting(0.10000000149011612f, {0.30000001192092896f, 0.800000011920929f, 0.5f});
    HLib::EnqueueTaskWithoutParameter(Update0519Waves, 3, 2, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
}

// @verified - p
void Update0519Waves(void *) {
    BuildWaveLightingTable(0, 1.0f, 1.0f, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, 50.0f, 1.0f);
    if (g_entryPoint < 2) {
        SetTextureAnimationUV(0.0f, 0.004699999932199717f);
        ApplyTextureAnimationResource(1);
        SetTextureAnimationUV(0.0f, 0.004699999932199717f);
        ApplyTextureAnimationResource(0);
        SetTextureAnimationUV(0.0f, -0.03999999910593033f);
        ApplyTextureAnimationResource(3);
    } else {
        SetTextureAnimationUV(0.0f, 0.004999999888241291f);
        ApplyTextureAnimationResource(2);
        SetTextureAnimationUV(0.0f, 0.004999999888241291f);
        ApplyTextureAnimationResource(3);
        SetTextureAnimationUV(0.0f, 0.019999999552965164f);
        ApplyTextureAnimationResource(4);
    }
}

// @verified - p
void Create0519Splash(std::int32_t selection) {
    const Vector3 spawn{0.2f, 0.0f, 0.2f};
    const Vector3 velocity{std::bit_cast<float>(0x3bf42750U), std::bit_cast<float>(0x3d75c28eU), 0.016078434884548187f};
    const Vector3 variance{std::bit_cast<float>(0x3c7a9428U), std::bit_cast<float>(0x3d28a8aaU), 0.03960784524679184f};
    const Vector3 position = selection == 1 ? Vector3{33.34000015258789f, 100.0999984741211f, 39.83000183105469f} :
                                              Vector3{47.72999954223633f, 100.5999984741211f, 61.529998779296875f};
    HLib::EnqueueTaskWithoutParameter(Update0519SplashTask, 4, 0x0b, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
    splash0519 = static_cast<std::uint32_t>(
        CreateSplashEffect(position, spawn, 1000, 0.019999999552965164f, shen_name{}, std::bit_cast<std::int32_t>(99.80000305175781f), nullptr, nullptr));
    SetParticleEffectColour(1, splash0519, 134, 175, 175, 255);
    SetParticleEffectPosition(1, splash0519, &position, &spawn);
    SetParticleEffectVelocity(1, splash0519, velocity, variance);
    SetParticleEffectBirthRate(1, splash0519, 0.1f);
    SetParticleEffectSize(1, splash0519, 0.012549019418656826f, 0.012549019418656826f);
    ResumeParticleEffect(1, splash0519);
}

// @verified - p
std::int64_t Update0519Splash() {
    if (splashAge0519 == 20) {
        SetParticleEffectBirthRate(1, splash0519, 0.0f);
        return ++splashAge0519;
    }
    if (splashAge0519 == 45) {
        splashAge0519 = 0;
        SuspendParticleEffect(1, splash0519);
        FreeParticleEffect(1, splash0519);
        return HLib::DestroyCurrentTask();
    }
    return ++splashAge0519;
}
void Update0519SplashTask(void *) {
    Update0519Splash();
}

const ScriptAreaDefinition area0519ScriptDefinition{.areaId = AREA_0519, .field04 = 0, .init = Initialize0519, .main = Start0519};

} // namespace shenmue::s2::areas
