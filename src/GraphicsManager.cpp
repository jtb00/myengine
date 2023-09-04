#include <iostream>

#include "spdlog/spdlog.h"

#include "Engine.h"
#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(int width, int height, std::string name, bool fs) {
	windowWidth = width;
	windowHeight = height;
	windowName = name;
	fullscreen = fs;
	engine = &globalEngine;
}

//Creates window
bool GraphicsManager::start() {
	glfwInit();
	// We don't want GLFW to set up a graphics API.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// Create the window.
	window = glfwCreateWindow( windowWidth, windowHeight, windowName.c_str(), fullscreen ? glfwGetPrimaryMonitor() : 0, 0 );
	glfwSetWindowAspectRatio( window, windowWidth, windowHeight );
	if( !window ) {
		spdlog::error("Failed to create a window.");
		glfwTerminate();
		return false;
	}
	return true;
}

void GraphicsManager::shutdown() {
	
}

void GraphicsManager::draw() {
	
}