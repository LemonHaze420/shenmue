#include "s2/author.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/author_control.hpp"
#include "s2/blur_figure_pose.hpp"

#include "s2/camera.hpp"
#include "s2/audio_data.hpp"
#include "s2/door.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/math.hpp"
#include "s2/fog.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scene_data.hpp"
#include "s2/snapshot.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/player.hpp"
#include "s2/weather.hpp"
#include "s2/screen_film.hpp"
#include "s2/character.hpp"
#include "s2/hlib.hpp"
#include "s2/map_info.hpp"
#include "s2/map_assets.hpp"
#include "s2/osage.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/pak.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <span>
#include <string_view>
#include <string>
#include <vector>
#if defined(__SSE__) || defined(_M_X64) || defined(_M_IX86)
#include <xmmintrin.h>
#endif
#include "s2/area_names.hpp"

namespace shenmue::s2 {
namespace {
std::int32_t authorStreamingAudioEnabled{};
std::array<std::int32_t, AUTHOR_STATE_FLAG_CAPACITY> authorStateFlags{};
struct AuthorMultichannelState {
    void *player;
    void *allocation;

    std::uint64_t silentStreamCounter;
    std::uint32_t state;
    std::int32_t countdown;
    std::uint64_t paused;
};
static_assert(sizeof(AuthorMultichannelState) == 0x28);
static_assert(offsetof(AuthorMultichannelState, state) == 0x18);
static_assert(offsetof(AuthorMultichannelState, countdown) == 0x1c);
static_assert(offsetof(AuthorMultichannelState, paused) == 0x20);
AuthorMultichannelState authorMultichannel{};
std::int32_t authorMultichannelDeviceMode{};


void AuthorMultichannelDecoderNullsub(void *, ...) {
}

constexpr std::uint32_t Tag(char a, char b, char c, char d) {
    return static_cast<std::uint8_t>(a) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) << 8U) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(c)) << 16U) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(d)) << 24U);
}

struct FcvChannel {
    std::vector<float> times;
    std::vector<float> values;
    std::vector<float> slopes;
};

struct AuthSpline {
    std::uint32_t flags{};
    std::vector<FcvChannel> channels;

    std::uint32_t *record{};
};

struct AuthCommand {
    std::int32_t frame{};
    std::uint8_t type{};
    std::uint16_t flags{};
    std::vector<std::int32_t> arguments;
};

struct AuthTimestampGroup {
    std::int32_t frame{};
    std::size_t firstCommand{};
    std::size_t commandCount{};
};


struct AuthDynamicStream {
    std::uint32_t tag{};
    std::uint16_t opcode{};
    std::uint16_t recordSize{};
    std::span<const std::byte> records;
    std::size_t cursor{};
};


constexpr std::array<std::uint32_t, 26> authDynamicStreamTags{Tag('S', 'T', 'G', 'C'), Tag('L', 'G', 'H', 'C'), Tag('C', 'H', 'D', 'S'),
    Tag('R', 'F', 'B', 'D'), Tag('E', 'Y', 'E', 'S'), Tag('H', 'A', 'N', 'D'), Tag('C', 'L', 'T', 'H'), Tag('O', 'S', 'G', 'C'), Tag('B', 'L', 'U', 'R'),
    Tag('W', 'A', 'V', 'E'), Tag('F', 'A', 'C', 'X'), Tag('F', 'A', 'D', 'E'), Tag('M', 'B', 'T', 'K'), Tag('S', 'M', 'K', 'C'), Tag('H', 'A', 'P', 'A'),
    Tag('P', 'R', 'T', 'S'), Tag('A', 'F', 'O', 'G'), Tag('F', 'O', 'B', 'J'), Tag('S', 'H', 'D', 'W'), Tag('F', 'C', 'L', 'P'), Tag('F', 'I', 'L', 'T'),
    Tag('L', 'E', 'N', 'S'), Tag('D', 'O', 'O', 'R'), Tag('L', 'G', 'H', 'S'), Tag('W', 'R', 'S', 'T'), Tag('F', 'P', 'L', 'U')};
constexpr std::array<std::uint16_t, 26> authDynamicStreamStrides{12, 12, 16, 28, 44, 20, 16, 16, 16, 12, 24, 12, 0, 24, 24, 44, 28, 44,
    16, 16, 20, 12, 16, 16, 28, 16};

struct AuthActor {
    shen_name subject{};
    std::int32_t transformSpline{-1};
    Vector3 position{};
    Vector3 rotation{};
    Vector3 scale{1.0f, 1.0f, 1.0f};
    std::uint32_t motionId{};
};

struct AuthorRuntime {
    shen_name id{};
    std::vector<AuthSpline> cameras;
    std::vector<AuthSpline> transforms;
    std::vector<AuthCommand> commands;
    std::vector<AuthTimestampGroup> groups;
    std::vector<AuthDynamicStream> dynamicStreams;

    std::byte *handPoseTable{};
    std::array<shen_name, 100> controlledDoors{};
    std::size_t controlledDoorCount{};
    std::array<AuthCharacterState, 32> characterStates{};
    std::size_t characterStateCount{};
    std::vector<std::string> strings;
    std::uint8_t version{};
    bool cleaned{};
    bool motionBanksPublished{};
    std::uint32_t savedWeatherInstruction{};
    std::array<std::uint32_t, 33> savedMapModelEnabled{};
    std::array<std::uint32_t, 33> savedMapFieldEnabled{};
    std::uint32_t savedPlayerFlag{};
    std::size_t nextGroup{};
    std::int32_t activeCamera{-1};
    std::int32_t cameraKeysRemaining{};
    const float *cameraKeyCursor{};
    std::vector<AuthActor> actors;
    std::vector<MotnFile> motions;
};

AuthorRuntime author{};
AuthorController *controller{};
const Matrix4x4 *authorPlacementMatrix{};
HLib::Task *authorMaskTask{};
std::int32_t authorMaskDelay{};


struct AuthorSmoke {
    std::int32_t effect;
    Vector3 position;
    Vector3 spawnVolume;
    Vector3 velocityVariance;
    Vector3 velocity;
    std::uint8_t fade, red, green, blue;
    std::uint8_t falloff;
    std::uint8_t state;
    std::int16_t count;
    float initialScale;
    float width, height;
    float birthRate;
};
static_assert(sizeof(AuthorSmoke) == 0x4c);

struct AuthorMiscParticles {
    std::int32_t effect;
    Vector3 position;
    Vector3 spawnVolume;
    Vector3 velocityVariance;
    Vector3 velocity;
    std::uint16_t state;
    std::int16_t count;
    float floorHeight;
    float width, height;
    float birthRate;
    std::int32_t texture;
};
static_assert(sizeof(AuthorMiscParticles) == 0x4c);
constexpr const char *authorMiscTextures[]{"GRENLEAF.pvr", "REDLEAF.pvr", "SHENLEAF.pvr"};
AuthorSmoke authorSmoke{.effect = -1};
AuthorMiscParticles authorMisc{.effect = -1};
bool authorSkipAllowed{true};
std::int32_t authorActive{};
std::int32_t authorUnusedFlag{};
std::int32_t authorHighQualityHands{1};
std::array<shen_name, AUTHOR_HAND_CHARACTER_CAPACITY> authorHandCharacters{};
std::int32_t authorHandCharacterCount{};
bool authorCannotSkip{};
bool authorForceFilmBars{true};
bool authorApplyYawOffset{};
AuthorPackageLoadResult lastPackageLoadResult{AuthorPackageLoadResult::RuntimeUnavailable};
std::array<char, 128> lastPackageLoadGate{};
AuthorMapInfoLoadResult lastMapInfoLoadResult{AuthorMapInfoLoadResult::RuntimeUnavailable};
std::array<char, 128> lastMapInfoLoadGate{};

constexpr shen_name Name(char a, char b, char c, char d) {
    return shen_name{make_shen_name(a, b, c, d)};
}

template <typename T> bool Read(std::span<const std::byte> bytes, std::size_t offset, T &value);

bool PackageNameEquals(const LoadedMapPackage &package, const char *packageName) {
    if (!packageName)
        return false;
    for (std::size_t index = 0; index < package.name.size(); ++index) {
        const unsigned char left = static_cast<unsigned char>(package.name[index]);
        const unsigned char right = static_cast<unsigned char>(packageName[index]);
        if (std::toupper(left) != std::toupper(right))
            return false;
        if (!left)
            return true;
    }
    return packageName[package.name.size()] == '\0';
}

const LoadedMapPackage *FindAuthorPackage(const char *packageName) {
    for (const LoadedMapPackage &package : GetLoadedMapPackages()) {
        if (PackageNameEquals(package, packageName))
            return &package;
    }
    return nullptr;
}

std::span<const std::byte> FindPackageEntry(const LoadedMapPackage &package, const char *filename) {
    if (const auto entry = package.pks.Find(filename); !entry.empty()) {
        return entry;
    }
    return package.pkf.Find(filename);
}

struct AuthorTrackSelection {
    std::span<const std::byte> bytes;
    shen_name name{};
    std::int32_t slot{-1};
};

AuthorTrackSelection FindAuthorTrack(std::span<const std::byte> bundle, std::int64_t requested) {
    // ReadAUTH: bare TRCK slots ascend from zero; named
    // eight-byte prefixes populate the same 70-slot table from slot 69 down.
    std::uint32_t bundleSize{}, firstTag{};
    if (!Read(bundle, 4, bundleSize) || bundleSize != bundle.size() || !Read(bundle, 8, firstTag))
        return {};
    const bool named = firstTag != Tag('T', 'R', 'C', 'K');
    std::size_t cursor = 8;
    for (std::int32_t index = 0; index < 70 && cursor < bundle.size(); ++index) {
        shen_name name{};
        if (named) {
            if (!Read(bundle, cursor, name) || bundle.size() - cursor < 8)
                return {};
            cursor += 8;
        }
        const std::size_t begin = cursor;
        std::uint32_t tag{}, size{};
        if (!Read(bundle, cursor, tag) || tag != Tag('T', 'R', 'C', 'K') || !Read(bundle, cursor + 4, size) || size < 8 || size > bundle.size() - cursor)
            return {};
        cursor += size;
        if (Read(bundle, cursor, tag) && tag == Tag('A', 'U', 'V', 'W')) {
            if (!Read(bundle, cursor + 4, size) || size < 8 || size > bundle.size() - cursor)
                return {};
            cursor += size;
        }
        const std::int32_t slot = named ? 69 - index : index;
        if ((requested >= 0 && requested < 70 && requested == slot) || (requested > 70 && name.name == static_cast<std::uint32_t>(requested))) {
            return {bundle.subspan(begin, cursor - begin), name, slot};
        }
    }
    return {};
}

void SetPackageLoadGate(AuthorPackageLoadResult result, const char *packageName, const char *asset, shen_name authorId = {}) {
    lastPackageLoadResult = result;
    const char *const package = packageName ? packageName : "<null>";
    if (authorId.name) {
        std::snprintf(lastPackageLoadGate.data(), lastPackageLoadGate.size(), "%s/%s:%.4s", package, asset, reinterpret_cast<const char *>(&authorId.name));
    } else if (asset) {
        std::snprintf(lastPackageLoadGate.data(), lastPackageLoadGate.size(), "%s/%s", package, asset);
    } else {
        std::snprintf(lastPackageLoadGate.data(), lastPackageLoadGate.size(), "%s", package);
    }
}
void SetMapInfoLoadGate(AuthorMapInfoLoadResult result, const char *asset) {
    lastMapInfoLoadResult = result;
    std::snprintf(lastMapInfoLoadGate.data(), lastMapInfoLoadGate.size(), "0004/%s", asset ? asset : "<unknown>");
}

AuthActor *FindActor(std::uint32_t subject);
bool AddScenarioActor(shen_name subject);

template <typename T> bool Read(std::span<const std::byte> bytes, std::size_t offset, T &value) {
    if (offset > bytes.size() || sizeof(T) > bytes.size() - offset)
        return false;
    std::memcpy(&value, bytes.data() + offset, sizeof(T));
    return true;
}

bool ReadChannel(std::span<const std::byte> bytes, std::size_t &cursor, std::size_t end, FcvChannel &channel) {
    std::int32_t count{};
    if (!Read(bytes, cursor, count) || count < 0 || count > 10000)
        return false;
    cursor += sizeof(count);
    const std::size_t size = static_cast<std::size_t>(count) * sizeof(float);
    if (cursor > end || size * 3 > end - cursor)
        return false;
    channel.times.resize(count);
    channel.values.resize(count);
    channel.slopes.resize(count);
    if (size) {
        std::memcpy(channel.times.data(), bytes.data() + cursor, size);
        cursor += size;
        std::memcpy(channel.values.data(), bytes.data() + cursor, size);
        cursor += size;
        std::memcpy(channel.slopes.data(), bytes.data() + cursor, size);
        cursor += size;
    }
    return true;
}

bool ReadSpline(std::span<const std::byte> bytes, std::size_t begin, std::size_t end, std::size_t axisCount, bool hasFlags, AuthSpline &spline) {
    std::size_t cursor = begin;
    if (hasFlags) {
        if (!Read(bytes, cursor, spline.flags))
            return false;
        cursor += sizeof(spline.flags);
    }
    spline.channels.resize(axisCount);
    for (FcvChannel &channel : spline.channels) {
        if (!ReadChannel(bytes, cursor, end, channel))
            return false;
    }
    return cursor == end;
}

std::int16_t DegreesToAngle(float degrees) {
    return static_cast<std::int16_t>(degrees * (65536.0f / 360.0f));
}

float FloatFromBits(std::int32_t bits) {
    float value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}


bool LoadAuthorActors() {
    author.actors.clear();
    for (const AuthCommand &command : author.commands) {
        if (command.type < 2 || command.type > 6 || command.arguments.empty()) {
            continue;
        }
        const shen_name subject{static_cast<std::uint32_t>(command.arguments[0])};
        if (subject.name == SHEN_NAME(NONE) || FindActor(subject.name)) {
            continue;
        }
        if (!AddScenarioActor(subject)) {
            return false;
        }
    }
    return !author.actors.empty();
}

AuthActor *FindActor(std::uint32_t subject) {
    const auto found = std::find_if(author.actors.begin(), author.actors.end(), [subject](const AuthActor &actor) { return actor.subject.name == subject; });
    return found == author.actors.end() ? nullptr : &*found;
}

