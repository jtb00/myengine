#pragma once

#include <string>
#include "glm/glm.hpp"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;

struct Position { double x, y; };
struct Velocity { double x, y; };
struct Gravity { double meters_per_second; };
struct Sprite { std::string name; int xPos; int yPos; float scale; float z; };
struct Health { double percent; };
struct Script { std::string name; };