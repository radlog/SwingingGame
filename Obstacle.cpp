#include "Obstacle.h"



Obstacle::Obstacle()
{
}


Obstacle::~Obstacle()
{
}

Obstacle::Obstacle(OBSTACLE_TYPE obstacle_type)
{
}

void Obstacle::explode(Character target, float dmg) const
{
	target.inflict(dmg);
}

void Obstacle::push_back(Character target, XMVECTOR force) const
{
	target.transform.apply_force(force);
}
