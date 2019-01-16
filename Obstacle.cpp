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

void Obstacle::explode(Character target, const float dmg) const
{
	target.inflict(dmg);
}

void Obstacle::push_back(Character target, const XMVECTOR force) const
{	
	target.get_transform()->apply_force(force);
}