void ReportAuthorResourceFailure(AuthorMapInfoLoadResult result, shen_name subject, const char *resource) {
    std::array<char, 64> asset{};
    std::snprintf(asset.data(), asset.size(), "%s:%.4s", resource, reinterpret_cast<const char *>(&subject.name));
    SetMapInfoLoadGate(result, asset.data());
}


const MotnSequence *FindAuthorSequence(std::uint32_t motionId) {
    for (const MotnFile &file : author.motions) {
        if (const MotnSequence *const sequence = FindMotnSequence(file, motionId)) {
            return sequence;
        }
    }
    return FindLoadedMotionSequence(motionId);
}

std::int32_t TruncateAuthorFloat(float value) {
#if defined(__SSE__) || defined(_M_X64) || defined(_M_IX86)
    return _mm_cvttss_si32(_mm_set_ss(value));
#else
    return value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : INT32_MIN;
#endif
}

// @verified - p
bool InitializeAuthorSplineTrack(CameraSplineTrack &track, float *coefficients, std::uint32_t count, float *times, float *values, float *tangents) {
    if (!coefficients)
        coefficients = static_cast<float *>(HLib::AllocateTaskData(8ULL * count, AUTHOR_SPLINE_ALLOCATION));
    if (!coefficients)
        return false;
    track.coefficients = coefficients;
    *coefficients = 0.0f;
    track.firstKey = 0;
    track.tangents = tangents;
    track.count = count;
    track.times = times;
    track.values = values;
    track.segmentCount = AllocSpline(track);
    return true;
}

// @verified - p
void InitializeAuthorSplines(AuthActorContext &context) {
    for (CameraSplineTrack &track : context.splineTracks)
        if (track.count)
            InitializeAuthorSplineTrack(track, nullptr, track.count, track.times, track.values, track.tangents);
    context.splineFrame = 0.0f;
}

// @verified - p
void ReleaseAuthorSplines(AuthActorContext &context) {
    for (CameraSplineTrack &track : context.splineTracks)
        if (track.count)
            HLib::FreeTaskData(track.coefficients);
}

// @verified - p
bool EvaluateAuthorSpline(AuthActorContext &context, AuthTransformPose &pose, const Matrix4x4 *placement) {
    const float time = context.splineFrame / 30.0f;
    context.splineFrame += shenGetScaledDeltaTickTime(1.0f);
    float values[12];
    std::int32_t result = 0;
    for (std::size_t index = 0; index < context.splineTracks.size(); ++index) {
        CameraSplineTrack &track = context.splineTracks[index];
        if (track.count)
            result |= DoKeyframeCubicHermiteInterpolation(track, time, &values[index]);
    }
    pose.position = {values[0], values[1], values[2]};
    pose.rotation = {
        TruncateAuthorFloat(values[3] * 8192.0f / 45.0f), TruncateAuthorFloat(values[4] * 8192.0f / 45.0f), TruncateAuthorFloat(values[5] * 8192.0f / 45.0f)};
    if (context.splineTracks[6].count)
        pose.scale = {values[6], values[7], values[8]};
    if (placement) {
        Matrix4x4 transform;
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(placement);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(pose.rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(pose.rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(pose.rotation.x));
        Matrix4x4::GetTransformMatrix(&transform);
        Matrix4x4::Pop(1);
        const float sine = -transform.rows[0].z;
        std::uint16_t x, y, z;
        if (static_cast<double>(sine) > 0.999999) {
            x = FixedAtan2(transform.rows[1].x, transform.rows[1].y);
            y = 0x4000;
            z = 0;
        } else if (static_cast<double>(sine) < -0.999999) {
            x = FixedAtan2(-transform.rows[1].x, transform.rows[1].y);
            y = 0xc000;
            z = 0;
        } else {
            const float cosine = std::sqrt(1.0f - sine * sine);
            x = FixedAtan2(transform.rows[1].z / cosine, transform.rows[2].z / cosine);
            y = FixedAtan2(sine, cosine);
            z = FixedAtan2(transform.rows[0].y / cosine, transform.rows[0].x / cosine);
        }
        if (x >= 0x8000U && z >= 0x8000U) {
            x = static_cast<std::uint16_t>(x + 0x8000U);
            y = static_cast<std::uint16_t>(0x8000U - y);
            z = static_cast<std::uint16_t>(z + 0x8000U);
        }
        const std::uint16_t alternateX = static_cast<std::uint16_t>(x + 0x8000U);
        const std::uint16_t alternateY = static_cast<std::uint16_t>(0x8000U - y);
        const std::uint16_t alternateZ = static_cast<std::uint16_t>(z + 0x8000U);
        const unsigned wide = unsigned(Abs16(x) > 0x4000U) + unsigned(Abs16(y) > 0x4000U) + unsigned(Abs16(z) > 0x4000U);
        const unsigned alternateWide = unsigned(Abs16(alternateX) > 0x4000U) + unsigned(Abs16(alternateY) > 0x4000U) + unsigned(Abs16(alternateZ) > 0x4000U);
        if (wide > alternateWide) {
            x = alternateX;
            y = alternateY;
            z = alternateZ;
        }
        pose.rotation = {static_cast<std::int16_t>(x), static_cast<std::int16_t>(y), static_cast<std::int16_t>(z)};
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(placement);
        Matrix4x4::TransformPoint(pose.position, pose.position);
        Matrix4x4::SetActiveTransform(placement);
        Matrix4x4::TransformPoint(pose.scale, pose.scale);
        Matrix4x4::Pop(1);
    }
    return result != 0;
}

AuthTransformPose authorTransformPose{};

bool ExtensionEquals(const PakContainerEntry &entry, const char *extension) {
    for (std::size_t index = 0; index < entry.extension.size(); ++index) {
        const char left = static_cast<char>(std::toupper(static_cast<unsigned char>(entry.extension[index])));
        const char right = static_cast<char>(std::toupper(static_cast<unsigned char>(extension[index])));
        if (left != right)
            return false;
    }
    return true;
}

bool LoadAuthorMotionBanks() {
    author.motions.clear();
    for (const LoadedMapPackage &package : GetLoadedMapPackages()) {
        for (const PakContainerEntry &entry : package.pks.entries) {
            if (!ExtensionEquals(entry, "MOTN"))
                continue;
            std::array<char, 14> name{};
            std::size_t cursor = 0;
            for (const char value : entry.stem) {
                if (!value)
                    break;
                name[cursor++] = value;
            }
            if (cursor + 5 > name.size())
                continue;
            name[cursor++] = '.';
            std::memcpy(name.data() + cursor, "MOTN", 4);
            const std::span<const std::byte> bytes = package.pks.Find(name.data());
            MotnFile file{};
            if (!bytes.empty() && ReadMotnFile(bytes, file)) {
                author.motions.push_back(std::move(file));
            }
        }
    }
    for (const AuthCommand &command : author.commands) {
        if (command.type == 3 && command.arguments.size() >= 2 && !FindAuthorSequence(static_cast<std::uint32_t>(command.arguments[1]) & 0xffffU)) {
            return false;
        }
    }
    return true;
}

std::string_view AuthString(std::int32_t index) {
    return index >= 0 && static_cast<std::size_t>(index) < author.strings.size() ? std::string_view(author.strings[index]) : std::string_view{};
}

bool AuthCameraCommand(std::int32_t phase, const std::int32_t *args, AuthActorContext &) {
    if (phase == 0) {
        author.activeCamera = args[1];


        if (author.activeCamera >= 0 && static_cast<std::size_t>(author.activeCamera) < author.cameras.size()) {
            const AuthSpline &spline = author.cameras[static_cast<std::size_t>(author.activeCamera)];
            author.cameraKeysRemaining = static_cast<std::int32_t>(spline.channels[0].times.size());
            author.cameraKeyCursor = spline.channels[0].times.data();
            const std::array<void *, 3> request{spline.record, nullptr, nullptr};
            camCtrlToggleSplineRebuildFlag(3, request.data());
        }
        camCtrlSetState(CameraState::SplineInterpolation);
    }
    return false;
}

constexpr std::array<ScreenFilmColourRequest, 4> authoredFadeColours{{
    {{0, 0, 0, 0}, {255, 0, 0, 0}, 0},
    {{255, 0, 0, 0}, {0, 0, 0, 0}, 0},
    {{0, 255, 255, 255}, {255, 255, 255, 255}, 0},
    {{255, 255, 255, 255}, {0, 255, 255, 255}, 0},
}};

HLib::Task *AuthorFilmTask() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? control->screenFilmTask : nullptr;
}

bool ReadAuthoredFilmRequest(std::span<const std::byte> record, std::uint16_t opcode, ScreenFilmColourRequest &request) {
    std::int16_t duration{};
    if (opcode == 11) {
        std::int16_t colour{};
        Read(record, 8, colour);
        if (colour < 0 || static_cast<std::size_t>(colour) >= authoredFadeColours.size())
            return false;
        request = authoredFadeColours[colour];
        Read(record, 10, duration);
    } else {
        Read(record, 8, request.start);
        Read(record, 12, request.target);
        Read(record, 16, duration);
    }
    request.durationFrames = duration;
    return true;
}

void ApplyAuthoredFilmRecord(std::span<const std::byte> record, std::uint16_t opcode) {
    ScreenFilmColourRequest request{};
    if (!ReadAuthoredFilmRequest(record, opcode, request))
        return;
    HLib::Task *const filmTask = AuthorFilmTask();
    if (opcode == 20) {
        if (g_currentMap == AREA_OP02)
            g_authoredFilmTrackSelection = 0;
        if (g_authoredFilmTrackSelection == 0) {
            constexpr std::array<std::int32_t, 4> alternateFrames{742, 810, 964, 2297};
            std::int32_t frame{};
            Read(record, 0, frame);
            for (const std::int32_t alternateFrame : alternateFrames) {
                if (frame != alternateFrame)
                    continue;
                ScreenFilmColourRequest alternate = request;
                if (frame == alternateFrames[0])
                    alternate.durationFrames = 40;
                if (frame == alternateFrames[1])
                    alternate.target[0] = 0;
                WaitForNextTask(filmTask, alternate, 1);
            }
        }
    }
    WaitForNextTask(filmTask, request, 0);
}

void StopAuthoredFilm() {
    std::span<const std::byte> finalRecord;
    std::int32_t finalFrame = -1;
    std::uint16_t finalOpcode{};
    for (const AuthDynamicStream &stream : author.dynamicStreams) {
        if (stream.opcode != 11 && stream.opcode != 20)
            continue;

        std::span<const std::byte> record;
        for (std::size_t cursor = stream.cursor; cursor + 8 <= stream.records.size();) {
            std::int32_t recordFrame{};
            std::int16_t size{};
            Read(stream.records, cursor, recordFrame);
            Read(stream.records, cursor + 6, size);
            if (recordFrame < 0 || size <= 0 || static_cast<std::size_t>(size) > stream.records.size() - cursor)
                break;
            record = stream.records.subspan(cursor, static_cast<std::size_t>(size));
            cursor += static_cast<std::size_t>(size);
        }
        if (record.empty())
            continue;
        std::int32_t frame{};
        Read(record, 0, frame);
        // Equal timestamps select FADE, not FILT.
        if (frame < finalFrame || (frame == finalFrame && stream.opcode != 11))
            continue;
        finalRecord = record;
        finalFrame = frame;
        finalOpcode = stream.opcode;
    }
    ScreenFilmColourRequest request{};
    if (!finalRecord.empty() && ReadAuthoredFilmRequest(finalRecord, finalOpcode, request)) {
        request.durationFrames = 1;
        WaitForNextTask(AuthorFilmTask(), request, 0);
    }
}

AuthCharacterState *FindAuthCharacterState(shen_name subject) {
    for (std::size_t index = 0; index < author.characterStateCount; ++index) {
        if (author.characterStates[index].subject.name == subject.name)
            return &author.characterStates[index];
    }
    return nullptr;
}

// @verified - p
std::uint64_t GetAuthorCharacterHandModelMask(Character *character) {
    std::uint8_t result = 0;
    const HandParams *const left = GetCharacterHand(character, 1);
    if (left && left->primaryNode)
        result = 2;
    const HandParams *const right = GetCharacterHand(character, 0);
    if (right && right->primaryNode)
        result |= 1U;
    return result;
}

// @verified - p
std::uint64_t PrepareAuthorCharacterHands(Character *character, shen_name subject) {
    for (std::int32_t index = 0; index < authorHandCharacterCount; ++index) {
        if (authorHandCharacters[index].name == subject.name)
            return 2;
    }
    authorHandCharacters[authorHandCharacterCount++] = subject;
    if (GetAuthorCharacterHandModelMask(character) == 3)
        return 2;
    const std::uint8_t result = static_cast<std::uint8_t>(SetLowQualityHands(character));
    AuthCharacterState *const state = FindAuthCharacterState(subject);
    if (state && result == 1)
        state->flags |= 4U;
    return result;
}

// @verified - p
void CaptureAuthCharacterState(Character *character, shen_name subject) {
    if (!character || FindAuthCharacterState(subject) || author.characterStateCount == author.characterStates.size())
        return;
    AuthCharacterState &state = author.characterStates[author.characterStateCount++];
    state.subject = subject;
    state.noMotion = FindMotionWork(character) == nullptr;
    state.pluralControl = SetPluralControlFor(character, 0);
    state.displayEnabled = (character->renderFlags & 4U) != 0;
    // sctlAuthorApplyKeyframesForObject saves the old EAX returned by
    // ccmSetFlagsFor, not the cleared value.
    if (CharacterControlModel *const control = FindCharacterControlModel(character)) {
        state.controlFlags = control->flags;
        control->flags &= ~0x0fU;
    }
    if (!state.noMotion) {
        std::int32_t mode{-1};
        ReadReflectorBoardState(character, mode, state.reflector);
        state.reflectorMode = static_cast<std::uint8_t>(mode);
        state.motionSnapshot = static_cast<std::int8_t>(CaptureMotionSnapshot(subject));
        state.entityType = static_cast<std::int8_t>(character->type);
        state.drawControlEnabled = IsCharacterDrawControlEnabled(character);
        state.characterFlag40 = (character->flags & 0x40U) != 0;
        const Vector3 eyeTarget{};
        MotionWork *const motion = FindMotionWork(character);
        character->flags &= ~0x40U;
        SetFaceFlagsForChara_0(character, FacialNodeReplacement);
        SetHighModelAndFaceFlagsForChara(character, 0, 0.0f, 1.0f);
        SetFacialBlinkDelay(character, 0);
        AT_SetEyeCtrl(character, 0.0f, 0.0f, 0.0f);
        hmdlSetFlagsFor(character, 2, 8, 0);
        CopyMotionTransition(motion, nullptr, 0, 1, 0);
        mtwkSetRequestedRootMode(motion, 0);
        UpdateMotionWorkWithPosition(motion, nullptr, 0);
        SetEyeToChara(character, &eyeTarget, 0, 0);
        ResetReflectorBoardMode(character, 1);
        SetReflectorBoardValues(character, {0.3f, 0.3f, 0.3f, 0.3f});
        if (authorHighQualityHands && PrepareAuthorCharacterHands(character, subject)) {
            HandParams *const right = GetCharacterHand(character, 0);
            if (right) {
                if (right->regionState || right->previousRegionState)
                    state.flags |= 8U;
                else
                    SetCharacterHandRegionEnabled(character, 0, true, 1);
            }
            HandParams *const left = GetCharacterHand(character, 1);
            if (left) {
                if (left->regionState || left->previousRegionState)
                    state.flags |= 0x10U;
                else
                    SetCharacterHandRegionEnabled(character, 1, true, 1);
            }
        }
    }
}


