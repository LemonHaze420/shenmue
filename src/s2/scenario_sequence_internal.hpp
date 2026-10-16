#pragma once
#include "s2/scenario_sequence.hpp"
#include "s2/camera.hpp"
#include <vector>

namespace shenmue::s2::event_runtime_detail {


struct ScenarioSequenceRuntime {
    std::vector<std::byte> retainedBytes;
    std::vector<ScenarioSequenceRecord> records;
    std::vector<std::uint8_t> unsupportedModes;
    HLib::Task *task{};
    CameraSequenceLease cameraLease{};
    ScenarioSequenceStatus status{ScenarioSequenceStatus::Empty};
    std::size_t recordIndex{};
    std::uint32_t result{};
    std::uint32_t motionRegion{};
    shen_name motionCharacter{};
    ConversationBuffer *voiceBuffer{};
    std::array<char, 13> voiceToken{};
    bool cameraCommandStarted{};
    bool dialogueCommandStarted{};
    bool subtitleOwned{};
    bool voiceRequested{};
    bool motionCommandStarted{};
    bool uiOwned{};
    bool fadeCommandStarted{};
};
} // namespace shenmue::s2::event_runtime_detail
