#include <iostream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

//#include "Types.h"
//include "Engine.h"
#include "GraphicsManager.h"

using namespace illengine;

illengine::GraphicsManager::GraphicsManager(int width, int height, string name, bool fs) {
	windowWidth = width;
	windowHeight = height;
	windowName = name;
	fullscreen = fs;
	Engine = globalEngine;
}

bool illengine::GraphicsManager::start() {
	glfwInit();
	// We don't want GLFW to set up a graphics API.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// Create the window.
	GLFWwindow* window = glfwCreateWindow( windowWidth, windowHeight, windowName, fullscreen ? glfwGetPrimaryMonitor() : 0, 0 );
	glfwSetWindowAspectRatio( window, windowWidth, windowHeight );
	if( !window ) {
		std::cerr << "Failed to create a window." << std::endl;
		glfwTerminate();
	}
}

void illengine::GraphicsManager::shutdown() {
	
}

void illengine::GraphicsManager::draw() {
	
}