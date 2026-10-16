#pragma once

#include <cstddef>
#include <span>

namespace shenmue::s2 {
namespace HLib {
struct Task;
}



bool ApplyAuthoredEyesRecord(std::span<const std::byte> record, HLib::Task *parent);
bool ApplyAuthoredPartsRecord(std::span<const std::byte> record, HLib::Task *parent);
void FreeAuthoredSubcontrollers();

} // namespace shenmue::s2
