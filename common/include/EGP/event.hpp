#pragma once
#include <optional>
#include <string>
#include <variant>

namespace egp::event {

struct MoveEvent {
  std::optional<uint64_t> character_index = std::nullopt;
  float x = 0.0f;
  float y = 0.0f;
};

struct SpeakEvent {
  std::optional<uint64_t> character_index = std::nullopt;
  std::string content;
};

struct ImageChangeEvent {
  std::optional<uint64_t> character_index = std::nullopt;
  std::string image_id;
};

struct SetCharacterHoleEvent {
  std::optional<uint64_t> character_index = std::nullopt;
  float hole_size = 1.0f;
  float halo_transparency = 1.0f;
  std::optional<uint64_t> halo_flicker_frequency = 0;
};

struct SetCharacterEvent {
  std::optional<uint64_t> character_index = std::nullopt;
  float character_size = 1.0f;
  float character_transparency = 1.0f;
};

using Event = std::variant<MoveEvent, SpeakEvent, ImageChangeEvent,
                           SetCharacterHoleEvent, SetCharacterEvent>;
}  // namespace egp::event