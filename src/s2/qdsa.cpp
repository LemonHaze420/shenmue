#include "s2/qdsa.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)}, tgy{SHEN_NAME(TGY_)}, invalid{0xffffffffU};
const shen_name entranceActors[]{ren, invalid};
const shen_name passageActors[]{ren, tgy, SHEN_NAME(YUA_), SHEN_NAME(SGT_), SHEN_NAME(MRC_), SHEN_NAME(SCX_), invalid};
const shen_name entranceProps[]{SHEN_NAME(a321), SHEN_NAME(a454), SHEN_NAME(a806), invalid};
const shen_name passageProps[]{SHEN_NAME(a834), SHEN_NAME(a397), SHEN_NAME(a398), invalid};
std::int64_t authorState, authorSelector;
const shen_name *authorActors;
const shen_name *authorProps;
void PrepareQdsaCharacter(shen_name character, bool actor) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    if (actor)
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}
void RestoreQdsaCharacter(shen_name character, bool actor) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    if (actor)
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
    EV_SetCharacterAlphaEnabled(character, true);
}
void PrepareQdsaCharacters() {
    PrepareQdsaCharacter(ryo, true);
    for (const shen_name *character = authorActors; character->name != invalid.name; ++character)
        PrepareQdsaCharacter(*character, true);
    for (const shen_name *character = authorProps; character->name != invalid.name; ++character)
        PrepareQdsaCharacter(*character, false);
}
void RestoreQdsaCharacters() {
    RestoreQdsaCharacter(ryo, true);
    for (const shen_name *character = authorActors; character->name != invalid.name; ++character)
        RestoreQdsaCharacter(*character, true);
    for (const shen_name *character = authorProps; character->name != invalid.name; ++character)
        RestoreQdsaCharacter(*character, false);
}
void QdsaLight(shen_name light) {
    EV_lightLoad(light);
    PushGlobalLightingProgram();
    SetDefaultVSByIndex(1);
    EV_AssignLightParams(1, 1);
}
} // namespace
void BeginQdsaAuthor(HLib::Task *parent, std::int32_t selector) {
    authorSelector = selector;
    authorState = 0;
    authorActors = nullptr;
    authorProps = nullptr;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QDSA"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDSA"), 0);
    if (selector == 0) {
        authorActors = entranceActors;
        authorProps = entranceProps;
    } else if (selector == 1) {
        authorActors = passageActors;
        authorProps = passageProps;
    }
    Queue_EVT_UTL_LoadNPCChara_3(authorActors, parent);
}
std::int32_t UpdateQdsaAuthor() {
    if (authorSelector == 0) {
        if (authorState == 0) {
            EV_SetNewSceneIndex();
            EV_LoadCutsceneFromCurrentScene("0247A000");
            QdsaLight(SHEN_NAME(l001));
            PrepareQdsaCharacters();
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(INE0)));
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EV_spmBeginSampleExecute();
            authorState = 1;
            return 0;
        }
        if (authorState == 1) {
            if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                authorState = 4;
            return 0;
        }
        if (authorState == 4) {
            ExecuteCleanupFuncForAreaWithCachedScene();
            RestoreQdsaCharacters();
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDSA"), 0);
            return 1;
        }
        return 0;
    }
    if (authorSelector != 1)
        return 0;
    switch (authorState) {
    case 0:
        EV_LoadCutsceneFromCurrentScene("0247A000");
        QdsaLight(SHEN_NAME(l201));
        SetPluralControlFor(ctrlGetCharaByName(ren), 0);
        PrepareQdsaCharacters();
        LoadBlurModelForCharacter(ryo, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(ren, "REN_B.MT7", 1);
        LoadBlurModelForCharacter(tgy, "TGY_B.MT7", 1);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(KOH0)));
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        authorState = 1;
        return 0;
    case 1:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            spmFadeOut();
            authorState = 2;
        } else if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) > 1060) {
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            spmFadeOut();
            authorState = 2;
        }
        return 0;
    case 2:
        if (!filmCheckFadeTrackColour()) {
            EV_LoadCutsceneFromCurrentScene("0248A000");
            EV_FreeAuthorPak(1, "0248");
            QdsaLight(SHEN_NAME(SGT0));
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(SGT0)));
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EV_spmBeginSampleExecute();
            authorState = 3;
        } else if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return 0;
    case 3:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            authorState = 4;
        return 0;
    case 4:
        EV_FreeAuthorPak(0, "0248");
        RestoreQdsaCharacters();
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDSA"), 0);
        return 1;
    default:
        return 0;
    }
}
} // namespace shenmue::s2