void RestoreAuthCharacterStates() {
    for (std::size_t index = 0; index < author.characterStateCount; ++index) {
        const AuthCharacterState &state = author.characterStates[index];
        Character *const character = FindScenarioCharacter(controller->task, state.subject);
        if (!character)
            continue;
        SetPluralControlFor(character, static_cast<std::int16_t>(state.pluralControl));
        if (state.displayEnabled)
            character->renderFlags |= 4U;
        else
            character->renderFlags &= ~4U;
        // FindCCM_OR_flags assigns the captured flags, despite its name.
        SetCharacterControlModelFlags(character, state.controlFlags);
        SetHighModelAndFaceFlagsForChara(character, 0, 10.0f, 1.0f);
        AT_SetEyeCtrl(character, 10.0f, 0.0f, 0.0f);
        SetFacialBlinkDelay(character, 90);
        if (FindMotionWork(character)) {
            const Vector3 eyeTarget{};
            SetEyeToChara(character, &eyeTarget, 0, 0);
            ResetReflectorBoardMode(character, static_cast<std::int8_t>(state.reflectorMode));
            SetReflectorBoardValues(character, state.reflector);
            if (state.motionSnapshot >= 0) {
                RestoreMotionSnapshot(state.subject, static_cast<std::uint8_t>(state.motionSnapshot));
                ReleaseMotionSnapshot(static_cast<std::uint8_t>(state.motionSnapshot));
            }
            SetCharacterType(character, state.entityType);
            SetCharacterDrawControlEnabled(character, state.drawControlEnabled != 0);
            HandParams *const right = GetCharacterHand(character, 0);
            if (right) {
                SetHandPrimaryNodeRotation(*right, {});
                SetCharacterHandRegionEnabled(character, 0, (state.flags & 8U) != 0, 1);
            }
            HandParams *const left = GetCharacterHand(character, 1);
            if (left) {
                SetHandPrimaryNodeRotation(*left, {});
                SetCharacterHandRegionEnabled(character, 1, (state.flags & 0x10U) != 0, 1);
            }
            if (state.flags & 4U)
                hndlSetFlags(character);
            if (state.characterFlag40)
                character->flags |= 0x40U;
            else
                character->flags &= ~0x40U;
            SetClothFlag(character, 2, 0);
            SetClothFlag(character, 4, 0);
            clmdUtil(character, 0);
        }
        if ((state.flags & 1U) != 0)
            figpAssignModeForCharacter(character, 0);
        if ((state.flags & 2U) != 0)
            ResetCharaFixo(character);
    }
    author.characterStateCount = 0;
}

bool AuthTransformCommand(std::int32_t phase, const std::int32_t *args, AuthActorContext &context) {
    AuthActor *actor = FindActor(static_cast<std::uint32_t>(args[0]));
    if (!actor && phase == 0 && AddScenarioActor(context.subject))
        actor = FindActor(context.subject.name);
    if (!actor) {
        if (phase == 0)
            ReportAuthorResourceFailure(AuthorMapInfoLoadResult::ModelMissing, context.subject, "SCNC character");
        return false;
    }
    Character *const character = FindScenarioCharacter(controller->task, context.subject);
    if (phase == 0) {
        actor->transformSpline = args[1];
        AuthSpline &spline = author.transforms[args[1]];
        for (std::size_t index = 0; index < context.splineTracks.size(); ++index) {
            CameraSplineTrack &track = context.splineTracks[index];
            if (index < 6 || author.version >= 5) {
                FcvChannel &channel = spline.channels[index];
                track.count = static_cast<std::uint32_t>(channel.times.size());
                track.times = channel.times.data();
                track.values = channel.values.data();
                track.tangents = channel.slopes.data();
            } else {
                track.count = 0;
                track.times = nullptr;
                track.values = nullptr;
                track.tangents = nullptr;
            }
        }
        for (std::size_t axis = 0; axis < 3; ++axis) {
            context.position[axis] = context.splineTracks[axis].values[0];
            context.rotation[axis] = TruncateAuthorFloat(context.splineTracks[axis + 3].values[0] * 8192.0f / 45.0f);
            if (context.splineTracks[6].count)
                context.scale[axis] = context.splineTracks[axis + 6].values[0];
        }
        InitializeAuthorSplines(context);
        context.flags |= std::uint16_t{1};
        context.setupCountdown = 0;
        EvaluateAuthorSpline(context, authorTransformPose, authorPlacementMatrix);
        EvaluateAuthorSpline(context, authorTransformPose, authorPlacementMatrix);
    } else if (phase == 1) {
        if (context.setupCountdown) {
            --context.setupCountdown;
            return true;
        }
        if (!EvaluateAuthorSpline(context, authorTransformPose, authorPlacementMatrix))
            return false;
        const AuthCharacterState *const state = FindAuthCharacterState(context.subject);
        if (state && (state->flags & 2U) != 0)
            return true;
        context.position = authorTransformPose.position;
        context.rotation = authorTransformPose.rotation;
        context.scale = authorTransformPose.scale;
    } else if (phase == 2 || phase == 4) {
        if (phase == 4 && TruncateAuthorFloat(context.splineFrame) < static_cast<std::int32_t>(controller->framesRemaining)) {
            bool active;
            do
                active = EvaluateAuthorSpline(context, authorTransformPose, authorPlacementMatrix);
            while (TruncateAuthorFloat(context.splineFrame) < static_cast<std::int32_t>(controller->framesRemaining));
            if (active) {
                context.position = authorTransformPose.position;
                context.rotation = authorTransformPose.rotation;
                context.scale = authorTransformPose.scale;
                actor->position = authorTransformPose.position;
                for (std::size_t axis = 0; axis < 3; ++axis)
                    actor->rotation[axis] = static_cast<float>(authorTransformPose.rotation[axis]) * 45.0f / 8192.0f;
                character->entity.node.position = authorTransformPose.position;
                for (std::size_t axis = 0; axis < 3; ++axis)
                    character->entity.node.rotation[axis] = authorTransformPose.rotation[axis];
                if (context.splineTracks[6].count)
                    SetMotionBoneTarget(FindMotionWork(character), 7, &authorTransformPose.scale);
            }
        }
        ReleaseAuthorSplines(context);
        actor->transformSpline = -1;
        context.flags &= static_cast<std::uint16_t>(~std::uint16_t{1});
        return false;
    } else
        return false;
    actor->position = authorTransformPose.position;
    for (std::size_t axis = 0; axis < 3; ++axis)
        actor->rotation[axis] = static_cast<float>(authorTransformPose.rotation[axis]) * 45.0f / 8192.0f;
    if (context.splineTracks[6].count)
        actor->scale = authorTransformPose.scale;
    character->entity.node.position = authorTransformPose.position;
    for (std::size_t axis = 0; axis < 3; ++axis)
        character->entity.node.rotation[axis] = authorTransformPose.rotation[axis];
    if (context.splineTracks[6].count)
        SetMotionBoneTarget(FindMotionWork(character), 7, &authorTransformPose.scale);
    if (phase == 0)
        CaptureAuthCharacterState(character, context.subject);
    return true;
}

bool AuthMotionCommand(std::int32_t phase, const std::int32_t *args, AuthActorContext &context) {
    AuthActor *actor = FindActor(context.subject.name);
    if (!actor && phase == 0 && AddScenarioActor(context.subject))
        actor = FindActor(context.subject.name);
    if (!actor) {
        if (phase == 0)
            ReportAuthorResourceFailure(AuthorMapInfoLoadResult::ModelMissing, context.subject, "SCNC character");
        return false;
    }
    Character *character = FindScenarioCharacter(controller->task, context.subject);
    TimedNodeMotion *const nodeMotion = static_cast<TimedNodeMotion *>(FindModelComponent(character, TASK_TMNM));
    context.motionType = nodeMotion ? 1U : 0U;
    if (phase == 0) {
        const bool transformed = (context.flags & 1U) != 0;
        if (!transformed) {
            actor->position = {};
            actor->rotation = {};
            context.position = {};
            if (nodeMotion) {
                character->entity.node.position = {};
                for (std::size_t axis = 0; axis < 3; ++axis)
                    character->entity.node.rotation[axis] = 0;
            }
            context.rotation = {};
        }
        actor->motionId = static_cast<std::uint32_t>(args[1]) & 0xffffU;
        if (nodeMotion ? FindLoadedMotionData(actor->motionId).empty() : !FindAuthorSequence(actor->motionId)) {
            ReportAuthorResourceFailure(AuthorMapInfoLoadResult::MotionMissing, context.subject, "MOTI motion");
            return false;
        }
        context.motionSpeed = author.version >= 6 ? FloatFromBits(args[5]) : 1.0f;
        context.motionRemaining = static_cast<float>(args[3] - args[2]) / context.motionSpeed + 1.0f;
        if (nodeMotion && !character->model) {
            ReportAuthorResourceFailure(AuthorMapInfoLoadResult::ModelMissing, context.subject, "TMNM model");
            return false;
        }
        if (nodeMotion) {
            InitializeTimedNodeMotion(*nodeMotion, static_cast<MT7Model *>(character->model), static_cast<std::uint16_t>(actor->motionId));
            nodeMotion->begin = static_cast<float>(args[2] - 1);
            nodeMotion->end = static_cast<float>(args[3] - 1);
            nodeMotion->flags = 0;
            nodeMotion->speed = context.motionSpeed;
        } else if (!BeginAuthoredCharacterMotion(character, static_cast<std::uint16_t>(actor->motionId), transformed ? &context.rotation : nullptr,
                       static_cast<float>(args[2] - 1), static_cast<float>(args[3] - 1), context.motionSpeed, static_cast<std::uint32_t>(args[4]))) {
            ReportAuthorResourceFailure(AuthorMapInfoLoadResult::ModelMissing, context.subject, "MTWK component");
            return false;
        }
        context.flags |= std::uint16_t{6};
        return true;
    }
    if (phase == 1) {
        context.flags &= static_cast<std::uint16_t>(~std::uint16_t{4});
        context.motionRemaining -= 1.0f;
        if (context.motionRemaining == 0.0f) {
            if (nodeMotion)
                nodeMotion->speed = 0.0f;
            else
                SetMotionSpeed(FindMotionWork(character), 0, MotionRegion::LowerBody);
            return false;
        }
        return true;
    }
    if (phase == 3 || phase == 4) {
        if (nodeMotion) {
            if (phase == 3)
                nodeMotion->speed = context.motionSpeed;
        } else {
            FinishAuthoredCharacterMotion(character, context.motionSpeed, phase == 4, authorApplyYawOffset);
        }
    }
    return false;
}

bool AuthFaceCommand(std::int32_t, const std::int32_t *args, AuthActorContext &) {
    // sctlAuthorApplyFaceLooks is immediate in every phase.
    if (args[0] != 0) {
        Character *const character = FindScenarioCharacter(controller->task, shen_name{static_cast<std::uint32_t>(args[0])});
        if (character && FindMotionWork(character)) {
            const float frames = static_cast<float>(static_cast<std::uint32_t>(args[4]));
            const float scale = static_cast<float>(static_cast<std::uint32_t>(args[5])) / 100.0f;
            SetUpperFacialExpression(character, args[1], frames, scale);
            SetLowerFacialExpression(character, args[2], frames, scale);
            SetFacialMouthExpression(character, args[3], frames, 1.0f);
        }
    }
    return false;
}

// @verified - p
bool AuthDialogueCommand(std::int32_t phase, const std::int32_t *args, AuthActorContext &context) {
    if (phase != 0) {
        if (phase != 1)
            return false;
        const std::uint8_t countdown = context.voiceCountdown;
        context.voiceCountdown = static_cast<std::uint8_t>(countdown - 1U);
        if (countdown)
            return true;
        Character *const character = FindScenarioCharacter(controller->task, shen_name{static_cast<std::uint32_t>(args[0])});
        if (character) {
            if (args[2] != -1) {
                const std::byte *const table = controller->lipTable;
                std::int32_t offset;
                std::memcpy(&offset, table + 16 + 4ULL * static_cast<std::uint32_t>(args[2]), sizeof(offset));
                AssignFacialClip(character, reinterpret_cast<const std::int16_t *>(table + offset));
            } else
                AssignFacialClip(character, context.generatedMouthClip.data());
        }
        return false;
    }
    controller->dialogueFrames = TruncateAuthorFloat(FloatFromBits(args[4]) * 30.0f);
    Character *const character = FindScenarioCharacter(controller->task, shen_name{static_cast<std::uint32_t>(args[0])});
    controller->dialogueCharacter = character;
    if (!character)
        return false;
    const std::string_view path = AuthString(args[1]);
    const std::size_t slash = path.find_last_of('/');
    if (slash != std::string_view::npos) {
        std::array<char, AUTHOR_DIALOGUE_CODE_CAPACITY> voiceCode;
        const std::string_view code = path.substr(slash + 1);
        std::memcpy(voiceCode.data(), code.data(), code.size() + 1);
        if (char *const dot = std::strchr(voiceCode.data(), '.'))
            *dot = 0;
        ConversationBuffer *const voice = EV_PlayDialogue(voiceCode.data());
        for (const AuthorDialogueStat &entry : authorDialogueStats)
            if (std::strcmp(voiceCode.data(), entry.code) == 0)
                IncrementSteamStat(entry.stat);
        if (std::strcmp(voiceCode.data(), "0009A009") == 0 && static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 50)
            IncrementSteamStat(12);
        if (voice)
            return false;
    }
    context.voiceCountdown = 13;
    const std::int8_t version = static_cast<std::int8_t>(author.version);
    const std::uint32_t alternate = static_cast<std::uint32_t>(args[3]);
    if (version >= 3 && alternate != UINT32_MAX && alternate > 0x0c000000U) {
        EV_PlayDialogue(reinterpret_cast<const char *>(static_cast<std::uintptr_t>(alternate)));
        context.voiceCountdown = 5;
    }
    if (version < 4 || args[2] != -1)
        return controller->lipTable != nullptr;
    const float duration = FloatFromBits(args[4]);
    if (duration < 0.5f || args[4] == 0)
        return false;
    std::int32_t count = TruncateAuthorFloat(duration * 60.0f * 0.125f);
    if (count > 73)
        count = 73;
    std::size_t words = 0;
    for (std::int32_t index = 0; index < count; ++index) {
        const float random = IncrementSharedPrngFloat();
        context.generatedMouthClip[words + 1] = 8;
        context.generatedMouthClip[words] = static_cast<std::int16_t>(TruncateAuthorFloat(random * 6.0f));
        words += 2;
    }
    context.generatedMouthClip[words] = -1;
    return true;
}

