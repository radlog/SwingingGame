#pragma once
#include "Character.h"
class Obstacle
{

	enum OBSTACLE_TYPE {
		STATIC,
		MOVING		
	};

public:
	Obstacle();
	~Obstacle();

	explicit Obstacle(OBSTACLE_TYPE obstacle_type); // initialize obstacle

	void explode(Character target, float dmg) const; // explode when target collided
};

