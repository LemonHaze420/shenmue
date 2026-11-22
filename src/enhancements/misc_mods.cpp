#include "enhancements/misc_mods.hpp"
#if SHENMUE_MODS_ENABLED
#include <json/json.h>
#include <cJSON.h>
#include <algorithm>
#include <cmath>
#include <limits>
namespace shenmue::mods {
namespace {
constexpr Option options[]{
#define SHEN_MOD(id, kind, key, label, description, initial, minimum, maximum) \
    {Id::id, Kind::kind, key, label, description, initial, minimum, maximum},
#include "enhancements/misc_mods.def"
#undef SHEN_MOD
    {Id::Count, Kind::Toggle, "", "", "", 0, 0, 0}
};
using State = std::array<float, static_cast<std::size_t>(Id::Count)>;
State InitialState() {
    State state{};
    state[static_cast<std::size_t>(Id::FrameRate)] = 30;
    state[static_cast<std::size_t>(Id::TimeSpeed)] = 100;
    state[static_cast<std::size_t>(Id::RainDensity)] = 100;
    state[static_cast<std::size_t>(Id::RainOpacity)] = 100;
    for (const Option &option : Options())
        state[static_cast<std::size_t>(option.id)] = option.initial;
    return state;
}
State state = InitialState();
State recorded = state;
}
std::span<const Option> Options() { return std::span{options}.first(std::size(options) - 1); }
const Option &Spec(Id id) {
    for (const Option &option : Options())
        if (option.id == id)
            return option;
    return options[std::size(options) - 1];
}
float Get(Id id) { return id < Id::Count ? state[static_cast<std::size_t>(id)] : 0; }
bool Enabled(Id id) { return Get(id) != 0; }
void Set(Id id, float value) {
    const Option &spec = Spec(id);
    if (spec.id == Id::Count || !std::isfinite(value))
        return;
    value = std::clamp(value, spec.minimum, spec.maximum);
    if (spec.kind == Kind::Toggle)
        value = value != 0 ? 1.0f : 0.0f;
    else if (spec.kind == Kind::List) {
        bool found = false;
        for (std::int32_t index = 0; ChoiceLabel(id, index); ++index)
            found |= value == ChoiceValue(id, index);
        if (!found)
            return;
    }
    state[static_cast<std::size_t>(id)] = value;
}
const char *ChoiceLabel(Id id, std::int32_t index) {
    if (index < 0)
        return nullptr;
    if (id == Id::FrameRate) {
        constexpr const char *labels[]{"30 FPS", "60 FPS", "200 FPS"};
        return index < 3 ? labels[index] : nullptr;
    }
    if (id == Id::CameraDistance) {
        constexpr const char *labels[]{"Original", "1", "2", "3", "5", "10"};
        return index < 6 ? labels[index] : nullptr;
    }
    constexpr const char *labels[]{"UIOTGLOFF0", "UIOTGLON00"};
    return index < 2 ? labels[index] : nullptr;
}
std::int32_t ChoiceValue(Id id, std::int32_t index) {
    if (Spec(id).kind == Kind::Slider)
        return static_cast<std::int32_t>(index ? Spec(id).maximum : Spec(id).minimum);
    if (id == Id::FrameRate) {
        constexpr std::int32_t values[]{30, 60, 200};
        return index >= 0 && index < 3 ? values[index] : 30;
    }
    if (id == Id::CameraDistance) {
        constexpr std::int32_t values[]{0, 1, 2, 3, 5, 10};
        return index >= 0 && index < 6 ? values[index] : 0;
    }
    return index;
}
void Defaults() { state = InitialState(); }
void Reset() { Defaults(); Record(); }
void Read(const Json::Value &settings) {
    if (!settings.isObject())
        return;
    for (const Option &option : Options()) {
        const Json::Value &value = settings[option.key];
        if (option.kind == Kind::Toggle ? value.isBool() : value.isNumeric())
            Set(option.id, option.kind == Kind::Toggle ? static_cast<float>(value.asBool()) : value.asFloat());
    }
    if (!settings.isMember("FrameRate") && settings["60Fps"].isBool())
        Set(Id::FrameRate, settings["60Fps"].asBool() ? 60.0f : 30.0f);
}
void Write(Json::Value &settings) {
    if (!settings.isObject())
        settings = Json::Value(Json::objectValue);
    for (const Option &option : Options()) {
        if (option.kind == Kind::Toggle)
            settings[option.key] = Enabled(option.id);
        else
            settings[option.key] = Get(option.id);
    }
}
void ReadCJson(const cJSON *settings) {
    if (!settings || settings->type != cJSON_Object)
        return;
    for (const auto &option : Options()) {
        const auto *value = cJSON_GetObjectItem(const_cast<cJSON *>(settings), option.key);
        if (!value) continue;
        if (option.kind == Kind::Toggle && (value->type == cJSON_True || value->type == cJSON_False))
            Set(option.id, value->type == cJSON_True ? 1.0f : 0.0f);
        else if (option.kind != Kind::Toggle && value->type == cJSON_Number)
            Set(option.id, static_cast<float>(value->valuedouble));
    }
}
cJSON *WriteCJson() {
    cJSON *settings = cJSON_CreateObject();
    for (const auto &option : Options()) {
        if (option.kind == Kind::Toggle)
            cJSON_AddBoolToObject(settings, option.key, Enabled(option.id));
        else
            cJSON_AddNumberToObject(settings, option.key, Get(option.id));
    }
    return settings;
}
bool Changed() { return state != recorded; }
void Record() { recorded = state; }
void Revert() { state = recorded; }
#if SHENMUE_ENHANCEMENT_MISC_MODS_CAMERA
namespace {
struct CameraControls {
    Vector3 position{};
    float yaw{}, pitch{};
    std::uint32_t area{};
    bool firstPerson{}, free{}, fHeld{};
} camera;
}
void ResetCamera() { camera = {}; }
bool FreeCameraActive() { return camera.free && Enabled(Id::PhotoMode); }
bool FirstPersonActive() { return camera.firstPerson && Enabled(Id::FirstPerson) && !FreeCameraActive(); }
bool AdjustCamera(bool allowed, float seconds, std::uint32_t area, CameraKey key, Vector3 &eye, Vector3 &target) {
    const bool fHeld = key('F');
    if (!allowed || area != camera.area) {
        camera = {};
        camera.area = area;
        camera.fHeld = fHeld;
        if (!allowed)
            return false;
    }
    if (!Enabled(Id::FirstPerson))
        camera.firstPerson = false;
    else if (fHeld && !camera.fHeld)
        camera.firstPerson = !camera.firstPerson;
    camera.fHeld = fHeld;
    Vector3 direction{target.x - eye.x, target.y - eye.y, target.z - eye.z};
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (!(length > 0.0001f) || !std::isfinite(length))
        return false;
    direction.x /= length;
    direction.y /= length;
    direction.z /= length;
    if (Enabled(Id::PhotoMode)) {
        if (!camera.free) {
            camera.position = eye;
            camera.yaw = std::atan2(direction.x, direction.z);
            camera.pitch = std::asin(std::clamp(direction.y, -1.0f, 1.0f));
            camera.free = true;
        }
        const float dt = std::clamp(seconds, 0.0f, 0.1f);
        camera.yaw += (static_cast<float>(key(0x27)) - static_cast<float>(key(0x25))) * dt;
        camera.pitch = std::clamp(camera.pitch + (static_cast<float>(key(0x26)) - static_cast<float>(key(0x28))) * dt, -1.55f, 1.55f);
        direction = {std::sin(camera.yaw) * std::cos(camera.pitch), std::sin(camera.pitch), std::cos(camera.yaw) * std::cos(camera.pitch)};
        const float forward = static_cast<float>(key('W')) - static_cast<float>(key('S'));
        const float right = static_cast<float>(key('D')) - static_cast<float>(key('A'));
        const float up = static_cast<float>(key('E')) - static_cast<float>(key('Q'));
        const float speed = (key(0x10) ? 10.0f : 3.0f) * dt;
        const float magnitude = std::fmax(1.0f, std::sqrt(forward * forward + right * right + up * up));
        camera.position.x += (forward * direction.x + right * std::cos(camera.yaw)) * speed / magnitude;
        camera.position.y += (forward * direction.y + up) * speed / magnitude;
        camera.position.z += (forward * direction.z - right * std::sin(camera.yaw)) * speed / magnitude;
        eye = camera.position;
        target = {eye.x + direction.x, eye.y + direction.y, eye.z + direction.z};
        return true;
    }
    camera.free = false;
    if (camera.firstPerson) {
        eye = target;
        target = {eye.x + direction.x, eye.y + direction.y, eye.z + direction.z};
        return true;
    }
    const float distance = Get(Id::CameraDistance);
    if (distance <= 0.0f)
        return false;
    eye = {target.x - direction.x * distance, target.y - direction.y * distance, target.z - direction.z * distance};
    return true;
}
#endif
}
#endif
