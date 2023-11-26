#include "Engine.h"
#include "Types.h"
#include "ECS.h"
#include "PhysicsManager.h"
#include "SoundManager.h"

PhysicsManager::PhysicsManager() {}

bool PhysicsManager::start()
{
	globalEngine.ecs.ForEach<Physics>([&](EntityID e)
									  {
		Physics& p = globalEngine.ecs.Get<Physics>(e);
		p.a.x = 0;
		p.a.y = p.g;
		p.v.x = 0;
		p.v.y = 0;
		p.lastUpdate = std::chrono::steady_clock::now(); });
	return true;
}

void PhysicsManager::update()
{
	globalEngine.ecs.ForEach<Sprite, Physics, BoundingBox>([&](EntityID e)
	{
		Sprite &s = globalEngine.ecs.Get<Sprite>(e);
		Physics &phy = globalEngine.ecs.Get<Physics>(e);
		BoundingBox& b = globalEngine.ecs.Get<BoundingBox>(e);
		std::chrono::duration<float> delta = std::chrono::steady_clock::now() - phy.lastUpdate;
		auto dt = delta.count();
		phy.a.y = phy.g;
		phy.v.x = phy.v.x + phy.a.x * dt;
		phy.v.y = phy.v.y + phy.a.y * dt;
		s.xPos = s.xPos + phy.v.x * dt;
		s.yPos = s.yPos + phy.v.y * dt;
		b.x = s.xPos;
		b.y = s.yPos;
		phy.lastUpdate = std::chrono::steady_clock::now();
		});
	// Check for collisions
	globalEngine.ecs.ForEach<BoundingBox>([&](EntityID a)
										  {
        BoundingBox& boxA = globalEngine.ecs.Get<BoundingBox>(a);

        globalEngine.ecs.ForEach<BoundingBox>([&](EntityID b) {
            if (a != b) {
                BoundingBox& boxB = globalEngine.ecs.Get<BoundingBox>(b);
                if (checkCollision(boxA, boxB)) {
                    onCollision(a, b);
                }
            }
        }); });
}

bool PhysicsManager::checkCollision(const BoundingBox &a, const BoundingBox &b)
{
	return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
		   (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void PhysicsManager::onCollision(EntityID a, EntityID b)
{
	printf("Collision between %d and %d\n", a, b);
	Sprite& spriteA = globalEngine.ecs.Get<Sprite>(a);
	Sprite& spriteB = globalEngine.ecs.Get<Sprite>(b);
	if (spriteA.name == "player" || spriteB.name == "player") {
		globalEngine.sound.playSound("boing.wav");
		// addition logic to update health bla bla bla
	}

}