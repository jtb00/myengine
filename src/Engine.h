#pragma once

#include <string>
#include <functional>
#include "GraphicsManager.h"
#include "InputManager.h"

class GraphicsManager;
class InputManager;

typedef std::function<void()> UpdateCallback;

class Engine {
public:
	GraphicsManager graphics;
	InputManager input;

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