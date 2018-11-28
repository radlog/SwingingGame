#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
	~Player();

	Player(std::string name);

	void update_input();

	void update();

};

