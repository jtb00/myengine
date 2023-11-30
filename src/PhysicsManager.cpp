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
		phy.lastUpdate = std::chrono::steady_clock::now();
		});
	globalEngine.ecs.ForEach<Sprite, BoundingBox>([&](EntityID e)
		{
			Sprite& s = globalEngine.ecs.Get<Sprite>(e);
			BoundingBox& b = globalEngine.ecs.Get<BoundingBox>(e);
			b.x = s.xPos;
			b.y = s.yPos;
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
	Sprite& spriteA = globalEngine.ecs.Get<Sprite>(a);
	Sprite& spriteB = globalEngine.ecs.Get<Sprite>(b);
	if (spriteA.name == "bunny" && spriteB.name == "spike1" ||
		spriteA.name == "bunny" && spriteB.name == "spike2" || 
		spriteA.name == "bunny" && spriteB.name == "spikeMan" || 
		spriteA.name == "bunny" && spriteB.name == "flyMan" ) {
		//printf("Collision between %s and %s\n", spriteA.name, spriteB.name);
		spriteA.scale = 0;
		Sprite& gameOver = globalEngine.ecs.Get<Sprite>(a);
		globalEngine.ecs.ForEach<Sprite>([&](EntityID e) {
			if (globalEngine.ecs.Get<Sprite>(e).name == "gameOver") {
				gameOver = globalEngine.ecs.Get<Sprite>(e);
			}
		});
		gameOver.scale = 100.0;
		

	}

}

/*const float BASE_SPRITE_HEIGHT = 50.0; // Base height of a sprite before scaling

void PhysicsManager::onCollision(EntityID a, EntityID b)
{
    printf("Collision between %d and %d\n", a, b);
    Sprite& spriteA = globalEngine.ecs.Get<Sprite>(a);
    Sprite& spriteB = globalEngine.ecs.Get<Sprite>(b);
    Physics& phyA = globalEngine.ecs.Get<Physics>(a);
    Physics& phyB = globalEngine.ecs.Get<Physics>(b);

    // Calculate the height of the sprites based on their scale
    float heightA = BASE_SPRITE_HEIGHT * spriteA.scale;
    float heightB = BASE_SPRITE_HEIGHT * spriteB.scale;

    // Check for player and ground collision
    if ((spriteA.name == "player" && spriteB.name == "ground_grass") ||
        (spriteB.name == "player" && spriteA.name == "ground_grass")) {
        // Handle collision with ground
        if (spriteA.name == "player") {
            phyA.v.y = 0;
            spriteA.yPos = spriteB.yPos + heightB; // Position player on top of ground
        }
        if (spriteB.name == "player") {
            phyB.v.y = 0;
            spriteB.yPos = spriteA.yPos + heightA;
        }
        globalEngine.sound.playSound("land.wav");
    }
    else if (spriteA.name == "player" || spriteB.name == "player") {
        globalEngine.sound.playSound("boing.wav");
        // Additional logic for other collisions
    }
}
*/