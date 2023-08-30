#pragma once

#include "Types.h"

class GraphicsManager {
public:
	int windowWidth;
	int windowHeight;
	illengine::string windowName;
	bool fullscreen;
	illengine::Engine engine;

	GraphicsManager(int width, int height, illengine::string name, bool fs) {
		windowWidth = width;
		windowHeight = height;
		windowName = name;
		fullscreen = fs;
		engine = globalEngine;
	}
	bool start();
	void shutdown();
	void draw();
};