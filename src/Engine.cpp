#include <iostream>
#include <thread>
#include <chrono>

#include "Engine.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

//Creates and initializes Engine components
Engine::Engine(): graphics(), input(), resources(), ecs(), scripts(), physics() {}

//Signals Engine components to start, returns false if a component fails
bool Engine::start() {
	if(!graphics.start()) {
		return false;
	}
	//graphics.loadImg("square", "C:/Users/gamec/Projects/CS 425/myengine/assets/Untitled.png");
	if(!input.start()) {
		return false;
	}
	if (!resources.start()) {
		return false;
	}
	if (!scripts.start()) {
		return false;
	}
	return true;
}

//Signals Engine components to shutdown
void Engine::shutdown() {
	graphics.shutdown();
	input.shutdown();
}

//Main game loop
void Engine::gameLoop(const UpdateCallback& callback) {
	//1/60th of a second
	const auto step = std::chrono::duration<double>(1.0/60.0);
	//Get input, execute callback function, draw graphics, repeat every 60th of a second
	while(true) {
		//current time
		const auto start = std::chrono::steady_clock::now();
		if (glfwWindowShouldClose(graphics.window)) {
			break;
		}
		scripts.update();
		input.update();
		callback();
		physics.update();
		graphics.draw();
		std::this_thread::sleep_for(start + step - std::chrono::steady_clock::now());
	}
}