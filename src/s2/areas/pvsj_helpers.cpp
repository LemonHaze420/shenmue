#include "s2/areas/area_pvsj.hpp"
#include "s2/areas/pati_event.hpp"
#include "s2/areas/pati_event_tables.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/character.hpp"
#include "s2/input.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/debug.hpp"
#include "s2/ui.hpp"
#include <cstdio>
#include <cstring>
namespace shenmue::s2 {

void PvsjReleaseVoice(PvsjEventWork *work) {
    if (work->voicePrefix[0]) {
        EV_FreeAuthorPak(0, work->voicePrefix);
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 1021, "PATI_FREE_VOICE_FILE:%s\n", work->voicePrefix);
        work->voicePrefix[0] = 0;
    }
}

void PvsjLoadVoice(PvsjEventWork *work, shen_name character) {
    std::sprintf(
        work->voicePrefix, "0104%c%c%c", static_cast<char>(character.name), static_cast<char>(character.name >> 8), static_cast<char>(character.name >> 16));
    const PatiVoicePartition *voice = PATI_VOICE_PARTITIONS;
    for (; voice->name && std::strcmp(voice->name, work->voicePrefix); ++voice) {
    }
    if (!voice->name) {
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 1038, "NoVoiceFile:%s!!\n", work->voicePrefix);
        std::strcpy(work->voicePrefix, "0104");
    }
    EV_ControlPackageResources(PakCategory::LoadAfsWithMode, reinterpret_cast<std::uintptr_t>(work->voiceLoadBuffer),
        reinterpret_cast<std::uintptr_t>(work->gamePartition), static_cast<std::uintptr_t>(voice->text), static_cast<std::uint32_t>(voice->sound));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentAfs, reinterpret_cast<std::uintptr_t>(work->gamePartition),
        static_cast<std::uintptr_t>(voice->text), static_cast<std::uint32_t>(voice->sound));
    const void *source = LoadCurrentPakBinary(work->voicePrefix);
    const auto bytes = static_cast<std::int32_t>(GetCurrentPakBinarySize(work->voicePrefix));
    const auto size = static_cast<std::int32_t>((bytes + ((static_cast<std::uint32_t>(bytes >> 31)) & 3U)) & ~3U);
    if (size >= 0x4000)
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 1055, "SIZE OVER\n");
    std::memcpy(work->voiceLengths, source, size);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FreeAfs, reinterpret_cast<std::uintptr_t>(work->gamePartition),
        static_cast<std::uintptr_t>(voice->text), static_cast<std::uint32_t>(voice->sound));
    EV_FreeAuthorPak(1, work->voicePrefix);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
}



void PvsjQueueVoice(PvsjEventWork *work, const char *code, std::int32_t *frame) {
    shen_name name;
    std::memcpy(&name, code, 4);
    const auto *length = work->voiceLengths;
    for (; length->code.name && length->code.name != name.name; ++length) {
    }
    const auto duration = length->frames;
    if (length->code.name) {
        char voice[32];
        std::sprintf(voice, "%s%s", work->voicePrefix, code);
        EVT_AT_SetVoice(voice, 1, *frame);
        *frame += duration + 10;
    } else
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 1169, "PVSJ:No VoiceID [%s%s]\n", work->voicePrefix, code);
}

void PvsjSetMotion(shen_name character, std::int32_t motion, float frame, float speed, std::uint32_t flags, std::int32_t delay) {
    if (pvsjSkipMotion) {
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 3423, "PVSJ:SkipMot(Closer):mot=%i,frm=%f,flg=%x\n", motion, static_cast<double>(frame), flags);
        return;
    }
    SetPatiMotion(character, motion, frame, speed, delay, flags);
}

void PvsjHintCallback(void *data) {
    auto *work = *static_cast<PvsjEventWork **>(data);
    bool overlay = false, button = false;
    switch (work->state) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 31:
    case 32:
    case 33:
    case 59:
    case 60:
    case 61:
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 15:
    case 16:
    case 17:
    case 34:
        button = true;
        [[fallthrough]];
    default:
        overlay = true;
        break;
    }
    if (overlay && !filmCheckFadeTrackColour()) {
        if (button)
            GetBattleDataConfig(0, SHEN_NAME(BTFT));
        CheckHelpIcon(0x100000, 0, CheckHelpIconOp::FrameOverlay);
    }
    if (!pvsjSkipMotion)
        return;
    const shen_name character = (work->customerActive & 1) ? work->customer : work->owner;
    if (!GetNpcScriptRouteCompletion(character)) {
        pvsjSkipMotion = 0;
        if (DoesScenarioCharacterExist(character))
            PvsjSetMotion(character, work->boards->motions[(work->customerActive & 1) ? 12 : 5], 0, 1, 0, 0);
    }
}
} // namespace shenmue::s2
