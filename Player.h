#pragma once
#include "Input.h";
#include "Character.h"
class Input;
class Player : public Character
{
public:
	Player();
	~Player();

	void update_input();

private:
	Input* input;
};

