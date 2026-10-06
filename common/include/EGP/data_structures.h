#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "event.hpp"

namespace egp {

enum class EndingMethod { END_WITH_TIME = 0, END_WITH_EVENT = 1 };

struct Character {
  struct Halo {
    float halo_size = 1.0f;
    float halo_transparency = 1.0f;
    std::optional<uint64_t> halo_flicker_frequency = 0;
  };
  std::optional<std::string> name;
  float character_size = 1.0f;
  float character_transparency = 1.0f;
  Halo halo;
};

struct Round {
  std::vector<event::Event> event_log;
  int ending_method = static_cast<int>(EndingMethod::END_WITH_EVENT);
};

inline std::vector<Round> round_pool;
inline std::vector<Character> characters;

enum class RequestType : std::int8_t {
  NAME = 0,
  CHARACTER_SIZE = 1,
  CHARACTER_TRANSPARENCY = 2,
  HALO_SIZE = 3,
  HALO_TRANSPARENCY = 4,
  HALO_FLICKER_FREQUENCY = 5
};

using CharacterProperty = std::variant<std::string, float, int>;

inline void AddCharacter(const std::string& name, float character_size,
                         float character_transparency, float halo_size,
                         float halo_transparency,
                         std::int8_t halo_flicker_frequency) {
  characters.push_back(Character{
      .name = name,
      .character_size = character_size,
      .character_transparency = character_transparency,
      .halo = {.halo_size = halo_size,
               .halo_transparency = halo_transparency,
               .halo_flicker_frequency = halo_flicker_frequency},
  });
}
inline void EditCharacter(std::int8_t character_index, std::int8_t witch,
                          CharacterProperty a) {
  if (character_index >= 0 &&
      character_index < static_cast<std::int8_t>(characters.size())) {
    Character& character = characters[character_index];
    switch (static_cast<RequestType>(witch)) {
      using enum RequestType;
      case NAME:
        if (const auto* value = std::get_if<std::string>(&a)) {
          character.name = *value;
        }
        break;
      case CHARACTER_SIZE:
        character.character_size = std::get<float>(a);
        break;
      case CHARACTER_TRANSPARENCY:
        character.character_transparency = std::get<float>(a);
        break;
      case HALO_SIZE:
        character.halo.halo_size = std::get<float>(a);
        break;
      case HALO_TRANSPARENCY:
        character.halo.halo_transparency = std::get<float>(a);
        break;
      case HALO_FLICKER_FREQUENCY:
        character.halo.halo_flicker_frequency =
            static_cast<std::int8_t>(std::get<int>(a));
        break;
    }
  }
}
inline void AddRound(const std::vector<event::Event>& event_log,
                     EndingMethod ending_method) {
  Round new_round;
  new_round.event_log = event_log;
  new_round.ending_method = static_cast<int>(ending_method);

  round_pool.push_back(new_round);
}
}  // namespace egp