#include "s2/areas/pati_event.hpp"
#include "s2/event_queued.hpp"
#include "s2/pak.hpp"
#include <cstdio>
#include <cstring>
namespace shenmue::s2 {
std::int32_t g_patiPlayerCamera, g_patiDealerCamera, g_patiCameraTime, g_patiCameraId;
shen_name g_patiCameraEye, g_patiCameraTarget;
PatiMotionCache g_patiMotionCache[4];
// @verified - p
void PatiDebugLog(const char *, std::int64_t, const char *, ...) {
}
// @verified - p
void QueuePatiVoice(PatiGameWork *game, const char *code, std::int32_t *frame) {
    shen_name name;
    std::memcpy(&name, code, sizeof(name));
    const PatiVoiceLength *length = game->voiceLengths;
    while (length->code.name && length->code.name != name.name)
        ++length;
    const std::int32_t duration = length->frames;
    if (length->code.name) {
        char voice[32];
        std::sprintf(voice, "%s%s", game->voicePrefix, code);
        EVT_AT_SetVoice(voice, 1, *frame);
        *frame += duration + 10;
    }
}
// @verified - p
void SetPatiCamera(std::int32_t camera, shen_name eye, shen_name target, std::int32_t delay) {
    if (camera == g_patiCameraId && eye.name == g_patiCameraEye.name && target.name == g_patiCameraTarget.name)
        return;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EVT_AT_SetCam(camera, eye, target, 0.0f, 0, delay);
    g_patiCameraId = camera;
    g_patiCameraEye = eye;
    g_patiCameraTarget = target;
    g_patiCameraTime = delay;
}
void SetPatiMotion(shen_name character, std::int32_t motion, float frame, float speed, std::int32_t delay, std::uint32_t flags) {
    std::int32_t index = 0;
    while (index < 4 && (g_patiMotionCache[index].character.name != character.name || g_patiMotionCache[index].motion != motion))
        ++index;
    if (index < 4) {
        g_patiMotionCache[index].age = 0;
        for (PatiMotionCache &entry : g_patiMotionCache)
            if (entry.age < 10000)
                ++entry.age;
        return;
    }
    EVT_AT_SetMot(character, motion, frame, speed, flags | (delay == g_patiCameraTime ? 0x40000000U : 0), delay);
    std::int32_t selected = 0;
    for (std::int32_t candidate = 0; candidate < 4; ++candidate) {
        if (g_patiMotionCache[selected].age > g_patiMotionCache[candidate].age)
            selected = candidate;
        if (!g_patiMotionCache[candidate].character.name || g_patiMotionCache[candidate].character.name == character.name) {
            selected = candidate;
            break;
        }
    }
    g_patiMotionCache[selected].character = character;
    g_patiMotionCache[selected].motion = motion;
    g_patiMotionCache[selected].age = 0;
}
// @verified - p
std::int32_t GetPatiPayout(const PatiEventWork *work) {
    switch (work->payoutMode) {
    case 1:
        return 2;
    case 2:
        return 5;
    case 3:
        return 10;
    case 4:
        return 20;
    case 5:
        return 30;
    case 6:
        return 40;
    case 7:
        return 50;
    case 8:
        return 60;
    case 9:
        return 70;
    case 10:
        return 80;
    case 11:
        return 90;
    case 12:
        return 100;
    case 13:
        return 120;
    case 14:
        return 150;
    case 15:
        return 200;
    case 16:
        return 250;
    case 17:
        return 300;
    case 18:
        return 400;
    case 19:
        return 500;
    case 20:
        return 1000;
    case 21:
        return work->game.wins == 2 ? 10 : work->game.wins == 3 ? 20 : 5;
    case 22:
        return work->game.wins == 2 ? 20 : work->game.wins == 3 ? 40 : 10;
    case 23:
        return work->game.wins == 2 ? 50 : work->game.wins == 3 ? 100 : 0;
    case 24:
        return work->game.wins == 2 ? 50 : work->game.wins == 3 ? 150 : 0;
    default:
        return 1;
    }
}
} // namespace shenmue::s2
