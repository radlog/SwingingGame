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
	target.inflict(dmg); // inflict damage to target
}

