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

	explicit Obstacle(OBSTACLE_TYPE obstacle_type);

	void explode(Character target, float dmg) const;
	void push_back(Character target, XMVECTOR force) const;
};

