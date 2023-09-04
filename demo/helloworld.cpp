#include <iostream>

#include "spdlog/spdlog.h"

#include "../src/Engine.h"
#include "../src/InputManager.h"

void keyPressed() {
	Key a = Key::KEY_A;
	if(globalEngine.input.isKeyPressed(a)) {
		spdlog::info("A key pressed!");
	}
}

//start globalEngine and run until gameLoop exits
int main( int argc, const char* argv[] ) {
    if(!globalEngine.start()) {
		return -1;
	}
    globalEngine.gameLoop(&keyPressed);
	globalEngine.shutdown();
    return 0;
}