#include <iostream>
#include <thread>
#include <chrono>

//#include "Types.h"
#include "Engine.h"

using namespace illengine;

illengine::Engine::Engine() {
	GraphicsManager graphics;
}

void illengine::Engine::start() {
	graphics.start();
}

void illengine::Engine::shutdown() {
	graphics.shutdown();
}

void illengine::Engine::gameLoop() {
	const auto step = std::chrono::duration<real>(1.0/60.0);
	while(true) {
		const auto start = std::chrono::steady_clock::now();
		graphics.draw();
		std::this_thread::sleep_for(start + step - std::chrono::steady_clock::now());
	}
}