#pragma once

#include <string>
#include <functional>
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "ECS.h"

class GraphicsManager;
class InputManager;
class ECS;

typedef std::function<void()> UpdateCallback;

class Engine {
public:
	GraphicsManager graphics;
	InputManager input;
	ResourceManager resources;
	ECS ecs;

	Engine();
	bool start();
	void shutdown();
	void gameLoop(const UpdateCallback& callback);
};

struct GraphicsConfig {
	int width;
	int height;
	std::string name;
	bool fs;
};

//Globally-accessible engine which communicates with all components
inline Engine globalEngine;