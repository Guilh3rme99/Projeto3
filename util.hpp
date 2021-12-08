#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <vector>

#include "abcg.hpp"
struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  bool operator==(const Vertex& other) const noexcept {
    static const auto epsilon{std::numeric_limits<float>::epsilon()};
    return glm::all(glm::epsilonEqual(position, other.position, epsilon)) &&
           glm::all(glm::epsilonEqual(normal, other.normal, epsilon)) &&
           glm::all(glm::epsilonEqual(texCoord, other.texCoord, epsilon));
  }
};

#endif