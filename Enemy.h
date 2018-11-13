#pragma once
#include "Character.h"

class Enemy
{

	enum Enemy_Type {
		Grounder,
		Swinger
	};

public:
	Enemy();
	~Enemy();

	void attack(Character target);
};

