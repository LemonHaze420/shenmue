#pragma once
#include "common/types.hpp"
#include <cstdint>

namespace shenmue::s2::areas {
extern std::int32_t bookAuthoredScene;
extern shen_name bookAuthoredActors[10];
void PrepareBookAuthoredActor(shen_name actor);
void QueueBookAuthoredScene(std::int32_t scene);
void BookAuthoredPreCallback(void *);
void BookAuthoredCallback(void *);
bool CompleteBookAuthoredScene();
} // namespace shenmue::s2::areas
