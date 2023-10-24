#pragma once

#include <string>
#include <functional>
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "ECS.h"
#include "ScriptManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"

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
	ScriptManager scripts;
	PhysicsManager physics;
	SoundManager sound;

	Engine();
	bool start(std::string scriptPath);
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