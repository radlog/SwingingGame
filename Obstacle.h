#pragma once
#include "Character.h"
class Obstacle
{

	enum Obstacle_Type {
		Static,
		Moving		
	};

public:
	Obstacle();
	~Obstacle();

	Obstacle(Obstacle_Type obstacle_type);

	void explode();
	void push_back(Character target);
};

