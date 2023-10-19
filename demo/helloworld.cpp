#include <iostream>

#include "spdlog/spdlog.h"

#include "../src/Engine.h"
#include "../src/InputManager.h"
#include "../src/GraphicsManager.h"

void keyPressed() {
	Key left = Key::KEY_LEFT;
	Key right = Key::KEY_RIGHT;
	if(globalEngine.input.isKeyPressed(right)) {
		globalEngine.ecs.ForEach<Sprite>([&](EntityID entity) {
			globalEngine.ecs.Get<Sprite>(entity).xPos += 10;
			//spdlog::info("Key pressed!");
			});
	}
	else if (globalEngine.input.isKeyPressed(left)) {
		globalEngine.ecs.ForEach<Sprite>([&](EntityID entity) {
			globalEngine.ecs.Get<Sprite>(entity).xPos -= 10;
			});
	}
}

//start globalEngine and run until gameLoop exits
int main( int argc, const char* argv[] ) {
    if(!globalEngine.start()) {
		return -1;
	}
	auto setup = globalEngine.scripts.lua.load_file("C:/Users/gamec/Projects/CS 425/myengine/src/setup.lua");
	setup();
	/*
	EntityID sprite = globalEngine.ecs.Create();
	globalEngine.ecs.Get<Sprite>(sprite).name = "square";
	globalEngine.ecs.Get<Sprite>(sprite).xPos = 0;
	globalEngine.ecs.Get<Sprite>(sprite).yPos = 0;
	globalEngine.ecs.Get<Sprite>(sprite).scale = 50.0;
	globalEngine.ecs.Get<Sprite>(sprite).z = 1.0;
	globalEngine.scripts.LoadScript("script", "C:/Users/gamec/Projects/CS 425/myengine/src/script.lua");
	EntityID script = globalEngine.ecs.Create();
	globalEngine.ecs.Get<Script>(script).name = "script";
	*/
    globalEngine.gameLoop(&keyPressed);
	globalEngine.shutdown();
    return 0;
}