// @verified - p
std::int16_t AuthorSoundPan(shen_name subject) {
    Character *const character = FindScenarioCharacter(controller->task, subject);
    if (!character)
        return 0;
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    const Vector3 point = GetCharacterWorldPosition(character);
    const std::int16_t yaw = static_cast<std::int16_t>(camCtrlGetNextRotY());
    const std::int16_t angle = InverseFixedAtan2(point.z - eye.z, point.x - eye.x);
    const std::int16_t delta = static_cast<std::int16_t>(0x4000 - yaw + angle);
    const std::int16_t folded = delta < 0 ? static_cast<std::int16_t>(~delta) : delta;
    return static_cast<std::int16_t>(-63 * (folded - 0x4000) / 0x4000);
}

// @verified - p
std::int32_t AuthorAudioProperty(std::uint32_t bank, std::uint32_t lane, std::uint32_t command, std::int32_t value) {
    SubmitAicaBackendRequest({.operation = AicaBackendOperation::SetBankProperty, .bank = 1, .value = 0x5a});
    return SubmitAicaBackendRequest({.operation = AicaBackendOperation::TwoChannelCommand,
        .command = command,
        .bank = static_cast<std::uint8_t>(bank),
        .channel = static_cast<std::uint8_t>(lane),
        .value = static_cast<std::int16_t>(value)});
}

// @verified - p
std::int32_t ExecuteAuthorMusicCommand(const char *name) {
    if (std::strstr(name, "fade_out")) {
        spmFadeOut();
        return 1;
    }
    if (std::strstr(name, "bgm_off")) {
        EV_LoadAudio(GetCurrentBgm(), SOUND_PROGRAM_STOP_INDEX);
        return 1;
    }
    const char *const command = std::strstr(name, "bgm00");
    if (!command)
        return 0;
    EV_LoadAudio(FindSoundProgramNameForMap(GetCurrentBgm()), static_cast<std::int8_t>(command[5]) - '0');
    EV_spmBeginSampleExecute();
    return 1;
}

bool AuthSoundCommand(std::int32_t phase, const std::int32_t *args, AuthActorContext &) {
    if (phase == 1) {
        const std::uint32_t lane = static_cast<std::uint32_t>(args[6]);
        std::byte *const storage = reinterpret_cast<std::byte *>(controller) + offsetof(AuthorController, audioCountdown) + 4ULL * lane;
        std::uint32_t previous;
        std::memcpy(&previous, storage, sizeof(previous));
        const std::uint32_t next = previous - 1U;
        std::memcpy(storage, &next, sizeof(next));
        if (!previous)
            return false;
        AuthorAudioProperty(static_cast<std::uint32_t>(args[5]), lane, 0xab0U, AuthorSoundPan(shen_name{static_cast<std::uint32_t>(args[0])}));
        return true;
    }
    if (phase != 0)
        return false;
    if (args[1] <= 0) {
        if (!GetSamplePlaybackSuppressed())
            ExecuteAuthorMusicCommand(AuthString(args[2]).data());
        return false;
    }
    std::uint32_t flags = 0, bank = 4, lane = 16;
    std::int32_t gain = 127;
    std::int16_t pan = 0;
    float loopContext = 0.0f;
    if (args[3] == -1) {
        flags = static_cast<std::uint32_t>(args[7]);
        gain = args[4];
        bank = static_cast<std::uint32_t>(args[5]);
        lane = static_cast<std::uint32_t>(args[6]);
        if (flags & 4U)
            return true;
        if (flags & 2U) {
            pan = AuthorSoundPan(shen_name{static_cast<std::uint32_t>(args[0])});
            loopContext = FloatFromBits(args[8]) * 30.0f;
            const std::int32_t countdown = TruncateAuthorFloat(loopContext);
            std::byte *const storage = reinterpret_cast<std::byte *>(controller) + offsetof(AuthorController, audioCountdown) + 4ULL * lane;
            std::memcpy(storage, &countdown, sizeof(countdown));
        }
    }
    if (!GetSamplePlaybackSuppressed())
        Loop3DSoundEffect(static_cast<std::uint32_t>(args[1]), loopContext);
    if (flags & 1U)
        AuthorAudioProperty(bank, lane, 0x7b0U, gain);
    if (flags & 2U) {
        AuthorAudioProperty(bank, lane, 0xab0U, pan);
        return true;
    }
    return false;
}

bool AuthAnimatedObjectCommand(std::int32_t, const std::int32_t *, AuthActorContext &) {
    // sctlAuthorApplyAnimatedObj only reads camera positions
    // into discarded locals and returns one for every phase.
    return true;
}

constexpr std::array<AuthCommandHandler, 7> authHandlers{
    AuthAnimatedObjectCommand, AuthCameraCommand, AuthTransformCommand, AuthMotionCommand, AuthFaceCommand, AuthDialogueCommand, AuthSoundCommand};

void DispatchAuthCommand(const AuthCommand &command) {
    if (command.type >= authHandlers.size() || !authHandlers[command.type] || command.arguments.empty())
        return;
    AuthActiveCommand *previous = nullptr;
    AuthActiveCommand *available = nullptr;
    for (AuthActiveCommand &slot : controller->activeCommands) {
        if (!slot.arguments && !available)
            available = &slot;
        if (!previous && slot.arguments && slot.channel == command.type && slot.arguments[0] == command.arguments[0])
            previous = &slot;
    }
    if (!available)
        return;
    AuthActorContext *context = nullptr;
    for (AuthActorContext &candidate : controller->actorContexts) {
        if (candidate.subject.name == static_cast<std::uint32_t>(command.arguments[0])) {
            context = &candidate;
            break;
        }
    }
    if (!context) {
        for (AuthActorContext &candidate : controller->actorContexts) {
            if (!candidate.subject.name) {
                candidate.subject.name = static_cast<std::uint32_t>(command.arguments[0]);
                context = &candidate;
                break;
            }
        }
    }
    if (!context)
        return;
    *available = {command.type, authHandlers[command.type], command.arguments.data(), context};
    if (!available->handler(0, available->arguments, *context)) {
        available->arguments = nullptr;
    }
    if (previous)
        previous->arguments = nullptr;
}

void AuthorMask_Callback(void *) {
    if (authorMaskDelay) {
        --authorMaskDelay;
        return;
    }
    filmSetCinemaBarVisibility(nullptr, FilmControlFlag::Hide);
    authorMaskTask = nullptr;
    HLib::DestroyCurrentTask();
}

void BeginAuthorRuntime() {
    // Named AUTH consumes the package/NPC MOTI registry without publishing private banks.
    if (!author.motions.empty()) {
        RegisterAuthorMotionBanks(author.motions);
        author.motionBanksPublished = true;
    }
    if (authorMaskTask) {
        HLib::DestroyTask(authorMaskTask);
        authorMaskTask = nullptr;
        authorMaskDelay = 0;
    }
    filmSetCinemaBarVisibility(nullptr, authorForceFilmBars ? FilmControlFlag::ForceShow : FilmControlFlag::Force);
    SetSoundListenerUsesCamera(1);
    author.savedWeatherInstruction = GetCurrentWeatherInstruction();
    const std::span<const MapModelRuntimeSlot, 33> modelSlots = GetMapModelSlots();
    const std::span<const MapFieldDepthSlot, 33> fieldSlots = GetMapFieldDepthSlots();
    for (std::size_t index = 0; index < modelSlots.size(); ++index) {
        author.savedMapModelEnabled[index] = modelSlots[index].enabled;
        author.savedMapFieldEnabled[index] = fieldSlots[index].enabled;
    }
    author.savedPlayerFlag = plymRemoveFlags(4U) & 4U;
    authorSmoke = {.effect = -1};
    authorMisc = {.effect = -1};
    authorCannotSkip = false;
    author.cameraKeysRemaining = 0;
    authorHandCharacters = {};
    authorHandCharacterCount = 0;
    controller->loaded = 1;
}

// @verified - p
void DoSteamChecks(shen_name map, std::int32_t act, std::int8_t track, std::uint64_t scene) {
    if (map.name == SHEN_NAME(0142)) {
        if (act == 2 && track == 0)
            IncrementSteamStat(act + 2);
    } else if (map.name == SHEN_NAME(AR02)) {
        if (act == 1 && track == 'D' && scene == SHEN_NAME(0014))
            IncrementSteamStat(0);
    } else if (map.name == SHEN_NAME(WB01)) {
        if (act == 1 && track == '<')
            IncrementSteamStat(act);
    } else if (map.name == SHEN_NAME(WB00)) {
        if (act == 2 && track == 'C' && scene == SHEN_NAME(AT01))
            IncrementSteamStat(act);
    } else if (map.name == SHEN_NAME(QE00)) {
        if (act == 3 && track == 'B')
            IncrementSteamStat(act + 2);
    } else if (map.name == SHEN_NAME(QDKJ)) {
        if (act == 3 && track == 'C')
            IncrementSteamStat(6);
    } else if (map.name == SHEN_NAME(QR00)) {
        if (act == 3 && track == 'D' && scene == SHEN_NAME(0264))
            IncrementSteamStat(7);
    } else if (map.name == SHEN_NAME(QFRR)) {
        if (act == 3 && track == 'D')
            IncrementSteamStat(act + 5);
    } else if (map.name == SHEN_NAME(KSH1)) {
        if (act == 4 && track == 'E')
            IncrementSteamStat(act + 5);
    }
}

// @verified - p
void authDestroySubtitleConverseData() {
    if (!controller || !controller->loaded)
        return;
    scenCvsCtrlFree(nullptr);
    scenDestroySubtitles(controller->task);
}

// @verified - p
void sctlAuthApplyMotion() {
    for (const AuthCommand &command : author.commands) {
        if (command.type != 3)
            continue;
        AuthCharacterState *const state = FindAuthCharacterState(shen_name{static_cast<std::uint32_t>(command.arguments[0])});
        if (!state)
            continue;
        state->finalMotionArguments = command.arguments.data();
        state->finalMotionFrame = command.frame;
    }
    for (AuthCharacterState &state : author.characterStates) {
        const std::int32_t *const arguments = state.finalMotionArguments;
        if (!arguments)
            continue;
        const float speed = FloatFromBits(arguments[5]);
        const float end = ((static_cast<float>(arguments[3]) - static_cast<float>(arguments[2])) + 1.0f) / speed + static_cast<float>(state.finalMotionFrame);
        if (static_cast<float>(controller->currentFrame) >= end)
            continue;
        Character *const character = FindScenarioCharacter(controller->task, shen_name{static_cast<std::uint32_t>(arguments[0])});
        if (!character)
            continue;
        TimedNodeMotion *const nodeMotion = static_cast<TimedNodeMotion *>(FindModelComponent(character, TASK_TMNM));
        MotionWork *const work = nodeMotion ? nullptr : FindMotionWork(character);
        if (!nodeMotion && !work)
            continue;
        const std::uint16_t motion = static_cast<std::uint16_t>(arguments[1]);
        if (nodeMotion) {
            InitializeTimedNodeMotion(*nodeMotion, static_cast<MT7Model *>(character->model), motion);
        } else {
            SetMotionRegionFlag(work, 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(work, motion, MotionRegion::LowerBody);
        }
        const float time = (static_cast<float>(static_cast<std::int32_t>(controller->framesRemaining)) - static_cast<float>(state.finalMotionFrame)) * speed +
            static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(arguments[2]) - 1U));
        if (nodeMotion) {
            nodeMotion->frame = time;
            nodeMotion->speed = speed;
        } else {
            SetMotionRegionEvalTime(work, time, MotionRegion::LowerBody);
            SetMotionSpeed(work, speed, MotionRegion::LowerBody);
        }
        Vector3 position = character->entity.node.position;
        Vector3i rotation{character->entity.node.rotation[0], character->entity.node.rotation[1], character->entity.node.rotation[2]};
        if (authorApplyYawOffset) {
            if (!nodeMotion)
                ApplyCharacterMotionToPose(character, &position, &rotation, time, motion);
            character->entity.node.position = position;
            character->entity.node.rotation[0] = rotation.x;
            character->entity.node.rotation[1] = rotation.y;
            character->entity.node.rotation[2] = rotation.z;
        } else {
            if (!nodeMotion)
                SampleNativeMotionRoot(motion, GetMotionBoneIdForEntityType(GetCharacterImageSourceType(character)), time, position);
            position.y = 0.0f;
            if (arguments[4] & 1)
                position.x = -position.x;
            Vector3 displacement{};
            Matrix4x4_Push();
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
            Matrix4x4::TransformPoint(position, displacement);
            Matrix4x4::Pop(1);
            character->entity.node.position = {displacement.x + character->entity.node.position.x, displacement.y + character->entity.node.position.y,
                displacement.z + character->entity.node.position.z};
        }
    }
}


