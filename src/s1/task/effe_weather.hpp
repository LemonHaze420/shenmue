#pragma once

#include "s1/task/effe.hpp"
#include "s1/task/char.hpp"
#include "s1/render_passes.hpp"

#include <array>
#include <cstring>

namespace shenmue::s1 {

struct WeatherNodeData;
struct WeatherDataEntry;
struct WeatherWindData;

#pragma pack(push, 4)
struct WeatherParticlePool {
    void *records;
    std::int32_t capacity;
    std::int32_t activeCount;
    std::int32_t renderCount;
    std::uint32_t reserved14;
    WeatherDataEntry *fade;
    std::int32_t availableCount;
};

struct WeatherRainGroup {
    std::uint32_t flags;
    std::uint32_t reserved04;
    WeatherParticlePool pool;
    float fallHeight;
    Vector3 frameVelocity;
    float damping;
    EffeMaterialWords material;
    std::uint32_t textureHandle;
    std::uint32_t colour;
    float width;
    float height;
    std::int32_t angleX;
    std::int32_t angleY;
    std::int32_t angleZ;
    Vector3 velocity;
    Vector3 velocityRange;
    Vector3 smoothedVelocity;
};

union WeatherTextureReference {
    void *texture;
    std::uint32_t words[2];
};

struct WeatherSnowGroup {
    std::uint32_t flags;
    WeatherParticlePool pool;
    float fallHeight;
    Vector3 cameraPush;
    float damping;
    EffeMaterialWords material;
    std::uint32_t textureHandle;
    std::uint32_t colour;
    float size;
    Vector3 velocity;
    Vector3 velocityRange;
    WeatherTextureReference textureReference;
};

struct WeatherRippleGroup {
    std::uint32_t flags;
    std::uint32_t reserved04;
    WeatherParticlePool pool;
    EffeMaterialWords material;
    std::uint32_t textureHandle;
    std::uint32_t colour;
    float size;
    float cameraEyeY;
    std::uint32_t *colourRamp;
    CharacterIdTransform collisionProbe;
    std::uint32_t reserved7c;
    WeatherTextureReference textureReference;
};

struct WeatherVolumeBounds {
    float maximumY;
    float minimumY;
    float maximumX;
    float minimumX;
    float maximumZ;
    float minimumZ;
    float dimensionX;
    float dimensionY;
    float dimensionZ;
};

struct WeatherSnowCenters {
    Vector3 current;
    Vector3 reflection;
    Vector3 previous;
};

struct WeatherNodeData {
    WeatherNodeData *next;
    WeatherNodeData **backlink;
    WeatherRainGroup rain;
    WeatherSnowGroup snow;
    WeatherRippleGroup ripple;
    float volume;
    union {
        WeatherVolumeBounds bounds;
        WeatherSnowCenters snowCenters;
    };
};

struct WeatherClipBounds {
    float maximumY;
    float minimumY;
    float cornerX[4];
    float cornerZ[4];
    Vector2 edgeNormal[4];
};

// The first volume clip maximum overlaps the authored volume's Z extent.
struct WeatherClippedVolumeBounds {
    std::byte reserved000[0x1bc];
    WeatherClipBounds clips[2];
    std::byte reserved24c[0x14];
};

union WeatherClippedVolume {
    WeatherNodeData node;
    WeatherClippedVolumeBounds bounds;
};

struct WeatherSnowSphere {
    WeatherNodeData node;
    float radius;
    float radiusSquared;
    float depth;
    float previousRadius;
    float previousRadiusSquared;
    float density;
    std::int32_t burstCount;
};

struct WeatherClippedSnowSphere {
    WeatherSnowSphere sphere;
    WeatherClipBounds clips[2];
    Vector3 minimumVelocity;
    std::byte reserved278[8];
};
#pragma pack(pop)

static_assert(sizeof(WeatherParticlePool) == 0x24);
static_assert(sizeof(WeatherRainGroup) == 0x8c);
static_assert(sizeof(WeatherSnowGroup) == 0x74);
static_assert(sizeof(WeatherRippleGroup) == 0x88);
static_assert(offsetof(WeatherNodeData, rain) == 0x10);
static_assert(offsetof(WeatherNodeData, snow) == 0x9c);
static_assert(offsetof(WeatherNodeData, ripple) == 0x110);
static_assert(offsetof(WeatherNodeData, volume) == 0x198);
static_assert(offsetof(WeatherNodeData, bounds) == 0x19c);
static_assert(sizeof(WeatherNodeData) == 0x1c0);
static_assert(sizeof(WeatherClipBounds) == 0x48);
static_assert(offsetof(WeatherClippedVolumeBounds, clips) == 0x1bc);
static_assert(sizeof(WeatherClippedVolume) == 0x260);
static_assert(offsetof(WeatherSnowSphere, radius) == 0x1c0);
static_assert(sizeof(WeatherSnowSphere) == 0x1dc);
static_assert(offsetof(WeatherClippedSnowSphere, clips) == 0x1dc);
static_assert(offsetof(WeatherClippedSnowSphere, minimumVelocity) == 0x26c);
static_assert(sizeof(WeatherClippedSnowSphere) == 0x280);

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

struct WeatherWindLink {
    WeatherWindLink *previous;
    WeatherWindLink *next;
    std::int32_t type;
    std::uint32_t reserved14;
    WeatherWindData *wind;
};

struct WeatherFixc {
    std::int32_t type;
    std::uint32_t reserved04;
    void *data;
    WeatherWindLink *windLinks;
};

struct WeatherCaraData {
    WeatherNodeData *rainPrimary;
    WeatherNodeData *rainSecondary;
    WeatherNodeData *snowPrimary;
    WeatherNodeData *snowSecondary;
    WeatherNodeData *windSystem;
    WeatherNodeData *windSecondary;
    std::int32_t fieldOfView;
    std::byte reserved034[0x0c];
};

struct RainParticleNative;

using RainRandomFunction = void (*)(RainParticleNative *, WeatherNodeData *);

struct RainParticleNative {
    std::uint32_t flags;
    std::uint32_t reserved04;
    RainRandomFunction random;
    Vector3 position;
    Vector3 velocity;
};

static_assert(sizeof(RainParticleNative) == 0x28);
static_assert(offsetof(RainParticleNative, position) == 0x10);
static_assert(offsetof(RainParticleNative, velocity) == 0x1c);


struct WeatherDrawGroup {
    WeatherNodeData *owner;
    std::byte *records;
    std::int32_t count;
    std::int32_t stride;
    std::uint32_t flags;
    float width;
    float height;
    float scaledWidth;
    float scaledHeight;
};

// 0x38-byte ripple (hamon) record walked by ConstructAndCommitVertsForDrawing_2.
struct RippleParticleNative {
    std::uint32_t flags;
    std::uint32_t reserved04;
    RainRandomFunction random;
    Vector3 position;
    std::int32_t angleX;
    std::int32_t angleY;
    std::int32_t angleZ;
    const std::uint32_t *colour;
    float scale;
    float maximumScale;
};

static_assert(sizeof(RippleParticleNative) == 0x38);

static_assert(offsetof(RippleParticleNative, angleX) == 0x1c);

static_assert(offsetof(RippleParticleNative, colour) == 0x28);


struct WeatherSpriteTexture {
    std::uint32_t handle;
    std::uint32_t width;
    std::uint32_t height;
};

struct ActiveWeatherNode {
    ActiveWeatherNode *previous;
    ActiveWeatherNode *next;
    std::int32_t type;
    std::uint32_t reserved14;
    WeatherFixc *weather;
};

static_assert(sizeof(WeatherCaraData) == 0x40);

static_assert(offsetof(WeatherCaraData, fieldOfView) == 0x30);

static_assert(sizeof(WeatherFixc) == 0x18);

static_assert(sizeof(WeatherWindLink) == 0x20);

static_assert(sizeof(ActiveWeatherNode) == 0x20);

// EFFE_Init publishes these once; nothing rewrites them.
inline constexpr float kWeatherSnowNearCut = 0.3f;

inline constexpr float kWeatherRippleDensityCap = 4.0f;


inline constexpr std::size_t kGlobalWeatherBucketBytes = 0x260;

inline constexpr std::size_t kRainWeatherBucketBytes = 0x1c0;

inline constexpr std::array<std::int32_t, 2> kWeatherSnowSecondaryCap{10, 200};

inline constexpr std::array<float, 2> kWeatherSnowBudgetCost{1.698113203048706f, 0.5299999713897705f};

inline constexpr std::array<float, 2> kWeatherSnowBudgetCapacity{0.5888888835906982f, 1.8867924213409424f};

} // namespace shenmue::s1::effe_detail
