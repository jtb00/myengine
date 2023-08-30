#include <iostream>
#include <thread>
#include <chrono>

//#include "Types.h"
#include "Engine.h"
#include "GraphicsManager.h"

//using namespace illengine;

Engine::Engine() {
	GraphicsManager graphics();
}

void Engine::start() {
	graphics.start();
}

void Engine::shutdown() {
	graphics.shutdown();
}

void Engine::gameLoop() {
	const auto step = std::chrono::duration<double>(1.0/60.0);
	while(true) {
		const auto start = std::chrono::steady_clock::now();
		graphics.draw();
		std::this_thread::sleep_for(start + step - std::chrono::steady_clock::now());
	}
}