void CleanupAuthCommands() {
    if (!controller || !controller->loaded)
        return;
    authorMaskDelay = 1;
    authorMaskTask = HLib::QueueTaskAfterCurrentTask(controller->task, AuthorMask_Callback, 3, 0x0b, static_cast<std::int32_t>(SHEN_NAME(SCMS)));
    controller->loaded = 0;
    const bool skipped = controller->field06c != 0;
    for (AuthActiveCommand &slot : controller->activeCommands) {
        if (!slot.arguments)
            continue;
        slot.handler(skipped ? 4 : 2, slot.arguments, *slot.context);
        slot.arguments = nullptr;
    }
    if (skipped) {
        sctlAuthAllocSplines(2 * static_cast<std::int32_t>(controller->framesRemaining));
        sctlAuthApplyMotion();
    }
    for (AuthActorContext &context : controller->actorContexts) {
        if ((context.flags & 2U) == 0)
            continue;
        const auto subject = static_cast<std::int32_t>(context.subject.name);
        AuthMotionCommand(skipped ? 4 : 3, &subject, context);
    }
    if (g_cameraControl) {
        g_cameraControl->nextRotation.roll = 0;
        g_cameraControl->currentFovAngle = 10922;
    }
    RestoreAuthCharacterStates();
    for (std::size_t index = 0; index < author.controlledDoorCount; ++index) {
        if (author.controlledDoors[index].name)
            SetDoorSubjectEnabled(author.controlledDoors[index], 1);
    }
    SetWeatherInstruction(0xffffffffU, false);
    SetWeatherInstruction(author.savedWeatherInstruction, true);
    SetSoundListenerUsesCamera(0);
    SetWeatherLightManager(1);
    for (std::uint32_t index = 0; index < author.savedMapModelEnabled.size(); ++index) {
        SetMapModelSlotEnabled(static_cast<std::int32_t>(index), static_cast<std::int32_t>(author.savedMapModelEnabled[index]));
        SetMapFieldDepthSlotEnabled(index, author.savedMapFieldEnabled[index]);
    }
    plymAddFlags(author.savedPlayerFlag);
    authorHighQualityHands = 1;
    if (authorSmoke.effect >= 0) {
        FreeParticleEffect(0, static_cast<std::uint32_t>(authorSmoke.effect));
        authorSmoke.effect = -1;
        authorSmoke.state = 0;
    }
    if (authorMisc.effect >= 0) {
        FreeParticleEffect(3, static_cast<std::uint32_t>(authorMisc.effect));
        authorMisc.effect = -1;
        authorMisc.state = 0;
    }
    const MapRenderProfile &profile = MatchMapRenderProfile(g_currentMap);
    camCtrlSetFarDistance(profile.farDistance);
    camCtrlSetNearDistance(profile.nearDistance);
    camCtrlSetNpcRadius(profile.npcRadius);
    authorApplyYawOffset = false;
    if (skipped)
        authorCannotSkip = true;
    authorUnusedFlag = 0;
    authorActive = 0;
    authorPlacementMatrix = nullptr;
    controller->skipTimer = 0.0f;
    controller->skipToNext = 0;
    controller->finished = 0;
    DoSteamChecks(g_currentMap, g_actNumber, controller->track, controller->postUpdateContext);
    nullsub();
    g_authoredFilmTrackSelection = -1;
    author.cleaned = true;
}

void ApplyAuthoredDynamicStreams() {
    for (AuthDynamicStream &stream : author.dynamicStreams) {
        while (stream.cursor + 8 <= stream.records.size()) {

            std::int32_t frame{};
            std::uint16_t opcode{};
            if (!Read(stream.records, stream.cursor, frame) || !Read(stream.records, stream.cursor + 4, opcode) || frame < 0 ||
                frame > controller->currentFrame - 1) {
                break;
            }
            std::int16_t recordSize = static_cast<std::int16_t>(stream.recordSize);
            if (stream.opcode == 12)
                Read(stream.records, stream.cursor + 6, recordSize);
            if (recordSize <= 0 || static_cast<std::size_t>(recordSize) > stream.records.size() - stream.cursor)
                break;
            const std::span<const std::byte> record = stream.records.subspan(stream.cursor, static_cast<std::size_t>(recordSize));

            if (opcode != stream.opcode) {
                stream.cursor += static_cast<std::size_t>(recordSize);
                continue;
            }
            if (stream.tag == Tag('S', 'T', 'G', 'C')) {
                std::int16_t kind{};
                std::uint8_t index{}, enabled{};
                Read(record, 8, kind);
                Read(record, 10, index);
                Read(record, 11, enabled);
                if (kind == 0)
                    SetMapModelSlotEnabled(index, enabled);
                else if (kind == 1)
                    SetMapFieldDepthSlotEnabled(index, enabled);
            } else if (stream.tag == Tag('C', 'H', 'D', 'S')) {
                shen_name subject{};
                std::int32_t enabled{};
                Read(record, 8, subject);
                Read(record, 12, enabled);
                if (Character *character = FindScenarioCharacter(controller->task, subject)) {
                    constexpr std::uint8_t displayEnabled = 0x04;
                    if (enabled)
                        character->renderFlags |= displayEnabled;
                    else
                        character->renderFlags &= ~displayEnabled;
                }
            } else if (stream.tag == Tag('R', 'F', 'B', 'D')) {
                std::int32_t enabled{};
                shen_name subject{};
                Vector3 values{};
                Read(record, 8, enabled);
                Read(record, 12, subject);
                Read(record, 16, values);
                Character *const character = FindScenarioCharacter(controller->task, subject);
                ResetReflectorBoardMode(character, enabled ? 1 : 0);
                if (enabled)
                    SetReflectorBoardValues(character, {1.0f, values.x, values.y, values.z});
            } else if (stream.tag == Tag('E', 'Y', 'E', 'S')) {


                (void)ApplyAuthoredEyesRecord(record, controller->task);
            } else if (stream.opcode == 5) {


                shen_name subject{};
                std::uint8_t enabled{}, right{};
                std::int16_t pose{};
                std::uint16_t ticks{};
                Read(record, 8, subject);
                Read(record, 12, enabled);
                Read(record, 13, right);
                Read(record, 14, pose);
                Read(record, 16, ticks);
                Character *const character = FindScenarioCharacter(controller->task, subject);
                PrepareAuthorCharacterHands(character, subject);
                const std::int32_t side = right ? 0 : 1;
                if (HandParams *const hand = GetCharacterHand(character, side)) {
                    if (!enabled) {
                        SetCharacterHandRegionEnabled(character, static_cast<std::uint8_t>(side), false, 1);
                    } else {
                        if (!hand->regionState && !hand->previousRegionState)
                            SetCharacterHandRegionEnabled(character, static_cast<std::uint8_t>(side), true, 1);
                        ParseHandPoseCommand(hand, author.handPoseTable + 228 * pose, static_cast<std::int16_t>(ticks));
                    }
                }
            } else if (stream.opcode == 24) {

                shen_name subject{};
                std::int32_t right{};
                Vector3i rotation{};
                Read(record, 8, subject);
                Read(record, 12, right);
                Read(record, 16, rotation);
                if (Character *const character = FindScenarioCharacter(controller->task, subject))
                    if (HandParams *const hand = GetCharacterHand(character, right ? 0 : 1))
                        SetHandPrimaryNodeRotation(*hand, rotation);
            } else if (stream.opcode == 10) {

                shen_name subject{};
                std::int8_t upper{}, lower{}, mouth{};
                std::int16_t frames{};
                float scale{};
                Read(record, 8, subject);
                Read(record, 12, upper);
                Read(record, 13, lower);
                Read(record, 14, mouth);
                Read(record, 16, frames);
                Read(record, 20, scale);
                Character *const character = FindScenarioCharacter(controller->task, subject);
                SetUpperFacialExpression(character, upper, static_cast<float>(frames), scale);
                SetLowerFacialExpression(character, lower, static_cast<float>(frames), scale);
                SetFacialMouthExpression(character, mouth, static_cast<float>(frames), 1.0f);
            } else if (stream.opcode == 12) {

                shen_name subject{};
                std::uint16_t mode{};
                Read(record, 8, subject);
                Read(record, 12, mode);
                Character *const character = FindScenarioCharacter(controller->task, subject);
                if (character) {
                    switch (mode) {
                    case 1:
                    case 2: {
                        std::int16_t frames{};
                        Read(record, 14, frames);
                        const float weight = mode == 1 ? 1.0f : 0.0f;
                        AT_SetEyeCtrl(character, static_cast<float>(frames), weight, weight);
                        break;
                    }
                    case 3:
                        AT_SetEyeCtrl(character, 0.0f, 0.0f, 0.0f);
                        StartFacialBlink(character);
                        break;
                    case 4: {
                        std::int16_t frames{};
                        float right{}, left{};
                        Read(record, 14, frames);
                        Read(record, 16, right);
                        Read(record, 20, left);
                        AT_SetEyeCtrl(character, static_cast<float>(frames), left, right);
                        break;
                    }
                    case 5:
                        AT_SetEyeCtrl(character, 0.0f, 0.0f, 0.0f);
                        SetFacialBlinkDelay(character, 90);
                        break;
                    case 6:
                        SetFacialBlinkDelay(character, 0);
                        break;
                    default:
                        break;
                    }
                }
            } else if (stream.tag == Tag('P', 'R', 'T', 'S')) {
                (void)ApplyAuthoredPartsRecord(record, controller->task);
            } else if (stream.opcode == 11 || stream.opcode == 20) {
                ApplyAuthoredFilmRecord(record, stream.opcode);
            } else if (stream.opcode == 16) {
                std::int32_t mode{};
                std::uint32_t first{}, second{}, distance{};
                float red{}, green{}, blue{};
                Read(record, 8, mode);
                Read(record, 12, first);
                Read(record, 16, second);
                if (mode == 0) {
                    Read(record, 12, red);
                    Read(record, 16, green);
                    Read(record, 20, blue);
                    Read(record, 24, distance);
                    SetWeatherInstruction(0x780U, false);
                    SetFogRed(red);
                    SetFogGreen(green);
                    SetFogBlue(blue);
                    SetFogDistance(distance);
                    ReinitFogTasks(1);
                } else if (mode == 1) {
                    SetWeatherInstruction(0x1800U, false);
                    SetCloudAndSkyColour(first, second);
                } else if (mode == 2) {
                    Read(record, 12, red);
                    Read(record, 16, green);
                    SetWeatherInstruction(0x6000U, false);
                    shenUpdatePlanetByLayer(red, 0, 0);
                    shenUpdatePlanetByLayer(red, 0, 1);
                    shenUpdatePlanetByLayer(green, 1, 0);
                    shenUpdatePlanetByLayer(green, 1, 1);
                } else if (mode == 3) {
                    SetWeatherInstruction(0x1800U, false);
                    EV_SetPlanetLayerColours_Impl(first, second, 0);
                    EV_SetPlanetLayerColours_Impl(first, second, 1);
                }
            } else if (stream.opcode == 18) {
                shen_name subject{};
                std::uint16_t mode{}, enabled{};
                Read(record, 8, subject);
                Read(record, 12, mode);
                Read(record, 14, enabled);
                Character *const character = FindScenarioCharacter(controller->task, subject);
                if (mode <= 1) {
                    SetCharacterType(character, mode == 0 ? 0 : 4);
                    SetCharacterDrawControlEnabled(character, enabled > 0);
                } else if (mode == 2) {
                    SetCharacterDrawControlEnabled(character, false);
                }
            } else if (stream.opcode == 1) {

                std::uint32_t program{};
                Read(record, 8, program);
                SetMapModelDataDefaultVertexShader();
                LGHT_Parser_Internal(program);
                PushGlobalLightingProgram();
                SetWeatherInstruction(4, false);
            } else if (stream.opcode == 7) {

                shen_name subject{};
                std::int32_t mode{};
                Read(record, 8, subject);
                Read(record, 12, mode);
                if (Character *const character = FindScenarioCharacter(controller->task, subject)) {
                    if (mode == -1)
                        toggleOsagFlags(character);
                    else if (mode >= 0)
                        SetCharacterOsageMode(character, static_cast<std::uint8_t>(mode));
                }
            } else if (stream.opcode == 8) {


                std::int8_t scale{}, direction{}, mode{};
                shen_name subject{};
                Read(record, 8, scale);
                Read(record, 9, direction);
                Read(record, 10, mode);
                Read(record, 12, subject);
                Character *const character = FindScenarioCharacter(controller->task, subject);
                figpAssignModeForCharacter(character, mode);
                if (mode)
                    figpAssignMotScaleAndDirFor(character, scale, direction);
                if (AuthCharacterState *const state = subject.name ? FindAuthCharacterState(subject) : nullptr) {
                    if (mode)
                        state->flags |= 1U;
                    else
                        state->flags &= ~1U;
                }
            } else if (stream.opcode == 9) {

            } else if (stream.opcode == 13) {

                std::int32_t command{};
                Read(record, 8, command);
                AuthorSmoke &smoke = authorSmoke;
                const std::uint32_t effect = static_cast<std::uint32_t>(smoke.effect);
                switch (command) {
                case 0: {
                    std::int32_t control{};
                    Read(record, 12, control);
                    if (control == 0 && smoke.state == 2) {
                        SuspendParticleEffect(0, effect);
                        smoke.state = 1;
                    } else if (control == 1 && smoke.state == 1) {
                        ResumeParticleEffect(0, effect);
                        smoke.state = 2;
                    } else if (control == 2 && smoke.state) {
                        FreeParticleEffect(0, effect);
                        smoke.state = 0;
                        smoke.effect = -1;
                    }
                    break;
                }
                case 1:
                    Read(record, 12, smoke.count);
                    if (smoke.state)
                        break;
                    smoke.effect =
                        static_cast<std::int32_t>(CreateSmokeEffect(smoke.position, smoke.spawnVolume, smoke.count, smoke.birthRate, shen_name{}, 0));
                    SetParticleEffectColour(0, static_cast<std::uint32_t>(smoke.effect), 0, smoke.red, smoke.green, smoke.blue);
                    SetSmokeEffectFadeFactor(static_cast<std::uint32_t>(smoke.effect), smoke.fade);
                    SetSmokeEffectFalloff(static_cast<std::uint32_t>(smoke.effect), smoke.falloff);
                    SetParticleEffectInitialScale(0, static_cast<std::uint32_t>(smoke.effect), smoke.initialScale);
                    SetParticleEffectSize(0, static_cast<std::uint32_t>(smoke.effect), smoke.width, smoke.height);
                    SetParticleEffectBirthRate(0, static_cast<std::uint32_t>(smoke.effect), smoke.birthRate);
                    SetParticleEffectVelocity(0, static_cast<std::uint32_t>(smoke.effect), smoke.velocity, smoke.velocityVariance);
                    smoke.state = 2;
                    break;
                case 2:
                case 3:
                    Read(record, 12, command == 2 ? smoke.position : smoke.spawnVolume);
                    if (smoke.state)
                        SetParticleEffectPosition(0, effect, &smoke.position, &smoke.spawnVolume);
                    break;
                case 4:
                case 5:
                    Read(record, 12, command == 4 ? smoke.velocityVariance : smoke.velocity);
                    if (smoke.state)
                        SetParticleEffectVelocity(0, effect, smoke.velocity, smoke.velocityVariance);
                    break;
                case 6:
                    Read(record, 12, smoke.falloff);
                    if (smoke.state)
                        SetSmokeEffectFalloff(effect, smoke.falloff);
                    break;
                case 7:
                    Read(record, 12, smoke.initialScale);
                    if (smoke.state)
                        SetParticleEffectInitialScale(0, effect, smoke.initialScale);
                    break;
                case 8:
                    Read(record, 12, smoke.width);
                    Read(record, 16, smoke.height);
                    if (smoke.state)
                        SetParticleEffectSize(0, effect, smoke.width, smoke.height);
                    break;
                case 9:
                    Read(record, 12, smoke.fade);
                    Read(record, 13, smoke.red);
                    Read(record, 14, smoke.green);
                    Read(record, 15, smoke.blue);
                    if (!smoke.state)
                        break;
                    SetParticleEffectColour(0, effect, 0, smoke.red, smoke.green, smoke.blue);
                    SetSmokeEffectFadeFactor(effect, smoke.fade);
                    break;
                case 10:
                    Read(record, 12, smoke.birthRate);
                    if (smoke.state)
                        SetParticleEffectBirthRate(0, effect, smoke.birthRate);
                    break;
                default:
                    break;
                }
            } else if (stream.opcode == 14) {

                std::int32_t command{};
                Read(record, 8, command);
                AuthorMiscParticles &misc = authorMisc;
                const std::uint32_t effect = static_cast<std::uint32_t>(misc.effect);
                switch (command) {
                case 0: {
                    std::int32_t control{};
                    Read(record, 12, control);
                    if (control == 0 && misc.state == 2) {
                        SuspendParticleEffect(3, effect);
                        misc.state = 1;
                    } else if (control == 1 && misc.state == 1) {
                        ResumeParticleEffect(3, effect);
                        misc.state = 2;
                    } else if (control == 2 && misc.state) {
                        FreeParticleEffect(3, effect);
                        misc.effect = -1;
                        misc.state = 0;
                    }
                    break;
                }
                case 1:
                    Read(record, 12, misc.count);
                    if (misc.state)
                        break;
                    misc.effect = static_cast<std::int32_t>(CreateFallingParticleEffect(
                        misc.position, misc.spawnVolume, misc.count, misc.birthRate, shen_name{}, "misc", authorMiscTextures[misc.texture], misc.floorHeight));
                    SetParticleEffectSize(3, static_cast<std::uint32_t>(misc.effect), misc.width, misc.height);
                    SetParticleEffectBirthRate(3, static_cast<std::uint32_t>(misc.effect), misc.birthRate);
                    SetParticleEffectVelocity(3, static_cast<std::uint32_t>(misc.effect), misc.velocity, misc.velocityVariance);
                    misc.state = 2;
                    break;
                case 2:
                case 3:
                    Read(record, 12, command == 2 ? misc.position : misc.spawnVolume);
                    if (misc.state)
                        SetParticleEffectPosition(3, effect, &misc.position, &misc.spawnVolume);
                    break;
                case 4:
                case 5:
                    Read(record, 12, command == 4 ? misc.velocityVariance : misc.velocity);
                    if (misc.state)
                        SetParticleEffectVelocity(3, effect, misc.velocity, misc.velocityVariance);
                    break;
                case 7:
                    Read(record, 12, misc.floorHeight);
                    if (misc.state)
                        SetFallingParticleFloor(misc.floorHeight);
                    break;
                case 8:
                    Read(record, 12, misc.width);
                    Read(record, 16, misc.height);
                    if (misc.state)
                        SetParticleEffectSize(3, effect, misc.width, misc.height);
                    break;
                case 10:
                    Read(record, 12, misc.birthRate);
                    if (misc.state)
                        SetParticleEffectBirthRate(3, effect, misc.birthRate);
                    break;
                case 11:
                    Read(record, 12, misc.texture);
                    break;
                default:
                    break;
                }
            } else if (stream.opcode == 21) {


                std::int32_t mode{};
                Read(record, 8, mode);
                SetWeatherLightManager(mode != 0);
            } else if (stream.opcode == 6) {

                shen_name subject{};
                std::int32_t mode{};
                Read(record, 8, subject);
                Read(record, 12, mode);
                if (Character *const character = FindScenarioCharacter(controller->task, subject)) {
                    switch (mode) {
                    case -5:
                        SetClothFlag(character, 16, 1);
                        break;
                    case -4:
                        SetClothFlag(character, 4, 0);
                        break;
                    case -3:
                        SetClothFlag(character, 4, 1);
                        break;
                    case -2:
                        SetClothFlag(character, 2, 1);
                        break;
                    case -1:
                        SetClothFlag(character, 2, 0);
                        break;
                    default:
                        clmdUtil(character, static_cast<std::int8_t>(mode));
                        break;
                    }
                }
            } else if (stream.opcode == 25) {


                shen_name subject{};
                std::int16_t enabled{};
                Read(record, 8, enabled);
                Read(record, 12, subject);
                SetPluralControlFor(FindScenarioCharacter(controller->task, subject), enabled);
            } else if (stream.tag == Tag('F', 'O', 'B', 'J')) {
                shen_name source{}, target{};
                std::int32_t bone{};
                std::array<std::int32_t, 3> rotation{};
                Vector3 position{};
                Read(record, 8, source);
                Read(record, 12, target);
                Read(record, 16, bone);
                Read(record, 20, rotation);
                Read(record, 32, position);
                Character *const character = FindScenarioCharacter(controller->task, target);
                AuthCharacterState *const state = FindAuthCharacterState(target);
                if (source.name == SHEN_NAME(RECV)) {
                    if (character) {
                        ResetCharaFixo(character);
                        if (state)
                            state->flags &= ~2U;
                    }
                } else {
                    constexpr std::array<std::uint16_t, 8> boneIds{1, 5, 3, 21, 14, 31, 26, 0};
                    Character *const owner = FindScenarioCharacter(controller->task, source);
                    if (character && owner && bone >= 0 && static_cast<std::size_t>(bone) < boneIds.size()) {
                        SetFixoDataForMotionWork(character, owner, boneIds[bone], position, rotation);
                        if (state)
                            state->flags |= 2U;
                    }
                }
            } else if (stream.tag == Tag('F', 'C', 'L', 'P')) {
                std::int32_t mode{};
                float distance{};
                Read(record, 8, mode);
                Read(record, 12, distance);
                if (mode == 0)
                    camCtrlSetFarDistance(distance);
                else if (mode == 1)
                    camCtrlSetNearDistance(distance);
                else
                    camCtrlSetNpcRadius(distance);
            } else if (stream.tag == Tag('D', 'O', 'O', 'R')) {
                shen_name subject{};
                std::int32_t enabled{};
                Read(record, 8, subject);
                Read(record, 12, enabled);
                if (subject.name) {
                    SetDoorSubjectEnabled(subject, enabled);
                    const auto end = author.controlledDoors.begin() + author.controlledDoorCount;
                    if (std::none_of(author.controlledDoors.begin(), end, [subject](shen_name name) { return name.name == subject.name; }) &&
                        author.controlledDoorCount < author.controlledDoors.size()) {
                        author.controlledDoors[author.controlledDoorCount++] = subject;
                    }
                }
            } else if (stream.tag == Tag('L', 'G', 'H', 'S')) {
                std::uint32_t index{}, enabled{};
                Read(record, 8, index);
                Read(record, 12, enabled);
                SetLightEntryIndex(index, enabled);
            }
            stream.cursor += static_cast<std::size_t>(recordSize);
        }
    }
}
// @verified - p
std::int32_t AuthorFramePauseNullsub(const AuthorController *) {
    return 0;
}

