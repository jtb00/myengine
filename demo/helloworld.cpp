#include <iostream>

#include "spdlog/spdlog.h"

#include "../src/Engine.h"
#include "../src/InputManager.h"
#include "../src/GraphicsManager.h"

void temp() {}

//start globalEngine and run until gameLoop exits
int main( int argc, const char* argv[] ) {
	std::string setup = "src/setup.lua";
	if(!globalEngine.start(setup)) {
		return -1;
	}	
	globalEngine.scripts.LoadScript("loopScript", "src/loop.lua");
	EntityID loopScript = globalEngine.ecs.Create();
	globalEngine.ecs.Get<Script>(loopScript).name = "loopScript";
    globalEngine.gameLoop(&temp);
	globalEngine.shutdown();
    return 0;
}