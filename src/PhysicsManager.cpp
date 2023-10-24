#include "Engine.h"
#include "Types.h"
#include "ECS.h"
#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() {}

bool PhysicsManager::start() {
	globalEngine.ecs.ForEach<Physics>([&](EntityID e) {
		Physics& p = globalEngine.ecs.Get<Physics>(e);
		p.a.x = 0;
		p.a.y = p.g;
		p.v.x = 0;
		p.v.y = 0;
		p.lastUpdate = std::chrono::steady_clock::now();
		});
	return true;
}

void PhysicsManager::update() {
	globalEngine.ecs.ForEach<Sprite, Physics>([&](EntityID e) {
		Sprite& s = globalEngine.ecs.Get<Sprite>(e);
		Physics& phy = globalEngine.ecs.Get<Physics>(e);
		std::chrono::duration<float> delta = std::chrono::steady_clock::now() - phy.lastUpdate;
		auto dt = delta.count();
		phy.v.x = phy.v.x + phy.a.x * dt;
		phy.v.y = phy.v.y + phy.a.y * dt;
		s.xPos = s.xPos + phy.v.x * dt;
		s.yPos = s.yPos + phy.v.y * dt;
		if (s.yPos <= -80) {
			s.yPos = -80;
			phy.v.y = 0;
		}
		phy.lastUpdate = std::chrono::steady_clock::now();
		//printf("yPos %d\n", s.yPos);
		});
}