// @verified - p
void UpdateAuthorFCVCurves() {
    bool crossed = false;
    std::int32_t remaining = author.cameraKeysRemaining;
    const float *key = author.cameraKeyCursor;
    if (remaining >= 2) {
        const float current = static_cast<float>(controller->currentFrame) / 30.0f - 0.016666668f;
        const float next = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(controller->currentFrame) + 1U)) / 30.0f - 0.016666668f;
        do {
            const float first = key[0];
            if (current <= first)
                break;
            const float second = key[1];
            ++key;
            if (next > second && first != second)
                crossed = true;
            --remaining;
        } while (remaining >= 2);
        author.cameraKeysRemaining = remaining;
        author.cameraKeyCursor = key;
    }
    for (AuthActorContext &context : controller->actorContexts) {
        if ((context.flags & 2U) == 0)
            continue;
        Character *const character = FindScenarioCharacter(controller->task, context.subject);
        MotionWork *const motion = FindMotionWork(character);
        if (crossed) {
            if ((context.flags & 4U) && context.motionType == 0) {
                context.transformFlags = 0x80;
                context.transformFlags |= SetMotionRegionFlag(motion, 68, 1, MotionRegion::LowerBody) != 0;
            }
            SetClothFlag(character, 16, 1);
            toggleOsagFlags(character);
        } else if (context.motionType == 0 && static_cast<std::int8_t>(context.transformFlags) < 0) {
            context.transformFlags = 0;
        }
    }
}

// @verified - p
void Author_Callback(void *) {
    if (!controller)
        return;
    if (controller->needsInitAuthorMask) {
        controller->needsInitAuthorMask = 0;
        CleanupAuthCommands();
        controller->currentFrame = static_cast<std::int32_t>(controller->framesRemaining);
    } else {
        const std::int32_t paused = controller->authorStopped | AuthorFramePauseNullsub(controller);
        for (const AuthActorContext &context : controller->actorContexts) {
            if ((context.flags & 2U) == 0)
                continue;
            SetMotionSpeed(FindMotionWork(FindScenarioCharacter(controller->task, context.subject)),
                paused || context.motionRemaining == 0.0f ? 0.0f : context.motionSpeed, MotionRegion::LowerBody);
        }
        if (paused) {
            camCtrlSetSkipInterpTo(1);
            return;
        }
        camCtrlSetSkipInterpTo(0);
        ++controller->currentFrame;
        if (controller->dialogueFrames && --controller->dialogueFrames == 0) {
            std::memset(controller->reserved078, 0, 4);
            controller->dialogueCharacter = nullptr;
        }
        for (AuthActiveCommand &slot : controller->activeCommands) {
            if (slot.arguments && !slot.handler(1, slot.arguments, *slot.context)) {
                slot.handler(2, slot.arguments, *slot.context);
                slot.arguments = nullptr;
            }
        }
        if (author.nextGroup < author.groups.size()) {
            const AuthTimestampGroup &group = author.groups[author.nextGroup];
            if (group.frame <= controller->currentFrame) {
                for (std::size_t index = 0; index < group.commandCount; ++index)
                    DispatchAuthCommand(author.commands[group.firstCommand + index]);
                ++author.nextGroup;
            }
        }
        UpdateAuthorFCVCurves();
        if (controller->framesRemaining <= static_cast<std::uint32_t>(controller->currentFrame))
            CleanupAuthCommands();
    }
    if (authorPlacementMatrix)
        camCtrlSetXform(authorPlacementMatrix);
}

bool ParseAseq(std::span<const std::byte> bytes, std::size_t offset, std::size_t end) {
    if (offset + 16 > end)
        return false;
    std::uint8_t endian{}, version{};
    if (!Read(bytes, offset + 8, endian) || endian != 0 || !Read(bytes, offset + 9, version) || !Read(bytes, offset + 12, controller->framesRemaining) ||
        controller->framesRemaining > 0x7fffffffU) {
        return false;
    }
    author.version = version;
    std::size_t cursor = offset + (version >= 7 ? 32U : 16U);
    if (cursor > end)
        return false;
    std::int32_t previousFrame = -1;
    while (cursor < end) {
        std::int32_t frame{};
        if (!Read(bytes, cursor, frame))
            return false;
        cursor += 4;
        if (frame == -1)
            return cursor == end;
        if (frame < previousFrame || frame > static_cast<std::int32_t>(controller->framesRemaining)) {
            return false;
        }
        previousFrame = frame;
        const std::size_t firstCommand = author.commands.size();
        for (;;) {
            std::uint32_t raw{};
            if (!Read(bytes, cursor, raw))
                return false;
            cursor += 4;
            const std::uint8_t type = static_cast<std::uint8_t>(raw);
            const std::uint8_t argumentCount = static_cast<std::uint8_t>(raw >> 8U);
            if (type == 0) {
                if (raw != 0)
                    return false;
                break;
            }
            if (argumentCount > 64 || cursor > end || static_cast<std::size_t>(argumentCount) * 4 > end - cursor) {
                return false;
            }
            constexpr std::array<std::uint8_t, 7> minimumArguments{
                0, 2, 2, 5, 6, 5, 9};
            if (type >= minimumArguments.size() || argumentCount < minimumArguments[type] || (type == 3 && version >= 6 && argumentCount < 6))
                return false;
            AuthCommand command{};
            command.frame = frame;
            command.type = type;
            command.flags = static_cast<std::uint16_t>(raw >> 16U);
            command.arguments.resize(argumentCount);
            if (argumentCount) {
                std::memcpy(command.arguments.data(), bytes.data() + cursor, static_cast<std::size_t>(argumentCount) * 4);
            }
            if (type == 3 && argumentCount == 5) {

                std::int32_t adjacent{};
                if (!Read(bytes, cursor + 20, adjacent))
                    return false;
                command.arguments.push_back(adjacent);
            }
            cursor += static_cast<std::size_t>(argumentCount) * 4;
            author.commands.push_back(std::move(command));
        }
        author.groups.push_back({frame, firstCommand, author.commands.size() - firstCommand});
    }
    return false;
}

