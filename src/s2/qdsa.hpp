#pragma once
#include "s2/hlib.hpp"
#include <cstdint>
namespace shenmue::s2 {
void BeginQdsaAuthor(HLib::Task *parent, std::int32_t selector);
std::int32_t UpdateQdsaAuthor();
} // namespace shenmue::s2
