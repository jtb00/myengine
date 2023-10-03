#include <iostream>
#include <thread>
#include <chrono>

#include "Engine.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

//Creates and initializes Engine components
Engine::Engine(): graphics(), input(), resources(), ecs() {}

//Signals Engine components to start, returns false if a component fails
bool Engine::start() {
	if(!graphics.start()) {
		return false;
	}
	graphics.loadImg("square", "C:/Users/Jack/Projects/CS 425/myengine/assets/Untitled.png");
	if(!input.start()) {
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
		input.update();
		//callback();
		Sprite s = { "square", 0, 0, 50.0, 1.0 };
		const std::vector<Sprite> sprites = { s };
		graphics.draw(sprites);
		std::this_thread::sleep_for(start + step - std::chrono::steady_clock::now());
	}
}