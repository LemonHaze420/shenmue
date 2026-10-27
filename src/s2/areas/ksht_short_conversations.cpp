#include "s2/areas/area_ksht.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/npc.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void SelectCamera(std::int32_t camera, shen_name actor) {
    EventSetCamera request{.cleanup = 0, .camera = camera, .eye = actor, .target = actor, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
} // namespace
void KshtPrepareShortConversation(void *) {
    SetupKshtConversationPose(false);
    QueueKshtSerif(0, 15);
    QueueKshtSerif(1, 15);
    SelectCamera(610, KSHT_DUMMY);
    EVT_AT_SetCam(620, KSHT_SIN, KSHT_SIN, 0, 0, 60);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(KSHT_RYO), 0, 0, 1);
    EVT_AT_SetFaceLooks(KSHT_SIN, FacialExpressionId::Angry, 45, 1, 30);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(KSHT_RYO), 0, 150, 1);
    EVT_AT_SetPoint(KSHT_RYO, KSHT_SIN, 5, 3, 0, 0, 150, 0);
    EVT_AT_SetPoint(KSHT_SIN, KSHT_RYO, 5, 1, 2, 45, 150, 0);
    kshtDelay = 150;
}
void KshtWaitShortConversation(void *) {
    if (--kshtDelay <= 0) {
        HLib::SetCurrentTaskTo(KshtSimpleExitCallback);
        shenSetFreeQuestInactive();
    }
}
void KshtPrepareHelp(void *) {
    SetupKshtConversationPose(false);
    SelectCamera(610, KSHT_DUMMY);
}
void KshtWaitHelp(void *) {
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        HLib::SetCurrentTaskTo(KshtSimpleExitCallback);
}
void KshtPrepareInstructions(void *) {
    SetupKshtConversationPose(false);
    QueueKshtSerif(2, 15);
    QueueKshtSerif(3, 15);
    QueueKshtSerif(4, 15);
    SelectCamera(620, KSHT_SIN);
    EVT_AT_SetCam(610, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 75);
    EVT_AT_SetCam(620, KSHT_SIN, KSHT_SIN, 0, 0, 130);
    EVT_AT_SetFaceLooks(KSHT_SIN, FacialExpressionId::Angry, 0, 1, 30);
    EVT_AT_SetFaceLooks(KSHT_RYO, FacialExpressionId::Normal, 75, 1, 30);
    EVT_AT_SetFaceLooks(KSHT_SIN, FacialExpressionId::Normal, 130, 1, 30);
    EVT_AT_SetPoint(KSHT_SIN, KSHT_RYO, 5, 1, 2, 0, 160, 0);
    EVT_AT_SetPoint(KSHT_RYO, KSHT_SIN, 5, 3, 0, 0, 300, 0);
    kshtDelay = 260;
}
void KshtWaitInstructions(void *) {
    if (--kshtDelay <= 0)
        HLib::SetCurrentTaskTo(KshtSimpleExitCallback);
}
void KshtPrepareFinalConversation(void *) {
    SetupKshtConversationPose(false);
    QueueKshtSerif(6, 45);
    SelectCamera(900, KSHT_DUMMY);
    EVT_AT_SetCam(910, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 60);
    kshtDelay = 150;
}
} // namespace shenmue::s2::areas
