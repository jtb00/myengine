#include "Engine.h"
#include "Types.h"
#include "ECS.h"
#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() {}

bool PhysicsManager::start() {
	globalEngine.ecs.ForEach<Physics>([&](EntityID e) {
		Physics& p = globalEngine.ecs.Get<Physics>(e);
		p.lastUpdate = std::chrono::steady_clock::now();
		});
	return true;
}

void PhysicsManager::update() {
	globalEngine.ecs.ForEach<Physics>([&](EntityID e) {
		Physics& p = globalEngine.ecs.Get<Physics>(e);
		});
}