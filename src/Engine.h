#pragma once

#include "Types.h"

class Engine {
public:
	illengine::GraphicsManager graphics;

	Engine();
	void start();
	void shutdown();
	void gameLoop();
};

struct GraphicsConfig {
	int width;
	int height;
	illengine::string name;
	bool fs;
};

inline Engine globalEngine;