#pragma once

#include <glaze/glaze.hpp>
#include <string>

namespace EGP {
struct Settings {
  int windowWidth = 800;
  int windowHeight = 600;

  struct Theme {
    std::string primary = "#2196F3";
    std::string secondary = "#797979";
    std::string tertiary = "#888888";
    std::string background = "#572525";
    std::string text = "#333333";
    std::string surface = "#ffffff";
    std::string surfacePressed = "#f0f0f0";
    std::string border = "#a0a0a0";
    std::string sliderBorder = "#bdbdbd";
    std::string track = "#ffffff";
    std::string dark = "#2a2a2a";
    std::string danger = "#ff0000";
    std::string sidebar = "#a9a9a9";
    std::string statusBar = "#d3d3d3";
    std::string controlPressed = "#d0d0d0";
  } theme;
};
}  // namespace EGP