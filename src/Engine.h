#pragma once

//#include "Types.h"
#include <string>
#include "GraphicsManager.h"

class GraphicsManager;

class Engine {
public:
	GraphicsManager graphics;

	Engine();
	void start();
	void shutdown();
	void gameLoop();
};

struct GraphicsConfig {
	int width;
	int height;
	std::string name;
	bool fs;
};

inline Engine globalEngine;