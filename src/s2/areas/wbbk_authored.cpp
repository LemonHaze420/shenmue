#include "s2/areas/wbbk_authored.hpp"
#include "s2/areas/wbbk_motion.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstddef>
#include <cstring>

namespace shenmue::s2::areas {
std::int32_t bookAuthoredScene;
shen_name bookAuthoredActors[10];
namespace {
struct BookAuthoredDefinition {
    shen_name scene;
    std::uint32_t reserved04;
    const char *actors;
    std::uint64_t reserved10;
    const char *sound;
    std::uint8_t soundTrack, flags;
    std::byte reserved22[6];
};
static_assert(sizeof(BookAuthoredDefinition) == 0x28);
static_assert(offsetof(BookAuthoredDefinition, sound) == 0x18 && offsetof(BookAuthoredDefinition, flags) == 0x21);
constexpr BookAuthoredDefinition bookAuthoredDefinitions[]{
    {.scene = SHEN_NAME(AU00), .reserved04 = 0, .actors = "SYE_", .reserved10 = 0, .sound = nullptr, .soundTrack = 0, .flags = 2, .reserved22 = {}},
    {.scene = SHEN_NAME(AU01), .reserved04 = 0, .actors = "KNK_", .reserved10 = 0, .sound = nullptr, .soundTrack = 0, .flags = 2, .reserved22 = {}},
    {.scene = SHEN_NAME(AU02), .reserved04 = 0, .actors = "SYE_", .reserved10 = 0, .sound = "0035A000", .soundTrack = 5, .flags = 0x22, .reserved22 = {}},
    {.scene = SHEN_NAME(AU03),
        .reserved04 = 0,
        .actors = "KUN_ a058 ab04",
        .reserved10 = 0,
        .sound = nullptr,
        .soundTrack = 0,
        .flags = 0x1a,
        .reserved22 = {}},
    {.scene = SHEN_NAME(AU04), .reserved04 = 0, .actors = "", .reserved10 = 0, .sound = nullptr, .soundTrack = 0, .flags = 0x12, .reserved22 = {}},
    {.scene = SHEN_NAME(AU10), .reserved04 = 0, .actors = "SAI_", .reserved10 = 0, .sound = "0035A000", .soundTrack = 3, .flags = 2, .reserved22 = {}},
    {.scene = SHEN_NAME(AU11), .reserved04 = 0, .actors = "SAI_", .reserved10 = 0, .sound = "0035A000", .soundTrack = 3, .flags = 0x22, .reserved22 = {}}};
} // namespace

void PrepareBookAuthoredActor(shen_name actor) {
    if (!FindCharacterByScenarioName(actor))
        return;
    EV_EnableCharacterDisplay(actor);
    EV_EnsureCharacterDraw(actor);
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    if ((actor.name & 0xff000000U) == 0x5f000000U) {
        const Vector4 reflector{.x = 1, .y = .3f, .z = .3f, .w = .3f};
        mtwkSetExtraFlag(scenFindMotionwork(actor), 6, 2);
        EV_SetCharacterAlphaEnabled(actor, false);
        SetScenarioReflectorBoardValues(actor, &reflector);
        EV_SetRefBoardParamsFor(actor, 1);
        hmdlSetFlagsFor(FindCharacterByScenarioName(actor), 0, 8, 16);
        hmdlSetFlagsFor(FindCharacterByScenarioName(actor), 1, 8, 16);
    }
}
void QueueBookAuthoredScene(std::int32_t scene) {
    bookAuthoredScene = scene;
    const BookAuthoredDefinition &definition = bookAuthoredDefinitions[scene];
    const char *source = definition.actors;
    shen_name *destination = bookAuthoredActors;
    while (*source) {
        if (*source == ' ')
            ++source;
        else {
            std::uint32_t name;
            std::memcpy(&name, source, sizeof(name));
            source += sizeof(name);
            const shen_name actor{name};
            *destination++ = actor;
            PrepareBookAuthoredActor(actor);
        }
    }
    *destination = shen_name{};
    if (!(definition.flags & 1U)) {
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(definition.scene.name));
        HLib::EnqueueTaskWithoutParameter(BookAuthoredPreCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
    } else {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(BookAuthoredPreCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
        HLib::SetupTimedTask(task, 30);
    }
    HLib::SetupTaskDelay(0);
}
void BookAuthoredPreCallback(void *) {
    const BookAuthoredDefinition &definition = bookAuthoredDefinitions[bookAuthoredScene];
    if (definition.sound) {
        EV_LoadAudio(definition.sound, definition.soundTrack);
        EV_spmBeginSampleExecute();
    }
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(definition.scene.name));
    HLib::SetCurrentTaskTo(BookAuthoredCallback);
    if (definition.flags & 2U)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
bool CompleteBookAuthoredScene() {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return false;
    for (const shen_name *actor = bookAuthoredActors; actor->name; ++actor) {
        if (actor->name != BOOK_SAI.name && FindCharacterByScenarioName(*actor)) {
            EV_DisableCharacterDisplay(*actor);
            EV_ToggleCharacterFlag(*actor);
        }
    }
    if (bookAuthoredDefinitions[bookAuthoredScene].flags & 0x10U) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_LoadAudio("0035A000", 2);
        EV_spmBeginSampleExecute();
    }
    return true;
}
void BookAuthoredCallback(void *) {
    if (CompleteBookAuthoredScene()) {
        HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
        HLib::DestroyCurrentTask();
    } else if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 60) {
        const std::uint8_t flags = bookAuthoredDefinitions[bookAuthoredScene].flags;
        if (flags & 4U)
            SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        if (flags & 0x20U)
            spmFadeOut();
    }
}
} // namespace shenmue::s2::areas