bool ParseSplineTable(std::span<const std::byte> bytes, std::size_t offset, std::size_t size, std::size_t axisCount, std::vector<AuthSpline> &splines) {
    if (size < 16 || offset + size > bytes.size())
        return false;
    std::uint32_t count{};
    if (!Read(bytes, offset + 12, count) || count > 0x10000 || 16 + static_cast<std::size_t>(count) * 4 > size) {
        return false;
    }
    for (std::uint32_t index = 0; index < count; ++index) {
        std::uint32_t relative{}, next = static_cast<std::uint32_t>(size);
        if (!Read(bytes, offset + 16 + index * 4, relative))
            return false;
        if (index + 1 < count && !Read(bytes, offset + 20 + index * 4, next))
            return false;
        if (relative < 16 + count * 4 || relative >= size || next <= relative || next > size) {
            return false;
        }
        AuthSpline spline{};
        if (!ReadSpline(bytes, offset + relative, offset + next, axisCount, axisCount == 8, spline)) {
            return false;
        }
        spline.record = reinterpret_cast<std::uint32_t *>(const_cast<std::byte *>(bytes.data() + offset + relative));
        splines.push_back(std::move(spline));
    }
    return true;
}

bool ParseAuthStrings(std::span<const std::byte> bytes, std::size_t offset, std::size_t size) {
    std::uint32_t count{};
    if (size < 16 || !Read(bytes, offset + 12, count) || count > (size - 16) / 4)
        return false;
    author.strings.reserve(count);
    for (std::uint32_t index = 0; index < count; ++index) {
        std::uint32_t relative{};
        if (!Read(bytes, offset + 16 + index * 4, relative) || relative < 16 + count * 4 || relative >= size)
            return false;
        const char *begin = reinterpret_cast<const char *>(bytes.data() + offset + relative);
        const void *end = std::memchr(begin, 0, size - relative);
        if (!end)
            return false;
        author.strings.emplace_back(begin, static_cast<const char *>(end));
    }
    return true;
}


void RegisterAuthoredDynamicStream(std::span<const std::byte> bytes, std::size_t offset, std::uint32_t tag, std::uint16_t opcode, std::uint16_t recordSize) {
    const AuthDynamicStream stream{tag, opcode, recordSize, bytes.subspan(offset + 8), 0};
    for (AuthDynamicStream &existing : author.dynamicStreams) {
        if (existing.tag == tag) {
            existing = stream;
            return;
        }
    }
    author.dynamicStreams.push_back(stream);
}

bool ParseAuvw(std::span<const std::byte> bytes, std::size_t offset) {
    std::uint32_t tag{}, size{};
    if (!Read(bytes, offset, tag) || tag != Tag('A', 'U', 'V', 'W') || !Read(bytes, offset + 4, size) || size < 8 || size > bytes.size() - offset) {
        return false;
    }
    const std::size_t end = offset + size;
    for (std::size_t cursor = offset + 8; cursor < end;) {
        std::uint32_t childTag{}, childSize{};
        if (!Read(bytes, cursor, childTag) || !Read(bytes, cursor + 4, childSize) || childSize < 8 || childSize > end - cursor) {
            return false;
        }
        if (childTag == Tag('H', 'A', 'D', 'T'))
            author.handPoseTable = const_cast<std::byte *>(bytes.data() + cursor + 8);
        for (std::uint16_t index = 0; index < authDynamicStreamTags.size(); ++index) {
            if (authDynamicStreamTags[index] == childTag) {
                RegisterAuthoredDynamicStream(bytes, cursor, childTag, index, authDynamicStreamStrides[index]);
                break;
            }
        }
        cursor += childSize;
    }
    std::sort(author.dynamicStreams.begin(), author.dynamicStreams.end(),
        [](const AuthDynamicStream &left, const AuthDynamicStream &right) { return left.opcode < right.opcode; });
    return true;
}

bool ParseTrack(std::span<const std::byte> bytes, std::size_t offset) {
    std::uint32_t tag{}, size{};
    if (!Read(bytes, offset, tag) || tag != Tag('T', 'R', 'C', 'K') || !Read(bytes, offset + 4, size) || size < 8 || size > bytes.size() - offset) {
        return false;
    }
    const std::size_t end = offset + size;
    controller->lipTable = nullptr;
    bool foundSequence = false;
    for (std::size_t cursor = offset + 8; cursor + 8 <= end;) {
        std::uint32_t childTag{}, childSize{};
        if (!Read(bytes, cursor, childTag) || !Read(bytes, cursor + 4, childSize) || childSize < 8 || childSize > end - cursor)
            return false;
        if (childTag == Tag('A', 'S', 'E', 'Q')) {
            if (!ParseAseq(bytes, cursor, cursor + childSize))
                return false;
            foundSequence = true;
        } else if (childTag == Tag('A', 'C', 'A', 'M')) {
            if (!ParseSplineTable(bytes, cursor, childSize, 8, author.cameras))
                return false;
        } else if (childTag == Tag('A', 'M', 'O', 'V')) {
            if (!ParseSplineTable(bytes, cursor, childSize, author.version >= 5 ? 9 : 6, author.transforms))
                return false;
        } else if (childTag == Tag('A', 'S', 'T', 'R')) {
            if (!ParseAuthStrings(bytes, cursor, childSize))
                return false;
        } else if (childTag == Tag('A', 'L', 'I', 'P') && !controller->lipTable) {
            controller->lipTable = bytes.data() + cursor;
        }
        cursor += childSize;
    }
    // AUTH_Parser admits ASEQ independently of optional camera, actor and audio streams.
    return foundSequence;
}

bool ParseAuthorAssets(std::span<const std::byte> bytes) {
    bool foundTrack = false;
    for (std::size_t cursor = 0; cursor < bytes.size();) {
        std::uint32_t tag{}, size{};
        if (!Read(bytes, cursor, tag) || !Read(bytes, cursor + 4, size) || size < 8 || size > bytes.size() - cursor) {
            return false;
        }
        if (tag == Tag('T', 'R', 'C', 'K')) {
            if (foundTrack || !ParseTrack(bytes, cursor))
                return false;
            foundTrack = true;
        } else if (tag == Tag('A', 'U', 'V', 'W') && !ParseAuvw(bytes, cursor)) {
            return false;
        }
        cursor += size;
    }
    return foundTrack;
}
bool LoadSleepPackageMotionBank(const LoadedMapPackage &package) {
    author.motions.clear();
    const std::span<const std::byte> bytes = package.pks.Find("0006A.MOTN");
    MotnFile file{};
    if (bytes.empty() || !ReadMotnFile(bytes, file))
        return false;
    author.motions.push_back(std::move(file));
    for (const AuthCommand &command : author.commands) {
        if (command.type == 3 && command.arguments.size() >= 2 && !FindAuthorSequence(static_cast<std::uint32_t>(command.arguments[1]) & 0xffffU)) {
            return false;
        }
    }
    return true;
}

bool AddScenarioActor(shen_name subject) {
    if (!FindScenarioCharacter(controller->task, subject))
        return false;
    author.actors.push_back({.subject = subject});
    return true;
}

AuthorTrackSelection FindMapInfoAuthorTrack(const void *mapInfo, std::int64_t requested) {
    if (!mapInfo || !g_mapInfoSize)
        return {};
    const auto bytes = std::span<const std::byte>(static_cast<const std::byte *>(mapInfo), static_cast<std::size_t>(g_mapInfoSize));
    const MapInfoChunk *const scex = FindMapInfoChunk("SCEX", mapInfo);
    if (!scex || scex->nextByteOffset < 16)
        return {};
    const std::size_t offset = reinterpret_cast<const std::byte *>(scex) - bytes.data();
    if (offset > bytes.size() || scex->nextByteOffset > bytes.size() - offset) {
        return {};
    }
    return FindAuthorTrack(bytes.subspan(offset, scex->nextByteOffset), requested);
}

std::array<AuthorTableEntry, 70> authorTable{};
std::array<AuthorPackageRegistration, 6> authorPackageRecords{};

std::uint32_t ChunkTag(const std::byte *chunk) {
    std::uint32_t tag{};
    std::memcpy(&tag, chunk, sizeof(tag));
    return tag;
}

std::uint32_t ChunkSize(const std::byte *chunk) {
    std::uint32_t size{};
    std::memcpy(&size, chunk + 4, sizeof(size));
    return size;
}

// An unnamed TRCK list fills free slots from 0 up; a named list (eight-byte name prefixes) fills them
// from 69 down and records the slots it took. Each track may be followed by its AUVW view.
void RegisterAuthorTracks(const std::byte *bundle, AuthorPackageRegistration *record) {
    if (!bundle)
        return;
    const std::byte *cursor = bundle + 8;
    const std::byte *const end = bundle + ChunkSize(bundle);
    if (ChunkTag(cursor) == Tag('T', 'R', 'C', 'K')) {
        std::size_t index = 0;
        do {
            AuthorTableEntry &entry = authorTable[index];
            if (!entry.track) {
                entry = {.file = nullptr, .track = cursor, .view = nullptr, .fileOwned = 0, .name = {}};
                cursor += ChunkSize(cursor);
                if (cursor < end && ChunkTag(cursor) == Tag('A', 'U', 'V', 'W')) {
                    entry.view = cursor;
                    cursor += ChunkSize(cursor);
                }
            }
            ++index;
        } while (index < authorTable.size() && cursor < end && ChunkTag(cursor) == Tag('T', 'R', 'C', 'K'));
        return;
    }
    for (std::int32_t index = 69; index >= 0 && cursor < end; --index) {
        AuthorTableEntry &entry = authorTable[static_cast<std::size_t>(index)];
        if (entry.track)
            continue;
        const std::byte *const track = cursor + 8;
        std::memcpy(&entry.name, cursor, sizeof(entry.name));
        if (record)
            record->slots[record->count++] = static_cast<std::uint8_t>(index);
        entry.track = track;
        entry.view = nullptr;
        entry.fileOwned = 0;
        cursor = track + ChunkSize(track);
        if (cursor < end && ChunkTag(cursor) == Tag('A', 'U', 'V', 'W')) {
            entry.view = cursor;
            cursor += ChunkSize(cursor);
        }
    }
}

AuthorTrackSelection AuthorTableTrack(std::int32_t slot) {
    const AuthorTableEntry &entry = authorTable[static_cast<std::size_t>(slot)];
    if (!entry.track)
        return {};
    const std::size_t size = entry.view ? static_cast<std::size_t>(entry.view - entry.track) + ChunkSize(entry.view) : ChunkSize(entry.track);
    return {.bytes = {entry.track, size}, .name = entry.name, .slot = slot};
}

bool InitializeAuthorTrack(const AuthorTrackSelection &track) {
    if (track.bytes.empty() || !ParseAuthorAssets(track.bytes)) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::InvalidTrack, "SCEX/AUTH");
        Author_Free();
        return false;
    }
    author.id = track.name;
    controller->track = static_cast<std::int8_t>(track.slot);
    BeginAuthorRuntime();
    SetMapInfoLoadGate(AuthorMapInfoLoadResult::Started, "SCEX/AUTH");
    return true;
}

} // namespace

// @verified - p
bool IsAuthorMultichannelAudioEnabled() {
    return false;
}

// @verified - p
bool InitializeAuthorMultichannelAudio() {
    authorMultichannel = {};
    AuthorMultichannelDecoderNullsub(nullptr);
    authorMultichannel.allocation = HLib::AllocateTaskData(0x6952c, AUTHOR_MULTICHANNEL_ALLOCATION);
    if (authorMultichannel.allocation) {
        authorMultichannel.player = reinterpret_cast<void *>(static_cast<std::uintptr_t>(IsAuthorMultichannelAudioEnabled()));
        if (authorMultichannel.player)
            return true;
        HLib::FreeTaskData(authorMultichannel.allocation);
        authorMultichannel.allocation = nullptr;
    }
    AuthorMultichannelDecoderNullsub(nullptr);
    AuthorMultichannelDecoderNullsub(nullptr);
    return false;
}

// @verified - p
void ReleaseAuthorMultichannelAudio() {
    if (!authorMultichannel.player)
        return;
    AuthorMultichannelDecoderNullsub(authorMultichannel.player);
    AuthorMultichannelDecoderNullsub(authorMultichannel.player);
    HLib::FreeTaskData(authorMultichannel.allocation);
    authorMultichannel.player = nullptr;
    authorMultichannel.allocation = nullptr;
    authorMultichannel.silentStreamCounter = 0;
    authorMultichannel.state = 0;
    authorMultichannel.countdown = 0;
    authorMultichannel.paused &= 0xffffffff00000000ULL;
    AuthorMultichannelDecoderNullsub(nullptr);
}

std::uint64_t &GetAuthorSilentStreamCounter() {
    return authorMultichannel.silentStreamCounter;
}

// @verified - p
bool IsAuthorMultichannelAudioActive() {
    return authorMultichannel.player != nullptr;
}

// @verified - p
void StartAuthorMultichannelAudio(std::int32_t delay) {
    authorMultichannel.countdown = delay;
    authorMultichannel.state = 1;
}

// @verified - p
void AdvanceAuthorMultichannelAudio() {
    if (authorMultichannelDeviceMode != 1 || authorMultichannel.state != 1)
        return;
    if (!authorMultichannel.countdown) {
        AuthorMultichannelDecoderNullsub(authorMultichannel.player, 0);
        authorMultichannel.state = 0;
        authorMultichannel.paused = 0;
    }
    authorMultichannel.countdown = static_cast<std::int32_t>(static_cast<std::uint32_t>(authorMultichannel.countdown) - 1U);
}

// @verified - p
void PauseAuthorMultichannelAudio(std::uint32_t value) {
    if (authorMultichannel.player && !authorMultichannel.paused)
        AuthorMultichannelDecoderNullsub(authorMultichannel.player, value);
}

// @verified - p
void UpdateAuthorMultichannelDevice() {
    if (authorMultichannelDeviceMode != 1)
        return;
    AuthorMultichannelDecoderNullsub(nullptr);
    (void)IsAuthorMultichannelAudioEnabled();
    AuthorMultichannelDecoderNullsub(nullptr);
}

// @verified - p
std::uint64_t GetAuthorStreamingAudioEnabled() {
    return static_cast<std::uint32_t>(authorStreamingAudioEnabled);
}

// @verified - p
bool SetAuthorStreamingAudioEnabled(std::int32_t enabled) {
    authorStreamingAudioEnabled = enabled != 0;
    return enabled != 0;
}

// @verified - p
void ResetAuthorAudioState() {
    SetSamplePlaybackSuppressed(0);
    authorStreamingAudioEnabled = 1;
}

// @verified - p
void authInit() {
    ResetAuthorAudioState();
    SetConversationCameraCodesEnabled(1);
}

// @verified - p
bool IsAuthorStateFlagSet(std::int32_t index) {
    return authorStateFlags.data()[index] != 0;
}

// @verified - p
bool HasAuthorCancelBeenPressed() {
    return (g_inputBuffer[0].pressedPriority & 0x0200U) != 0;
}

