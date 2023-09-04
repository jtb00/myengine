#include "Engine.h"
#include "GraphicsManager.h"
#include "InputManager.h"

InputManager::InputManager() {
	engine = &globalEngine;
	graphics = &(engine->graphics);
}

bool InputManager::start() {
	return true;
}

void InputManager::shutdown() {
	
}

void InputManager::update() {
	glfwPollEvents();
}

bool InputManager::isKeyPressed(Key key) {
	return (glfwGetKey(graphics->window, static_cast<int>(key)) == 1) ? true : false;
}