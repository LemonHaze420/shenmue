#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0527_EVENT{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0527_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0527_SIN{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0527_CHOICE_PROP{SHEN_NAME(a016)};
inline constexpr shen_name AREA_0527_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01), SHEN_NAME(AT02), SHEN_NAME(AT03), SHEN_NAME(AT04), SHEN_NAME(AT05),
    SHEN_NAME(AT06), SHEN_NAME(AT07), SHEN_NAME(AT08), SHEN_NAME(AT09), SHEN_NAME(AT0A), SHEN_NAME(AT0B), SHEN_NAME(AT0C), SHEN_NAME(AT0D), SHEN_NAME(AT0E),
    SHEN_NAME(AT0F), SHEN_NAME(AT0G), SHEN_NAME(AT0H), SHEN_NAME(AT0I), SHEN_NAME(AT0J), SHEN_NAME(AT0K), SHEN_NAME(AT0L), SHEN_NAME(AT0M), SHEN_NAME(AT0N),
    SHEN_NAME(AT0O), SHEN_NAME(AT0P), SHEN_NAME(AT0Q), SHEN_NAME(AT0R), SHEN_NAME(AT0S), SHEN_NAME(AT0T), SHEN_NAME(AT0U), SHEN_NAME(AT0V), SHEN_NAME(AT0W)};
inline constexpr shen_name AREA_0527_PROPS[]{SHEN_NAME(a778), SHEN_NAME(a784), SHEN_NAME(a786), SHEN_NAME(a788), SHEN_NAME(a790), SHEN_NAME(a792),
    SHEN_NAME(a794), SHEN_NAME(a738), SHEN_NAME(a740), SHEN_NAME(a742), SHEN_NAME(a744), SHEN_NAME(a746), SHEN_NAME(a748), SHEN_NAME(a750), SHEN_NAME(a752),
    SHEN_NAME(a754), SHEN_NAME(a756), SHEN_NAME(a758), SHEN_NAME(a760), SHEN_NAME(a762), SHEN_NAME(a764), SHEN_NAME(a766), SHEN_NAME(a768), SHEN_NAME(a770),
    SHEN_NAME(a772), SHEN_NAME(a774), SHEN_NAME(a776), SHEN_NAME(a780), SHEN_NAME(a782), SHEN_NAME(a736), shen_name{0xffffffffU}};
struct Area0527Request {
    std::int32_t author;
    Vector3 position;
    std::uint32_t yaw;
};
struct Area0527Parameter {
    HLib::Task *self;
    std::byte reserved08[12];
    std::int32_t author;
    std::int16_t state;
    std::byte reserved1a[6];
    Area0527Request *request;
    std::int32_t choiceCompleted;
    std::byte reserved2c[4];
};
struct Area0527AuthorParameter {
    Area0527Parameter *parent;
    std::int16_t frame;
    std::int16_t choiceFrame;
    std::byte reserved0c[4];
};
static_assert(sizeof(Area0527Request) == 20 && sizeof(Area0527Parameter) == 48 && sizeof(Area0527AuthorParameter) == 16);
static_assert(offsetof(Area0527Parameter, author) == 20 && offsetof(Area0527Parameter, request) == 32);
static_assert(offsetof(Area0527Parameter, state) == 24 && offsetof(Area0527Parameter, choiceCompleted) == 40);
static_assert(offsetof(Area0527AuthorParameter, frame) == 8 && offsetof(Area0527AuthorParameter, choiceFrame) == 10);
void Initialize0527();
void Start0527(std::uintptr_t argument);
void Update0527(void *data);
void Queue0527Author(Area0527Parameter *parent);
void Update0527Author(void *data);
void Wait0527Choice(void *data);
void Finish0527Choice(void *data);
void Prepare0527Character(shen_name character);
void Prepare0527Characters();
void Restore0527Characters();
void Restore0527Motions(Area0527AuthorParameter *child);
void Cleanup0527(Area0527Parameter *parent);
extern const ScriptAreaDefinition area0527ScriptDefinition;
} // namespace shenmue::s2::areas