// @verified - p
std::int64_t SetAuthorStateFlag(std::int32_t index, std::int32_t value) {
    authorStateFlags.data()[index] = value;
    return index;
}

// @verified - p
void ResetAuthorStateFlags() {
    for (std::int32_t &flag : authorStateFlags)
        flag = 0;
}

bool Author_HasMapInfoAuthor(const void *mapInfo, shen_name authorId) {
    return !FindMapInfoAuthorTrack(mapInfo, authorId.name).bytes.empty();
}

bool Author_InitializeFromMapInfoAuthor(const void *mapInfo, shen_name authorId) {
    Author_Free();
    if (!controller) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::RuntimeUnavailable, "SCNC");
        return false;
    }
    return InitializeAuthorTrack(FindMapInfoAuthorTrack(mapInfo, authorId.name));
}

void SCEX_Reader(const MapInfoChunk *scex) {
    authorTable = {};
    ResetAuthorAudioState();

    for (AuthorPackageRegistration &record : authorPackageRecords) {
        record.package = nullptr;
        record.count = 0;
    }
    if (scex)
        RegisterAuthorTracks(reinterpret_cast<const std::byte *>(scex), nullptr);
}

std::int32_t ReadAUTH(const std::byte *bundle, const void *package) {
    std::size_t index = 0;
    while (index < authorPackageRecords.size() && authorPackageRecords[index].package)
        ++index;
    if (index == authorPackageRecords.size())
        return 0;
    AuthorPackageRegistration &record = authorPackageRecords[index];
    record.package = package;
    if (!bundle)
        return 1;
    RegisterAuthorTracks(bundle, &record);
    return 1;
}

std::int32_t FreeAUTH(const void *package) {
    if (!package)
        return 0;
    std::size_t index = 0;
    while (index < authorPackageRecords.size() && authorPackageRecords[index].package != package)
        ++index;
    if (index == authorPackageRecords.size())
        return 0;
    AuthorPackageRegistration &record = authorPackageRecords[index];
    for (std::uint8_t slot = 0; slot < record.count; ++slot)
        authorTable[record.slots[slot]] = {};
    record.package = nullptr;
    record.count = 0;
    return 1;
}

std::int32_t probeSceneById(std::int32_t id) {
    for (std::int32_t index = 69; index >= 0; --index) {
        if (authorTable[static_cast<std::size_t>(index)].name.name == static_cast<std::uint32_t>(id))
            return index;
    }
    return -1;
}

bool FindAuthor(shen_name id) {
    return probeSceneById(static_cast<std::int32_t>(id.name)) > -1;
}

void authResetLoadFlag() {
    if (controller)
        controller->loaded = 0;
}

bool Author_InitializeFromMapInfo(const void *mapInfo) {
    Author_Free();
    if (!controller) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::RuntimeUnavailable, "SCNC");
        return false;
    }
    if (!mapInfo || !g_mapInfoSize) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::MapInfoMissing, "MAPINFO");
        return false;
    }
    const auto bytes = std::span<const std::byte>(static_cast<const std::byte *>(mapInfo), static_cast<std::size_t>(g_mapInfoSize));
    const MapInfoChunk *const scex = FindMapInfoChunk("SCEX", mapInfo);
    if (!scex) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::SceneExtensionMissing, "SCEX");
        return false;
    }
    const std::size_t offset = reinterpret_cast<const std::byte *>(scex) - bytes.data();
    if (offset > bytes.size() || scex->nextByteOffset < 16 || scex->nextByteOffset > bytes.size() - offset ||
        !ParseAuthorAssets(bytes.subspan(offset + 8, scex->nextByteOffset - 8))) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::InvalidTrack, "SCEX/AUTH");
        Author_Free();
        return false;
    }
    if (!LoadAuthorMotionBanks()) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::MotionMissing, "MOTN");
        Author_Free();
        return false;
    }
    if (!LoadAuthorActors()) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::ModelMissing, "SCEN/characters");
        Author_Free();
        return false;
    }
    BeginAuthorRuntime();
    SetMapInfoLoadGate(AuthorMapInfoLoadResult::Started, "SCEX/AUTH");
    return true;
}
AuthorMapInfoLoadResult Author_GetLastMapInfoLoadResult() {
    return lastMapInfoLoadResult;
}
const char *Author_GetLastMapInfoLoadGate() {
    return lastMapInfoLoadGate.data();
}
AuthorPackageLoadResult Author_InitializeFromPackage(const char *packageName, shen_name authorId) {
    Author_Free();
    if (!controller) {
        SetPackageLoadGate(AuthorPackageLoadResult::RuntimeUnavailable, packageName, "SCNC", authorId);
        return lastPackageLoadResult;
    }
    const LoadedMapPackage *const package = FindAuthorPackage(packageName);
    if (!package) {
        SetPackageLoadGate(AuthorPackageLoadResult::PackageMissing, packageName, nullptr);
        return lastPackageLoadResult;
    }
    const std::span<const std::byte> bundle = FindPackageEntry(*package, "AUTHOR.AUTH");
    if (bundle.empty()) {
        SetPackageLoadGate(AuthorPackageLoadResult::AuthorBundleMissing, packageName, "AUTHOR.AUTH");
        return lastPackageLoadResult;
    }
    std::uint32_t signature{}, bundleSize{};
    if (!Read(bundle, 0, signature) || signature != Tag('A', 'U', 'T', 'H') || !Read(bundle, 4, bundleSize) || bundleSize != bundle.size()) {
        SetPackageLoadGate(AuthorPackageLoadResult::InvalidAsset, packageName, "AUTHOR.AUTH");
        return lastPackageLoadResult;
    }
    const AuthorTrackSelection track = FindAuthorTrack(bundle, authorId.name);
    if (track.bytes.empty()) {
        SetPackageLoadGate(AuthorPackageLoadResult::AuthorIdMissing, packageName, "AUTHOR.AUTH", authorId);
        return lastPackageLoadResult;
    }
    if (!ParseAuthorAssets(track.bytes)) {
        SetPackageLoadGate(AuthorPackageLoadResult::InvalidAsset, packageName, "AUTHOR.AUTH", authorId);
        Author_Free();
        return lastPackageLoadResult;
    }
    if (!LoadSleepPackageMotionBank(*package)) {
        SetPackageLoadGate(AuthorPackageLoadResult::MotionMissing, packageName, "0006A.MOTN", authorId);
        Author_Free();
        return lastPackageLoadResult;
    }
    if (!LoadAuthorActors()) {
        SetPackageLoadGate(AuthorPackageLoadResult::ModelMissing, packageName, "SCEN/characters", authorId);
        Author_Free();
        return lastPackageLoadResult;
    }
    author.id = track.name;
    controller->track = static_cast<std::int8_t>(track.slot);
    BeginAuthorRuntime();
    SetPackageLoadGate(AuthorPackageLoadResult::Started, packageName, "AUTHOR.AUTH", authorId);
    return lastPackageLoadResult;
}

AuthorPackageLoadResult Author_GetLastPackageLoadResult() {
    return lastPackageLoadResult;
}

const char *Author_GetLastPackageLoadGate() {
    return lastPackageLoadGate.data();
}

void Author_BindController(AuthorController *value) {
    if (controller == value)
        return;
    Author_Free();


    if (value)
        authorMaskTask = nullptr;
    controller = value;
    authorSkipAllowed = true;
    if (value)
        authorHighQualityHands = 1;
    if (value)
        authorActive = 0;
    if (value)
        authorUnusedFlag = 0;
    authorCannotSkip = false;
    authorForceFilmBars = true;
    authorApplyYawOffset = false;
}

const AuthorController *Author_GetController() {
    return controller;
}

const Matrix4x4 *Author_GetPlacementMatrix() {
    return authorPlacementMatrix;
}

// @verified - p
void Author_SetPlacementMatrix(const Matrix4x4 *matrix) {
    authorPlacementMatrix = matrix;
}

void Author_Update() {
    if (!controller)
        return;
    if (controller->loaded == 1) {
        // ProcessScenario delays a skip for two frame callbacks.
        if (controller->field06c == 0 && controller->skipToNext != 0) {
            if (--controller->skipToNext == 0) {
                controller->flag2 = 0;
                controller->needsInitAuthorMask = 1;
                controller->field06c = 1;
            }
        } else if (controller->sequenceCount <= 1) {
            Author_Callback(nullptr);
        } else {
            const std::int8_t track = controller->track;
            std::int32_t tick = 0;
            while (tick < static_cast<std::int32_t>(25U * controller->sequenceCount - 25U)) {
                if (controller->track != track)
                    break;
                Author_Callback(nullptr);
                ++tick;
            }
        }
    }
    if (controller->flag2 && controller->track >= 0) {
        const std::int32_t stopped = controller->authorStopped;
        controller->flag2 = 0;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(controller->track));
        controller->authorStopped = stopped;
        controller->sequenceCount = 1;
    }
}

void Author_UpdateDynamic() {
    if (controller && controller->loaded == 1 && !author.cleaned) {
        ApplyAuthoredDynamicStreams();
    }
}

void Author_RequestAbort() {
    if (!authGetLoaded() || controller->finished)
        return;
    controller->finished = 1;
    controller->skipToNext = 2;
    // authStopCurrent owns this global CVS cutover.
    scenCvsCtrlFree(nullptr);
    scenDestroySubtitles(controller->task);
    StopAuthoredFilm();
    snapStopControl();
}

void Author_Free() {
    authDestroySubtitleConverseData();
    CleanupAuthCommands();
    FreeAuthoredSubcontrollers();
    if (author.motionBanksPublished)
        RegisterAuthorMotionBanks({});
    DestroyElementByIndex(UIElement::CutsceneSkip);
    if (controller) {
        controller->loaded = 0;
        controller->flag2 = 0;
        controller->needsInitAuthorMask = 0;
        controller->dialogueFrames = 0;
        controller->dialogueCharacter = nullptr;
        controller->audioCountdown = {};
        controller->currentFrame = 0;
        controller->framesRemaining = 0;
        controller->authorStopped = 0;
        controller->skipTimer = 0.0f;
        controller->currentInput = 0;
        controller->skipToNext = 0;
        controller->field06c = 0;
        controller->finished = 0;
        controller->activeCommands = {};
        controller->actorContexts = {};
    }
    author = {};
}

shen_name GetCurrentAuthorId() {
    return controller && controller->loaded && static_cast<std::uint8_t>(controller->track + 100) > 0x63U ?
        authorTable[static_cast<std::size_t>(controller->track)].name :
        shen_name{};
}

// @verified - p
std::uint64_t GetAuthorFlags() {
    return controller->field06c;
}

// @verified - p
void EV_SetAuthorActiveTo(std::int32_t active) {
    authorActive = active;
}

bool Author_IsSkipActive() {
    return authorSkipAllowed || authorActive != 0;
}

std::int64_t EV_authSceneCtrl(AuthSceneCtrl command) {
    switch (command) {
    case AuthSceneCtrl::GetCurrentAuthor:
        return GetCurrentAuthorId().name;
    case AuthSceneCtrl::ApplyYangleOffset:
        authorApplyYawOffset = true;
        return 1;
    case AuthSceneCtrl::UnusedAuthorFlag:
        authorUnusedFlag = 1;
        return 1;
    case AuthSceneCtrl::SetCharaHQFlags:
        authorHighQualityHands = 0;
        return 1;
    case AuthSceneCtrl::GetCantSkipAuthor:
        return authorCannotSkip;
    case AuthSceneCtrl::GetSkipAuthorEnabled:
        return authorSkipAllowed;
    case AuthSceneCtrl::DisallowSkipAuthor:
        authorSkipAllowed = false;
        return 1;
    case AuthSceneCtrl::AllowSkipAuthor:
        authorSkipAllowed = true;
        return 1;
    case AuthSceneCtrl::GetFilmBarsActive:
        return authorForceFilmBars;
    case AuthSceneCtrl::HideFilmBars:
        authorForceFilmBars = false;
        return 0;
    case AuthSceneCtrl::ShowFilmBars:
        authorForceFilmBars = true;
        return 1;
    case AuthSceneCtrl::GetCurrentFrame:
        return authGetLoaded() ? static_cast<std::uint32_t>(controller->currentFrame) : std::uint32_t{0xffffffffU};
    case AuthSceneCtrl::GetFramesRemaining:
        return authGetLoaded() ? controller->framesRemaining : std::uint32_t{0xffffffffU};
    case AuthSceneCtrl::RefreshAndEnqueue:
        authDestroySubtitleConverseData();
        CleanupAuthCommands();
        return 1;
    case AuthSceneCtrl::ResetAuthorStopped:
    case AuthSceneCtrl::SetAuthorStopped:
        if (!controller)
            return 0;
        controller->authorStopped = command == AuthSceneCtrl::SetAuthorStopped ? 1 : 0;
        return 1;
    case AuthSceneCtrl::IsLoaded:
        return controller ? controller->loaded : 0;
    default:
        break;
    }
    CleanupAuthCommands();
    const std::int64_t requested = static_cast<std::int64_t>(command);
    if (requested < 0 || requested > 0xffffffffLL || !controller)
        return 0;
    const std::int32_t slot = requested <= 70 ? static_cast<std::int32_t>(requested) : probeSceneById(static_cast<std::int32_t>(requested));
    if (slot < 0 || slot >= 70) {
        SetMapInfoLoadGate(AuthorMapInfoLoadResult::InvalidTrack, "registered AUTH");
        return 0;
    }
    Author_Free();
    controller->postUpdateContext = static_cast<std::uint64_t>(requested);
    if (!InitializeAuthorTrack(AuthorTableTrack(slot))) {
        return 0;
    }
    return 1;
}

std::int64_t authGetFinished() {
    return authGetLoaded() ? controller->finished : 0;
}

int Author_GetCurrentFrame() {
    return controller && controller->loaded ? controller->currentFrame : -1;
}

std::int32_t authGetLoaded() {
    return controller ? controller->loaded : 0;
}

bool Author_GetActorTransform(shen_name subject, Vector3 &position, Vector3 &rotation) {
    const AuthActor *const actor = FindActor(subject.name);
    if (!actor)
        return false;
    position = actor->position;
    rotation = actor->rotation;
    return true;
}

bool Author_SetActorPosition(shen_name subject, const Vector3 &position) {
    AuthActor *const actor = FindActor(subject.name);
    if (!actor)
        return false;
    actor->position = position;
    return true;
}

} // namespace shenmue::s2
