#include <iostream>

#include "spdlog/spdlog.h"

#include "../src/Engine.h"
#include "../src/InputManager.h"
#include "../src/GraphicsManager.h"

void temp() {}

//start globalEngine and run until gameLoop exits
int main( int argc, const char* argv[] ) {
    if(!globalEngine.start()) {
		return -1;
	}
	
	auto setup = globalEngine.scripts.lua.load_file("C:/Users/Jack/Projects/CS 425/myengine/src/setup.lua");
	setup();
	
	globalEngine.scripts.LoadScript("loopScript", "C:/Users/Jack/Projects/CS 425/myengine/src/loop.lua");
	EntityID loopScript = globalEngine.ecs.Create();
	globalEngine.ecs.Get<Script>(loopScript).name = "loopScript";
    globalEngine.gameLoop(&temp);
	globalEngine.shutdown();
    return 0;
}