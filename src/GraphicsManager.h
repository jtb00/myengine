#pragma once

//#include "Types.h"
#include <string>

class Engine;

class GraphicsManager {
public:
	int windowWidth;
	int windowHeight;
	std::string windowName;
	bool fullscreen;
	Engine* engine;

	GraphicsManager(void);
	GraphicsManager(int width, int height, std::string name, bool fs);
	bool start();
	void shutdown();
	void draw();
};