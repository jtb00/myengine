#pragma once

//manages simulated physics
class PhysicsManager
{
public:
	PhysicsManager();
	bool start();
	void update();
	bool checkCollision(const BoundingBox& a, const BoundingBox& b);
	void onCollision(EntityID a, EntityID b);
};


