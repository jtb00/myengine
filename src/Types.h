#pragma once

#include <string>
#include <chrono>
#include <unordered_map>
#include "glm/glm.hpp"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;

struct Physics { vec2 p, v, a; double g; std::chrono::steady_clock::time_point lastUpdate; };
struct Sprite { std::string name; float xPos; float yPos; float scale; float z; bool hurt;};
struct Health { double percent;};
struct BoundingBox { int x, y, width, height; };
struct Score { int value; };